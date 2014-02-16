#include "Histogram.hh"
#include "Binners.hh"
#include "H5Cpp.h"

#include <stdexcept>
#include <set>
#include <algorithm>
#include <cassert> 

namespace { 
  // internal check used in setup
  void check_dimensions(const std::vector<Axis>& axes); 
  // for adding annotaton 
  void dim_atr(H5::DataSet& target, unsigned number, const Axis& dim); 
}

//______________________________________________________________________
// constructors / destructors / copy / swap

Histogram::Histogram(int n_bins, double low, double high, std::string units, 
		     unsigned flags): 
  Histogram({{"x",n_bins,low,high,units}}, flags)
{ 
  assert(m_dimsensions.size() == 1); 
}

Histogram::Histogram(const std::initializer_list<Axis>& dims, 
		     unsigned flags): 
  Histogram(std::vector<Axis>(dims), flags) 
{
}

Histogram::Histogram(const std::vector<Axis>& dims, unsigned flags) :
  m_binner(0), 
  m_dimsensions(dims), 
  m_n_nan(0), 
  m_eat_nan(flags & hist::eat_nan), 
  m_old_serialization(flags & hist::flat_attributes), 
  m_wt2(0), 
  m_wt2_ext("Wt2")
{ 
  check_dimensions(dims); 
  assert(dims.size() > 0); 
  Axes::const_reverse_iterator itr = dims.rbegin(); 
  int n_values = itr->n_bins + 2; 
  m_binner = new LinBinner(itr->name, itr->n_bins, itr->low, itr->high);
  itr++; 
  for (; itr != dims.rend(); itr++) { 
    m_binner->add_dimension(new LinBinner(itr->name, itr->n_bins, 
					 itr->low, itr->high)); 
    n_values *= (itr->n_bins + 2); 
  }
  assert(m_binner); 
  m_values = std::vector<double>(n_values, 0); 
  if (flags & hist::wt2) { 
    m_wt2 = new std::vector<double>(n_values, 0); 
  }
}

Histogram::Histogram(const Histogram& old): 
  m_binner(0), 
  m_dimsensions(old.m_dimsensions), 
  m_values(old.m_values), 
  m_chunking(old.m_chunking), 
  m_n_nan(old.m_n_nan), 
  m_eat_nan(old.m_eat_nan), 
  m_wt2(0)
{ 
  assert(old.m_binner); 
  m_binner = old.m_binner->clone(); 
  if (old.m_wt2) { 
    *m_wt2 = *old.m_wt2; 
  }
}

Histogram& Histogram::operator=(Histogram old)
{ 
  using std::swap; 
  swap(*this, old);
  return *this; 
}

Histogram::~Histogram() 
{
  delete m_binner; 
  delete m_wt2; 
}

void swap(Histogram& f, Histogram& s) 
{
  using std::swap; 
  swap(f.m_binner,      s.m_binner); 
  swap(f.m_dimsensions, s.m_dimsensions); 
  swap(f.m_values,      s.m_values); 
  swap(f.m_chunking,    s.m_chunking); 
  swap(f.m_n_nan,       s.m_n_nan); 
  swap(f.m_eat_nan,     s.m_eat_nan); 
  swap(f.m_wt2,         s.m_wt2); 
  swap(f.m_wt2_ext,     s.m_wt2_ext); 
}

//______________________________________________________________________
// fill methods 

void Histogram::fill(const std::map<std::string, double>& input, 
		     double weight) { 
  safe_fill(input, weight); 
}

void Histogram::fill(const std::vector<double>& input, 
		     double weight) { 
  safe_fill(input, weight); 
}
void Histogram::fill(const std::initializer_list<double>& input, 
		     double weight) { 
  safe_fill(std::vector<double>(input), weight); 
}

void Histogram::fill(double value, double weight) { 
  assert(m_dimsensions.size() == 1); 
  std::vector<double> v(1,value); 
  safe_fill(v, weight); 
}

//______________________________________________________________________
// file IO related 

void Histogram::set_wt_ext(const std::string& ext) { 
  if (ext.size() == 0) { 
    throw std::invalid_argument(
      "tried to use no wt2 extension: this will overwrite the normal hist");
  }
  m_wt2_ext = ext; 
}

