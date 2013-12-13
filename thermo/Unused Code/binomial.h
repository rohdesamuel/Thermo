#ifndef BINOMIAL__H
#define BINOMIAL__H

class Binomial
{
public:
	Binomial(int Max);
	~Binomial();

	double choose(unsigned int n, unsigned int k);
	double _choose(unsigned int n, unsigned int k);
	

private:
	double privChoose(unsigned int n, unsigned int k);
	bool contains(unsigned int n, unsigned int k);
    
	int max;
	double **table;
};

#endif