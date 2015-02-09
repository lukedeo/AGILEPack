//-----------------------------------------------------------------------------
//  struct_generator.hh -- header only class for heuristically generating
//  deep network structures.
//  Author: Luke de Oliveira (lukedeo@stanford.edu)
//-----------------------------------------------------------------------------

#include <cmath>
#include <string>
#include <iostream>
#include <vector>


namespace agile
{
namespace heuristics
{

enum difficulty { easy, medium, hard };

class struct_generator
{
public:
    struct_generator() = default;
    ~struct_generator() = default;

    void parse_formula(std::string formula, std::vector<std::string> branches_available, bool verbose = true);

    std::vector<int> get_structure(difficulty problem = medium, bool verbose = true);

private:
    std::string weighting_variable = "";
    std::vector<std::string> inputs, outputs;
    std::unordered_set<std::string> exclusions;
};

//----------------------------------------------------------------------------
inline void struct_generator::parse_formula(std::string formula, std::vector<std::string> branches_available, bool verbose)
{
    if (verbose)
    {
        std::cout << "Checking for reweighting...";
    }
    auto pipe = formula.find_first_of("|");

    if (pipe != std::string::npos)
    {
        weighting_variable = agile::no_spaces(formula.substr(pipe + 1));
        formula = formula.substr(0, pipe);
        exclusions.insert(weighting_variable);
        if (verbose)
        {
            std::cout << "found." << std::endl;
        }
    }
    else
    {
        if (verbose)
        {
            std::cout << "not found." << std::endl;
        }
    }

    if (verbose)
    {
        std::cout << "Parsing formula for outputs...";
    }
    
    formula = agile::no_spaces(formula);
    bool parsing = true, wildcard = false;
    auto tilde = formula.find_first_of("~");
    if (tilde != formula.find_last_of("~"))
    {
        std::string e("can't specify multiple \'is a function of\'");
        e.append(" operators (uses of \'~\') in one formula.");
        throw agile::parsing_error(e);
    }

    auto wildcard_find = formula.find_first_of("*");
    if (wildcard_find != formula.find_last_of("*"))
    {
        std::string e("can't specify multiple \'include all defined");
        e.append(" variables\' operators (uses of \'*\') in one formula.");
        throw agile::parsing_error(e);
    }
    
    if (wildcard_find != std::string::npos)
    {
        wildcard = true;
        std::string::iterator end_pos = std::remove(
            formula.begin(), formula.end(), '*');

        formula.erase(end_pos, formula.end());
    }
    auto lhs = formula.substr(0, tilde);
    auto rhs = formula.substr(tilde + 1);
    auto end = lhs.find_first_of('+');
    unsigned long int start = 0;
    while(parsing == true)
    {
        auto new_var = lhs.substr(start, end - start);
        outputs.push_back(new_var);
        if (wildcard)
        {
            exclusions.insert(new_var);
        }  
        
        if (end == std::string::npos) parsing = false;
        start = end + 1;
        end = lhs.find_first_of('+', start);
    }
    if (verbose)
    {
        std::cout << "done." << std::endl;
        std::cout << "Parsing formula for inputs...";
    }
    parsing = true;
    start = (rhs[0] == '-' || rhs[0] == '+') ? 1 : 0;
    end = rhs.find_first_of("+-", start);
    while(parsing == true)
    {
        auto new_var = rhs.substr(start, end - start);
        if (rhs[start - 1] == '-')
        {
            exclusions.insert(new_var);
        }
        else
        {
            if (!wildcard)
            {
                inputs.push_back(new_var);
            }
        }
        if (end == std::string::npos) parsing = false;
        start = end + 1;
        end = rhs.find_first_of("+-", start);
    }
    if (verbose)
    {
        std::cout << "done." << std::endl;
    }
    if (wildcard)
    {
        if (verbose)
        {
            std::cout << "Filling in wildcard...";
        }
        for (auto &name : branches_available)
        {
            if (exclusions.count(name) == 0)
            {
                inputs.push_back(name);
            }
        }
        if (verbose)
        {
            std::cout << "done." << std::endl;
        } 
    }
}
//----------------------------------------------------------------------------
inline std::vector<int> struct_generator::get_structure(difficulty problem, bool verbose)
{
    double boost, depth_factor;
    switch(problem)
    {
        case easy: 
            boost = 1.05;
            depth_factor = 1.0;
            break;
        case medium: 
            boost = 1.15;
            depth_factor = 1.05;
            break;
        case hard: 
            boost = 1.30;
            depth_factor = 1.1;
            break;
    }

    int n_inputs = inputs.size(), n_outputs = outputs.size();

    int next_layer = ceil(boost * (double)n_inputs);

    if (verbose)
    {
        std::cout << "Found " << n_inputs << " input variables, and " << n_outputs << " output variables." << std::endl;
        std::cout << "Boosting to a " << next_layer << " dimensional encoding in layer 2." << std::endl;
    }
    int n_layers;

    if (n_inputs < 6)
    {
        n_layers = 3;
    }
    else
    {
        n_layers = floor(log2(n_inputs));
    }

    if ((n_layers > 5) && (n_inputs < 63))
    {
        n_layers = 5;
    }

    if ((weighting_variable != "") && (problem != easy))
    {
        boost *= 1.1;
    }

    n_layers *= depth_factor;

    std::vector<int> structure;

    structure.push_back(n_inputs);
    structure.push_back(next_layer);

    double last;

    int drop = floor((n_inputs - n_outputs) / (n_layers - 1));
    for (int i = 0; i < (n_layers - 1); ++i)
    {
        last = structure.back();
        structure.push_back(last - drop);
    }
    structure.push_back(n_outputs);

    for (int i = structure.size() - 1; i > 1; --i)
    {
       if (structure.at(i - 1) == structure.at(i))
       {
           ++structure.at(i - 1);
       }
    }

    if (verbose)
    {
        std::cout << "Auto-generated structure of ";
        for (auto &entry : structure)
        {
            std::cout << entry << ", ";
        }
        std::cout << "which will be used by the algorithm." << std::endl;

    }
    return structure;
}

}
}