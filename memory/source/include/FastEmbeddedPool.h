#ifndef _FAST_EMBEDDED_POOL_H_
#define _FAST_EMBEDDED_POOL_H_

#include <iostream>

/// \class FastEmbeddedPool
///
///		This class was designed to be as light as possible. It does not 
/// free memory, call destructors for classes or allocate memory. 
/// I'm planning on writing a couple more heavy-duty memory pooling 
/// classes later on. 
///
///		Here is a example of how this class works.
///
///	\code
///		//In the following example 'x' means unallocated and a number 
///		//means the value of a pointer.
///		//
///		//For example:
///		//	88xxx	///< Pool begins with two allocations of 8, 
///		//          ///< followed by three unallocated slots.
///		//
///		FastEmbeddedPool<int> pool;
///
///		//Allocate some memory and give it to the pool.
///		void *pBlock1 = static_cast<void*>(new int[10]);
///		pool.add_block(pBlock1, sizeof(int) * 10, sizeof(int));
///
///		///Fill the first 5 allocations with the number 8
///		int *pa = static_cast<int*>(pool.malloc());
///		int *pb = static_cast<int*>(pool.malloc());
///		int *pc = static_cast<int*>(pool.malloc());
///		int *pd = static_cast<int*>(pool.malloc());
///		int *pe = static_cast<int*>(pool.malloc());
///		*pa = 8;
///		*pb = 8;
///		*pc = 8;
///		*pd = 8;
///		*pe = 8;
///
///		//The pool now looks like 88888xxxxx
///		
///		//free some elements
///		pool.free(pb);
///		pool.free(pd);
///
///		//The pool now looks like 8x8x8xxxxx
///		
///		//After the following allocation/assignment the pool 
///		//will look like 8x858xxxxx
///		pb = static_cast<int*>(pool.malloc());
///		*pb = 5;
///
///		//After the following allocation/assignment the pool will 
///		//look like 85858xxxxx
///		pd = static_cast<int*>(pool.malloc());
///		*pd = 5;
///	\endcode
///
///		The following is a example of how to add more memory to a pool:
///	TJB: TODO!!!!!!!!!
///
template<class SizeT>
class FastEmbeddedPool
{
public:
	typedef SizeT size_type;

protected:
	void *first;	///< The first element in the pool.

public:
	/// Default constructor that initializes the internal array to 0
	FastEmbeddedPool() : first(0) {}

	/// Destructor
	~FastEmbeddedPool() {}

	///	Checks to see if this class is empty. Meaning that first == 0.
	///
	/// \return True if first == 0; false otherwise.
	inline bool empty() const { return first == 0; }

	///	Adds a allocated block of memory for this FastEmbeddedPool class to 
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

	/// Will print the mem block.
	friend std::ostream& operator<<(std::ostream& s, FastEmbeddedPool<SizeT>& pool)
	{
		s << "ToBy!" << std::endl;	
	}

private:
	//Helper Function
	static inline void * & next_of(void * const pointer)
	{
		return *(static_cast<void **>(pointer));
	}

};

////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void FastEmbeddedPool<SizeT>::add_block(void *block, size_type blockSize, size_type allocSize)
{
	//The new end points at the old first (which is 0 the first time in)
	void *end = first;

	//We need to get the pointer to the last valid chunk to prevent overflow on size calculations.
	char *old = static_cast<char*>(block) + ((blockSize - allocSize) / allocSize) * allocSize;

	//Set the last element of the new block to be a pointer to the end old first.
	next_of(old) = end;

	if(old == block)
	{
		first = block;
		return;
	}

	for(char *iterator = old - allocSize; iterator != block; old = iterator, iterator -= allocSize)
	{
		next_of(iterator) = old;
	}

	next_of(block) = old;

	///Set first to point at this block
	first = block;
}

////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void * FastEmbeddedPool<SizeT>::malloc()
{
	void * ret = first;

	//Increment first
	first = next_of(first);

	return ret;
}

////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void FastEmbeddedPool<SizeT>::free(void *block)
{
	next_of(block) = first;
	first = block;
}

#endif // _FAST_EMBEDDED_POOL_H_

