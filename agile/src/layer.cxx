//-----------------------------------------------------------------------------
//  layer.cxx:
//  Implementation for layer class, responsible for s(Wx+b) calculation
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "agile/include/layer.hh"
#include <stdlib.h>

//----------------------------------------------------------------------------
layer::layer(int n_inputs, int n_outputs, layer_type type) :
m_inputs(n_inputs), 
m_outputs(n_outputs), 
m_batch_size(3), 

W(n_outputs, n_inputs), 
W_cache(n_outputs, n_inputs), 
W_cache_2(n_outputs, n_inputs), 
W_grad(n_outputs, n_inputs), 

b(n_outputs), 
b_cache(n_outputs), 
b_cache_2(n_outputs), 
b_grad(n_outputs), 

m_out(n_outputs), 
m_in(n_inputs), 

learning(0.2), 
momentum(0.5), 
regularizer(0.00), 

m_layer_type(type),
m_paradigm(agile::types::Basic)

{
    ctr = 0;
    reset_weights(sqrt((numeric)5 / (numeric)(n_inputs + n_outputs)));
}
//----------------------------------------------------------------------------
layer::layer(const layer &L): 
m_inputs(L.m_inputs),
m_outputs(L.m_outputs),
m_batch_size(L.m_batch_size),

W(L.W),
W_cache(L.W_cache),
W_cache_2(L.W_cache_2),
W_grad(L.W_grad),

b(L.b),
b_cache(L.b_cache),
b_cache_2(L.b_cache_2),
b_grad(L.b_grad),

m_out(L.m_out),
m_in(L.m_in),

learning(L.learning),
momentum(L.momentum),
regularizer(L.regularizer),

m_layer_type(L.m_layer_type),
m_paradigm(L.m_paradigm)

{
    ctr = 0;
}
//----------------------------------------------------------------------------
layer::layer(layer &&L): 
m_inputs(std::move(L.m_inputs)),
m_outputs(std::move(L.m_outputs)),
m_batch_size(std::move(L.m_batch_size)),

W(std::move(L.W)),
W_cache(std::move(L.W_cache)),
W_cache_2(std::move(L.W_cache_2)),
W_grad(std::move(L.W_grad)),

b(std::move(L.b)),
b_cache(std::move(L.b_cache)),
b_cache_2(std::move(L.b_cache_2)),
b_grad(std::move(L.b_grad)),

m_out(std::move(L.m_out)),
m_in(std::move(L.m_in)),

learning(std::move(L.learning)),
momentum(std::move(L.momentum)),
regularizer(std::move(L.regularizer)),

m_layer_type(std::move(L.m_layer_type)),
m_paradigm(std::move(L.m_paradigm))

{
    ctr = 0;
}
//----------------------------------------------------------------------------
layer::layer(layer *L): 
m_inputs(L->m_inputs),
m_outputs(L->m_outputs),
m_batch_size(L->m_batch_size),

W(L->W),
W_cache(L->W_cache),
W_cache_2(L->W_cache_2),
W_grad(L->W_grad),

b(L->b),
b_cache(L->b_cache),
b_cache_2(L->b_cache_2),
b_grad(L->b_grad),

m_out(L->m_out),
m_in(L->m_in),

learning(L->learning),
momentum(L->momentum),
regularizer(L->regularizer),

m_layer_type(L->m_layer_type),
m_paradigm(L->m_paradigm)

