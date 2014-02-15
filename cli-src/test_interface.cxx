#include "Base"
#include "include/parser.hh"


void complain(const std::string &complaint);

//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    std::string s("A simple CLI for AGILEPack centered around providing ");
    s += "functionality for \ntesting Deep Learning";

    optionparser::parser p(s + " models on ROOT TTrees");

//----------------------------------------------------------------------------
    p.add_option("--file", "-f")    .help("Pass at least one file to add to a TChain for testing.")
                                    .mode(optionparser::store_mult_values);
//----------------------------------------------------------------------------
    p.add_option("--tree", "-t")    .help("Name of the TTree to extract.")
                                    .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("--load")          .help("Name of a YAML neural network file to load.")
                                    .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("--verbose", "-v") .help("Make the output verbose");
//----------------------------------------------------------------------------
    p.add_option("-start")          .help("Start index for testing. (Default = 0)")
                                    .mode(optionparser::store_value)
                                    .default_value(0);
//----------------------------------------------------------------------------
    p.add_option("-end")            .help("End index for testing. (Default, whole tree)")
                                    .mode(optionparser::store_value)
                                    .default_value(-1);  
//----------------------------------------------------------------------------

    p.eat_arguments(argc, argv);

    if (!p.get_value("file")) complain("need to pass at least one file.");

    if (!p.get_value("tree")) complain("need to pass a tree name.");

    if (!p.get_value("load")) complain("need a neural network to load.");



    std::vector<std::string> root_files(p.get_value<std::vector<std::string>>("file"));

    std::string ttree_name =    p.get_value<std::string>("tree"),
                load_file =     p.get_value<std::string>("load");

    int     start =       p.get_value<int>("start"),
            end =         p.get_value<int>("end");

    bool    verbose =     p.get_value("verbose");

//----------------------------------------------------------------------------

    agile::root::tree_reader TR;


    for (auto &file : root_files)
    {
       TR.add_file(file, ttree_name);
    }

    TR.set_branches(config_file);

//----------------------------------------------------------------------------
    agile::dataframe D = TR.get_dataframe(end - start, start, verbose);

    agile::neural_net net;
    net.add_data(D);

    layer_type net_type;
    std::string passed_target = p.get_value<std::string>("type");

//----------------------------------------------------------------------------

    if (passed_target == "regress") net_type = linear;
    else if (passed_target == "multiclass") net_type = softmax;
    else if (passed_target == "binary") net_type = sigmoid;
    else complain("type of target needs to be one of 'regress', 'multiclass', or 'binary'.");
    
//----------------------------------------------------------------------------

    int i;
    for (i = 0; i < (structure.size() - 2); ++i)
    {
        if (i < deepauto)
        {
            net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
        }
        else
        {
            net.emplace_back(new layer(structure[i], structure[i + 1], sigmoid));
        }
        
    }
    if (i < deepauto)
    {
        net.emplace_back(new autoencoder(structure[i], structure[i + 1], net_type));
    }
    else
    {
        net.emplace_back(new layer(structure[i], structure[i + 1], net_type));
    }
    
    if (verbose)
    {
        std::cout << "\nParsing model formula " << model_formula << "...";
    }
    net.model_formula(model_formula, true, verbose);
    if (verbose)
    {
        std::cout << "Done." << std::endl;
    }

    net.set_learning(learning);
    net.set_regularizer(regularizer);
    net.set_momentum(momentum);
    net.set_batch_size(batch);
    
    net.check(0);

    if (verbose)
    {
        std::cout << "Performing Unsupervised Pretraining...";
    }
    net.train_unsupervised(uepochs, verbose);
    if (verbose)
    {
        std::cout << "\nPerforming Supervised Training...\n";
    }
    net.train_supervised(sepochs, verbose);
    if (verbose)
    {
        std::cout << "\nDone.\nSaving to " << save_file << "...";
    }
    net.to_yaml(save_file);
    if (verbose)
    {
        std::cout << "Done." << std::endl;
    }

    return 0;
}


void complain(const std::string &complaint)
{
    std::cerr << "Error: " << complaint << std::endl;
    exit(1);
}
