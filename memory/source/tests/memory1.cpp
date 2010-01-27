#include <iostream>

#include <stdlib.h>

#include "smallobjectallocator.h"

using namespace std;

//#define TEST_CHUNK 1

//Change Chunk.h so that everything is public and then compile 
//again if you want to test the chunk class 
//
//Comment out this error once Chunk.h is public.
#if TEST_CHUNK
#error See above comment
#endif // TEST_CHUNK


void TestChunk()
{
#if TEST_CHUNK
	cout << "Running TestChunk Test" << endl;

    Chunk chunk;
    chunk.Init(4, 10);
    int *i = (int*)chunk.Allocate();
    cout << "\tThe value of i " << *i << endl;
    *i = 100;
    cout << "\tThe value of i " << *i << endl;
    chunk.Release();

	cout << "Done running TestChunk Test" << endl;
#endif // TEST_CHUNK
}

void TestFixedAllocator()
{
	cout << "Running FixedAllocator Test" << endl;
    FixedAllocator fa;
    fa.Init(4, 5);

    int *a = (int*)fa.Allocate();
    int *b = (int*)fa.Allocate();
    int *c = (int*)fa.Allocate();
    int *d = (int*)fa.Allocate();
    int *e = (int*)fa.Allocate();
    cout << "\tThe value of a " << *a << endl;
    cout << "\tThe value of b " << *b << endl;
    cout << "\tThe value of c " << *c << endl;
    cout << "\tThe value of d " << *d << endl;
    cout << "\tThe value of e " << *e << endl;

    *a = 5;
    *b = 10;
    *c = 15;
    *d = 20;
    *e = 25;
    cout << "\tThe value of a " << *a << endl;
    cout << "\tThe value of b " << *b << endl;
    cout << "\tThe value of c " << *c << endl;
    cout << "\tThe value of d " << *d << endl;
    cout << "\tThe value of e " << *e << endl;

    int *f = (int*)fa.Allocate();
    int *g = (int*)fa.Allocate();
    int *h = (int*)fa.Allocate();
    int *i = (int*)fa.Allocate();

    cout << "\tThe value of f " << *f << endl;
    cout << "\tThe value of g " << *g << endl;
    cout << "\tThe value of h " << *h << endl;
    cout << "\tThe value of i " << *i << endl;

    *f = 30;
    *g = 35;
    *h = 40;
    *i = 45;
    cout << "\tThe value of f " << *f << endl;
    cout << "\tThe value of g " << *g << endl;
    cout << "\tThe value of h " << *h << endl;
    cout << "\tThe value of i " << *i << endl;

    fa.Deallocate(i);
    fa.Deallocate(h);
    fa.Deallocate(g);
    fa.Deallocate(f);
    fa.Deallocate(e);
    fa.Deallocate(d);
    fa.Deallocate(c);
    fa.Deallocate(b);
    fa.Deallocate(a);
    
    cout << "\tThe value of a " << *a << endl;
    cout << "\tThe value of b " << *b << endl;
    cout << "\tThe value of c " << *c << endl;
    cout << "\tThe value of d " << *d << endl;
    cout << "\tThe value of e " << *e << endl;
    cout << "\tThe value of f " << *f << endl;
    cout << "\tThe value of g " << *g << endl;
    cout << "\tThe value of h " << *h << endl;
    cout << "\tThe value of i " << *i << endl;

	cout << "Done running FixedAllocator Test" << endl << endl;
}


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


void TestSmallAllocator()
{
	cout << "Running SmallObjectAllocator Test" << endl;
    const unsigned int pageSize = 10;
    const unsigned int maxObjectSize = 128;
    const unsigned int objectAlignSize = 16;

    so = new SmallObjectAllocator (pageSize, maxObjectSize, objectAlignSize);
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



int main()
{
    TestChunk();
    TestFixedAllocator();
    TestSmallAllocator();

    return 0;
}

