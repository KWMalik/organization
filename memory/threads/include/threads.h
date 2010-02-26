#ifndef _THREADS_H_
#define _THREADS_H_

#define PTHREAD

#include <cassert>

// Define to 1 enable recursive mutex support.
#if 0
//Experimental recursive mutex support
#define THREADS_MUTEX_CTOR(x)
#else
//No recursive mutex support.
#define THREADS_MUTEX_CTOR(x)
#endif

/// Implementation for Unix/Mac.
#ifdef PTHREAD
#include <pthread.h>

#define THREADS_MUTEX(x)			pthread_mutex_t(x);
#define THREADS_MUTEX_INIT(x)		::pthread_mutex_init(x, 0);

#define THREADS_MUTEX_DELETE(x)		::pthread_mutex_destroy(x)
#define THREADS_MUTEX_LOCK(x)		::pthread_mutex_lock(x)
#define THREADS_MUTEX_UNLOCK(x)		::pthread_mutex_unlock(x)
#define THREADS_LONG				long


#define DEFAULT_THREADING 			::System::SingleThreaded
#define DEFAULT_MUTEX 				::System::Mutex

#define THREADS_ATOMIC(x)									\
	pthread_mutex_lock(&atomic_mutex_);						\
	x;														\
	pthread_mutex_unlock(&atomic_mutex_);


#define THREADS_ATOMIC_FUNCTIONS							\
	private:												\
		static pthread_mutex_t atomic_mutex_;				\
	public:													\
		static IntType AtomicMultiply(volatile IntType& lval, const IntType val)		\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			lval *= val;									\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static IntType AtomicDivide(volatile IntType& lval, const IntType val)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			lval /= val;									\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static IntType AtomicIncrement(volatile IntType& lval)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			++lval;											\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static IntType AtomicDecrement(volatile IntType& lval)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			--lval;											\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static void AtomicAssign(volatile IntType& lval, const IntType val)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			lval = val;										\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static void AtomicAssign(IntType& lval, volatile const IntType& val)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			lval = val;										\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
		static IntType AtomicIncrement(volatile IntType& lval, const IntType compare, bool & matches)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			++lval;											\
			matches = ( compare == lval );					\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static IntType AtomicDecrement(volatile IntType& lval, const IntType compare, bool & matches)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			--lval;											\
			matches = ( compare == lval );					\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static IntType AtomicMultiply(volatile IntType& lval, const IntType val, const IntType compare, bool & matches)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			lval *= val;									\
			matches = ( compare == lval );					\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}													\
															\
		static IntType AtomicDivide(volatile IntType& lval, const IntType val, const IntType compare, bool & matches)	\
		{													\
			::pthread_mutex_lock(&atomic_mutex_);			\
			lval /= val;									\
			matches = ( compare == lval );					\
			::pthread_mutex_unlock(&atomic_mutex_);			\
			return lval;									\
		}


namespace System
{
	/// A simple portable mutex.
	class Mutex
	{
	public:
		Mutex() THREADS_MUTEX_CTOR(mtx_)
		{
			THREADS_MUTEX_INIT(&mtx_);
		}
		~Mutex()
		{
			THREADS_MUTEX_DELETE(&mtx_);
		}
		void Lock()
		{
			THREADS_MUTEX_LOCK(&mtx_);
		}
		void Unlock()
		{
			THREADS_MUTEX_UNLOCK(&mtx_);
		}
	private:
		Mutex(const Mutex &);
		Mutex & operator=(const Mutex &);
		THREADS_MUTEX(mtx_);
	};

	template<class Host, class MutexPolicy = DEFAULT_MUTEX>
	class SingleThreaded
	{
	public:
		/// \class Lock
		///
		class Lock
		{
		public:
			Lock() {}
			explicit Lock(const SingleThreaded&) {}
			explicit Lock(const SingleThreaded*) {}
		};

		typedef Host VolatileType;

		typedef int IntType;

		static IntType AtomicAdd(volatile IntType& lval, const IntType val) { return lval += val; }

		static IntType AtomicSubtract(volatile IntType& lval, const IntType val) { return lval -= val; }

		static IntType AtomicMultiply(volatile IntType& lval, const IntType val) { return lval *= val; }

		static IntType AtomicDivide(volatile IntType& lval, const IntType val) { return lval /= val; }

		static IntType AtomicIncrement(volatile IntType& lval) { return ++lval; }

		static IntType AtomicDivide(volatile IntType& lval) { return --lval; }

