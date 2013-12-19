#include <iostream>
#include <Eigen/Dense>


class Activator
{
public:
	Activator();
	~Activator();
private:
	fragile::vector (*m_activation)(const fragile::vector&);
	fragile::vector (*m_activation_derivative)(const fragile::vector&);
};