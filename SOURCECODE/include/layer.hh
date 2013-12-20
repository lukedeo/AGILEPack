#ifndef LAYER_H
#define LAYER_H 

#include <iostream>
#include "basedefs.hh"
#include "activation.hh"



class layer
{
public:
	layer(int n_inputs, int n_outputs);

	void reset_weights(numeric bound);
	// void perturb_weights(numeric bound);

	// void set_batch_size(int size);

	// pass an input vector, the layer class holds the "charge"
	void charge(const agile::vector& v); 
	// Fire the charge.
	virtual agile::vector fire();


	~layer();	
private:
	int m_inputs, m_outputs, m_batch_size;
	agile::matrix W, W_old;
	agile::vector b, b_old, m_out, m_in, delta;
};

// namespace agile
// {
// 	typedef std::vector<std::unique_ptr<layer>> layer_stack;
// }

#endif


