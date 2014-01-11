//-----------------------------------------------------------------------------
//	architecture.hh:
//	Implementation for architecture class, responsible for joining layers
//	Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "architecture.hh"

architecture::architecture(int num_layers) 
: n_layers(num_layers), stack(num_layers)
{
}
//----------------------------------------------------------------------------
architecture::architecture(std::initializer_list<int> il, problem_type type) 
: n_layers(0), stack(0)
{
	int prev = *(il.begin());

	for (auto value = (il.begin() + 1); value != il.end(); ++value)
	{
		if (value != (il.end() - 1))
		{
			stack.emplace_back(new layer(prev, *value, sigmoid));
		}
		else
		{
			if (type == multiclass)
			{
				stack.emplace_back(new layer(prev, *value, softmax));
			}
			else if (type == classify)
			{
				stack.emplace_back(new layer(prev, *value, sigmoid));
			}
			else
			{
				stack.emplace_back(new layer(prev, *value, linear));	
			}			
		}
		++n_layers;
		prev = *value;
	}
}
//----------------------------------------------------------------------------
architecture::architecture(const architecture &arch) : n_layers(0), stack(0)
{
	for (auto &entry : arch.stack)
	{
		stack.emplace_back(entry->clone());
		++n_layers;
	}
}
//----------------------------------------------------------------------------
architecture::~architecture()
{
}
//----------------------------------------------------------------------------
void architecture::add_layer(int n_inputs, int n_outputs, layer_type type)
{
	++n_layers;
	stack.emplace_back(new layer(n_inputs, n_outputs, type));
}
//----------------------------------------------------------------------------
void architecture::resize(unsigned int size)
{
	stack.resize(size);
	n_layers = size;
}
//----------------------------------------------------------------------------
void architecture::pop_back()
{
	stack.pop_back();
	--n_layers;
}
//----------------------------------------------------------------------------
unsigned int architecture::size()
{
	return n_layers;
}
//----------------------------------------------------------------------------
void architecture::clear()
{
	n_layers = 0;
	stack.clear();
}
//----------------------------------------------------------------------------
std::unique_ptr<layer> const& architecture::at(const unsigned int &idx)
{
	if (idx >= n_layers)
	{
		throw std::out_of_range("Accessing layer in \
			'Class: architecture' beyond contained range.");
	}
	return stack.at(idx);
}
//----------------------------------------------------------------------------
agile::vector architecture::predict(const agile::vector &v)
{
	stack.at(0)->charge(v);
	for (unsigned int i = 1; i < n_layers; ++i)
	{
		stack.at(i)->charge(stack.at(i - 1)->fire());
	}
	return stack.at(n_layers - 1)->fire();
}
//----------------------------------------------------------------------------
void architecture::correct(const agile::vector &in, 
	const agile::vector &target)
{
	agile::vector error = predict(in) - target;
	// std::cout << "error: " << error << std::endl;
	int l = n_layers - 1;
	stack.at(l)->backpropagate(error);

	for (l = (n_layers - 2); l >= 0; --l)
	{
		stack.at(l)->backpropagate( stack.at(l + 1)->dump_below() );
	}
}
//----------------------------------------------------------------------------
YAML::Emitter& operator << (YAML::Emitter& out, const architecture &arch)
{
	out << YAML::BeginMap << YAML::Key << "layer_hash";
	out << YAML::Key << YAML::BeginSeq;
	unsigned char hash[20];
	char hexstring[41];
	std::string weight_string;
	for (auto &entry : arch.stack)
	{
		weight_string = agile::stringify(entry->W);
		// 10 is the length of the string
		sha1::calc(weight_string.c_str(),weight_string.size(),hash); 
		sha1::toHexString(hash, hexstring);
		out << hexstring;
	}
	out << YAML::EndSeq;
	for (auto &entry : arch.stack)
	{
		weight_string = agile::stringify(entry->W);
		// 10 is the length of the string
		sha1::calc(weight_string.c_str(),weight_string.size(),hash); 
		sha1::toHexString(hash, hexstring);
		out << YAML::Key << hexstring;
		out << YAML::Value << *(entry);
	}
	out << YAML::EndMap;
	return out;
	
}
