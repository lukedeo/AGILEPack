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
	architecture();
	~architecture();
	void push_back(layer &L);
	void pop_back();
	unsigned int size();
	agile::vector predict(const agile::vector &v);
	void correct(const agile::vector &in, const agile::vector &target);

private:
	unsigned int n_layers;
	agile::layer_stack stack;
};




#endif