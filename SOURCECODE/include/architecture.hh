#ifndef ARCHITECTURE_HH
#define ARCHITECTURE_HH 

#include "layer.hh"

// namespace agile
// {
// 	enum problem_type { regression, classification, softmax_classification };
// }

class architecture
{
public:
	architecture(int num_layers = 0);
	~architecture();
	void push_back(layer &L);
	layer& at(unsigned int idx);
	void pop_back();
	void clear();
	unsigned int size();
	void resize(unsigned int size);
	agile::vector predict(const agile::vector &v);
	void correct(const agile::vector &in, const agile::vector &target);
	// void to_yaml(std::string filename = "file.yaml");
	friend YAML::Emitter& operator << (YAML::Emitter& out, const architecture &arch);

// private:
	unsigned int n_layers;
	agile::layer_stack stack;
};

namespace YAML 
{
	template<>
	struct convert<architecture> 
	{
		static Node encode(const architecture &arch)
		{
			Node node;
			unsigned char hash[20];
			char hexstring[41];
			std::string weight_string;
			std::vector<std::string> hash_vec;

			unsigned int tmp_ctr = 0;

			for (auto &entry : arch.stack)
			{
				weight_string = agile::stringify(entry.W);
				sha1::calc(weight_string.c_str(),weight_string.size(),hash); // 10 is the length of the string
				sha1::toHexString(hash, hexstring);
				node["layer_hash"].push_back(static_cast<std::string>(hexstring));
				hash_vec.push_back(static_cast<std::string>(hexstring));
			}
			for (auto &entry : arch.stack)
			{
				node[hash_vec.at(tmp_ctr)] = entry;
			}
			return node;
		}

		static bool decode(const Node& node, architecture &arch) 
		{

			arch.clear();

			auto hash_names = node["layer_hash"];

			arch.resize(hash_names.size());

			for (unsigned int i = 0; i < hash_names.size(); ++i)
			{
				arch.at(i) = node[hash_names[i].as<std::string>()].as<layer>();
			}

			return true;
		}
	};
}




#endif