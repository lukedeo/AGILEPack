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
    TR.set_branch("pt", root::double_precision);
    TR.set_branch("ip3d_pb", root::double_precision);
    TR.set_branch("ip3d_pu", root::double_precision);
    TR.set_branch("ip3d_pc", root::double_precision);
    TR.set_branch("bottom", root::integer);
	TR.set_branch("charm", root::integer);
	TR.set_branch("light", root::integer);
    TR.set_branch("eta", root::double_precision);
    TR.set_branch("subMaxSecondaryVertexRho", root::double_precision);
    TR.set_branch("MV1", root::double_precision);
    TR.set_branch("jfit_nvtx", root::integer);

    agile::dataframe D = TR.get_dataframe(100);



    // std::ofstream out("example_root.csv");

    // out << D;

    // out.close();

    auto formula = std::string(argv[3]);

    agile::model_frame Model;

    Model.add_dataset(D);
    Model.model_formula(formula);
    Model.generate();
    Model.scale();



    auto X = Model.X();
    auto Y = Model.Y();

    std::cout << X << std::endl;

	architecture arch;
    arch += new autoencoder(X.cols(), 9, sigmoid);
    arch += new layer(9, 7, sigmoid);
    arch += new layer(7, 1, sigmoid);
    // std::cout << "here" << std::endl;

    arch.set_learning(0.1);
	arch.set_regularizer(0.00001);
	// arch.set_momentum(0.8);
	arch.set_batch_size(1);
	// std::cout << "here" << std::endl;


	for (int i = 0; i < 500; ++i)
    {
        for (int point = 0; point < 4; ++point)
        {
            arch.at(0)->encode(X.row(point), true);
            // aut.encode(X.row(point), false);
        }
    }

    

    for (int i = 0; i < 1000; ++i)
    {
    	for (int point = 0; point < X.rows(); ++point)
    	{
    		arch.correct(X.row(point), Y.row(point));
    	}
    }

    std::ofstream file("network.yaml");
    YAML::Emitter out;

	YAML::Node net;
	net["network"] = arch;
	out << net;
	file << out.c_str();
	file.close();
    for (int point = 0; point < X.rows(); ++point)
    {
        // arch.at(0)->encode(M.row(point), true);
        // aut.encode(M.row(point), true);
        // std::cout << "input:\n" <<  M.row(point) << "\n";
        // agile::rowvec rv(arch.at(1)->get_encoding(arch.at(0)->get_encoding(M.row(point))));
        // std::cout <</* "encoding:\n" << */ rv<< "\n";
        // std::cout << "reconstructed:\n" << arch.at(0)->reconstruct(M.row(point), false) << "\n";
        // std::cout << "reconstructed:\n" << aut.reconstruct(M.row(point)) << "\n";
        // std::cout << "original:\n" << Y.row(point) << ", predicted: \n";

        std::cout << /*"predicted:\n" <<*/ arch.predict(X.row(point)) << ", " << Y.row(point) << std::endl;
        // getchar();
    }
	return 0;
}