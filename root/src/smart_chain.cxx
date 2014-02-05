//-----------------------------------------------------------------------------
//  smart_chain.cxx:
//  Implementation for TChain wrapper
//  Authors: Dan Guest (dguest@cern.ch) (primary)
//           Luke de Oliveira (luke.deoliveira@yale.edu) (basically nothing)
//-----------------------------------------------------------------------------

#include "include/smart_chain.hh"
#include "TChain.h"
#include "TFile.h"
#include "TError.h"
#include <sstream>
#include <iostream>

namespace agile
{
namespace root
{
//----------------------------------------------------------------------------
smart_chain::smart_chain(std::string tree_name)
: TChain(tree_name.c_str()), m_tree_name(tree_name)
{ 
}
//----------------------------------------------------------------------------
int smart_chain::add(std::string file_name, long long nentries) 
{ 
    m_files.push_back(file_name); 
    TFile file(file_name.c_str()); 
    if (!file.IsOpen() || file.IsZombie()) 
    { 
        throw std::runtime_error("bad file: " + file_name); 
    }
    TTree* tree = static_cast<TTree*>(file.Get(m_tree_name.c_str())); 
    if (!tree || (tree->GetEntries() == 0)) 
    { 
        return 0; 
    }
    return TChain::Add(file_name.c_str(), nentries); 
}
//----------------------------------------------------------------------------
std::vector<std::string> smart_chain::get_all_branch_names() 
const { 
    return m_set_branches; 
}
//----------------------------------------------------------------------------
void smart_chain::SetBranchAddressPrivate(std::string name, void* branch) 
{ 
    check_for_dup(name); 
    m_set_branch_set.insert(name); 
    m_set_branches.push_back(name);

    unsigned branches_found = 0; 
    SetBranchStatus(name.c_str(), 1, &branches_found); 
    if (branches_found != 1) 
    { 
        std::string prob = "missing branch: " + name;
        throw MissingBranchError(prob);
    }

    int return_code = TChain::SetBranchAddress(name.c_str(), branch); 
    if (return_code != 0 && return_code != 5 )
    { 
        std::stringstream issue; 
        issue << "can not set " << name << ", return code" << return_code;
        throw std::runtime_error(issue.str()); 
    }
    if (!GetBranch(name.c_str())) 
    { 
        throw_bad_branch(name); 
    }

}
//----------------------------------------------------------------------------

// ================ private functions ====================

void smart_chain::throw_bad_branch(std::string name) 
const { 
    std::string issue = "can't find branch " + name; 
    std::string file = GetFile()->GetName(); 
    issue.append(", bad file: " + file); 
    throw MissingBranchError(issue); 

}
//----------------------------------------------------------------------------
std::string smart_chain::get_files_string() 
const { 
    std::string issue = " files: "; 
    for (auto itr = m_files.begin(); itr != m_files.end(); itr++) 
    {
        issue.append(*itr); 
        if (*itr != *m_files.rbegin()) issue.append(", "); 
    }
    return issue; 
}
//----------------------------------------------------------------------------
void smart_chain::check_for_dup(const std::string& name) 
const { 
    if (m_set_branch_set.count(name)) 
    { 
        std::string err = "setting branch address: " + name + " is already"
          " set in " + GetName(); 
        throw std::runtime_error(err); 
    }
}
//----------------------------------------------------------------------------
MissingBranchError::MissingBranchError(const std::string& what_arg)
: std::runtime_error(what_arg)
{
}

}
}
