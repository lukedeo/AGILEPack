#ifndef ROOT__tree_reader_HH
#define ROOT__tree_reader_HH 

#include "smart_chain.hh"
#include "numeric_handler.hh"
#include "dataframe.hh"
#include <cstddef> //std::size_t



namespace root
{


enum loading_phase { naming, access };
enum numeric_type { single_precision, double_precision, integer };

struct var_traits
{
	var_traits() {}
	var_traits(std::size_t size, numeric_type type)
	: pos(size), type(type) {}

	std::size_t pos;
	numeric_type type;
};


class tree_reader
{
public:
	tree_reader(std::string filename = "", std::string tree_name = "");

	tree_reader(const std::vector<std::string>& files, std::string tree_name = "");

	void add_file(std::string filename, std::string tree_name = "");

	void set_branch(std::string branch_name, numeric_type type);

	std::vector<std::string> get_ordered_branch_names();

	agile::dataframe get_dataframe(int entries = -1, int start = -1);



//-----------------------------------------------------------------------------
//	Element Access
//-----------------------------------------------------------------------------
	std::vector<double> at(const unsigned int &idx);
	std::vector<double> operator[](const unsigned int &idx);
	std::vector<double> operator()(const unsigned int &idx);
	double operator()(const unsigned int &idx, std::string col_name);
	// std::vector<double> operator[](std::string col_name);

//-----------------------------------------------------------------------------
//	Information
//-----------------------------------------------------------------------------

	std::size_t size();


	~tree_reader();

private:

	smart_chain *m_smart_chain;

	unsigned int m_size, m_num_cols;
	bool m_in_memory;

	std::string m_tree_name;

	std::map<std::string, var_traits> traits;

	typedef __INTERNAL::numeric_handler number_container;

	std::vector<std::unique_ptr<number_container>> storage;

	std::vector<std::string> feature_names;
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