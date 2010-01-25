#include "corepch.h"
#include "memory/chunk.h"
#include "debug/assert.h"

#include <bitset>

using namespace Axe;
using namespace Axe::Memory;
using namespace Axe::Debug;

///////////////////////////////////////////////////////////////////////////////////////////////
Bool Chunk::Init(const UInt size, const UInt8 numberOfElements)
{
    m_uiTotalNumElements = numberOfElements;
    m_uiSize = size;

    const UInt sizeToAllocate = m_uiSize * m_uiTotalNumElements;
    m_pData = static_cast<UInt8*>(malloc(sizeToAllocate));
    memset(m_pData, 0, sizeToAllocate);

    Reset();

    return True;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void * Chunk::Allocate()
{
    Assert(ASSERT_ALL, !IsFull(), "Trying to allocate when this chunk is full.\n");
    void * pReturn = static_cast<void*>(&(m_pData[m_uiSize * m_uiFirstAvailableBlock]));
    
    //Set our next block
    UInt8* nextBlock = static_cast<UInt8*>(pReturn);
    m_uiFirstAvailableBlock = *nextBlock;

    --m_uiBlocksAvailable;

    return pReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void Chunk::Deallocate(void *pointer)
{
#if AXE_DEBUG
    Assert(ASSERT_ALL, (pointer >= m_pData), "Pointer is outside of the memory range of this block.\n");
    Assert(ASSERT_ALL, (pointer <= static_cast<void*>(m_pData + (m_uiSize * m_uiTotalNumElements))), 
        "Pointer is outside of the memory range of this block.\n");
#endif // AXE_DEBUG

    UInt8* release = static_cast<UInt8*>(pointer);

#if AXE_DEBUG
    //Alignment check.
    Assert(ASSERT_ALL, (((release - m_pData) % m_uiSize) == 0));
#endif // AXE_DEBUG

    //Calculate the index of this block.
    UInt8 index = static_cast<UInt8>((release - m_pData) / m_uiSize);

#if AXE_DEBUG
    //Check to see if the block was already deleted.
    Assert(ASSERT_ALL, m_uiFirstAvailableBlock != index);
#endif // AXE_DEBUG

    *release = m_uiFirstAvailableBlock;
    m_uiFirstAvailableBlock = index;

#if AXE_DEBUG
    Assert(ASSERT_ALL, (m_uiFirstAvailableBlock == (release - m_pData) / m_uiSize));
#endif // AXE_DEBUG

    ++m_uiBlocksAvailable;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void Chunk::Reset()
{
    Assert(ASSERT_ALL, m_uiSize > 0, "Invalid size");
    Assert(ASSERT_ALL, m_uiTotalNumElements > 0, "There are no blocks available.\n");
    Assert(ASSERT_ALL, ((m_uiSize * m_uiTotalNumElements) / m_uiSize) == m_uiTotalNumElements);

    m_uiFirstAvailableBlock = 0;
    m_uiBlocksAvailable = m_uiTotalNumElements;

    UInt8 i = 1;
    for(UInt8 * p = m_pData; i != m_uiTotalNumElements; p += m_uiSize)
    {
        *p = i;
        ++i;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
void Chunk::Release()
{
    Assert(ASSERT_TJB, m_pData != 0, "You are trying to release a chunk that has not been initialized.\n");

    free(m_pData);
    m_pData = 0;

    m_uiFirstAvailableBlock = 0;
    m_uiBlocksAvailable = 0;
    m_uiTotalNumElements = 0;
    m_uiSize = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
Bool Chunk::IsCorrupt() const 
{
    if(m_uiTotalNumElements < m_uiBlocksAvailable)
    {
        Assert(ASSERT_ALL, false, "Chunk is corrupted.\n");
        return True;
    }

    if(IsFull())
    {
        //If we are full we can't do corruption checks on the indexes.
        return False;
    }

    if(m_uiTotalNumElements <= m_uiFirstAvailableBlock)
    {
        Assert(ASSERT_ALL, False, "Chunk is corrupted.\n");
        return True;
    }

    // Time to test our linked list.
    std::bitset<UCHAR_MAX> foundBlocks;
    UInt8 index = m_uiFirstAvailableBlock;
    UInt8 cc = 0;
    UInt8 * nextBlock = 0;

    while(True)
    {
        if(cc >= m_uiBlocksAvailable)
        {
            //We've successfully checked the linked list.
            break;
        }

        nextBlock = m_pData + (index * m_uiSize);
        foundBlocks.set(index, true);

        //Advance to the next block.
        index = *nextBlock;
        if(m_uiTotalNumElements <= index)
        {
            //We have a index that is out of range of this block.
            Assert(ASSERT_TJB, False, "Index is out of range.\n");
            return True;
        }

        if(foundBlocks.test(index))
        {
            //We have a index that is a duplicate.
            // i.e. 7 -> 10 -> 29 -> 7 (Corrupt list. 7 is a duplicate)
            Assert(ASSERT_ALL, False, "We have a index that is a duplicate");
            return True;
        }
        ++cc;
    } // End while(True);

    if(foundBlocks.count() != m_uiBlocksAvailable)
    {
        // The linked list is corrupt.
        Assert(ASSERT_ALL, False, "The linked list is corrupt.\n");
        return True;
    }

    return False;
}

///////////////////////////////////////////////////////////////////////////////////////////////
Bool Chunk::IsBlockAvailable(void * pointer) const
{
    if(IsFull())
    {
        return False;
    }
    UInt8 *block = static_cast<UInt8*>(pointer);

    //Alignment check.
    Assert(ASSERT_ALL, (((block - m_pData) * m_uiSize) == 0));

    //Calculate the index of block.
    UInt8 blockIndex = static_cast<UInt8>((block - m_pData) / m_uiSize);

    UInt8 index = m_uiFirstAvailableBlock;
    Assert(ASSERT_ALL, m_uiTotalNumElements > index);

    if(index == blockIndex)
    {
        //The index of the parameter pointer is actually pointing to 
        //our first available block (m_uiFirstAvailableBlock).
        return True;
    }

    //We need to iterate over our linked list of available blocks
    std::bitset<UCHAR_MAX> foundBlocks;
    UInt8 *nextBlock = 0;
    UInt8 cc = 0;
    while(True)
    {
        nextBlock = (m_pData + (index * m_uiSize));
        foundBlocks.set(index, true);
        ++cc;

        if(cc >= m_uiBlocksAvailable)
        {
            //Successfully iterated through available nodes in
            //our linked list.
            break;
        }
        index = *nextBlock;
        if(index == blockIndex)
        {
            // We found a available block.
            return True;
        }
        Assert(ASSERT_ALL, m_uiTotalNumElements > index, "Index is out of range.\n");
        Assert(ASSERT_ALL, !foundBlocks.test(index));
    } // while(True)

    return False;
}

