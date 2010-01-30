#include <iostream>
#include <stdlib.h>

#include "fixedallocator.h"
#include "smallobjectallocator.h"

using namespace std;

void Test_Fixed_Allocator()
{
	cout << "Running FixedAllocator Test" << endl;

    unsigned char * buffer = new unsigned char[256];
    FixedAllocator fa;
    fa.Init(sizeof(int), 64, buffer);

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

