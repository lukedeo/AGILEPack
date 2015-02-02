//-----------------------------------------------------------------------------
//  Hcc_training.cxx -- a commandline interface for training a deep net for
//  For: Joshua Loyal
//  Author: Luke de Oliveira (lukedeo@stanford.edu)
//-----------------------------------------------------------------------------

#include "AGILEPack"
#include "parser.hh"
#include "struct_generator.hh"
#include <time.h>

// if something is wrong, gotta complain
void complain(const std::string &complaint);

// time stamp for default saving.
const std::string timestamp(void);

// make the optionparser.
optionparser::parser generate_parser();

//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    // Get, eat, and pull out all arguments from the command line
    //----------------------------------------------------------------------------
    auto p = generate_parser();

    p.eat_arguments(argc, argv);

    if (!p.get_value("file")) complain("need to pass at least one file.");

    if (!p.get_value("tree")) complain("need to pass a tree name.");

    if (!p.get_value("config")) complain("need a config file for variable specification. Look at READMEPLZ if you're unsure how to do this.");

    if (!p.get_value("formula")) complain("need a model formula to train.");


    std::vector<std::string> root_files(p.get_value<std::vector<std::string>>("file"));

    std::string ttree_name = p.get_value<std::string>("tree"),
                config_file = p.get_value<std::string>("config"),
                save_file = p.get_value<std::string>("save"),
                model_formula = p.get_value<std::string>("formula");


    double  learning =    p.get_value<double>("learning"), 
            momentum =    p.get_value<double>("momentum"),
            regularizer = p.get_value<double>("regularize");


    int     start = p.get_value<int>("start"),
            end = p.get_value<int>("end"),
            uepochs = p.get_value<int>("uepochs"),
            sepochs = p.get_value<int>("sepochs"),
            batch = p.get_value<int>("batch"),
            prog = p.get_value<int>("prog");

    bool    verbose = !p.get_value("quiet");

//----------------------------------------------------------------------------

    if (verbose)
    {
        std::cout << "\n-------------------------------------------------" << std::endl;
        std::cout << "|       AGILEPack Hcc training procedure.       |" << std::endl;
        std::cout << "-------------------------------------------------\n" << std::endl;
    }

//----------------------------------------------------------------------------

    agile::root::tree_reader reader;

    for (auto &file : root_files)
    {
       reader.add_file(file, ttree_name);
    }

    reader.set_branches(config_file);

    std::vector<int> structure;
    // if a structure is passed, use that
    if (p.get_value("struct"))
    {
        structure = p.get_value<std::vector<int>>("struct");
    }
    else // otherwise, figure out one!
    {
        agile::heuristics::struct_generator gen;
        gen.parse_formula(model_formula, reader.get_ordered_branch_names(), verbose);
        structure = gen.get_structure(agile::heuristics::easy, verbose);
    }

//----------------------------------------------------------------------------

    // pull the data out of the tree, and stick it in a data frame...
    agile::dataframe D = reader.get_dataframe(end - start, start, verbose);

    if (p.get_value("shuffle"))
    {
        D.shuffle();
    }

    agile::neural_net net;
    
    // ...and give it to the net.
    net.add_data(D);

    layer_type net_type = sigmoid;

    if (p.get_value("regression"))
    {
        net_type = linear;
    }

//----------------------------------------------------------------------------

    int i = 0;
    net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
    for (i = 1; i < (structure.size() - 2); ++i)
    {
        net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
    }
    
    net.emplace_back(new autoencoder(structure[i], structure[i + 1], net_type));
    
    if (verbose)
    {
        std::cout << "\nParsing model formula " << model_formula << "...";
    }
    net.model_formula(model_formula, true, verbose);
    if (verbose)
    {
        std::cout << "Done." << std::endl;
    }

    // parameter setting
    net.set_learning(learning);
    net.set_regularizer(regularizer);
    net.set_momentum(momentum);
    net.set_batch_size(batch);
    
    net.check(0);

    if (verbose)
    {
        std::cout << "Performing Unsupervised Pretraining...";
    }
    // unsupervised pretraining
    net.train_unsupervised(uepochs, verbose);
    if (verbose)
    {
        std::cout << "\nPerforming Supervised Training...\n";
    }

    // supervised fine tuning
    net.train_supervised(sepochs, verbose, false, prog, save_file);

    if (verbose)
    {
        std::cout << "\nDone.\nSaving to " << save_file << "...";
    }

    // aaaaand save it.
    net.to_yaml(save_file, reader.get_var_types());
    //   along with the branch info ~~~~~^
    
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

