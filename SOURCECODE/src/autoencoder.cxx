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

void autoencoder::encode(const agile::vector &v, bool noisify)
{
	agile::vector error = reconstruct(v, noisify) - v;
	decoder.backpropagate(error);
	this->backpropagate(decoder.dump_below());
}

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


autoencoder::~autoencoder()
{
}	
