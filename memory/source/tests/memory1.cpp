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

int main()
{
	cout << "Running core-test-memory1" << endl;

#if TEST_CHUNK
	Chunk chunk;
	chunk.Init(4, 10);
	int *i = (int*)chunk.Allocate();
	cout << "The value of i " << *i << endl;
	*i = 100;
	cout << "The value of i " << *i << endl;
	chunk.Release();
#endif // TEST_CHUNK

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


	const unsigned int pageSize = 10;
	const unsigned int maxObjectSize = 128;
	const unsigned int objectAlignSize = 16;
	SmallObjectAllocator so(pageSize, maxObjectSize, objectAlignSize);

	cout << "Finished running core-test-memory1" << endl;

	return 0;
}

