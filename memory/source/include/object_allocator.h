#ifndef _OBJECT_ALLOCATOR_H_
#define _OBJECT_ALLOCATOR_H_

#include "allocators.h"
#include "fixed_size_allocators.h"

//Fixed size allocators.
typedef Fixed_Size_Allocator<size_t, New_Delete_Allocator, No_Growth_Policy> No_Growth_New_Allocator;
typedef Fixed_Size_Allocator<size_t, Malloc_Free_Allocator, No_Growth_Policy> No_Growth_Malloc_Allocator;

//Constant growth allocators.
typedef Fixed_Size_Allocator<size_t, New_Delete_Allocator, Constant_Growth_Policy> Growing_New_Allocator;
typedef Fixed_Size_Allocator<size_t, Malloc_Free_Allocator, Constant_Growth_Policy> Growing_Malloc_Allocator;

typedef No_Growth_New_Allocator Default_Allocator;

/// \struct ObjectAllocationSizes
///
///
struct ObjectAllocationSizes
{
    int number_fixed_allocations;   //< The number of fixed sized allocations.
    int fixed_allocator_size;       //< The size of a chunk of memory stored in a the fixed object allocator.
};

/// \class Object_Allocator
///
///
template<class Allocator>
class Object_Allocator
{
public:
    ///
    /// \precond Assumes that the pools have been created in order from smallest to largest.
    void Init(ObjectAllocationSizes *fixed_pool_sizes, size_t number_of_fixed_pools);

    inline void * allocate(size_t size);
    //inline bool deallocate(void * pointer);
    inline bool deallocate(void * pointer, size_t size_of_allocation);

private:
    Allocator * find_appropriate_pool(size_t size) const;

private:
    ObjectAllocationSizes *oas;
    Allocator * pools;
    size_t number_of_pools;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
void Object_Allocator<Allocator>::Init(ObjectAllocationSizes *fixed_pool_sizes, size_t number_of_fixed_pools)
{
    number_of_pools = number_of_fixed_pools;
    oas = fixed_pool_sizes;
    pools = new Allocator[number_of_pools];

    for(int i = 0; i < number_of_pools; ++i)
    {
        pools[i].construct(oas[i].number_fixed_allocations, oas[i].fixed_allocator_size);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
inline Allocator * Object_Allocator<Allocator>::find_appropriate_pool(size_t size) const
{
    for(int i = 0; i < number_of_pools; ++i)
    {
        if(oas[i].fixed_allocator_size >= size)
        {
            return &(pools[i]);
        }
    }

    //Assert(TJB, "Should Never hit here");
    return NULL;
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
inline void * Object_Allocator<Allocator>::allocate(size_t size)
{
    //Find the appropriate pool.
    Allocator * appropriate_allocator = find_appropriate_pool(size);
    if(appropriate_allocator)
    {
        //We found a allocator
        return appropriate_allocator->allocate();
    }
    else
    {
        //Assert(TJB, "Unable to find a appropriate allocator");
        return NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//template<>
//inline bool Object_Allocator<No_Growth_Malloc_Allocator>::deallocate(void *pointer)
//{
//    //Assert(TJB, "DEATH!!! If the underlying pool is a FastEmbedded then you must track the size of the allocation");
//    //TJB: MAKE THIS FUNCTION DIE HERE!!!
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//template<>
//inline bool Object_Allocator<No_Growth_New_Allocator>::deallocate(void *pointer)
//{
//    //Assert(TJB, "DEATH!!! If the underlying pool is a FastEmbedded then you must track the size of the allocation");
//    //TJB: MAKE THIS FUNCTION DIE HERE!!!
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
inline bool Object_Allocator<Allocator>::deallocate(void *pointer, size_t size_of_allocation)
{
    //Find the appropriate pool.
    Allocator * appropriate_allocator = find_appropriate_pool(size_of_allocation);
    bool success = false;
    if(appropriate_allocator)
    {
        appropriate_allocator->deallocate(static_cast<unsigned char *>(pointer));

        //NOTE: WE ARE ASSUMING THAT THE FREE WORKED!!! IT IS A BIG ASSUMPTION!!! BUT IT'S FAST!!!
        return true;
    }
    return false;
}

#endif // _OBJECT_ALLOCATOR_H_

