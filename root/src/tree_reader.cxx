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
: m_smart_chain(0), m_size(0), m_tree_name(tree_name)
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
: m_smart_chain(0), m_size(0), m_tree_name(tree_name)
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
            break;
        }
        case double_precision:
        {
            m_smart_chain->set_branch(branch_name, 
                &storage.back()->set_address<double>());
            break;
        }
        case integer:
        {
            m_smart_chain->set_branch(branch_name, 
                &storage.back()->set_address<int>());
            break;
        }
    }
}
//----------------------------------------------------------------------------
agile::dataframe tree_reader::get_dataframe(int entries, int start, bool verbose)
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
        
        D.push_back(std::move(at((unsigned int)curr_entry)));
    }
    return std::move(D);
}
//----------------------------------------------------------------------------
// std::pair<agile::dataframe, agile::dataframe> get_data_pair(
//     const std::vector<std::string> &inputs, 
//     const std::vector<std::string> &targets, int entries, int start)
// {
//     if ((entries > (int)m_size) || ((start + entries) > (int)m_size))
//     {
//         throw dimension_error("tried to access element in TTree beyond range.");
//     }
//     entries = (entries < 0) ? m_size : entries;
//     start = (start < 0) ? 0 : start;
//     auto stop = start + entries;

//     int curr_entry = 0;

//     agile::dataframe X;
//     X.set_column_names(inputs);
//     agile::dataframe Y;
//     Y.set_column_names(targets);


    


// }

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

std::map<std::string, double> tree_reader::operator()(const unsigned int &idx, const std::vector<std::string> &names)
{
    m_smart_chain->GetEntry(idx);
    std::map<std::string, double> map;
    for (auto &name : names)
    {
        map[name] = storage.at(traits[name].pos)->get_value<double>();
    }
    return std::move(map);

}
// std::vector<double> tree_reader::operator[](std::string col_name);

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