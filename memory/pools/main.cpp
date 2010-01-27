#include <iostream>

#include "FastEmbeddedPool.h"

using namespace std;

int main(int argv, char ** argc)
{
	cout << "Start" << endl;

	//In the following example 'x' means unallocated and a number 
	//means the value of a pointer.
	//
	//For example:
	//	88xxx	///< Pool begins with two allocations of 8, 
	//          ///< followed by three unallocated slots.
	//
	FastEmbeddedPool<int> pool;

	//Allocate some memory and give it to the pool.
	void *pBlock1 = static_cast<void*>(new int[10]);
	pool.add_block(pBlock1, sizeof(int) * 10, sizeof(int));

	///Fill the first 5 allocations with the number 8
	int *pa = static_cast<int*>(pool.malloc());
	int *pb = static_cast<int*>(pool.malloc());
	int *pc = static_cast<int*>(pool.malloc());
	int *pd = static_cast<int*>(pool.malloc());
	int *pe = static_cast<int*>(pool.malloc());
	*pa = 8;
	*pb = 8;
	*pc = 8;
	*pd = 8;
	*pe = 8;

	//The pool now looks like 88888xxxxx
	
	//free some elements
	pool.free(pb);
	pool.free(pd);

	//The pool now looks like 8x8x8xxxxx
	
	//After the following allocation/assignment the pool 
	//will look like 8x858xxxxx
	pb = static_cast<int*>(pool.malloc());
	*pb = 5;

	//After the following allocation/assignment the pool will 
	//look like 85858xxxxx
	pd = static_cast<int*>(pool.malloc());
	*pd = 5;


	cout << pool << endl;

	cout << "Finish" << endl;

	return 0;
}
