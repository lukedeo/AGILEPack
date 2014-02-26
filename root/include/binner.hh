#ifndef BINNER__HH
#define BINNER__HH 

#include "yaml-cpp/yaml_core.hh"
#include "agile/agile_base.hh"


namespace agile 
{ 
namespace root 
{ 
    class binner; 
}
}
//----------------------------------------------------------------------------
namespace YAML
{
    template <>
    struct convert<agile::root::binner>;
}
//----------------------------------------------------------------------------
namespace agile 
{ 
namespace root 
{

class binner
{
public:
    inline binner(const std::string &name, const std::initializer_list<double> &il);
    inline binner(const std::string &name, const std::vector<double> &v);
    inline binner(const std::string &name);

    inline binner();

    inline binner& set_name(const std::string &name);

    inline binner& set_bins(const std::initializer_list<double> &il);
    inline binner& set_bins(const std::vector<double> &v);

    inline std::vector<double> get_bins();

    template <typename T>
    inline int get_bin(const std::map<std::string, T> &map);

    template <typename T>
    inline int get_bin(const T& var);

    template <typename T>
    inline bool in_range(const std::map<std::string, T> &map);

    template <typename T>
    inline bool in_range(const T& var);

    inline ~binner();
private:
    friend struct YAML::convert<binner>;
    inline int find_bin(const double &val);
    std::string m_name;
    std::vector<double> m_bins;
    bool m_bins_present;
    bool m_name_present;
};

//----------------------------------------------------------------------------
inline binner::binner(const std::string &name, 
    const std::initializer_list<double> &il)
: m_name(name), m_bins(il) {}
//----------------------------------------------------------------------------
inline binner::binner(const std::string &name, const std::vector<double> &v)
: m_name(name), m_bins(v) {}
//----------------------------------------------------------------------------
inline binner::binner(const std::string &name)
: m_name(name) {}
//----------------------------------------------------------------------------
inline binner::binner() 
: m_name("") {}
//----------------------------------------------------------------------------

inline binner& binner::set_name(const std::string &name)
{
    m_name = name;
    return *this;
}
//----------------------------------------------------------------------------

inline std::vector<double> binner::get_bins()
{
    return std::move(m_bins);
}
//----------------------------------------------------------------------------
inline binner& binner::set_bins(const std::initializer_list<double> &il)
{
    std::vector<double> v(il);
    m_bins = std::move(v);
    return *this;
}
//----------------------------------------------------------------------------
inline binner& binner::set_bins(const std::vector<double> &v)
{
    m_bins = v;
    return *this;
}
//----------------------------------------------------------------------------

template <typename T>
inline int binner::get_bin(const std::map<std::string, T> &map)
{
    if (!std::is_arithmetic<T>::value)
    {
        throw std::domain_error("invalid type passed to binner.");
    }
    try
    {
        return find_bin(static_cast<double>(map.at(m_name)));
    }
    catch(std::out_of_range &e)
    {
        throw std::out_of_range("no key named \'" + 
            m_name +  "\' found in map.");
    }
}
//----------------------------------------------------------------------------

template <typename T>
inline int binner::get_bin(const T& var)
{
    if (!std::is_arithmetic<T>::value)
    {
        throw std::domain_error("invalid type passed to binner.");
    }
    return find_bin(static_cast<double>(var));
}
//----------------------------------------------------------------------------

template <typename T>
inline bool binner::in_range(const std::map<std::string, T> &map)
{
    if (!std::is_arithmetic<T>::value)
    {
        throw std::domain_error("invalid type passed to binner.");
    }
    try
    {
        double val = static_cast<double>(map.at(m_name));
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
inline bool binner::in_range(const T& var)
{
    if (!std::is_arithmetic<T>::value)
    {
        throw std::domain_error("invalid type passed to binner.");
    }
    double val = static_cast<double>(var);
    if ((val >= m_bins.front()) && (val <= m_bins.back()))
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------

inline binner::~binner()
{

}
//----------------------------------------------------------------------------


inline int binner::find_bin(const double &val)
{
    double lower_bound = m_bins.front();
    for (unsigned int i = 1; i < m_bins.size(); ++i)
    {
        if((val >= lower_bound) && (val < m_bins[i]))
        {
            return static_cast<int>(i - 1);
        }
    }
    throw std::out_of_range("value = " + std::to_string(val) + 
        " outside of binning range.");
}
//----------------------------------------------------------------------------

}

}

namespace YAML 
{

template<>
struct convert<agile::root::binner> 
{
    static Node encode(const agile::root::binner &b)
    {
        Node node;
        node[b.m_name] = b.m_bins;
        return node;
    }

    static bool decode(const Node& node, agile::root::binner &b) 
    {
        auto tmp_map = node.as<std::map<std::string, std::vector<double>>>();
        for (auto &entry : tmp_map)
        {
            b.m_bins = entry.second;
            b.m_name = entry.first;
        }
        return true;
    }
};


}

#endif