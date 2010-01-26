#include <iostream>

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
    Chunk chunk;
    chunk.Init(4, 10);
    int *i = (int*)chunk.Allocate();
    cout << "The value of i " << *i << endl;
    *i = 100;
    cout << "The value of i " << *i << endl;
    chunk.Release();
#endif // TEST_CHUNK
}

void TestFixedAllocator()
{
    FixedAllocator fa;
    fa.Init(4, 5);

    int *a = (int*)fa.Allocate();
    int *b = (int*)fa.Allocate();
    int *c = (int*)fa.Allocate();
    int *d = (int*)fa.Allocate();
    int *e = (int*)fa.Allocate();
    cout << "The value of a " << *a << endl;
    cout << "The value of b " << *b << endl;
    cout << "The value of c " << *c << endl;
    cout << "The value of d " << *d << endl;
    cout << "The value of e " << *e << endl;

    *a = 5;
    *b = 10;
    *c = 15;
    *d = 20;
    *e = 25;
    cout << "The value of a " << *a << endl;
    cout << "The value of b " << *b << endl;
    cout << "The value of c " << *c << endl;
    cout << "The value of d " << *d << endl;
    cout << "The value of e " << *e << endl;

    int *f = (int*)fa.Allocate();
    int *g = (int*)fa.Allocate();
    int *h = (int*)fa.Allocate();
    int *i = (int*)fa.Allocate();

    cout << "The value of f " << *f << endl;
    cout << "The value of g " << *g << endl;
    cout << "The value of h " << *h << endl;
    cout << "The value of i " << *i << endl;

    *f = 30;
    *g = 35;
    *h = 40;
    *i = 45;
    cout << "The value of f " << *f << endl;
    cout << "The value of g " << *g << endl;
    cout << "The value of h " << *h << endl;
    cout << "The value of i " << *i << endl;

    fa.Deallocate(i);
    fa.Deallocate(h);
    fa.Deallocate(g);
    fa.Deallocate(f);
    fa.Deallocate(e);
    fa.Deallocate(d);
    fa.Deallocate(c);
    fa.Deallocate(b);
    fa.Deallocate(a);
    
    cout << "The value of a " << *a << endl;
    cout << "The value of b " << *b << endl;
    cout << "The value of c " << *c << endl;
    cout << "The value of d " << *d << endl;
    cout << "The value of e " << *e << endl;
    cout << "The value of f " << *f << endl;
    cout << "The value of g " << *g << endl;
    cout << "The value of h " << *h << endl;
    cout << "The value of i " << *i << endl;
}


struct Vector4
{
    int x;
    int y;
    int z;
    int w;
};

struct Size_16 
{
    Vector4 objectThatTakes16Bytes;
};

struct Size_32
{
    Size_16 a;
    Size_16 b;
};

struct Size_64
{
    Size_32 c;
    Size_32 d;
};

struct Size_128
{
    Size_64 e;
    Size_64 f;
};

//Creating a global small object allocator
SmallObjectAllocator *so = 0;


void *operator new(size_t size)
{
    std::cout << "Global New" << std::endl;
    void *p = 0;
    if(so)
    {
        //SmallObjectAllocator exists.. Try to allocate with it.
        std::cout << "\tGlobal New: Allocate a block of size " << size << std::endl;
        p = so->Allocate(size, false);
        std::cout << "\t\tMemory allocated at: " << p << std::endl;
    }
    if(!p)
    {
        std::cout << "\tGlobal New: Unable to allocate a block of size " << size << std::endl;
        p = malloc(size);
        std::cout << "\t\tMemory allocated at: " << p << std::endl;
    }
    return p;
}

void operator delete(void *p)
{
    std::cout << "Global Delete" << std::endl;
    bool ableToDeallocateFromPools = false;
    if(so)
    {
        //SmallObjectAllocator exists.. Try to allocate with it.
        std::cout << "\tGlobal Delete: Allocation found in pools " << p << std::endl;
        ableToDeallocateFromPools = so->Deallocate(p);
    }
    if(!ableToDeallocateFromPools)
    {
        std::cout << "\tGlobal Delete: Allocation not found in pools" << p << std::endl;
        free(p);
    }
}

void TestSmallAllocator()
{
    const unsigned int pageSize = 10;
    const unsigned int maxObjectSize = 128;
    const unsigned int objectAlignSize = 16;
    so = new SmallObjectAllocator (pageSize, maxObjectSize, objectAlignSize);

    Size_16 *a1  = new Size_16();

    std::cout << "here" << std::endl;
    Size_32 *b1  = new Size_32();
    Size_32 *b2  = new Size_32();

    Size_64 *c1  = new Size_64();
    Size_64 *c2  = new Size_64();
    Size_64 *c3  = new Size_64();

    Size_128 *d1 = new Size_128();
    Size_128 *d2 = new Size_128();
    Size_128 *d3 = new Size_128();
    Size_128 *d4 = new Size_128();

    so->PrintStats();

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


}



int main()
{
    cout << "Running core-test-memory1" << endl;

    TestChunk();
    TestFixedAllocator();
    TestSmallAllocator();

    cout << "Finished running core-test-memory1" << endl;

    return 0;
}

