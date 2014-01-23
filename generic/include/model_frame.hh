#ifndef MODEL__FRAME__HH
#define MODEL__FRAME__HH 
#include "dataframe.hh"
#include <unordered_set>
namespace agile
{



class model_frame
{
public:
	model_frame(const agile::dataframe &D);
	// model_frame(agile::dataframe &&D);
	model_frame();
	~model_frame();

	void add_dataset(const agile::dataframe &D);
	// void add_dataset(agile::dataframe &&D);

	void model_formula(const std::string &formula);

	void add_constraint(const std::string &name, const std::string constraint);

	void make_binned(const std::string &name, const std::vector<double> bins);

	void generate();

	agile::matrix& Y();
	agile::matrix& X();

private:

	void parse_formula(std::string formula);
	void parse_constraint(std::string formula);

	std::vector<agile::dataframe> datasets;

	agile::matrix X, Y;
	std::string m_formula

	std::map<std::string, std::pair<double, double>> constraints;

	std::map<std::string, std::vector<double> > binner;

	std::vector<std::string> inputs, outputs;

	std::unordered_set<std::string> exclusions;

	bool x_set, y_set;

};


class parsing_error : public std::runtime_error
{
public:
	parsing_error(const std::string &what);
};



}

#endif