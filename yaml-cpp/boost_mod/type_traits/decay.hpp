//  (C) Copyright John Maddock & Thorsten Ottosen 2005.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef BOOST_TT_DECAY_HPP_INCLUDED
#define BOOST_TT_DECAY_HPP_INCLUDED

#include "yaml-cpp/boost_mod/type_traits/config.hpp"
#include "yaml-cpp/boost_mod/type_traits/is_array.hpp"
#include "yaml-cpp/boost_mod/type_traits/is_function.hpp"
#include "yaml-cpp/boost_mod/type_traits/remove_bounds.hpp"
#include "yaml-cpp/boost_mod/type_traits/add_pointer.hpp"
#include "yaml-cpp/boost_mod/type_traits/remove_reference.hpp"
#include "yaml-cpp/boost_mod/mpl/eval_if.hpp"
#include "yaml-cpp/boost_mod/mpl/identity.hpp"

namespace boost 
{

    template< class T >
    struct decay
    {
    private:
        typedef BOOST_DEDUCED_TYPENAME remove_reference<T>::type Ty;
    public:
        typedef BOOST_DEDUCED_TYPENAME mpl::eval_if< 
            is_array<Ty>,
            mpl::identity<BOOST_DEDUCED_TYPENAME remove_bounds<Ty>::type*>,
            BOOST_DEDUCED_TYPENAME mpl::eval_if< 
                is_function<Ty>,
                add_pointer<Ty>,
                mpl::identity<Ty>
            >
        >::type type;
    };
    
} // namespace boost


#endif // BOOST_TT_DECAY_HPP_INCLUDED
