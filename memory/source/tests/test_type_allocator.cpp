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

void Test_Type_Allocator()
{
    cout << "Running Test_Type_Allocator Tests" << endl;
    const int buffer_count = 4;
    
    TypeAllocator<Vector4f> type_allocator; 
    type_allocator.construct(sizeof(Vector4f), buffer_count); 
    Vector4f * vectors = type_allocator.allocate();

    for(int i = 0; i < buffer_count; ++i)
    {
        vectors[i].x = i + 1;
        vectors[i].y = i + 2;
        vectors[i].z = i + 3;
        vectors[i].x = 1;
    }

    for(int i = 0; i < buffer_count; ++i)
    {
        cout << "Element #" << i << " " << vectors[i] << endl;
    }

    type_allocator.deallocate(vectors);
    cout << "DONE Running Test_Type_Allocator Tests" << endl << endl;
}

