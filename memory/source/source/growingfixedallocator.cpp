#include <algorithm>
#include <iostream>

#include "fixedallocator.h"

#define Assert(...) 

///////////////////////////////////////////////////////////////////////////////////////////////
void GrowingFixedAllocator::Init(unsigned int blockSize, unsigned int chunkSize)
{
    Assert(ASSERT_TJB, blockSize > 0);
    Assert(ASSERT_TJB, chunkSize >= blockSize);

    m_uiBlockSize = blockSize;

    unsigned int numberOfBlocks = chunkSize / blockSize;
    if(numberOfBlocks > m_uiMaxObjectsPerChunk)
    {
        numberOfBlocks = m_uiMaxObjectsPerChunk;
    }
    else if(numberOfBlocks < m_uiMinObjectsPerChunk)
    {
        numberOfBlocks = m_uiMinObjectsPerChunk;
    }

    m_uiNumberOfBlocks = static_cast<unsigned char>(numberOfBlocks);
    Assert(ASSERT_ALL, numberOfBlocks == m_uiNumberOfBlocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void * GrowingFixedAllocator::Allocate()
{
    Assert(ASSERT_ALL, (NULL == m_pEmptyChunk) || (m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks)));
    Assert(ASSERT_ALL, NumberOfEmptyChunks() < 2);

    if((m_pAllocChunk == NULL) || m_pAllocChunk->IsFull())
    {
        //We need a new alloc chunk to allocate into

        if(m_pEmptyChunk != NULL)
        {
            m_pAllocChunk = m_pEmptyChunk;
            m_pEmptyChunk = NULL;
        }
        else
        {
            for(iterator it = m_pChunks.begin(); ; ++it)
            {
                if(m_pChunks.end() == it)
                {
                    //We are out of chunks; add a new chunk.
                    if(!MakeNewChunk())
                    {
                        // We failed adding a new chunk
                        return NULL;
                    }
                    break;
                }
                if(!it->IsFull())
                {
                    //We found a chunk with a free block in it.
                    m_pAllocChunk = &(*it);
                    break;
                }
            }
        }
    }
    else if(m_pAllocChunk == m_pEmptyChunk)
    {
        //We need to set the empty chunk to NULL since the chunk will no longer 
        //be empty.
        m_pEmptyChunk = NULL;
    }

    Assert(ASSERT_ALL, m_pAllocChunk != NULL);
    Assert(ASSERT_ALL, !m_pAllocChunk->IsFull());
    void *pReturn = m_pAllocChunk->Allocate();
    Assert(ASSERT_ALL, (m_pEmptyChunk == NULL) || (m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks)));
    Assert(ASSERT_ALL, NumberOfEmptyChunks() < 2);

    return pReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool GrowingFixedAllocator::Deallocate(void *pointer, Chunk* hint)
{
    Assert(ASSERT_ALL, !m_pChunks.empty());
    Assert(ASSERT_ALL, &m_pChunks.front() <= m_pAllocChunk);
    Assert(ASSERT_ALL, &m_pChunks.front() <= m_pDeallocChunk);
    Assert(ASSERT_ALL, &m_pChunks.back() >= m_pAllocChunk);
    Assert(ASSERT_ALL, &m_pChunks.back() >= m_pDeallocChunk);
    Assert(ASSERT_ALL, NumberOfEmptyChunks() < 2);

    Chunk * foundChunk = (hint == NULL) ? VicinityFind(pointer) : hint;

    if(foundChunk == NULL)
    {
        return false;
    }

    Assert(ASSERT_ALL, foundChunk->HasBlock(pointer));

#if MEMORY_EXTREME_TEST
    if(foundChunk->IsCorrupt())
    {
        Assert(ASSERT_ALL, false);
    }
    if(foundChunk->IsBlockAvailable(pointer))
    {
        Assert(ASSERT_ALL, false);
    }
#endif // MEMORY_EXTREME_TEST
    
    m_pDeallocChunk = foundChunk;
    DoDeallocate(pointer);
    Assert(ASSERT_TJB, NumberOfEmptyChunks() < 2);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void GrowingFixedAllocator::DoDeallocate(void * pointer)
{
    Assert(ASSERT_ALL, m_pDeallocChunk->HasBlock(pointer));
    Assert(ASSERT_ALL, m_pEmptyChunk != m_pDeallocChunk);
    Assert(ASSERT_ALL, !m_pDeallocChunk->HasAvailable(m_uiNumberOfBlocks));
    Assert(ASSERT_ALL, m_pEmptyChunk == NULL);

    m_pDeallocChunk->Deallocate(pointer);

    if(m_pDeallocChunk->HasAvailable(m_uiNumberOfBlocks))
    {
        Assert(ASSERT_ALL, m_pEmptyChunk != m_pDeallocChunk);

        if(m_pEmptyChunk != NULL)
        {
            //If we have a valid empty chunk and we are about to create 
            //a second empty chunk we should free
            Chunk *lastChunk = &(m_pChunks.back());
            if(lastChunk == m_pDeallocChunk)
            {
                m_pDeallocChunk = m_pEmptyChunk;
            }
            else if(lastChunk != m_pEmptyChunk)
            {
				std::swap(*m_pEmptyChunk, *lastChunk);
            }
            Assert(ASSERT_ALL, lastChunk->HasAvailable(m_uiNumberOfBlocks));
            lastChunk->Release();
            m_pChunks.pop_back();

            if(m_pAllocChunk == lastChunk || m_pAllocChunk->IsFull())
            {
                m_pAllocChunk = m_pDeallocChunk;
            }
        }
    }

    Assert(ASSERT_ALL, m_pEmptyChunk == NULL || m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks));
}

///////////////////////////////////////////////////////////////////////////////////////////////
unsigned int GrowingFixedAllocator::NumberOfEmptyChunks() const
{
    return (0 == m_pEmptyChunk) ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool GrowingFixedAllocator::IsCorrupt() const
{
    const bool isEmpty = m_pChunks.empty();
    const_iterator begin = m_pChunks.begin();
    const_iterator end = m_pChunks.end();

    const unsigned int numberOfEmptyChunks = NumberOfEmptyChunks();

    if(isEmpty)
    {
        // Test that if we have a empty Chunks vector all of our values are zero'd out.
        if(begin != end)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }
        if(0 < numberOfEmptyChunks)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }
        if(NULL != m_pAllocChunk)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }
        if(NULL != m_pDeallocChunk)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }
        if(NULL != m_pEmptyChunk)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }
    }
    else
    {
        const Chunk * front = &(m_pChunks.front());
        const Chunk * back = &(m_pChunks.back());

        if(begin >= end)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }
        if(front > m_pDeallocChunk || back < m_pDeallocChunk)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }
        if(front > m_pAllocChunk || back < m_pAllocChunk)
        {
            Assert(ASSERT_ALL, false);
            return true;
        }

        if(numberOfEmptyChunks == 0)
        {
            if(m_pEmptyChunk != NULL)
            {
                Assert(ASSERT_ALL, false);
                return true;
            }
        }
        else if(numberOfEmptyChunks == 1)
        {
            if(m_pEmptyChunk == NULL)
            {
                Assert(ASSERT_ALL, false);
                return true;
            }
            if(front > m_pEmptyChunk || back < m_pEmptyChunk)
            {
                Assert(ASSERT_ALL, false);
                return true;
            }
            if(!m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks))
            {
                Assert(ASSERT_ALL, false, "This may imply that a block was deleted twice.\n");
                return true;
            }
        }
        else
        {
            Assert(ASSERT_ALL, false);
            return true;
        }

        for(const_iterator it = begin; it != end; ++it)
        {
            if((*it).IsCorrupt())
            {
                return true;
            }
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool GrowingFixedAllocator::MakeNewChunk()
{
    bool allocated = false;
    try
    {
        unsigned int size = m_pChunks.size();

        if(m_pChunks.capacity() == size)
        {
            if(size == 0)
            {
                size = 4;
            }
            //Size double each time.. 
            //Should I uses a function pointer to determine the growth rate?
            m_pChunks.reserve(size * 2);
        }

        Chunk newChunk;
        allocated = newChunk.Init(m_uiBlockSize, m_uiNumberOfBlocks);
        if(allocated)
        {
            m_pChunks.push_back(newChunk);
        }
    }
    catch (...)
    {
        allocated = false;
    }

    if(!allocated)
    {
        return false;
    }

    m_pAllocChunk = &(m_pChunks.back());
    m_pDeallocChunk = &(m_pChunks.front());
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Chunk *GrowingFixedAllocator::VicinityFind(void * pointer) const
{
    if(m_pChunks.empty())
    {
        return NULL;
    }
    Assert(ASSERT_ALL, m_pDeallocChunk);

    Chunk *lo = m_pDeallocChunk;
    Chunk *hi = m_pDeallocChunk + 1;

    const Chunk * loBound = &(m_pChunks.front());
    const Chunk * hiBound = &(m_pChunks.back()) + 1;

    //Handles a special case where m_pDeallocChunk is the last in the array.
    if(hi == hiBound)
    {
        hi = NULL;
    }

    while(true)
    {
        if(hi == NULL && lo == NULL)
        {
            break;
        }
        if(lo)
        {
            if(lo->HasBlock(pointer))
            {
                return lo;
            }
            else if(lo == loBound)
            {
                lo = NULL;
            }
            else
            {
                --lo;
            }
        }
        if(hi)
        {
            if(hi->HasBlock(pointer))
            {
                return hi;
            }
            else if(hi == hiBound)
            {
                hi = NULL;
            }
            else
            {
                ++hi;
            }
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool GrowingFixedAllocator::TrimEmptyChunk()
{
    Assert(ASSERT_ALL, m_pEmptyChunk == NULL || m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks));
    
    if(m_pEmptyChunk == NULL)
    {
        return false;
    }

    Assert(ASSERT_ALL, !m_pChunks.empty());
    Assert(ASSERT_ALL, NumberOfEmptyChunks() == 1);

    Chunk * lastChunk = &(m_pChunks.back());
    if(lastChunk != m_pEmptyChunk)
    {
		std::swap(*m_pEmptyChunk, *lastChunk);
    }

    Assert(ASSERT_ALL, lastChunk->HasAvailable(m_uiNumberOfBlocks));
    lastChunk->Release();
    m_pChunks.pop_back();

    if(m_pChunks.empty())
    {
        m_pAllocChunk = NULL;
        m_pDeallocChunk = NULL;
    }
    else
    {
        if(m_pDeallocChunk == m_pEmptyChunk)
        {
            m_pDeallocChunk = &(m_pChunks.front());
            Assert(ASSERT_ALL, m_pDeallocChunk->GetNumberOfBlocksAvailable() < m_uiNumberOfBlocks);
        }
        if(m_pAllocChunk == m_pEmptyChunk)
        {
            m_pAllocChunk = &(m_pChunks.back());
            Assert(ASSERT_ALL, m_pAllocChunk->GetNumberOfBlocksAvailable() < m_uiNumberOfBlocks);
        }
    }
    m_pEmptyChunk = NULL;
    Assert(ASSERT_ALL, NumberOfEmptyChunks() == 0);

    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////
bool GrowingFixedAllocator::TrimChunkContainer()
{
    if(m_pChunks.empty())
    {
        Assert(ASSERT_ALL, m_pAllocChunk == NULL);
        Assert(ASSERT_ALL, m_pDeallocChunk == NULL);
    }
    if(m_pChunks.size() == m_pChunks.capacity())
    {
        return false;
    }

    for(iterator it = m_pChunks.begin(); it != m_pChunks.end(); ++it)
    {
        if(it->GetNumberOfBlocksAvailable() == it->GetTotalNumElements())
        {
            //This is a free Chunk.
            m_pChunks.erase(it);
            it->Release();
            break;
        }
    }

    for(iterator it = m_pChunks.begin(); it != m_pChunks.end(); ++it)
    {
        std::cout << "Hello World!!" << std::endl;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void GrowingFixedAllocator::PrintStats() const
{
    std::cout   << "\tMin Obj Per Chunk: " << m_uiMinObjectsPerChunk << std::endl
                << "\tMax Obj Per Chunk: " << m_uiMaxObjectsPerChunk << std::endl
                << "\tBlock Size: " << m_uiBlockSize << std::endl
                << "\tNumber of Blocks " << m_uiNumberOfBlocks << std::endl;
}

