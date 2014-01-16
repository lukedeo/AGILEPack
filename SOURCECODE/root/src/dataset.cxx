#include "dataset.hh"


namespace root
{



typedef std::vector<std::vector<double>> data_frame;
enum loading_phase { naming, access };
enum numeric_type { single_precision, double_precision, integer };

struct var_traits
{
	std::size_t pos;
	numeric_type type;
};


dataset::dataset(std::string filename, std::string tree_name)
: m_chain(0), m_entry(0), data(0)
{
	if(tree_name != "")
	{
		m_smart_chain = new smart_chain(tree_name);
		if (filename != "")
		{
			m_smart_chain->add(filename);
			m_size = m_smart_chain->GetEntries();
		}
	}
	else if (filename != "")
	{
		throw std::runtime_error("you must provide a tree name to load files.")
	}	
}


dataset::dataset(const std::vector<std::string>& files, std::string tree_name = "");
dataset::~dataset();
{
	delete m_smart_chain;
}

void dataset::add_file(std::string filename, std::string tree_name = "");

void dataset::set_branch(std::string branch_name = "", numeric_type);

//-----------------------------------------------------------------------------
//	Element Access
//-----------------------------------------------------------------------------
std::vector<double> dataset::at(const unsigned int &idx);
std::vector<double> dataset::operator[](const unsigned int &idx);
std::vector<double> dataset::operator[](const unsigned int &idx, std::string col_name = "");
std::vector<double> dataset::operator[](std::string col_name);

//-----------------------------------------------------------------------------
//	Information
//-----------------------------------------------------------------------------

std::size_t dataset::size();





smart_chain *m_smart_chain;

int m_size, m_num_cols;
bool m_in_memory;

std::map<std::string, var_traits> traits;

data_frame *data;


}