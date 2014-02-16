#ifndef AXIS_HH
#define AXIS_HH

#include <string>

// Axis defines each axis in an Nd histogram. 
struct Axis
{
  std::string name;		// required to be unique (and nonempty)
  int n_bins; 			// Must be >= 1
  double low; 			// Lower bound
  double high; 			// Upper bound
  std::string units; 		// Units (optional)
};

// These flags can be fed to the Histogram constructor. 
namespace hist { 

  // Histogram::fill(...) will throw a std::range_error if it gets nan
  // set this flag to simply count the number of nan. 
  const unsigned eat_nan = 1u << 0; 

  // Save a second histogram weighted by weight^2. (for statistical error)
  // Second histogram has default name <name>Wt2. 
  const unsigned wt2     = 1u << 1; 

  // Save using the "old" <axis name>_<property> attribute naming scheme. 
  // (the "new" way saves arrays of values, keyed by <property>, 
  // indexed by the axis number)
  const unsigned flat_attributes = 1u << 2; 
}

#endif 
