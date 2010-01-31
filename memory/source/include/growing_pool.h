#ifndef _GROWING_POOL_H_
#define _GROWING_POOL_H_

////////////////TODO CREATE A BASE CLASS//////////////////////////////////////////

#include "allocators.h"
#include "growth_policies.h"

/// \class GrowingPool
///
template<class SizeT, class Allocator, template <typename, typename> class GrowthPolicy>
class GrowingPool : public GrowthPolicy<SizeT, Allocator>
{
public:
	typedef SizeT size_type;

protected:
	void *first;	                    ///< The first element in the pool.
public:
	/// Default constructor that initializes the internal array to 0
	GrowingPool() : first(0) {}

	/// Destructor
	~GrowingPool() {}

	///	Checks to see if this class is empty. Meaning that first == 0.
	///
	/// \return True if first == 0; false otherwise.
	inline bool empty() const { return first == 0; }

	///	Adds a allocated block of memory for this GrowingPool class to 
	///	pool allocations from.
	///
	/// \param block The block of memory to pool.
	/// \param blockSize Size (bytes) of the memory allocated 
	///		   in block.
	/// \param allocSize Size (bytes) that block should be split into
	void add_block(void *block, size_type blockSize, size_type allocSize);

	///	After a block has been added malloc will return a open allocation.
	inline void * malloc();

	///	frees a allocation.
	///
	/// \param block The allocation to free.
	inline void free(void *block);

private:
	//Helper Function
	static inline void * & get_next_address(void * const pointer)
	{
		return *(static_cast<void **>(pointer));
	}

};

////////////////////////////////////////////////////////////////////////////
template<class SizeT, class Allocator, template <typename, typename> class GrowthPolicy>
void GrowingPool<SizeT, Allocator, GrowthPolicy>::add_block(void *block, size_type blockSize, size_type allocSize)
{
    if(!GrowthPolicy<SizeT, Allocator>::growth_policy_initialized())
    {
        create_growth_policy(blockSize, allocSize);
    }

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
template<class SizeT, class Allocator, template <typename, typename> class GrowthPolicy>
void * GrowingPool<SizeT, Allocator, GrowthPolicy>::malloc()
{
    if(first == NULL)
    {
        //We are out of memory so we should grow
        void * new_memory_block = GrowthPolicy<SizeT, Allocator>::grow();
        add_block(new_memory_block, GrowthPolicy<SizeT, Allocator>::block_size(), GrowthPolicy<SizeT, Allocator>::element_size());
    }

	void * ret = first;

	//Increment first
	first = get_next_address(first);

	return ret;
}

////////////////////////////////////////////////////////////////////////////
template<class SizeT, class Allocator, template <typename, typename> class GrowthPolicy>
void GrowingPool<SizeT, Allocator, GrowthPolicy>::free(void *block)
{
	get_next_address(block) = first;
	first = block;
}

#endif // _GROWING_POOL_H_

