//-----------------------------------------------------------------------------
//  model_frame.hh:
//  Header for managing variable names, scaling, and formula parsing
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef MODEL__FRAME__HH
#define MODEL__FRAME__HH 
#include "dataframe.hh"
#include "Core.hh"
#include <unordered_set>

//----------------------------------------------------------------------------
namespace agile
{
//----------------------------------------------------------------------------
struct scaling
{
	std::map<std::string, double> mean;
	std::map<std::string, double> sd;
};
//----------------------------------------------------------------------------
inline void calc_normalization(const agile::vector &input, 
	const std::string col_name, agile::scaling &scale)
{
	unsigned int count = input.rows();
    double M = input(0);
    double Q = 0.0;
    for (unsigned int k = 1 ; k < count ; k++) 
    {
        double del = input(k) - M;
        unsigned int j = k + 1;
        Q += k * (del * del) / j;
        M += del / j;
    }
    double sd = std::sqrt(Q / (count - 1));
    scale.sd[col_name] = sd;
    scale.mean[col_name] = M;
}

//-----------------------------------------------------------------------------
//  model_frame class declaration
//-----------------------------------------------------------------------------
class model_frame
{
public:
//-----------------------------------------------------------------------------
//  Constructors, destructors, etc.
//-----------------------------------------------------------------------------

	model_frame(const agile::dataframe &D);
	// model_frame(agile::dataframe &&D);
	model_frame();
	~model_frame();
//-----------------------------------------------------------------------------
//  Adding dataframes
//-----------------------------------------------------------------------------

	void add_dataset(const agile::dataframe &D);
	void add_dataset(agile::dataframe &&D);

//-----------------------------------------------------------------------------
//  Parsing!
//-----------------------------------------------------------------------------

	void model_formula(const std::string &formula);
	// void add_constraint(const std::string &name, const std::string constraint);

	// void make_binned(const std::string &name, const std::vector<double> bins);

//-----------------------------------------------------------------------------
//  generation and final model frames.
//-----------------------------------------------------------------------------
	void generate();
	void scale();


	void load_scaling(const agile::scaling &scale);
	// void load_scaling(agile::scaling &&scale);
	agile::scaling get_scaling();
	agile::matrix& Y();
	agile::matrix& X();

	std::vector<std::string> get_inputs();
	std::vector<std::string> get_outputs();

private:

//-----------------------------------------------------------------------------
//  Internal implementations and private data members.
//-----------------------------------------------------------------------------

	void parse_formula(std::string formula);
	// void parse_constraint(std::string formula);

	agile::dataframe DF;

	agile::matrix m_X, m_Y;
	std::string m_formula;

	// std::map<std::string, std::pair<double, double>> constraints;

	std::map<std::string, std::vector<double> > binner;

	std::vector<std::string> inputs, outputs;

	std::unordered_set<std::string> exclusions;

	bool x_set, y_set;

	agile::scaling m_scaling;

};

//-----------------------------------------------------------------------------
//  Error for bd formula parsing
//-----------------------------------------------------------------------------
class parsing_error : public std::runtime_error
{
public:
	parsing_error(const std::string &what);
};
}


namespace YAML 
{

template<>
struct convert<agile::scaling> 
{
    static Node encode(const agile::scaling &scale)
    {
        Node node;

        node["mean"] = scale.mean;
        node["sd"] = scale.sd;

        return node;
    }

    static bool decode(const Node& node, agile::scaling &scale) 
    {
    	scale.mean = node["mean"].as<std::map<std::string, double>>();
    	scale.sd = node["sd"].as<std::map<std::string, double>>();
        return true;
    }
};

} // end namespace yaml






#endif