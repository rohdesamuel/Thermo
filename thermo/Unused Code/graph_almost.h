#if 0
#ifndef GRAPH__H
#define GRAPH__H

#include <vector>

using namespace std;

#include "vertex.h"

template<int Dim>
class Graph
{
public:
	Graph();
	~Graph();
	Vertex<Dim> * insertVertex();
	Vertex<Dim> * insertVertex(Vertex<Dim> * vertex);

	void insertVertices(int num);
	void insertVertices(const vector<Vertex<Dim> *> & vertices);
	void removeVertex(Vertex<Dim> * vertex);
	void insertEdge(Vertex<Dim> * source, Vertex<Dim> * dest);
	void removeEdge(Vertex<Dim> * source, Vertex<Dim> * dest);
	vector<Vertex<Dim>*> getIn(Vertex<Dim> * v) const;
	vector<Vertex<Dim>*> getOut(Vertex<Dim> * v) const;
	vector<Vertex<Dim>*> getAdjacent(Vertex<Dim> * v) const;
	vector<Vertex<Dim>*> getVertices() const;

private:
	Vertex<Dim> ** vertices;
	int size;
	int capacity;
};

template<int Dim>
Graph<Dim>::Graph():size(0), capacity(1)
{
	vertices = new Vertex<Dim>*;
	*vertices = new Vertex<Dim>(0);
}

template<int Dim>
Graph<Dim>::~Graph()
{
	for (int i = 0; i < size; i ++)
		delete vertices[i];
	delete [] vertices;
}

template<int Dim>
Vertex<Dim> * Graph<Dim>::insertVertex()
{
	int new_size = size + 1;
	if (new_size > capacity)
	{
		capacity *= 2;
		Vertex<Dim> ** new_vertices = new Vertex<Dim>* [capacity];
		for(int i = 0; i < size; i ++)
			new_vertices[i] = vertices[i];
		new_vertices[size] = new Vertex<Dim>(size);
		delete [] vertices;
		vertices = new_vertices;
	}
	size = new_size;
	return vertices[size-1];
}

template<int Dim>
Vertex<Dim> * Graph<Dim>::insertVertex(Vertex<Dim> * vertex)
{
	int new_size = size + 1;
	if (new_size > capacity)
	{
		capacity *= 2;
		Vertex<Dim> ** new_vertices = new Vertex<Dim> * [capacity];
		for(int i = 0; i < size; i ++)
			new_vertices[i] = vertices[i];
		delete [] vertices;
		vertices = new_vertices;
	}
	vertex->index = size;
	vertices[size] = vertex;
	size = new_size;
	return vertices[size-1];
}

template<int Dim>
void Graph<Dim>::insertVertices(int num)
{
	for (int i = 0; i < num; i ++)
		insertVertex();
}

template<int Dim>
void Graph<Dim>::insertVertices(const vector<Vertex<Dim> *> & vertices)
{
	for (int i = 0; i < vertices.size(); i ++)
		insertVertex(vertices[i]);
}

template<int Dim>
void Graph<Dim>::removeVertex(Vertex<Dim> * vertex)
{
	if (vertex != NULL)
	{
		int v = vertex->index;
		int new_size = size - 1;
		Vertex<Dim> * tmp = vertices[new_size];
		tmp->index = vertex->index;
		vertices[new_size] = vertex;
		vertices[vertex->index] = tmp;
		delete vertices[new_size];

		if (new_size < capacity / 2)
		{
			capacity = capacity / 2 == 0 ? 1 : capacity / 2;
			Vertex ** new_vertices = new Vertex<Dim>* [capacity];
			for(int i = 0; i < new_size; i ++)
				new_vertices[i] = vertices[i];
			delete [] vertices;
			vertices = new_vertices;
		}

		size = new_size;
		if (size == 0)
			*vertices = new Vertex<Dim>(0);
	}
}

template<int Dim>
void Graph<Dim>::insertEdge(Vertex<Dim> * source, Vertex<Dim> * dest)
{
	if (source != NULL && dest != NULL)
	{
		source->insertEdge( source, dest );
		dest->insertEdge( source, dest );
	}
}

template<int Dim>
void Graph<Dim>::removeEdge(Vertex<Dim> * source, Vertex<Dim> * dest)
{
	if (source != NULL && dest != NULL)
	{
		source->removeEdge( source, dest );
		dest->removeEdge( source, dest );
	}
}

template<int Dim>
vector<Vertex<Dim>*> Graph<Dim>::getIn(Vertex<Dim> * v) const
{
	if (v != NULL)
		return v->getIn();
	else
		return vector<Vertex<Dim>*>();
}

template<int Dim>
vector<Vertex<Dim>*> Graph<Dim>::getOut(Vertex<Dim> * v) const
{
	if (v != NULL)
		return v->getOut();
	else
		return vector<Vertex<Dim>*>();
}

template<int Dim>
vector<Vertex<Dim>*> Graph<Dim>::getAdjacent(Vertex<Dim> * v) const
{
	if (v != NULL)
		return v->getAdjacent();
	else
		return vector<Vertex<Dim>*>();
}

template<int Dim>
vector<Vertex<Dim>*> Graph<Dim>::getVertices() const
{
	vector<Vertex<Dim>*> vxs;
	for (int i = 0; i < size; i ++)
		vxs.push_back(vertices[i]);
	return vxs;
}

#endif
#endif