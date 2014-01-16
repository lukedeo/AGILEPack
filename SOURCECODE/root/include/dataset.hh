#ifndef ROOT__DATASET_HH
#define ROOT__DATASET_HH 

#include "basedefs.hh"
#include "smart_chain.hh"
#include <cstddef> //std::size_t



namespace root
{



typedef std::vector<std::vector<double>> data_frame;
enum loading_phase { naming, access };
enum numeric_type { single_precision, double_precision, integer };

struct var_traits
{
	std::size_t pos;
	numeric_type type;
};


class dataset
{
public:
	dataset(std::string filename = "", std::string tree_name = "");

	dataset(const std::vector<std::string>& files, std::string tree_name = "");

	void add_file(std::string filename, std::string tree_name = "");

	void set_branch(std::string branch_name = "", numeric_type);

//-----------------------------------------------------------------------------
//	Element Access
//-----------------------------------------------------------------------------
	std::vector<double> at(const unsigned int &idx);
	std::vector<double> operator[](const unsigned int &idx);
	std::vector<double> operator[](const unsigned int &idx, std::string col_name = "");
	std::vector<double> operator[](std::string col_name);

//-----------------------------------------------------------------------------
//	Information
//-----------------------------------------------------------------------------

	std::size_t size();


	~dataset();

private:

	smart_chain *m_smart_chain;

	int m_size, m_num_cols;
	bool m_in_memory;

	std::map<std::string, var_traits> traits;

	data_frame *data;
};

}

#endif

/*
TO DO:
--------------
* make iterators
* make variable name access
* make option to store in memory




*/