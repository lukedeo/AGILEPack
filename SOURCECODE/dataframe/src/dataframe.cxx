#include "dataframe.hh"
#include <iostream>

namespace agile
{

std::map<std::string, std::size_t> column_names;
dataframe::dataframe(std::string filename, bool colnames)
: m_cols(0), m_rows(0)
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
                // for each field we wish to convert it to a double
                // (since we require that the CSV contains nothing but floating-point values)
                std::stringstream fs( field );

                column_names[agile::trim(fs.str())] = ctr;
                ++ctr;
            }
        }
        input >> data;
        m_rows = data.size();
        m_cols = data[0].size();
        input.close();
    }
}
dataframe::dataframe(const dataframe &D)
: column_names(D.column_names), data(D.data), m_columns_set(D.m_columns_set), m_cols(D.m_cols), m_rows(D.m_rows)
{
}
dataframe::dataframe(dataframe &&D)
: column_names(std::move(D.column_names)), data(std::move(D.data)), m_columns_set(std::move(D.m_columns_set)), m_cols(std::move(D.m_cols)), m_rows(std::move(D.m_rows))
{
}

dataframe& dataframe::operator=(const dataframe &D)
{
    column_names = (D.column_names);
    data = (D.data);
    m_columns_set = (D.m_columns_set);
    m_cols = (D.m_cols);
    m_rows = (D.m_rows);
    return *this;
}
dataframe& dataframe::operator=(dataframe &&D)
{
    column_names = std::move(D.column_names);
    data = std::move(D.data);
    m_columns_set = std::move(D.m_columns_set);
    m_cols = std::move(D.m_cols);
    m_rows = std::move(D.m_rows);
    return *this;

}

// dataframe& dataframe::operator=(const root::tree_reader &TR)
// {
//     set_column_names(TR.get_ordered_branch_names());
//     for (unsigned int i = 0; i < TR.size(); ++i)
//     {
//         push_back(TR[i]);
//     }
//     m_cols = data[0].size();
// }

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
            // for each field we wish to convert it to a double
            // (since we require that the CSV contains nothing but floating-point values)
            std::stringstream fs( field );

            column_names[agile::trim(fs.str())] = ctr;
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
std::size_t dataframe::columns()
{
    return m_cols;
}
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
void dataframe::set_column_names(std::vector<std::string> v)
{
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
}

//-----------------------------------------------------------------------------
//  Element Access
//-----------------------------------------------------------------------------

record_t& dataframe::at(const std::size_t &idx)
{
    return data.at(idx);
}
// record_t& at(const std::size_t &idx, 
//  const std::vector<std::string> &colnames);

double& dataframe::at(const std::size_t &idx, const std::string &colname)
{
    try
    {
        return data.at(idx).at(column_names.at(colname));
    }
    catch(std::out_of_range &e)
    {
        throw std::out_of_range("tried to access non-existent column \'" + colname + "\'.");
    }
    
}

record_t& dataframe::operator[](const std::size_t &idx)
{
    return data[idx];
}

//-----------------------------------------------------------------------------
//  Additions
//-----------------------------------------------------------------------------

void dataframe::push_back(const record_t &r)
{
    if (r.size() != m_cols)
    {
        throw dimension_error("vectors to be \
            push_back()'d must be the same size");
    }
    data.push_back(r);
    ++m_rows;
}
void dataframe::push_back(record_t &&r)
{
    if (r.size() != m_cols)
    {
        throw dimension_error("vectors to be \
            push_back()'d must be the same size");
    }
    data.push_back(std::move(r));
    ++m_rows;
}

void dataframe::push_back(std::initializer_list<double> il)
{
    data.emplace_back(il);
}
    // void pop_back();

void dataframe::append(const dataframe &D)
{
    if (D.m_cols != m_cols)
    {
        throw dimension_error("cannot append dataframes with \
            differing numbers of columns.");
    }
    m_rows += D.m_rows;
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
void dataframe::append(dataframe &&D)
{
    if (D.m_cols != m_cols)
    {
        throw dimension_error("cannot append dataframes with \
            differing numbers of columns.");
    }
    m_rows += std::move(D.m_rows);
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

//-----------------------------------------------------------------------------
//  iterators
//-----------------------------------------------------------------------------

    // TO DO

}


dimension_error::dimension_error(const std::string& what_arg)
: std::runtime_error(what_arg)
{
}