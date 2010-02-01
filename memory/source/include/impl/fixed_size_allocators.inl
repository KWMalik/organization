/// \file   fixed_size_allocators.inl
/// \author Toby Banks
/// 
/// The implementation of the Fixed_Size_Allocator and Fixed_Size_Type_Allocator interface. I always prefer to have 
/// implementations in seperate files. That way you can scan interfaces much more quickly. The only drawback
/// is the much more verbose syntax as well as slightly increase compile time due to preprocessing.
///

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
Fixed_Size_Allocator<SizeT, Allocator, GrowthPolicy>::~Fixed_Size_Allocator() {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class SizeT, 
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
void Fixed_Size_Allocator<SizeT, Allocator, GrowthPolicy>::construct(unsigned long numberOfElements, unsigned long chunkSize)
{
    unsigned char * buffer = GrowthPolicy<size_type, Allocator>::create_growth_policy(numberOfElements, chunkSize);
    fixed_pool.add_block(reinterpret_cast<void *>(buffer),
                         numberOfElements * chunkSize,
                         chunkSize);
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
        GrowthPolicy<SizeT, Allocator>::grow();
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
Fixed_Size_Type_Allocator<TElement, Allocator, GrowthPolicy>::~Fixed_Size_Type_Allocator() {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
            class TElement,
            class Allocator,
            template <typename, typename> class GrowthPolicy
        >
void Fixed_Size_Type_Allocator<TElement, Allocator, GrowthPolicy>::construct(unsigned long numberOfElements, unsigned long chunkSize)
{
    unsigned char * buffer = Allocator::allocate(numberOfElements * chunkSize);
    type_pool.add_block(reinterpret_cast<void *>(buffer), numberOfElements * sizeof(TElement), sizeof(TElement));

    GrowthPolicy<size_type, Allocator>::create_growth_policy(numberOfElements, chunkSize);
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


