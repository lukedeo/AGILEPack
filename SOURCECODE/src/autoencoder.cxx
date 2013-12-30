#include "autoencoder.hh"

autoencoder::autoencoder(int n_inputs, int n_outputs, layer_type encoder_type, layer_type decoder_type)
: layer(n_inputs, n_outputs, encoder_type), decoder(n_outputs, n_inputs, decoder_type)
{
}

autoencoder::autoencoder(const autoencoder &L)
: layer(L), decoder(L.decoder)
{
}

void autoencoder::construct(int n_inputs, int n_outputs, layer_type encoder_type, layer_type decoder_type)
{
	layer::construct(n_inputs, n_outputs, encoder_type);
	decoder.construct(n_outputs, n_inputs, decoder_type);
}

void autoencoder::reset_weights(numeric bound)
{
	layer::reset_weights(bound);
	decoder.reset_weights(bound);
}


autoencoder::~autoencoder()
{

}	
