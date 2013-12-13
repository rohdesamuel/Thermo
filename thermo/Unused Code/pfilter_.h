#ifndef PFILTER__H
#define PFILTER__H

#include <vector>
#include <algorithm>
#include <utility>

#include "graph.h"
#include "filter.h"
#include "priorityqueue.h"

using namespace std;

#define MIN_AVG 0.75f

namespace filters
{

	template <int Dim>
	class PFilter: public Filter<Dim, Dim>
	{
	public:
		PFilter(int Radius, int Active):radius(Radius),active(Active),mean(1.0f/Dim),iterations(0) { 
			distr.resize(Dim); 
		}
		~PFilter();

		virtual vector<Point<Dim> *> & get(void);
		virtual Point<Dim> * add(Point<Dim> * point);

		int iterations;
		int radius;
		int active;

	private:
		struct __compare {
			inline bool operator()( const float & f1, const float & f2 ){ return(f1 > f2); };
		};

		float mean;
		vector<float> distr;
		vector<int> count;
		vector<PriorityQueue<int, float, __compare> > means;
		vector<Point<Dim> *> clusters;
	};

	template <int Dim>
	PFilter<Dim>::~PFilter()
	{

	}

	template <int Dim>
	Point<Dim> * PFilter<Dim>::add(Point<Dim> * point)
	{
		Point<Dim> * ret = NULL;
	
		//Iterate to find first match with distance >= radius
		//This search should be replaced with a (kdtree?) for faster nearest neghbor querying
		int best = -1;
		int bestDis = 0;
		for (int i = 0; i < clusters.size(); i ++)
		{
			int dis = point->distance(clusters[i]);
			if (dis >= radius && dis > bestDis)
			{
				bestDis = dis;
				best = i;
			}
		}

		if (best != -1)
		{
			//vector<pair<int,float> > heap;
			count[best] ++;
			PriorityQueue<int, float, __compare> & pqueue = means[best];
			//Recompute the means with the new data point
			for (int j = 0; j < Dim; j ++)
			{
				//means[best][j] = (means[best][j] + point->data[j]) / 2;
 				float priority = (pqueue.getPriority(j) + point->get(j)) / 2;
				pqueue.setPriority(j, priority);


				distr[j] = (distr[j] + point->get(j)) / 2;
				//heap.push_back(pair<int,float>(j, means[best][j])); std::push_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare());
				(clusters[best])->set(j, false);
			}

			//This Recomputes the new code for a data point to match to
			/*for (int j = 0; j < active; j ++)
			{
				int index = heap.front().first;
				(clusters[best])->data[ index ] = true;
				std::pop_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare()); heap.pop_back();
			}*/
			int j = 0;
			PriorityQueue<int, float, typename PFilter<Dim>::__compare >::iterator iterator = pqueue.begin();
			while(j < active)
			{
				int index = iterator->second;
				(clusters[best])->set(index, true);
				++iterator;
				j++;
			}

			if (count[best] > 5)
				ret = clusters[best];
		}
		else
		{
			count.push_back(0);
			means.push_back(PriorityQueue<int, float, PFilter<Dim>::__compare >());
			for (int j = 0; j < Dim; j ++)
			{
				means.back().push(j, point->get(j));
			}
			clusters.push_back(new Point<Dim>(*point));
			ret = clusters.back();
		}

		iterations ++;
		return ret;
	}

	template <int Dim>
	vector<Point<Dim> *> & PFilter<Dim>:: get(void)
	{
		return clusters;
	}

}

#endif