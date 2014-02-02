//-----------------------------------------------------------------------------
//  basedefs.hh:
//  Header for basic definitions, typedefs, and such
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef BASEDEFS_HH
#define BASEDEFS_HH 

#include <stdexcept>
#include <iostream>
#include <Eigen/Dense>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <cmath>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstddef>
#include <stdlib.h>
#include "yaml-cpp/yaml.h"

typedef double numeric;
enum layer_type { linear, rectified, sigmoid, softmax };

namespace agile
{
//----------------------------------------------------------------------------
typedef Eigen::Matrix<numeric, Eigen::Dynamic, Eigen::Dynamic> matrix;
typedef Eigen::Matrix<numeric, Eigen::Dynamic, 1>  colvec;
typedef Eigen::Matrix<numeric, 1, Eigen::Dynamic>  rowvec;
typedef colvec vector;

typedef Eigen::Map<Eigen::VectorXd> std_2_eigen;
namespace types
{
	enum paradigm { Basic, Autoencoder, Boltzmann, Dropout };
}
//----------------------------------------------------------------------------
std::mt19937_64& mersenne_engine();
//----------------------------------------------------------------------------
std::string stringify(const agile::matrix &M);
//----------------------------------------------------------------------------
agile::matrix destringify(const std::string &s);
//----------------------------------------------------------------------------
agile::vector std_to_Eigen(std::vector<numeric> &v);
//----------------------------------------------------------------------------
template<class D>
inline agile::matrix eigen_spew(D &d)
{
    agile::matrix M(d.rows(), d.columns());
    int ctr = 0;
    for (auto &row : d.raw())
    {
        M.row(ctr) = agile::std_to_Eigen(row);
        ++ctr;
    }
    return std::move(M);
}

}

#endif








