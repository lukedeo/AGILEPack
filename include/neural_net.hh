#ifndef NEURAL__NET__HH
#define NEURAL__NET__HH 

#include "agile/agile_base.hh"
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
//----------------------------------------------------------------------------
class neural_net : public architecture
{
public:
    neural_net(int num_layers = 0);
    neural_net(std::initializer_list<int> il, problem_type type = regress);
    neural_net(const neural_net &arch);
    ~neural_net();

    neural_net& operator =(const neural_net &arch);
    neural_net& operator =(neural_net &&arch);

    void add_data(const agile::dataframe &D);
    void add_data(agile::dataframe &&D);

    void model_formula(const std::string &formula, 
        bool scale = true, bool verbose = false);

    void from_yaml(const std::string &filename);
    void to_yaml(const std::string &filename);
    void to_yaml(const std::string &filename, 
        const std::map<std::string, std::string> &types);

    void to_yaml(const std::string &filename, 
        const std::map<std::string, std::string> &types,
        const std::map<std::string, std::vector<double>> &binning);

    // template <class T>
    // void to_yaml(const std::string &filename, const T &R);

    void train_unsupervised(const unsigned int &epochs, bool verbose = false, 
        bool denoising = false, bool tantrum = false);

    void train_supervised(const unsigned int &epochs, bool verbose = false, 
        bool tantrum = false);

    void check(bool tantrum = true);

    std::map<std::string, double> predict_map(std::map<std::string, double> v, 
        bool scale = true);

    std::vector<std::string> get_inputs();
    std::vector<std::string> get_outputs();

    void load_scaling(const agile::scaling &scale);
    void load_scaling(agile::scaling &&scale);
    agile::scaling get_scaling();

    void load_model_frame_config(const agile::model_frame &m);

// Overrides
//----------------------------------------------------------------------------
    void set_X(const agile::matrix &A, bool tantrum = 1);
    void set_Y(const agile::matrix &A, bool tantrum = 1);

private:
    void internal_train_unsupervised_weighted(const unsigned int &epochs, 
        bool verbose = false, bool denoising = false, bool tantrum = false);

    void internal_train_supervised_weighted(const unsigned int &epochs, 
        bool verbose = false, bool tantrum = false);



    void internal_train_unsupervised(const unsigned int &epochs, 
        bool verbose = false, bool denoising = false, bool tantrum = false);

    void internal_train_supervised(const unsigned int &epochs, 
        bool verbose = false, bool tantrum = false);


    friend struct YAML::convert<neural_net>;
    std::vector<std::string> predictor_order, target_order;

    agile::matrix X, Y, pattern_weights;

    agile::model_frame m_model;
    unsigned int n_training;
    bool m_checked, m_weighted;
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

        // if(arch.m_weighted())
        // {
        //     node["weighting"] = 
        // }

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

        return true;
    }
};

} // end namespace yaml


#endif