void Histogram::write_to(H5::CommonFG& file, 
			 const std::string& name, int deflate) 
  const 
{
  write_internal(file, name, deflate, m_values); 
  if (m_wt2) { 
    write_internal(file, name + m_wt2_ext, deflate, *m_wt2); 
  }
}

// ==================== private ==========================

// ____________________________________________________________________
// attribute adding forward declarations

namespace { 

  template<typename M> 
  void write_attr(H5::DataSet&, const std::string& name, M val); 

  // vector attribute adding function
  template<typename M>
  void write_attr_vec(H5::DataSet&, const std::string& name, const M& vec); 

  // need specialization for strings...
  template<> 
  void write_attr(H5::DataSet&, const std::string& name, 
		  const std::string& val); 
  template<>
  void write_attr_vec(H5::DataSet&, const std::string& name, 
		      const std::vector<std::string>&); 

  // store attributes as arrays (indexed by axis number)
  void add_axis_attributes(H5::DataSet&, const std::vector<Axis>& axes); 

  // various overloads to use in template
  H5::PredType get_type(double val); 
  H5::PredType get_type(int val); 
  H5::PredType get_type(unsigned val); 
  H5::StrType get_type(std::string val); 
}

// ______________________________________________________________________
// various Histogram internal methods

// write method called by the public Histogram write methods
void Histogram::write_internal(
  H5::CommonFG& file, const std::string& name, int deflate, 
  const std::vector<double>& values) const
{
  if (H5Lexists(file.getLocId(), name.c_str(), H5P_DEFAULT)) { 
    throw HistogramSaveError("tried to overwrite '" + name + "'"); 
  }
  using namespace H5; 

  // define the DataSpace
  const hsize_t n_dims = m_dimsensions.size(); 
  std::vector<hsize_t> ds_dims(n_dims); 
  std::vector<hsize_t> ds_chunks(n_dims); 
  hsize_t total_entries = 1;	
  for (unsigned dim = 0; dim < n_dims; dim++) { 
    // 2 extra for overflow bins
    hsize_t bins = m_dimsensions.at(dim).n_bins + 2; 	
    ds_dims.at(dim) = bins; 
    // datasets can be "chucked", i.e. stored and retrieved as smaller 
    // pieces. Probably not needed for HEP histograms. 
    ds_chunks.at(dim) = get_chunk_size(bins); // for now just returns value
    total_entries *= bins; 
  }
  H5::DataSpace data_space(n_dims, ds_dims.data()); 

  // write the file
  H5::DSetCreatPropList params; 
  params.setChunk(n_dims, ds_chunks.data());
  params.setDeflate(deflate); 
  H5::DataSet dataset = file.createDataSet(
    name, PredType::NATIVE_DOUBLE, data_space, params); 
  assert(values.size() == total_entries); 
    dataset.write(values.data(), PredType::NATIVE_DOUBLE); 
  if (m_old_serialization) { 
    for (unsigned dim = 0; dim < n_dims; dim++) { 
      const Axis& dim_info = m_dimsensions.at(dim); 
      dim_atr(dataset, dim, dim_info); 
    }
  } else { 
    add_axis_attributes(dataset, m_dimsensions); 
  }
  write_attr(dataset, "nan", m_n_nan); 
}

// Internal wrapper on fill method. Takes care of NaN inputs, and 
// filling the weight**2 hist (if it exists)
template<typename T> 
void Histogram::safe_fill(T input, double weight) {
  try { 
    int bin = m_binner->get_bin(input); 
    m_values.at(bin) += weight; 
    if (m_wt2) { 
      m_wt2->at(bin) += weight*weight; 
    }
  }
  catch (std::range_error& r) { 
    if (m_eat_nan) { 
      m_n_nan++; 
    }
    else { 
      throw; 
    }
  }
}

// internal chunking function (may do more elaborate chunking someday)
int Histogram::get_chunk_size(int input) const { 
  return input; 
}

