#include <algorithm>
#include <iostream>

#include "fixedallocator.h"

#define Assert(...) 

///////////////////////////////////////////////////////////////////////////////////////////////
void StagnantFixedAllocator::Init(unsigned int singleAllocationSize, unsigned int numberOfAllocations, unsigned char * buffer)
{
    Assert(ASSERT_TJB, singleAllocationSize > 0);
    Assert(ASSERT_TJB, numberOfAllocations >= singleAllocationSize);
    Assert(ASSERT_TJB, buffer);

    m_uiBlockSize = singleAllocationSize;
    m_uiNumberOfBlocks = numberOfAllocations;
    m_Chunk.Init(singleAllocationSize, numberOfAllocations, buffer);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void * StagnantFixedAllocator::Allocate()
{
    Assert(ASSERT_ALL, (NULL == m_Chunk) || (m_Chunk.HasAvailable(m_uiNumberOfBlocks)));
    Assert(ASSERT_ALL, !m_Chunk.IsFull());
    void *pReturn = m_Chunk.Allocate();
    return pReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool StagnantFixedAllocator::Deallocate(void *pointer, ChunkT* hint)
{
    Assert(ASSERT_ALL, pointer);
    Assert(ASSERT_ALL, hint);
    Assert(ASSERT_ALL, m_Chunk);

    bool hasPointer = m_Chunk.HasBlock(pointer);
    Assert(ASSERT_ALL, hasPointer);

#if MEMORY_EXTREME_TEST
    if(m_Chunk.IsCorrupt())
    {
        Assert(ASSERT_ALL, false);
    }
    if(m_Chunk.IsBlockAvailable(pointer))
    {
        Assert(ASSERT_ALL, false);
    }
#endif // MEMORY_EXTREME_TEST
    if(hasPointer)
    {
        m_Chunk.Deallocate(pointer);
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool StagnantFixedAllocator::IsCorrupt() const
{
    m_Chunk.IsCorrupt();
}

///////////////////////////////////////////////////////////////////////////////////////////////
void StagnantFixedAllocator::PrintStats() const
{
    std::cout   << "\tBlock Size: " << m_uiBlockSize << std::endl
                << "\tNumber of Blocks " << m_uiNumberOfBlocks << std::endl;
}

