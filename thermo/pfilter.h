#ifndef PFILTER__H
#define PFILTER__H

#include <vector>
#include <algorithm>
#include <utility>

#include "filter.h"
#include "sparserep.h"

using namespace std;

#define MIN_AVG 0.75f

namespace filters
{
	template <int Dim>
	class PFilter: public Filter<Dim, Dim>
	{
	public:
		PFilter(int Radius, int Active):radius(Radius),active(Active),iterations(0) { 
			__precision = 10000;
		}
		~PFilter();

		virtual vector<Point<Dim> *> & get(void);
		virtual Point<Dim> * add(Point<Dim> * point);
		virtual Point<Dim> * find(Point<Dim> * point, std::unordered_map<Point<Dim>*, float> * distribution, float * normal_constant);

		int iterations;
		int radius;
		int active;

	private:
		struct __compare {
			inline bool operator()( pair<int, int> & p1, pair<int, int> & p2 ){ return(p1.second < p2.second); };
		};

		int __precision;
		SparseRep<int, Dim> distr;
		vector<int> count;
		vector<SparseRep<int, Dim> > means;
		vector<Point<Dim> *> clusters;
	};

	template <int Dim>
	PFilter<Dim>::~PFilter()
	{

	}

	template <int Dim>
	Point<Dim> * PFilter<Dim>::find(Point<Dim> * point, std::unordered_map<Point<Dim>*, float> * distribution, float * normal_constant)
	{
		Point<Dim> * ret = NULL;
		
		*normal_constant = 0;

		//Iterate to find first match with distance >= radius
		//This search should be replaced with a (kdtree?) for faster nearest neghbor querying
		int best = -1;
		int bestDis = 0;
		for (int i = 0; i < clusters.size(); i ++)
		{
			int dis = point->distance(clusters[i]);
			float sim = point->similarity(clusters[i], dis);
			distribution->insert(std::make_pair(clusters[i], sim));
			*normal_constant += sim;

			if ( (dis >= radius || (point->size() == 0 && clusters[i]->size() == 0)) && dis >= bestDis)
			{
				bestDis = dis;
				best = i;
			}
		}
		*normal_constant = 1 / *normal_constant;

		if (best != -1)
			ret = clusters[best];
		return ret;
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
			if ( (dis >= radius || (point->size() == 0 && clusters[i]->size() == 0)) && dis >= bestDis)
			{
				bestDis = dis;
				best = i;
			}
		}

		if (best != -1)
		{
			vector<pair<int,int> > heap;
			count[best] ++;
			//Recompute the means with the new data point
			SparseRep<int, Dim>::iterator it = means[best].begin();
			
			while(it != means[best].end())
			{
				means[best].set(it->first, (it->second + (point->get(it->first) * __precision)) / 2);
				if (distr.find(it->first) == distr.end())
					distr.set(it->first, 0);
				distr.set(it->first, (distr.get(it->first) + point->get(it->first)) / 2);
				heap.push_back(pair<int,int>(it->first, it->second)); std::push_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare());
				it ++;
			}

			//This Recomputes the new code for a data point to match to
			(clusters[best])->clear();
			for (int j = 0; j < active && heap.size() > 0; j ++)
			{
				int index = heap.front().first;
				(clusters[best])->set(index , true);
				std::pop_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare()); heap.pop_back();
			}

			//Only return a cluster when it's trained 
			//if (count[best] > 5)
			ret = clusters[best];
		}
		else if (point->size() == 0 || point->size() >= radius)
		{
			count.push_back(0);
			means.push_back(SparseRep<int, Dim>());
			
			Point<Dim>::iterator it = point->begin();
			while(it != point->end())
			{
				(means.back()).set(*it, point->get(*it) * __precision);
				it ++;
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

/* This is a backup to the pfilter that does not include sparsereps */
#if 0
namespace filters
{
	template <int Dim>
	class PFilter: public Filter<Dim, Dim>
	{
	public:
		PFilter(int Radius, int Active):radius(Radius),active(Active),iterations(0) { 
			__precision = 10000;
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
			inline bool operator()( pair<int, int> & p1, pair<int, int> & p2 ){ return(p1.second < p2.second); };
		};

		int __precision;
		vector<int> distr;
		vector<int> count;
		vector<vector<int>> means;
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
			vector<pair<int,int> > heap;
			count[best] ++;
			//Recompute the means with the new data point
			for (int j = 0; j < Dim; j ++)
			{
				means[best][j] = (means[best][j] + (point->get(j) * __precision)) / 2;
				distr[j] = (distr[j] + point->get(j)) / 2;
				heap.push_back(pair<int,int>(j, means[best][j])); std::push_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare());
			}

			//This Recomputes the new code for a data point to match to
			(clusters[best])->clear();
			for (int j = 0; j < active; j ++)
			{
				int index = heap.front().first;
				(clusters[best])->set(index , true);
				std::pop_heap(heap.begin(), heap.end(), PFilter<Dim>::__compare()); heap.pop_back();
			}

			//Only return a cluster when it trained 
			if (count[best] > 5)
				ret = clusters[best];
		}
		else
		{
			count.push_back(0);
			means.push_back(vector<int>(Dim));
			for (int i = 0; i < Dim; i ++)
			{
				means.back()[i] = point->get(i) * __precision;
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
