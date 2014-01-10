#include "architecture.hh"
#include "layer.hh"
#include "autoencoder.hh"
#include <fstream>
#include "sha1.hh"

int main(int argc, char const *argv[])
{
	agile::matrix X(4, 2);

	X << 0.0, 0.0,
	     0.0, 1.0,
	     1.0, 0.0, 
	     1.0, 1.0;

	agile::matrix T(4, 1);
	T << 0.0,
	     1.0,
	     1.0,
	     0.0;


	architecture arch; 

	auto aut1 = autoencoder(2, 4, sigmoid); // can set parms this way
	auto aut = autoencoder(2, 4, sigmoid); // can set parms this way

	// arch.add_layer(layer_factory<autoencoder>(2, 4, sigmoid));

	// arch.add_a_layer<autoencoder>(2, 4, sigmoid);
	arch.add_layer(new autoencoder(2, 4, sigmoid));

	for (int i = 0; i < 1000; ++i)
	{
		for (int point = 0; point < 4; ++point)
		{
			arch.at(0)->encode(X.row(point), false);
			// aut.encode(X.row(point), false);
		}
	}
	for (int point = 0; point < 4; ++point)
	{
		arch.at(0)->encode(X.row(point), true);
		// aut.encode(X.row(point), true);
		std::cout << "input:\n" <<  X.row(point) << "\n";
		std::cout << "reconstructed:\n" << arch.stack.at(0)->reconstruct(X.row(point)) << "\n";
		// std::cout << "reconstructed:\n" << aut.reconstruct(X.row(point)) << "\n";
		getchar();
	}

	// arch.add_layer(aut);

	// arch.add_layer(2, 4, sigmoid);

	arch.add_layer(4, 3, sigmoid);
	arch.add_layer(3, 1, sigmoid);



	// architecture arch({2, 4, 3, 1}, classify);


	std::ofstream file("network.yaml");

	// train it
	for (int i = 0; i < 8000; ++i)
	{
		for (int point = 0; point < 4; ++point)
		{
			arch.correct(X.row(point), T.row(point));
		}
	}

	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "network" << YAML::Value << arch; // save it
	out << YAML::EndMap;
	file << out.c_str();
	file.close();


	//see what it predicts
	std::cout << "Original: " << std::endl;

	for (int i = 0; i < 4; ++i)
	{
		std::cout << "input: " << X.row(i) << ", output: \n" << arch.predict(X.row(i)) << "\n, truth: \n" << T.row(i) << std::endl;
	}



	// now load it and cross check

	// YAML::Node config = YAML::LoadFile("network.yaml");
	
	// architecture ARCH = config["network"].as<architecture>();

	// // layer l = ARCH.at(6); // make sure this throws an error correctly

	// std::cout << "loaded: " << std::endl;

	// for (int i = 0; i < 4; ++i)
	// {
	// 	std::cout << "input: " << X.row(i) << ", output: \n" << ARCH.predict(X.row(i)) << "\n, truth: \n" << T.row(i) << std::endl;
	// }

	// std::cout << "encoded: " << encoded << std::endl;

	return 0;
}
