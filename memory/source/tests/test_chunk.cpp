#include <iostream>
#include <stdlib.h>

#include "chunk.h"

using namespace std;

//#define TEST_CHUNK 1

//Change Chunk.h so that everything is public and then compile 
//again if you want to test the chunk class 
//
//Comment out this error once Chunk.h is public.
#if TEST_CHUNK
#error See above comment
#endif // TEST_CHUNK


void Test_Chunk()
{
#if TEST_CHUNK
	cout << "Running TestChunk Test" << endl;

    Chunk<> chunk;
    chunk.Init(4, 10);
    int *i = (int*)chunk.Allocate();
    cout << "\tThe value of i " << *i << endl;
    *i = 100;
    cout << "\tThe value of i " << *i << endl;
    chunk.Release();

	cout << "Done running TestChunk Test" << endl << endl;
#endif // TEST_CHUNK
}

