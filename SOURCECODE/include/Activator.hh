#include <iostream>
#include <Eigen/Dense>


class Activator
{
public:
	Activator();
	~Activator();
	agile::vector activate(const agile::vector& v);
	agile::vector deriv_activate(const agile::vector& v);

private:
	bool m_special_derivative, m_lin_out, m_soft_out;
	agile::vector (*m_activation)(const agile::vector&);
	agile::vector (*m_activation_derivative)(const agile::vector&);
};