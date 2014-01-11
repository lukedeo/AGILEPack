//-----------------------------------------------------------------------------
//  activation.hh:
//  Header for neural activation functions
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef ACTIVATION_HH
#define ACTIVATION_HH 

#include "basedefs.hh"

namespace agile
{
namespace functions
{
//----------------------------------------------------------------------------
agile::vector rect_lin_unit(const agile::vector &v);
//----------------------------------------------------------------------------
agile::vector rect_lin_unit_deriv(const agile::vector &v);
//----------------------------------------------------------------------------
agile::vector exp_sigmoid(const agile::vector &v);
//----------------------------------------------------------------------------
// this is for s'(x) = s(x) * (1 - s(x))
agile::vector exp_sigmoid_deriv(const agile::vector &v);
//----------------------------------------------------------------------------
agile::vector softmax(const agile::vector &v);
//----------------------------------------------------------------------------
agile::vector add_noise(const agile::vector &v, numeric level = 0.09);
//----------------------------------------------------------------------------
}
}
#endif