#include "Histogram.hh"
#include "Binners.hh"
#include "H5Cpp.h"
#include <vector> 

int write_test(); 
int binner_test(); 
int error_checks(); 

int main(int narg, char* argv[]) {
  error_checks();
  binner_test(); 
  write_test();
}

int error_checks() { 
  try { 
    Histogram hist({}); 
    puts("failed: zero axis error check"); 
    return -1; 
  } catch (std::invalid_argument& err) { 
    printf("passed: %s\n", err.what()); 
  }

  try { 
    Histogram hist({{"",10, 0, 1}}); 
    puts("failed: no axis name error check"); 
    return -1; 
  } catch (std::invalid_argument& err) { 
    printf("passed: %s\n", err.what()); 
  }

  Axis x; 
  x.name = "x"; 
  x.n_bins = 10; 
  x.low = 0; 
  x.high = 10; 
  try { 
    Histogram hist({x, x}); 
    puts("failed: identical axis name"); 
    return -1; 
  } catch (std::invalid_argument& err) { 
    printf("passed: %s\n", err.what()); 
  }
  
  x.high = -1; 
  try { 
    Histogram hist({x}); 
    puts("failed: bad limits"); 
    return -1; 
  } catch (std::invalid_argument& err) { 
    printf("passed: %s\n", err.what()); 
  }
  x.high = 1; 
  x.n_bins = 0; 
  try { 
    Histogram hist({x}); 
    puts("failed: bad n_bins"); 
    return -1; 
  } catch (std::invalid_argument& err) { 
    printf("passed: %s\n", err.what()); 
  }

  return 0; 
}

int binner_test() {
  LinBinner binner("x", 1, 0, 3); 
  binner.add_dimension(new LinBinner("y", 1, 0, 3)); 
  binner.add_dimension(new LinBinner("z", 1, 0, 1)); 

  std::map<std::string, double> corrd; 
  corrd["x"] = 1.3; 
  corrd["y"] = 4.5; 
  corrd["z"] = 0.3; 
  int bin = binner.get_bin(corrd); 
  if (bin != 16) {
    puts("failed: binner is broken"); 
    return -1; 
  }
  puts("passed: binner"); 
  return 0; 
}

int write_test() { 
  std::map<std::string, double> corrd; 
  corrd["x"] = 1.3; 
  corrd["y"] = 4.5; 
  corrd["z"] = 0.3; 

  std::vector<double> v_corrd; 
  v_corrd.push_back(1.3); 
  v_corrd.push_back(4.5); 
  v_corrd.push_back(0.3); 

  Axis x; 
  x.name = "x"; 
  x.n_bins = 10; 
  x.low = 0; 
  x.high = 10; 
  Axis y; 
  y.name = "y"; 
  y.n_bins = 10; 
  y.low = 0; 
  y.high = 10; 
  Axis z; 
  z.name = "z"; 
  z.n_bins = 1; 
  z.low = 0; 
  z.high = 1; 

  std::vector<Axis> dims; 
  dims.push_back(x); 
  dims.push_back(y); 
  dims.push_back(z); 

  Histogram hist(dims, hist::eat_nan); 
  Histogram copy_hist(hist); 
  hist = copy_hist; 
  hist.fill(corrd);
  corrd["y"] -= 1; 
  hist.fill(corrd); 
  hist.fill(v_corrd);
  corrd["z"] -= 0.0/0.0; 
  hist.fill(corrd); 

  H5::H5File file("test.h5", H5F_ACC_TRUNC); 
  hist.write_to(file, "testhist"); 
  try { 
    hist.write_to(file, "testhist"); 
  } catch (HistogramSaveError& err) { 
    printf("passed: %s\n", err.what()); 
  }
		  
  return 0; 
}
