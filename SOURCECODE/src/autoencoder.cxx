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
void autoencoder::construct(int n_inputs, int n_outputs, 
    layer_type encoder_type, layer_type decoder_type)
{
    layer::construct(n_inputs, n_outputs, encoder_type);
    decoder.construct(n_outputs, n_inputs, decoder_type);
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
    this->backpropagate(decoder.dump_below());
}
//----------------------------------------------------------------------------
agile::vector autoencoder::reconstruct(const agile::vector &v, bool noisify)
{
    if (noisify)
    {
        this->charge(agile::functions::add_noise(v));
    }
    else
    {
        this->charge(v);
    }
    decoder.charge(this->fire());
    return decoder.fire();
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
