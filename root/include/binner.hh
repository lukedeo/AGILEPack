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
	int get_bin(const std::map<std::string, T> &map);

	template <typename T>
	int get_bin(const T& var);

	template <typename T>
	bool in_range(const std::map<std::string, T> &map);

	template <typename T>
	bool in_range(const T& var);

	~binner();

private:
	int find_bin(const double &val);
	std::string m_name;
	std::vector<double> m_bins;
	bool m_bins_present;
	bool m_name_present;
	// binner m_sub_binner;

};

//----------------------------------------------------------------------------
binner::binner(const std::string &name, 
	const std::initializer_list<double> &il)
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
int binner::get_bin(const std::map<std::string, T> &map)
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
int binner::get_bin(const T& var)
{
	if (!std::is_arithmetic<T>::value)
	{
		throw std::domain_error("invalid type passed to binner.");
	}
	return find_bin(dynamic_cast<double>(var));
}
//----------------------------------------------------------------------------

template <typename T>
bool binner::in_range(const std::map<std::string, T> &map)
{
	if (!std::is_arithmetic<T>::value)
	{
		throw std::domain_error("invalid type passed to binner.");
	}
	try
	{
		double val = dynamic_cast<double>(map.at(m_name));
		if ((val >= m_bins.front()) && (val <= m_bins.back()))
		{
			return true;
		}
		return false;
	}
	catch(std::out_of_range &e)
	{
		throw std::out_of_range("no key named \'" + 
			m_name +  "\' found in map.");
	}
}
//----------------------------------------------------------------------------

template <typename T>
bool binner::in_range(const T& var)
{
	if (!std::is_arithmetic<T>::value)
	{
		throw std::domain_error("invalid type passed to binner.");
	}
	double val = dynamic_cast<double>(var);
	if ((val >= m_bins.front()) && (val <= m_bins.back()))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------

binner::~binner()
{

}
//----------------------------------------------------------------------------


int binner::find_bin(const double &val)
{
	double lower_bound = m_bins.front();
	for (unsigned int i = 1; i < m_bins.size(); ++i)
	{
		if((val >= lower_bound) && (val < m_bins[i]))
		{
			return dynamic_cast<int>(i - 1);
		}
	}
	throw std::out_of_range("value = " + std::to_string(val) + 
		" outside of binning range.")
}
//----------------------------------------------------------------------------

}

}

#endif