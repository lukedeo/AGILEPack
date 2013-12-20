#include "layer.hh"

layer::layer(int n_inputs, int n_outputs) : m_inputs(n_inputs), m_outputs(n_outputs), W(n_outputs, n_inputs), W_old(n_outputs, n_inputs),
                                            b(n_outputs), b_old(n_outputs), m_out(n_outputs), m_in(n_inputs)
                                            
{
	reset_weights(sqrt((numeric)1 / (numeric)(n_inputs + n_outputs)));
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
		}
	}
	for (int row = 0; row < m_outputs; ++row)
	{
		b(row) = distribution(agile::mersenne_engine());
		b_old(row) = 0;
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
	return agile::functions::exp_sigmoid(m_out);
}

layer::~layer()
{
}