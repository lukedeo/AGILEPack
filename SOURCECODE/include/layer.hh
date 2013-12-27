#ifndef LAYER_H
#define LAYER_H 

#include <iostream>
#include "basedefs.hh"
#include "activation.hh"


class layer
{
public:
	explicit layer(int n_inputs = 0, int n_outputs = 0, layer_type type = linear);

	layer(const layer &L);

	void construct(int n_inputs, int n_outputs, layer_type type);

	void reset_weights(numeric bound);
	// pass an input vector, the layer class holds the "charge"
	void charge(const agile::vector& v); 
	virtual agile::vector fire(); // Fire the charge.

	// void perturb_weights(numeric bound);
	// void set_batch_size(int size);

	virtual void backpropagate(const agile::vector &v);

	agile::vector dump_below();

	void update();

	friend YAML::Emitter& operator << (YAML::Emitter& out, const layer &L);

	~layer();	

	int m_inputs, m_outputs, m_batch_size, ctr;
	agile::matrix W, W_old, W_change;
	agile::vector b, b_old, b_change, m_out, m_in, delta, m_dump_below;
	numeric learning, momentum, regularizer;
	layer_type m_layer_type;
};

namespace agile
{
	typedef std::vector<layer> layer_stack;
}


//-----------------------------------------------------------------------------
//	PUT COUTS HERE TO DEBUG
//-----------------------------------------------------------------------------
namespace YAML 
{
	template<>
	struct convert<layer> 
	{
		static Node encode(const layer& L)
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
			else
			{
				node["activation"] = "softmax";
			}

			node["weights"] = agile::stringify(L.W);
			node["bias"] = agile::stringify(L.b);
			return node;
		}

		static bool decode(const Node& node, layer& L) 
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


