#ifndef _STAGNANT_FIXED_ALLOCATOR_H_
#define _STAGNANT_FIXED_ALLOCATOR_H_

#include <vector>

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
    /// Zero's out all non-static members.
    StagnantFixedAllocator() : m_uiBlockSize(0), m_uiNumberOfBlocks(0), m_pAllocChunk(0), 
                               m_pDeallocChunk(0), m_pEmptyChunk(0) {}

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
    void Init(unsigned int singleAllocationSize, unsigned int numberOfAllocations, char *buffer);

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
    bool Deallocate(void *pointer, Chunk* hint = NULL);

    /// \function BlockSize
    /// \author Toby Banks
    ///
    /// \brief Returns the size this StagnantFixedAllocator was initialized with.
    /// \return The size this StagnantFixedAllocator was initialized with.
    inline unsigned int BlockSize() const { return m_uiBlockSize; }

    /// \function IsCorrupt
    /// \author Toby Banks
    ///
    /// Checks to see if any of the chunks in the chunk container are 
    /// corrupted. Also checks to see if the chunklist itself is corrupted. 
    ///
    /// \brief Checks to see if we have any corruption in the chunk container.
    /// \return True if the chunklist is corrupt.
    bool IsCorrupt() const;

    /// \function PrintStats
    /// \author Toby Banks
    ///
    ///
    void PrintStats() const;

//Helper functions
private:
    /// \function DoDeallocate
    /// \author Toby Banks
    ///
    /// Performs the actual deallocation. This is called inside of Deallocate.
    ///
    /// \pre Assumes that m_pDeallocChunk pointer to the correct chunk.
    /// \brief Performs the actual deallocation.
    /// \param pointer The memory to deallocate.
    void DoDeallocate(void * pointer);

    /// \function VicinityFind
    /// \author Toby Banks
    ///
    /// \brief Returns a chunk that contains pointer or NULL.
    /// \param pointer The address to search for.
    /// \return A pointer to the chunk that container pointer; or NULL if it's not found.
    Chunk *VicinityFind(void * pointer) const;


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
    Chunks *m_pChunks;
};

#endif // _STAGNANT_FIXED_ALLOCATOR_H_
