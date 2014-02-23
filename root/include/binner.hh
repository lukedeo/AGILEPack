#ifndef BINNER__HH
#define BINNER__HH 

#include "yaml-cpp/yaml_core.hh"
#include "agile/agile_base.hh"


namespace agile 
{ 
namespace root 
{

class binner
{
public:
	binner(const std::string &name, const std::initializer_list<double> &il);

	binner(const std::string &name, const std::vector<double> &v);
	binner(const std::string &name);

	binner();

	void set_name(const std::string &name);

	void set_bins(const std::initializer_list<double> &il);
	void set_bins(const std::vector<double> &v);

	template <typename T>
	double get_bin(const std::map<std::string, T> &map);

	template <typename T>
	double get_bin(const T& var);

	~binner();

private:
	double find_bin(const double &val);
	std::string m_name;
	std::vector<double> m_bins;
	// binner m_sub_binner;

};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
binner::binner(const std::string &name, const std::initializer_list<double> &il)
: m_name(name), m_bins(il) {}
//----------------------------------------------------------------------------

binner::binner(const std::string &name, const std::vector<double> &v)
: m_name(name), m_bins(v) {}
//----------------------------------------------------------------------------
binner::binner(const std::string &name)
: m_name(name) {}
//----------------------------------------------------------------------------

binner::binner() 
: m_name("") {}
//----------------------------------------------------------------------------

void binner::set_name(const std::string &name)
{
	m_name = name;
}
//----------------------------------------------------------------------------

void binner::set_bins(const std::initializer_list<double> &il)
{
	std::vector<double> v(il);
	m_bins = std::move(v);
}
//----------------------------------------------------------------------------
void binner::set_bins(const std::vector<double> &v)
{
	m_bins = v;
}
//----------------------------------------------------------------------------

template <typename T>
double binner::get_bin(const std::map<std::string, T> &map)
{
	if (!std::is_arithmetic<T>::value)
	{
		throw std::domain_error("invalid type passed to binner.");
	}
	try
	{
		return find_bin(dynamic_cast<double>(map.at(m_name)));
	}
	catch(std::out_of_range &e)
	{
		throw std::out_of_range("no key named \'" + 
			m_name +  "\' found in map.");
	}
}
//----------------------------------------------------------------------------

template <typename T>
double binner::get_bin(const T& var)
{
	if (!std::is_arithmetic<T>::value)
	{
		throw std::domain_error("invalid type passed to binner.");
	}
	return find_bin(dynamic_cast<double>(var));
}
//----------------------------------------------------------------------------

binner::~binner()
{

}
//----------------------------------------------------------------------------


double binner::find_bin(const double &val)
{
	
}
//----------------------------------------------------------------------------




}

}

#endif