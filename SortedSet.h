#ifndef MTM4_SORTEDSET_H
#define MTM4_SORTEDSET_H

#include <functional>
#include <cstdlib>
#include <iostream>
using namespace std;
template <class T, class Compare = std::less<T> >
class SortedSet {
public:
	struct node_t {
		T* data;
		struct node_t* next;
	};
	class iterator {
    public:
    	iterator();
    	iterator(node_t* ptr);
    	iterator(const iterator& it);
    	iterator& operator=(const iterator& i);
    	//iterator& operator=(T* element);
    	friend class SortedSet;
    	T& operator*(); // current
    	iterator& operator++();  // ++x
		iterator operator++(int); // x++
		bool operator==(const iterator& it) const;
		bool operator!=(const iterator& it) const;
    private:
    	node_t* index; // address
    };
    int size() const;
    iterator begin() const;
    iterator end() const;
    iterator find(const T& element) const;
    bool insert(const T& element);
    bool remove(const T& element);
    SortedSet(const SortedSet& set1);
    SortedSet();
    SortedSet& operator=(const SortedSet& set1);
    void destroyNode(node_t* ptr);
    ~SortedSet();
private:
    node_t* Node;
};

template <class T, class Compare = std::less<T> >
SortedSet<T,Compare> operator&(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2);

template <class T, class Compare = std::less<T> >
SortedSet<T,Compare> operator|(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2);

template <class T, class Compare = std::less<T> >
SortedSet<T,Compare> operator-(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2);

template <class T, class Compare = std::less<T> >
SortedSet<T,Compare> operator^(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2);


// Realization


/*
 * This function constructs a new iterator.
 */
template<class T, class Compare>
SortedSet<T,Compare>::iterator::iterator() :
	index(NULL) {
}



/*
 * This function constructs a new iterator with ptr.
 */
template<class T, class Compare>
SortedSet<T,Compare>::iterator::iterator(node_t* ptr) :
	index(ptr) {
}
/*
 * Copy function for iterator.
 */


template<class T, class Compare>
SortedSet<T,Compare>::iterator::iterator(const SortedSet::iterator& it) {
	index=it.index;
}
/*
 * Operators =
 */
template <class T, class Compare>
typename SortedSet<T,Compare>::iterator& SortedSet<T, Compare>::iterator::operator=(const SortedSet::iterator& i) {
	index=i.index;
	return *this;
}
/*
 * An operator* returns a "current" element of the set.
 */
template <class T, class Compare>
T& SortedSet<T, Compare>::iterator::operator *() {
	return *(this->index->data);
}

/*
* An ++operator returns next element.
*/
template <class T, class Compare>
typename SortedSet<T,Compare>::iterator& SortedSet<T, Compare>::iterator::operator++() {
	this->index=this->index->next;
	return *this;
}

// an operator++
template <class T, class Compare>
typename SortedSet<T,Compare>::iterator SortedSet<T,Compare>::iterator::operator++(int)
{
	SortedSet<T,Compare>::iterator iter(index);
	++(*this);
	return iter;
}



/*
 * operator== and operator!= check if iterators are same or not.
 */
template <class T, class Compare>
bool SortedSet<T,Compare>::iterator::operator==(const SortedSet::iterator& it) const {
	return (this->index==it.index);
}


template <class T, class Compare>
bool SortedSet<T,Compare>::iterator::operator!=(const SortedSet::iterator& it) const{
	return !(*this==it);
}


/*
 * This function creates a new sorted set.
 */
template<class T, class Compare>
SortedSet<T,Compare>::SortedSet() :
Node(NULL) {
}
/*
 * Copy c'tor
 */
template<class T, class Compare>
SortedSet<T,Compare>::SortedSet(const SortedSet& set1):
	Node(NULL) {
	for (typename SortedSet<T,Compare>::iterator it(set1.begin()); it!=set1.end(); ++it) {
		this->insert(*it);
	}
}
/*
 * Copy
 */
template<class T,class Compare>
SortedSet<T,Compare>& SortedSet<T,Compare>::operator=(const SortedSet& set1){
	if (this == &set1){
		return *this;
	}
	while(size())
	{
		remove(*begin());
	}
	for (typename SortedSet<T,Compare>::iterator it1(set1.begin()); it1!=set1.end(); ++it1) {
		insert(*it1);
	}
	return *this;
}
/*
 * This function deletes a sorted set.
 */
template<class T, class Compare>
void SortedSet<T,Compare>::destroyNode(node_t* ptr) {
	while(ptr) {
		node_t* toDelete = ptr;
		ptr = ptr->next;
		if (toDelete) {
			delete toDelete->data;
		}
		delete toDelete;
	}
}
template<class T, class Compare>
SortedSet<T,Compare>::~SortedSet() {
	node_t* ptr=this->Node;
	destroyNode(ptr);
}
/*
 * This function returns a size of set.
 */
