#ifndef NEURAL__NET__HH
#define NEURAL__NET__HH 

#include "architecture.hh"
#include "dataframe.hh"
#include "model_frame.hh"

class neural_net;

namespace YAML
{
    template <>
    struct convert<neural_net>;
}
	
class neural_net : public architecture
{
public:
	neural_net(int num_layers = 0);
	neural_net(std::initializer_list<int> il, problem_type type = regress);
	neural_net(const neural_net &arch);
	~neural_net();

	neural_net& operator =(const neural_net &arch);

	// void add_data(const std::string &filename);
	void add_data(const agile::dataframe &D);
	void add_data(agile::dataframe &&D);

	// void set_formula(const std::string &formula);
	// void add_predictor(const std::string &name);
	// void add_target(const std::string &name);
	void model_formula(const std::string &formula, bool scale = true);
	// agile::dataframe& data();
	// void finalize(bool scale = true);
	// void load_network(const std::string &formula);
	// void save_network(const std::string &formula);
	// void train_unsupervised(const unsigned int &iters);
	// void train_supervised(const unsigned int &iters);

private:
	friend struct YAML::convert<neural_net>;
	std::vector<std::string> predictor_order, target_order;
	agile::matrix X, Y;
	agile::model_frame m_model;
};




//-----------------------------------------------------------------------------
//  YAML Serialization Structure 
//  (look at https://code.google.com/p/yaml-cpp/wiki/Tutorial)
//-----------------------------------------------------------------------------
namespace YAML 
{

template<>
struct convert<neural_net> 
{
    static Node encode(const neural_net &arch)
    {
        Node node;
        for (unsigned int i = 0; i < arch.stack.size(); ++i)
        {
            std::string layer_index = "layer_" + std::to_string(i);
            node["layer_access"].push_back(layer_index);
            if (arch.stack.at(i)->get_paradigm() == agile::types::Autoencoder)
            {
                node[layer_index] = *(dynamic_cast<autoencoder*>(
                    arch.stack.at(i).get()));
            }
            else
            {
                node[layer_index] = *(arch.stack.at(i).get());
            }
        }
        node["input_order"] = arch.predictor_order;
        node["target_order"] = arch.predictor_order;
        return node;
    }

    static bool decode(const Node& node, neural_net &arch) 
    {

        arch.clear();

        auto layer_names = node["layer_access"];

        for (unsigned int i = 0; i < layer_names.size(); ++i)
        {
            std::string layer_id = layer_names[i].as<std::string>(); 
            std::string class_type = node[layer_id]["class"].as<std::string>();

            if (class_type == "autoencoder")
            {
                arch.emplace_back(
                    new autoencoder(node[layer_id].as<autoencoder>()));
            }
            else if (class_type == "layer")
            {
                arch.emplace_back(new layer(node[layer_id].as<layer>()));
            }
            else
            {
                throw std::logic_error(
                    "class " + class_type + " not recognized.");
            }

                     
        }

        return true;
    }
};
}

#endif