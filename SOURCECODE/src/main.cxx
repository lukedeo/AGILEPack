#include "architecture.hh"
#include "layer.hh"
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


	// architecture arch(1);

	// arch.at(0).construct(2, 4, sigmoid);

	// // layer l1(2, 4, sigmoid);
	// layer l2(4, 3, sigmoid);
	// layer l3(3, 1, sigmoid);
	// arch.push_back(l2);
	// arch.push_back(l3);


	// std::ofstream file("network.yaml");

	// for (int i = 0; i < 4000; ++i)
	// {
	// 	for (int point = 0; point < 4; ++point)
	// 	{
	// 		arch.correct(X.row(point), T.row(point));
	// 	}
	// }

	// YAML::Emitter out;

	// out << YAML::BeginMap;
	// out << YAML::Key << "network" << YAML::Value << arch;
	// out << YAML::EndMap;
	// file << out.c_str();
	// file.close();

	// std::cout << "Original: " << std::endl;

	// for (int i = 0; i < 4; ++i)
	// {
	// 	std::cout << "input: " << X.row(i) << ", output: \n" << arch.predict(X.row(i)) << "\n, truth: \n" << T.row(i) << std::endl;
	// }


	// arch.pop_back();

	YAML::Node config = YAML::LoadFile("network.yaml");
	
	architecture ARCH = config["network"].as<architecture>();
	// std::cout << NEW.W << std::endl;
	// std::cout << NEW.b << std::endl;
	// arch.push_back(NEW);

	layer l = ARCH.at(6); // make sure this throws an error correctly


	std::cout << "loaded: " << std::endl;

	for (int i = 0; i < 4; ++i)
	{
		std::cout << "input: " << X.row(i) << ", output: \n" << ARCH.predict(X.row(i)) << "\n, truth: \n" << T.row(i) << std::endl;
	}

	// std::cout << "encoded: " << encoded << std::endl;

	return 0;
}
