#include "dataframe.hh"
#include "model_frame.hh"
#include "tree_reader.hh"
#include "architecture.hh"
#include "layer.hh"
#include "autoencoder.hh"
#include "neural_net.hh"
#include <fstream>

int main(int argc, char const *argv[])
{
    auto in_file = std::string(argv[1]);
    auto tree_name = std::string(argv[2]);

    root::tree_reader TR;

    TR.add_file(in_file, tree_name);

    TR.set_branch("pt", root::double_precision);
    TR.set_branch("bottom", root::integer);
    TR.set_branch("charm", root::integer);
    TR.set_branch("light", root::integer);
    TR.set_branch("eta", root::double_precision);
    TR.set_branch("cat_pT", root::integer);
    TR.set_branch("cat_eta", root::integer);
    TR.set_branch("nSingleTracks", root::integer);
    TR.set_branch("nTracks", root::integer);
    TR.set_branch("nTracksAtVtx", root::integer);
    TR.set_branch("nVTX", root::integer);
    TR.set_branch("SV1", root::double_precision);
    TR.set_branch("SV0", root::double_precision);
    TR.set_branch("ip3d_pb", root::double_precision);
    TR.set_branch("ip3d_pu", root::double_precision);
    TR.set_branch("ip3d_pc", root::double_precision);
    TR.set_branch("jfit_efrc", root::double_precision);
    TR.set_branch("jfit_mass", root::double_precision);
    TR.set_branch("jfit_sig3d", root::double_precision);
    TR.set_branch("svp_mass", root::double_precision);
    TR.set_branch("svp_efrc", root::double_precision);
    TR.set_branch("energyFraction", root::double_precision);
    TR.set_branch("mass", root::double_precision);
    TR.set_branch("maxSecondaryVertexRho", root::double_precision);
    TR.set_branch("maxTrackRapidity", root::double_precision);
    TR.set_branch("meanTrackRapidity", root::double_precision);
    TR.set_branch("minTrackRapidity", root::double_precision);
    TR.set_branch("significance3d", root::double_precision);
    TR.set_branch("subMaxSecondaryVertexRho", root::double_precision);
    TR.set_branch("jfit_nvtx", root::integer);
    TR.set_branch("jfit_nvtx1t", root::integer);
    TR.set_branch("jfit_ntrkAtVx", root::integer);

    std::cout << "Pulling dataset from ROOT file...";
    agile::dataframe D = TR.get_dataframe(1000);

    std::cout << "Done." << std::endl;

    std::string formula = (argc <= 3) ? "bottom + light + charm ~ * -eta -pt": std::string(argv[3]);

    std::cout << "Formula: " << formula << std::endl;

    neural_net arch;
    std::cout << "Adding dataset to the neural net...";
    arch.add_data(D);
    std::cout << "Done." << std::endl;
    std::cout << "Parsing and extracting formula...";
    arch.model_formula(formula);
    std::cout << "Done." << std::endl;

    std::cout << "Forming Network Structure...";

    arch.emplace_back(new autoencoder(20, 40, sigmoid)); 
    arch.emplace_back(new autoencoder(40, 30, sigmoid)); 
    arch.emplace_back(new autoencoder(30, 20, sigmoid)); 
    arch.emplace_back(new autoencoder(20, 10, sigmoid)); 
    arch.emplace_back(new autoencoder(10, 3, softmax)); 

    arch.set_learning(0.01);
    arch.set_regularizer(0.001);
    arch.set_batch_size(1);

    std::cout << "Done." << std::endl;

    arch.check();

    std::cout << "Unsupervised Learning...";

    arch.train_unsupervised(10);

    arch.set_learning(0.05);
    
    int epochs = 300;

    std::cout << "Done." << std::endl;

    std::cout << "Supervised Learning...";

    arch.train_supervised(epochs);

    std::cout << "Done." << std::endl;

    std::ofstream file("neural_network.yaml");
    YAML::Emitter out;

    YAML::Node net;
    net["network"] = arch;
    out << net;
    file << out.c_str();
    file.close();
    std::cout << "original:\n";

    //----------------------------------------------------------------------------
    // This is cross check stuff

    agile::model_frame Model;

    Model.add_dataset(D);
    Model.model_formula(formula);
    Model.generate();
    Model.scale();

    auto X = Model.X();
    auto Y = Model.Y();
    //----------------------------------------------------------------------------

    for (int point = 0; point < 3; ++point)
    {
        agile::rowvec r = arch.predict(X.row(point));
        std::cout << "predicted:\n" << r << "\nactual:\n" << Y.row(point) << std::endl;
    }

    std::cout << "loaded:\n";

    YAML::Node config = YAML::LoadFile("neural_network.yaml");
    architecture ARCH = std::move(config["network"].as<neural_net>());


    for (int point = 0; point < 3; ++point)
    {
        agile::rowvec r = arch.predict(X.row(point));
        std::cout << "predicted:\n" << r << "\nactual:\n" << Y.row(point) << std::endl;
    }


    return 0;
}