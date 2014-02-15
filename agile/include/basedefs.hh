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
#include "yaml-cpp/yaml_core.hh"

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
//----------------------------------------------------------------------------
void progress_bar(int percent);

namespace colors
{

inline std::string reset()
{
	return "\033[0m";
}
inline std::string black()
{
	return "\033[30m";
}
inline std::string red()
{
	return "\033[31m";
}
inline std::string green()
{
	return "\033[32m";
}
inline std::string yellow()
{
	return "\033[33m";
}
inline std::string blue()
{
	return "\033[34m";
}
inline std::string magenta()
{
	return "\033[35m";
}
inline std::string cyan()
{
	return "\033[36m";
}
inline std::string white()
{
	return "\033[37m";
}
	
}

}

#endif








