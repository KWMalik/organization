#include <memory.h>
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
	//
	//After calling pool.add_block the memory  will look like:
	//		0000 0000
	//
	void *pBlock1 = static_cast<void*>(new int[8]);
	memset(pBlock1, 0, sizeof(int) * 8);
	pool.add_block(pBlock1, sizeof(int) * 8, sizeof(int));

	///Fill the first 5 allocations with the number 8
	int *pa = static_cast<int*>(pool.malloc());
	int *pb = static_cast<int*>(pool.malloc());
	int *pc = static_cast<int*>(pool.malloc());
	int *pd = static_cast<int*>(pool.malloc());
	int *pe = static_cast<int*>(pool.malloc());
	int *pf = static_cast<int*>(pool.malloc());
	int *pg = static_cast<int*>(pool.malloc());
	int *ph = static_cast<int*>(pool.malloc());

	// The memory will become:
	// 		1000 0000
	*pa = 1;

	// The memory will become:
	// 		1200 0000
	*pb = 2;

	// The memory will become:
	// 		1230 0000
	*pc = 3;

	// The memory will become:
	// 		1234 0000
	*pd = 4;

	// The memory will become:
	// 		1234 5000
	*pe = 5;

	// The memory will become:
	// 		1234 5600
	*pf = 6;

	// The memory will become:
	// 		1234 5670
	*pg = 7;

	// The memory will become:
	// 		1234 5678
	*ph = 8;

	
	//Free some elements
	
	// The memory will become: (x means unallocated)
	// 		1x34 5670
	pool.free(pb);

	// The memory will become: (x means unallocated)
	// 		1x3x 5670
	pool.free(pd);
	
	// The memory will become: (x means unallocated)
	// 		1x39 5670
	pb = static_cast<int*>(pool.malloc());
	*pb = 9;

	// The memory will become: (x means unallocated)
	// 		1939 5670
	pd = static_cast<int*>(pool.malloc());
	*pd = 9;


	cout << pool << endl;

	cout << "Finish" << endl;


	return 0;
}
