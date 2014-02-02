#include "dataframe.hh"
#include "model_frame.hh"
#include "tree_reader.hh"
#include "architecture.hh"
#include "layer.hh"
#include "autoencoder.hh"
#include <fstream>

int main(int argc, char const *argv[])
{
    // auto in_file = std::string(argv[1]);
    // agile::dataframe X_(in_file, true);

    // auto out_file = std::string(argv[2]);
    // agile::dataframe Y_(out_file, true);

    // agile::matrix X = eigen_spew(X_);
    // agile::matrix Y = eigen_spew(Y_);

    auto in_file = std::string(argv[1]);
    auto tree_name = std::string(argv[2]);

    root::tree_reader TR;

    TR.add_file(in_file, tree_name);
    // TR.set_branch("pt", root::double_precision);

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

    agile::dataframe D = TR.get_dataframe(2000);

    std::string formula = (argc <= 3) ? "bottom + light + charm ~ * -eta": std::string(argv[3]);

    agile::model_frame Model;

    Model.add_dataset(D);
    Model.model_formula(formula);
    Model.generate();
    Model.scale();

    auto X = Model.X();
    auto Y = Model.Y();

    architecture arch;

    arch.emplace_back(new autoencoder(X.cols(), 40, sigmoid)); 
    arch.emplace_back(new autoencoder(40, 30, sigmoid)); 
    arch.emplace_back(new autoencoder(30, 20, sigmoid)); 
    arch.emplace_back(new autoencoder(20, 10, sigmoid)); 
    arch.emplace_back(new autoencoder(10, 3, softmax)); 

    arch.set_learning(0.01);
    arch.set_regularizer(0.001);
    arch.set_batch_size(1);

    for (int l = 0; l < 4; ++l)
    {
        for (int i = 0; i < 100; ++i)
        {
            for (int point = 0; point < X.rows(); ++point)
            {
                arch.encode(X.row(point), l, false);
            }
        }
    }

    arch.set_learning(0.05);
    
    int epochs = 300;

    for (int i = 0; i < epochs; ++i)
    {
        for (int point = 0; point < X.rows(); ++point)
        {
            if(Y.row(point).sum() > 0)
            {
                arch.correct(X.row(point), Y.row(point));
            }
        }
    }

    std::ofstream file("network.yaml");
    YAML::Emitter out;

    YAML::Node net;
    net["network"] = arch;
    out << net;
    file << out.c_str();
    file.close();
    for (int point = 0; point < 100; ++point)
    {
        // arch.at(0)->encode(M.row(point), true);
        // aut.encode(M.row(point), true);
        // std::cout << "input:\n" <<  M.row(point) << "\n";
        // agile::rowvec rv(arch.at(1)->get_encoding(arch.at(0)->get_encoding(M.row(point))));
        // std::cout <</* "encoding:\n" << */ rv<< "\n";
        // std::cout << "reconstructed:\n" << arch.at(0)->reconstruct(M.row(point), false) << "\n";
        // std::cout << "reconstructed:\n" << aut.reconstruct(M.row(point)) << "\n";
        // std::cout << "original:\n" << Y.row(point) << ", predicted: \n";
        agile::rowvec r = arch.predict(X.row(point));
        std::cout << "predicted:\n" << r << "\nactual:\n" << Y.row(point) << std::endl;
        // getchar();
    }
    return 0;
}