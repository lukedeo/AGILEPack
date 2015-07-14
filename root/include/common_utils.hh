#ifndef COMMON__UTILS__HH
#define COMMON__UTILS__HH 

#include <cstddef> //std::size_t

namespace agile
{
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

} // end ns root
} // end ns agile

#endif