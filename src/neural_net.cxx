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
: architecture(arch), predictor_order(arch.predictor_order), 
target_order(arch.target_order), X(arch.X), Y(arch.Y), //DF(arch.DF), 
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
    // DF = arch.DF;
    m_model = arch.m_model;

    return *this;
}
//----------------------------------------------------------------------------
neural_net::~neural_net()
{
}
//---------------------------------------------------------------------------
void neural_net::add_data(const agile::dataframe &D)
{
    m_model.add_dataset(D);
}
void neural_net::add_data(agile::dataframe &&D)
{
    m_model.add_dataset(std::move(D));

}

// void set_formula(const std::string &formula);
// void add_predictor(const std::string &name);
// void add_target(const std::string &name);
void neural_net::model_formula(const std::string &formula, bool scale)
{
    m_model.model_formula(formula);
    m_model.generate();
    if (scale)
    {
        m_model.scale();
    }
    predictor_order = m_model.get_inputs();
    target_order = m_model.get_outputs();
    X = std::move(m_model.X());
    Y = std::move(m_model.Y());
}
// void neural_net::load_network(const std::string &formula)
// {

// }
// void neural_net::save_network(const std::string &formula)
// {
//     std::ofstream file(formula);
//     if (file.good())
//     {
//         YAML::Emitter out;
//         YAML::Node net;
//         net["network"] = *this;
//         out << net;
//         file << out.c_str();
//         file.close();
//     }
// }

// void neural_net::train_unsupervised(const unsigned int &iters)
// {

// }
// void neural_net::train_supervised(const unsigned int &iters)
// {

// }



