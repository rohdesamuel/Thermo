#ifndef HASH_GRAPH__H
#define HASH_GRAPH__H

#include "point.h"
#include "edge.h"

#include <vector>
#include <unordered_map>

template <int Dim>
class HashGraph
{
	friend class next_itertor;

private:
	typedef unsigned long long Code_;
	typedef std::pair<Edge<Dim> *, bool> EdgeInfo_;
	typedef std::unordered_map<Code_, EdgeInfo_ > EdgeData_;
	typedef std::unordered_map<Code_, Code_> Next_;
	typedef std::unordered_map<Point<Dim> *, Code_> Heads_;
	typedef std::unordered_map<Point<Dim> *, int> Weights_;

	Code_ ptrToCode_(Point<Dim> *x, Point<Dim> *y) const;
	Point<Dim> *codeToDst_(Code_ code) const;
	Point<Dim> *codeToSrc_(Code_ code) const;

	EdgeData_ edgeData_;

	Next_ nextIn_;
	Next_ nextOut_;
	Heads_ headsIn_;
	Heads_ headsOut_;
	Weights_ weights_;
public:
	class next_iterator {
		friend class HashGraph<Dim>;
	private:
		Next_ * nextData;
		EdgeData_ * edgeData;
		Next_::iterator next_it;
	public:
		bool operator==(next_iterator & other){
			return	(edgeData == other.edgeData) &&
					(nextData == other.nextData) &&
					(next_it == other.next_it);
		}

		bool operator!=(next_iterator & other){
			return !(*this == other);
		}

		void operator++(){
			next_it = nextData->find(next_it->second);
		}

		Edge<Dim> * operator*(){
			EdgeData_::iterator edge_it = edgeData->find(next_it->first);
			return (edge_it->second).first;
		}
	};

	HashGraph(){};
	~HashGraph(){};

	Edge<Dim> * insertEdge(Point<Dim> * x, Point<Dim> * y);
	void insertVertex(Point<Dim> * v);

	void normalize();
	void normalize(Point<Dim> * v);

	std::vector<Point<Dim> *> getAdjacent(Point<Dim> *v) const;
	std::vector<Edge<Dim> *> getIn(Point<Dim> *v) const;
	std::vector<Edge<Dim> *> getOut(Point<Dim> *v) const;

	next_iterator beginOut(Point<Dim> * v){
		next_iterator nxt;
		Heads_::iterator heads_it = headsOut_.find(v);
		nxt.nextData = &nextOut_;
		nxt.edgeData = &edgeData_;
		if (heads_it != headsOut_.end())
			nxt.next_it = nextOut_.find( heads_it->second );
		else
			nxt.next_it = nextOut_.end();
		return nxt;
	}

	next_iterator beginIn(Point<Dim> * v){
		next_iterator nxt;
		Heads_::iterator heads_it = headsIn_.find(v);
		nxt.nextData = &nextIn_;
		nxt.edgeData = &edgeData_;
		if (heads_it != headsIn_.end())
			nxt.next_it = nextIn_.find( heads_it->second );
		else
			nxt.next_it = nextIn_.end();
		return nxt;
	}

	next_iterator endOut(){
		next_iterator nxt;
		nxt.nextData = &nextOut_;
		nxt.edgeData = &edgeData_;
		nxt.next_it = nextOut_.end();
		return nxt;
	}

	next_iterator endIn(){
		next_iterator nxt;
		nxt.nextData = &nextIn_;
		nxt.edgeData = &edgeData_;
		nxt.next_it = nextIn_.end();
		return nxt;
	}
};

template <int Dim>
Point<Dim> *HashGraph<Dim>::codeToDst_(Code_ code) const
{
	return  (Point<Dim> *) code;
}

template <int Dim>
Point<Dim> *HashGraph<Dim>::codeToSrc_(Code_ code) const
{
	return (Point<Dim> *) (code >> (sizeof(long) * 8));
}

template <int Dim>
typename HashGraph<Dim>::Code_ HashGraph<Dim>::ptrToCode_(Point<Dim> *src, Point<Dim> *dst) const
{
	return (Code_(src) << (sizeof(long) * 8)) | Code_(dst);
}

template <int Dim>
void HashGraph<Dim>::insertVertex(Point<Dim> *v)
{
	if (weights_.find(v) == weights_.end())
	{
		headsIn_[v] = 0;
		headsOut_[v] = 0;
		weights_[v] = 0;
	}
}

