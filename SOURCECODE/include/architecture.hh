#ifndef ARCHITECTURE_HH
#define ARCHITECTURE_HH 

#include "layer.hh"

// namespace agile
// {
// 	enum problem_type { regression, classification, softmax_classification };
// }

class architecture
{
public:
	architecture(int num_layers = 0);
	~architecture();
	void push_back(layer &L);
	layer& at(unsigned int idx);
	void pop_back();
	unsigned int size();
	agile::vector predict(const agile::vector &v);
	void correct(const agile::vector &in, const agile::vector &target);
	// void to_yaml(std::string filename = "file.yaml");
	friend YAML::Emitter& operator << (YAML::Emitter& out, const architecture &arch);

// private:
	unsigned int n_layers;
	agile::layer_stack stack;
};




#endif