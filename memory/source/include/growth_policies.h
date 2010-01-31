#ifndef _GROWTH_POLICIES_H_
#define _GROWTH_POLICIES_H_

#include "allocators.h"

/// \class No_Growth_Policy
///
/// 
template<class SizeT, class Allocator>
class No_Growth_Policy
{
public:
    typedef SizeT size_type;

public:
    No_Growth_Policy() {}

    inline bool growth_policy_initialized() const
    {
        //We have a growth policy.. It's to do nothing.
        return true;
    }

    inline size_type block_size() const { return 0; }
    inline size_type element_size() const { return 0; }

    inline void create_growth_policy(size_type blockSize, size_type allocSize) {}

    inline void * grow() { /* Assert(TJB, "GROWING OF THIS CLASS IS NOT ALLOWED"); */ /*exit();*/ return NULL; }
};




/// \class Constant_Growth_Policy
///
template<class SizeT, class Allocator>
class Constant_Growth_Policy
{
public:
    typedef SizeT size_type;

protected:
    size_type size_of_block;            ///< The number of bytes to request when growing.
    size_type allocation_size;          ///< The size of a single block of memory in the pool.

public:
    Constant_Growth_Policy() : size_of_block(0), allocation_size(0) {}

    inline bool growth_policy_initialized() const
    {
        return size_of_block ? true : false;
    }

    inline size_type block_size() const { return size_of_block; }
    inline size_type element_size() const { return allocation_size; }

    inline void create_growth_policy(size_type blockSize, size_type allocSize)
    {
        size_of_block = blockSize;
        allocation_size = allocSize;
    }

    inline void * grow()
    {
        return Allocator::allocate(size_of_block);
    }
};

#endif //_GROWTH_POLICIES_H_

