#include "tree_reader.hh"
#include <iomanip>

int main(int argc, char const *argv[])
{
    std::string tree_name(argv[2]);
    std::string file(argv[1]);

    root::tree_reader D;

    D.add_file(file, tree_name);

    D.set_branch("pt", root::double_precision);
    D.set_branch("ip3d_pb", root::double_precision);
    D.set_branch("ip3d_pu", root::double_precision);
    D.set_branch("ip3d_pc", root::double_precision);
    D.set_branch("eta", root::double_precision);
    D.set_branch("subMaxSecondaryVertexRho", root::double_precision);
    D.set_branch("MV1", root::double_precision);
    D.set_branch("jfit_nvtx", root::integer);

    agile::dataframe DF = D.get_dataframe(20);

    std::ofstream out("example_root.csv");

    out << DF;

    out.close();

    for (auto &row : DF.raw())
    {
        row is a vector, and you can do shit with it.
    }





    return 0;
}