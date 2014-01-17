#include "dataset.hh"
#include <iomanip>

int main(int argc, char const *argv[])
{
	std::string tree_name(argv[2]);
	std::string file(argv[1]);
	std::cout << "file: " << file << std::endl;
	std::cout << "tree: " << tree_name << std::endl;

	root::tree_reader D;

	D.add_file(file, tree_name);

	std::cout << "There are " << D.size() << " entries." << std::endl;


	D.set_branch("pt", root::double_precision);
	D.set_branch("ip3d_pb", root::double_precision);
	D.set_branch("ip3d_pu", root::double_precision);
	D.set_branch("ip3d_pc", root::double_precision);
	D.set_branch("eta", root::double_precision);
	D.set_branch("subMaxSecondaryVertexRho", root::double_precision);
	D.set_branch("MV1", root::double_precision);
	D.set_branch("jfit_nvtx", root::integer);
	

	for (int i = 0; i < 100; ++i)
	{
		std::vector<double> v = D[i];

		for (auto &entry : v)
		{
			std::cout << entry << std::setw(20);
		}
		std::cout << "\n";
	}



	return 0;
}