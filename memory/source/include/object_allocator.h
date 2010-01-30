#ifndef _OBJECT_ALLOCATOR_H_
#define _OBJECT_ALLOCATOR_H_

#include "allocators.h"

/// \struct ObjectAllocationSizes
///
///
struct ObjectAllocationSizes
{
    int number_fixed_allocations;   //< The number of fixed sized allocations.
    int fixed_allocator_size;       //< The size of a chunk of memory stored in a the fixed object allocator.
};


template<class Allocator>
class ObjectAllocator
{ 
public:
    ObjectAllocator() {}
    ~ObjectAllocator() {}

    ///
    /// \precond Assumes that the pools have been created in order from smallest to largest.
    void Init(ObjectAllocationSizes *fixed_pool_sizes, size_t number_of_fixed_pools);

    inline void * Allocate(size_t size);
    inline bool Deallocate(void * pointer);
    inline bool Deallocate(void * pointer, size_t size_of_allocation);

private:
    Allocator * FindAppropriatePool(size_t size) const;

private:
    ObjectAllocationSizes *oas;
    Allocator * pools;
    size_t number_of_pools;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
void ObjectAllocator<Allocator>::Init(ObjectAllocationSizes *fixed_pool_sizes, size_t number_of_fixed_pools)
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
inline Allocator * ObjectAllocator<Allocator>::FindAppropriatePool(size_t size) const
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
inline void * ObjectAllocator<Allocator>::Allocate(size_t size)
{
    //Find the appropriate pool.
    Allocator * appropriate_allocator = FindAppropriatePool(size);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline bool ObjectAllocator<FixedSizedChunkAllocator<> >::Deallocate(void *pointer)
{
    //Assert(TJB, "DEATH!!! If the underlying pool is a FastEmbedded then you must track the size of the allocation");
    //TJB: MAKE THIS FUNCTION DIE HERE!!!
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
inline bool ObjectAllocator<Allocator>::Deallocate(void *pointer, size_t size_of_allocation)
{
    //Find the appropriate pool.
    Allocator * appropriate_allocator = FindAppropriatePool(size_of_allocation);
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

