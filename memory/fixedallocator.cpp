#include "corepch.h"
#include "containers/algorithms/algorithms.h"
#include "memory/fixedallocator.h"
#include "debug/assert.h"

using namespace Axe;
using namespace Axe::Memory;
using namespace Axe::Debug;

UInt8 FixedAllocator::s_uiMinObjectsPerChunk = 8;
UInt8 FixedAllocator::s_uiMaxObjectsPerChunk = UCHAR_MAX;


///////////////////////////////////////////////////////////////////////////////////////////////
void FixedAllocator::Init(UInt blockSize, UInt chunkSize)
{
    Assert(ASSERT_TJB, blockSize > 0);
    Assert(ASSERT_TJB, chunkSize >= blockSize);

    m_uiBlockSize = blockSize;

    UInt numberOfBlocks = chunkSize / blockSize;
    if(numberOfBlocks > s_uiMaxObjectsPerChunk)
    {
        numberOfBlocks = s_uiMaxObjectsPerChunk;
    }
    else if(numberOfBlocks < s_uiMinObjectsPerChunk)
    {
        numberOfBlocks = s_uiMinObjectsPerChunk;
    }

    m_uiNumberOfBlocks = static_cast<UInt8>(numberOfBlocks);
    Assert(ASSERT_ALL, numberOfBlocks == m_uiNumberOfBlocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void * FixedAllocator::Allocate()
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
            for(Iterator it = m_pChunks.Begin(); ; ++it)
            {
                if(m_pChunks.End() == it)
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
Bool FixedAllocator::Deallocate(void *pointer, Chunk* hint)
{
    Assert(ASSERT_ALL, !m_pChunks.Empty());
    Assert(ASSERT_ALL, &m_pChunks.Front() <= m_pAllocChunk);
    Assert(ASSERT_ALL, &m_pChunks.Front() <= m_pDeallocChunk);
    Assert(ASSERT_ALL, &m_pChunks.Back() >= m_pAllocChunk);
    Assert(ASSERT_ALL, &m_pChunks.Back() >= m_pDeallocChunk);
    Assert(ASSERT_ALL, NumberOfEmptyChunks() < 2);

    Chunk * foundChunk = (hint == NULL) ? VicinityFind(pointer) : hint;

    if(foundChunk == NULL)
    {
        return False;
    }

    Assert(ASSERT_ALL, foundChunk->HasBlock(pointer));

#if MEMORY_EXTREME_TEST
    if(foundChunk->IsCorrupt())
    {
        Assert(ASSERT_ALL, False);
    }
    if(foundChunk->IsBlockAvailable(pointer))
    {
        Assert(ASSERT_ALL, False);
    }
#endif // MEMORY_EXTREME_TEST
    
    m_pDeallocChunk = foundChunk;
    DoDeallocate(pointer);
    Assert(ASSERT_TJB, NumberOfEmptyChunks() < 2);

    return True;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void FixedAllocator::DoDeallocate(void * pointer)
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
            Chunk *lastChunk = &(m_pChunks.Back());
            if(lastChunk == m_pDeallocChunk)
            {
                m_pDeallocChunk = m_pEmptyChunk;
            }
            else if(lastChunk != m_pEmptyChunk)
            {
                using namespace Axe::Containers::Algorithms;
                Swap(*m_pEmptyChunk, *lastChunk);
            }
            Assert(ASSERT_ALL, lastChunk->HasAvailable(m_uiNumberOfBlocks));
            lastChunk->Release();
            m_pChunks.PopBack();

            if(m_pAllocChunk == lastChunk || m_pAllocChunk->IsFull())
            {
                m_pAllocChunk = m_pDeallocChunk;
            }
        }
    }

    Assert(ASSERT_ALL, m_pEmptyChunk == NULL || m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks));
}