template<class T, class Compare>
int SortedSet<T,Compare>::size() const{
	node_t* ptr=this->Node;
	int size=0;
	while(ptr) {
		size++;
		ptr=ptr->next;
	}
	return size;
}

/*
 * This function sets an iterator to the 1st element of set.
 */
template <class T, class Compare>
typename SortedSet<T,Compare>::iterator SortedSet<T,Compare>::begin() const {
	return iterator(this->Node);

}
/*
 * This function sets an iterator to the last element of set (invalid).
 */
template <class T, class Compare>
typename SortedSet<T,Compare>::iterator SortedSet<T,Compare>::end() const{ // check for the error
	return iterator(NULL);
}
/*
 * This function returns a number of an element in the set, and if an element
 * isn't included to the set, the function returns invalid value.
 */
template <class T, class Compare>
typename SortedSet<T,Compare>::iterator SortedSet<T,Compare>::find(const T& element) const
{
	Compare cmp;
	for (typename SortedSet<T,Compare>::iterator temp=begin(); temp!=end(); ++temp)
	{
		T item=*temp;
		if (!cmp(item,element) && !cmp(element,item))
		{
			return temp;
		}
	}
	return iterator(NULL);
}



//template <class T, class Compare>
//typename SortedSet<T,Compare>::iterator& SortedSet<T,Compare>::find(const T& element) {
//	iterator it=this->Iterator;
	//while (it!=NULL) {
		//if (!Compare(*it, element) && !Compare(element, *it)) {
	//		return it;
		//}
		//++it;
//	}
	//return end();
//}


/*
 * This function returns true if an element was successfully inserted to the set,
 * and if an element has already been in the set, function returns false.
 */
template <class T, class Compare>
bool SortedSet<T, Compare>::insert(const T& element) {
	iterator f=find(element);
	Compare cmp;
	if (f.index==NULL) //!=end()
	{
		if  ((this->size()>0))
		{

		node_t* ptr=this->Node;
		node_t* prev=NULL;
		//T item=element;
		while (ptr && cmp(*(ptr->data), element)) {
			prev=ptr;
			ptr=ptr->next;
		}
		node_t* current=new node_t;
		current->data=new T(element);
		current->next=ptr;
		if (prev) {
			prev->next=current;
		}
		else {
			this->Node=current;
		}
		return true;
		}
		else {
			this->Node=new node_t;
			this->Node->data=new T(element);
			//*(this->Node->data)=element;
			this->Node->next=NULL;
			return true;
		}

	}
	return false;
}

/*
 * This function returns true if an element was successfully removed from the set,
 * and if an element has not been in the set, function returns false.
 */
template <class T, class Compare>
bool SortedSet<T,Compare>::remove(const T& element) {
	iterator f=find(element);
	if (f.index==NULL) {
		return false;
	}
	node_t* ptr=this->Node;
	node_t* prev=NULL;
	while (ptr!=f.index) // deletes a tale = new version
	{
		prev=ptr;
		ptr=ptr->next;
	}
	if (prev) {
		prev->next=ptr->next;
	}
	else {
		this->Node=ptr->next;
	}
	delete ptr->data;
	delete ptr;
	return true;
}



/*
 * This function returns a Sorted Set which contains elements
 * of both 1st and 2nd sets.
 */
template <class T, class Compare>
SortedSet<T,Compare> operator&(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2) {
	SortedSet<T,Compare> set;
	for (typename SortedSet<T,Compare>::iterator it1(set1.begin()); it1!=set1.end(); ++it1) {
		if (set2.find(*it1)!=set2.end()) {
			set.insert(*it1);
		}
	}
	return set;
}
/*
 * This function returns a Sorted Set which contains elements
 * of 1st or 2nd set.
*/
template <class T, class Compare>
SortedSet<T,Compare> operator|(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2) {
	SortedSet<T,Compare> set;
	for (typename SortedSet<T,Compare>::iterator it1(set1.begin()); it1!=set1.end(); ++it1) {
			set.insert(*it1);
	}
	for (typename SortedSet<T,Compare>::iterator it2(set2.begin()); it2!=set1.end(); ++it2) {
			set.insert(*it2);
	}
	return set;
}
/*
 * This function returns a Sorted Set which contains elements
 * from 1st and not from 2nd set.
 */
template <class T, class Compare>
SortedSet<T,Compare> operator-(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2) {
	SortedSet<T,Compare> set;
	for (typename SortedSet<T,Compare>::iterator it1(set1.begin()); it1!=set1.end(); ++it1) {
			if (set2.find(*it1)==set2.end()) {
					set.insert(*it1);
			}
	}
	return set;
}
/*
 * This function returns a Sorted Set which contains elements
 * from one set and not from another.
*/
template <class T, class Compare>
SortedSet<T,Compare> operator^(const SortedSet<T,Compare>& set1, const SortedSet<T,Compare>& set2) {
	SortedSet<T,Compare> set4, set3, set;
	set4=set1-set2;
	set3=set2-set1;
	set=set4|set3;
	return set;
}

#endif