template <int Dim>
Edge<Dim> * HashGraph<Dim>::insertEdge(Point<Dim> *src, Point<Dim> *dst)
{
	Edge<Dim> * ret;
	
	Code_ src_code = ptrToCode_(src, dst);
	Code_ dst_code = ptrToCode_(dst, src);

	weights_[dst] ++;

	EdgeData_::iterator edge_it;
	if ((edge_it = edgeData_.find(src_code)) == edgeData_.end())
	{
		ret = new Edge<Dim>(src, dst);
		edgeData_[src_code] = EdgeInfo_(ret, true);
	}
	else
	{
		ret = (edge_it->second).first;
		if ((edge_it->second).second == false)
			(edge_it->second).second = true;
		else
		{
			ret->weight ++;
			return ret;
		}
	}
	
	if (edgeData_.find(dst_code) == edgeData_.end())
		edgeData_[dst_code] = EdgeInfo_(new Edge<Dim>(dst, src), false);

	nextOut_[src_code] = headsOut_[src];
	headsOut_[src] = src_code;

	nextIn_[src_code] = headsIn_[dst];
	headsIn_[dst] = src_code;

	return ret;
}

template <int Dim>
void HashGraph<Dim>::normalize()
{
	Weights_::iterator weight_it = weights_.begin();
	while( weight_it != weights_.end())
	{
		Point<Dim> * point = weight_it->first;
		float weight = (float)weight_it->second;

		Code_ head = headsOut_[point];
		std::unordered_map<Code_, Code_>::iterator head_it = nextIn_.find(head);
		while( (head_it = nextOut_.find(head)) != nextOut_.end() )
		{
			EdgeData_::iterator edge_it = edgeData_.find(head);
			((edge_it->second).first)->normal = (float)((edge_it->second).first)->weight / weight;
			head = head_it->second;
		}
		weight_it++;
	}
}

template <int Dim>
void HashGraph<Dim>::normalize(Point<Dim> * v)
{
	Weights_::iterator weight_it = weights_.find(v);
	if (weight_it == weights_.end())
		return;

	float weight = (float)weight_it->second;
	Code_ head = headsOut_[v];
	std::unordered_map<Code_, Code_>::iterator head_it = nextIn_.find(head);
	while( (head_it = nextOut_.find(head)) != nextOut_.end() )
	{
		EdgeData_::iterator edge_it = edgeData_.find(head);
		((edge_it->second).first)->normal = (float)((edge_it->second).first)->weight / weight;
		head = head_it->second;
	}
}
template <int Dim>
std::vector<Point<Dim>* > HashGraph<Dim>::getAdjacent(Point<Dim> *v) const
{
	std::vector<Point<Dim> *> ret;
	std::unordered_map<Point<Dim> *, Code_>::iterator it = heads_.find(v);
	if (it == heads_.end())
		return ret;
	Code_ head = it->second;
	while(head != 0)
	{
		ret.push_back(codeToDst_(head));
		head = nextIn_[head];
	}

	head = it->second;
	while(head != 0)
	{
		ret.push_back(codeToSrc_(head));
		head = nextOut_[head];
	}

	return ret;
}

template <int Dim>
std::vector<Edge<Dim>* > HashGraph<Dim>::getIn(Point<Dim> *v) const
{
	std::vector<Edge<Dim> *> ret;
	std::unordered_map<Point<Dim> *, Code_>::const_iterator it = headsIn_.find(v);
	if (it == headsIn_.end())
		return ret;

	Code_ head = it->second;
	std::unordered_map<Code_, Code_>::const_iterator head_it = nextIn_.find(head);
	while( (head_it = nextIn_.find(head)) != nextIn_.end() )
	{
		EdgeData_::const_iterator edge_it = edgeData_.find(head);
		ret.push_back((edge_it->second).first);
		head = head_it->second;
	}
	
	return ret;
}


template <int Dim>
std::vector<Edge<Dim>* > HashGraph<Dim>::getOut(Point<Dim> *v) const
{
	std::vector<Edge<Dim> *> ret;
	std::unordered_map<Point<Dim> *, Code_>::const_iterator it = headsOut_.find(v);
	if (it == headsOut_.end())
		return ret;

	Code_ head = it->second;
	std::unordered_map<Code_, Code_>::const_iterator head_it = nextIn_.find(head);
	while( (head_it = nextOut_.find(head)) != nextOut_.end() )
	{
		EdgeData_::const_iterator edge_it = edgeData_.find(head);
		ret.push_back((edge_it->second).first);
		head = head_it->second;
	}
	
	return ret;
}

#endif /* HASH_GRAPH__H */