		static void AtomicAssign(volatile IntType& lval, const IntType val) { return lval = val; }

		static void AtomicAssign(IntType& lval, volatile IntType & val) { return lval = val; }

		static IntType AtomicAdd(volatile IntType& lval, const IntType val, const IntType compare, bool & matches)
		{
			lval += val;
			matches = ( lval == compare );
			return lval;
		}

		static IntType AtomicSubtract(volatile IntType& lval, const IntType val, const IntType compare, bool & matches)
		{
			lval -= val;
			matches = ( lval == compare );
			return lval;
		}

		static IntType AtomicMultiply(volatile IntType& lval, const IntType val, const IntType compare, bool & matches)
		{
			lval *= val;
			matches = ( lval == compare );
			return lval;
		}

		static IntType AtomicDivide(volatile IntType& lval, const IntType val, const IntType compare, bool & matches)
		{
			lval /= val;
			matches = ( lval == compare );
			return lval;
		}

		static IntType AtomicIncrement(volatile IntType& lval, const IntType compare, bool & matches)
		{
			++lval;
			matches = ( lval == compare );
			return lval;
		}

		static IntType AtomicDecrement(volatile IntType& lval, const IntType compare, bool & matches)
		{
			--lval;
			matches = ( lval == compare );
			return lval;
		}
	};


	template<class Host, class MutexPolicy = DEFAULT_MUTEX>
	class ObjectLevelLockable
	{
	private:
		mutable MutexPolicy mtx_;

	public:
		ObjectLevelLockable() : mtx_() {}

		ObjectLevelLockable(const ObjectLevelLockable&) : mtx_() {}

		~ObjectLevelLockable() {}

		class Lock;
		friend class Lock;

		/// \class Lock
		/// Lock class to lock on a object level.
		class Lock
		{
		public:
			/// Lock Object
			explicit Lock(const ObjectLevelLockable& host) : host_(host)
			{
				host_.mtx_.Lock();
			}

			/// Lock Object
			explicit Lock(const ObjectLevelLockable* host) : host_(*host)
			{
				host_.mtx_.Lock();
			}

			/// Unlock the object
			~Lock()
			{
				host_.mtx_.Unlock();
			}

		private:
			/// Private by design of the object level threading
			Lock();
			Lock(const Lock&);
			Lock& operator=(const Lock&);
			const ObjectLevelLockable & host_;
		};

		typedef volatile Host VolatileType;
		typedef THREADS_LONG IntType;
		THREADS_ATOMIC_FUNCTIONS
	};

	template<class Host, class MutexPolicy>
	pthread_mutex_t ObjectLevelLockable<Host, MutexPolicy>::atomic_mutex_ = PTHREAD_MUTEX_INITIALIZER;



	/// \class ClassLevelLockable
	///
	///
	template<class Host, class MutexPolicy = DEFAULT_MUTEX>
	class ClassLevelLockable
	{
	private:
		struct Initializer
		{
			bool init_;
			MutexPolicy mtx_;

			Initializer() : init_(false), mtx_()
			{
				init_ = true;
			}

			~Initializer()
			{
				assert(init_);
			}
		};

		static Initializer initializer_;

	public:
		class Lock;
		friend class Lock;

		/// \class Lock
		///
		class Lock
		{
		public:
			/// Lock class
			Lock()
			{
				assert(initializer_.init_);
				initializer_.mtx_.Lock();
			}

			/// Lock class
			explicit Lock(const ClassLevelLockable&)
			{
				assert(initializer_.init_);
				initializer_.mtx_.Lock();
			}

			/// Lock class
			explicit Lock(const ClassLevelLockable*)
			{
				assert(initializer_.init_);
				initializer_.mtx_.Lock();
			}

			~Lock()
			{
				assert(initializer_.init_);
				initializer_.mtx_.Unlock();
			}

		private:
			Lock(const Lock&);
			Lock& operator=(const Lock&);
		};

		typedef volatile Host VolatileType;
		typedef THREADS_LONG IntType;
		THREADS_ATOMIC_FUNCTIONS
	};

	template<class Host, class MutexPolicy>
	pthread_mutex_t ClassLevelLockable<Host, MutexPolicy>::atomic_mutex_ = PTHREAD_MUTEX_INITIALIZER;

	template<class Host, class MutexPolicy>
	typename ClassLevelLockable<Host, MutexPolicy>::Initializer ClassLevelLockable<Host, MutexPolicy>::initializer_;

} // namespace System


#endif // PTHREAD

#endif // _THREADS_H_

