#ifndef DATAFRAME__HH
#define DATAFRAME__HH 

#include "csv_reader.hh"
#include <cstddef>
#include <map>
#include <utility>
#include <memory>
#include <stdexcept>

namespace agile
{

class dataframe
{
public:
	dataframe(std::string filename = "", bool colnames = false);
	// dataframe(const dataframe &D);
	// dataframe(dataframe &&D);

	// dataframe& operator=(const dataframe &D);
	// dataframe& operator=(dataframe &&D);

	~dataframe();

//-----------------------------------------------------------------------------
//	Loading and writing
//-----------------------------------------------------------------------------

	void from_csv(std::string filename, bool colnames = false);
	// void to_csv(std::string filename);
	// friend std::istream& operator >> ( std::istream& ins, dataframe &data );
	// friend std::istream& operator << ( std::ostream& ins, dataframe &data );

	// data_t& raw();
// 
//-----------------------------------------------------------------------------
//	Size / other Information
//-----------------------------------------------------------------------------
	std::size_t rows();
	std::size_t columns();
	std::vector<std::string> get_column_names();
	void set_column_names(std::vector<std::string> v);

//-----------------------------------------------------------------------------
//	Element Access
//-----------------------------------------------------------------------------

	record_t& at(const std::size_t &idx);
	// record_t& at(const std::size_t &idx, 
	// 	const std::vector<std::string> &colnames);

	double& at(const std::size_t &idx, const std::string &colname);
	record_t& operator[](const std::size_t &idx);

//-----------------------------------------------------------------------------
//	Additions
//-----------------------------------------------------------------------------

	void push_back(const record_t &r);
	void push_back(record_t &&r);
	// void pop_back();

	// dataframe& append(const dataframe &D);
	// dataframe& append(dataframe &&D);

//-----------------------------------------------------------------------------
//	iterators
//-----------------------------------------------------------------------------

	// TO DO

private:
	std::map<std::string, std::size_t> column_names;
	data_t data;
	bool m_columns_set;
	std::size_t m_cols, m_rows;

};

}

class dimension_error: public std::runtime_error 
{
public: 
    dimension_error(const std::string& what_arg); 
}; 

#endif