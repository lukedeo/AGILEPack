#include "layer.hh"
#include <stdlib.h>
layer::layer(int n_inputs, int n_outputs, layer_type type) 
: m_inputs(n_inputs), m_outputs(n_outputs), m_batch_size(1), W(n_outputs, n_inputs), 
W_old(n_outputs, n_inputs), W_change(n_outputs, n_inputs), 
b(n_outputs), b_old(n_outputs),b_change(n_outputs), m_out(n_outputs), m_in(n_inputs),
learning(0.1), momentum(0.0), regularizer(0.00), m_layer_type(type)
{
	ctr = 0;
	reset_weights(sqrt((numeric)1 / (numeric)(n_inputs + n_outputs)));
}

void layer::construct(int n_inputs, int n_outputs, layer_type type)
{
	m_inputs = n_inputs;
	m_outputs = n_outputs;
	W.resize(n_outputs, n_inputs);
	W_old.resize(n_outputs, n_inputs);
	b.resize(n_outputs, Eigen::NoChange);
	b_old.resize(n_outputs, Eigen::NoChange);
	m_out.resize(n_outputs, Eigen::NoChange);
	m_in.resize(n_inputs, Eigen::NoChange);
	m_layer_type = type;
}

void layer::reset_weights(numeric bound)
{
	std::uniform_real_distribution <numeric> distribution(-bound, bound);
	for (int col = 0; col < m_inputs; ++col)
	{
		for (int row = 0; row < m_outputs; ++row)
		{
			W(row, col) = distribution(agile::mersenne_engine());
			W_old(row, col) = 0;
			W_change(row, col) = 0;
		}
	}
	for (int row = 0; row < m_outputs; ++row)
	{
		b(row) = distribution(agile::mersenne_engine());
		b_old(row) = 0;
		b_change(row) = 0;
	}

}
// void layer::perturb_weights(numeric bound);

// void layer::set_batch_size(int size);

// pass an input vector, the layer class holds the "charge"
void layer::charge(const agile::vector& v)
{
	m_in = v;
	m_out.noalias() = W * v + b;
}
// Fire the charge.
agile::vector layer::fire()
{
 	switch(m_layer_type)
 	{
 		case sigmoid:
 			return agile::functions::exp_sigmoid(m_out);
 		case softmax:
 			return agile::functions::softmax(m_out);
 		case linear:
 			return m_out;
 		default:
	 		return m_out;

 	}	
}

void layer::backpropagate(const agile::vector &v)
{
	delta.noalias() = v;

	if (m_layer_type == sigmoid)
	{
		delta = delta.cwiseProduct(agile::functions::exp_sigmoid_deriv(agile::functions::exp_sigmoid(m_out)));
	}
	getchar();
	

	m_dump_below = W.transpose() * delta; // we need something to make this not happen for base 0layer

	W_change += delta * m_in.transpose(); 
	b_change += delta;

	++ctr;

	if (ctr >= m_batch_size) // if we need to start a new batch
	{	
		ctr = 0;
		update();
	}
}

agile::vector layer::dump_below()
{
	return m_dump_below;
}

void layer::update()
{
	W_change /= m_batch_size;
	// std::cout << W_change << std::endl;
	W_old = momentum * W_old - learning * (W_change + regularizer * W);
	W += W_old;

	b_change /= m_batch_size;
	b_old = momentum * b_old - learning * b_change;
	b += b_old;

	b_change.fill(0.00);
	W_change.fill(0.00);
}

















layer::~layer()
{
}