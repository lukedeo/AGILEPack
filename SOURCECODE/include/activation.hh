#ifndef ACTIVATION_HH
#define ACTIVATION_HH 

#include "basedefs.hh"

namespace agile
{
namespace functions
{


inline agile::vector exp_sigmoid(const agile::vector &v)
{
	agile::vector w(v);
	for (int row = 0; row < v.rows(); ++row)
	{
		w(row) = 1 / (1 + exp(-w(row)));
	}
	return (w);
}

// this is for s'(x) = s(x) * (1 - s(x))
inline agile::vector exp_sigmoid_deriv(const agile::vector &v)
{
	agile::vector w(v);
	for (int row = 0; row < v.rows(); ++row)
	{
		w(row) = w(row) * (1 - w(row));
	}
	return (w);
}

inline agile::vector softmax(const agile::vector &v)
{
	agile::vector w(v);
	for (int row = 0; row < v.rows(); ++row)
	{
		w(row) = exp(w(row));
	}
	w /= w.sum();
	return (w);
}

}
}




#endif