/// \file FastEmbeddedPool.inl
///
/// Just a implementation of the FastEmbeddedPool.
///


////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void FastEmbeddedPool<SizeT>::add_block(void *block, size_type blockSize, size_type allocSize)
{
	//The new end points at the old first (which is 0 the first time in)
	void *end = first;

	//We need to get the pointer to the last valid chunk to prevent overflow on size calculations.
	char *old = static_cast<char*>(block) + ((blockSize - allocSize) / allocSize) * allocSize;

	//Set the last element of the new block to be a pointer to the end old first.
	get_next_element(old) = end;

	if(old == block)
	{
		first = block;
		return;
	}

	for(char *iterator = old - allocSize; iterator != block; old = iterator, iterator -= allocSize)
	{
		get_next_element(iterator) = old;
	}

	get_next_element(block) = old;

	///Set first to point at this block
	first = block;
}


////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void * FastEmbeddedPool<SizeT>::malloc()
{
	void * ret = first;

	//Increment first
	first = get_next_element(first);

	return ret;
}


////////////////////////////////////////////////////////////////////////////
template<class SizeT>
void FastEmbeddedPool<SizeT>::free(void *block)
{
	get_next_element(block) = first;
	first = block;
}

