#ifndef _ALLOCATORS_H_
#define _ALLOCATORS_H_


/// \class New_Delete_Allocator
///
/// Creates/destroys memory using standard new and delete operators.
///
///
class New_Delete_Allocator
{
public:
    static unsigned char * allocate(unsigned int size) { return new unsigned char[size]; }
    static void deallocate(unsigned char * pointer) { delete []pointer; pointer = 0; }
};

/// \class Malloc_Free_Allocator
///
/// Creates/destroys memory using malloc/free.
///
///
class Malloc_Free_Allocator
{
public:
    static unsigned char * allocate(unsigned int size) { return static_cast<unsigned char *>(malloc(size)); }
    static void deallocate(unsigned char * pointer) { free(pointer); pointer = 0; }
};


/// \class FixedSizeChunkAllocator
///
///
///
template<class TElement, unsigned int TNumberOfElements, class Allocator = New_Delete_Allocator>
class FixedSizedChunkAllocator
{
public:
    typedef FastEmbeddedPool<unsigned long> pool;
    typedef pool::size_type  size_type;

    FixedSizedChunkAllocator() 
    {
        buffer = Allocator::allocate(sizeof(TElement) * TNumberOfElements);
        fixed_pool.add_block(reinterpret_cast<void *>(buffer),
                             sizeof(TElement) * TNumberOfElements,
                             sizeof(TElement));
    }

    ~FixedSizedChunkAllocator() 
    {
        Allocator::deallocate(buffer);
        buffer = 0;
    }

    unsigned char * allocate(unsigned int size) { return static_cast<unsigned char *>(fixed_pool.malloc()); }
    void deallocate(unsigned char * buffer) { fixed_pool.free(buffer); }

private:
    pool fixed_pool;
    unsigned char * buffer;

   // New_Delete_Allocator allocator;
};

/// \class TypeAllocator
///
///
///
template<class TElement, unsigned int TNumberOfElements, class Allocator = New_Delete_Allocator>
class TypeAllocator 
{
public:
    typedef FastEmbeddedPool<unsigned long> pool;
    typedef pool::size_type size_type;

    TypeAllocator() 
    {
        buffer = Allocator::allocate(sizeof(TElement) * TNumberOfElements);
        type_pool.add_block(reinterpret_cast<void *>(buffer), sizeof(TElement) * TNumberOfElements, sizeof(TElement));
    }

    ~TypeAllocator() 
    {
        Allocator::deallocate(buffer);
        buffer = 0;
    }

    TElement * allocate(unsigned int size) { return reinterpret_cast<TElement *>(type_pool.malloc()); }
    void deallocate(TElement * element) { type_pool.free(reinterpret_cast<void *>(element)); }

private:
    pool type_pool;
    unsigned char * buffer;

   // New_Delete_Allocator allocator;
};



#endif // _ALLOCATORS_H_

