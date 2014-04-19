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
    agile::dataframe data = reader.get_dataframe(600000); 

    std::ofstream dframe("topjets.csv");
   
    dframe << data;

    dframe.close();

    agile::neural_net net;

    net.add_data(data);

    net.emplace_back(new autoencoder(4, 24, sigmoid));
    net.emplace_back(new autoencoder(24,15, sigmoid));
    net.emplace_back(new autoencoder(15,10, sigmoid));
    net.emplace_back(new autoencoder(10,5, sigmoid));
    net.emplace_back(new autoencoder(5, 3, sigmoid));
    net.emplace_back(new autoencoder(3, 1, sigmoid));
// 
    // net.emplace_back(new autoencoder(6, 5, sigmoid));
    // net.emplace_back(new autoencoder(5, 4, sigmoid));
    // net.emplace_back(new autoencoder(4, 2, sigmoid));
    // net.emplace_back(new autoencoder(2, 1, sigmoid));
// 
    // first true is to scale variables, second is for verbosity
 // 
    net.model_formula("top ~ * -mcevt_weight_flat -fjet_pt_flat -fjet_eta_flat", true, true);
    // net.model_formula("top ~ fjet_Tau1_flat + fjet_Tau2_flat + fjet_Tau3_flat + fjet_SPLIT23_flat + Tau32 + Tau21", true, true);

    net.set_learning(0.00086);
    net.set_regularizer(0.00001);
    net.set_batch_size(1);
    net.set_momentum(0.8);

    net.check(false);

    // pretraining
    net.train_unsupervised(10, true);

    // fine tuning 
    std::cout << "\nSupervised Training..." << std::endl;
    net.train_supervised(30, true);

    net.to_yaml(savefile, reader.get_var_types());

    return 0;
}