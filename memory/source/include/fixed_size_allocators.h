/// \file fixed_size_allocators.h
/// \author Toby Banks
///
/// This file provides two classes that are very similar, the only real difference is in the interface.
///		Fixed_Size_Allocator both allocate/deallocate work with unsigned char's. This is so they
/// can be used generically with any underlying type.
///
///		Fixed_Size_Type_Allocator works directly with a given type. It is for more general use, for example
/// you might want to be able to create a pool to hold 1000 instances of a class that can be created/destroyed
/// quicly without having to go to the heap. This is just a nicer interface than having to do the casting
/// that would come along with using the Fixed_Size_Allocator.
///
/// Fixed_Size_Allocator is the class that is used with the memory pools you'll be using.

#ifndef _FIXED_SIZE_ALLOCATORS_H_
#define _FIXED_SIZE_ALLOCATORS_H_

#include "memory_pool.h"

/// \class FixedSizeChunkAllocator
///
/// In almost all cases you'll want to access this class by creating a typedef of it.
/// \begincode
///		class Object_That_Is_512_Bytes { ... };
///
///		typedef Fixed_Size_Allocator<size_t, New_Delete_Allocator, No_Growth_Policy> MyAllocator;
///		MyAllocator ma;
///		ma.construct(128, 512);
///		Object_That_Is_512_Bytes * obj1 = reinterpret_cast<Object_That_Is_512_Bytes>(ma.allocate());
///
///		...
///		ma.deallocate(reinterpret_cast<unsigned char *>(obj1));
/// \endcode
///
/// Some default allocators are provided in allocators.h.
/// Some default growth policies are provided in growth_policies.h.
///
/// As long as you implement the whole interface for either a allocator or a growth policy
/// you can have great power and flexibility with how the Fixed_Size_Allocator will behave.
///
/// \tparam SizeT The size_type for this class (assume size_t).
/// \tparam Allocator The allocator used to get more memory from the heap/pool/something external.
/// \tparam GrowthPolicy Used to determine how much more memory to request when we are out of memory.
template<
			class SizeT, 
			class Allocator,
			template <typename, typename> class GrowthPolicy
		>
class Fixed_Size_Allocator : public GrowthPolicy<SizeT, Allocator>
{
public:
	typedef Memory_Pool<SizeT> pool;	///< We are using a very fast memory pool with no error checking.
	typedef SizeT size_type;			///< Safe to assume 99% of the time this is size_t.

	/// TODO: This class shouldn't have ownership of the memory (the growth policy probably should).
	~Fixed_Size_Allocator();

	/// This fucntion is used to construct a memory pool with numberOfElements of size chunkSize.
	///
	/// This should only be called once. There is error checking so if you call it twice.. BEWARE.
	///
	/// \param numberOfElements The number of elements of chunkSize this pool will contain initally.
	/// \param chunkSize The size of a individual element in this pool.
	void construct(unsigned long numberOfElements, unsigned long chunkSize);

	/// Used to gain access to a new block of memory. 
	///
	/// \return A pointer to a fixed sized block of memory.
	unsigned char * allocate();

	/// Used to free a previously allocated block of memory. Again with this function there is no error checking, so 
	/// if you call it with a block of memory that doesn't belong to this pool then what happens is undefined (probably a crash).
	///
	/// \param buffer The allocation to be freed/deallocated.
	void deallocate(unsigned char * buffer);

private:
	pool fixed_pool;			///< Our memory pool.
};

/// \class Fixed_Size_Type_Allocator
///
/// In almost all cases you'll want to access this class by creating a typedef of it.
/// \begincode
///	 class Object_That_Is_1024_Bytes { ... };
///
///	 typedef Fixed_Size_Type_Allocator<Object_That_Is_1024_Bytes, New_Delete_Allocator, No_Growth_Policy> MyAllocator;
///	 MyAllocator ma;
///	 ma.construct(128, 1024);
///	 Object_That_Is_1024_Bytes * obj1 = ma.allocate();
///
///	 ...
///	 ma.deallocate(obj1);
/// \endcode
///
/// Some default allocators are provided in allocators.h.
/// Some default growth policies are provided in growth_policies.h.
///
/// As long as you implement the whole interface for either a allocator or a growth policy
/// you can have great power and flexibility with how the Fixed_Size_Allocator will behave.
///
/// \tparam TElement The element/class that we want to provide a pool for.
/// \tparam Allocator The allocator used to get more memory from the heap/pool/something external.
/// \tparam GrowthPolicy Used to determine how much more memory to request when we are out of memory.
template<
			class TElement,
			class Allocator,
			template <typename, typename> class GrowthPolicy
		>
class Fixed_Size_Type_Allocator : public GrowthPolicy<size_t, Allocator>
{
public:
	typedef size_t size_type;
	typedef Memory_Pool<size_type> pool;

	/// TODO: This class shouldn't have ownership of the memory (the growth policy probably should).
	~Fixed_Size_Type_Allocator();

	/// This fucntion is used to construct a memory pool with numberOfElements of TElement.
	///
	/// This should only be called once. There is error checking so if you call it twice.. BEWARE.
	///
	/// \param numberOfElements The number of elements of TElement this pool will contain initally.
	/// \param chunkSize The size of a individual element in this pool (not needed we derive this from TElement).
	void construct(unsigned long numberOfElements, unsigned long chunkSize = sizeof(TElement));

	/// Used to gain access to a new block of memory for TElement. 
	///
	/// \return A pointer to a fixed sized block of memory.
	TElement * allocate();

	/// Used to free a previously allocated block of memory. Again with this function there is no error checking, so 
	/// if you call it with a block of memory that doesn't belong to this pool then what happens is undefined (probably a crash).
	///
	/// \param buffer The allocation to be freed/deallocated.
	void deallocate(TElement * element);

private:
	pool type_pool;			 ///< Our memory pool.
};

#include "impl/fixed_size_allocators.inl"

#endif // _FIXED_SIZE_ALLOCATORS_H_

