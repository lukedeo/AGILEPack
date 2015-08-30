//-----------------------------------------------------------------------------
//  csv_reader.hh:
//  Header for CSV manip, and some string utilities
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef CSV__READER__HH
#define CSV__READER__HH 

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

namespace agile
{

//----------------------------------------------------------------------------
typedef std::vector <double> record_t;
typedef std::vector <record_t> data_t;

// stream operators for csv
//----------------------------------------------------------------------------
std::istream& operator >> ( std::istream& ins, record_t& record );
std::istream& operator >> ( std::istream& ins, data_t& data );

// Some string manip utility functions
//----------------------------------------------------------------------------
std::string trim(const std::string& str, const std::string& whitespace = " ");

template <class T>
inline std::string knit(std::vector<T> v, std::string knitter = ",")
{
    std::stringstream ss;
    for (unsigned int i = 0; i < v.size() - 1; ++i)
    {
        ss << v.at(i) << knitter;
    }
    ss << v.back();
    return std::move(ss.str());
}
inline std::string no_spaces(std::string str)
{
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
    str.erase(end_pos, str.end());
    return str;
}
inline std::string no_quotes(std::string str)
{
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), '"');
    str.erase(end_pos, str.end());
    return str;
}
//----------------------------------------------------------------------------

}



#endif
