//-----------------------------------------------------------------------------
//  layer.hh:
//  Header for layer class, responsible for s(Wx+b) calculation
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef LAYER_HH
#define LAYER_HH

#include <iostream>
#include "agile/include/basedefs.hh"
#include "agile/include/activation.hh"

//-----------------------------------------------------------------------------
//  A simple layer factory for this and all derived versions
//-----------------------------------------------------------------------------
template <class T, class ...Args>
typename std::enable_if<!std::is_array<T>::value, T*>::type
layer_factory(Args&& ...args)
{
    return dynamic_cast<T*>(new T(std::forward<Args>(args)...));
}

//-----------------------------------------------------------------------------
//  Hacky things for yaml-cpp friendship
//-----------------------------------------------------------------------------

class layer;
class architecture;

namespace YAML
{
    template <>
    struct convert<layer>;
}

namespace YAML
{
    template <>
    struct convert<architecture>;
}

namespace agile
{
    class neural_net;
}

//-----------------------------------------------------------------------------
//  layer class implementation
//-----------------------------------------------------------------------------
class layer
{
public:
//-----------------------------------------------------------------------------
//  Construction, destruction, and copying
//-----------------------------------------------------------------------------
    explicit layer(int n_inputs = 0, 
        int n_outputs = 0, layer_type type = linear);

    layer(const layer &L);
    layer(layer &&L);
    layer(layer *L);
    virtual layer& operator= (const layer &L);
    virtual layer& operator= (layer &&L);
    virtual ~layer() = default;

    void construct(int n_inputs, int n_outputs, layer_type type);

    virtual agile::types::paradigm get_paradigm()
    {
        return m_paradigm;
    }

    virtual void resize_input(int n_inputs);
    virtual void resize_output(int n_outputs);

    int num_inputs()
    {
        return m_inputs;
    }
    int num_outputs()
    {
        return m_outputs;
    }
//-----------------------------------------------------------------------------
//  Training methods for inter-layer communication
//-----------------------------------------------------------------------------
    virtual void reset_weights(numeric bound);
    void charge(const agile::vector& v); 
    agile::vector fire(); // Fire the charge.
    agile::vector dump_below();

    void backpropagate(const agile::vector &v);
    void backpropagate(const agile::vector &v, double weight);

    void update();
    void update(double weight);
//-----------------------------------------------------------------------------
//  Parameter Setting methods
//-----------------------------------------------------------------------------
    virtual void set_batch_size(int size)
    {
        if (ctr > 0)
        {
            update();
            m_batch_size = size;
        }
        else
        {
            m_batch_size = size;
        }
    }
    virtual void set_learning(const numeric &value)
    {
        learning = value;
    }
    virtual void set_momentum(const numeric &value)
    {
        momentum = value;
    }
    virtual void set_regularizer(const numeric &value)
    {
        regularizer = value;
    }
    void set_layer_type(const layer_type &type)
    {
        m_layer_type = type;
    }
    layer_type get_layer_type()
    {
        return m_layer_type;
    }

    agile::matrix get_weights()
    {
        return W;
    }
//-----------------------------------------------------------------------------
//  Access for YAML serialization
//-----------------------------------------------------------------------------
    friend YAML::Emitter& operator << (YAML::Emitter& out, const layer &L);

    friend YAML::Emitter& operator << (YAML::Emitter& out, 
        const architecture &arch);

    friend struct YAML::convert<layer>;
    friend struct YAML::convert<architecture>;

    friend class architecture;
    friend class agile::neural_net;


//-----------------------------------------------------------------------------
//  Stupid virtuals for derived classes
//----------------------------------------------------------------------------- 
    virtual agile::vector reconstruct(const agile::vector &v, 
        bool noisify = true) 
    {
        throw std::logic_error("layer::reconstruct() called on class\
         layer -- only valid for class autoencoder");
    }
    virtual void encode(const agile::vector &v, bool noisify = true) 
    {
        throw std::logic_error("layer::reconstruct() called on class\
         layer -- only valid for class autoencoder");
    }

