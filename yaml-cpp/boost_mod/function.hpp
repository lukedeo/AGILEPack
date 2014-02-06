// Boost.Function library

//  Copyright Douglas Gregor 2001-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/function

// William Kempf, Jesse Jones and Karl Nelson were all very helpful in the
// design of this library.

#include <functional> // unary_function, binary_function

#include "yaml-cpp/boost_mod/preprocessor/iterate.hpp"
#include "yaml-cpp/boost_mod/detail/workaround.hpp"

#ifndef BOOST_FUNCTION_MAX_ARGS
#  define BOOST_FUNCTION_MAX_ARGS 10
#endif // BOOST_FUNCTION_MAX_ARGS

// Include the prologue here so that the use of file-level iteration
// in anything that may be included by function_template.hpp doesn't break
#include "yaml-cpp/boost_mod/function/detail/prologue.hpp"

// Older Visual Age C++ version do not handle the file iteration well
#if BOOST_WORKAROUND(__IBMCPP__, >= 500) && BOOST_WORKAROUND(__IBMCPP__, < 800)
#  if BOOST_FUNCTION_MAX_ARGS >= 0
#    include "yaml-cpp/boost_mod/function/function0.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 1
#    include "yaml-cpp/boost_mod/function/function1.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 2
#    include "yaml-cpp/boost_mod/function/function2.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 3
#    include "yaml-cpp/boost_mod/function/function3.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 4
#    include "yaml-cpp/boost_mod/function/function4.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 5
#    include "yaml-cpp/boost_mod/function/function5.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 6
#    include "yaml-cpp/boost_mod/function/function6.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 7
#    include "yaml-cpp/boost_mod/function/function7.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 8
#    include "yaml-cpp/boost_mod/function/function8.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 9
#    include "yaml-cpp/boost_mod/function/function9.hpp"
#  endif
#  if BOOST_FUNCTION_MAX_ARGS >= 10
#    include "yaml-cpp/boost_mod/function/function10.hpp"
#  endif
#else
// What is the '3' for?
#  define BOOST_PP_ITERATION_PARAMS_1 (3,(0,BOOST_FUNCTION_MAX_ARGS,"yaml-cpp/boost_mod/function/detail/function_iterate.hpp"))
#  include BOOST_PP_ITERATE()
#  undef BOOST_PP_ITERATION_PARAMS_1
#endif
