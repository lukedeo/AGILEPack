#ifndef BASEDEFS_HH
#define BASEDEFS_HH 

#include <iostream>
#include <Eigen/Dense>
#include <memory>
#include <utility>
#include <vector>
#include <cmath>
#include <random>

typedef double numeric;

namespace agile
{
	typedef Eigen::Matrix<numeric, Eigen::Dynamic, Eigen::Dynamic> matrix;
	typedef Eigen::Matrix<numeric, Eigen::Dynamic, 1>  colvec;
	typedef Eigen::Matrix<numeric, 1, Eigen::Dynamic>  rowvec;
	typedef colvec vector;

	inline std::mt19937_64& mersenne_engine() // dumb random number generator
	{
		static std::mt19937_64 _eng{};
		return _eng;
	}
}

#endif








