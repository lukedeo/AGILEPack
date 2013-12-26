#ifndef BASEDEFS_HH
#define BASEDEFS_HH 

#include <iostream>
#include <Eigen/Dense>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstddef>
#include <stdlib.h>
#include <yaml-cpp/yaml.h> 
#include "base64.hh"
#include "sha1.hh"

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


	inline std::string stringify(const agile::matrix &M)
	{
		std::stringstream ss;
		ss << "#EM|" << M.rows() << "|" << M.cols() << std::setprecision(15);
		for (int i = 0; i < M.rows(); ++i)
		{
			for (int j = 0; j < M.cols(); ++j)
			{
				ss << "," << M(i, j);
			}
		}
		return ss.str();
	}

	inline agile::matrix destringify(const std::string &s)
	{
		auto t(s);
		auto first_bar = s.find_first_of("|");
		auto last_bar = s.find_last_of("|");
		auto found = s.find_first_of(",");
		int rows = std::stoi(s.substr(first_bar + 1, last_bar - first_bar - 1));
		int cols = std::stoi(s.substr(last_bar + 1, found - last_bar - 1));
		
		t.erase(0, found + 1);

		agile::matrix M(rows, cols);
		std::istringstream iss( t );
	    std::vector<double> data;

	    while (std::getline(iss, t, ',')) 
	    {
	        double fieldvalue = 0.000;
	        std::istringstream( t ) >> fieldvalue;
	        data.push_back( fieldvalue );
	    }
	    for (int i = 0; i < rows; ++i)
	    {
	    	for (int j = 0; j < cols; ++j)
	    	{
	    		M(i, j) = data.at(i * cols + j);
	    	}
	    }
	    return M;
	}
}

#endif








