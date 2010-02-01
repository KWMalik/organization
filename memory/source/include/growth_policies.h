/// \file	growth_policies.h
/// \author Toby Banks
///
/// A growth policy tells a pool whether or not that pool is allowed to grow and if so by how much.
/// The growth policy is responsible for the following:
///	 -Determining how much a pool can grow by (if that pool is allowed to grow).
///	 -Allocating additional memory for a pool (if that pool is allowed to grow).
///	 -Providing a interface so that clients can access the growth policy's state.
///	 -Pools that can grow, need the new pointers tracked. Growth policies must track these pointers.
///	 -Growth policies are responsible for going external for memory (i.e. using the allocator interface).
///

#ifndef _GROWTH_POLICIES_H_
#define _GROWTH_POLICIES_H_

#include <vector>
#include "hack_unsigned_char_container.h"


/// \class Growth_Policy_Base
///
/// All growth policies should derive from this class and must implement the following functions.
///	 inline bool can_grow() const;
///	 inline unsigned char * create_growth_policy(size_type numberOfElements, size_type allocSize);
///	 inline size_type grow_size() const;
///	 inline void * grow()
///
/// This class provides a decent base for what most growth policies would need.
///
template<class SizeT, class Allocator, class TContainer>
class Growth_Policy_Base
{
public:
	typedef SizeT size_type;		///< It's generally safe to assume this will be size_t.
	typedef TContainer Container;	///< We should always refer to Container instead of the vector.
	typedef typename Container::iterator iterator;

protected:
	size_type num_elements;		///< The number of bytes to request when growing.
	size_type allocation_size;	///< The size of a single block of memory in the pool.

	/// A container that tracks the allocations that we are using for pools.
	Container buffers;

	///HACK ALERT!!! THIS HACK IS NEEDED BECAUSE OF MY Hack_Unsigned_Char_Container hack
	///
	/// I need to remove these Destroy functions before sending to Rick.
	template<class T>
	void Destroy(T it)
	{
		Allocator::deallocate((*it));
	}
	void Destroy(unsigned char * it)
	{
		Allocator::deallocate(it);
	}


public:

	/// Constructor zero's out members.
	Growth_Policy_Base() : num_elements(0), allocation_size(0) {}

	/// Destroy the buffers we allocated when we were growing.
	~Growth_Policy_Base()
	{
		iterator it = buffers.begin();
		for(; it != buffers.end(); ++it)
		{
			Destroy(it); 
		}
	}

	/// Return the number of elements that the clients class initally allocated. 
	/// With a Constant_Growth_Policy this number will never change over time.
	/// 
	/// \return The number of elements that the client initially allocated.
	inline size_type number_of_elements() const { return num_elements; }

	/// Return the size of a element that the clients class can create.
	/// With a Constant_Growth_Policy this number will never change over time.
	/// 
	/// \return The size of a element or block that a fixed pool would return. 0 in this special case.
	inline size_type element_size() const { return allocation_size; }

	/// This function caches the blockSize and allocSize for further calculations
	/// on how to grow to store more memory.
	///
	/// Error checking does not occur on either of the parameters. So it's up to the
	/// programmer to ensure that they satisfy the following conditions:
	///		numberOfElements > 0
	///		allocSize > 0
	///
	/// After calling this function growth_policy_initialized returns true.
	///
	/// \param numberOfElements The number of elements the current block of memory can store.
	/// \param allocSize The size of a single element (allocation).
	/// \param buffer A already allocated buffer of size numberOfElements * allocSize.
	inline unsigned char * create_growth_policy(size_type numberOfElements, size_type allocSize)
	{
		num_elements = numberOfElements;
		allocation_size = allocSize;

		unsigned char * buffer = Allocator::allocate(numberOfElements * allocSize);
		buffers.push_back(buffer);
		return buffer;
	}
};

