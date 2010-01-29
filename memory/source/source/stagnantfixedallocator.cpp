#include <algorithm>
#include <iostream>

#include "fixedallocator.h"

#define Assert(...) 

///////////////////////////////////////////////////////////////////////////////////////////////
void StagnantFixedAllocator::Init(unsigned int singleAllocationSize, unsigned int numberOfAllocations, char * buffer)
{
    Assert(ASSERT_TJB, singleAllocationSize > 0);
    Assert(ASSERT_TJB, numberOfAllocations >= singleAllocationSize);
    Assert(ASSERT_TJB, buffer);

    m_uiBlockSize = singleAllocationSize;

    unsigned int numberOfBlocks = numberOfAllocations / singleAllocationSize;
    m_uiNumberOfBlocks = static_cast<unsigned char>(numberOfBlocks);
    Assert(ASSERT_ALL, numberOfBlocks == m_uiNumberOfBlocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void * StagnantFixedAllocator::Allocate()
{
    Assert(ASSERT_ALL, (NULL == m_pChunk) || (m_pChunk->HasAvailable(m_uiNumberOfBlocks)));
    Assert(ASSERT_ALL, !m_pChunk->IsFull());
    void *pReturn = m_pAllocChunk->Allocate();
    return pReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool StagnantFixedAllocator::Deallocate(void *pointer, Chunk* hint)
{
    Assert(ASSERT_ALL, pointer);
    Assert(ASSERT_ALL, hint);
    Assert(ASSERT_ALL, m_pChunk);

    bool hasPointer = m_pChunk->HasBlock(pointer);
    Assert(ASSERT_ALL, hasPointer);

#if MEMORY_EXTREME_TEST
    if(m_pChunk->IsCorrupt())
    {
        Assert(ASSERT_ALL, false);
    }
    if(m_pChunk->IsBlockAvailable(pointer))
    {
        Assert(ASSERT_ALL, false);
    }
#endif // MEMORY_EXTREME_TEST
    if(hasPointer)
    {
        m_pDeallocChunk->Deallocate(hasPointer);
        return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool StagnantFixedAllocator::IsCorrupt() const
{
    m_pChunk->IsCorrupt();
}

///////////////////////////////////////////////////////////////////////////////////////////////
void StagnantFixedAllocator::PrintStats() const
{
    std::cout   << "\tBlock Size: " << m_uiBlockSize << std::endl
                << "\tNumber of Blocks " << m_uiNumberOfBlocks << std::endl;
}

