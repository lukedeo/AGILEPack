//-----------------------------------------------------------------------------
//  model_frame.cxx:
//  Implementation for managing variable names, scaling, and formula parsing
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "model_frame.hh"

namespace agile
{

//----------------------------------------------------------------------------
model_frame::model_frame(const agile::dataframe &D)
: DF(D), weighting_variable(""), x_set(false), y_set(false), weights_set(false)
{
}
// model_frame(agile::dataframe &&D);
//----------------------------------------------------------------------------
model_frame::model_frame()
: weighting_variable(""), x_set(false), y_set(false), weights_set(false)
{
}
//----------------------------------------------------------------------------
model_frame::~model_frame()
{
}
//----------------------------------------------------------------------------
void model_frame::add_dataset(const agile::dataframe &D)
{
	DF.append(D);
}
//----------------------------------------------------------------------------
void model_frame::add_dataset(agile::dataframe &&D)
{
    DF.append(std::move(D));
}
//----------------------------------------------------------------------------
void model_frame::model_formula(const std::string &formula)
{
    m_formula = formula;
	parse_formula(formula);
}

void model_frame::generate(bool verbose)
{
    agile::matrix T = eigen_spew(DF);

    m_X.resize(DF.rows(), inputs.size());
    m_Y.resize(DF.rows(), outputs.size());

    unsigned int columns_to_extract = inputs.size();

    if (outputs.at(0) != "")
    {
        columns_to_extract += outputs.size();
    }
    if (verbose)
    {
        std::cout << "\nGenerating base model formula..." << std::endl;
    }

    int idx = 0, ctr = 0;
    double pct;
    for (auto &name : inputs)
    {
        m_X.col(idx) = T.col(DF.get_column_idx(name));
        ++idx;
        ++ctr;
        if (verbose)
        {
            pct = (double)(ctr) / (double)(columns_to_extract);
            agile::progress_bar(pct * 100);
        }
    }
    x_set = true;
    idx = 0;
    if (outputs.at(0) != "")
    {
        for (auto &name : outputs)
        {
            m_Y.col(idx) = T.col(DF.get_column_idx(name));
            ++idx;
            ++ctr;
            if (verbose)
            {
                pct = (double)(ctr) / (double)(columns_to_extract);
                agile::progress_bar(pct * 100);
            }
        }
        y_set = true;
    }
    if (weighting_variable != "")
    {
        m_weighting = T.col(DF.get_column_idx(weighting_variable));
        weights_set = true;
    }
}

//----------------------------------------------------------------------------
void model_frame::scale(bool verbose)
{
    if (!x_set)
    {
        throw std::runtime_error("must load an X into model frame before scaling.");
    }
    unsigned int columns_to_extract = inputs.size();
    int idx = 0;
    double pct;
    if (verbose)
    {
        std::cout << "\nScaling model frame..." << std::endl;
    }
    for (auto &name : inputs)
    {
        agile::calc_normalization(m_X.col(idx), name, m_scaling);
        m_X.col(idx).array() -= m_scaling.mean[name];
        m_X.col(idx) /= m_scaling.sd[name];
        ++idx;
        if (verbose)
        {
            pct = (double)(idx) / (double)(columns_to_extract);
            agile::progress_bar(pct * 100);
        }
    }
    if (verbose)
    {
        std::cout << std::endl;
    }
}
//----------------------------------------------------------------------------
void model_frame::load_scaling(const agile::scaling &scale)
{
    m_scaling = scale;
    int idx = 0;
    for (auto &name : inputs)
    {
        m_X.col(idx).array() -= m_scaling.mean[name];
        m_X.col(idx) /= m_scaling.sd[name];
        ++idx;
    }
    idx = 0;
}

//----------------------------------------------------------------------------
agile::scaling model_frame::get_scaling()
{
    return m_scaling;
}
//----------------------------------------------------------------------------
agile::matrix& model_frame::Y()
{
    return m_Y;
}
//----------------------------------------------------------------------------
agile::matrix& model_frame::X()
{
    return m_X;
}
//----------------------------------------------------------------------------
agile::vector& model_frame::weighting()
{
    if (weighting_variable != "")
    {
        return m_weighting;
    }
    else
    {
        throw std::runtime_error("no weighting variable set.");
    }  
}
//----------------------------------------------------------------------------
// parses formulas of the form bottom ~ pt + eta | weight
void model_frame::parse_formula(std::string formula)
{
    auto pipe = formula.find_first_of("|");

    if (pipe != std::string::npos)
    {
        weighting_variable = agile::no_spaces(formula.substr(pipe + 1));
        formula = formula.substr(0, pipe);
        exclusions.insert(weighting_variable);
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
    if (wildcard)
    {
    	for (auto &name : DF.get_column_names())
    	{
    		if (exclusions.count(name) == 0)
    		{
    			inputs.push_back(name);
    		}
    	}    
    }
    x_set = (inputs.size() == 0) ? false : true;
    y_set = (outputs.size() == 0) ? false : true;
}
//----------------------------------------------------------------------------

std::vector<std::string> model_frame::get_inputs()
{
    return inputs;
}
//----------------------------------------------------------------------------
std::vector<std::string> model_frame::get_outputs()
{
    return outputs;
}
//----------------------------------------------------------------------------
parsing_error::parsing_error(const std::string &what)
: std::runtime_error(what)
{}

}