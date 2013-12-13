/*
* Defines a priority queue
* idea taken from http://stackoverflow.com/questions/2288241/priority-queue-with-dynamic-item-priorities
*/

#ifndef PRIORITYQUEUE__H
#define PRIORITYQUEUE__H

#include <map>
#include <unordered_map>

//_Kty is key
//_Ty is priority
//PriorityQueue cannot have duplicate keys
template <typename _Kty, typename _Ty, typename _Pr = less<_Ty> >
class PriorityQueue
{
public:
	PriorityQueue(){};
	//PriorityQueue(const PriorityQueue<_Kty, _Ty, _Pr> & other);
	PriorityQueue(const PriorityQueue<_Kty, _Ty, _Pr> && other);

	//PriorityQueue & operator=(const PriorityQueue<_Kty, _Ty, _Pr> & other);
	PriorityQueue & operator=(const PriorityQueue<_Kty, _Ty, _Pr> && other);
	
	typedef typename std::multimap<_Ty, _Kty, _Pr>::iterator iterator;

	void setPriority(_Kty & k, _Ty & v);
	void setPriority(_Kty & k, _Ty && v);
	void setPriority(_Kty && k, _Ty & v);
	void setPriority(_Kty && k, _Ty && v);

	const _Ty & getPriority(_Kty & k);

	void refreshPriority(_Kty & k);

	iterator push(_Kty & k, _Ty & v);
	iterator push(_Kty & k, _Ty && v);
	iterator push(_Kty && k, _Ty & v);
	iterator push(_Kty && k, _Ty && v);

	_Kty && pop(void);
	_Kty & peek(void);

	int size(void) const;

	iterator begin(void);
	iterator end(void);
	
private:
	typedef std::multimap<_Ty, _Kty, _Pr> __PriorityQueue; 
	typedef std::unordered_map<_Kty, typename __PriorityQueue::iterator > __Dictionary; 
	__PriorityQueue __priorityQueue;
	__Dictionary __dictionary;

	typename __PriorityQueue::iterator __find(_Kty & k);
	void __remove(_Kty & k);
	void __copy(const PriorityQueue<_Kty, _Ty, _Pr> && other);
	void __clear();
};

template <typename _Kty, typename _Ty, typename _Pr>
PriorityQueue<_Kty, _Ty, _Pr>::PriorityQueue(const PriorityQueue<_Kty, _Ty, _Pr> && other)
{
	__clear();
	__copy(std::move(other));
}

template <typename _Kty, typename _Ty, typename _Pr>
PriorityQueue<_Kty, _Ty, _Pr> & PriorityQueue<_Kty, _Ty, _Pr>::operator=(const PriorityQueue<_Kty, _Ty, _Pr> && other)
{
	if (this != &other)
	{
		__clear();
		__copy(std::move(other));
	}
	return *this;
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::__copy(const PriorityQueue<_Kty, _Ty, _Pr> && other)
{
	/*__PriorityQueue local = std::move(other.__priorityQueue);
	typename __PriorityQueue::iterator iterator = local.begin();
	while(iterator != local.end())
	{
		__dictionary[std::move(iterator->second)] = __priorityQueue.insert(std::make_pair<_Ty, _Kty>(std::move(iterator->second), std::move(iterator->first)));
		iterator++;
	}*/
	__priorityQueue = std::move(other.__priorityQueue);
	typename __PriorityQueue::iterator iterator = __priorityQueue.begin();
	while(iterator != __priorityQueue.end())
	{
		__dictionary[iterator->second] = iterator;
		++iterator;
	}
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::__clear()
{
	__priorityQueue.clear();
	__dictionary.clear();
}

template <typename _Kty, typename _Ty, typename _Pr>
typename PriorityQueue<_Kty, _Ty, _Pr>::iterator PriorityQueue<_Kty, _Ty, _Pr>::begin(void)
{
	return __priorityQueue.begin();
}

template <typename _Kty, typename _Ty, typename _Pr>
typename PriorityQueue<_Kty, _Ty, _Pr>::iterator PriorityQueue<_Kty, _Ty, _Pr>::end(void)
{
	return __priorityQueue.end();
}

template <typename _Kty, typename _Ty, typename _Pr>
typename PriorityQueue<_Kty, _Ty, _Pr>::iterator PriorityQueue<_Kty, _Ty, _Pr>::push(_Kty & k, _Ty & v)
{
	typename __PriorityQueue::iterator iterator = __priorityQueue.end();
	if (__find(k) == __priorityQueue.end())
	{
		iterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(v, k));
		__dictionary[k] = iterator;
	}
	return iterator;
}



/*
* @Param: k, key that is assigned priority v
* @Param: v, priority that is assigned to key k
* @Return: Returns iterator to inserted key upon successful push, returns iterator to end otherwise
*/
template <typename _Kty, typename _Ty, typename _Pr>
typename PriorityQueue<_Kty, _Ty, _Pr>::iterator PriorityQueue<_Kty, _Ty, _Pr>::push(_Kty && k, _Ty & v)
{
	typename __PriorityQueue::iterator iterator = __priorityQueue.end();
	if (__find(k) == __priorityQueue.end())
	{
		iterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(v, std::move(k)));
		__dictionary[k] = iterator;
	}
	return iterator;
}

