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