const std::string timestamp(void)  
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

optionparser::parser generate_parser()
{
    std::string s("A simple interface for studies using AGILEPack.");

    optionparser::parser p(s);

    //----------------------------------------------------------------------------
    p.add_option("--file", "-f")    .help("Pass at least one file to add to a smart_chain for training.")
                                    .mode(optionparser::store_mult_values);
    //----------------------------------------------------------------------------
    p.add_option("--tree", "-t")    .help("Name of the TTree to extract from the ROOT file.")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("--save", "-s")    .help("Name of file to save the YAML neural network file to. Something like my-model.yaml")
                                    .mode(optionparser::store_value)
                                    .default_value(std::string("neural_net" + timestamp() + ".yaml"));
    //----------------------------------------------------------------------------
    p.add_option("--learning")      .help("Pass a learning rate.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.0001);
    //----------------------------------------------------------------------------
    p.add_option("--momentum")      .help("Pass a momentum value.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.8);
    //----------------------------------------------------------------------------
    p.add_option("--regularize")    .help("Pass an l2 norm regularizer.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.00001);

    p.add_option("--batch")         .help("Mini-batch size.")
                                    .mode(optionparser::store_value)
                                    .default_value(1);
    //----------------------------------------------------------------------------

    p.add_option("--config", "-c")  .help("Pass a configuration file containing branch names and numeric types of interest.")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    std::string struct_help = "Pass the structure of the neural network. For example, one\n";
    struct_help.append(25, ' ');
    struct_help += "could specify --struct=13 24 23 15 3 [other flags]. If not passed, a heuristic will choose one for you.";

    p.add_option("--struct")        .help(struct_help)
                                    .mode(optionparser::store_mult_values);

    //----------------------------------------------------------------------------
    p.add_option("--quiet", "-q")   .help("Make the output quiet. If not passed, the training procedure will tell you things, show you progress bars, etc.");
    p.add_option("--shuffle")       .help("If passed, AGILEPack will re-shuffle the data you pass before training. This is HIGHLY recommended.");
    p.add_option("--regression")    .help("By default, AGILEPack assumes classification. If you are performing regression, pass this flag.");
    //----------------------------------------------------------------------------
    p.add_option("--start")         .help("Start index for training. (Default = 0)")
                                    .mode(optionparser::store_value)
                                    .default_value(0);
    //----------------------------------------------------------------------------
    p.add_option("--end")           .help("End index for training. (Default, whole tree)")
                                    .mode(optionparser::store_value)
                                    .default_value(-1);  
    //----------------------------------------------------------------------------
    p.add_option("--uepochs")       .help("Number of passes over the Trees for Unsupervised Pretraining(Default = 5)")
                                    .mode(optionparser::store_value)
                                    .default_value(5);
    //----------------------------------------------------------------------------
    p.add_option("--sepochs")       .help("Number of passes over the Trees for Supervised Training(Default = 10)")
                                    .mode(optionparser::store_value)
                                    .default_value(10);
    //----------------------------------------------------------------------------
    std::string prog_string = "Output progress files every (n) epochs. If n isn't passed,\n";
    prog_string.append(25, ' ');
    p.add_option("--prog")           .help(prog_string + "uses default. (Default = 1)")
                                    .mode(optionparser::store_value)
                                    .default_value(1);
    //----------------------------------------------------------------------------
    p.add_option("--formula", "-F") .help("Specify Model Formula. Has to be of the form --formula=\"signal~var1+var2\"")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------  

    return p; 
    
}
