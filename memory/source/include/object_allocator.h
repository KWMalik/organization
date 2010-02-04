/// \file   object_allocator.h
/// \author Toby Banks
///
/// The Object_Allocator class is a manager class for multiple fixed pools. The error checking
/// in this class is absolutely minimal on purpose. Error checking takes time, you don't have time,
/// which is why you are using this pool. 
///
/// The best that this pool offers is asserting on preconditions/postconditions. Those asserts will 
/// also be compiled out in a release/final build.
/// 

#ifndef _OBJECT_ALLOCATOR_H_
#define _OBJECT_ALLOCATOR_H_

#include "allocators.h"
#include "growth_policies.h"
#include "fixed_size_allocators.h"

/// \typedef No_Growth_New_Allocator
///
/// This typedef provides simple access to a fixed size allocator that uses global new/delete for
/// memory allocation, and does not have the ability to grow when it is out of memory.
typedef Fixed_Size_Allocator<size_t, New_Delete_Allocator, No_Growth_Policy> No_Growth_New_Allocator;

/// \typedef No_Growth_Malloc_Allocator
///
/// This typedef provides simple access to a fixed size allocator that uses malloc/free for
/// memory allocation, and does not have the ability to grow when it is out of memory.
typedef Fixed_Size_Allocator<size_t, Malloc_Free_Allocator, No_Growth_Policy> No_Growth_Malloc_Allocator;

/// \typedef Growing_New_Allocator
///
/// This typedef provides simple access to a fixed size allocator that uses global new/delete for
/// memory allocation, and has the ability to grow when it is out of memory at a constant rate.
typedef Fixed_Size_Allocator<size_t, New_Delete_Allocator, Constant_Growth_Policy> Growing_New_Allocator;

/// \typedef Growing_New_Allocator
///
/// This typedef provides simple access to a fixed size allocator that uses global new/delete for
/// memory allocation, and has the ability to grow when it is out of memory at a constant rate.
typedef Fixed_Size_Allocator<size_t, Malloc_Free_Allocator, Constant_Growth_Policy> Growing_Malloc_Allocator;

/// \typedef Default_Allocator
///
/// Just simple access to the allocator which we default to. In this case we default to a 
/// No_Growth_New_Allocator.
typedef No_Growth_New_Allocator Default_Allocator;


/// \struct ObjectAllocationSizes
/// \author Toby Banks
///
/// The purpose of this class is to provide a nice way of initializing our object allocator.
///
/// This needs to be a struct with GCC 4.0 so that we can use an array still initialization. 
/// While we don't need a array style initialization, it is certainly very nice, easy and
/// efficient. For example:
///
/// \begincode
///	 	static const unsigned long NumberOfFixedPools = 4;
///	 	ObjectAllocationSizes oas[NumberOfFixedPools] =
///	 	{
///			{   4,	8}, 
///			{   4,   16},
///			{   4,   32},
///			{   4,   64} 
///		};
///
///		...
///
///		Object_Allocator oa;
///		oa.init(&(oas[0]), NumberOfFixedPools);
/// \endcode
///
struct ObjectAllocationSizes
{
	int number_fixed_allocations;	///< The number of fixed sized allocations.
	unsigned int fixed_allocator_size;		///< The size of a chunk of memory stored in the fixed object allocator.
};

/// \class  Object_Allocator
/// \author Toby Banks
///
/// This class is used to manage a set of different pools. Basically you should be able to create a arbitrary
/// number of pools and this class will be able to manage them. Any time that you want to try to allocate
/// a object you can call allocate with a given size and it will either successfully return you a chunk of 
/// memory or NULL.
///
/// Here is some example usage that assumes that we already have initialized the Object_Allocator (see 
/// ObjectAllocationSizes documentation):
///
/// \begincode
///		static const unsigned long NumberOfFixedPools = 4;
///		ObjectAllocationSizes oas[NumberOfFixedPools] =
///		{
///			{4,  8}, 
///			{4, 16},
///			{4, 32},
///			{4, 64} 
///		};
///
///		...
///
///		Object_Allocator oa;
///		oa.init(&(oas[0]), NumberOfFixedPools);
///
///		...
///
///		class Object_With_Size_8   { ... };
///		class Object_With_Size_16  { ... };
///		class Object_With_Size_32  { ... };
///		class Object_With_Size_64  { ... };
///		class Object_With_Size_128 { ... };
///
///		...
///
///		//The following two lines are fine and will used pooled allocation (as long as the pools aren't full and can't grow).
///		Object_With_Size_8 * ows8 = static_cast<Object_With_Size_8>(oa.allocate(sizeof(Object_With_Size_8)));
///		Object_With_Size_16 * ows16 = static_cast<Object_With_Size_16>(oa.allocate(sizeof(Object_With_Size_16)));
///
///		//This Object_With_Size_128 is too large for our pools and owd128 will contain NULL.
///		Object_With_Size_128 * ows8 = static_cast<Object_With_Size_128>(oa.allocate(sizeof(Object_With_Size_128)));
///
///	\endcode
///
/// NOTE: When one size is full in a No_Growth pool, we don't then try to allocate in the next larger pool. Instead,
/// we allow the system to fail so the programmer can increment the number of allocations in a given pool.
///
/// \tparam Allocator The type of allocator used to get memory. See examples at the top of this file.
template<class Pool>
class Object_Allocator
{
public:
	/// Deletes both pools and oas pointers.
	~Object_Allocator();

	/// Used to initialize this Object_Allocator with an arbitrary number of pools. 
	///
	/// \precond Assumes that the fixed_pool_sizes have been created in order from smallest to largest.
	/// \param fixed_pool_sizes A set of values to create fixed object pools for.
	/// \param number_of_fixed_pools The number of fixed pools to be created.
	void init(ObjectAllocationSizes *fixed_pool_sizes, size_t number_of_fixed_pools);

	/// Used to allocate a block of memory of size. Can return NULL, so watch out.
	///
	/// \param size The size of memory requested.
	/// \return A pointer with memory for use (or NULL if we were unable to allocate memory).
	inline void * allocate(size_t size);

	/// Used to deallocate a block of memory pointed to by pointer. Note, that because of
	/// several optimization in the way that the pools are designed we always need to 
	/// pass the size of the pointer into the deallocate function. The following won't work:
	///	 	deallocate(void * pointer);
	///
	/// \param size The size of memory requested.
	/// \return True doesn't mean we were able to free memory, it just means that the 
	///		 size is something that might be in a pool. False means we definitely couldn't deallocate.
	inline bool deallocate(void * pointer, size_t size_of_allocation);

private:
	/// Helper function to find the the pool that size fits into naturally.
	///
	/// \param size The size to search for in the pools. 
	/// \return The pool used for allocations of a given size. NULL if no pool suits.
	Pool * find_appropriate_pool(size_t size) const;

private:
	ObjectAllocationSizes *oas;		///< Keeps track of the different sized pools we manage.
	Pool * pools;					///< The actual fixed-sized memory pools.
	size_t number_of_pools;			///< The number of pools that the pointer pools points to.
};

#include "impl/object_allocator.inl"

#endif // _OBJECT_ALLOCATOR_H_

