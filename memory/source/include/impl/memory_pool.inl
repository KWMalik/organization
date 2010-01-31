/// \file   memory_pool.inl
/// \author Toby Banks
/// 
/// The implementation of the Memory_Pool interface. I always prefer to have implementations in seperate files.
/// That way you can scan interfaces much more quickly. The only drawback is the much more verbose syntax as well
/// as slightly increase compile time due to preprocessing.
///

////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void Memory_Pool<SizeT>::add_block(void *block, size_type blockSize, size_type allocSize)
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
void * Memory_Pool<SizeT>::allocate()
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
void Memory_Pool<SizeT>::deallocate(void *block)
{
	get_next_address(block) = first;
	first = block;
}
