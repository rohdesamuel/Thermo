#ifndef DSETS__H
#define DSETS__H

#include <map>
#include <vector>
using namespace std;

template<class T>
class DSets 
{
public:
	DSets():__size(0) { };
	void add (T elem);
	int find (T elem);
	void setunion( T a, T b);
	void setunion( int a, int b);

	int size() { return __size; };
private:
	void __unionbysize ( int root1, int root2 );
	int __find ( int elem );
    
	map<T, int > __elementDictionary;
	vector<int> __data;
	int __size;
};

template<class T>
void DSets<T>::add( T elem )
{
	if (__elementDictionary.find( elem ) == __elementDictionary.end())
	{
		pair<T, int> element(elem, __data.size());
		__elementDictionary.insert(element);
		__data.push_back(-1);
		__size ++;
	}
}

template<class T>
int DSets<T>::find ( T elem )
{ 
	map< T, int>::iterator it = __elementDictionary.find(elem);
	if (it != __elementDictionary.end())
		return __find((*it).second);
	return -1;
}


template<class T>
int DSets<T>::__find ( int elem )
{ 
	if (elem < __data.size() )
	{
		if ( __data[elem] < 0 ) 
			return elem;
		else
			return __data[elem] = __find ( __data[elem] );
	}
	return -1;
}

template<class T>
void DSets<T>::setunion( T a, T b)
{
	int seta = find(a), 
		setb = find(b);
	if (seta >= 0 && setb >= 0)
		__unionbysize( seta, setb );
}

template<class T>
void DSets<T>::setunion( int a, int b)
{
	__unionbysize( a, b );
}

template<class T>
void DSets<T>::__unionbysize ( int root1, int root2 )
{
    int new_size = __data[root1] + __data[root2];
    if ( root1 > root2 ) 
    {
        __data[root2] = root1;
        __data[root1] = new_size;
		__size --;
    }
    else if (root1 < root2)
    {
        __data[root1] = root2;
        __data[root2] = new_size;
		__size --;
    }
}


#endif
