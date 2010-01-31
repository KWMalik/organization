#ifndef _GROWING_POOL_H_
#define _GROWING_POOL_H_

////////////////TODO CREATE A BASE CLASS//////////////////////////////////////////

#include "allocators.h"
#include "growth_policies.h"

/// \class MemoryPool
///
template<class SizeT>
class MemoryPool
{
public:
	typedef SizeT size_type;

protected:
	void *first;	                    ///< The first element in the pool.
public:
	/// Default constructor that initializes the internal array to 0
	MemoryPool() : first(0) {}

	/// Destructor
	~MemoryPool() {}

	///	Checks to see if this class is empty. Meaning that first == 0.
	///
	/// \return True if first == 0; false otherwise.
	inline bool empty() const { return first == 0; }

	///	Adds a allocated block of memory for this MemoryPool class to 
	///	pool allocations from.
	///
	/// \param block The block of memory to pool.
	/// \param blockSize Size (bytes) of the memory allocated 
	///		   in block.
	/// \param allocSize Size (bytes) that block should be split into
	void add_block(void *block, size_type blockSize, size_type allocSize);

	///	After a block has been added allocate will return a open allocation.
	inline void * allocate();

	///	Deallocate
	///
	/// \param block The allocation to deallocate.
	inline void deallocate(void *block);

private:
	//Helper Function
	static inline void * & get_next_address(void * const pointer)
	{
		return *(static_cast<void **>(pointer));
	}

};

////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void MemoryPool<SizeT>::add_block(void *block, size_type blockSize, size_type allocSize)
{
	//The new end points at the old first (which is 0 the first time in)
	void *end = first;

	//We need to get the pointer to the last valid chunk to prevent overflow on size calculations.
	char *old = static_cast<char*>(block) + ((blockSize - allocSize) / allocSize) * allocSize;

	//Set the last element of the new block to be a pointer to the end old first.
	get_next_address(old) = end;

	if(old == block)
	{
		first = block;
		return;
	}

	for(char *iterator = old - allocSize; iterator != block; old = iterator, iterator -= allocSize)
	{
		get_next_address(iterator) = old;
	}

	get_next_address(block) = old;

	///Set first to point at this block
	first = block;
}

////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void * MemoryPool<SizeT>::allocate()
{
    if(first == NULL)
    {
		//We have no more free elements. If we were not to bail out
		//here we would crash dereferencing first on get_next_address(first)
        return NULL;
    }

	void * ret = first;

	//Increment first to point at the next free element.
	first = get_next_address(first);

	return ret;
}

////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void MemoryPool<SizeT>::deallocate(void *block)
{
	get_next_address(block) = first;
	first = block;
}

#endif // _GROWING_POOL_H_

