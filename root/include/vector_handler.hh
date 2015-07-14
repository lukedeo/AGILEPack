//-----------------------------------------------------------------------------
//  vector_handler.hh:
//  Header for setting ROOT branches, handling vector<int>, vector<float>, vector<double>
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//          Michela Paganini (michela.paganini@yale.edu)
//-----------------------------------------------------------------------------

#ifndef INTERNAL__VECTOR__HH
#define INTERNAL__VECTOR__HH 

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
//  vector_handler -- a generic numeric container
//-----------------------------------------------------------------------------
class vector_handler //union (one can be set at a time)
{
public:
    vector_handler() = default;
    ~vector_handler() = default;

    template <typename T>
    std::vector<T>& set_address()
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
	case has_double:
	  {
	  //	  std::vector<double> v_double(_double.begin(), _double.end());
	  T v_double(_double.begin(), _double.end());
	  return v_double;
	  }
	case has_int:
	  {
	  // std::vector<double> v_int(_int.begin(), _int.end());
	  T v_int(_int.begin(), _int.end());
	  return v_int;
	  }	
	case has_float:
	  { 
	  //std::vector<double> v_float(_float.begin(), _float.end());
          T v_float(_float.begin(), _float.end());
	  return v_float;
	  }    
        }
        throw std::runtime_error(
            "something weird happened in the numeric_handler.");
    }
private:    
  std::vector<double> _double;
  std::vector<int> _int;
  std::vector<float> _float;

  container_contents contains;
};

//-----------------------------------------------------------------------------
//  Template specializations!
//-----------------------------------------------------------------------------
template<>
inline std::vector<double>& vector_handler::set_address<double>()
{
    contains = has_double;
    return _double;
}
//----------------------------------------------------------------------------
template<>
inline std::vector<float>& vector_handler::set_address<float>()
{
    contains = has_float;
    return _float;
}
//----------------------------------------------------------------------------
template<>
inline std::vector<int>& vector_handler::set_address<int>()
{
    contains = has_int;
    return _int;
}
//----------------------------------------------------------------------------






}

#endif
