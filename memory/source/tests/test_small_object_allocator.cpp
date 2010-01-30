#include <iostream>
#include <stdlib.h>

#include "smallobjectallocator.h"

using namespace std;

//Creating a global small object allocator
SmallObjectAllocator *so = 0;

#define OVERLOAD_NEW_DELETE									\
void *operator new(size_t size)								\
{															\
	using namespace std;									\
    void *p = 0;											\
    if(so)													\
    {														\
        cout	<< "\tNew: Allocate a block of size "		\
				<< size << endl;							\
        p = so->Allocate(size, false);						\
        cout << "\t\tMemory allocated at: " << p << endl;	\
    }														\
    if(!p)													\
    {														\
        cout 	<< "\tNew: Unable to alloc a block "		\
				<< size << endl;							\
        p = malloc(size);									\
        cout << "\t\tMemory allocated at: " << p << endl;	\
    }														\
    return p;												\
}															\
															\
void operator delete(void *p)								\
{															\
	using namespace std;									\
    bool ableToDeallocateFromPools = false;					\
    if(so)													\
    {														\
        cout	<< "\tDelete: Allocation found pool :"		\
				<< p << endl;								\
        ableToDeallocateFromPools = so->Deallocate(p);		\
    }														\
    if(!ableToDeallocateFromPools)							\
    {														\
        cout	<< "\tDelete: Alloc not found in pools " 	\
				<< p << endl;								\
        free(p);											\
    }														\
}															\



struct Vector4
{
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

void Test_Small_Object_Allocator()
{
	cout << "Running SmallObjectAllocator Test" << endl;
    const unsigned int pageSize = 10;
    const unsigned int maxObjectSize = 128;
    const unsigned int objectAlignSize = 16;

    const int AD = 4;
    FixedAllocatorDescriptor fad [AD] = {   {   8,    8 },
                                            {  16,    8 },
                                            {  32,    8 },
                                            {  64,    8 } };


    so = new SmallObjectAllocator(fad, objectAlignSize);

    Size_16 *a1  = new Size_16();

    Size_32 *b1  = new Size_32();
    Size_32 *b2  = new Size_32();

    Size_64 *c1  = new Size_64();
    Size_64 *c2  = new Size_64();
    Size_64 *c3  = new Size_64();

    Size_128 *d1 = new Size_128();
    Size_128 *d2 = new Size_128();
    Size_128 *d3 = new Size_128();
    Size_128 *d4 = new Size_128();

    delete a1;
    //so->PrintStats();

    
    delete b1;
    delete b2;

    delete c1;
    delete c2;
    delete c3;

    delete d1;
    delete d2;
    delete d3;
    delete d4;

	cout << "Done running SmallObjectAllocator Test" << endl << endl;
}

