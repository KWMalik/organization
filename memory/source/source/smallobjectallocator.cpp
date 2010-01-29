#include "smallobjectallocator.h"

#include <iostream>

#define Assert(...) 


/// \ingroup SmallObjectGroupInternal
///
/// Calculates index into array where a FixedAllocator of numBytes is located.
inline unsigned int GetOffset(unsigned int numBytes, unsigned int alignment)
{
    const unsigned int alignExtra = alignment-1;
    return (numBytes + alignExtra) / alignment;
}

/////////////////////////////////////////////////////////////////////////////////
/// \function DefaultAllocator
/// \author Toby Banks
///
/// Calls the default allocator when SmallObjectAllocator decides not to handle a
/// request.  SmallObjectAllocator calls this if the number of bytes is bigger than
/// the size which can be handled by any FixedAllocator.
///
/// \param numBytes number of bytes
/// \param doThrow true if this function should throw an exception. If false if 
///                it should indicate failure by returning a NULL pointer.
void * DefaultAllocator( unsigned int numBytes, bool doThrow )
{
    return doThrow ? ::operator new(numBytes) : ::operator new(numBytes, std::nothrow_t());
}

/////////////////////////////////////////////////////////////////////////////////
/// \function DefaultDeallocator
/// \author Toby Banks
///
///
/// Calls default deallocator when SmallObjectAllocator decides not to handle a   
/// request.  The default deallocator could be the global delete operator or the
/// free function.  The free function is the preferred default deallocator since
/// it matches malloc which is the preferred default allocator.  SmallObjectAllocator
/// will call this if an address was not found among any of its own blocks.
/// 
/// \param p Pointer to the allocation to be freed.
void DefaultDeallocator( void * p )
{
    ::operator delete(p);
}

/////////////////////////////////////////////////////////////////////////////////
unsigned int SmallObjectAllocator::FindTotalSizeForTheFixedPool(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors)
{
    unsigned int fixedPoolSize = 0;
    for(int i = 0; i < numberOfAllocatorDescriptors; ++i)
    {
        fixedPoolSize += fad[i].SingleAllocationSize() * fad[i].NumberOfAllocations();

    }
    return fixedPoolSize;
}

/////////////////////////////////////////////////////////////////////////////////
void SmallObjectAllocator::InitializeTheFixedPools(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors, char * buffer)
{
    //Create a pointer to track our location in the pool as we create chunks.
    char * pointerToCurrentBlock = buffer; 

 	for(int i = 0; i < numberOfAllocatorDescriptors; ++i)
	{
        //Create a pool of objects that increase by the objectAlignSize
		m_pPool[i].Init(fad[i].SingleAllocationSize(), fad[i].NumberOfAllocations(), pointerToCurrentBlock);

        //Increment the pool pointer
        pointerToCurrentBlock += fad[i].SingleAllocationSize() * fad[i].NumberOfAllocations();
	}
}

/////////////////////////////////////////////////////////////////////////////////
unsigned int SmallObjectAllocator::DetermineMaxSmallObjectSize(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors)
{
    int largestSize = fad[0].SingleAllocationSize();;
 	for(int i = 1; i < numberOfAllocatorDescriptors; ++i)
	{
        if(fad[i].SingleAllocationSize() > largestSize)
        {
            largestSize = fad[i].SingleAllocationSize();
        }
    }
    return largestSize; 
}

/////////////////////////////////////////////////////////////////////////////////
unsigned int SmallObjectAllocator::DetermineObjectAlignSize(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors)
{
    //We will use the smallest pool size to determine our alignment (anything less than 8 will be considered 8).
    int smallestSize = fad[0].SingleAllocationSize();;
 	for(int i = 1; i < numberOfAllocatorDescriptors; ++i)
	{
        if(fad[i].SingleAllocationSize() < smallestSize)
        {
            smallestSize = fad[i].SingleAllocationSize();
        }
    }
    if(smallestSize < 8)
    {
        smallestSize = 8;
    }
    return smallestSize;
}

/////////////////////////////////////////////////////////////////////////////////
SmallObjectAllocator::SmallObjectAllocator(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors) : m_pPool(0), m_uiMaxSmallObjectSize(0), m_uiObjectAlignSize(8)
{
	Assert(ASSERT_TJB, fad);
	Assert(ASSERT_TJB, numberOfAllocatorDescriptors > 0);

    //Calculate the total size of the fixed pool.
    unsigned int fixedPoolSize = FindTotalSizeForTheFixedPool(fad, numberOfAllocatorDescriptors);

    //Allocate a single block of memory to parse out to the Fixed Pools.
    char *buffer = new char[fixedPoolSize];

    //Allocated a array of FixedAllocator buffers.
	m_pPool = new FixedAllocator[numberOfAllocatorDescriptors];

    //Determine the largest object size.
    m_uiMaxSmallObjectSize = DetermineMaxSmallObjectSize(fad, numberOfAllocatorDescriptors);

    //Determine the alignment size.
    m_uiObjectAlignSize = DetermineObjectAlignSize(fad, numberOfAllocatorDescriptors);

    //Hand out the memory in buffer to all of the fixed pools to watch over
    InitializeTheFixedPools(fad, numberOfAllocatorDescriptors, buffer);
}

