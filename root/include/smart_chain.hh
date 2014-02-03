//-----------------------------------------------------------------------------
//  smart_chain.hh:
//  Header for TChain wrapper
//  Authors: Dan Guest (dguest@cern.ch) (primary)
//           Luke de Oliveira (luke.deoliveira@yale.edu) (basically nothing)
//-----------------------------------------------------------------------------

#ifndef SMART_CHAIN_H
#define SMART_CHAIN_H

#include "TChain.h"
#include <string> 
#include <vector> 
#include <set> 
#include <stdexcept>


namespace agile
{
namespace root
{


//-----------------------------------------------------------------------------
//  smart_chain: a TChain that actually works and handles nasty setting.
//-----------------------------------------------------------------------------
class smart_chain: public TChain 
{ 
public: 
    using TChain::Add; 
    smart_chain(std::string tree_name); 
    virtual int add(std::string file_name, long long nentries = -1); 
    template<typename T, typename Z>
    void set_branch(T name, Z branch); 
    std::vector<std::string> get_all_branch_names() const; 
private: 
    typedef std::vector<std::string> Strings; 
    void SetBranchAddressPrivate(std::string name, void* branch);
    void throw_bad_branch(std::string name) const; 
    std::string get_files_string() const; 
    void check_for_dup(const std::string& branch_name) const; 
    std::string m_tree_name; 
    Strings m_set_branches; 
    std::set<std::string> m_set_branch_set; 
    Strings m_files; 
}; 

//----------------------------------------------------------------------------
class MissingBranchError: public std::runtime_error 
{
public: 
    MissingBranchError(const std::string& what_arg); 
}; 

// -------- implementation -----

template<typename T, typename Z>
void smart_chain::set_branch(T name, Z branch)
{
    *branch = 0; 
    SetBranchAddressPrivate(name, branch); 
}

}
}


#endif //SMART_CHAIN_H