template <typename _Kty, typename _Ty, typename _Pr>
typename PriorityQueue<_Kty, _Ty, _Pr>::iterator PriorityQueue<_Kty, _Ty, _Pr>::push(_Kty & k, _Ty && v)
{
	typename __PriorityQueue::iterator iterator = __priorityQueue.end();
	if (__find(k) == __priorityQueue.end())
	{
		iterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(std::move(v), k));
		__dictionary[k] = iterator;
	}
	return iterator;
}

template <typename _Kty, typename _Ty, typename _Pr>
typename PriorityQueue<_Kty, _Ty, _Pr>::iterator PriorityQueue<_Kty, _Ty, _Pr>::push(_Kty && k, _Ty && v)
{
	typename __PriorityQueue::iterator iterator = __priorityQueue.end();
	if (__find(k) == __priorityQueue.end())
	{
		iterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(std::move(v), std::move(k)));
		__dictionary[k] = iterator;
	}
	return iterator;
}

template <typename _Kty, typename _Ty, typename _Pr>
_Kty && PriorityQueue<_Kty, _Ty, _Pr>::pop()
{
	_Kty && ret = std::move(_Kty());
	if (__priorityQueue.size() > 0)
	{
		typename __PriorityQueue::iterator iterator = __priorityQueue.begin();
		 ret = std::move((*iterator).second);
		 __priorityQueue.erase(iterator);
		 __dictionary.erase(ret);
	}
	return std::move(ret);
}

template <typename _Kty, typename _Ty, typename _Pr>
_Kty & PriorityQueue<_Kty, _Ty, _Pr>::peek()
{
	_Ty ret = _Ty();
	if (__priorityQueue.size() > 0)
	{
		typename __PriorityQueue::iterator iterator = __priorityQueue.begin();
		 ret = (*iterator).second;
	}
	return ret;
}

//Searches the dictionary for the specified Key
//Returns iterator to item if found, returns iterator to end if not found
template <typename _Kty, typename _Ty, typename _Pr>
typename PriorityQueue<_Kty, _Ty, _Pr>::__PriorityQueue::iterator PriorityQueue<_Kty, _Ty, _Pr>::__find(_Kty & k)
{
	typename __Dictionary::iterator iterator = __dictionary.find(k);
	if (iterator != __dictionary.end())
		return (*iterator).second;
	else
		return __priorityQueue.end();
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::__remove(_Kty & k)
{
	typename __PriorityQueue::iterator iterator = __find(k);
	__dictionary.erase(k);
	__priorityQueue.erase(iterator);
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::refreshPriority(_Kty & k)
{
	typename __PriorityQueue::iterator iterator = __find(k);
	if (iterator != __priorityQueue.end())
	{
		typename __PriorityQueue::iterator newIterator;
		_Kty v = std::move(iterator->first);
		__priorityQueue.erase(iterator);
		newIterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(k, v ));
		__dictionary.at(k) = newIterator;
	}
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::setPriority(_Kty & k, _Ty & v)
{
	typename __PriorityQueue::iterator iterator = __find(k);
	if (iterator != __priorityQueue.end())
	{
		typename __PriorityQueue::iterator newIterator;
		__priorityQueue.erase(iterator);
		newIterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(v, k));
		__dictionary[k] = newIterator;
	}
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::setPriority(_Kty & k, _Ty && v)
{
	typename __PriorityQueue::iterator iterator = __find(k);
	if (iterator != __priorityQueue.end())
	{
		typename __PriorityQueue::iterator newIterator;
		__remove(k);
		newIterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(std::move(v), k));
		__dictionary[k] = newIterator;
	}
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::setPriority(_Kty && k, _Ty & v)
{
	typename __PriorityQueue::iterator iterator = __find(k);
	if (iterator != __priorityQueue.end())
	{
		typename __PriorityQueue::iterator newIterator;
		__remove(k);
		newIterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(v, std::move(k)));
		__dictionary[std::move(k)] = newIterator;
	}
}

template <typename _Kty, typename _Ty, typename _Pr>
void PriorityQueue<_Kty, _Ty, _Pr>::setPriority(_Kty && k, _Ty && v)
{
	typename __PriorityQueue::iterator iterator = __find(k);
	if (iterator != __priorityQueue.end())
	{
		typename __PriorityQueue::iterator newIterator;
		__remove(k);
		newIterator = __priorityQueue.insert(std::pair<_Ty, _Kty>(std::move(v), std::move(k)));
		__dictionary[std::move(k)] = newIterator;
	}
}

template <typename _Kty, typename _Ty, typename _Pr>
const _Ty &  PriorityQueue<_Kty, _Ty, _Pr>::getPriority(_Kty & k)
{
	typename __Dictionary::iterator iterator = __dictionary.find(k);
	if (iterator != __dictionary.end())
		return (iterator->second)->first;

	//Error! Cannot find key!
	else
		return _Ty();
}

template <typename _Kty, typename _Ty, typename _Pr>
int PriorityQueue<_Kty, _Ty, _Pr>::size() const
{
	return __priorityQueue.size();
}

#endif