    virtual void encode(const agile::vector &v, double weight, bool noisify = true) 
    {
        throw std::logic_error("layer::reconstruct() called on class\
         layer -- only valid for class autoencoder");
    }

    virtual agile::vector get_encoding(const agile::vector &v)
    {
        throw std::logic_error("layer::get_encoding() called on class\
         layer -- only valid for class autoencoder");
    }
    
protected:
//-----------------------------------------------------------------------------
//  Protected Members
//-----------------------------------------------------------------------------
    int m_inputs,     // number of inputs to the layer
        m_outputs,    // number of outputs leaving the layer
        m_batch_size, // number of examples to consider when updating gradient
        ctr;          // number of examples we've considered so far

    agile::matrix W,       // current weight matrix
                  W_old,   // previous weight matrix
                  W_change;// the change to make to W

    agile::vector b,            // bias vector
                  b_old,        // previous bias vector
                  b_change,     // change to make to b
                  m_out,        // untransformed layer output
                  m_in,         // input to the layer
                  delta,        // intermediate derivative
                  m_dump_below; // quantity to feed to a lower layer

    numeric learning,    // learning rate
            momentum,    // momentum (gradient smoothing) parameter
            regularizer; // l2 regularization scalar

    layer_type m_layer_type; // what type of layer (linear, sigmoid, etc.)
    agile::types::paradigm m_paradigm; //type of pre-training

private:
    layer* clone()
    {
        return new layer(*this);
    }

};

//-----------------------------------------------------------------------------
//  Typedef the stack of layers for the architecture class
//-----------------------------------------------------------------------------
namespace agile
{
    typedef std::vector<std::unique_ptr<layer>> layer_stack;
}

//-----------------------------------------------------------------------------
//  YAML Serialization Structure 
//  (look at https://code.google.com/p/yaml-cpp/wiki/Tutorial)
//-----------------------------------------------------------------------------
namespace YAML 
{
    template<>
    struct convert<layer> 
    {
        static Node encode(const layer& L)
        {
            Node node;
            node["class"] = "layer";
            node["inputs"] = L.m_inputs;
            node["outputs"] = L.m_outputs;
            node["learning"] = L.learning;
            node["momentum"] = L.momentum;
            node["regularizer"] = L.regularizer;
            node["batchsize"] = L.m_batch_size;

            if (L.m_layer_type == linear)
            {
                node["activation"] = "linear";
            }
            else if (L.m_layer_type == softmax)
            {
                node["activation"] = "softmax";
            }
            else if (L.m_layer_type == rectified)
            {
                node["activation"] = "rectified";
            }
            else
            {
                node["activation"] = "sigmoid";
            }

            node["weights"] = agile::stringify(L.W);
            node["bias"] = agile::stringify(L.b);
            // node["decoder"] = layer::_reveal(L);

            return node;
        }

        static bool decode(const Node& node, layer& L) 
        {
            L.m_paradigm = agile::types::Basic;
            L.m_inputs = node["inputs"].as<int>();
            L.m_outputs = node["outputs"].as<int>();
            L.learning = node["learning"].as<double>();
            L.momentum = node["momentum"].as<double>();
            L.regularizer = node["regularizer"].as<double>();
            L.m_batch_size = node["batchsize"].as<int>();

            L.m_in.conservativeResize(L.m_inputs);
            L.m_out.conservativeResize(L.m_outputs);



            std::string tmp_str = node["activation"].as<std::string>();
            if (tmp_str == "linear")
            {
                L.m_layer_type = linear;
            }
            else if (tmp_str == "sigmoid")
            {
                L.m_layer_type = sigmoid;
            }
            else if (tmp_str == "rectified")
            {
                L.m_layer_type = rectified;
            }
            else
            {
                L.m_layer_type = softmax;
            }

            L.W = agile::destringify(node["weights"].as<std::string>());
            L.b = agile::destringify(node["bias"].as<std::string>());

            return true;
        }
    };
}


#endif


