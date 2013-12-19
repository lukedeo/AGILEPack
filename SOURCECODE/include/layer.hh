#include <iostream>
#include "basedefs.hh"

class layer
{
public:
	layer(int n_inputs, int n_outputs);

	void reset_weights(numeric bound);
	void perturb_weights(numeric bound);

	void set_batch_size(int size);

	void charge(agile::vector);
	agile::vector fire();


	~layer();
	
private:
	int m_inputs, m_outputs, m_batch_size;
	agile::matrix W, W_old;
	agile::vector b, b_old, m_outputs, ;
};


