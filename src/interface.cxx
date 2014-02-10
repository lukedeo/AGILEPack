// #include "Base"
#include "cmd-parser/include/parser.hh"

// void config_info(); 


//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    std::string s("A simple CLI for AGILEPack centered around providing ");
    s += "functionality for \ntraining and testing Deep Learning";

    optionparser::parser p(s + " models on ROOT TTrees");

//----------------------------------------------------------------------------
    p.add_option("--file", "-f").help("Pass at least one file to add to a TChain for training.")
                                .required(false)
                                .mode(optionparser::store_mult_values);
//----------------------------------------------------------------------------
    p.add_option("--save", "-s").help("Name of file to save the YAML neural network file to.")
                                .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("--learning")  .help("Pass a learning rate.")
                                .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("--momentum")  .help("Pass a learning rate.")
                                .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("--load")      .help("Name of a YAML neural network file to load to begin training")
                                .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    std::string config_help = "Pass a configuration file for training specifications instead\n";
    config_help.append(25, ' ');
    config_help += "of over the command line. For help on formatting, pass\n";
    config_help.append(25, ' ');
    config_help += "the '-confighelp' flag. This overrides all paramaters from CLI.";

    p.add_option("--config", "-c")  .help(config_help)
                                    .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("-confighelp") .help("Display info about YAML training config files.");
//----------------------------------------------------------------------------
    std::string struct_help = "Pass the structure of the neural network. For example, one\n";
    struct_help.append(25, ' ');
    struct_help += "could specify --struct=13 24 23 15 3 [other flags].";

    p.add_option("--struct")    .help(struct_help)
                                .mode(optionparser::store_mult_values);
//----------------------------------------------------------------------------
    std::string autoencoder_help = "Pretraining using deep autoencoders, if you pass an\n";
    autoencoder_help.append(25, ' ');
    autoencoder_help += "integer n, it will train the first n layers. It defaults to all.";

    p.add_option("--deepauto", "-d").help(autoencoder_help)
                                    .mode(optionparser::store_value)
                                    .default_value("-1");
//----------------------------------------------------------------------------
    p.eat_arguments(argc, argv);

    return 0;
}


// void config_info()
// {

// }