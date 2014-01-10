#ifndef ARCHITECTURE_HH
#define ARCHITECTURE_HH 

#include "layer.hh"
#include "autoencoder.hh"



enum problem_type { regress, classify, multiclass };


class architecture
{
public:
	architecture(int num_layers = 0);
	architecture(std::initializer_list<int> il, problem_type type = regress);
	~architecture();
	
	template <class T>
	void add_layer(const T &L)
	{
		stack.emplace_back(new T(L));
		++n_layers;
	}

	void add_layer(int n_inputs = 0, int n_outputs = 0, layer_type type = linear);
	
	template <class T>
	void add_layer(T *L)
	{
		// std::cout << "the original desc is " << L->get_desc() << "\n";
		++n_layers;
		stack.emplace_back((T*)L);
		// std::cout << "the final desc is " << stack.at(0)->get_desc() << "\n";
	}

	template <class T, class ...Args>
	void add_a_layer(Args&& ...args)
	{
		++n_layers;
	    stack.emplace_back(new T(std::forward<Args>(args)...));
	}

	std::unique_ptr<layer> const& at(const unsigned int &idx);

	
	// template <class T>
	// std::unique_ptr<T> const& at(const unsigned int &idx)
	// {
	// 	if (idx >= n_layers)
	// 	{
	// 		throw std::out_of_range("Accessing layer in 'Class: architecture' beyond contained range.");
	// 	}
	// 	return stack.at(idx);
	// }
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
				weight_string = agile::stringify(entry->W);
				sha1::calc(weight_string.c_str(),weight_string.size(),hash); // 10 is the length of the string
				sha1::toHexString(hash, hexstring);
				node["layer_hash"].push_back(static_cast<std::string>(hexstring));
				hash_vec.push_back(static_cast<std::string>(hexstring));
			}
			for (auto &entry : arch.stack)
			{
				node[hash_vec.at(tmp_ctr)] = (*entry);
			}
			return node;
		}

		static bool decode(const Node& node, architecture &arch) 
		{

			arch.clear();

			auto hash_names = node["layer_hash"];

			for (unsigned int i = 0; i < hash_names.size(); ++i)
			{
				arch.add_layer(new layer(node[hash_names[i].as<std::string>()].as<layer>()));
			}

			return true;
		}
	};
}




#endif