#ifndef NFILTER__H
#define NFILTER__H

#include <vector>

#include "filter.h"
#include "sparserep.h"

using namespace std;

namespace filters
{
	template <int Dim>
	class NFilter: public Filter<Dim, Dim>
	{
	public:
		NFilter();
		~NFilter();

		virtual vector<Point<Dim> *> & get(void);
		virtual Point<Dim> * add(Point<Dim> * point);
		virtual Point<Dim> * find(Point<Dim> * point, std::unordered_map<Point<Dim>*, float> * distribution, float * normal_constant);

	private:
		vector<Point<Dim> *> clusters;
	};

	template <int Dim>
	NFilter<Dim>::NFilter() { }

	template <int Dim>
	NFilter<Dim>::~NFilter() { }

	template <int Dim>
	Point<Dim> * NFilter<Dim>::find(Point<Dim> * point, std::unordered_map<Point<Dim>*, float> * distribution, float * normal_constant)
	{
		Point<Dim> * ret = NULL;
		
		*normal_constant = 0;

		int best = -1;
		int bestSim = 0;
		for (int i = 0; i < clusters.size(); i ++)
		{
			float sim = point->similarity(clusters[i]);
			distribution->insert(std::make_pair(clusters[i], sim));
			*normal_constant += sim;

			if (sim >= sim)
			{
				bestSim = sim;
				best = i;
			}
		}
		*normal_constant = 1 / *normal_constant;

		if (best != -1)
			ret = clusters[best];
		return ret;
	}

	template <int Dim>
	Point<Dim> * NFilter<Dim>::add(Point<Dim> * point)
	{
		Point<Dim> * ret = NULL;
		bool found = false;
		for (int i = 0; i < clusters.size(); i ++)
		{
			if (*clusters[i] == *point)
			{
				found = true;
				ret = clusters[i];
				break;
			}
		}
		
		if (!found)
			clusters.push_back(ret = new Point<Dim>(*point));

		return ret;
	}

	template <int Dim>
	vector<Point<Dim> *> & NFilter<Dim>:: get(void)
	{
		return clusters;
	}
}
#endif