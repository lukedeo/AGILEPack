#include "dataset.hh"


namespace root
{

dataset::dataset(std::string filename, std::string tree_name)
: m_smart_chain(0), m_size(0), m_tree_name(tree_name), data(0) 
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
		throw std::runtime_error("you must provide a tree name to load files.");
	}	
}


dataset::dataset(const std::vector<std::string>& files, std::string tree_name)
: m_smart_chain(0), m_size(0), m_tree_name(tree_name), data(0) 
{
	if (tree_name == "")
	{
		throw std::runtime_error("you must provide a tree name to load files.");
	}
	for (auto &file : files)
	{
		m_smart_chain->add(file);
	}
	m_size = m_smart_chain->GetEntries();
}

dataset::~dataset()
{
	delete m_smart_chain;
}

void dataset::add_file(std::string filename, std::string tree_name)
{
	m_smart_chain->add(filename);
	m_size = m_smart_chain->GetEntries();
}

void dataset::set_branch(std::string branch_name, numeric_type type)
{
	if (branch_name == "")
	{
		throw std::runtime_error("cant have a NULL-named branch.");
	}
	storage.emplace_back(new number_container);

	var_traits new_trait(storage.size() - 1, type);
	traits[branch_name] = new_trait;

	feature_names.push_back(branch_name);

	switch(type)
	{
		case single_precision: m_smart_chain->set_branch(branch_name, 
			&storage.back()->set_address<float>());
		case double_precision: m_smart_chain->set_branch(branch_name, 
			&storage.back()->set_address<double>());
		case integer: m_smart_chain->set_branch(branch_name, 
			&storage.back()->set_address<int>());
	}
}

//-----------------------------------------------------------------------------
//	Element Access
//-----------------------------------------------------------------------------
std::vector<double> dataset::at(const unsigned int &idx)
{
	std::vector<double> v;
	for (auto &name : feature_names)
	{
		v.push_back(storage.at(traits[name].pos)->get_value<double>());
	}
	return std::move(v);
}
// std::vector<double> dataset::operator[](const unsigned int &idx);
// std::vector<double> dataset::operator[](const unsigned int &idx, std::string col_name = "");
// std::vector<double> dataset::operator[](std::string col_name);

//-----------------------------------------------------------------------------
//	Information
//-----------------------------------------------------------------------------

std::size_t dataset::size()
{
	return m_size;
}



}