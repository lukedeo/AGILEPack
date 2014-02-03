//-----------------------------------------------------------------------------
//  neural_net.cxx:
//  Implementation for neural_net class, responsible for joining layers
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "neural_net.hh"

neural_net::neural_net(int num_layers) 
: architecture(num_layers)
{
}
//----------------------------------------------------------------------------
neural_net::neural_net(std::initializer_list<int> il, problem_type type) 
: architecture(il, type)
{
}
//----------------------------------------------------------------------------
neural_net::neural_net(const neural_net &arch) 
: n_layers(0), stack(0), predictor_order(arch.predictor_order), 
target_order(arch.target_order), X(arch.X), Y(arch.Y), DF(arch.DF), 
m_model(arch.m_model)
{
    for (auto &entry : arch.stack)
    {
        stack.emplace_back(entry->clone());
        ++n_layers;
    }
}
//----------------------------------------------------------------------------
neural_net& neural_net::operator =(const neural_net &arch)
{
    clear();
    for (auto &entry : arch.stack)
    {
        stack.emplace_back(entry->clone());
        ++n_layers;
    }
    predictor_order = arch.predictor_order;
    target_order = arch.target_order;
    X = arch.X;
    Y = arch.Y;
    DF = arch.DF;
    m_model = arch.m_model;
    
    return *this;
}
//----------------------------------------------------------------------------
neural_net::~neural_net()
{
}
//----------------------------------------------------------------------------
void neural_net::add_layer(int n_inputs, int n_outputs, layer_type type)
{
    ++n_layers;
    stack.emplace_back(new layer(n_inputs, n_outputs, type));
}
//----------------------------------------------------------------------------
void neural_net::resize(unsigned int size)
{
    stack.resize(size);
    n_layers = size;
}
//----------------------------------------------------------------------------
void neural_net::pop_back()
{
    stack.pop_back();
    --n_layers;
}
//----------------------------------------------------------------------------
unsigned int neural_net::size()
{
    return n_layers;
}
//----------------------------------------------------------------------------
void neural_net::clear()
{
    n_layers = 0;
    stack.clear();
}
//----------------------------------------------------------------------------
std::unique_ptr<layer>& neural_net::at(const unsigned int &idx)
{
    if (idx >= n_layers)
    {
        throw std::out_of_range(
            "Accessing layer in 'Class: neural_net' beyond contained range.");
    }
    return stack.at(idx);
}
//----------------------------------------------------------------------------
agile::vector neural_net::predict(const agile::vector &v)
{
    stack.at(0)->charge(v);
    for (unsigned int i = 1; i < n_layers; ++i)
    {
        stack.at(i)->charge(stack.at(i - 1)->fire());
    }
    return stack.at(n_layers - 1)->fire();
}
//----------------------------------------------------------------------------
void neural_net::correct(const agile::vector &in, 
    const agile::vector &target)
{
    agile::vector error = predict(in) - target;
    // std::cout << predict(in) << std::endl;
    // std::cout << "error: " << error << std::endl;
    int l = n_layers - 1;
    stack.at(l)->backpropagate(error);

    for (l = (n_layers - 2); l >= 0; --l)
    {
        stack.at(l)->backpropagate( stack.at(l + 1)->dump_below() );
    }
}
//---------------------------------------------------------------------------

