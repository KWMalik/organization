#include <iostream>
#include <stdlib.h>

#include "allocators.h"
#include "growth_policies.h"
#include "fixed_size_allocators.h"

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

void Test_Fixed_Size_Type_Allocator()
{
	cout << "Running Test_Type_Allocator Tests" << endl;
	const int buffer_count = 4;
	
	typedef Fixed_Size_Type_Allocator<Vector4f, New_Delete_Allocator, No_Growth_Policy> MyTypeAllocator;
	MyTypeAllocator type_allocator; 

	type_allocator.construct(sizeof(Vector4f), buffer_count); 
	Vector4f * v1 = type_allocator.allocate();
	Vector4f * v2 = type_allocator.allocate();
	Vector4f * v3 = type_allocator.allocate();
	Vector4f * v4 = type_allocator.allocate();

	v1->x = 1;
	v1->y = 2;
	v1->z = 3;
	v1->w = 4;

	v2->x = 5;
	v2->y = 6;
	v2->z = 7;
	v2->w = 8;

	v3->x = 5;
	v3->y = 6;
	v3->z = 7;
	v3->w = 8;

	v4->x = 9;
	v4->y = 10;
	v4->z = 11;
	v4->w = 12;

	cout << "Element #1: " << *v1 << endl;
	cout << "Element #2: " << *v2 << endl;
	cout << "Element #3: " << *v3 << endl;
	cout << "Element #4: " << *v4 << endl;

	type_allocator.deallocate(v1);
	type_allocator.deallocate(v2);
	type_allocator.deallocate(v3);
	type_allocator.deallocate(v4);

	cout << "DONE Running Test_Type_Allocator Tests" << endl << endl;
}

