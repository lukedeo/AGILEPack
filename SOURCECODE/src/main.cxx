#include "architecture.hh"
#include "layer.hh"
#include <fstream>
#include "base64.hh"
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

	layer l1(2, 4, sigmoid);
	layer l2(4, 3, sigmoid);
	layer l3(3, 1, sigmoid);
	// layer l3(4, 3);

	// layer l4(20, 30, softmax);


	// YAML::Emitter out;

	// out << YAML::BeginMap;
	// out << YAML::Key << "Layer" << YAML::Value << l3;
	// out << YAML::EndMap;
	// std::cout << out.c_str();



	// out << YAML::BeginMap;
	// out << YAML::Key << "Layer New" << YAML::Value << NEW;
	// out << YAML::EndMap;
	// std::cout << out.c_str();

	arch.push_back(l1);
	arch.push_back(l2);
	arch.push_back(l3);

	// std::cout << "Untrained: " << std::endl;

	// for (int i = 0; i < 4; ++i)
	// {
	// 	std::cout << "input: " << X.row(i) << ", output: \n" << arch.predict(X.row(i)) << "\n, truth: \n" << T.row(i) << std::endl;
	// }

	std::ofstream file("network.yaml");

	for (int i = 0; i < 4000; ++i)
	{
		for (int point = 0; point < 4; ++point)
		{
			arch.correct(X.row(point), T.row(point));
		}
	}

	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "network" << YAML::Value << arch;
	out << YAML::EndMap;
	file << out.c_str();
	file.close();

	// std::string yaml_file(out.c_str());

	// unsigned char hash[20];
	// sha1::calc(yaml_file.c_str(),yaml_file.size(),hash); // 10 is the length of the string

	// std::string hash_string(reinterpret_cast<const char*>(hash));


	// std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(hash_string.c_str()), hash_string.length());

	std::cout << "Original: " << std::endl;

	for (int i = 0; i < 4; ++i)
	{
		std::cout << "input: " << X.row(i) << ", output: \n" << arch.predict(X.row(i)) << "\n, truth: \n" << T.row(i) << std::endl;
	}


	// arch.pop_back();

	// YAML::Node config = YAML::LoadFile("layer.yaml");
	
	// layer NEW = config["Layer"].as<layer>();
	// std::cout << NEW.W << std::endl;
	// std::cout << NEW.b << std::endl;
	// arch.push_back(NEW);


	// std::cout << "loaded: " << std::endl;

	// for (int i = 0; i < 4; ++i)
	// {
	// 	std::cout << "input: " << X.row(i) << ", output: \n" << arch.predict(X.row(i)) << "\n, truth: \n" << T.row(i) << std::endl;
	// }

	// std::cout << "encoded: " << encoded << std::endl;

	return 0;
}
