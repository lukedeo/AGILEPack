#include "autoencoder.hh"

autoencoder(int n_inputs = 0, int n_outputs = 0, layer_type encoder_type = linear, layer_type decoder_type = linear)
: layer(n_inputs, n_outputs, encoder_type), decoder(n_outputs, n_inputs, decoder_type)
{

}

autoencoder(const autoencoder &L)
{

}

void construct(int n_inputs, int n_outputs, layer_type encoder_type = linear, layer_type decoder_type = linear)
{

}

void reset_weights(numeric bound)
{

}


~autoencoder()
{

}	

layer decoder
