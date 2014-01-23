#include "model_frame.hh"

namespace agile
{

model_frame::model_frame(const agile::dataframe &D)
: x_set(false), y_set(false)
{
	datasets.push_back(D);
}
// model_frame(agile::dataframe &&D);
//----------------------------------------------------------------------------
model_frame::model_frame()
: x_set(false), y_set(false)
{

}
//----------------------------------------------------------------------------
model_frame::~model_frame()
{

}
//----------------------------------------------------------------------------
model_frame::add_dataset(const agile::dataframe &D)
{
	datasets.push_back(D);
}
// model_frame::add_dataset(agile::dataframe &&D);
//----------------------------------------------------------------------------
model_frame::model_formula(const std::string &formula);

model_frame::add_constraint(const std::string &name, const std::string constraint);

model_frame::make_binned(const std::string &name, const std::vector<double> bins);

model_frame::generate();


agile::matrix& model_frame::Y();
agile::matrix& model_frame::X();

void model_frame::parse_formula(std::string formula)
{
    formula = agile::no_spaces(formula);
    bool parsing = true, wildcard = false;
    auto tilde = formula.find_first_of("~");
    if (tilde != formula.find_last_of("~"))
    {
        throw std::runtime_error("can't specify multiple \'is a function of\' operators (uses of \'~\') in one formula.");
    }

    auto wildcard_find = formula.find_first_of("*");
    if (wildcard_find != formula.find_last_of("*"))
    {
        throw std::runtime_error("can't specify multiple \'include all defined variables\' operators (uses of \'*\') in one formula.");
    }
    
    if (wildcard_find != std::string::npos)
    {
        wildcard = true;
        std::string::iterator end_pos = std::remove(formula.begin(), formula.end(), '*');
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
    	for (auto &name : datasets.at(0).get_column_names())
    	{
    		if (exclusions.count(name) == 0)
    		{
    			inputs.push_back(name)
    		}
    	}
        
    }
}




void model_frame::parse_constraint(const std::string &formula);

	std::vector<agile::dataframe> datasets;

	agile::matrix X, Y;
	std::string m_formula

	std::map<std::string, std::pair<double, double>> constraints;

	std::map<std::string, std::vector<double> > binner;

	std::vector<std::string> inputs, outputs;



parsing_error::parsing_error(const std::string &what)
: std::runtime_error(what)
{
}

}