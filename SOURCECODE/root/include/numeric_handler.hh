#include "basedefs.hh"



namespace __INTERNAL
{

class numeric_handler //union (one can be set at a time)
{
public:
	numeric_handler() = default;
	~numeric_handler() = default;

	template <typename T>
	T get_value();

	template <typename T>
	T& set_address()
	{
		throw std::runtime_error("unrecognized type passed to numeric_handler.");
	}
	enum container_contents { has_double, has_int, has_float };
private:
	
	double _double = 0;
	int _int = 0;
	float _float = 0;

	container_contents contains;

};


template<>
double& numeric_handler::set_address<double>()
{
	contains = has_double;
	return _double;
}

template<>
float& numeric_handler::set_address<float>()
{
	contains = has_float;
	return _float;
}

template<>
int& numeric_handler::set_address<int>()
{
	contains = has_int;
	return _int;
}


template <typename T>
T numeric_handler::get_value()
{
	switch(contains)
	{
		case has_double: return static_cast<T>(_double);
		case has_int: return static_cast<T>(_int);
		case has_float: return static_cast<T>(_float);
	}
	throw std::runtime_error("something weird happened in the numeric_handler.");
}




}