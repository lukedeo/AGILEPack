#ifndef CSV__READER__HH
#define CSV__READER__HH 

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace agile
{

typedef std::vector <double> record_t;
typedef std::vector <record_t> data_t;

//-----------------------------------------------------------------------------
// Let's overload the stream input operator to read a list of CSV fields (which a CSV record).
// Remember, a record is a list of doubles separated by commas ','.
std::istream& operator >> ( std::istream& ins, record_t& record );

//-----------------------------------------------------------------------------
// Let's likewise overload the stream input operator to read a list of CSV records.
// This time it is a little easier, just because we only need to worry about reading
// records, and not fields.
std::istream& operator >> ( std::istream& ins, data_t& data );

std::string trim(const std::string& str, const std::string& whitespace = " ");

template <class T>
std::string knit(std::vector<T> v, std::string knitter = ", ")
{
	std::stringstream ss;
	for (auto &item : v)
	{
		ss << item;
		if(item != v.back()) ss << knitter;
	}
	return std::move(ss.str());
}


}



#endif