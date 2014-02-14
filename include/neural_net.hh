#ifndef NEURAL__NET__HH
#define NEURAL__NET__HH 

#include "agile/agile_base.hh"

// #include "Core.hh"
#include "dataframe/dataframe_core.hh"
#include "model_frame.hh"

namespace agile
{
    class neural_net;
}
namespace YAML
{
    template <>
    struct convert<agile::neural_net>;
}
namespace agile
{

class neural_net : public architecture
{
public:
	neural_net(int num_layers = 0);
	neural_net(std::initializer_list<int> il, problem_type type = regress);
	neural_net(const neural_net &arch);
	~neural_net();

	neural_net& operator =(const neural_net &arch);
    neural_net& operator =(neural_net &&arch);

	// void add_data(const std::string &filename);
	void add_data(const agile::dataframe &D);
	void add_data(agile::dataframe &&D);

	// void set_formula(const std::string &formula);
	// void add_predictor(const std::string &name);
	// void add_target(const std::string &name);
	void model_formula(const std::string &formula, bool scale = true, bool verbose = false);
	// agile::dataframe& data();
	// void finalize(bool scale = true);
	void from_yaml(const std::string &filename);
	void to_yaml(const std::string &filename);
	void train_unsupervised(const unsigned int &epochs, bool verbose = false, bool denoising = false, bool tantrum = false);
	void train_supervised(const unsigned int &epochs, bool verbose = false, bool tantrum = false);

	void check(bool tantrum = true);

    std::map<std::string, double> predict_map(std::map<std::string, double> v, bool scale = true);

    std::vector<std::string> get_inputs()
    {
        return predictor_order;
    }
    std::vector<std::string> get_outputs()
    {
        return target_order;
    }

    void load_scaling(const agile::scaling &scale)
    {
        // for (auto &entry : scale.mean)
        // {
        //     std::cout << "variable name: " << entry.first << ", mean: " << entry.second << std::endl;
        // }
        m_scaling = scale;
        m_model.load_scaling(scale);

    }

    void load_scaling(agile::scaling &&scale)
    {
        m_scaling = (scale);
        m_model.load_scaling(std::move(scale));
    }

    agile::scaling get_scaling()
    {
        return m_scaling;
    }

private:
	friend struct YAML::convert<neural_net>;
	std::vector<std::string> predictor_order, target_order;
	agile::matrix X, Y;
	agile::model_frame m_model;
	unsigned int n_training;
	bool m_checked;
    agile::vector m_tmp_input, m_tmp_output;

    agile::scaling m_scaling;
};

}

//-----------------------------------------------------------------------------
//  YAML Serialization Structure 
//  (look at https://code.google.com/p/yaml-cpp/wiki/Tutorial)
//-----------------------------------------------------------------------------
namespace YAML 
{

template<>
struct convert<agile::neural_net> 
{
    static Node encode(const agile::neural_net &arch)
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
        node["target_order"] = arch.target_order;

        node["scaling"] = arch.m_scaling;

        return node;
    }

    static bool decode(const Node& node, agile::neural_net &arch) 
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

        arch.predictor_order = node["input_order"].as<std::vector<std::string>>();
		arch.target_order = node["target_order"].as<std::vector<std::string>>();

        agile::scaling s = node["scaling"].as<agile::scaling>();


        arch.load_scaling(s);
        // arch.m_scaling = node["scaling"].as<agile::scaling>();

        // auto merp = node["scaling"]["means"].as<std::map<std::string, double>>();

        // for (auto &entry : merp)
        // {
        //     std::cout << "first = " << entry.first << ", second = " << entry.second << std::endl;
        //     arch.m_scaling.mean[entry.first] = entry.second;

        // }

        // // arch.m_scaling.mean = std::move(node["scaling"]["means"].as<std::map<std::string, double>>());
        // arch.m_scaling.mean =((node["scaling"]["stdevs"].as<std::map<std::string, double>>()));

        return true;
    }
};

} // end namespace yaml


#endif