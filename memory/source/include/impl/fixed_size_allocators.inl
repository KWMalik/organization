////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////Fixed_Size_Allocator///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class SizeT, 
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
Fixed_Size_Allocator<SizeT, Allocator, GrowthPolicy>::~Fixed_Size_Allocator() 
{
    if(buffer)
    {
        Allocator::deallocate(buffer);
        buffer = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class SizeT, 
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
void Fixed_Size_Allocator<SizeT, Allocator, GrowthPolicy>::construct(unsigned long numberOfElements, unsigned long chunkSize)
{
    buffer = Allocator::allocate(numberOfElements * chunkSize);
    fixed_pool.add_block(reinterpret_cast<void *>(buffer),
                         numberOfElements * chunkSize,
                         chunkSize);

    GrowthPolicy<size_type, Allocator>::create_growth_policy(numberOfElements, chunkSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class SizeT, 
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
unsigned char * Fixed_Size_Allocator<SizeT, Allocator, GrowthPolicy>::allocate()
{
    unsigned char * ret = static_cast<unsigned char *>(fixed_pool.allocate());

    if(ret == NULL && GrowthPolicy<SizeT, Allocator>::can_grow())
    {
        //We are out of memory so we should grow
        void * new_memory_block = GrowthPolicy<SizeT, Allocator>::grow();

        if(new_memory_block)
        {
            fixed_pool.add_block(new_memory_block, GrowthPolicy<SizeT, Allocator>::grow_size(), GrowthPolicy<SizeT, Allocator>::element_size());
        }
        else
        {
            //Assert(TJB, "DEATH!!!!");
        }

        ret = static_cast<unsigned char *>(fixed_pool.allocate());
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class SizeT, 
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
void Fixed_Size_Allocator<SizeT, Allocator, GrowthPolicy>::deallocate(unsigned char * buffer)
{
    fixed_pool.deallocate(buffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////Fixed_Size_Type_Allocator///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class TElement,
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
Fixed_Size_Type_Allocator<TElement, Allocator, GrowthPolicy>::~Fixed_Size_Type_Allocator() 
{
    if(buffer)
    {
        Allocator::deallocate(buffer);
        buffer = 0;
    } 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class TElement,
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
void Fixed_Size_Type_Allocator<TElement, Allocator, GrowthPolicy>::construct(unsigned long numberOfElements, unsigned long chunkSize)
{
    buffer = Allocator::allocate(numberOfElements * chunkSize);
    type_pool.add_block(reinterpret_cast<void *>(buffer), numberOfElements * sizeof(TElement), sizeof(TElement));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class TElement,
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
TElement * Fixed_Size_Type_Allocator<TElement, Allocator, GrowthPolicy>::allocate()
{
    return reinterpret_cast<TElement *>(type_pool.allocate());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class TElement,
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
void Fixed_Size_Type_Allocator<TElement, Allocator, GrowthPolicy>::deallocate(TElement * element)
{
    type_pool.deallocate(reinterpret_cast<void *>(element));
}


