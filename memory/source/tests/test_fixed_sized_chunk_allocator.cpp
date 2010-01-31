#include <iostream>
#include <stdlib.h>

#include "allocators.h"

using namespace std;

struct Vector4f
{
    float x, y, z, w;

	friend std::ostream& operator<<(std::ostream& os, Vector4f & vec)
    {
        os << "x = " << vec.x << " y = " << vec.y << " z = " << vec.z << " w = " << vec.w;
        return os;
    }
};

void Test_Fixed_Sized_Chunk_Allocator()
{
    cout << "Running Test_Fixed_Sized_Chunk_Allocator Tests" << endl;

    typedef Fixed_Sized_Allocator<size_t, New_Delete_Allocator, No_Growth_Policy> MyFixedAllocator;
    //Fixed_Size_Type_Allocator 
    MyFixedAllocator fixed_allocator; 

    const int buffer_count = 4;
    fixed_allocator.construct(buffer_count, sizeof(Vector4f));

	Vector4f *v1 = reinterpret_cast<Vector4f *>(fixed_allocator.allocate());
	Vector4f *v2 = reinterpret_cast<Vector4f *>(fixed_allocator.allocate());
	Vector4f *v3 = reinterpret_cast<Vector4f *>(fixed_allocator.allocate());
	Vector4f *v4 = reinterpret_cast<Vector4f *>(fixed_allocator.allocate());

	v1->x = 1;
	v1->y = 2;
	v1->z = 3;
	v1->w = 4;
	
	v2->x = 5;
	v2->y = 6;
	v2->z = 7;
	v2->w = 8;
	
	v3->x = 9;
	v3->y = 10;
	v3->z = 11;
	v3->w = 12;
	
	v4->x = 13;
	v4->y = 14;
	v4->z = 15;
	v4->w = 16;
	
	cout << "Vector4f #1: " << *v1 << endl;
	cout << "Vector4f #2: " << *v2 << endl;
 	cout << "Vector4f #3: " << *v3 << endl;
	cout << "Vector4f #4: " << *v4 << endl;
	
    fixed_allocator.deallocate(reinterpret_cast<unsigned char *>(v2));
    fixed_allocator.deallocate(reinterpret_cast<unsigned char *>(v4));	
	
	
    cout << "DONE Running Test_FixedSizedChunkAllocator Tests" << endl << endl;
}


