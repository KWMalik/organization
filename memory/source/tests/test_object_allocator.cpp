#include <iostream>
#include <stdlib.h>

#include "object_allocator.h"

using namespace std;

typedef Object_Allocator<Growing_New_Allocator> MyAllocator;
MyAllocator *oa;

#define OVERLOAD_NEW_DELETE									\
void *operator new(size_t size)								\
{															\
	using namespace std;									\
    void *p = 0;											\
    if(oa)													\
    {														\
        p = oa->allocate(size);        						\
		cout	<< "\tNew: PoolAllocate a block of size "	\
				<< size << " at " << p << endl;				\
    }														\
    if(!p)													\
    {														\
        cout 	<< "\tNew: Unable to alloc a block size: "	\
				<< size << endl;							\
        p = malloc(size);									\
        cout << "\t\tMalloc use created at: " << p << endl;	\
    }														\
    return p;												\
}															\
															\
void operator delete(void *p, size_t size)					\
{															\
	using namespace std;									\
    bool ableToDeallocateFromPools = false;					\
    if(oa)													\
    {														\
        ableToDeallocateFromPools = oa->deallocate(p, size);\
		if(ableToDeallocateFromPools)						\
		{													\
			cout	<< "\tDelete: Allocation found pool: "	\
			<< p << endl;									\
		}													\
    }														\
    if(!ableToDeallocateFromPools)							\
    {														\
        cout	<< "\tDelete: Alloc not found in pools: " 	\
				<< p << endl;								\
        free(p);											\
    }														\
}															\



struct Vector4
{
    Vector4() : x(1), y(2), z(3), w(4) {}

	OVERLOAD_NEW_DELETE
    int x;
    int y;
    int z;
    int w;
};

struct Size_16 
{
	OVERLOAD_NEW_DELETE
    Vector4 objectThatTakes16Bytes;
};

struct Size_32
{
	OVERLOAD_NEW_DELETE
    Size_16 a;
    Size_16 b;
};

struct Size_64
{
	OVERLOAD_NEW_DELETE
    Size_32 c;
    Size_32 d;
};

struct Size_128
{
	OVERLOAD_NEW_DELETE
    Size_64 e;
    Size_64 f;
};


static const unsigned long NumberOfFixedPools = 4;
ObjectAllocationSizes g_oas[NumberOfFixedPools] =
{
    {   4,    8}, 
    {   4,   16},
    {   4,   32},
    {   4,   64} 
};

void Test_Object_Allocator()
{
    cout << "Running Test_Object_Allocator Tests" << endl;
    oa = new MyAllocator();

    oa->Init(&(g_oas[0]), NumberOfFixedPools);

    Size_16 *a1  = new Size_16();
    Size_16 *a2  = new Size_16();
	Size_16 *a3  = new Size_16();
	Size_16 *a4  = new Size_16();
	Size_16 *a5  = new Size_16();
	Size_16 *a6  = new Size_16();
	Size_16 *a7  = new Size_16();
	
	
    Size_32 *b1  = new Size_32();
    Size_32 *b2  = new Size_32();
    Size_32 *b3  = new Size_32();
    Size_32 *b4  = new Size_32();
	
    Size_64 *c1  = new Size_64();
    Size_64 *c2  = new Size_64();
    Size_64 *c3  = new Size_64();
    Size_64 *c4  = new Size_64();
	
    Size_128 *d1 = new Size_128();
    Size_128 *d2 = new Size_128();
    Size_128 *d3 = new Size_128();
    Size_128 *d4 = new Size_128();

    delete a1;
    
    delete b1;
    delete b2;

    delete c1;
    delete c2;
    delete c3;

    delete d1;
    delete d2;
    delete d3;
    delete d4;

    cout << "DONE Running Test_Object_Allocator Tests" << endl;
}

