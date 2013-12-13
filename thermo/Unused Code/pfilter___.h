#ifndef PFILTER__H
#define PFILTER__H

#include <vector>
#include <algorithm>
#include <utility>

#include "graph.h"
#include "filter.h"

using namespace std;

#define MIN_AVG 0.75f

template <int Dim>
class PFilter: public Filter<Dim, Dim>
{
public:
	PFilter(int Radius, int Active):radius(Radius),active(Active),mean(1.0f/Dim) { 
		distr.resize(Dim); 
		boost.resize(Dim);
		for(int i = 0; i < Dim; i ++)
			boost[i] = 1.0f;
	}
	~PFilter();

	virtual vector<Point<Dim> *> & get(void);
	virtual Point<Dim> * add(Point<Dim> * point);

	int radius;
	int active;

private:
	struct __compare {
		bool operator()( pair<int, float> p1, pair<int, float> p2 ){ return(p1.second < p2.second); };
	};

	float mean;
	vector<float> distr;
	vector<pair<int,float> > distrHeap;
	vector<float> boost;
	vector<vector<float>> means;
	//vector<vector<int>> counts;
	vector<Point<Dim> *> clusters;
};

template <int Dim>
PFilter<Dim>::~PFilter()
{

}
#define a_
#ifdef a_
template <int Dim>
Point<Dim> * PFilter<Dim>::add(Point<Dim> * point)
{
	Point<Dim> * ret = NULL;

	//Regulate input to only have an "active" amount of bits on
	if (distrHeap.size() > 0)
	{
		//If less than the radius set the minimum 
		if (point->data.count() < radius)
		{
			while(point->data.count() < active)
			{
				int index = distrHeap.front().first;
				point->data[ index ] = true;
				std::pop_heap(distrHeap.begin(), distrHeap.end(), PFilter<Dim>::__compare()); distrHeap.pop_back();
			}
		}

		//If more take away to active
		else if (point->data.count() > active)
		{
			std::sort(distrHeap.begin(), distrHeap.end(), PFilter<Dim>::__compare());
			int i = 0;
			while(point->data.count() > active)
			{
				point->data[ distrHeap[i].first ] = false;
				i ++;
			}
		}
	}

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
		vector<pair<int,float> > heap;
		distrHeap.clear();
		//Recompute the means with the new data point
		for (int j = 0; j < Dim; j ++)
		{
			means[best][j] = (means[best][j] + point->data[j]) / 2; //means[best][j] + ((float)point->data[j] / (float)counts[best][j]);//
			distr[j] = (distr[j] + point->data[j]) / 2;

			if (distr[j] < mean)
				boost[j] += 0.1f;
			else
				boost[j] = 1.0f;
			heap.push_back(pair<int,float>(j, means[best][j])); std::push_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare());
			distrHeap.push_back(pair<int,float>(j, distr[j] * boost[j])); std::push_heap(distrHeap.begin(), distrHeap.end(), PFilter<Dim>::__compare());
			(clusters[best])->data[j] = false;
		}

		//This Recomputes the new code for a data point to match to
		for (int j = 0; j < active; j ++)
		{
			int index = heap.front().first;
			(clusters[best])->data[ index ] = true;
			std::pop_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare()); heap.pop_back();
		}

		ret = clusters[best];
	}
	else
	{
		means.push_back(vector<float>(Dim));
		for (int i = 0; i < Dim; i ++)
		{
			means.back()[i] = point->data[i];
		}
		clusters.push_back(new Point<Dim>(*point));
		ret = clusters.back();
	}

	return ret;
}
#endif

#ifdef b
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
		vector<pair<int,float>> heap;

		//Recompute the means with the new data point
		for (int j = 0; j < Dim; j ++)
		{
			(clusters[best])->data[j] = false;
			means[best][j] = (means[best][j] + point->data[j]) / 2;
			distr[j] = (distr[j] + point->data[j]) / 2;

			if (distr[j] < mean)
				boost[j] += 0.1f;
			else
				boost[j] = 1.0f;
			heap.push_back(pair<int,float>(j, means[best][j] * boost[j]));
		}

		//Want to keep all the orders random for good bit distribution
		//This will be replaced in the future by individual columns inhibiting neighbors to
		//keep an even districution
		//std::random_shuffle(heap.begin(), heap.end());

		//Sort and then find the first n-active cells and set them to '1'
		//This Recomputes the new code for a data point to match to
		std::sort(heap.begin(), heap.end(), PFilter<Dim>::__compare() );
		for (int j = 0; j < active; j ++)
		{
			(clusters[best])->data[ heap[j].first ] = true;
		}

		ret = clusters[best];
	}
	else
	{
		means.push_back(vector<float>(Dim));
		for (int i = 0; i < Dim; i ++)
		{
			//distr[i] = (distr[i] + point->data[i]) / 2;
			means.back()[i] = point->data[i];
		}
		clusters.push_back(new Point<Dim>(*point));
		ret = clusters.back();
	}

	return ret;
}
#endif

#ifdef c
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
		vector<pair<int,float>> heap;

		//Recompute the means with the new data point
		for (int j = 0; j < Dim; j ++)
		{
			(clusters[best])->data[j] = false;
			means[best][j] = (means[best][j] + point->data[j]) / 2;
			distr[j] = (distr[j] + point->data[j]) / 2;

			if (distr[j] < mean)
				boost[j] += 0.1f;
			else
				boost[j] = 1.0f;
			heap.push_back(pair<int,float>(j, means[best][j] * boost[j])); std::push_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare());
		}

		//Want to keep all the orders random for good bit distribution
		//This will be replaced in the future by individual columns inhibiting neighbors to
		//keep an even districution
		//std::random_shuffle(heap.begin(), heap.end());

		//Sort and then find the first n-active cells and set them to '1'
		//This Recomputes the new code for a data point to match to
		std::sort_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare() );
		for (int j = 0; j < active; j ++)
		{
			(clusters[best])->data[ heap[j].first ] = true;
		}

		ret = clusters[best];
	}
	else
	{
		means.push_back(vector<float>(Dim));
		for (int i = 0; i < Dim; i ++)
		{
			//distr[i] = (distr[i] + point->data[i]) / 2;
			means.back()[i] = point->data[i];
		}
		clusters.push_back(new Point<Dim>(*point));
		ret = clusters.back();
	}

	return ret;
}
#endif

template <int Dim>
vector<Point<Dim> *> & PFilter<Dim>:: get(void)
{
	return clusters;
}

#endif