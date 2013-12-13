#ifndef GRAPH__H
#define GRAPH__H

#include <unordered_map>
#include "point.h"

using namespace std;

template<int Dim>
class Graph
{
public:
	Graph(void);
	~Graph(void);

	Point<Dim> * insertVertex(Point<Dim> * Point);
	void removeVertex(Point<Dim> * Point);
	Edge<Dim>* insertEdge(Point<Dim> * source, Point<Dim> * dest);
	void removeEdge(Point<Dim> * source, Point<Dim> * dest);
	
	Edge<Dim>* operator[](Point<Dim> * point);
	const Edge<Dim>* operator[](Point<Dim> * point) const;

	unordered_map<Point<Dim> *, Edge<Dim> *> graph;

private:
	Edge<Dim>* _insertEdge(Point<Dim> * source, Point<Dim> * dest);
	void _removeEdge(Point<Dim> * source, Point<Dim> * dest);
};

template<int Dim>
Graph<Dim>::Graph(void){ }

template<int Dim>
Graph<Dim>::~Graph(void){ }

template<int Dim>
inline Edge<Dim>* Graph<Dim>::operator[](Point<Dim> * point)
{
	if (graph.find(point) != graph.end())
		return graph[point];
	else
		return NULL;
}

template<int Dim>
inline const Edge<Dim>* Graph<Dim>::operator[](Point<Dim> * point) const
{
	if (graph.find(point) != graph.end())
		return graph[point];
	else
		return NULL;
}

template<int Dim>
Point<Dim> * Graph<Dim>::insertVertex(Point<Dim> * point)
{
	//If not found, insert new vertex
	if (point != NULL)
	{
		//Use a sentinal node for the list
  		if (graph.find(point) == graph.end())
		{
			Edge<Dim> * head = new Edge<Dim>();
			Edge<Dim> * tail = new Edge<Dim>();
			head->source = head->dest = point;
			head->next = tail;
			tail->prev = head;
			graph[point] = head;
		}
	}
	return point;
}

template<int Dim>
void Graph<Dim>::removeVertex(Point<Dim> * point)
{
	//If not found, insert new vertex
	if (point != NULL)
	{
		Edge<Dim> * edge = graph[point];
		point = edge->source;
		while(edge)
		{
			Edge<Dim> * next = edge->next;
			//If there's an incoming edge
			if (edge->source != point && edge->dest == point)
				removeEdge(edge);
			else
				delete edge;
			edge = next;
		}
		delete point;
	}
}

template<int Dim>
Edge<Dim>* Graph<Dim>::insertEdge(Point<Dim> * source, Point<Dim> * dest)
{
	if (source != NULL && dest != NULL)
		return _insertEdge( source, dest );
	return NULL;
}

template<int Dim>
void Graph<Dim>::removeEdge(Point<Dim> * source, Point<Dim> * dest)
{
	if (source != NULL && dest != NULL)
		_removeEdge( source, dest );
}

template<int Dim>
Edge<Dim> * Graph<Dim>::_insertEdge(Point<Dim> * source, Point<Dim> * dest)
{
	Edge<Dim> * edge = NULL;
	if (source != NULL && dest != NULL)
	{
		Edge<Dim> * head = graph[source];
		edge = head;
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
			edge = new Edge<Dim>(source, dest);
			Edge<Dim> * tmp = head->next;
			head->next = edge;
			edge->next = tmp;
			edge->prev = head;
			tmp->prev = edge;
		}
		else
			edge->weight ++;
	}
	return edge;
}

template<int Dim>
void Graph<Dim>::_removeEdge(Point<Dim> * source, Point<Dim> * dest)
{
	if (dest != NULL)
	{
		Edge * edge = graph[source];
		while(edge)
		{
			if (edge->source == source && edge->dest == dest)
			{
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

#endif