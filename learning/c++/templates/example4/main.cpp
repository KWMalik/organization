
#include <iostream>

using namespace std;

/// \class Incrementable
///
/// This class servers to tag container classes as having incremental iterators.
/// A incrementable iterator is a iterator thats index is changed by using the
/// operator++.
///
struct Incrementable {};

/// \class SinglePass
///
/// This class servers to tag container classes as having incremental iterators.
/// A incrementable iterator is a iterator thats index is changed by using the
/// operator++.
///
struct SinglePass : public Incrementable {};



struct ForwardTraversal : public SinglePass {};



struct BidirectionalTraversal : public ForwardTraversal {};




struct RandomAccessTraversal : public BidirectionalTraversal {};


// A normal vector. This class if implemented would emulate a std::vector.
template<class T>
class Vector
{
public:
	typedef RandomAccessTraversal IteratorTag;
};

// This is a linked list that can only be iterated from the first element 
// to the last element.
template<class T>
class ForwardLinkedList
{
public:
	typedef ForwardTraversal IteratorTag;

};

// This class can traverse a list of elements in both forwards and backwards. 
// It cannot be accessed randomly
template<class T>
class BidirectionalLinkedList
{
public:
	typedef BidirectionalTraversal IteratorTag;

	BidirectionalLinkedList() {}
};


namespace Private
{
	template<class T>
	void Sort(T& listToBeSorted, ForwardTraversal &it)
	{
		//The search algorithm must be based on only being able to iterate forward.
		cout << "Sorting a container with ForwardTraversal" << endl;
	}

	template<class T>
	void Sort(T& listToBeSorted, BidirectionalTraversal &it)
	{
		//The search algorithm must be based on only being able to iterate forward.
		cout << "Sorting a container with BidirectionalTraversal" << endl;
	}

	template<class T>
	void Sort(T& listToBeSorted, RandomAccessTraversal &it)
	{
		//The search algorithm must be based on only being able to iterate forward.
		cout << "Sorting a container with RandomAccessTraversal" << endl;
	}

} // namespace Private



template<class T>
inline void Sort(T& listToBeSorted)
{
	typedef typename T::IteratorTag Tag;
	Tag t;
	Private::Sort(listToBeSorted, t);
}


int main(int argc, char **argv)
{
	Vector<int> myVector;
	ForwardLinkedList<int> myForwardLinkedList;
	BidirectionalLinkedList<int> myBidirectionalLinkedList;


	Sort(myVector);
	Sort(myForwardLinkedList);
	Sort(myBidirectionalLinkedList);
	
	return 0;
}




