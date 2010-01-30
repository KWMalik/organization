#ifndef _STAGNANT_FIXED_ALLOCATOR_H_
#define _STAGNANT_FIXED_ALLOCATOR_H_

#include "chunk.h"

/// \class StagnantFixedAllocator
/// \author Toby Banks
/// 
/// A StagnantFixedAllocator is responsible for allocating objects of a given size. 
/// A StagnantFixedAllocator manages 1 or more Chunks of a object of a given size.
/// 
class StagnantFixedAllocator
{
public:
    typedef Chunk<NullChunkAllocator> ChunkT;

    /// Zero's out all non-static members.
    StagnantFixedAllocator() : m_uiBlockSize(0), m_uiNumberOfBlocks(0), m_Chunk() {}

    /// Doesn't need to do anything. The class that creates this one provides the memory to use.
    ~StagnantFixedAllocator() {}

    /// \function Init
    /// \author Toby Banks
    ///
    /// Initializes a StagnantFixedAllocator by calculating the number of blocks per chunk.
    ///
    /// \brief Causes this StagnantFixedAllocator to create blocks that are blockSize by chunkSize.
    /// \param blockSize The size of a single element. 
    /// \param chunkSize The number of elements of blocksize that a single chunk contains.
    /// \param buffer The buffer to use for the allocation (assumes correct size)
    void Init(unsigned int singleAllocationSize, unsigned int numberOfAllocations, unsigned char *buffer);

    /// \function Allocate
    /// \author Toby Banks
    ///
    /// Returns a pointer to a memory block of fixed size. Or NULL if it fails.
    ///
    /// \brief Returns a pointer to a block of memory; or NULL if it can't.
    /// \return A memory block of a fixed size.
    void * Allocate();

    /// \function Deallocate
    /// \author Toby Banks
    ///
    /// Frees a block of memory. If this StagnantFixedAllocator doesn't own the block
    /// it will return false. 
    ///
    /// \brief Frees a block of memory.
    /// \param pointer The pointer to free.
    /// \param hint The first block to look in.
    /// \return True if the block was found and freed; false otherwise.
    bool Deallocate(void *pointer, ChunkT * hint = 0);

    /// \function BlockSize
    /// \author Toby Banks
    ///
    /// \brief Returns the size this StagnantFixedAllocator was initialized with.
    /// \return The size this StagnantFixedAllocator was initialized with.
    inline unsigned int BlockSize() const { return m_uiBlockSize; }

    /// \function FreeEmptyChunk
    /// \author Toby Banks
    /// 
    /// \brief There will never be a empty chunk. This pool cannot grow.
    /// \return False, always.
    bool TrimEmptyChunk() { return false; }

    /// \function FreeChunkContainer
    /// \author Toby Banks
    ///
    /// \brief Does nothing. Just a interface for generic client code.
    /// \return False, always.
    bool TrimChunkContainer() { return false; }

    /// \function NumberOfEmptyChunks
    /// \author Toby Banks
    ///
    /// \brief Just a interface to simplify generic code.
    /// \return 0 Always.
    unsigned int NumberOfEmptyChunks() const { return 0; }

    /// \function IsCorrupt
    /// \author Toby Banks
    ///
    /// Checks to see if any of the chunks in the chunk container are 
    /// corrupted. Also checks to see if the chunklist itself is corrupted. 
    ///
    /// \brief Checks to see if we have any corruption in the chunk container.
    /// \return True if the chunklist is corrupt.
    bool IsCorrupt() const;

    /// \function HasBlock
    /// \author Toby Banks
    /// 
    /// \brief Checks to see if address pointer is owned by this GrowingFixedAllocator.
    /// \param pointer The address to check to see if this GrowingFixedAllocator owns it.
    /// \return A pointer to the Chunk that contains pointer; or NULL.
    inline const ChunkT * HasBlock(void * pointer) const;
    inline ChunkT * HasBlock(void * pointer);

    /// \function PrintStats
    /// \author Toby Banks
    ///
    ///
    void PrintStats() const;

private:
    /// Not implemented.
    StagnantFixedAllocator(const StagnantFixedAllocator&);

    /// Not implemented.
    StagnantFixedAllocator& operator=(const StagnantFixedAllocator&);

    /// The size of allocations that our chunks contain.
    unsigned int m_uiBlockSize;

    /// The number of blocks managed by each chunk.
    unsigned char m_uiNumberOfBlocks;

    /// Container of chunks.
    ChunkT m_Chunk;
};

///////////////////////////////////////////////////////////////////////////////////////////////
inline const StagnantFixedAllocator::ChunkT * StagnantFixedAllocator::HasBlock(void * pointer) const
{
    if(m_Chunk.HasBlock(pointer))
    {
        return &m_Chunk;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
inline StagnantFixedAllocator::ChunkT * StagnantFixedAllocator::HasBlock(void * pointer)
{
    return const_cast<ChunkT *>(const_cast<const StagnantFixedAllocator *>(this)->HasBlock(pointer));
}


#endif // _STAGNANT_FIXED_ALLOCATOR_H_
