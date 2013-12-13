#ifndef BITSET__H
#define BITSET__H

#include <set>
#include <vector>

template<unsigned int _Bits>
class Bitset
{
public:
	Bitset();
	Bitset(Bitset<_Bits> && other);
	Bitset(std::vector<unsigned int> Bits);

	typedef typename std::set<unsigned int>::iterator iterator;

	void flip(unsigned int i);
	void operator()(unsigned int i, bool value);
	bool operator()(unsigned int i) const;
	bool operator==(const Bitset<_Bits> & other) const;
	bool operator!=(const Bitset<_Bits> & other) const;

	void clear();
	int count();
	int distance(const Bitset & other) const;
	int similarity(const Bitset & other) const;
	int similarity(const Bitset & other, int distance) const;
	
	iterator begin(){ return __bits.begin(); };
	iterator end(){ return __bits.end(); };

private:
	void __clear();
	void __move(Bitset<_Bits> && other);

	std::set<unsigned int> __bits;
};

template<unsigned int _Bits>
Bitset<_Bits>::Bitset(){ };

template<unsigned int _Bits>
Bitset<_Bits>::Bitset(Bitset<_Bits> && other)
{ 
	__move();
};

template<unsigned int _Bits>
void Bitset<_Bits>::__move(Bitset<_Bits> && other)
{ 
	__bits = std::move(other.__bits);
};

template<unsigned int _Bits>
Bitset<_Bits>::Bitset(std::vector<unsigned int> Bits)
{ 
	__bits = std::set<int>(Bits.begin(), Bits.end());
};

template<unsigned int _Bits>
void Bitset<_Bits>::clear()
{ 
	__bits.clear();
};

template<unsigned int _Bits>
void Bitset<_Bits>::flip(unsigned int i)
{
	if (i < _Bits)
	{
		iterator it = __bits.find(i);
		if (it == __bits.end())
			__bits.insert(i);
		else
			__bits.erase(it);
	}
}

template<unsigned int _Bits>
void Bitset<_Bits>::operator()(unsigned int i, bool value){
	if (i >= 0 && i < _Bits)
	{
		if (value)
			__bits.insert(i);
		else
			__bits.erase(i);
	}
};

template<unsigned int _Bits>
bool Bitset<_Bits>::operator()(unsigned int i) const{
	if (i >= 0 && i < _Bits)
	{
		if (__bits.find(i) != __bits.end())
			return true;
	}
	return false;
}

template<unsigned int _Bits>
inline bool Bitset<_Bits>::operator==(const Bitset<_Bits> & other) const{
	std::set<unsigned int>::const_iterator
		first1 = __bits.begin(),
		first2 = other.__bits.begin(),
		last1 = __bits.end(),
		last2 = other.__bits.end();
	while (first1!=last1 && first2!=last2)
	{
		if (*first1 == *first2){
			++first1; ++first2;
		}
		else
			return false;
	}
	if ((first1==last1 && first2!=last2) || (first1!=last1 && first2==last2))
		return false;
	else
		return true;
}

template<unsigned int _Bits>
inline bool Bitset<_Bits>::operator!=(const Bitset<_Bits> & other) const{
	return !(*this == other);
}


template<unsigned int _Bits>
int Bitset<_Bits>::count()
{
	return __bits.size();
}

template<unsigned int _Bits>
inline int Bitset<_Bits>::distance(const Bitset & other) const
{
	std::set<unsigned int>::iterator 
		first1 = __bits.begin(),
		first2 = other.__bits.begin(),
		last1 = __bits.end(),
		last2 = other.__bits.end();
	int result = 0;
	while (first1!=last1 && first2!=last2)
	{
		if (*first1<*first2) ++first1;
		else if (*first2<*first1) ++first2;
		else { //if *first1 == *first2
			++result; ++first1; ++first2;
		}
	}
	return result;
}

template<unsigned int _Bits>
inline int Bitset<_Bits>::similarity(const Bitset & other) const
{
	return distance(other) + min((_Bits - __bits.size()),(_Bits - other.__bits.size()));
}

template<unsigned int _Bits>
inline int Bitset<_Bits>::similarity(const Bitset & other, int distance) const
{
	return distance + min((_Bits - __bits.size()),(_Bits - other.__bits.size()));
}

#endif