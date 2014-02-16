#ifndef BINNERS_H
#define BINNERS_H

#include <string> 
#include <map> 
#include <vector>

class IBinner
{
public: 
  virtual ~IBinner() {}; 
  virtual IBinner* clone() const = 0; 
  virtual int get_bin(const std::map<std::string, double>&) const = 0; 
  virtual int get_bin(const std::vector<double>&, size_t = 0) const = 0; 
  virtual void add_dimension(IBinner*) = 0; 
}; 

class LinBinner: public IBinner 
{
public: 
  LinBinner(std::string name, int n_bins, double low, double high); 
  LinBinner(const LinBinner&); 
  LinBinner& operator=(LinBinner); 
  friend void swap(LinBinner&, LinBinner&); 
  LinBinner* clone() const; 
  ~LinBinner(); 
  int get_bin(const std::map<std::string, double>& locator) const; 
  int get_bin(const std::vector<double>& locator, size_t offset = 0) const; 
  void add_dimension(IBinner* sub_binner); 
private: 
  int get_bin(double) const; 
  std::string m_name; 
  int m_n_bins; 
  double m_low; 
  double m_high; 
  IBinner* m_subbinner; 
};

#endif //BINNERS_H
