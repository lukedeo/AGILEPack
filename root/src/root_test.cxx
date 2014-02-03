#include "tree_reader.hh"
#include <iomanip>

int main(int argc, char const *argv[])
{
    std::string tree_name(argv[2]);
    std::string file(argv[1]);

    agile::root::tree_reader D;

    D.add_file(file, tree_name);

    D.set_branch("pt", agile::root::double_precision);
    D.set_branch("ip3d_pb", agile::root::double_precision);
    D.set_branch("ip3d_pu", agile::root::double_precision);
    D.set_branch("ip3d_pc", agile::root::double_precision);
    D.set_branch("eta", agile::root::double_precision);
    D.set_branch("subMaxSecondaryVertexRho", agile::root::double_precision);
    D.set_branch("MV1", agile::root::double_precision);
    D.set_branch("jfit_nvtx", agile::root::integer);

    agile::dataframe DF = D.get_dataframe(20);

    std::ofstream out("example_root.csv");

    out << DF;

    out.close();





    return 0;
}