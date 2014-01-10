#ifndef AUTOENCODER_HH
#define AUTOENCODER_HH 

#include "layer.hh"
#include "basedefs.hh"

class autoencoder;

namespace YAML
{
	template <>
	struct convert<autoencoder>;
}



class autoencoder : public layer
{
public:
	autoencoder(int n_inputs = 0, int n_outputs = 0, layer_type encoder_type = linear, layer_type decoder_type = linear);

	autoencoder(const autoencoder &L);

	void construct(int n_inputs, int n_outputs, layer_type encoder_type = linear, layer_type decoder_type = linear);

	void reset_weights(numeric bound);
	virtual std::string get_desc()
	{
		return "this is derived";
	}
	// pass an input vector, the autoencoder class holds the "charge"

	// friend YAML::Emitter& operator << (YAML::Emitter& out, const autoencoder &L);

	virtual void encode(const agile::vector &v, bool noisify = true);
	virtual agile::vector reconstruct(const agile::vector &v, bool noisify = true);

	~autoencoder();	

	friend struct YAML::convert<autoencoder>;
protected:

	layer decoder;
};


//-----------------------------------------------------------------------------
//	PUT COUTS HERE TO DEBUG
//-----------------------------------------------------------------------------
namespace YAML 
{
	template<>
	struct convert<autoencoder> 
	{
		static Node encode(const autoencoder& L)
		{
			Node node;
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
				node["activation"] = "softmax";
			}

			node["weights"] = agile::stringify(L.W);
			node["bias"] = agile::stringify(L.b);
			node["decoder"] = L.decoder;
			return node;
		}

		static bool decode(const Node& node, autoencoder& L) 
		{

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
			L.decoder = node["decoder"].as<layer>();

			return true;
		}
	};
}
#endif