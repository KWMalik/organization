#ifndef _AXE_MEMORY_CHUNK_H_
#define _AXE_MEMORY_CHUNK_H_

#include "config.h"

namespace Axe
{
namespace Memory
{
    /// \class Chunk
    /// \author Toby Banks
    ///
    /// Represents a fixed size block of memory. This class isn't usually 
    /// used directly. It's managed by the FixedAllocator. For example 
	/// this class might represent 100 24-bytes blocks. 
	///
	/// \par Optimizations
	/// No constructors, destructors or copy-assignment operator is defined. 
	/// Init and Release are used instead of constructors and destructors 
	/// which allows the compiler to optimize copying instances of this 
	/// class around.
	///
	/// Everything in this class is private. It's only friend is the
	/// FixedAllocator, this is to ensure that NO ONE ever hacks this 
	/// with nonsense.
    /// 
    class Chunk
    {
    private:
	//public: // TEMPORARY HACK FOR TESTING CHUNK SEE MEMORYTEST1

		/// Only the FixedPageAllocator is allowed to interact with this class.
		friend class FixedAllocator;

        /// \function Init
		/// \author Toby Banks
		///
        /// Initializes this Block to contain a number of blocks (numblocks) 
        /// of size size.
		///
        /// \brief Creates a block of numberOfElements of size.
        /// \param size The size of data to store in this Block (in bytes).
        /// \param numberOfElements The number of elements of size to store.
        Bool Init(const UInt size, const UInt8 numberOfElements);

        /// \function Allocate
		/// \author Toby Banks
		///
        /// Returns a pointer to a element stored in this block. Absolutely
        /// no error checking is performed by this function. Neither new,
		/// malloc or any other heap memory allocation function is called. 
		/// The only thing this function does is adjust some indices and
		/// return a pointer to the already allocated block of memory.
		///
		/// Complexity is always O(1).
		///
        /// \brief Provides a allocation from this pool.
        /// \return A pointer to a valid element in this block.
        void * Allocate();

        /// \function Deallocate
		/// \author Toby Banks
		///
        /// Used to deallocate a single element in this chunk. This does
		/// not deallocate using delete, free or any other heap mechanism,
		/// instead it just adjusts some internal indices.
		///
		/// Complexity is always O(1).
		///
        /// \brief Returns the pointer to the pool we are managing.
        /// \param pointer The address we wish to free.
        void Deallocate(void *pointer);

        /// \function Reset
		/// \author Toby Banks
		///
        /// Resets this block back to zero values. The index is set
        /// back to the first block and all values are cleared.
		///
        /// \brief Resets the indexes in this block. Does not free memory.
        void Reset();

        /// \function Release
		/// \author Toby Banks
		///
        /// Frees this block of memory.
		///
        /// \brief Clears the indexes, and frees memory.
        void Release();

        /// \function IsBlockAvailable
		/// \author Toby Banks
		///
		/// Used to check the availablity of a block in this Chunk.
        ///
        /// \brief Checks to see if this Block has been corrupted. 
        /// \param pointer The address to check to if it's available.
        /// \return True if the block at pointer is available.
        Bool IsBlockAvailable(void * pointer) const;

        /// \function IsCorrupt
        ///
		/// Checks to see if there is any corruption in this chunk.
		///
        /// \brief Checks to see if this block is corrupted.
        /// \return True if this block is corrupted.
        Bool IsCorrupt() const;

        /// \function IsFull
		/// \author Toby Banks
		///
        /// Is this Block full?
		///
        /// \brief Checks to see if this block can hold any more elements.
        /// \return True if this Block has no more elements.
        inline Bool IsFull() const { return m_uiBlocksAvailable == 0; }

        /// \function HasBlock
		/// \author Toby Banks
		///
        /// Checks to see if this Chunk container the address pointer.
		///
        /// \brief Checks if pointer exists in this chunk.
        /// \param pointer The address to check to see if this chunk contains.
        /// \return True if pointer is in this Chunk.
        inline Bool HasBlock(void * pointer) const
        {
            const UInt8 * pc = static_cast<const UInt8 *>(pointer);
            return (m_pData <= pc) && (pc < (m_pData + (m_uiTotalNumElements * m_uiSize)));
        }

        /// \function HasAvailable
		/// \author Toby Banks
		///
        /// \brief Returns true if this chunk contains numberOfBlocks of free blocks.
        /// \param numberOfBlocks Number of blocks to see if this Chunk has available.
        /// \return True if numberOfBlocks equals the blocks this instance has available.
        inline Bool HasAvailable(UInt8 numberOfBlocks) const
        {
            return m_uiBlocksAvailable == numberOfBlocks;
        }

        /// \function GetNumberOfBlocksAvailable
		/// \author Toby Banks
		///
        /// \brief Returns the number of free blocks this chunk contains.
        /// \return The number of free blocks this chunk contains.
        inline UInt8 GetNumberOfBlocksAvailable() const
        {
            return m_uiBlocksAvailable;
        }

        /// \function GetTotalNumElements
		/// \author Toby Banks
		///
        /// \brief Returns the max number of blocks this chunk can contain.
        /// \return The max number of blocks this chunk can store.
        inline UInt8 GetTotalNumElements() const
        {
            return m_uiTotalNumElements;
        }

    protected:
        UInt8 *m_pData;                  ///< Our array of allocated elements.
        UInt8  m_uiFirstAvailableBlock;  ///< Index of first available element.
        UInt8  m_uiBlocksAvailable;      ///< Number of elements available.
        UInt8  m_uiTotalNumElements;     ///< The number of elements we store. 
        UInt   m_uiSize;                 ///< The size in bytes of each element.
    };

} // namespace Memory
} // namespace Axe
        
#endif // _AXE_MEMORY_CHUNK_H_
