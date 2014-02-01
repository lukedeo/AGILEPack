#ifndef MODEL__FRAME__HH
#define MODEL__FRAME__HH 
#include "dataframe.hh"
#include "basedefs.hh"
#include <unordered_set>
namespace agile
{


struct scaling
{
	std::map<std::string, numeric> mean;
	std::map<std::string, numeric> sd;
};

inline void calc_normalization(const agile::vector &input, const std::string col_name,
	agile::scaling &scale)
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

    // input.array() -= M;
    // input /= sd;

    scale.sd[col_name] = sd;
    scale.mean[col_name] = M;
}



class model_frame
{
public:
	model_frame(const agile::dataframe &D);
	// model_frame(agile::dataframe &&D);
	model_frame();
	~model_frame();

	void add_dataset(const agile::dataframe &D);
	void add_dataset(agile::dataframe &&D);

	void model_formula(const std::string &formula);
	// void add_constraint(const std::string &name, const std::string constraint);

	// void make_binned(const std::string &name, const std::vector<double> bins);

	void generate();
	void scale();
	// void load_scaling(const agile::scaling &scale);

	// agile::scaling get_scaling();

	agile::matrix& Y();
	agile::matrix& X();

private:

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


class parsing_error : public std::runtime_error
{
public:
	parsing_error(const std::string &what);
};






}

#endif