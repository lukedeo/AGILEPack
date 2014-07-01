#include "AGILEPack"
#include <sstream>

namespace agile
{


class network_client
{
public:
	network_client();
	network_client(const std::string &filename);
	// network_client(std::stringstream &s);
	~network_client() = default;
	void load(const std::string &filename);
	void load(std::stringstream &s);
	std::map<std::string, double> predict(const std::map<std::string, double> &x);

	std::vector<std::string> inputs();

private:
	agile::neural_net net;
};

network_client::network_client() {}

network_client::network_client(const std::string &filename) 
{
	net.from_yaml(filename);
}

inline void network_client::load(const std::string &filename)
{
	net.from_yaml(filename);
}

inline void network_client::load(std::stringstream &s)
{
	net.from_yaml(s);
}


inline std::map<std::string, double> network_client::predict(const std::map<std::string, double> &x)
{
	return std::move(net.predict_map(x));
}

inline std::vector<std::string> network_client::inputs()
{
	return net.get_inputs();
}


}