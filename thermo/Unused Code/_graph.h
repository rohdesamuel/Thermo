#pragma once
#ifdef _GRAPH__H
#define _GRAPH__H

#include <iostream>
#include <vector>
using namespace std;

typedef int Vertex;

template <class T>
class Graph
{
public:
	Graph(void);
	Graph(const Graph & other);
	~Graph(void);
	
	Graph<T> & operator=(const Graph<T> & other);

	Vertex insertVertex(void);
	void removeVertex(Vertex v);
	void insertEdge(Vertex source, Vertex dest);
	void removeEdge(Vertex source, Vertex dest);
	vector<Vertex> getAdjacent(Vertex v);
	vector<Vertex> getIn(Vertex v);
	vector<Vertex> getOut(Vertex v);
	int getDegree(Vertex v);
	Graph<float> normalize();

	void print();

	int size;
private:

	class element
	{
	public:
		element( ):data(0), connection(0){ };
		T data;
		int connection;
	};

	element ** vertices;
	int * degree;
	int capacity;
	void clear();
	void copy(const Graph<T> & other);
};


template <class T>
Graph<T>::Graph(void):capacity(1),size(0)
{
	degree = new int [1];
	*degree = 0;
	vertices = new element * [1];
	*vertices = new element [1];
	**vertices = element();
}

template <class T>
Graph<T>::Graph(const Graph & other)
{
	copy(other);
}

template <class T>
Graph<T>::~Graph(void)
{
	clear();
}

template <class T>
Vertex Graph<T>::insertVertex(void)
{
	int new_size = size + 1;
	if (new_size > capacity)
	{
		capacity *= 2;

		int * new_degree = new int [capacity];
		element ** new_vertices = new element* [capacity];
		for (int i = 0; i < capacity; i ++)
			new_vertices[i] = new element [capacity];

		for (int i = 0; i < new_size; i ++)
		{
			if (i < size)
				new_degree[i] = degree[i];
			else
				new_degree[i] = 0;
			for (int j = 0; j < new_size; j ++)
			{
				if (i < size && j < size)
					new_vertices[i][j] = vertices[i][j];
				else
					new_vertices[i][j] = 0;
			}
		}
		clear();
		vertices = new_vertices;
		degree = new_degree;
	}
	size = new_size;
	return size;
}

template <class T>
void Graph<T>::removeVertex(Vertex v)
{
	if (size > 0 && v >= 0 && v < size)
	{
		int new_size = size - 1;

		for (int i = 0; i < size; i ++)
		{
			removeEdge(i, v);
			removeEdge(v, i);
		}

		for (int j = v; j < new_size; j ++)
		{
			for (int i = 0; i < size; i ++)
				vertices[i][j] = vertices[i][j+1];
		}

		for (int i = v; i < new_size; i ++)
		{
			for (int j = 0; j < size; j ++)
				vertices[i][j] = vertices[i+1][j];
		}
		
		for (int i = v; i < new_size; i ++)
			degree[i] = degree[i+1];

		if (new_size < capacity / 2)
		{
			capacity = capacity / 2 == 0 ? 1 : capacity / 2;

			int * new_degree = new int [capacity];
			element ** new_vertices = new element* [capacity];
			for (int i = 0; i < capacity; i ++)
				new_vertices[i] = new element [capacity];

			for (int i = 0; i < new_size; i ++)
			{
				if (i < size)
					new_degree[i] = degree[i];
				else
					new_degree[i] = 0;

				for (int j = 0; j < new_size; j ++)
				{
					if (i < size && j < size)
						new_vertices[i][j] = vertices[i][j];
					else
						new_vertices[i][j] = element();
				}
			}
			clear();
			vertices = new_vertices;
			degree = new_degree;
		}
		size = new_size;
	}
}

template <class T>
void Graph<T>::insertEdge(Vertex source, Vertex dest)
{
	if (source < size && dest < size)
	{
		vertices[source][dest].connection ++;
		degree[source] ++;
		degree[dest] ++;
	}
}

template <class T>
void Graph<T>::removeEdge(Vertex source, Vertex dest)
{
	if (source < size && dest < size && vertices[source][dest].connection > 0)
	{
		degree[source] -= (vertices[source][dest]).connection;
		degree[dest] -= vertices[source][dest].connection;
		vertices[source][dest] = element();
	}
}

template <class T>
vector<Vertex> Graph<T>::getAdjacent(Vertex v)
{
	vector<Vertex> adjacent;
	for (int i = 0; i < size; i ++)
	{
		if (vertices[v][i].connection > 0 || vertices[i][v].connection > 0)
			adjacent.push_back(i);
	}
	return adjacent;
}

template <class T>
vector<Vertex> Graph<T>::getOut(Vertex v)
{
	vector<Vertex> out;
	for (int i = 0; i < size; i ++)
	{
		if (vertices[v][i].connection > 0)
			out.push_back(i);
	}
	return out;
}

template <class T>
vector<Vertex> Graph<T>::getIn(Vertex v)
{
	vector<Vertex> in;
	for (int i = 0; i < size; i ++)
	{
		if (vertices[i][v].connection > 0)
			in.push_back(i);
	}
	return in;
}

template <class T>
int Graph<T>::getDegree(Vertex v)
{
	return degree[v];
}

template <class T>
Graph<float> Graph<T>::normalize()
{
	Graph<float> normal;
}

template <class T>
void Graph<T>::clear()
{
	if (degree != NULL)
		delete degree;
	degree = NULL;

	if (vertices != NULL)
	{
		for (int i = 0; i < size; i ++)
			delete [] vertices[i];
		delete[] vertices;
	}
	vertices = NULL;
}

template <class T>
void Graph<T>::copy(const Graph<T> & other)
{
	size = other.size;
	capacity = other.capacity;
	
	degree = new int [capacity];
	vertices = new element* [capacity];
	for (int i = 0; i < capacity; i ++)
		vertices[i] = new element [capacity];

	for (int i = 0; i < size; i ++)
	{
		degree[i] = other.degree[i];
		for (int j = 0; j < size; j ++)
			vertices[i][j] = other.vertices[i][j];
	}
}

template <class T>
Graph<T> & Graph<T>::operator=(const Graph<T> & other)
{
	if (this != &other)
	{
		clear();
		copy(other);
	}
	return *this;
}

template <class T>
void Graph<T>::print()
{
	for (int i = 0; i < size; i ++)
	{
		for (int j = 0; j < size; j ++)
			cout<<vertices[i][j].connection<<" ";
		cout<<degree[i];
		cout<<endl;
	}
}

#endif