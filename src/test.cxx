#include "dataframe.hh"
#include "model_frame.hh"
#include "tree_reader.hh"
#include "architecture.hh"
#include "layer.hh"
#include "autoencoder.hh"
#include "neural_net.hh"
#include <fstream>


template <class T, class U>
std::ostream& operator << ( std::ostream& o, const std::map<T, U> &m)
{
    for (auto &e : m)
    {
        o << e.first << ": " << e.second << std::endl;
    }
    return o;
}

int main(int argc, char const *argv[])
{
    auto in_file = std::string(argv[1]);
    auto tree_name = std::string(argv[2]);

    agile::root::tree_reader TR;

    TR.add_file(in_file, tree_name);

    TR.set_branch("pt", agile::root::double_precision);
    TR.set_branch("bottom", agile::root::integer);
    TR.set_branch("charm", agile::root::integer);
    TR.set_branch("light", agile::root::integer);
    TR.set_branch("eta", agile::root::double_precision);
    TR.set_branch("cat_pT", agile::root::integer);
    TR.set_branch("cat_eta", agile::root::integer);
    TR.set_branch("nSingleTracks", agile::root::integer);
    TR.set_branch("nTracks", agile::root::integer);
    TR.set_branch("nTracksAtVtx", agile::root::integer);
    TR.set_branch("nVTX", agile::root::integer);
    TR.set_branch("SV1", agile::root::double_precision);
    TR.set_branch("SV0", agile::root::double_precision);
    TR.set_branch("ip3d_pb", agile::root::double_precision);
    TR.set_branch("ip3d_pu", agile::root::double_precision);
    TR.set_branch("ip3d_pc", agile::root::double_precision);
    TR.set_branch("jfit_efrc", agile::root::double_precision);
    TR.set_branch("jfit_mass", agile::root::double_precision);
    TR.set_branch("jfit_sig3d", agile::root::double_precision);
    TR.set_branch("svp_mass", agile::root::double_precision);
    TR.set_branch("svp_efrc", agile::root::double_precision);
    TR.set_branch("energyFraction", agile::root::double_precision);
    TR.set_branch("mass", agile::root::double_precision);
    TR.set_branch("maxSecondaryVertexRho", agile::root::double_precision);
    TR.set_branch("maxTrackRapidity", agile::root::double_precision);
    TR.set_branch("meanTrackRapidity", agile::root::double_precision);
    TR.set_branch("minTrackRapidity", agile::root::double_precision);
    TR.set_branch("significance3d", agile::root::double_precision);
    TR.set_branch("subMaxSecondaryVertexRho", agile::root::double_precision);
    TR.set_branch("jfit_nvtx", agile::root::integer);
    TR.set_branch("jfit_nvtx1t", agile::root::integer);
    TR.set_branch("jfit_ntrkAtVx", agile::root::integer);

    std::cout << "Pulling dataset from ROOT file...";
    agile::dataframe D = TR.get_dataframe(1000);

    // std::cout << "D = " << agile::knit(D[0]) << "\n reader = " << (TR(0 , TR.get_ordered_branch_names())) << std::endl;

    std::cout << "Done." << std::endl;

    std::string formula = (argc <= 3) ? "bottom + light + charm ~ * -eta -pt": std::string(argv[3]);

    std::cout << "Formula: " << formula << std::endl;

    agile::neural_net arch;
    std::cout << "Adding dataset to the neural net...";
    arch.add_data(D);
    std::cout << "Done." << std::endl;
    std::cout << "Parsing and extracting formula...";
    arch.model_formula(formula);
    std::cout << "Done." << std::endl;

    std::cout << "Forming Network Structure...";

    arch.emplace_back(new autoencoder(25, 40, sigmoid)); 
    arch.emplace_back(new autoencoder(40, 30, sigmoid)); 
    arch.emplace_back(new autoencoder(30, 20, sigmoid)); 
    arch.emplace_back(new autoencoder(23, 10, sigmoid)); 
    arch.emplace_back(new autoencoder(10, 5, softmax)); 

    arch.check(0);



    arch.set_learning(0.01);
    arch.set_regularizer(0.001);
    arch.set_batch_size(1);



    std::cout << "Done." << std::endl;

    

    std::cout << "Unsupervised Learning...";

    arch.train_unsupervised(15);

    arch.to_yaml("neural_network_pretrain.yaml");

    arch.set_learning(0.001);
    
    int epochs = 300;

    std::cout << "Done." << std::endl;

    std::cout << "Supervised Learning...";

    arch.train_supervised(epochs);

    std::cout << "Done." << std::endl;

    arch.to_yaml("neural_network_refined.yaml");



    // ----------------------------------------------------------------------------
    // This is cross check stuff

    agile::model_frame Model;

    Model.add_dataset(D);
    Model.model_formula(formula);
    Model.generate();
    Model.load_scaling(arch.get_scaling());
    // Model.scale();

    auto X = Model.X();
    auto Y = Model.Y();


    std::cout << "within model = " << arch.predict(X.row(0)) << std::endl;
    // std::cout << X.row(0) << std::endl;
    std::cout << "in reader = " << arch.predict_map(TR(0, arch.get_inputs())) << std::endl;
    //----------------------------------------------------------------------------

    // for (int point = 0; point < 6; ++point)
    // {
    //     agile::rowvec r = arch.predict(X.row(point));
    //     std::cout << "predicted:\n" << r << "\nactual:\n" << Y.row(point) << std::endl;
    // }

    // std::cout << "loaded:\n";

    
    // agile::neural_net ARCH;
    // ARCH.from_yaml("neural_network_refined.yaml");

    // for (int point = 0; point < 6; ++point)
    // {
    //     agile::rowvec r = ARCH.predict(X.row(point));
    //     std::cout << "predicted:\n" << r << "\nactual:\n" << Y.row(point) << std::endl;
    // }

    // for (int point = 0; point < 6; ++point)
    // {
    //     auto r = arch.predict_map(TR(point, arch.get_inputs()));
        
    //     auto o = TR(point, arch.get_outputs());

    //     std::cout << "predicted:\n" << r << "\nactual:\n" << o << std::endl;
    // }

    // std::cout << "loaded:\n";

    
    // agile::neural_net ARCH;
    // ARCH.from_yaml("neural_network_refined.yaml");

    // for (int point = 0; point < 6; ++point)
    // {
    //     auto r = ARCH.predict_map(TR(point, ARCH.get_inputs()));
        
    //     auto o = TR(point, ARCH.get_outputs());

    //     std::cout << "predicted:\n" << r << "\nactual:\n" << o << std::endl;
    // }


    // ARCH.to_yaml("neural_network_refined_2.yaml");

    // auto mymap = TR(0, ARCH.get_inputs());

    // std::cout << "X = " << X.row(0) << std::endl;

    // std::cout << mymap << std::endl;

    // std::cout << "" << std::endl;

    // auto pred = ARCH.predict_map(mymap);

    // std::cout << pred << std::endl;


    return 0;
}