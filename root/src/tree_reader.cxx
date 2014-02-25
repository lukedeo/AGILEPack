//-----------------------------------------------------------------------------
//  tree_reader.cxx:
//  Header for dumping ROOT data into a dataframe or some std::vectors
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------
#include "include/tree_reader.hh"

namespace agile
{
namespace root
{

//----------------------------------------------------------------------------
tree_reader::tree_reader(std::string filename, std::string tree_name)
: m_smart_chain(0), m_size(0), m_tree_name(tree_name), m_binned_present(false)
{
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
}
//----------------------------------------------------------------------------
tree_reader::tree_reader(const std::vector<std::string>& files, 
    std::string tree_name)
: m_smart_chain(0), m_size(0), m_tree_name(tree_name), m_binned_present(false)
{
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
}
//----------------------------------------------------------------------------
tree_reader::~tree_reader()
{
    delete m_smart_chain;
}
//----------------------------------------------------------------------------
void tree_reader::add_file(std::string filename, std::string tree_name)
{
    if (!m_smart_chain)
    {
        m_smart_chain = new smart_chain(tree_name);
    }
    m_smart_chain->add(filename);
    m_size = m_smart_chain->GetEntries();
}
//----------------------------------------------------------------------------
void tree_reader::set_branch(std::string branch_name, numeric_type type)
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
}
//----------------------------------------------------------------------------
bool tree_reader::entry_in_range()
{
    if (!m_binned_present)
    {
        return true;
    }
    bool ok = true;
    for (auto &name : binned_names)
    {
        ok = ok && m_binned_vars[name].in_range(
            storage.at(traits[name].pos)->get_value<double>());
    }
    return ok;
}
//----------------------------------------------------------------------------
void tree_reader::create_binning(const std::string &branch_name, 
    const std::initializer_list<double> &il)
{
    binned_names.push_back(branch_name);
    m_binned_vars[branch_name].set_name(branch_name).set_bins(il);
    m_binned_present = true;
}

//----------------------------------------------------------------------------
void tree_reader::create_binning(const std::string &branch_name, 
    const std::vector<double> &v)
{
    binned_names.push_back(branch_name);
    m_binned_vars[branch_name].set_name(branch_name).set_bins(v);
    m_binned_present = true;
}
//----------------------------------------------------------------------------
void tree_reader::set_branches(const std::string &yamlfile)
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
                throw std::domain_error("type " + type + " for branch " + entry.first + ".");
            }
        }
    }
    catch(YAML::BadConversion &e)
    {
        throw std::runtime_error("configuration files must have a map entitled 'branches'");
    }
    try
    {   
        std::cout << "HERE!" << std::endl;

        YAML::Node strat = tmp["binning"]["pt"];

        auto vec = strat.as<std::vector<double>>();


        YAML::Node binning = tmp["binning"];

        auto bins = binning.as<std::map<std::string, std::vector<double>>>();



        for (auto &entry : bins["pt"])
        {
            std::cout << "  " << entry << std::endl;
        }
    }
    catch(YAML::BadConversion &e){}

    
}
//----------------------------------------------------------------------------
std::map<std::string, std::string> tree_reader::get_var_types()
{
    return variable_type_map;
}
//----------------------------------------------------------------------------
agile::dataframe tree_reader::get_dataframe(int entries, int start, 
    bool verbose)
{
    if ((entries > (int)m_size) || ((start + entries) > (int)m_size))
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
    D.set_column_names(feature_names);
    for (curr_entry = start; curr_entry < stop; ++curr_entry)
    {
        if (verbose)
        {
            pct = (double)(curr_entry - start) / (double)(entries);
            agile::progress_bar(pct * 100);
        }

        m_smart_chain->GetEntry(curr_entry);

        if (entry_in_range())
        {
            D.push_back(std::move(at((unsigned int)curr_entry)));
        }
        
    }
    return std::move(D);
}

//-----------------------------------------------------------------------------
//  Element Access
//-----------------------------------------------------------------------------
std::vector<double> tree_reader::at(const unsigned int &idx)
{
    m_smart_chain->GetEntry(idx);
    std::vector<double> v;
    for (auto &name : feature_names)
    {
        v.push_back(storage.at(traits[name].pos)->get_value<double>());
    }
    if (m_binned_present)
    {
        for (auto &name : binned_names)
        {
            int bin = m_binned_vars[name].get_bin(
                storage.at(traits[name].pos)->get_value<double>());
            v.push_back((double)bin);
        }
    }
    return std::move(v);
}
//----------------------------------------------------------------------------
std::vector<double> tree_reader::operator[](const unsigned int &idx)
{
    m_smart_chain->GetEntry(idx);
    return std::move(at(idx));
}
//----------------------------------------------------------------------------
std::vector<double> tree_reader::operator()(const unsigned int &idx)
{
    m_smart_chain->GetEntry(idx);
    return std::move(at(idx));
}
//----------------------------------------------------------------------------
double tree_reader::operator()(const unsigned int &idx, std::string col_name)
{
    m_smart_chain->GetEntry(idx);
    return storage.at(traits[col_name].pos)->get_value<double>();
}

std::map<std::string, double> tree_reader::operator()(const unsigned int &idx, 
    const std::vector<std::string> &names)
{
    m_smart_chain->GetEntry(idx);
    std::map<std::string, double> map;
    for (auto &name : names)
    {
        map[name] = storage.at(traits[name].pos)->get_value<double>();
    }
    return std::move(map);

}

//-----------------------------------------------------------------------------
//  Information
//-----------------------------------------------------------------------------

std::size_t tree_reader::size()
{
    return m_size;
}
//----------------------------------------------------------------------------
std::vector<std::string> tree_reader::get_ordered_branch_names()
{
    return feature_names;
}
//----------------------------------------------------------------------------


}
}