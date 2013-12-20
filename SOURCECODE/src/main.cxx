#include "layer.hh"

int main(int argc, char const *argv[])
{
	layer l1(8, 13);
	layer l2(13, 7);
	layer l3(7, 3);
	agile::vector in(8);

	in(0) = 1;
	in(1) = 2;
	in(2) = 3.3;
	in(3) = 0.4;
	in(4) = 1;
	in(5) = 2;
	in(6) = 3.3;
	in(7) = 0.4;

	// std::cout << in << std::endl;

	l1.charge(in);
	l2.charge(l1.fire());
	l3.charge(l2.fire());

	agile::vector out(l3.fire());


	std::cout << out << std::endl;


	return 0;
}