//-----------------------------------------------------------------------------
//  activation.hh:
//  Header for neural activation functions
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef ACTIVATION_HH
#define ACTIVATION_HH 

#include "agile/include/basedefs.hh"

namespace agile
{
namespace functions
{
//----------------------------------------------------------------------------
agile::vector rect_lin_unit(const agile::vector &v);
//----------------------------------------------------------------------------
agile::vector rect_lin_unit_deriv(const agile::vector &v);
//----------------------------------------------------------------------------
/**
 * @brief Sigmoid Function
 * @details Calculates 
 * \f[
 * \sigma(x) = \frac{1}{1 + e ^ {-x}}
 * \f]
 * for each element \f$x\f$ of the agile::vector v;
 * 
 * @param v An agile::vector
 * @return An agile::vector with \f$\sigma(x)\f$ applied to each element.
 */
agile::vector exp_sigmoid(const agile::vector &v);
//----------------------------------------------------------------------------
// this is for s'(x) = s(x) * (1 - s(x))
/**
 * @brief Calculates a sigmoid derivative.
 * @details It is assumed that the agile::vector v that is passed contains 
 * activations from the agile::functions::exp_sigmoid() function. Then, for each
 * \f$\sigma(x)\f$ in v, it calculates \f$\sigma'(x) = \sigma(x)(1 - \sigma(x))\f$.
 * 
 * @param v The input vector.
 * @return The calculated derivative.
 */
agile::vector exp_sigmoid_deriv(const agile::vector &v);
//----------------------------------------------------------------------------
/**
 * @brief Calculates the softmax.
 * @details The agile::functions::softmax() function provides an implementation
 * of the continuous extension of the max() function. It calulates
 * \f[
 * \mathsf{softmax}(x_i) = \frac{e^{x_i}}{\sum_{j}e^{x_j}}
 * \f]
 * for each element of v, and returns this.
 * 
 * @param v The agile::vector to impose softmax over.
 * @return The agile::vector of softmax activations.
 */
agile::vector softmax(const agile::vector &v);
//----------------------------------------------------------------------------

/**
 * @brief Adds noise to an agile::vector passed.
 * @param v an agile::vector to be corrupted.
 * @param level The level of noise to add.
 * @return Returns \f$v+N(0,\sigma^2 I)\f$ where \f$\sigma = \f$level.
 */
agile::vector add_noise(const agile::vector &v, numeric level = 0.02);
//----------------------------------------------------------------------------

}
}
#endif