#include "layer.hh"

int main(int argc, char const *argv[])
{
	layer l1(2, 3);
	agile::vector in(2);

	in(0) = 1;
	in(1) = 2;

	std::cout << in << std::endl;

	l1.charge(in);

	agile::vector out(l1.fire());


	std::cout << out << std::endl;


	return 0;
}