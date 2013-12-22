#include "architecture.hh"
#include "layer.hh"

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
	     1.0;


	architecture arch;

	layer l1(2, 3, sigmoid);
	layer l2(3, 1, sigmoid);

	arch.push_back(l1);
	arch.push_back(l2);


	for (int i = 0; i < 1000; ++i)
	{
		for (int point = 0; point < 4; ++point)
		{
			arch.correct(X.row(point), T.row(point));
		}
	}


	for (int i = 0; i < 4; ++i)
	{
		std::cout << "input: " << X.row(i) << ", output: " << arch.predict(X.row(i)) << ", truth: " << T.row(i) << std::endl;
	}
	return 0;
}
