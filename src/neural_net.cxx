//-----------------------------------------------------------------------------
//  neural_net.cxx:
//  Implementation for neural_net class, responsible for joining layers
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "neural_net.hh"

namespace agile
{

neural_net::neural_net(int num_layers) 
: architecture(num_layers), m_checked(false)
{
}
//----------------------------------------------------------------------------
neural_net::neural_net(std::initializer_list<int> il, problem_type type) 
: architecture(il, type),  m_checked(false)
{
}
//----------------------------------------------------------------------------
neural_net::neural_net(const neural_net &arch) 
: architecture(arch), predictor_order(arch.predictor_order), 
target_order(arch.target_order), X(arch.X), Y(arch.Y), //DF(arch.DF), 
m_model(arch.m_model),  m_checked(false)
{
    for (auto &entry : arch.stack)
    {
        stack.emplace_back(entry->clone());
        ++n_layers;
    }
    n_training = X.rows();
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
    n_training = X.rows();
    m_checked = arch.m_checked;
    return *this;
}
//----------------------------------------------------------------------------
neural_net& neural_net::operator =(neural_net &&arch)
{
    clear();
    stack = std::move(arch.stack);
    n_layers = std::move(n_layers);

    predictor_order = std::move(arch.predictor_order);
    target_order = std::move(arch.target_order);
    X = std::move(arch.X);
    Y = std::move(arch.Y);
    // DF = arch.DF;
    m_model = std::move(arch.m_model);
    n_training = std::move(n_training);
    m_checked = std::move(arch.m_checked);
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
    n_training = X.rows();
    m_tmp_input.resize(X.cols(), Eigen::NoChange);
    // std::cout << "m_tmp_input.size() = " << m_tmp_input.size() << std::endl;
    m_tmp_output.resize(Y.cols(), Eigen::NoChange);
    // std::cout << "m_tmp_output.size() = " << m_tmp_output.size() << std::endl;

    m_scaling = m_model.get_scaling();
}
//----------------------------------------------------------------------------
void neural_net::from_yaml(const std::string &filename)
{
    YAML::Node config = YAML::LoadFile(filename);
    // auto net = std::move(config["network"].as<agile::neural_net>());

    YAML::convert<agile::neural_net>::decode(config["network"], *this);
}
//----------------------------------------------------------------------------
void neural_net::to_yaml(const std::string &filename)
{
    std::ofstream file(filename);
    if (file.good())
    {
        YAML::Emitter out;
        YAML::Node net;
        net["network"] = *this;
        out << net;
        file << out.c_str();
        file.close();
    }
}
//----------------------------------------------------------------------------
void neural_net::train_unsupervised(const unsigned int &epochs, 
    bool denoising, bool tantrum)
{
    if (!m_checked)
    {
        check(tantrum);
    }
    int idx = 0;
    while(stack.at(idx)->get_paradigm() == agile::types::Autoencoder)
    {
        for (int e = 0; e < epochs; ++e)
        {
            for (int i = 0; i < n_training; ++i)
            {
                encode(X.row(i), idx, denoising);
            }
        }
        ++idx;
        if (idx >= stack.size()) break;
    }
}
//----------------------------------------------------------------------------
void neural_net::train_supervised(const unsigned int &epochs, bool tantrum)
{
    if (!m_checked)
    {
        check(tantrum);
    }
    for (int e = 0; e < epochs; ++e)
    {
        for (int i = 0; i < n_training; ++i)
        {
            correct(X.row(i), Y.row(i));
        }
    }
}
//----------------------------------------------------------------------------
void neural_net::check(bool tantrum)
{
    if ((stack.size() > 0) && (!m_checked))
    {
        if (X.cols() != stack.front()->num_inputs())
        {
            if (tantrum)
            {
                throw std::runtime_error("dimension mismatch in base layer");
            }
            std::cout << "Formula passed specifies ";
            std::cout << X.cols() << " inputs." << std::endl;
            std::cout << "Changing base layer (layer 1) from ";
            std::cout << stack.front()->num_inputs() << " to ";
            std::cout << X.cols() << " inputs." << std::endl;
            stack.front()->resize_input(X.cols());

        }
        for (int l = 1; l < (stack.size() - 1); ++l)
        {
            if (stack.at(l - 1)->num_outputs() != stack.at(l)->num_inputs())
            {
                if (tantrum)
                {
                    throw std::runtime_error("dimension mismatch in layer" 
                        + std::to_string(l + 1));
                }
                std::cout << "Changing layer " << l + 1 << " from ";
                std::cout << stack.at(l)->num_inputs() << " inputs to ";
                std::cout << stack.at(l - 1)->num_outputs();
                std::cout << " inputs." << std::endl;
                stack.at(l)->resize_input(stack.at(l - 1)->num_outputs());
            }
        }
        if (Y.cols() != stack.back()->num_outputs())
        {
            if (tantrum)
            {
                throw std::runtime_error("dimension mismatch in output layer");
            }
            std::cout << "Changing output layer from ";
            std::cout << stack.back()->num_outputs() << " to " << Y.cols();
            std::cout << " outputs." << std::endl;
            stack.back()->resize_output(Y.cols());
        }
        m_checked = true;

        if ((stack.back()->num_outputs() == 1) && 
            (stack.back()->get_layer_type() == softmax))
        {
            if (tantrum)
            {
                throw std::runtime_error("can't use softmax for a 1D output.");
            }
            std::cout << "WARNING: softmax output type set";
            std::cout << " for a 1 dimensional output. ";
            std::cout << "Assuming you meant regression, ";
            std::cout << "so continuing with that..." << std::endl;
            stack.back()->set_layer_type(linear);
        }

    }
   
}

//----------------------------------------------------------------------------
std::map<std::string, double> neural_net::predict_map(std::map<std::string, double> v, bool scale)
{
    int idx = 0;

    m_tmp_input.resize(predictor_order.size(), Eigen::NoChange);
    m_tmp_output.resize(target_order.size(), Eigen::NoChange);
    if (scale)
    {
        for (auto &name : predictor_order)
        {
            m_tmp_input(idx) = (v.at(name) - m_scaling.mean.at(name)) /  m_scaling.sd.at(name);
            ++idx;
        }
    }
    else
    {
        for (auto &name : predictor_order)
        {
            m_tmp_input(idx) = v.at(name);
            ++idx;
        }
    }
    // std::cout << "m_tmp_input = " << m_tmp_input << std::endl;
    m_tmp_output = predict(m_tmp_input);
    idx = 0;
    std::map<std::string, double> prediction;
    for (auto &name : target_order)
    {
        prediction[name] = m_tmp_output(idx);
        ++idx;
    }

    return std::move(prediction);
}

}



