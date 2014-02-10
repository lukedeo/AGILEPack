// #include "Base"
#include "include/parser.hh"


//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    std::string s("A simple CLI for AGILEPack centered around providing ");
    s += "functionality for \ntraining and testing Deep Learning";

    optionparser::parser p(s + " models on ROOT TTrees");

    p.add_option("--file", "-f").help("Pass at least one file to add to a TChain for training.")
                                .required(true)
                                .mode(optionparser::store_mult_values);

    p.add_option("--save", "-s").help("Name of file to save the YAML config to.")
                                .required(false)
                                .mode(optionparser::store_value);

    p.add_option("--learning")  .help("Pass a learning rate.")
                                .mode(optionparser::store_value);

    p.add_option("--momentum")  .help("Pass a learning rate.")
                                .mode(optionparser::store_value);

    p.add_option("--load")      .help("Name of a YAML config file to load to begin training")
                                .mode(optionparser::store_value);

    p.eat_arguments(argc, argv);





    return 0;
}