{
    ctr = 0;
}
//----------------------------------------------------------------------------
layer& layer::operator= (const layer &L)
{
    m_inputs = (L.m_inputs);
    m_outputs = (L.m_outputs);
    m_batch_size = (L.m_batch_size);

    W = (L.W);
    W_cache = (L.W_cache);
    W_cache_2 = (L.W_cache_2);
    W_grad = (L.W_grad);

    b = (L.b);
    b_cache = (L.b_cache);
    b_cache_2 = (L.b_cache_2);
    b_grad = (L.b_grad);

    m_out = (L.m_out);
    m_in = (L.m_in);

    learning = (L.learning);
    momentum = (L.momentum);
    regularizer = (L.regularizer);

    m_layer_type = (L.m_layer_type);
    m_paradigm = (L.m_paradigm);

    return *this;
}
//----------------------------------------------------------------------------
layer& layer::operator= (layer &&L)
{
    m_inputs = std::move(L.m_inputs);
    m_outputs = std::move(L.m_outputs);
    m_batch_size = std::move(L.m_batch_size);

    W = std::move(L.W);
    W_cache = std::move(L.W_cache);
    W_cache_2 = std::move(L.W_cache_2);
    W_grad = std::move(L.W_grad);

    b = std::move(L.b);
    b_cache = std::move(L.b_cache);
    b_cache_2 = std::move(L.b_cache_2);
    b_grad = std::move(L.b_grad);

    m_out = std::move(L.m_out);
    m_in = std::move(L.m_in);

    learning = std::move(L.learning);
    momentum = std::move(L.momentum);
    regularizer = std::move(L.regularizer);

    m_layer_type = std::move(L.m_layer_type);
    m_paradigm = std::move(L.m_paradigm);

    return *this;
}
//----------------------------------------------------------------------------
void layer::construct(int n_inputs, int n_outputs, layer_type type)
{
    ctr = 0;
    m_batch_size = 3;
    learning = 0.2;
    momentum = 0.5;
    regularizer = 0.00;

    m_inputs = n_inputs;
    m_outputs = n_outputs;

    W.resize(n_outputs, n_inputs);
    W_grad.resize(n_outputs, n_inputs);
    W_cache.resize(n_outputs, n_inputs);
    W_cache_2.resize(n_outputs, n_inputs);

    b.resize(n_outputs, Eigen::NoChange);
    b_grad.resize(n_outputs, Eigen::NoChange);
    b_cache.resize(n_outputs, Eigen::NoChange);
    b_cache_2.resize(n_outputs, Eigen::NoChange);

    m_out.resize(n_outputs, Eigen::NoChange);
    m_in.resize(n_inputs, Eigen::NoChange);
    m_layer_type = type;
    m_paradigm = agile::types::Basic;

    reset_weights(sqrt((numeric)6 / (numeric)(n_inputs + n_outputs)));
}
//----------------------------------------------------------------------------
void layer::reset_weights(numeric bound)
{
    std::uniform_real_distribution <numeric> distribution(-bound, bound);
    for (int col = 0; col < m_inputs; ++col)
    {
        for (int row = 0; row < m_outputs; ++row)
        {
            W(row, col) = distribution(agile::mersenne_engine());
            W_cache(row, col) = 0;
            W_cache_2(row, col) = 0;
            W_grad(row, col) = 0;
        }
    }
    for (int row = 0; row < m_outputs; ++row)
    {
        b(row) = distribution(agile::mersenne_engine());
        b_cache(row) = 0;
        b_cache_2(row) = 0;
        b_grad(row) = 0;
    }

}
//----------------------------------------------------------------------------
void layer::resize_input(int n_inputs)
{
    m_inputs = n_inputs;
    W.resize(m_outputs, n_inputs);
    W_grad.resize(m_outputs, n_inputs);
    W_cache.resize(m_outputs, n_inputs);
    W_cache_2.resize(m_outputs, n_inputs);
    m_in.resize(n_inputs, Eigen::NoChange);
    reset_weights(sqrt((numeric)6 / (numeric)(m_inputs + m_outputs)));

}
//----------------------------------------------------------------------------
void layer::resize_output(int n_outputs)
{
    m_outputs = n_outputs;
    W.resize(n_outputs, m_inputs);
    W_grad.resize(n_outputs, m_inputs);
    W_cache.resize(n_outputs, m_inputs);
    W_cache_2.resize(n_outputs, m_inputs);
    b.resize(n_outputs, Eigen::NoChange);
    b_grad.resize(n_outputs, Eigen::NoChange);
    b_cache.resize(n_outputs, Eigen::NoChange);
    b_cache_2.resize(n_outputs, Eigen::NoChange);
    m_out.resize(n_outputs, Eigen::NoChange);
    reset_weights(sqrt((numeric)6 / (numeric)(m_inputs + m_outputs)));
}
//----------------------------------------------------------------------------
void layer::charge(const agile::vector& v)
{   
    m_in = v;
    m_out.noalias() = W * v + b;
}
//----------------------------------------------------------------------------
agile::vector layer::fire()
{
    switch(m_layer_type)
    {
        case sigmoid: return agile::functions::exp_sigmoid(m_out);
        case softmax: return agile::functions::softmax(m_out);
        case linear: return m_out;
        case rectified: return agile::functions::rect_lin_unit(m_out);
        default: throw std::domain_error("layer type not recongized.");
    }   
}
//----------------------------------------------------------------------------
void layer::backpropagate(const agile::vector &v)
{
    delta.noalias() = v;

    if (m_layer_type == sigmoid)
    {
        delta = delta.array() * (agile::functions::exp_sigmoid_deriv(
            agile::functions::exp_sigmoid(m_out))).array();
    }
    if (m_layer_type == rectified)
    {
        delta = delta.array() * (agile::functions::rect_lin_unit_deriv(
            agile::functions::rect_lin_unit(m_out))).array();
    }
    // we need something to make this not happen for base 0layer
    m_dump_below.noalias() = W.transpose() * delta; 

    W_grad += delta * m_in.transpose(); 
    b_grad += delta;

    ++ctr;
    if (ctr >= m_batch_size) // if we need to start a new batch
    {   
        ctr = 0;
        update();
    }
}
//----------------------------------------------------------------------------
void layer::backpropagate(const agile::vector &v, double weight)
{
    // std::cout << "updating with weight = " << weight  << std::endl;
    delta.noalias() = v;

    if (m_layer_type == sigmoid)
    {
        delta = delta.array() * (agile::functions::exp_sigmoid_deriv(
            agile::functions::exp_sigmoid(m_out))).array();
    }
    if (m_layer_type == rectified)
    {
        delta = delta.array() * (agile::functions::rect_lin_unit_deriv(
            agile::functions::rect_lin_unit(m_out))).array();
    }
    // we need something to make this not happen for base 0layer
    m_dump_below.noalias() = W.transpose() * delta; 

    W_grad += weight * delta * m_in.transpose(); 
    b_grad += weight * delta;

    ++ctr;
    if (ctr >= m_batch_size) // if we need to start a new batch
    {   
        ctr = 0;
        if ((weight > 0.0) && (m_batch_size == 1))
        {
            update(weight);
        }
        else
        {
            update();
        }
    }
}
//----------------------------------------------------------------------------
agile::vector layer::dump_below()
{
    return m_dump_below;
}
//----------------------------------------------------------------------------
void layer::update(bool adam)
{
    if (adam == false)
    {
        W_grad += m_batch_size * regularizer * W;
        // W_grad /= m_batch_size;
        W_cache = momentum * W_cache - learning * W_grad;
        W += W_cache;
        
        // b_grad /= m_batch_size;
        b_cache = momentum * b_cache - learning * b_grad;
        b += b_cache;
    }
    else
    {
        W_grad += m_batch_size * regularizer * W;
        W_cache = momentum * W_cache + (1 - momentum) * W_grad;
        W_cache_2 = momentum_2 * W_cache_2 + (1 - momentum_2) * W_grad.array() * W_grad.array();
        std::pow(
        b_cache = momentum * b_cache + (1 - momentum) * b_grad;
        b_cache_2 = momentum_2 * b_cache_2 + (1 - momentum_2) * b_grad.array() * b_grad.array();
    }


    b_grad.fill(0.00);
    W_grad.fill(0.00);
}
//----------------------------------------------------------------------------
void layer::update(double weight, bool adam) // will soon be deprecated
{
    // W_grad /= m_batch_size;
    W_cache = momentum * W_cache - learning * (W_grad + regularizer * W);
    // W += weight * W_cache;
    W += W_cache;
    
    // b_grad /= m_batch_size;
    b_cache = momentum * b_cache - learning * b_grad;
    // b += weight * b_cache;
    b += b_cache;

    b_grad.fill(0.00);
    W_grad.fill(0.00);
}
//----------------------------------------------------------------------------
YAML::Emitter& operator << (YAML::Emitter& out, const layer &L) 
{
    out << YAML::BeginMap;
    out << YAML::Key << "inputs"      << YAML::Value << L.m_inputs;
    out << YAML::Key << "outputs"     << YAML::Value << L.m_outputs;
    out << YAML::Key << "learning"    << YAML::Value << L.learning;
    out << YAML::Key << "momentum"    << YAML::Value << L.momentum;
    out << YAML::Key << "regularizer" << YAML::Value << L.regularizer;
    out << YAML::Key << "batchsize"   << YAML::Value << L.m_batch_size;
    out << YAML::Key << "activation";

    if (L.m_layer_type == linear)
    {
        out << YAML::Value << "linear";
    }
    else if (L.m_layer_type == softmax)
    {
        out << YAML::Value << "softmax";
    }
    else if (L.m_layer_type == rectified)
    {
        out << YAML::Value << "rectified";
    }
    else
    {
        out << YAML::Value << "sigmoid";
    }

    out << YAML::Key << "weights" << YAML::Value << agile::stringify(L.W);
    out << YAML::Key << "bias"    << YAML::Value << agile::stringify(L.b);
    out << YAML::EndMap;
    return out;
}
//----------------------------------------------------------------------------
