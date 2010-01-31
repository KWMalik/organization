#ifndef _GROWING_POOL_H_
#define _GROWING_POOL_H_


/// \class Memory_Pool
///
/// This class was designed to be as light as possible. It does not 
/// deallocate memory, call destructors for classes or allocate memory. 
/// I'm planning on writing a couple more heavy-duty memory pooling 
/// classes later on. 
///
/// It is EXTREMELY IMPORTANT to note that this class only works 
/// with data types larger than the default size of a pointer on the 
/// system you are working on.
/// 
/// This means if you are using 32-bit addressing, only place objects
/// in this pool that are equal to or large than 32-bits.
///
/// The same is true of a 64-bit system. If you have 64-bit addresses
/// then you want to ensure that all pooled allocations are larger 
/// than 64-bits.
///
/// Here is a example of how this class works.
///
///	\code
///		//In the following example 'x' means unallocated and a number 
///		//means the value of a pointer.
///		//
///		//For example:
///		//	88xxx	///< Pool begins with two allocations of 8, 
///		//          ///< followed by three unallocated slots.
///		//
///		Memory_Pool<int> pool;
///
///		//Allocate some memory and give it to the pool.
///		void *pBlock1 = static_cast<void*>(new int[10]);
///		pool.add_block(pBlock1, sizeof(int) * 10, sizeof(int));
///
///		///Fill the first 5 allocations with the number 8
///		int *pa = static_cast<int*>(pool.allocate());
///		int *pb = static_cast<int*>(pool.allocate());
///		int *pc = static_cast<int*>(pool.allocate());
///		int *pd = static_cast<int*>(pool.allocate());
///		int *pe = static_cast<int*>(pool.allocate());
///		*pa = 8;
///		*pb = 8;
///		*pc = 8;
///		*pd = 8;
///		*pe = 8;
///
///		//The pool now looks like 88888xxxxx
///		
///		//deallocate some elements
///		pool.deallocate(pb);
///		pool.deallocate(pd);
///
///		//The pool now looks like 8x8x8xxxxx
///		
///		//After the following allocation/assignment the pool 
///		//will look like 8x858xxxxx
///		pb = static_cast<int*>(pool.allocate());
///		*pb = 5;
///
///		//After the following allocation/assignment the pool will 
///		//look like 85858xxxxx
///		pd = static_cast<int*>(pool.allocate());
///		*pd = 5;
///	\endcode
///
///	TJB: TODO: Write up the following example.
///		The following is a example of how to add more memory to a pool:
///
template<class SizeT>
class Memory_Pool
{
public:
	typedef SizeT size_type;    ///< It's generally safe to assume this will be size_t.

protected:
	void *first;	            ///< The first free (unallocated) element in the pool.
public:
	/// Default constructor that initializes the internal array to 0
	Memory_Pool() : first(0) {}

	/// Destructor
	~Memory_Pool() {}

	///	Checks to see if this class is empty. Meaning that first == 0.
	///
	/// \return True if first == 0; false otherwise.
	inline bool empty() const { return first == 0; }

	///	Adds a allocated block of memory for this Memory_Pool class to 
	///	pool allocations from.
	///
	/// \param block The block of memory to pool.
	/// \param blockSize Size (bytes) of the memory allocated 
	///		   in block.
	/// \param allocSize Size (bytes) that block should be split into
	void add_block(void *block, size_type blockSize, size_type allocSize);

	///	After a block has been added allocate will return a open allocation, or NULL.
    ///
    /// \return A address of a block of memory for use, or NULL if no block exists.
	inline void * allocate();

	///	Used to free up a block of memory.
	///
	/// \param block The allocation to deallocate.
	inline void deallocate(void *block);

private:
    /// Converts pointer to a pointer-to-a-pointer and then dereferences it.
    ///
    /// \return The dereferenced address contained under pointer.
	static inline void * & get_next_address(void * const pointer)
	{
		return *(static_cast<void **>(pointer));
	}
};

#include "impl/memorypool.inl"

#endif // _GROWING_POOL_H_

