//-----------------------------------------------------------------------------
//  tree_reader.hh:
//  Header for dumping ROOT data into a dataframe or some std::vectors
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef ROOT__tree_reader_HH
#define ROOT__tree_reader_HH 

#include <cstddef> //std::size_t
#include "smart_chain.hh"
#include "numeric_handler.hh"
#include "binner.hh"
#include "dataframe/dataframe_core.hh"
#include "agile/agile_base.hh"
#include "yaml-cpp/yaml_core.hh"


namespace agile
{
namespace root
{

//-----------------------------------------------------------------------------
//  Typedefs and utility structures
//-----------------------------------------------------------------------------
enum loading_phase { naming, access };
enum numeric_type { single_precision, double_precision, integer };


struct var_traits
{
    var_traits() {}
    var_traits(std::size_t size, numeric_type type)
    : pos(size), type(type) {}

    std::size_t pos;
    numeric_type type;
};

//-----------------------------------------------------------------------------
//  Tree reader class
//-----------------------------------------------------------------------------
class tree_reader
{
public:
    tree_reader(std::string filename = "", std::string tree_name = "");

    tree_reader(const std::vector<std::string>& files, 
        std::string tree_name = "");

    void add_file(std::string filename, std::string tree_name = "");

    void set_branch(std::string branch_name, numeric_type type);

    void create_binning(const std::string &branch_name, 
        const std::initializer_list<double> &il, bool absolute = false);

    void create_binning(const std::string &branch_name, 
        const std::vector<double> &v, bool absolute = false);

    void create_constraint(const std::string &branch_name, 
        const std::initializer_list<double> &il, bool absolute = false);

    void create_constraint(const std::string &branch_name, 
        const std::vector<double> &v, bool absolute = false);

    void set_branches(const std::string &yamlfile);

    bool entry_in_range();

    void get_entry(int idx)
    {
        m_smart_chain->GetEntry(idx);
    }

    std::vector<std::string> get_ordered_branch_names();

    agile::dataframe get_dataframe(int entries = 1000, int start = -1, 
        bool verbose = false);

    template <class T>
    agile::dataframe get_dataframe(T &weights, int entries = 1000, int start = -1, 
        bool verbose = false);

    std::map<std::string, std::string> get_var_types();

    std::map<std::string, std::vector<double>> get_binning();
    std::map<std::string, std::vector<double>> get_constraints();

    bool is_binned(){return m_binned_present;}
    bool is_constrained(){return m_constraint_present;}

//-----------------------------------------------------------------------------
//  Element Access
//-----------------------------------------------------------------------------
    std::vector<double> at(const unsigned int &idx);
    std::vector<double> operator[](const unsigned int &idx);
    std::vector<double> operator()(const unsigned int &idx);
    double operator()(const unsigned int &idx, std::string col_name);
    std::map<std::string, double> at(const unsigned int &idx, 
        const std::vector<std::string> &names);
    std::map<std::string, double> operator()(const unsigned int &idx, 
        const std::vector<std::string> &names);

//-----------------------------------------------------------------------------
//  Information
//-----------------------------------------------------------------------------

    std::size_t size();
    ~tree_reader();

private:

    smart_chain *m_smart_chain;
    unsigned int m_size, m_num_cols;
    bool m_in_memory;
    std::string m_tree_name;
    std::map<std::string, var_traits> traits;
    std::map<std::string, std::string> variable_type_map;
    typedef __INTERNAL::numeric_handler number_container;
    std::vector<std::unique_ptr<number_container>> storage;


    bool m_binned_present, m_constraint_present;
    std::vector<std::string> feature_names, binned_names, constraint_names;

    std::map<std::string, agile::root::binner> m_binned_vars;
    std::map<std::string, std::vector<double>> m_binning_strategy;

    std::map<std::string, agile::root::binner> m_constraint_vars;
    std::map<std::string, std::vector<double>> m_constraint_strategy;

};

// template function implementation
//----------------------------------------------------------------------------


template <class T>
agile::dataframe tree_reader::get_dataframe(T &weights, int entries, int start, 
    bool verbose)
{
    if ((entries > (int)m_size) || ((start + entries) > (int)m_size))
    {
        throw dimension_error(
            "tried to access element in TTree beyond range.");
    }

    if (verbose)
    {
        std::cout << "\nPulling agile::dataframe from tree_reader..." << std::endl;
    }
    entries = (entries < 0) ? m_size : entries;
    start = (start < 0) ? 0 : start;
    auto stop = start + entries;

    int curr_entry = 0;
    double pct;
    agile::dataframe D;

    auto all_names = feature_names;
    auto temp_names = feature_names;
    if (m_binned_present)
    {
        for (auto &entry : binned_names)
        {
            all_names.push_back("categ_" + entry);
        }
        temp_names = all_names;
    }
    all_names.push_back("JET_WEIGHT");
    D.set_column_names(all_names);
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
            auto tmp_vec = std::move(at((unsigned int)curr_entry));

            auto categ_map = at((unsigned int)curr_entry, temp_names);
            
            tmp_vec.push_back(weights.get_weight(categ_map));
            D.push_back(tmp_vec);

        }
        
    }
    return std::move(D);
}









} // end ns root
} // end ns agile

#endif