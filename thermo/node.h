#ifndef NODE__H
#define NODE__H

#include <unordered_map>

#include "sparserep.h"
#include "HashGraph.h"
#include "filter.h"
#include "dsets.h"
#include "priorityqueue.h"

#define SpatialOutput std::unordered_map<Point<Dim> *, float>
#define TemporalOutput std::unordered_map<Point<Dim> *, float>
#define SpatialPatterns HashGraph<Dim>
#define TemporalPatterns DSets<Point<Dim>* >
#define TemporalQueue PriorityQueue<Edge<Dim>*, Edge<Dim>*, typename Edge<Dim>::__normal_compare>

typedef enum NODE_STATE {
	NODE_SLEEP,
	NODE_LEARNING,
	NODE_SENSING,
	NODE_SPATIAL,
	NODE_TEMPORAL
};

template <int Dim>
class Node
{
public:
	Node(void):prevInput(NULL), node_state(NODE_SLEEP), sOutputNormal(0){ };
	Node(filters::Filter<Dim, Dim> * filter):filter(filter), prevInput(NULL), node_state(NODE_SLEEP), sOutputNormal(0){ };

	void learn(Point<Dim> * input);
	void inference(Point<Dim> * input);

private:
	//Spatial Patterns
	SpatialPatterns sPatterns;

	//Temporal Patterns
	TemporalPatterns tPatterns;
	TemporalPatterns tPatternBuffer;
	
	//Priority Queue for sorting temporal patterns
	TemporalQueue tQueue;

	//Output Variables
	SpatialOutput sOutput;
	float sOutputNormal;
	//State Variables
	NODE_STATE node_state;
	Point<Dim> * prevInput;

	//Point filter
	filters::Filter<Dim, Dim> * filter; 
};

template <int Dim>
void Node<Dim>::inference(Point<Dim> * input)
{
	//Ready the spatial output
	sOutput.clear();

	//Set the probability distribution
	input = filter->find(input, &sOutput, &sOutputNormal);
}

template <int Dim>
void Node<Dim>::learn(Point<Dim> * input)
{
	//Add input to node filter 
	input = filter->add(input);

	//Don't do anything if returns NULL
	if (input == NULL)
		return;

	//Insert into spatial pattern memory
	sPatterns.insertVertex(input);

	//Update markov chain
	Edge<Dim> * inputEdge = NULL;
	if (prevInput != NULL)
	{
		inputEdge = sPatterns.insertEdge(prevInput, input);
		sPatterns.normalize(input);
	}
	prevInput = input;

#define DO_TEMPORAL_POOLING
#ifdef DO_TEMPORAL_POOLING

	//Add to temporal disjoint set template
	tPatternBuffer.add(input);

	//Add to Temporal Heap
	if (inputEdge != NULL)
	{
		//Add to the temporal queue
		tQueue.push(inputEdge, inputEdge);

		//Refresh the priority on the edge
		SpatialPatterns::next_iterator next_it = sPatterns.beginOut(input);
		while(next_it != sPatterns.endOut())
		{
			Edge<Dim> * edge = *next_it;
			tQueue.refreshPriority( edge );
			next_it ++;
		}
		
		//Extract Temporal Groups
		TemporalQueue tQueueBuffer;
		tPatterns = tPatternBuffer;
		
 		while(tQueue.size() > 0)
		{
			Edge<Dim> * edge = tQueue.peek();
			if (edge->normal <= 0.67f)
				break;
			tQueue.pop();
			tQueueBuffer.push(edge, edge);
			if (tPatterns.size() >= 0)
				tPatterns.setunion(edge->source, edge->dest);
		}
		//Push back all the elements that were popped off the queue
		while(tQueueBuffer.size() > 0)
		{
			Edge<Dim> * edge = tQueueBuffer.pop();
			tQueue.push(edge, edge);
		}
	}
#endif /* DO_TEMPORAL_POOLING */
}
#endif /* NODE__H */