#ifndef LAYER_H
#define LAYER_H 

#include <iostream>
#include "basedefs.hh"
#include "activation.hh"



class layer
{
public:
	layer(int n_inputs = 0, int n_outputs = 0);

	void construct(int n_inputs, int n_outputs);

	void reset_weights(numeric bound);
	// pass an input vector, the layer class holds the "charge"
	void charge(const agile::vector& v); 
	virtual agile::vector fire(); // Fire the charge.

	// void perturb_weights(numeric bound);
	// void set_batch_size(int size);

	void backpropagate(const agile::vector &v);

	agile::vector dump_below();

	void update();



	~layer();	
private:
	int m_inputs, m_outputs, m_batch_size, ctr;
	agile::matrix W, W_old, W_change;
	agile::vector b, b_old, b_change, m_out, m_in, delta, m_dump_below;
	numeric learning, momentum, regularizer;
};

namespace agile
{
	typedef std::vector<layer> layer_stack;
}

#endif


