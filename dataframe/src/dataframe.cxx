//-----------------------------------------------------------------------------
//  dataframe.cxx:
//  Implementation for dataset handling, almost in the vain of the R language
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "include/dataframe.hh"
#include <iostream>

namespace agile
{
//-----------------------------------------------------------------------------
//  Constructors, assignment, etc.
//-----------------------------------------------------------------------------
dataframe::dataframe(std::string filename, bool colnames)
: m_scaled(false), m_cols(0), m_rows(0)
{
    if (filename != "")
    {
        std::ifstream input;
        input.open(filename);
        if (colnames)
        {
            m_columns_set = true;
            std::string line;
            std::getline( input, line );

            std::stringstream ss( line );
            std::string field;
            std::size_t ctr = 0;
            while (getline( ss, field, ',' ))
            {
                std::stringstream fs( field );

                column_names[agile::no_quotes(agile::trim(fs.str()))] = ctr;
                ++ctr;
            }
        }
        input >> data;
        m_rows = data.size();
        m_cols = data[0].size();
        input.close();
    }
    else
    {
        m_columns_set = false;
    }
}
dataframe::dataframe(const dataframe &D) 
: column_names(D.column_names), data(D.data), m_columns_set(D.m_columns_set), 
m_scaled(false), m_cols(D.m_cols), m_rows(D.m_rows)
{
}
//----------------------------------------------------------------------------
dataframe::dataframe(dataframe &&D)
: column_names(std::move(D.column_names)), data(std::move(D.data)),
 m_columns_set(std::move(D.m_columns_set)), m_scaled(false), 
 m_cols(std::move(D.m_cols)), m_rows(std::move(D.m_rows))
{
}
//----------------------------------------------------------------------------
dataframe& dataframe::operator=(const dataframe &D)
{
    column_names = (D.column_names);
    data = (D.data);
    m_columns_set = (D.m_columns_set);
    m_cols = (D.m_cols);
    m_rows = (D.m_rows);
    m_scaled = (D.m_scaled);
    return *this;
}
//----------------------------------------------------------------------------
dataframe& dataframe::operator=(dataframe &&D)
{
    column_names = std::move(D.column_names);
    data = std::move(D.data);
    m_columns_set = std::move(D.m_columns_set);
    m_cols = std::move(D.m_cols);
    m_rows = std::move(D.m_rows);
    m_scaled = std::move(D.m_scaled);
    return *this;

}
//----------------------------------------------------------------------------
dataframe::~dataframe()
{
}

//-----------------------------------------------------------------------------
//  Loading and writing
//-----------------------------------------------------------------------------
void dataframe::from_csv(std::string filename, bool colnames)
{
    std::ifstream input(filename);
    data.clear();
    if (colnames)
    {
        m_columns_set = true;
        std::string line;
        std::getline( input, line );

        std::stringstream ss( line );
        std::string field;
        std::size_t ctr = 0;
        while (getline( ss, field, ',' ))
        {
            std::stringstream fs( field );
            column_names[agile::no_quotes(agile::trim(fs.str()))] = ctr;
            ++ctr;
        }
    }
    else
    {
        m_columns_set = false;
    }
    input >> data;
    m_rows = data.size();
    m_cols = data[0].size();
    input.close();
}
//----------------------------------------------------------------------------
void dataframe::to_csv(std::string filename, bool write_colnames)
{
    std::ofstream output(filename);
    if (write_colnames && m_columns_set)
    {
        output << knit(get_column_names()) << "\n";
    }
    for (auto &row : data)
    {
        output << knit(row) << "\n";
    }
}
//----------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& os, dataframe &data )
{
    if (data.m_columns_set)
    {
        os << knit(data.get_column_names()) << "\n";
    }
    for (auto &row : data.data)
    {
        os << knit(row) << "\n";
    }
    return os;
}
//----------------------------------------------------------------------------
data_t& dataframe::raw()
{
    return data;
}



//-----------------------------------------------------------------------------
//  Size / other Information
//-----------------------------------------------------------------------------
std::size_t dataframe::rows()
{
    return m_rows;
}
//----------------------------------------------------------------------------
std::size_t dataframe::columns()
{
    return m_cols;
}
//----------------------------------------------------------------------------
std::vector<std::string> dataframe::get_column_names()
{
    if (!m_columns_set)
    {
        throw std::runtime_error("column names not set.");
    }
    std::vector<std::string> v(m_cols);
    for (auto &var : column_names)
    {
        v.at(var.second) = agile::trim(var.first);
    }
    return std::move(v);
}

std::size_t dataframe::get_column_idx(const std::string &name)
{
    try
    {
        return column_names.at(name);    
    }
    catch(std::out_of_range &e)
    {
        throw std::out_of_range("no variable named \'" + name + "\' present.");
    }   
}
//----------------------------------------------------------------------------
void dataframe::set_column_names(std::vector<std::string> v)
{
    // std::cout << "callingit \n";
    if (m_columns_set)
    {
        throw std::runtime_error("column names already set.");
    }
    std::size_t ctr = 0;
    for (auto &e : v)
    {
        column_names[agile::trim(e)] = ctr;
        ++ctr;
    }
    m_columns_set = true;
    m_cols = v.size();
}

//-----------------------------------------------------------------------------
//  Element Access
//-----------------------------------------------------------------------------

record_t& dataframe::at(const std::size_t &idx)
{
    return data.at(idx);
}
//----------------------------------------------------------------------------
double& dataframe::at(const std::size_t &idx, const std::string &colname)
{
    try
    {
        return data.at(idx).at(column_names.at(colname));
    }
    catch(std::out_of_range &e)
    {
        std::string wha(
            "tried to access non-existent column \'" + colname + "\'.");
        throw std::out_of_range(wha);
    }
    
}
std::map<std::string, double> dataframe::at(const std::size_t &idx, const std::vector<std::string> cols)
{
    std::map<std::string, double> retval;
    for (auto &colname : cols)
    {
        try
        {
            retval[colname] = data.at(idx).at(column_names.at(colname));
        }
        catch(std::out_of_range &e)
        {
            std::string wha(
                "tried to access non-existent column \'" + colname + "\'.");
            throw std::out_of_range(wha);
        }
    }
    return retval;
}
//----------------------------------------------------------------------------
record_t& dataframe::operator[](const std::size_t &idx)
{
    return data[idx];
}

//-----------------------------------------------------------------------------
//  Additions
//-----------------------------------------------------------------------------

void dataframe::push_back(const record_t &r)
{
    if ((r.size() != m_cols) && (m_cols > 0))
    {
        std::string wha("vectors to be push_back()'d must be the same size");
        throw dimension_error(wha);
    }
    data.push_back(r);
    ++m_rows;
}
//----------------------------------------------------------------------------
void dataframe::push_back(record_t &&r)
{
    if ((r.size() != m_cols) && (m_cols > 0))
    {
        std::string wha("vectors to be push_back()'d must be the same size");
        throw dimension_error(wha);
    }
    // for (auto &entry : r)
    // {
    //     std::cout << entry << "   ";
    // }
    // std::cout << std::endl;
    data.push_back(std::move(r));
    ++m_rows;
}
//----------------------------------------------------------------------------
void dataframe::push_back(std::initializer_list<double> il)
{
    data.emplace_back(il);
}
    // void pop_back();
//----------------------------------------------------------------------------
void dataframe::append(const dataframe &D)
{
    if ((D.m_cols != m_cols) && (m_cols != 0))
    {
        std::string wha(
            "cannot append dataframes with differing numbers of columns.");
        
        throw dimension_error(wha);
    }
    if (m_cols == 0)
    {
        m_cols = D.m_cols;
    }
    for (auto &row : D.data)
    {
        push_back(row);
    }
    if ((!m_columns_set) && D.m_columns_set)
    {
        m_columns_set = true;
        column_names = D.column_names;
    }
}
//----------------------------------------------------------------------------
void dataframe::append(dataframe &&D)
{
    if ((D.m_cols != m_cols) && (m_cols != 0))
    {
        std::string wha(
            "cannot append dataframes with differing numbers of columns.");
        
        throw dimension_error(wha);
    }
    if (m_cols == 0)
    {
        m_cols = std::move(D.m_cols);
    }
    for (auto &row : D.data)
    {
        push_back(std::move(row));
    }
    if ((!m_columns_set) && D.m_columns_set)
    {
        m_columns_set = std::move(D.m_columns_set);
        column_names = std::move(D.column_names);
    }
}

}//end ns agile

dimension_error::dimension_error(const std::string& what_arg)
: std::runtime_error(what_arg)
{
}

