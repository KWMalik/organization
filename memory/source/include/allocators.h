/// \file allocators.h
/// \author Toby Banks
///
/// When accessing these classes it's best to use the typedef's at the bottom of the file.
///
///
#ifndef _ALLOCATORS_H_
#define _ALLOCATORS_H_

#include "growth_policies.h"
#include "pool.h"

/// \class New_Delete_Allocator
///
/// Creates/destroys memory using standard new and delete operators.
///
///
class New_Delete_Allocator
{
public:
    static inline unsigned char * allocate(unsigned int size) { return new unsigned char[size]; }
    static inline void deallocate(unsigned char * pointer) { delete []pointer; pointer = 0; }
};

/// \class Malloc_Free_Allocator
///
/// Creates/destroys memory using malloc/free.
///
///
class Malloc_Free_Allocator
{
public:
    static inline unsigned char * allocate(unsigned int size) { return static_cast<unsigned char *>(malloc(size)); }
    static inline void deallocate(unsigned char * pointer) { free(pointer); pointer = 0; }
};


/// \class FixedSizeChunkAllocator
///
///
template<
            class SizeT, 
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
class Fixed_Sized_Allocator : public GrowthPolicy<SizeT, Allocator>
{
public:
    typedef MemoryPool<SizeT> pool;
    typedef SizeT size_type;

    Fixed_Sized_Allocator() {}

    ~Fixed_Sized_Allocator() 
    {
        if(buffer)
        {
            Allocator::deallocate(buffer);
            buffer = 0;
        }
    }

    void construct(unsigned long numberOfElements, unsigned long chunkSize)
    {
        buffer = Allocator::allocate(numberOfElements * chunkSize);
        fixed_pool.add_block(reinterpret_cast<void *>(buffer),
                             numberOfElements * chunkSize,
                             chunkSize);

        GrowthPolicy<size_type, Allocator>::create_growth_policy(numberOfElements, chunkSize);
    }

    unsigned char * allocate()
    {
        unsigned char * ret = static_cast<unsigned char *>(fixed_pool.allocate());

        if(ret == NULL && GrowthPolicy<SizeT, Allocator>::can_grow())
        {
            //We are out of memory so we should grow
            void * new_memory_block = GrowthPolicy<SizeT, Allocator>::grow();

            if(new_memory_block)
            {
                fixed_pool.add_block(new_memory_block, GrowthPolicy<SizeT, Allocator>::grow_size(), GrowthPolicy<SizeT, Allocator>::element_size());
            }
            else
            {
                //Assert(TJB, "DEATH!!!!");
            }

            ret = static_cast<unsigned char *>(fixed_pool.allocate());
        }
        return ret;
    }

    void deallocate(unsigned char * buffer)
    {
        fixed_pool.deallocate(buffer);
    }

private:
    pool fixed_pool;
    unsigned char * buffer;
};

/// \class TypeAllocator
///
template<
            class TElement,
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
class Fixed_Size_Type_Allocator : public GrowthPolicy<size_t, Allocator>
{
public:
    typedef size_t size_type;
    typedef MemoryPool<size_type> pool;

    ///
    Fixed_Size_Type_Allocator() {}

    ///
    ~Fixed_Size_Type_Allocator() 
    {
        if(buffer)
        {
            Allocator::deallocate(buffer);
            buffer = 0;
        } 
    }

    ///
    void construct(unsigned long numberOfElements, unsigned long chunkSize = sizeof(TElement))
    {
        buffer = Allocator::allocate(numberOfElements * chunkSize);
        type_pool.add_block(reinterpret_cast<void *>(buffer), numberOfElements * sizeof(TElement), sizeof(TElement));
    }

    ///
    TElement * allocate() { return reinterpret_cast<TElement *>(type_pool.allocate()); }

    ///
    void deallocate(TElement * element) { type_pool.deallocate(reinterpret_cast<void *>(element)); }

private:
    pool type_pool;
    unsigned char * buffer;
};

#endif // _ALLOCATORS_H_

