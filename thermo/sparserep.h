#ifndef SPARSEREP__H
#define	SPARSEREP__H

#include <map>

template<typename _Ty, size_t _Dim>
class SparseRep
{
public:
	SparseRep();
	SparseRep(SparseRep<_Ty, _Dim> && other);

	typedef typename std::map<size_t, _Ty>::iterator iterator;
	typedef typename std::map<size_t, _Ty>::const_iterator const_iterator;

	void set(size_t i, _Ty t);
	_Ty & get(size_t i);

	void erase(size_t i);
	void erase(iterator it);
	void clear();
	int count() const;

	iterator begin() { return __nums.begin(); };
	iterator end()  { return __nums.end(); };
	iterator find(size_t i) { return __nums.find(i); };

	const_iterator begin() const { return __nums.begin(); };
	const_iterator end() const { return __nums.end(); };
	const_iterator find(size_t i) const { return __nums.find(i); };

private:
	void __clear();
	void __move(SparseRep<_Ty, _Dim> && other);

	std::map<size_t, _Ty> __nums;
};

template<typename _Ty, size_t _Dim>
SparseRep< _Ty, _Dim>::SparseRep(){ };

template<typename _Ty, size_t _Dim>
SparseRep< _Ty, _Dim>::SparseRep(SparseRep< _Ty, _Dim> && other)
{ 
	__move(std::move(other));
}

template<typename _Ty, size_t _Dim>
void SparseRep< _Ty, _Dim>::__move(SparseRep< _Ty, _Dim> && other)
{ 
	__nums = std::move(other.__nums);
}

template<typename _Ty, size_t _Dim>
void SparseRep< _Ty, _Dim>::clear()
{ 
	__nums.clear();
}


template<typename _Ty, size_t _Dim>
void SparseRep< _Ty, _Dim>::set(size_t i, _Ty t)
{
	iterator it;
	if (i >= 0 && i < _Dim)
	{
		it = __nums.find(i);
		if (it == __nums.end())
			__nums.insert( std::pair<size_t, _Ty>(i, t));
		else
			it->second = t;
	}
}

template<typename _Ty, size_t _Dim>
_Ty & SparseRep< _Ty, _Dim>::get(size_t i)
{
	iterator it = __nums.find(i);
	return it->second;
}

template<typename _Ty, size_t _Dim>
int SparseRep< _Ty, _Dim>::count() const
{
	return __nums.size();
}

template<typename _Ty, size_t _Dim>
void SparseRep< _Ty, _Dim>::erase(size_t i)
{
	erase(__nums.find(i));
}

template<typename _Ty, size_t _Dim>
void SparseRep< _Ty, _Dim>::erase(iterator it)
{
	__nums.erase(it);
}

#endif