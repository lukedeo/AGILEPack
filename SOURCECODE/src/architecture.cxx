#include "architecture.hh"

architecture::architecture(int num_layers) : n_layers(num_layers)
{
	stack.resize(num_layers);
}
architecture::~architecture()
{
}
void architecture::push_back(layer &L)
{
	stack.push_back(L);
	++n_layers;
}

void architecture::pop_back()
{
	stack.pop_back();
	--n_layers;
}
unsigned int architecture::size()
{
	return n_layers;
}

layer& architecture::at(unsigned int idx)
{
	return stack.at(idx);
}

agile::vector architecture::predict(const agile::vector &v)
{
	stack.at(0).charge(v);
	for (unsigned int i = 1; i < n_layers; ++i)
	{
		stack.at(i).charge(stack.at(i - 1).fire());
	}
	return stack.at(n_layers - 1).fire();
}

void architecture::correct(const agile::vector &in, const agile::vector &target)
{
	agile::vector error = predict(in) - target;
	// std::cout << "error: " << error << std::endl;
	int l = n_layers - 1;
	stack.at(l).backpropagate(error);

	for (l = (n_layers - 2); l >= 0; --l)
	{
		stack.at(l).backpropagate( stack.at(l + 1).dump_below() );
	}
}


YAML::Emitter& operator << (YAML::Emitter& out, const architecture &arch)
{
	out << YAML::BeginMap << YAML::Key << "layer_hash" << YAML::Key << YAML::BeginSeq;
	unsigned char hash[20];
	std::string weight_string;
	for (auto &entry : arch.stack)
	{
		weight_string = agile::stringify(entry.W);
		sha1::calc(weight_string.c_str(),weight_string.size(),hash); // 10 is the length of the string
		std::string hash_string(reinterpret_cast<const char*>(hash));
		out << base64_encode(reinterpret_cast<const unsigned char*>(hash_string.c_str()), hash_string.length());
	}
	out << YAML::EndSeq;
	for (auto &entry : arch.stack)
	{
		weight_string = agile::stringify(entry.W);
		sha1::calc(weight_string.c_str(),weight_string.size(),hash); // 10 is the length of the string
		std::string hash_string(reinterpret_cast<const char*>(hash));
		out << YAML::Key << base64_encode(reinterpret_cast<const unsigned char*>(hash_string.c_str()), hash_string.length());
		out << YAML::Value << entry;
	}
	out << YAML::EndMap;
	return out;
	
}
