#include "architecture.hh"

architecture::architecture(int num_layers) : n_layers(num_layers)
{
	stack.resize(num_layers);
}
architecture::~architecture()
{
}
void architecture::push_back(layer &L)
{
	stack.push_back(L);
	++n_layers;
}

void architecture::pop_back()
{
	stack.pop_back();
	--n_layers;
}
unsigned int architecture::size()
{
	return n_layers;
}

agile::vector architecture::predict(const agile::vector &v)
{
	stack.at(0).charge(v);
	for (unsigned int i = 1; i < n_layers; ++i)
	{
		stack.at(i).charge(stack.at(i - 1).fire());
	}
	return stack.at(n_layers - 1).fire();
}

void architecture::correct(const agile::vector &in, const agile::vector &target)
{
	agile::vector error = predict(in) - target;
	int l = n_layers - 1;
	stack.at(l).backpropagate(error);

	for (l = (l - 1); l >= 0; --l)
	{
		stack.at(l).backpropagate( stack.at(l + 1).dump_below() );
	}
}