#include "dataframe.hh"
#include <iostream>

int main(int argc, char const *argv[])
{
	std::cout << "hi" << std::endl;
	auto csv_file = std::string(argv[1]);
	agile::dataframe D(csv_file, true);
	std::cout << "There are " << D.rows() << " rows and " << D.columns() << " columns." << std::endl;

	std::cout << "the column names are:\n";
	for (auto &entry : D.get_column_names())
	{
		std::cout << entry << "   ";
	}
	std::cout << "\nThe second row is:\n";
	
	std::vector<double> v = D.at(1);
	for (auto &entry : v)
	{
		std::cout << entry << "   ";
	}
	std::cout << "\nD.at(1, col2) = " << D.at(1, "col2") << std::endl;
	std::cout << "\nD.at(1, mycol) = " << D.at(1, "mycol") << std::endl;
	return 0;
}