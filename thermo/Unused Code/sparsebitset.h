#ifndef SPARSEBITSET__H
#define SPARSEBITSET__H

#include "sparserep.h"

template<unsigned int _Bits>
class SparseBitset
{
public:
	SparseBitset();
	SparseBitset(SparseBitset<_Bits> && other);

	typedef typename SparseRep<bool, _Bits>::iterator iterator;
	typedef typename SparseRep<bool, _Bits>::const_iterator const_iterator;

	void flip(unsigned int i);
	void operator()(unsigned int i, bool value);
	bool operator()(unsigned int i) const;

	void clear();
	int count();
	int distance(const SparseBitset & other) const;
	
	iterator begin() { return __bits.begin(); };
	iterator end()  { return __bits.end(); };
	iterator find(size_t i) { return __bits.find(i); };

	const_iterator begin() const { return __bits.begin(); };
	const_iterator end() const { return __bits.end(); };
	const_iterator find(size_t i) const { return __bits.find(i); };

private:
	void __clear();
	void __move(SparseBitset<_Bits> && other);

	SparseRep<bool, _Bits> __bits;
};

template<unsigned int _Bits>
SparseBitset<_Bits>::SparseBitset(){ };

template<unsigned int _Bits>
SparseBitset<_Bits>::SparseBitset(SparseBitset<_Bits> && other)
{ 
	__move();
};

template<unsigned int _Bits>
void SparseBitset<_Bits>::__move(SparseBitset<_Bits> && other)
{ 
	__bits = std::move(other.__bits);
};


template<unsigned int _Bits>
void SparseBitset<_Bits>::clear()
{ 
	__bits.clear();
};

template<unsigned int _Bits>
void SparseBitset<_Bits>::flip(unsigned int i)
{
	if (i < _Bits)
	{
		iterator it = __bits.find(i);
		if (it == __bits.end())
			__bits.set(i, true);
		else
			__bits.erase(it);
	}
}

template<unsigned int _Bits>
void SparseBitset<_Bits>::operator()(unsigned int i, bool value){
	if (i < _Bits)
	{
		if (value)
			__bits.set(i, true);
		else
			__bits.erase(i);
	}
};

template<unsigned int _Bits>
bool SparseBitset<_Bits>::operator()(unsigned int i) const{
	if (i < _Bits)
	{
		if (__bits.find(i) != __bits.end())
			return true;
	}
	return false;
}

template<unsigned int _Bits>
int SparseBitset<_Bits>::count()
{
	return __bits.count();
}

template<unsigned int _Bits>
int SparseBitset<_Bits>::distance(const SparseBitset & other) const
{
	const_iterator 
		first1 = __bits.begin(),
		first2 = other.__bits.begin(),
		last1 = __bits.end(),
		last2 = other.__bits.end();
	int result = 0;
	while (first1!=last1 && first2!=last2)
	{
		if (first1->first < first2->first) ++first1;
		else if (first2->first < first1->first) ++first2;
		else {
			++result; ++first1; ++first2;
		}
	}
	return result;
}

#endif