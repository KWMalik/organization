#include "test_allocators.h"

int main()
{
    Test_New_Delete_Allocator();
    Test_Malloc_Free_Allocator();

    Test_Fixed_Size_Allocator();
    Test_Fixed_Size_Type_Allocator();
 
    Test_Memory_Pool();

    Test_Object_Allocator();

    return 0;
}

