//-----------------------------------------------------------------------------
//  vec_tree_reader.cxx:
//  Header for dumping ROOT data into a dataframe or some std::vectors
//  Authors: Luke de Oliveira (luke.deoliveira@yale.edu)  -- primary development
//           Michela Paganini (michela.paganini@yale.edu) -- small changes
//-----------------------------------------------------------------------------
#include "include/vec_tree_reader.hh"

namespace agile
{
namespace root
{

//----------------------------------------------------------------------------
vec_tree_reader::vec_tree_reader(std::string filename, std::string tree_name)
: m_smart_chain(0), m_size(0), m_tree_name(tree_name), m_binned_present(false)
{
  /* for one file */
    if(tree_name != "")
    {

        m_smart_chain = new smart_chain(tree_name);
        if (filename != "")
        {
            m_smart_chain->add(filename);
            m_size = m_smart_chain->GetEntries(); 
        }
    }
    else if (filename != "")
    {
        throw std::runtime_error("you must provide a \
            tree name to load files.");
    }   
} // ✓ 
//----------------------------------------------------------------------------
vec_tree_reader::vec_tree_reader(const std::vector<std::string>& files, 
    std::string tree_name)
: m_smart_chain(0), m_size(0), m_tree_name(tree_name), m_binned_present(false)
{
  /* for multiple files */
    if (tree_name == "")
    {
        throw std::runtime_error("you must provide a \
            tree name to load files.");
    }
    m_smart_chain = new smart_chain(tree_name);
    for (auto &file : files)
    {
        m_smart_chain->add(file);
    }
    m_size = m_smart_chain->GetEntries();
} // ✓
//----------------------------------------------------------------------------
vec_tree_reader::~vec_tree_reader()
{
    delete m_smart_chain;
} // ✓
//----------------------------------------------------------------------------
void vec_tree_reader::add_file(std::string filename, std::string tree_name)
{
    if (!m_smart_chain)
    {
        m_smart_chain = new smart_chain(tree_name);
    }
    m_smart_chain->add(filename);
    m_size = m_smart_chain->GetEntries(); 
} // ✓
//----------------------------------------------------------------------------
void vec_tree_reader::set_branch(std::string branch_name, numeric_type type)
{
    if (branch_name == "")
    {
        throw std::runtime_error("cant have a NULL-named branch.");
    }
    if (!m_smart_chain)
    {
        throw std::runtime_error("no files added to smart_chain.");
    }
    storage.emplace_back(new number_container);

    var_traits new_trait(storage.size() - 1, type);
    traits[branch_name] = new_trait;

    feature_names.push_back(branch_name);

    switch(type)
    {
        case single_precision:
        {
            m_smart_chain->set_branch(branch_name, 
                &storage.back()->set_address<float>());

            variable_type_map[branch_name] = "float";
            break;
        }
        case double_precision:
        {
            m_smart_chain->set_branch(branch_name, 
                &storage.back()->set_address<double>());

            variable_type_map[branch_name] = "double";
            break;
        }
        case integer:
        {
            m_smart_chain->set_branch(branch_name, 
                &storage.back()->set_address<int>());

            variable_type_map[branch_name] = "int";
            break;
        }
    }
} // ✓
//----------------------------------------------------------------------------
bool vec_tree_reader::entry_in_range( double branch_at_jetn, std::string name) 
{ /* Info from the config file */
    if ((!m_binned_present) && (!m_constraint_present))
    {
        return true;
    }
    //    bool ok = true;
    /*    for (auto &name : binned_names)
    {
        ok = ok && m_binned_vars[name].in_range(
            storage.at(traits[name].pos)->get_value<double>());
        if(!ok) return false;
	}*/
    //    for (auto &name : constraint_names)
    //{ /// --- modified
    //for (auto &ent : storage.at(traits[name].pos)->get_value<std::vector<double> >()) // jet loop
    //{
    bool ok = m_constraint_vars[name].in_range(branch_at_jetn); // OK defined for each jet

    return ok;  // OK defined for each jet 
} 
 
//----------------------------------------------------------------------------
/*void vec_tree_reader::create_binning(const std::string &branch_name, 
    const std::initializer_list<double> &il, bool absolute)
{
    binned_names.push_back(branch_name);

    m_binned_vars[branch_name].set_name(branch_name)
                              .set_bins(il)
                              .set_abs(absolute);

    std::vector<double> v(il);
    if (absolute)
    {
        m_binning_strategy["abs(" + branch_name + ")"] = v;
    }
    else
    {
        m_binning_strategy[branch_name] = v;
    }
    m_binned_present = true;
} //  modify where this is used because we can only bin when v is not a vector

//----------------------------------------------------------------------------
void vec_tree_reader::create_binning(const std::string &branch_name, 
    const std::vector<double> &v, bool absolute)
{
    binned_names.push_back(branch_name);

    m_binned_vars[branch_name].set_name(branch_name)
                              .set_bins(v)
                              .set_abs(absolute);
    if (absolute)
    {
        m_binning_strategy["abs(" + branch_name + ")"] = v;
    }
    else
    {
        m_binning_strategy[branch_name] = v;
    }
    m_binned_present = true;
} // same as above
*/
//----------------------------------------------------------------------------
void vec_tree_reader::create_constraint(const std::string &branch_name, 
    const std::initializer_list<double> &il, bool absolute)
{
    constraint_names.push_back(branch_name);

    m_constraint_vars[branch_name].set_name(branch_name)
                              .set_bins(il)
                              .set_abs(absolute);

    std::vector<double> v(il); // check what initializer_list is 
    if (absolute)
    {
        m_constraint_strategy["abs(" + branch_name + ")"] = v;
    }
    else
    {
        m_constraint_strategy[branch_name] = v;
    }
    m_constraint_present = true;
} // modify where this is used because we can only apply constraints when v is not a vector

//----------------------------------------------------------------------------
void vec_tree_reader::create_constraint(const std::string &branch_name, 
    const std::vector<double> &v, bool absolute)
{
    constraint_names.push_back(branch_name);

    m_constraint_vars[branch_name].set_name(branch_name)
                              .set_bins(v)
                              .set_abs(absolute);
    if (absolute)
    {
        m_constraint_strategy["abs(" + branch_name + ")"] = v;
    }
    else
    {
      m_constraint_strategy[branch_name] = v; // v, I believe, is a vector like [500,750] which contains the constraints
    }
    m_constraint_present = true;
} // same as above
//----------------------------------------------------------------------------
void vec_tree_reader::set_branches(const std::string &yamlfile)
{
    YAML::Node tmp = YAML::LoadFile(yamlfile);
    try
    {
        YAML::Node config = tmp["branches"];
        std::map<std::string, std::string> vars = 
        config.as<std::map<std::string, std::string>>();

        for (auto &entry : vars)
        {
            auto type = entry.second;
            if (type == "double")
            {
                set_branch(entry.first, agile::root::double_precision);
            }
            else if (type == "float")
            {
                set_branch(entry.first, agile::root::single_precision);
            }
            else if (type == "int")
            {
                set_branch(entry.first, agile::root::integer);
            }
            else
            {
                throw std::domain_error(
                    "type " + type + " for branch " + entry.first + ".");
            }
        }
    }
    catch(YAML::BadConversion &e)
    {
        throw std::runtime_error(
            "configuration files must have a map entitled 'branches'");
    }
    /*    try
    {   
        YAML::Node binning = tmp["binning"]; 

        auto bins = binning.as<std::map<std::string, std::vector<double>>>();

        for (auto &entry : bins)
        {
            auto expression(agile::no_spaces(entry.first));

            if (expression.find("abs(") == std::string::npos)
            {
                create_binning(entry.first, entry.second);
            }
            else
            {
                auto close_paren = expression.find_first_of(")");
                auto open_paren = expression.find_first_of("(");
                std::string arg;
                if (close_paren == std::string::npos)
                {
                    arg = expression.substr(open_paren + 1);
                    throw std::invalid_argument(
                        "missing close parentheses for argument " + arg);
                }
                arg = expression.substr(
                    open_paren + 1, close_paren - open_paren - 1);

                create_binning(arg, entry.second, true);
            }      
        }
    }
    catch(YAML::BadConversion &e){}
    */
    try
    {   
        YAML::Node binning = tmp["constraints"];

        auto bins = binning.as<std::map<std::string, std::vector<double>>>();

        for (auto &entry : bins)
        {
            auto expression(agile::no_spaces(entry.first));

            if (expression.find("abs(") == std::string::npos) // if abs is not found
            {
	      create_constraint(entry.first, entry.second); // absolute = false, by default
            }
            else
            {
                auto close_paren = expression.find_first_of(")");
                auto open_paren = expression.find_first_of("(");
                std::string arg;
                if (close_paren == std::string::npos)
                {
                    arg = expression.substr(open_paren + 1);
                    throw std::invalid_argument(
                        "missing close parentheses for constraint argument " + arg);
                }
                arg = expression.substr(
                    open_paren + 1, close_paren - open_paren - 1);

                create_constraint(arg, entry.second, true);
            }      
        }
    }
    catch(YAML::BadConversion &e){}

    
}
//----------------------------------------------------------------------------
std::map<std::string, std::string> vec_tree_reader::get_var_types()
{
    return variable_type_map;
}
//----------------------------------------------------------------------------
/*std::map<std::string, std::vector<double>> vec_tree_reader::get_binning()
{
    if(!m_binned_present)
    {
        throw std::logic_error("bins not set!");
    }
    return m_binning_strategy;
}*/
//----------------------------------------------------------------------------
std::map<std::string, std::vector<double>> vec_tree_reader::get_constraints()
{
    if(!m_constraint_present)
    {
        throw std::logic_error("Constraints not set!");
    }
    return m_constraint_strategy;
}
//----------------------------------------------------------------------------
agile::dataframe vec_tree_reader::get_dataframe(int entries, int start, 
    bool verbose)
{
  if ((entries > (int)m_size) || ((start + entries) > (int)m_size)) // ok
    {
      throw dimension_error(
			    "tried to access element in TTree beyond range.");
    }

    if (verbose)
    {
        std::cout << "Pulling agile::dataframe from tree_reader..." << std::endl;
    }
    entries = (entries < 0) ? m_size : entries;
    start = (start < 0) ? 0 : start;
    auto stop = start + entries;

    int curr_entry = 0;
    double pct;
    agile::dataframe D;

    auto all_names = feature_names;
    if (m_binned_present)
    {
        for (auto &entry : binned_names)
        {
            all_names.push_back("categ_" + entry);
        }
    }
    D.set_column_names(all_names);
    for (curr_entry = start; curr_entry < stop; ++curr_entry) // all these need to refer to events, not jets
    {
        if (verbose)
        {
            pct = (double)(curr_entry - start) / (double)(entries);
            agile::progress_bar(pct * 100);
        }

        m_smart_chain->GetEntry(curr_entry); // now gets an event
	
	for (int jetn = 0; jetn < storage.at(traits[all_names[0]].pos)->get_value<std::vector<double> >().size(); ++jetn) // jet loop
	  {
	    std::vector<double> temp_v; // [pt1, eta1, phi1, ...]
	    for (auto &name : feature_names) // loops thru vectors of branches
	      {
		double branch_at_jetn = storage.at(traits[name].pos)->get_value<std::vector<double> >().at(jetn);
		
		if ( !entry_in_range( branch_at_jetn, name) )
		  {
		    break;
		  }
		else temp_v.push_back(branch_at_jetn);
	      }

	    if (temp_v.size() == feature_names.size()) 
	      {
		D.push_back(std::move(temp_v));
	      }
	  }
    
        
    }
    return std::move(D); // contains jet vectors like [pt1, eta1, phi1, ...]
}

//-----------------------------------------------------------------------------
//  Element Access
//-----------------------------------------------------------------------------
std::vector<std::vector<double> > vec_tree_reader::at(const unsigned int &idx) // returns a vector of doubles now 
{
  m_smart_chain->GetEntry(idx); // would be an event now
  std::vector<std::vector<double> >v; // vector of e.g. 'pt' for all jets in an event
    for (auto &name : feature_names)
    {
      v.push_back(storage.at(traits[name].pos)->get_value<std::vector<double> >()); // now a vector of doubles
    }
    /*
    // this below is probably to be modified: 
    if (m_binned_present)
    {
        for (auto &name : binned_names)
        {
	  //            int bin = m_binned_vars[name].get_bin(storage.at(traits[name].pos)->);
            v.push_back((double)bin); // gives you the number of the bin instead of the value for that variable??
        }
    }
    */
    return std::move(v);
}
//----------------------------------------------------------------------------
std::vector<std::vector<double> > vec_tree_reader::operator[](const unsigned int &idx)
{
  m_smart_chain->GetEntry(idx); // an event
  return std::move(at(idx)); 
}
//----------------------------------------------------------------------------
std::vector<std::vector<double> > vec_tree_reader::operator()(const unsigned int &idx)
{
  m_smart_chain->GetEntry(idx); // an event
  return std::move(at(idx));
}
//----------------------------------------------------------------------------
std::vector<double> vec_tree_reader::operator()(const unsigned int &idx, std::string col_name)
{
    m_smart_chain->GetEntry(idx);

    //try
    //{   
    return storage.at(traits.at(col_name).pos)->get_value<std::vector<double> >();
      /*}
    catch(std::out_of_range &e)
    {   
        auto categ_split = col_name.find_first_of('_');
        auto arg = col_name.substr(categ_split + 1);
        return (double) m_binned_vars.at(arg).get_bin(
		        storage.at(traits.at(arg).pos)->get_value<double>()); // to be modified
    }
      */
}
//---------------------------------------------------------------------------- 
std::map<std::string, std::vector<double> > vec_tree_reader::operator()(const unsigned int &idx, 
    const std::vector<std::string> &names)
{
    m_smart_chain->GetEntry(idx);
    std::map<std::string, std::vector<double> >map;
    for (auto &name : names)
    {
      //try
      // {
      map[name] = storage.at(traits.at(name).pos)->get_value<std::vector<double> >();
      /*  }
        catch(std::out_of_range &e)
        {
            auto categ_split = name.find_first_of('_');
            auto arg = name.substr(categ_split + 1);
            int bin = m_binned_vars.at(arg).get_bin(
      		      storage.at(traits.at(arg).pos)->get_value<double>()); // to be modified

            map[name] = (double)(bin);
        }
      */
    }
    return std::move(map);

}
//----------------------------------------------------------------------------
std::map<std::string, std::vector<double> > vec_tree_reader::at(const unsigned int &idx, 
    const std::vector<std::string> &names)
{
    m_smart_chain->GetEntry(idx);
    std::map<std::string, std::vector<double> > map;
    for (auto &name : names)
    {
      //try
      //  {
      map[name] = storage.at(traits.at(name).pos)->get_value<std::vector<double> >();
      /*  }
	  catch(std::out_of_range &e)
	  {
	  auto categ_split = name.find_first_of('_');
	  auto arg = name.substr(categ_split + 1);
            int bin = m_binned_vars.at(arg).get_bin(
                      storage.at(traits.at(arg).pos)->get_value<double>()); // to be modified
            map[name] = (double)(bin);
        }
	  */
    }
    return std::move(map);

}

//-----------------------------------------------------------------------------
//  Information
//-----------------------------------------------------------------------------

std::size_t vec_tree_reader::size()
{
    return m_size;
}
//----------------------------------------------------------------------------
std::vector<std::string> vec_tree_reader::get_ordered_branch_names()
{
    auto all_names = feature_names;
    if (m_binned_present)
    {
        for (auto &entry : binned_names)
        {
            all_names.push_back("categ_" + entry);
        }
    }
    return all_names;
}
//----------------------------------------------------------------------------


} // end namespace root
} // end namespace agile
