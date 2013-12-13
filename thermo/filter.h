#pragma once
#ifndef FILTER__H
#define FILTER__H

#include <unordered_map>
#include <vector>
#include "point.h"

namespace filters
{
	template <int _InDim, int _OutDim>
	class Filter
	{
	public:
		virtual std::vector<Point<_OutDim> *> & get(void) = NULL;
		virtual Point<_OutDim> * add(Point<_InDim> * point) = NULL;
		virtual Point<_OutDim> * find(Point<_InDim> * point, std::unordered_map<Point<_OutDim>*, float> * distribution, float * normal_constant) = NULL;
	};
}
#endif