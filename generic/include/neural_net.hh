#ifndef NEURAL__NET__HH
#define NEURAL__NET__HH 

#include "architecture.hh"
#include "dataframe.hh"
#include "model_frame.hh"

namespace agile
{
	
class neural_net : public architecture
{
public:
	neural_net(int num_layers = 0);
	neural_net(std::initializer_list<int> il, problem_type type = regress);
	~neural_net();
	neural_net(const neural_net &arch);
	~neural_net();

	void add_data(const std::string &filename);
	void add_data(const agile::dataframe &D);
	void add_data(agile::dataframe &&D);

	// void set_formula(const std::string &formula);
	void add_predictor(const std::string &name);
	void add_target(const std::string &name);
	void model_formula(std::string)
	agile::dataframe& data();

private:
	std::vector<std::string> predictor_order, target_order;
	agile::matrix X, Y;

};


}

#endif