/////////////////////////////////////////////////////////////////////////////////
SmallObjectAllocator::~SmallObjectAllocator()
{
	delete [] m_pPool;
}

/////////////////////////////////////////////////////////////////////////////////
bool SmallObjectAllocator::TrimExcessMemory()
{
    bool found = false;
    const unsigned int allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());

    for(unsigned int i = 0; i < allocCount; ++i)
    {
        if(m_pPool[i].TrimEmptyChunk())
		{
            found = true;
		}
    }
    for(unsigned int i = 0; i < allocCount; ++i)
    {
        if(m_pPool[i].TrimChunkContainer())
		{
            found = true;
	    }
	}

    return found;
}

/////////////////////////////////////////////////////////////////////////////////
void * SmallObjectAllocator::Allocate(unsigned int numBytes, bool doThrow)
{
    if(numBytes > GetMaxObjectSize() )
	{
        return DefaultAllocator( numBytes, doThrow );
	}

    Assert(ASSERT_MEM, NULL != m_pPool );
    if( 0 == numBytes )
	{
		numBytes = 1;
	}

    const unsigned int index = GetOffset(numBytes, GetAlignment()) - 1;
    const unsigned int allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    (void) allocCount;

    Assert(ASSERT_MEM, index < allocCount);
    FixedAllocator & allocator = m_pPool[index];
    Assert(ASSERT_MEM, allocator.BlockSize() >= numBytes);
    Assert(ASSERT_MEM, allocator.BlockSize() < numBytes + GetAlignment());

    void * place = allocator.Allocate();

    if((NULL == place) && TrimExcessMemory())
	{
        place = allocator.Allocate();
	}

    if ((NULL == place) && doThrow)
    {
#ifdef _MSC_VER
        throw std::bad_alloc("could not allocate small object");
#else
        // GCC did not like a literal string passed to std::bad_alloc.
        // so just throw the default-constructed exception.
        throw std::bad_alloc();
#endif
    }
    return place;
}

/////////////////////////////////////////////////////////////////////////////////
bool SmallObjectAllocator::Deallocate( void * p, unsigned int numBytes )
{
    if(NULL == p)
	{
		return false;
	}

    if(numBytes > GetMaxObjectSize())
    {
        DefaultDeallocator(p);
        return false;
    }
    Assert(ASSERT_MEM, NULL != m_pPool );

    if(0 == numBytes)
	{
		numBytes = 1;
	}
    const unsigned int index = GetOffset(numBytes, GetAlignment()) - 1;
    const unsigned int allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    (void) allocCount;

    Assert(ASSERT_MEM, index < allocCount);
    FixedAllocator & allocator = m_pPool[index];
    Assert(ASSERT_MEM, allocator.BlockSize() >= numBytes);
    Assert(ASSERT_MEM, allocator.BlockSize() < numBytes + GetAlignment());

    const bool found = allocator.Deallocate(p);
    (void) found;

    Assert(ASSERT_MEM, found);

    return found;
}

/////////////////////////////////////////////////////////////////////////////////
bool SmallObjectAllocator::Deallocate(void * p)
{
    if(!p)
	{
		return false;
	}
    Assert(ASSERT_MEM, NULL != m_pPool);

    FixedAllocator * pAllocator = NULL;
    const unsigned int allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    FixedAllocator::ChunkT * chunk = NULL;

    for (unsigned int i = 0; i < allocCount; ++i)
    {
        chunk = m_pPool[i].HasBlock(p);
        if(chunk)
        {
            pAllocator = &m_pPool[i];
            break;
        }
    }
    if (!pAllocator)
    {
        DefaultDeallocator(p);
        return false;
    }

    Assert(ASSERT_MEM, NULL != chunk);

    const bool found = pAllocator->Deallocate(p, chunk);
    (void) found;
    Assert(ASSERT_MEM, found);
    return found;
}

/////////////////////////////////////////////////////////////////////////////////
bool SmallObjectAllocator::IsCorrupt( void ) const
{
    if ( NULL == m_pPool )
    {
        Assert(ASSERT_MEM, false );
        return true;
    }
    if ( 0 == GetAlignment() )
    {
        Assert(ASSERT_MEM, false );
        return true;
    }
    if ( 0 == GetMaxObjectSize() )
    {
        Assert(ASSERT_MEM, false );
        return true;
    }
    const unsigned int allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    for ( unsigned int i = 0; i < allocCount; ++i )
    {
        if ( m_pPool[i].IsCorrupt() )
		{
            return true;
		}
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
void SmallObjectAllocator::PrintStats() const
{
    std::cout   << "SmallObjectAllocator Statistics:" << std::endl
                << "Max Object Size " << m_uiMaxSmallObjectSize << std::endl
	            << "Object Align Size " << m_uiObjectAlignSize << std::endl;

    const unsigned int allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    for(unsigned int i = 0; i < allocCount; ++i)
    {
        m_pPool[i].PrintStats();
    }
}

