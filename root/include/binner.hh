#ifndef BINNER__HH
#define BINNER__HH 

#include "yaml-cpp/yaml_core.hh"
#include "agile/agile_base.hh"

class binner
{
public:
	template <typename T>
	binner(const std::string &name, const std::initializer_list<T> &il);
	binner(const std::string &name, const std::vector<T> &v);
	binner(const std::string &name);

	binner();

	void set_name(const std::string &name);

	void set_bins(const std::initializer_list<T> &il);
	void set_bins(const std::vector<T> &v);


	~binner();

private:

	std::string m_name;
	std::vector<double> m_bins;
	binner m_sub_binner;

};

#endif