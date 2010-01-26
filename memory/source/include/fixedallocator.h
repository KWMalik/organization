#ifndef _FIXED_ALLOCATOR_H_
#define _FIXED_ALLOCATOR_H_

#include <vector>

#include "chunk.h"

/// \class FixedAllocator
/// \author Toby Banks
/// 
/// A FixedAllocator is responsible for allocating objects of a given size. 
/// A FixedAllocator manages 1 or more Chunks of a object of a given size.
/// 
class FixedAllocator
{
public:
    /// Zero's out all non-static members.
    FixedAllocator() : m_uiBlockSize(0), m_uiNumberOfBlocks(0), m_pAllocChunk(0), 
                       m_pDeallocChunk(0), m_pEmptyChunk(0) {}


    /// Releases all of the Chunks owned by this class.
    ~FixedAllocator()
    {
        for(iterator it = m_pChunks.begin(); it != m_pChunks.end(); ++it)
        {
            it->Release();
        }
    }

    /// \function Init
    /// \author Toby Banks
    ///
    /// Initializes a FixedAllocator by calculating the number of blocks per chunk.
    ///
    /// \brief Causes this FixedAllocator to create blocks that are blockSize by chunkSize.
    /// \param blockSize The size of a single element. 
    /// \param chunkSize The number of elements of blocksize that a single chunk contains.
    void Init(unsigned int blockSize, unsigned int chunkSize);

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
    /// Frees a block of memory. If this FixedAllocator doesn't own the block
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
    /// \brief Returns the size this FixedAllocator was initialized with.
    /// \return The size this FixedAllocator was initialized with.
    inline unsigned int BlockSize() const { return m_uiBlockSize; }

    /// \function FreeEmptyChunk
    /// \author Toby Banks
    /// 
    /// \brief If there is a empty chunk it will be freed. 
    /// \return True if a empty chunk was found and freed; false otherwise.
    bool TrimEmptyChunk();

    /// \function FreeChunkContainer
    /// \author Toby Banks
    ///
    /// Used to free any unused spots in the chunk container. Takes constant time
    /// with respect to the number of chunks. 
    ///
    /// \brief Frees unused spots in the chunk container.
    /// \return True if some chunks were found and released; false otherwise.
    bool TrimChunkContainer();

    /// \function NumberOfEmptyChunks
    /// \author Toby Banks
    ///
    /// \brief Used to return the number of empty chunks.
    /// \return The number of empty chunks.
    unsigned int NumberOfEmptyChunks() const;

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
    /// \brief Checks to see if address pointer is owned by this FixedAllocator.
    /// \param pointer The address to check to see if this FixedAllocator owns it.
    /// \return A pointer to the Chunk that contains pointer; or NULL.
    inline const Chunk * HasBlock(void * pointer) const;
    inline Chunk * HasBlock(void * pointer);

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

    /// \function MakeNewChunk
    /// \author Toby Banks
    ///
    /// Adds a new chunk to the Chunks container.
    ///
    /// \return True if successful; false otherwise.
    bool MakeNewChunk();

    /// \function VicinityFind
    /// \author Toby Banks
    ///
    /// \brief Returns a chunk that contains pointer or NULL.
    /// \param pointer The address to search for.
    /// \return A pointer to the chunk that container pointer; or NULL if it's not found.
    Chunk *VicinityFind(void * pointer) const;


private:
    /// Not implemented.
    FixedAllocator(const FixedAllocator&);

    /// Not implemented.
    FixedAllocator& operator=(const FixedAllocator&);

    /// \typedef Chunks
    /// \brief Easy access to our Chunks container.
    typedef std::vector<Chunk> Chunks;

    /// \typedef iterator
    /// \brief Easy access to a iterator for our Chunks container.
    typedef Chunks::iterator iterator;

    /// \typedef const_iterator
    /// \brief Easy access to a const iterator for our Chunks container.
    typedef Chunks::const_iterator const_iterator;

    /// Least number of objects that a chunk can contain.
    static unsigned char s_uiMinObjectsPerChunk;

    /// Most number of objects that a chunk can contain (can't exceed UCHAR_MAX). 
    static unsigned char s_uiMaxObjectsPerChunk;

    /// The size of allocations that our chunks contain.
    unsigned int m_uiBlockSize;

    /// The number of blocks managed by each chunk.
    unsigned char m_uiNumberOfBlocks;

    /// Container of chunks.
    Chunks m_pChunks;

    /// Pointer to the Chunk that was used for the last or next allocation.
    Chunk * m_pAllocChunk;

    /// Pointer to the Chunk that was used for the last or next deallocation.
    Chunk * m_pDeallocChunk;

    /// Pointer to the only empty chunk if there is one; else NULL.
    Chunk * m_pEmptyChunk;

};

///////////////////////////////////////////////////////////////////////////////////////////////
inline const Chunk * FixedAllocator::HasBlock(void * pointer) const
{
    for (const_iterator it = m_pChunks.begin(); it != m_pChunks.end(); ++it)
    {
        const Chunk * chunk = &(*it);
        if(chunk->HasBlock(pointer))
        {
            return chunk;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
inline Chunk * FixedAllocator::HasBlock(void * pointer)
{
    return const_cast<Chunk *>(const_cast<const FixedAllocator *>(this)->HasBlock(pointer));
}

#endif // _FIXED_ALLOCATOR_H_