///////////////////////////////////////////////////////////////////////////////////////////////
UInt FixedAllocator::NumberOfEmptyChunks() const
{
    return (0 == m_pEmptyChunk) ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////
Bool FixedAllocator::IsCorrupt() const
{
    const Bool isEmpty = m_pChunks.Empty();
    ConstIterator begin = m_pChunks.Begin();
    ConstIterator end = m_pChunks.End();

    const UInt numberOfEmptyChunks = NumberOfEmptyChunks();

    if(isEmpty)
    {
        // Test that if we have a empty Chunks vector all of our values are zero'd out.
        if(begin != end)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }
        if(0 < numberOfEmptyChunks)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }
        if(NULL != m_pAllocChunk)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }
        if(NULL != m_pDeallocChunk)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }
        if(NULL != m_pEmptyChunk)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }
    }
    else
    {
        const Chunk * front = &(m_pChunks.Front());
        const Chunk * back = &(m_pChunks.Back());

        if(begin >= end)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }
        if(front > m_pDeallocChunk || back < m_pDeallocChunk)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }
        if(front > m_pAllocChunk || back < m_pAllocChunk)
        {
            Assert(ASSERT_ALL, False);
            return True;
        }

        if(numberOfEmptyChunks == 0)
        {
            if(m_pEmptyChunk != NULL)
            {
                Assert(ASSERT_ALL, False);
                return True;
            }
        }
        else if(numberOfEmptyChunks == 1)
        {
            if(m_pEmptyChunk == NULL)
            {
                Assert(ASSERT_ALL, False);
                return True;
            }
            if(front > m_pEmptyChunk || back < m_pEmptyChunk)
            {
                Assert(ASSERT_ALL, False);
                return True;
            }
            if(!m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks))
            {
                Assert(ASSERT_ALL, False, "This may imply that a block was deleted twice.\n");
                return True;
            }
        }
        else
        {
            Assert(ASSERT_ALL, False);
            return True;
        }

        for(ConstIterator it = begin; it != end; ++it)
        {
            if((*it).IsCorrupt())
            {
                return True;
            }
        }
    }
    return False;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Bool FixedAllocator::MakeNewChunk()
{
    Bool allocated = False;
    try
    {
        UInt size = m_pChunks.Size();

        if(m_pChunks.Capacity() == size)
        {
            if(size == 0)
            {
                size = 4;
            }
            //Size double each time.. 
            //Should I uses a function pointer to determine the growth rate?
            m_pChunks.Reserve(size * 2);
        }

        Chunk newChunk;
        allocated = newChunk.Init(m_uiBlockSize, m_uiNumberOfBlocks);
        if(allocated)
        {
            m_pChunks.PushBack(newChunk);
        }
    }
    catch (...)
    {
        allocated = False;
    }

    if(!allocated)
    {
        return False;
    }

    m_pAllocChunk = &(m_pChunks.Back());
    m_pDeallocChunk = &(m_pChunks.Front());
    return True;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Chunk *FixedAllocator::VicinityFind(void * pointer) const
{
    if(m_pChunks.Empty())
    {
        return NULL;
    }
    Assert(ASSERT_ALL, m_pDeallocChunk);

    Chunk *lo = m_pDeallocChunk;
    Chunk *hi = m_pDeallocChunk + 1;

    const Chunk * loBound = &(m_pChunks.Front());
    const Chunk * hiBound = &(m_pChunks.Back()) + 1;

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
Bool FixedAllocator::TrimEmptyChunk()
{
    Assert(ASSERT_ALL, m_pEmptyChunk == NULL || m_pEmptyChunk->HasAvailable(m_uiNumberOfBlocks));
    
    if(m_pEmptyChunk == NULL)
    {
        return False;
    }

    Assert(ASSERT_ALL, !m_pChunks.Empty());
    Assert(ASSERT_ALL, NumberOfEmptyChunks() == 1);

    Chunk * lastChunk = &(m_pChunks.Back());
    if(lastChunk != m_pEmptyChunk)
    {
        using namespace Axe::Containers::Algorithms;
        Swap(*m_pEmptyChunk, *lastChunk);
    }

    Assert(ASSERT_ALL, lastChunk->HasAvailable(m_uiNumberOfBlocks));
    lastChunk->Release();
    m_pChunks.PopBack();

    if(m_pChunks.Empty())
    {
        m_pAllocChunk = NULL;
        m_pDeallocChunk = NULL;
    }
    else
    {
        if(m_pDeallocChunk == m_pEmptyChunk)
        {
            m_pDeallocChunk = &(m_pChunks.Front());
            Assert(ASSERT_ALL, m_pDeallocChunk->GetNumberOfBlocksAvailable() < m_uiNumberOfBlocks);
        }
        if(m_pAllocChunk == m_pEmptyChunk)
        {
            m_pAllocChunk = &(m_pChunks.Back());
            Assert(ASSERT_ALL, m_pAllocChunk->GetNumberOfBlocksAvailable() < m_uiNumberOfBlocks);
        }
    }
    m_pEmptyChunk = NULL;
    Assert(ASSERT_ALL, NumberOfEmptyChunks() == 0);

    return True;
}


///////////////////////////////////////////////////////////////////////////////////////////////
Bool FixedAllocator::TrimChunkContainer()
{
    if(m_pChunks.Empty())
    {
        Assert(ASSERT_ALL, m_pAllocChunk == NULL);
        Assert(ASSERT_ALL, m_pDeallocChunk == NULL);
    }
    if(m_pChunks.Size() == m_pChunks.Capacity())
    {
        return False;
    }

    for(Iterator it = m_pChunks.Begin(); it != m_pChunks.End(); ++it)
    {
        if(it->GetNumberOfBlocksAvailable() == it->GetTotalNumElements())
        {
            //This is a free Chunk.
            m_pChunks.Erase(it);
            it->Release();
            break;
        }
    }

    for(Iterator it = m_pChunks.Begin(); it != m_pChunks.End(); ++it)
    {
        std::cout << "Hello World!!" << std::endl;
    }

    return False;
}

