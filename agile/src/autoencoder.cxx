//-----------------------------------------------------------------------------
//  autoencoder.cxx:
//  Implementation of autoencoder class, inherits from layer class
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "autoencoder.hh"

autoencoder::autoencoder(int n_inputs, int n_outputs, 
    layer_type encoder_type, layer_type decoder_type) :
layer(n_inputs, n_outputs, encoder_type), 
decoder(n_outputs, n_inputs, decoder_type),
m_paradigm(agile::types::Autoencoder)

{
}
//----------------------------------------------------------------------------
autoencoder::autoencoder(const autoencoder &L) :
layer(L), 
decoder(L.decoder)
{
}
//----------------------------------------------------------------------------
autoencoder::autoencoder(autoencoder *L) :
layer(*L), 
decoder(L->decoder)
{
}
//----------------------------------------------------------------------------
autoencoder& autoencoder::operator= (const autoencoder &L)
{
    m_inputs = (L.m_inputs);
    m_outputs = (L.m_outputs);
    m_batch_size = (L.m_batch_size);

    W = (L.W);
    W_old = (L.W_old);
    W_change = (L.W_change);

    b = (L.b);
    b_old = (L.b_old);
    b_change = (L.b_change);

    m_out = (L.m_out);
    m_in = (L.m_in);

    learning = (L.learning);
    momentum = (L.momentum);
    regularizer = (L.regularizer);
    m_paradigm = (L.m_paradigm);
    m_layer_type = (L.m_layer_type);
    decoder = (L.decoder);

    return *this;
}
//----------------------------------------------------------------------------
autoencoder& autoencoder::operator= (autoencoder &&L)
{
    m_inputs = std::move(L.m_inputs);
    m_outputs = std::move(L.m_outputs);
    m_batch_size = std::move(L.m_batch_size);

    W = std::move(L.W);
    W_old = std::move(L.W_old);
    W_change = std::move(L.W_change);

    b = std::move(L.b);
    b_old = std::move(L.b_old);
    b_change = std::move(L.b_change);

    m_out = std::move(L.m_out);
    m_in = std::move(L.m_in);

    learning = std::move(L.learning);
    momentum = std::move(L.momentum);
    regularizer = std::move(L.regularizer);

    m_layer_type = std::move(L.m_layer_type);
    m_paradigm = std::move(L.m_paradigm);
    decoder = std::move(L.decoder);

    return *this;
}
//----------------------------------------------------------------------------
void autoencoder::construct(int n_inputs, int n_outputs, 
    layer_type encoder_type, layer_type decoder_type)
{
    layer::construct(n_inputs, n_outputs, encoder_type);
    decoder.construct(n_outputs, n_inputs, decoder_type);
    m_paradigm = agile::types::Autoencoder;
}
//----------------------------------------------------------------------------
void autoencoder::resize_input(int n_inputs)
{
    decoder.resize_input(n_inputs);
    m_inputs = n_inputs;
    W.resize(m_outputs, n_inputs);
    W_change.resize(m_outputs, n_inputs);
    W_old.resize(m_outputs, n_inputs);
    m_in.resize(n_inputs, Eigen::NoChange);
}
//----------------------------------------------------------------------------
void autoencoder::resize_output(int n_outputs)
{
    decoder.resize_output(n_outputs);
    m_outputs = n_outputs;
    W.resize(n_outputs, m_inputs);
    W_change.resize(n_outputs, m_inputs);
    W_old.resize(n_outputs, m_inputs);
    b.resize(n_outputs, Eigen::NoChange);
    b_change.resize(n_outputs, Eigen::NoChange);
    b_old.resize(n_outputs, Eigen::NoChange);
    m_out.resize(n_outputs, Eigen::NoChange);
}
//----------------------------------------------------------------------------
void autoencoder::reset_weights(numeric bound)
{
    layer::reset_weights(bound);
    decoder.reset_weights(bound);
}
//----------------------------------------------------------------------------
void autoencoder::encode(const agile::vector &v, bool noisify)
{
    agile::vector error = reconstruct(v, noisify) - v;  
    decoder.backpropagate(error);
    backpropagate(decoder.dump_below());
    // getchar();
}
//----------------------------------------------------------------------------
agile::vector autoencoder::reconstruct(const agile::vector &v, bool noisify)
{
    // std::cout << "input = \n" << v << std::endl;

    if (noisify)
    {
        this->charge(agile::functions::add_noise(v));
    }
    else
    {
        this->charge(v);
    }
    decoder.charge(this->fire());
    // std::cout << "decoder.fire() = \n" << decoder.fire() << std::endl;
    // getchar();
    return decoder.fire();
}
//----------------------------------------------------------------------------
agile::vector autoencoder::get_encoding(const agile::vector &v)
{
    this->charge(v);
    return std::move(this->fire());
}
//----------------------------------------------------------------------------
autoencoder::~autoencoder()
{
}   
//----------------------------------------------------------------------------
YAML::Emitter& operator << (YAML::Emitter& out, const autoencoder &L) 
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
