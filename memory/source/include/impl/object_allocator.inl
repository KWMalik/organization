/// \file   object_allocator.inl
/// \author Toby Banks
/// 
/// The implementation of the Object_Allocator interface. I always prefer to have implementations in seperate files.
/// That way you can scan interfaces much more quickly. The only drawback is the much more verbose syntax as well
/// as slightly increase compile time due to preprocessing.
///

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Pool>
Object_Allocator<Pool>::~Object_Allocator()
{
	if(oas)
	{
		delete [] oas;
		oas = 0; 
	}
	if(pools)
	{
		delete [] pools;
		pools = 0;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Pool>
void Object_Allocator<Pool>::init(ObjectAllocationSizes *fixed_pool_sizes, size_t number_of_fixed_pools)
{
	number_of_pools = number_of_fixed_pools;
	oas = fixed_pool_sizes;
	pools = new Pool[number_of_pools];

	for(size_t i = 0; i < number_of_pools; ++i)
	{
		pools[i].construct(oas[i].number_fixed_allocations, oas[i].fixed_allocator_size);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Pool>
inline Pool * Object_Allocator<Pool>::find_appropriate_pool(size_t size) const
{
	for(size_t i = 0; i < number_of_pools; ++i)
	{
		if(oas[i].fixed_allocator_size >= size)
		{
			return &(pools[i]);
		}
	}

	//Assert(TJB, "Should Never hit here");
	return NULL;
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Pool>
inline void * Object_Allocator<Pool>::allocate(size_t size)
{
	//Find the appropriate pool.
	Pool * appropriate_allocator = find_appropriate_pool(size);
	if(appropriate_allocator)
	{
		//We found a allocator
		return appropriate_allocator->allocate();
	}
	else
	{
		//Assert(TJB, "Unable to find a appropriate allocator");
		return NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Pool>
inline bool Object_Allocator<Pool>::deallocate(void *pointer, size_t size_of_allocation)
{
	//Find the appropriate pool.
	Pool * appropriate_allocator = find_appropriate_pool(size_of_allocation);
	bool success = false;
	if(appropriate_allocator)
	{
		appropriate_allocator->deallocate(static_cast<unsigned char *>(pointer));

		//NOTE: WE ARE ASSUMING THAT THE FREE WORKED!!! IT IS A BIG ASSUMPTION!!! BUT IT'S FAST!!!
		return true;
	}
	return false;
}

