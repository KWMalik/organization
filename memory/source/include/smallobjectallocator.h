#ifndef _SMALL_OBJECT_ALLOCATOR_H_
#define _SMALL_OBJECT_ALLOCATOR_H_

#include "fixedallocator.h"

/// \class FixedAllocatorDescriptor
class FixedAllocatorDescriptor
{
public:
    unsigned int SingleAllocationSize() const;
    unsigned int NumberOfAllocations() const;

private:
    unsigned int singleAllocationSize;
    unsigned int numberOfAllocations;;
};

/// \class SmallObjectAllocator
/// \author Toby Banks
///
///
class SmallObjectAllocator
{
public:
	SmallObjectAllocator(unsigned int pageSize, unsigned int maxObjectSize, unsigned int objectAlignSize);

    SmallObjectAllocator(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors);



	~SmallObjectAllocator();

	/// \function Allocate
	/// \author Toby Banks
	///
	/// Allocates a block of memory of a requested size. It usually takes constant
	/// time, but might turn out to be O(C) whhere C is the number of Chunks 
	/// being managed by the FixedAllocator in the worst case.
	///
	/// \par Exception Safety Level
	/// Provides either strong-exception safety, or no-throw exception-safety
	/// level depending upon doThrow parameter.  The reason it provides two
	/// levels of exception safety is because it is used by both the nothrow
	/// and throwing new operators.  The underlying implementation will never
	/// throw of its own accord, but this can decide to throw if it does not
	/// allocate.  The only exception it should emit is std::bad_alloc.
	/// 
	/// \par Allocation Failure
	/// If it does not allocate, it will call TrimExcessMemory and attempt to
	/// allocate again, before it decides to throw or return NULL.  Many
	/// allocators loop through several new_handler functions, and terminate
	/// if they can not allocate, but not this one.  It only makes one attempt
	/// using its own implementation of the new_handler, and then returns NULL
	/// or throws so that the program can decide what to do at a higher level.
	/// (Side note: Even though the C++ Standard allows allocators and
	/// new_handlers to terminate if they fail, the Loki allocator does not do
	/// that since that policy is not polite to a host program.)
	/// 
	/// \param size # of bytes needed for allocation.
	/// \param doThrow True if this should throw if unable to allocate, false
	/// if it should provide no-throw exception safety level.
	/// \return A pointer to the memory block. If nothing is allocated and doThrow
	///         is false return NULL. the pointer to an available block of memory.
	///
	void * Allocate(unsigned int size, bool doThrow);

	/// \function Deallocate
	/// \author Toby Banks
	///
	/// Deallocates a block of memory at a given place and of a specific
	/// size.  Complexity is almost always constant-time, and is O(C) only if
	/// it has to search for which Chunk deallocates.  This never throws.
	///
	/// \param p Pointer to the allocation to be freed.
	/// \param size The size of the allocation
    /// \return True if if was able to deallocate the pointer.
	bool Deallocate( void * p, unsigned int size);

	/// \function Deallocate
	/// \author Toby Banks
	///
	/// Deallocates a block of memory at a given place but of unknown size
	/// size.  Complexity is O(F + C) where F is the count of FixedAllocator's
	/// in the pool, and C is the number of Chunks in all FixedAllocator's.  This
	/// does not throw exceptions.  This overloaded version of Deallocate is
	/// called by the nothow delete operator - which is called when the nothrow
	/// new operator is used, but a constructor throws an exception.
	///
	/// \param p Pointer to the allocation to be freed.
    /// \return True if if was able to deallocate the pointer.
	bool Deallocate( void * p );

	/// \return The max number of bytes which this can allocate.
	inline unsigned int GetMaxObjectSize() const
	{
		return m_uiMaxSmallObjectSize;
	}

	/// \return The number of bytes between allocation boundaries.
	inline unsigned int GetAlignment() const
	{ 
		return m_uiObjectAlignSize;
	}

	/// \function TrimExcessMemory
	/// \author Toby Banks
	///
	/// Releases empty Chunks from memory.  Complexity is O(F + C) where F
	/// is the count of FixedAllocator's in the pool, and C is the number of
	/// Chunks in all FixedAllocator's.  This will never throw.  This is called
	/// by AllocatorSingleto::ClearExtraMemory, the new_handler function for
	/// Loki's allocator, and is called internally when an allocation fails.
	///
	/// \return True if any memory released, or false if none released.
	bool TrimExcessMemory();

	/// \function IsCorrupt
	/// \author Toby Banks
	///
	/// Returns true if anything in implementation is corrupt.  Complexity
	/// is O(F + C + B) where F is the count of FixedAllocator's in the pool,
	/// C is the number of Chunks in all FixedAllocator's, and B is the number
	/// of blocks in all Chunks.  If it determines any data is corrupted, this
	/// will return true in release version, but assert in debug version at
	/// the line where it detects the corrupted data.  If it does not detect
	/// any corrupted data, it returns false.
	///
	/// \return True if corruption has been detected.
	bool IsCorrupt() const;


    /// \function PrintStats
    /// \author Toby Banks
    ///
    /// This simple little class just prints out information about our memory pools.
    ///
    void PrintStats() const;


private:

    //Just a few helper functions for our initialization code.
    unsigned int FindTotalSizeForTheFixedPool(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors);
    void InitializeTheFixedPools(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors, char * buffer);
    unsigned int DetermineMaxSmallObjectSize(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors);
    unsigned int DetermineObjectAlignSize(FixedAllocatorDescriptor * fad, int numberOfAllocatorDescriptors);
 
private:
	/// Default-constructor is not implemented.
	SmallObjectAllocator(void);
	/// Copy-constructor is not implemented.
	SmallObjectAllocator(const SmallObjectAllocator &);
	/// Copy-assignment operator is not implemented.
	SmallObjectAllocator & operator = (const SmallObjectAllocator &);

	/// Pointer to array of fixed-size allocators.
	GrowingFixedAllocator * m_pPool;

    //OPTINAL -- This needs ot go somewhere else
    char * buffer;

	/// Largest object size supported by allocators.
	const unsigned int m_uiMaxSmallObjectSize;

	/// Size of alignment boundaries.
	const unsigned int m_uiObjectAlignSize;
};

#endif // _SMALL_OBJECT_ALLOCATOR_H_

