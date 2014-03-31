#include "Base"


int main(int argc, char const *argv[])
{
    if (argc < 5)
    {
        std::cout << "usage: " << argv[0] << " CONFIGFILE DATAFILE TREENAME OUTFILE" << std::endl;
        return 0;
    }

    std::string config(argv[1]), 
                rootfile(argv[2]),
                tree(argv[3]),
                savefile(argv[4]);



    agile::root::tree_reader reader;
    reader.add_file(rootfile, tree);
    reader.set_branches(config); 

    // agile::dataframe data = reader.get_dataframe(0.5 * reader.size()); 
    agile::dataframe data = reader.get_dataframe(200000); 

    std::ofstream dframe("topjets.csv");
   
    dframe << data;

    dframe.close();

    agile::neural_net net;

    net.add_data(data);

    net.emplace_back(new autoencoder(4, 4, sigmoid));
    net.emplace_back(new autoencoder(4, 3, sigmoid));
    net.emplace_back(new autoencoder(3, 1, sigmoid));

    // first true is to scale variables, second is for verbosity
    net.model_formula("top ~ * -mcevt_weight_flat", true, true);

    net.set_learning(0.0005);
    net.set_regularizer(0.0000001);
    net.set_batch_size(1);
    net.set_momentum(0.79);

    net.check(false);

    // pretraining
    net.train_unsupervised(10, true);

    // fine tuning 
    std::cout << "\nSupervised Training..." << std::endl;
    net.train_supervised(20, true);

    net.to_yaml(savefile, reader.get_var_types());

    return 0;
}