namespace { 
  // throw exceptions if the constructor doesn't make sense. 
  void check_dimensions(const std::vector<Axis>& axes) { 
    if (axes.size() == 0) {
      throw std::invalid_argument(
	"Histogram: tried to initialize with no dimensions");
    }
    std::set<std::string> names; 
    for (size_t pos = 0; pos < axes.size(); pos++) { 
      auto& axis = axes.at(pos); 
      auto& name = axis.name; 
      if (name.size() == 0) { 
	throw std::invalid_argument(
	  "Histogram: unnamed axis " + std::to_string(pos)); 
      }
      if (names.count(name)) { 
	throw std::invalid_argument(
	  "Histogram: axis name " + name + " was used twice"); 
      } else { 
	names.insert(name); 
      }
      if (axis.low >= axis.high) { 
	throw std::invalid_argument(
	  "Histogram: axis " + name + " has bounds with high <= low"); 
      }
      if (axis.n_bins < 1) { 
	throw std::invalid_argument(
	  "Histogram: axis " + name + " has < 1 bin"); 
      }
    }
  }

  //________________________________________________________________________
  // implementation of the attribute adder functions

  // function to add axis attributes via the "flat" method (adds a magic '_' 
  // between the name of the axis and the property). 
  void dim_atr(H5::DataSet& target, unsigned number, const Axis& dim)
  {
    using namespace H5;

    write_attr(target, dim.name + "_axis", number); 
    write_attr(target, dim.name + "_bins", dim.n_bins); 
    write_attr(target, dim.name + "_max", dim.high); 
    write_attr(target, dim.name + "_min", dim.low); 
    write_attr(target, dim.name + "_units", dim.units); 
  }

  // much less ugly function to add axis attributes as arrays. 
  void add_axis_attributes(H5::DataSet& targ, const std::vector<Axis>& axes)
  { 
    std::vector<std::string> names; 
    std::vector<int> bins; 
    std::vector<double> mins; 
    std::vector<double> maxs; 
    std::vector<std::string> units; 
    for (auto axis: axes) { 
      names.push_back(axis.name); 
      bins.push_back(axis.n_bins); 
      mins.push_back(axis.low); 
      maxs.push_back(axis.high); 
      units.push_back(axis.units); 
    }
    write_attr_vec(targ, "names", names); 
    write_attr_vec(targ, "n_bins", bins); 
    write_attr_vec(targ, "min", mins); 
    write_attr_vec(targ, "max", maxs); 
    write_attr_vec(targ, "units", units); 
  }

  // templates to write attributes. 
  template<typename M> 
  void write_attr(H5::DataSet& loc, const std::string& name, M value) { 
    auto type = get_type(value); 
    loc.createAttribute(name, type, H5S_SCALAR).write(type, &value); 
  }
  template<typename M> 
  void write_attr_vec(H5::DataSet& loc, const std::string& name, 
		      const M& vec) { 
    auto type = get_type(vec.front()); 
    hsize_t size = vec.size(); 
    H5::DataSpace data_space(1, {&size}); 
    loc.createAttribute(name, type, data_space).write(type, vec.data()); 
  }

  // overloads for strings
  template<> 
  void write_attr(H5::DataSet& loc, const std::string& name, 
		  const std::string& value) { 
    auto type = get_type(value); 
    loc.createAttribute(name, type, H5S_SCALAR).write(type, value.data()); 
  }
  template<> 
  void write_attr_vec(H5::DataSet& loc, const std::string& name, 
		      const std::vector<std::string>& vec) { 
    auto type = get_type(vec.front()); 
    hsize_t size = vec.size(); 
    H5::DataSpace data_space(1, {&size}); 

    // HDF5 wants a char** for the strings. To be keep this memory safe, we
    // store the char* in a vector and access the char** with vec.data().
    std::vector<const char *> string_pointers;
    for (auto str: vec) { 
      string_pointers.push_back(str.data());
    }
    loc.createAttribute(name, type, data_space).write(
      type, string_pointers.data()); 
  }

  // called by the attribute writers to get the correct datatype. 
  H5::PredType get_type(int) { 
    return H5::PredType::NATIVE_INT; 
  }
  H5::PredType get_type(unsigned) { 
    return H5::PredType::NATIVE_UINT; 
  }
  H5::PredType get_type(double) { 
    return H5::PredType::NATIVE_DOUBLE; 
  }
  H5::StrType get_type(std::string) { 
    auto type = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
    type.setCset(H5T_CSET_UTF8); 
    return type; 
  }

}

//______________________________________________________________________
// exception definitions 

HistogramSaveError::HistogramSaveError(const std::string& what): 
  std::runtime_error(what) 
{}

