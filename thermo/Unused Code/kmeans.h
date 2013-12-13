#ifndef KMEANS__H
#define KMEANS__H

#include "filter.h"

template <class T, int Dim>
class KMeans: public Filter<Dim>
{
public:
	KMeans(void);
	KMeans(bool Sequential, int K);
	KMeans(bool Sequential, vector<Point<Dim> *> Means);
	KMeans(bool Sequential, vector<Point<Dim> *> Means, vector<Point<Dim> *> Data);
	~KMeans(void);

	virtual vector<Point<Dim> *> operator()(void);
	virtual vector<Point<Dim> *> add(T Data[]);
	virtual vector<Point<Dim> *> add(Point<Dim> * point);
	virtual vector<Point<Dim> *> get(void) const;

private:
	vector<Point<Dim> *> sequentialK(Point<Dim> * point);
	vector<Point<Dim> *> sequentialConstK(Point<Dim> * point);
	vector<Point<Dim> *> clusterK();
	T getDistance(Point<Dim> * p1, Point<Dim> * p2);

	vector<Point<Dim> *> means;
	vector<Point<Dim> *> data;
	vector<int> count;
	int k;
	bool sequential;
};

template <class T, int Dim>
KMeans<T, Dim>:: KMeans()
{
	k = 0;
	sequential = false;
}

template <class T, int Dim>
KMeans<T, Dim>:: ~KMeans()
{
}

template <class T, int Dim>
KMeans<T, Dim>:: KMeans(bool Sequential, int K): 
	sequential(Sequential), k(K)
{ 
	count.reserve(k);
	count.resize(k);
	for (int i = 0; i < k; i ++)
		count[i] = 0;
}

template <class T, int Dim>
KMeans<T, Dim>:: KMeans(bool Sequential, vector<Point<Dim> *> Means): 
	sequential(Sequential), k(Means.size()), means(Means)
{ 
	count.reserve(k);
	count.resize(k);
	for (int i = 0; i < k; i ++)
		count[i] = 0;
}

template <class T, int Dim>
KMeans<T, Dim>:: KMeans(bool Sequential, vector<Point<Dim> *> Means, vector<Point<Dim> *> Data): 
	sequential(Sequential), k(Means.size()), means(Means), data(Data)
{ 
	count.reserve(k);
	count.resize(k);
	for (int i = 0; i < k; i ++)
		count[i] = 0;
}

template <class T, int Dim>
vector<Point<Dim> *> KMeans<T, Dim>:: add(T Data[])
{
	Point<Dim> * point = new Point<Dim>(Data);
	data.push_back(point);
	if (sequential)
		return sequentialConstK(point);
	else
		return means;
}

template <class T, int Dim>
vector<Point<Dim> *> KMeans<T, Dim>:: add(Point<Dim> * point)
{
	data.push_back(point);
	if (sequential)
		return sequentialConstK(point);
	else
		return means;
}

template <class T, int Dim>
T KMeans<T, Dim>::getDistance(Point<Dim> * p1, Point<Dim> * p2)
{
	T dis = -1;
	if (p1 != NULL && p2 != NULL)
	{
		for (int i = 0; i < Dim; i ++)
			dis += ((*p1)[i] - (*p2)[i]) * ((*p1)[i] - (*p2)[i]);
	}

	return dis;
}

template <class T, int Dim>
vector<Point<Dim> *> KMeans<T, Dim>:: operator()(void)
{
	return clusterK();
}

template <class T, int Dim>
vector<Point<Dim> *> KMeans<T, Dim>:: get(void) const
{
	return means;
}

/*
	Make initial guesses for the means m1, m2, ..., mk
		Set the counts n1, n2, ..., nk to zero
		Until interrupted
			Acquire the next example, x
			If mi is closest to x
				Increment ni
				Replace mi by mi + (1/ni)*( x - mi)
			end_if
		end_until
*/
template <class T, int Dim>
vector<Point<Dim> *> KMeans<T, Dim>:: sequentialK(Point<Dim> * point)
{
	int max = means.size();
	int min_dis = getDistance(point, means[0]),
	    index = 0;
	for (int i = 1; i < max; i ++)
	{
		int dis = getDistance(point, means[i]);
		if (dis < min_dis)
		{
			min_dis = dis;
			index = i;
		}
	}
	int n = ++count[index];
	for (int i = 0; i < Dim; i ++)
	{
		T x = (*point)[i];
		T m = (*means[index])[i];
		(*means[index])[i] = m + (1/n)*( x - m );
	}
	return means;
}

/*
	Make initial guesses for the means m1, m2, ..., mk
		Set the counts n1, n2, ..., nk to zero
		Until interrupted
			Acquire the next example, x
			If mi is closest to x
				Increment ni
				Replace mi by mi + (1/ni)*( x - mi)
			end_if
		end_until
*/
template <class T, int Dim>
vector<Point<Dim> *> KMeans<T, Dim>:: sequentialConstK(Point<Dim> * point)
{
	int max = means.size();
	int index = 0;
	T min_dis = getDistance(point, means[0]);
	    
	for (int i = 1; i < max; i ++)
	{
		T dis = getDistance(point, means[i]);
		if (dis < min_dis)
		{
			min_dis = dis;
			index = i;
		}
	}
	for (int i = 0; i < Dim; i ++)
	{
		T x = (*point)[i];
		T m = (*means[index])[i];
		(*means[index])[i] = m + (T)(0.1)*( x - m );
	}
	return means;
}

/*
	Make initial guesses for the means m1, m2, ..., mk
		Until there are no changes in any mean
			Use the estimated means to classify the samples into clusters
			For i from 1 to k
				Replace mi with the mean of all of the samples for cluster i
			end_for
		end_until
*/
template <class T, int Dim>
vector<Point<Dim> *> KMeans<T, Dim>:: clusterK()
{
	return means;
}
#endif