/// \class No_Growth_Policy
/// \author Toby Banks
///
/// This class is used to designate that anything that prototypes this class
/// does not have the ability to add new memory to it.
///
/// Essentially what this means, is that once a pool that uses a No_Growth_Policy
/// is out of memory you can expect it to crash catastrophically shortly
/// after trying to make a allocation when it is out of memory.
///
/// \tparam SizeT The size_type for this class (generally assume size_t).
/// \tparam Allocator The allocator used to new/delete memory.
template<class SizeT, class Allocator>
class No_Growth_Policy : public Growth_Policy_Base<SizeT, Allocator, Hack_Unsigned_Char_Container>
{
public:
	typedef SizeT size_type;						///< It's generally safe to assume this will be size_t.
	typedef Hack_Unsigned_Char_Container Container;	///< HACK: Should be replaced with STL compliant array.
	typedef Growth_Policy_Base<SizeT, Allocator, Hack_Unsigned_Char_Container> Base; ///< Base-Parent class.

public:
	/// Constructor zero's out the member buffer.
	No_Growth_Policy() {}

	/// Destroy our buffer.
	~No_Growth_Policy() {}

	/// Used to determine if it's possible for this class to allocate more memory
	/// for the client to use. Since this is a No_Growth_Policy it will always return
	/// false.
	///
	/// \return False. A No_Growth_Policy can NEVER grow.
	inline bool can_grow() const { return false; }
	
	/// Usually this function would return the size of the next call to grow. Since we 
	/// don't want to have any member variables in this class (so we can take advantage
	/// of empty base class optimizations) we will always return 0.
	///
	/// \return The size of the next call to grow.
	inline size_type grow_size() const { return 0; }

	/// This function is responsible for creating a new block of memory for a pool. 
	/// It doesn't do anything for a No_Growth_Policy.
	///
	/// \return NULL. This class can't create more memory so NULL is returned.
	inline void * grow() { /* Assert(TJB, "GROWING OF THIS CLASS IS NOT ALLOWED"); */ /*exit();*/ return NULL; }
};

/// \class Constant_Growth_Policy
/// \author Toby Banks
///
/// This class allocates new memory according to a constant growth model. This means
/// that when this class is initialized with certain properties:
///		create_growth_policy(12, 128);
///
/// This class will always grow by a constant rate (in this case by 12 * 128 = 1536).
///
/// \tparam SizeT The size_type for this class (generally assume size_t).
/// \tparam Allocator The allocator used to new/delete memory.
template<class SizeT, class Allocator>
class Constant_Growth_Policy : public Growth_Policy_Base<SizeT, Allocator, std::vector<unsigned char *> >
{
public:
	typedef SizeT size_type;						///< Safe to assume this will be size_t.
	typedef std::vector<unsigned char *> Container;	///< Container instead of the vector.
	typedef Growth_Policy_Base<SizeT, Allocator, std::vector<unsigned char *> > Base; ///< Base-Parent class.

public:
	/// Constructor zeros out the member variables
	Constant_Growth_Policy() {}

	/// Destroy the buffers we allocated when we were growing.
	~Constant_Growth_Policy() {}

	/// Used to determine if it's possible for this class to allocate more memory
	/// for the client to use. Since this is a Constant_Growth_Policy it will always return
	/// true.
	///
	/// \return True. A Constant_Growth_Policy can always grow (within limits of the operating system/Allocator).
	inline bool can_grow() const { return true; }

	/// Used to access externally the size of the next call to grow. 
	/// With a Constant_Growth_Policy this number will never change over time.
	/// 
	/// \return The size of the next call to grow.	
	inline size_type grow_size() const { return Base::number_of_elements() * Base::element_size(); }

	/// This function is responsible for creating a new block of memory for a pool. 
	/// The new block is the size of the result of calling grow_size().
	///
	/// No error checking is done here. We are making a video game, most of the time
	/// exceptions are disabled.
	///
	/// \return A pointer to a new block of memory of size grow_size().
	inline void * grow()
	{
		unsigned char * buffer = Allocator::allocate(grow_size());
		Base::buffers.push_back(buffer);
		return buffer;
	}
};

#endif //_GROWTH_POLICIES_H_

