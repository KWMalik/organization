#include <iostream>
#include <stdlib.h>

#include "allocators.h"

using namespace std;

void Test_New_Delete_Allocator()
{
	cout << "Running Test_New_Delete_Allocator Tests" << endl;
	const int int_buffer_size = 16;;
	int * int_buffer = reinterpret_cast<int *>(New_Delete_Allocator::allocate(16));

	int_buffer[0] =  5;
	int_buffer[1] = 10;
	int_buffer[2] = 15;
	int_buffer[3] = 20;

	for(int i = 0; i < 16 / sizeof(int); ++i)
	{
		cout << "Element #" << i << " = " << int_buffer[i] << endl;
	}

	New_Delete_Allocator::deallocate(reinterpret_cast<unsigned char *>(int_buffer));
	cout << "DONE Running Test_New_Delete_Allocator Tests" << endl << endl;
}

void Test_Malloc_Free_Allocator()
{
	cout << "Running Test_Malloc_Free_Allocator Tests" << endl;
	const int int_buffer_size = 16;;
	int * int_buffer = reinterpret_cast<int *>(Malloc_Free_Allocator::allocate(16));

	int_buffer[0] =  5;
	int_buffer[1] = 10;
	int_buffer[2] = 15;
	int_buffer[3] = 20;

	for(int i = 0; i < 16 / sizeof(int); ++i)
	{
		cout << "Element #" << i << " = " << int_buffer[i] << endl;
	}

	Malloc_Free_Allocator::deallocate(reinterpret_cast<unsigned char *>(int_buffer));
	cout << "DONE Running Test_Malloc_Free_Allocator Tests" << endl << endl;
}

