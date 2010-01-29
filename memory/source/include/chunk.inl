#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bitset>
#include <limits>

#define Assert(...) 


///////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
bool Chunk<Allocator>::Init(const unsigned int size, const unsigned char numberOfElements, char * buffer)
{
    m_uiTotalNumElements = numberOfElements;
    m_uiSize = size;

    //If we are passed a buffer we should allocate from it.
    const unsigned int sizeToAllocate = m_uiSize * m_uiTotalNumElements;
    m_pData = Allocator::_Allocate(sizeToAllocate);

//  m_pData = buffer;
//  m_pData = static_cast<unsigned char*>(malloc(sizeToAllocate));
 
    memset(m_pData, 0, sizeToAllocate);

    Reset();

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
void * Chunk<Allocator>::Allocate()
{
    Assert(ASSERT_ALL, !IsFull(), "Trying to allocate when this chunk is full.\n");
    void * pReturn = static_cast<void*>(&(m_pData[m_uiSize * m_uiFirstAvailableBlock]));
    
    //Set our next block
    unsigned char* nextBlock = static_cast<unsigned char*>(pReturn);
    m_uiFirstAvailableBlock = *nextBlock;

    --m_uiBlocksAvailable;

    return pReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
void Chunk<Allocator>::Deallocate(void *pointer)
{
    Assert(ASSERT_ALL, (pointer >= m_pData), "Pointer is outside of the memory range of this block.\n");
    Assert(ASSERT_ALL, (pointer <= static_cast<void*>(m_pData + (m_uiSize * m_uiTotalNumElements))), 
        "Pointer is outside of the memory range of this block.\n");

    unsigned char* release = static_cast<unsigned char*>(pointer);

#if DEBUG
    //Alignment check.
    Assert(ASSERT_ALL, (((release - m_pData) % m_uiSize) == 0));
#endif // DEBUG

    //Calculate the index of this block.
    unsigned char index = static_cast<unsigned char>((release - m_pData) / m_uiSize);

#if DEBUG
    //Check to see if the block was already deleted.
    Assert(ASSERT_ALL, m_uiFirstAvailableBlock != index);
#endif // DEBUG

    *release = m_uiFirstAvailableBlock;
    m_uiFirstAvailableBlock = index;

#if DEBUG
    Assert(ASSERT_ALL, (m_uiFirstAvailableBlock == (release - m_pData) / m_uiSize));
#endif // DEBUG

    ++m_uiBlocksAvailable;
}

///////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
void Chunk<Allocator>::Reset()
{
    Assert(ASSERT_ALL, m_uiSize > 0, "Invalid size");
    Assert(ASSERT_ALL, m_uiTotalNumElements > 0, "There are no blocks available.\n");
    Assert(ASSERT_ALL, ((m_uiSize * m_uiTotalNumElements) / m_uiSize) == m_uiTotalNumElements);

    m_uiFirstAvailableBlock = 0;
    m_uiBlocksAvailable = m_uiTotalNumElements;

    unsigned char i = 1;
    for(unsigned char * p = m_pData; i != m_uiTotalNumElements; p += m_uiSize)
    {
        *p = i;
        ++i;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
void Chunk<Allocator>::Release()
{
    Assert(ASSERT_TJB, m_pData != 0, "You are trying to release a chunk that has not been initialized.\n");

    Allocator::free(m_pData);
    m_pData = 0;

    m_uiFirstAvailableBlock = 0;
    m_uiBlocksAvailable = 0;
    m_uiTotalNumElements = 0;
    m_uiSize = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
bool Chunk<Allocator>::IsCorrupt() const 
{
    if(m_uiTotalNumElements < m_uiBlocksAvailable)
    {
        Assert(ASSERT_ALL, false, "Chunk is corrupted.\n");
        return true;
    }

    if(IsFull())
    {
        //If we are full we can't do corruption checks on the indexes.
        return false;
    }

    if(m_uiTotalNumElements <= m_uiFirstAvailableBlock)
    {
        Assert(ASSERT_ALL, false, "Chunk is corrupted.\n");
        return true;
    }

    // Time to test our linked list.
	const size_t lim = std::numeric_limits<unsigned char>::max();
    std::bitset<UCHAR_MAX> foundBlocks;
    unsigned char index = m_uiFirstAvailableBlock;
    unsigned char cc = 0;
    unsigned char * nextBlock = 0;

    while(true)
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
            Assert(ASSERT_TJB, false, "Index is out of range.\n");
            return true;
        }

        if(foundBlocks.test(index))
        {
            //We have a index that is a duplicate.
            // i.e. 7 -> 10 -> 29 -> 7 (Corrupt list. 7 is a duplicate)
            Assert(ASSERT_ALL, false, "We have a index that is a duplicate");
            return true;
        }
        ++cc;
    } // End while(true);

    if(foundBlocks.count() != m_uiBlocksAvailable)
    {
        // The linked list is corrupt.
        Assert(ASSERT_ALL, false, "The linked list is corrupt.\n");
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
template<class Allocator>
bool Chunk<Allocator>::IsBlockAvailable(void * pointer) const
{
    if(IsFull())
    {
        return false;
    }
    unsigned char *block = static_cast<unsigned char*>(pointer);

    //Alignment check.
    Assert(ASSERT_ALL, (((block - m_pData) * m_uiSize) == 0));

    //Calculate the index of block.
    unsigned char blockIndex = static_cast<unsigned char>((block - m_pData) / m_uiSize);

    unsigned char index = m_uiFirstAvailableBlock;
    Assert(ASSERT_ALL, m_uiTotalNumElements > index);

    if(index == blockIndex)
    {
        //The index of the parameter pointer is actually pointing to 
        //our first available block (m_uiFirstAvailableBlock).
        return true;
    }

    //We need to iterate over our linked list of available blocks
    std::bitset<UCHAR_MAX> foundBlocks;
    unsigned char *nextBlock = 0;
    unsigned char cc = 0;
    while(true)
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
            return true;
        }
        Assert(ASSERT_ALL, m_uiTotalNumElements > index, "Index is out of range.\n");
        Assert(ASSERT_ALL, !foundBlocks.test(index));
    } // while(true)

    return false;
}

