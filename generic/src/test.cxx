#include "dataframe.hh"
#include "model_frame.hh"
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

    auto in_file = (argv[1]) ? std::string(argv[1]) : std::string("D.csv");
    agile::dataframe D(in_file, true);

    auto formula = (argv[2]) ? std::string(argv[2]) : std::string("Y ~ X");

    agile::model_frame Model;

    Model.add_dataset(D);
    Model.model_formula(formula);
    Model.generate();

    auto X = Model.X();
    auto Y = Model.Y();


	architecture arch;
    arch += new autoencoder(X.cols(), 8, sigmoid);
    arch += new layer(8, 3, sigmoid);
    arch += new layer(3, 1, linear);
    // std::cout << "here" << std::endl;

    arch.set_learning(0.03);
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

        std::cout << /*"predicted:\n" <<*/ arch.predict(X.row(point)) << std::endl;
        // getchar();
    }
	return 0;
}