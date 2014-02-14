//-----------------------------------------------------------------------------
//  numeric_handler.hh:
//  Header for setting ROOT branches, handling int, float, double
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef INTERNAL__NUMERIC__HH
#define INTERNAL__NUMERIC__HH 

#include <stdexcept>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <cstddef>
#include <stdlib.h>

// with some hiding
namespace __INTERNAL 
{
//-----------------------------------------------------------------------------
//  numeric_handler -- a generic numeric container
//-----------------------------------------------------------------------------
class numeric_handler //union (one can be set at a time)
{
public:
	numeric_handler() = default;
	~numeric_handler() = default;

	template <typename T>
	T& set_address()
	{
		// defaults to an exception, T needs to be instantiated with a type
		// that has been specialized (below)
		throw std::runtime_error(
			"unrecognized type passed to numeric_handler.");
	}
	enum container_contents { has_double, has_int, has_float };

	template <typename T>
	T get_value()
	{
		switch(contains)
		{
			case has_double: return static_cast<T>(_double);
			case has_int: return static_cast<T>(_int);
			case has_float: return static_cast<T>(_float);
		}
		throw std::runtime_error(
			"something weird happened in the numeric_handler.");
	}
private:	
	double _double = 0;
	int _int = 0;
	float _float = 0;

	container_contents contains;
};

//-----------------------------------------------------------------------------
//  Template specializations!
//-----------------------------------------------------------------------------
template<>
inline double& numeric_handler::set_address<double>()
{
	contains = has_double;
	return _double;
}
//----------------------------------------------------------------------------
template<>
inline float& numeric_handler::set_address<float>()
{
	contains = has_float;
	return _float;
}
//----------------------------------------------------------------------------
template<>
inline int& numeric_handler::set_address<int>()
{
	contains = has_int;
	return _int;
}
//----------------------------------------------------------------------------






}

#endif