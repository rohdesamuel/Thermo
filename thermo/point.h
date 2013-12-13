#ifndef POINT__H
#define POINT__H

#include "bitset.h"
#include "edge.h"

using namespace std;

template <int Dim>
class Point
{
public:
	Point() { };
	Point(Bitset<Dim> && Data):__data(Data) { };

	typedef typename Bitset<Dim>::iterator iterator;

	int size(){return __data.count(); };

	int distance(const Point & point) const;
	int distance(const Point * point) const;

	int similarity(const Point & point) const;
	int similarity(const Point * point) const;
	int similarity(const Point & point, int distance) const;
	int similarity(const Point * point, int distance) const;

	bool operator==(const Point & point) const;
	bool operator!=(const Point & point) const;

	bool get(int i);
	void set(int i, bool value);
	void flip(int i);
	void clear();

	iterator begin(){ return __data.begin(); };
	iterator end(){ return __data.end(); };

private:
	Bitset<Dim> __data;
};

template <int Dim>
void Point<Dim>::set(int i, bool value)
{
	__data(i, value);
}

template <int Dim>
bool Point<Dim>::get(int i)
{
	return __data(i);
}


template <int Dim>
void Point<Dim>::flip(int i)
{
	__data.flip(i);
}

template <int Dim>
void Point<Dim>::clear()
{
	__data.clear();
}


template <int Dim>
inline int Point<Dim>::distance(const Point & point) const
{
	return __data.distance(point.__data);
}

template <int Dim>
inline int Point<Dim>::distance(const Point * point) const
{
	if (point)
		return __data.distance(point->__data);
	else
		return -1;
}

template <int Dim>
inline int Point<Dim>::similarity(const Point & point) const
{
	if (point)
		return __data.similarity(point.__data);
	else
		return -1;
}
template <int Dim>
inline int Point<Dim>::similarity(const Point * point) const
{
	if (point)
		return __data.similarity(point->__data);
	else
		return -1;
}

template <int Dim>
inline int Point<Dim>::similarity(const Point & point, int distance) const
{
	if (point)
		return __data.similarity(point.__data, distance);
	else
		return -1;
}
template <int Dim>
inline int Point<Dim>::similarity(const Point * point, int distance) const
{
	if (point)
		return __data.similarity(point->__data, distance);
	else
		return -1;
}

template <int Dim>
inline bool Point<Dim>::operator==(const Point & point) const
{
	return this->__data == point.__data;
}

template <int Dim>
inline bool Point<Dim>::operator!=(const Point & point) const
{
	return !(this->__data == point.__data);
}

#endif