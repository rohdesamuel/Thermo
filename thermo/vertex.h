#if 0
#ifndef VERTEX__H
#define VERTEX__H

#include <vector>
using namespace std;

#include "edge.h"

template <int Dim>
class Vertex// : public Point<int Dim>
{
public:
	Vertex(): head(new Edge<Dim>()), tail(new Edge<Dim>()), degree(0), index(-1) { 
		head->next = tail; tail->prev = head; 
	};

	Vertex(int Index): head(new Edge<Dim>()), tail(new Edge<Dim>()), degree(0), index(Index) { 
		head->next = tail; tail->prev = head; 
	};

	~Vertex();

	void insertEdge(Vertex<Dim> * source, Vertex<Dim> * dest);
	void removeEdge(Vertex<Dim> * source, Vertex<Dim> * dest);

	vector<Vertex<Dim>*> getIn();
	vector<Vertex<Dim>*> getOut();
	vector<Vertex<Dim>*> getAdjacent();

	int index;
	int degree;

private:
	void removeEdge(Edge<Dim> * edge);
	Edge<Dim> * head;
	Edge<Dim> * tail;
};

template <int Dim>
Vertex<Dim>::~Vertex()
{
	Edge<Dim> * edge = head;
	while(edge)
	{
		Edge<Dim> * next = edge->next;
		//If there's an incoming edge
		if (edge->source != this && edge->dest == this)
			removeEdge(edge);
		else
			delete edge;
		edge = next;
	}
}

template <int Dim>
void Vertex<Dim>::insertEdge(Vertex * source, Vertex * dest)
{
	if (source != NULL && dest != NULL)
	{
		Edge * edge = head;
		bool found = false;
		while(edge)
		{
			if (edge->source == source && edge->dest == dest)
			{
				found = true;	
				break;
			}
			edge = edge->next;
		}
		if (!found)
		{
			Edge * edge = new Edge(source, dest);

			Edge * tmp = head->next;
			head->next = edge;
			edge->next = tmp;
			edge->prev = head;
			edge->weight ++;
			tmp->prev = edge;

			if (source == this)
				degree ++;
		}
		else
			edge->weight ++;
	}
}

template <int Dim>
void Vertex<Dim>::removeEdge(Vertex * source, Vertex * dest)
{
	if (dest != NULL)
	{
		Edge * edge = head;
		while(edge)
		{
			if (edge->source == source && edge->dest == dest)
			{
				if (source == this)
					degree --;

				Edge * h = edge->prev;
				Edge * t = edge->next;
				h->next = t;
				t->prev = h;

				delete edge;
				break;
			}
			edge = edge->next;
		}
	}
}

template <int Dim>
void Vertex<Dim>::removeEdge(Edge<Dim> * edge)
{
	if (edge != NULL)
	{
		degree --;

		Edge<Dim> * h = edge->prev;
		Edge<Dim> * t = edge->next;
		h->next = t;
		t->prev = h;

		delete edge;
	}
}

template <int Dim>
vector<Vertex<Dim>*> Vertex<Dim>::getIn()
{
	vector<Vertex*> in;
	Edge<Dim> * edge = head;
	while(edge)
	{
		if (edge->dest == this)
			in.push_back(edge->source);
		edge = edge->next;
	}
	return in;
}

template <int Dim>
vector<Vertex<Dim>*> Vertex<Dim>::getOut()
{
	vector<Vertex<Dim>*> out;
	Edge<Dim> * edge = head;
	while(edge)
	{
		if (edge->source == this)
			out.push_back(edge->dest);
		edge = edge->next;
	}
	return out;
}

template <int Dim>
vector<Vertex<Dim>*> Vertex<Dim>::getAdjacent()
{
	vector<Vertex<Dim>*> in = getIn();
	vector<Vertex<Dim>*> out = getOut();
	vector<Vertex<Dim>*> adjacent = in;

	for (int i = 0; i < out.size(); i ++)
	{
		bool found = false;
		for (int j = 0; j < adjacent.size(); j ++)
		{
			if (out[i] == adjacent[j])
			{
				found = true;
				break;
			}
		}
		if (!found)
			adjacent.push_back(out[i]);
	}
	return adjacent;
}

#endif
#endif