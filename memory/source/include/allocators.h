#ifndef _ALLOCATORS_H_
#define _ALLOCATORS_H_

#include "fastembeddedpool.h"

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
template<class Allocator = New_Delete_Allocator>
class FixedSizedChunkAllocator
{
public:
    typedef FastEmbeddedPool<unsigned long> pool;
    typedef pool::size_type  size_type;

    FixedSizedChunkAllocator() {}

    ~FixedSizedChunkAllocator() 
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
    }

    unsigned char * allocate() { return static_cast<unsigned char *>( fixed_pool.malloc()); }

    void deallocate(unsigned char * buffer) { fixed_pool.free(buffer); }

private:
    pool fixed_pool;
    unsigned char * buffer;
};

/// \class TypeAllocator
///
template<class TElement, class Allocator = New_Delete_Allocator>
class TypeAllocator 
{
public:
    typedef FastEmbeddedPool<unsigned long> pool;
    typedef pool::size_type size_type;

    ///
    TypeAllocator() {}

    ///
    ~TypeAllocator() 
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
    TElement * allocate() { return reinterpret_cast<TElement *>(type_pool.malloc()); }

    ///
    void deallocate(TElement * element) { type_pool.free(reinterpret_cast<void *>(element)); }

private:
    pool type_pool;
    unsigned char * buffer;

   // New_Delete_Allocator allocator;
};


#endif // _ALLOCATORS_H_

