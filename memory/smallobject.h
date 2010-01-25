#ifndef _AXE_MEMORY_SMALL_OBJECT_H_
#define _AXE_MEMORY_SMALL_OBJECT_H_

#include "memory/fixedallocator.h"

namespace Axe
{
namespace Memory
{

/// \class AllocatorSingleton
///
/// This template class is derived from SmallObjAllocator in order to pass   
/// template arguments into it, and still have a default constructor for the 
/// singleton. Each instance is a unique combination of all the template 
/// parameters, and hence is singleton only with respect 
/// to those parameters.  The template parameters have default
/// values and the class has typedefs identical to both SmallObject and
/// SmallValueObject so that this class can be used directly instead of going
/// through SmallObject or SmallValueObject.  That design feature allows
/// clients to use the new_handler without having the name of the new_handler
/// function show up in classes derived from SmallObject or SmallValueObject.
/// 
/// The only functions in the allocator which show up in SmallObject or
/// SmallValueObject inheritance hierarchies are the new and delete
/// operators.
template
<
	template <class, class> class ThreadingModel = LOKI_DEFAULT_THREADING_NO_OBJ_LEVEL,
	std::size_t chunkSize = LOKI_DEFAULT_CHUNK_SIZE,
	std::size_t maxSmallObjectSize = LOKI_MAX_SMALL_OBJECT_SIZE,
	std::size_t objectAlignSize = LOKI_DEFAULT_OBJECT_ALIGNMENT,
	template <class> class LifetimePolicy = kOKI_DEFAULT_SMALLOBJ_LIFETIME,
	class MutexPolicy = LOKI_DEFAULT_MUTEX
>
class AllocatorSingleton : public SmallObjAllocator
{
public:

};

class SmallObject
{
public:
	/// Zero's out all non-static members.
	SmallObject() {}

	/// Releases all of the Chunks owned by this class.
	~SmallObject() {}

};

} // namespace Memory
} // namespace Axe

#endif // _AXE_MEMORY_SMALL_OBJECT_H_
        /// Defines type of allocator.
        typedef AllocatorSingleton< ThreadingModel, chunkSize,
            maxSmallObjectSize, objectAlignSize, LifetimePolicy > MyAllocator;

        /// Defines type for thread-safety locking mechanism.
        typedef ThreadingModel< MyAllocator, MutexPolicy > MyThreadingModel;

        /// Defines singleton made from allocator.
        typedef Loki::SingletonHolder< MyAllocator, Loki::CreateStatic,
            LifetimePolicy, ThreadingModel > MyAllocatorSingleton;

        /// Returns reference to the singleton.
        inline static AllocatorSingleton & Instance( void )
        {
            return MyAllocatorSingleton::Instance();
        }

        /// The default constructor is not meant to be called directly.
        inline AllocatorSingleton() :
            SmallObjAllocator( chunkSize, maxSmallObjectSize, objectAlignSize )
            {}

        /// The destructor is not meant to be called directly.
        inline ~AllocatorSingleton( void ) {}

        /** Clears any excess memory used by the allocator.  Complexity is
         O(F + C) where F is the count of FixedAllocator's in the pool, and C
         is the number of Chunks in all FixedAllocator's.  This never throws.
         @note This function can be used as a new_handler when Loki and other
         memory allocators can no longer allocate.  Although the C++ Standard
         allows new_handler functions to terminate the program when they can
         not release any memory, this will not do so.
         */
        static void ClearExtraMemory( void );

        /** Returns true if anything in implementation is corrupt.  Complexity
         is O(F + C + B) where F is the count of FixedAllocator's in the pool,
         C is the number of Chunks in all FixedAllocator's, and B is the number
         of blocks in all Chunks.  If it determines any data is corrupted, this
         will return true in release version, but assert in debug version at
         the line where it detects the corrupted data.  If it does not detect
         any corrupted data, it returns false.
         */
        static bool IsCorrupted( void );

    private:
        /// Copy-constructor is not implemented.
        AllocatorSingleton( const AllocatorSingleton & );
        /// Copy-assignment operator is not implemented.
        AllocatorSingleton & operator = ( const AllocatorSingleton & );
    };

