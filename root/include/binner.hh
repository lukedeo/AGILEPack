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
	template <typename T>
	binner(const std::string &name, const std::initializer_list<T> &il);

	template <typename T>
	binner(const std::string &name, const std::vector<T> &v);
	binner(const std::string &name);

	binner();

	void set_name(const std::string &name);

	void set_bins(const std::initializer_list<T> &il);
	void set_bins(const std::vector<T> &v);

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

template <typename T>
binner::binner(const std::string &name, const std::initializer_list<T> &il)
: m_name(name)
{}

	template <typename T>
	binner(const std::string &name, const std::vector<T> &v);
	binner(const std::string &name);

	binner();

	void set_name(const std::string &name);

	void set_bins(const std::initializer_list<T> &il);
	void set_bins(const std::vector<T> &v);

	template <typename T>
	double get_bin(const std::map<std::string, T> &map);

	template <typename T>
	double get_bin(const T& var);

	~binner();

private:
	double find_bin(const double &val);



}

}

#endif