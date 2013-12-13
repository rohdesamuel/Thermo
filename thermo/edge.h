#ifndef EDGE__H
#define EDGE__H

template <int Dim>
class Point;

template <int Dim>
class Edge
{
public:
	Edge(): source(NULL), dest(NULL), weight(1), normal(0) { };
	Edge(Point<Dim> * Source, Point<Dim> * Dest): source(Source), dest(Dest), weight(1), normal(0){ };
	
	int weight;
	float normal;
	Point<Dim> * source;
	Point<Dim> * dest;

	struct __weight_compare {
		bool operator()( Edge<Dim> * e1, Edge<Dim> * e2 ){ return(e1->weight > e2->weight); };
	};

	struct __normal_compare {
		bool operator()( Edge<Dim> * e1, Edge<Dim> * e2 ){ return(e1->normal > e2->normal); };
	};
};

#endif