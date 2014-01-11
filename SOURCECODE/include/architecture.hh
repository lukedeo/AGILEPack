//-----------------------------------------------------------------------------
//  architecture.hh:
//  Header for architecture class, responsible for joining layers
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef ARCHITECTURE_HH
#define ARCHITECTURE_HH 

#include "layer.hh"
#include "autoencoder.hh"

//-----------------------------------------------------------------------------
//  Generic enum for initializer list construction
//-----------------------------------------------------------------------------

enum problem_type { regress, classify, multiclass };

//-----------------------------------------------------------------------------
//  Hacky things for yaml-cpp friendship
//-----------------------------------------------------------------------------
class architecture;

namespace YAML
{
    template <>
    struct convert<architecture>;
}

//-----------------------------------------------------------------------------
//  architecture class implementation
//-----------------------------------------------------------------------------
class architecture
{
public:
//-----------------------------------------------------------------------------
//  Construction, destruction, and copying
//-----------------------------------------------------------------------------
    architecture(int num_layers = 0);
    architecture(std::initializer_list<int> il, problem_type type = regress);
    ~architecture();
    architecture(const architecture &arch);

//-----------------------------------------------------------------------------
//  Layer Manipulation and access methods
//----------------------------------------------------------------------------- 
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
        ++n_layers;
        stack.emplace_back((T*)L);
    }

    template <class T, class ...Args>
    void add_layer(Args&& ...args)
    {
        ++n_layers;
        stack.emplace_back(new T(std::forward<Args>(args)...));
    }

    std::unique_ptr<layer> const& at(const unsigned int &idx);
    void pop_back();
    void clear();
    unsigned int size();
    void resize(unsigned int size);

//-----------------------------------------------------------------------------
//  Global parameter setting
//-----------------------------------------------------------------------------

    void set_batch_size(int size)
    {
        if (n_layers < 1)
        {
            throw std::logic_error("can't set batch size\
                -- network has no layers.");
        }
        for (auto &layer : stack)
        {
            layer->set_batch_size(size);
        }
    }
    void set_learning(const numeric &value)
    {
        if (n_layers < 1)
        {
            throw std::logic_error("can't set learning rate\
                -- network has no layers.");
        }
        for (auto &layer : stack)
        {
            layer->set_learning(value);
        }
    }
    void set_momentum(const numeric &value)
    {
        if (n_layers < 1)
        {
            throw std::logic_error("can't set momentum\
                -- network has no layers.");
        }
        for (auto &layer : stack)
        {
            layer->set_momentum(value);
        }
    }
    void set_regularizer(const numeric &value)
    {
        if (n_layers < 1)
        {
            throw std::logic_error("can't set regularizer\
                -- network has no layers.");
        }
        for (auto &layer : stack)
        {
            layer->set_regularizer(value);
        }
    }

//-----------------------------------------------------------------------------
//  Prediction and training methods
//-----------------------------------------------------------------------------
    agile::vector predict(const agile::vector &v);
    void correct(const agile::vector &in, const agile::vector &target);

//-----------------------------------------------------------------------------
//  Access for YAML serialization
//-----------------------------------------------------------------------------
    
    friend YAML::Emitter& operator << (YAML::Emitter& out, 
        const architecture &arch);

    friend struct YAML::convert<architecture>;

protected:
//-----------------------------------------------------------------------------
//  Protected Members
//-----------------------------------------------------------------------------
    unsigned int n_layers;    // number of network layers
    agile::layer_stack stack; // the stack of layers
};

//-----------------------------------------------------------------------------
//  YAML Serialization Structure 
//  (look at https://code.google.com/p/yaml-cpp/wiki/Tutorial)
//-----------------------------------------------------------------------------
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
                // 10 is the length of the string
                sha1::calc(weight_string.c_str(),weight_string.size(),hash);
                sha1::toHexString(hash, hexstring);
                node["layer_hash"].push_back(
                static_cast<std::string>(hexstring));

                hash_vec.push_back(static_cast<std::string>(hexstring));
            }
            for (auto &entry : arch.stack)
            {
                node[hash_vec.at(tmp_ctr)] = (*entry);
                ++tmp_ctr;
            }
            return node;
        }

        static bool decode(const Node& node, architecture &arch) 
        {

            arch.clear();

            auto hash_names = node["layer_hash"];

            for (unsigned int i = 0; i < hash_names.size(); ++i)
            {
                arch.add_layer(new layer(
                node[hash_names[i].as<std::string>()].as<layer>()));
            }

            return true;
        }
    };
}




#endif