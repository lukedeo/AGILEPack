#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// Lightweight histogram class for filling and saving to HDF5.
// 
// Histograms can be arbitrary dimensions (up to 32 can be saved by 
// HDF5). The result is saved as a HDF5 DataSet, with attributes 
// indicating the binning. Attributes are saved as an array, with the 
// Nth entry in the array corresponding to the Nth axis. The following
// attributes are saved: 
// 
// - `names`: axis names, each axis must have a unique name
// - `n_bins`: number of bins in this axis (not including overflow)
// - `min`: bottom range of the axis
// - `max`: upper range of the axis
// - `units`: (optional) string indicating units
// 
// If the hist::eat_nan flag is used, Histogram will also record the number
// of NaN inputs the binner encounters in the attribute 'nan'. 
// There is only one NaN counter for all axes. 


// Axis also includes the initialization flags.
#include "Axis.hh"

#include <vector>
#include <string> 
#include <map>
#include <initializer_list>
#include <stdexcept>

namespace H5 { 
  class CommonFG; 
  class DataSet; 
} 
class IBinner; 


class Histogram
{
public: 

  // -------------------- constructors ----------------------------

  // convenience 1d constructor (axis is called "x")
  Histogram(int n_bins, double low, double high, std::string units = "", 
	    unsigned flags = 0); 

  // Initializer list and vector constructor work the same way. 
  // In either case, the Axis can be constructed with an initializer list
  // so the form is 
  // Histogram( { {"name", n_bins, lower_edge, upper_edge[, units]}, ...})
  Histogram(const std::initializer_list<Axis>&, unsigned flags = 0); 
  Histogram(const std::vector<Axis>&, unsigned flags = 0); 

  // Histograms are safe to copy and assign. Copies are deep. 
  Histogram(const Histogram&); 
  Histogram& operator=(Histogram); 

  // destructor and swap operator. 
  ~Histogram(); 
  friend void swap(Histogram&, Histogram&); 

  // -------------------- Fill methods ---------------------------

  // map-based: probably slower (not sure it matters...)
  void fill(const std::map<std::string, double>&, double weight = 1); 

  // vector-based: first entry in is assumed to be the first axis
  void fill(const std::vector<double>&, double weight = 1); 

  // initializer based works like vector
  void fill(const std::initializer_list<double>&, double weight = 1); 

  // convenience method for 1d hists. 
  void fill(double value, double weight = 1); 

  // ---------------------- save methods ---------------------------

  // An extension is appended to the weight^2 histogram if the hist::wt2 
  // flag is used. The default extension is "Wt2". 
  void set_wt_ext(const std::string& ext); 

  // write method. Throws a HistogramSaveError if the DataSet exists. 
  void write_to(H5::CommonFG& file, 
		const std::string& name, int deflate = 7) const; 

  // ===================== internal things =========================
private: 
  typedef std::vector<Axis> Axes;
  void write_internal(
    H5::CommonFG& file, const std::string& name, int deflate, 
    const std::vector<double>& values) const; 
  template<typename T> void safe_fill(T, double); 
  int get_chunk_size(int) const; 
  IBinner* m_binner; 
  std::vector<Axis> m_dimsensions; 
  std::vector<double> m_values; 
  std::vector<int> m_chunking; 
  int m_n_nan; 
  bool m_eat_nan; 
  bool m_old_serialization; 
  std::vector<double>* m_wt2; 
  std::string m_wt2_ext; 
}; 

// exceptions
class HistogramSaveError: public std::runtime_error {
public: 
  HistogramSaveError(const std::string&); 
}; 

#endif //HISTOGRAM_H
