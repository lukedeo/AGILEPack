#include "Base"
#include "include/parser.hh"


//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
	std::stringstream s;
	s << "A simple CLI for AGILEPack centered around providing functionality";
	s << " for training and testing Deep Learning models on ROOT TTrees";
	optionparser::parser p(s.str());

	p.add_option("--file", "-f").help(
		"Pass at least one file to add to a TChain for training.")
								.required(true)
								.mode(optionparser::store_mult_values);

	p.add_option("--save", "-s").help(
		"Name of file to save the YAML config file to.")
								.required(false)
								.mode(optionparser::store_value);



	return 0;
}