#include "smallobjectallocator.h"

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
SmallObjectAllocator::SmallObjectAllocator(unsigned int pageSize, unsigned int maxObjectSize, unsigned int objectAlignSize) : 
					m_pPool(0), m_uiMaxSmallObjectSize(maxObjectSize), m_uiObjectAlignSize(objectAlignSize)
{
	Assert(ASSERT_TJB, 0 != objectAlignSize);
	const unsigned int allocCount = GetOffset(maxObjectSize, objectAlignSize);
	m_pPool = new FixedAllocator[allocCount];
	for(int i = 0; i < allocCount; ++i)
	{
		m_pPool[i].Init((i + 1) * objectAlignSize, pageSize);	
	}
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
void SmallObjectAllocator::Deallocate( void * p, unsigned int numBytes )
{
    if(NULL == p)
	{
		return;
	}

    if(numBytes > GetMaxObjectSize())
    {
        DefaultDeallocator(p);
        return;
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

    const bool found = allocator.Deallocate(p, NULL);
    (void) found;

    Assert(ASSERT_MEM, found);
}

/////////////////////////////////////////////////////////////////////////////////
void SmallObjectAllocator::Deallocate(void * p)
{
    if(!p)
	{
		return;
	}
    Assert(ASSERT_MEM, NULL != m_pPool);

    FixedAllocator * pAllocator = NULL;
    const unsigned int allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    Chunk * chunk = NULL;

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
        return;
    }

    Assert(ASSERT_MEM, NULL != chunk);

    const bool found = pAllocator->Deallocate(p, chunk);
    (void) found;
    Assert(ASSERT_MEM, found);
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

