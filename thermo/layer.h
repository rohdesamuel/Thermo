#ifndef LAYER__H
#define LAYER__H

#include <math.h>
#include <algorithm>

#include "node.h"

template <typename FilterTy, int Dim, int RField = 0>
class Layer
{ 
public:
	Layer(void);
	Layer(Point<Dim> * input, FilterTy * Filter);
	~Layer(void);

	void learn();
	void inference();

	Point<Dim> output;

private:
	int nodeSize;

	Point<Dim> * input;
	vector<Node<RField> > nodes;
};

template <typename FilterTy, int Dim, int RField>
Layer<FilterTy, Dim, RField>::Layer(void):nodeSize(0)
{
	input = NULL;
}

template <typename FilterTy, int Dim, int RField>
Layer<FilterTy, Dim, RField>::Layer(Point<Dim> * input, FilterTy * filter):input(input)
{
	if (RField != 0)
	{
		nodeSize = Dim / RField;
		nodes.reserve(nodeSize);
		for (int i = 0; i < nodeSize; i ++)
			nodes.push_back( Node<RField>( new FilterTy(*filter)) );
	}
}

template <typename FilterTy, int Dim, int RField>
Layer<FilterTy, Dim, RField>::~Layer()
{
}

template <typename FilterTy, int Dim, int RField>
void Layer<FilterTy, Dim, RField>::learn()
{
	for (int i = 0; i < nodeSize; i ++)
	{
		Point<RField> nodeIn;
		for (int j = 0; j < RField; j ++)
			nodeIn.set(j, input->get((RField * i) + j ));
		nodes[i].learn(&nodeIn);
	}	
}

template <typename FilterTy, int Dim, int RField>
void Layer<FilterTy, Dim, RField>::inference()
{
	for (int i = 0; i < nodeSize; i ++)
	{
		Point<RField> nodeIn;
		for (int j = 0; j < RField; j ++)
			nodeIn.set(j, input->get((RField * i) + j ));
		nodes[i].inference(&nodeIn);
	}	
}

#endif