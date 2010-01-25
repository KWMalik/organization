#include <iostream>

#include "memory/smallobjectallocator.h"

using namespace std;

//#define TEST_CHUNK 1

//Change Chunk.h so that everything is public and then compile 
//again if you want to test the chunk class 
//
//Comment out this error once Chunk.h is public.
#if TEST_CHUNK
#error See above comment
#endif // TEST_CHUNK

int main()
{
	using namespace Axe;
	using namespace Axe::Memory;

	cout << "Running core-test-memory1" << endl;

#if TEST_CHUNK
	Chunk chunk;
	chunk.Init(4, 10);
	Int32 *i = (Int32*)chunk.Allocate();
	cout << "The value of i " << *i << endl;
	*i = 100;
	cout << "The value of i " << *i << endl;
	chunk.Release();
#endif // TEST_CHUNK

	FixedAllocator fa;
	fa.Init(4, 5);

	Int32 *a = (Int32*)fa.Allocate();
	Int32 *b = (Int32*)fa.Allocate();
	Int32 *c = (Int32*)fa.Allocate();
	Int32 *d = (Int32*)fa.Allocate();
	Int32 *e = (Int32*)fa.Allocate();
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

	int *f = (Int32*)fa.Allocate();
	int *g = (Int32*)fa.Allocate();
	int *h = (Int32*)fa.Allocate();
	int *i = (Int32*)fa.Allocate();

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


	//SmallObjectAllocator(Axe::UInt pageSize, Axe::UInt maxObjectSize, Axe::UInt objectAlignSize);
	const UInt pageSize = 10;
	const UInt maxObjectSize = 128;
	const UInt objectAlignSize = 16;
	SmallObjectAllocator so(pageSize, maxObjectSize, objectAlignSize);

	cout << "Finished running core-test-memory1" << endl;

	return 0;
}

