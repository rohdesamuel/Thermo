#include "binomial.h"


Binomial::Binomial(int Max)
{
	max = Max+1;
	table = new double * [max]();
	for (int i=0; i < max; i++)
	{
		table[i] = new double[max]();
		for (int j = 0; j < max; j++)
			table[i][j] = 0;
	}

	for (int i = 0; i < 10; i ++)
		for (int j = 0; j < 10; j ++)
		{
			_choose(i, j);
		}
}

Binomial::~Binomial()
{
	for (int i =0; i < max; i++)
		delete table[i];
	delete table;
}

double Binomial::privChoose(unsigned int n, unsigned int k)
{
	return 0;
}

double Binomial::choose(unsigned int n, unsigned int k)
{
	if (n < k) return 0;
	if (k == 0 || n==1 ) return 1;
	if (n==2 && k==1) return 2;
	if (n==k) return 1;


	if (contains(n,k))
		return table[n][k];
    
	double val = 1.0f;
	val *= choose(n % 9, k % 9) * choose(n/9, k/9);
	table[n][k] = val;
	return table[n][k];
}

double Binomial::_choose(unsigned int n, unsigned int k)
{
	if (n < k) return 0;
	if (k == 0 || n==1 ) return 1;
	if (n==2 && k==1) return 2;
	if (n==2 && k==2) return 1;
	if (n==k) return 1;


	if (contains(n,k))
	{
		return table[n][k];
	}
	table[n][k] = choose(n-1,k) + choose(n-1,k-1);
	return table[n][k];
}

bool Binomial::contains(unsigned int n, unsigned int k)
{
	if (table[n][k] == 0) 
		return false;
	return true;
}