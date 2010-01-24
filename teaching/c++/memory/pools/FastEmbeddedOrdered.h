////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	FastEmbedded.h
/// \author	Toby Banks
///
///    Used to make fast pooled allocations. If you are not working on a embedded environment
/// use a different pool type.
///
///

#ifndef HB_JACKRABBIT_POOL_FAST_EMBEDDED_ORDERED_H
#define HB_JACKRABBIT_POOL_FAST_EMBEDDED_ORDERED_H

#include "config.h"
#include "Types.h"
#include "Utilities/Identities.h"

#include "Pool/FastEmbedded.h"

namespace HB
{
namespace JackRabbit
{
namespace Pool
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class FastEmbeddedOrdered
	///
	///		This class was designed to be as light as possible. It does not free memory, call destructors for classes
	///	or allocate memory. I'm planning on writing a couple more heavy-duty memory pooling classes later on. 
	///
	///		Here is a example of how this class works.
	///
 	template<class SizeT>
	class FastEmbeddedOrdered : private FastEmbedded<SizeT>
	{
	};

}
}
}
#endif
