#include <vector>
#include <algorithm>
#include <utility>

#include "graph.h"
#include "filter.h"

#ifndef SFILTER__H
#define SFILTER__H

namespace filters
{
	template <int _InDim, int _OutDim>
	class SFilter: public Filter<_InDim, _OutDim>
	{
	public:
		SFilter();
		~SFilter() { };

		virtual std::vector<Point<_OutDim> *> & get(void) { return std::vector<Point<_OutDim> *>(); };
		virtual Point<_OutDim> * add(Point<_InDim> * point);

	private:
		class __Column
		{
		public:
			__Column():__rfield(0) { };
			__Column(int Rfield):__rfield(Rfield), __perms(Rfield, 1.0f) {
			};

			void setSynapse(int i, float perm){
				if (__perms[i] = perm < 0.2)
					synapses.set(i, false);
				else
					synapses.set(i, true);
			};

			Point<_OutDim> synapses;

		private:
			
			std::vector<float> __perms;
			int __rfield;
		};

		typedef std::vector<__Column> __Columns;
		typedef std::vector<float> __Boost;
		__Columns __columns;
		__Boost __boost;
	};

	template <int _InDim, int _OutDim>
	SFilter<_InDim, _OutDim>::SFilter()
	{
		__boost = __Boost(_InDim, 1.0f);
		__columns = __Columns(_OutDim, __Column(_InDim) );
		for (int i = 0; i < _OutDim; i ++)
		{
			for (int j = 0; j < _InDim; j ++)
				__columns[i].setSynapse(j, 1.0f);
		}
	}
	template <int _InDim, int _OutDim>
	Point<_OutDim> * SFilter<_InDim, _OutDim>::add(Point<_InDim> * point)
	{
		std::vector<std::pair<int,float> > heap;
		int overlap [_OutDim];

		for(int c = 0; c < _OutDim; c++)
		{
			__Column & column = __columns[c];
			overlap[c] = column.synapses.distance(point);
			if (overlap[c] < 4)
				overlap[c] = 0;
			else
				overlap[c] *= __boost[c];
		}





		return NULL;
	}
}

#endif