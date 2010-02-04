/// \file allocators.h
/// \author Toby Banks
///
/// The purpose of this file is to provide a exact interface for specifying a allocator
/// to work with out memory pools. 
///
/// You can easily add you own allocators as long as you provide the following functions:
///		static inline unsigned char * allocate(unsigned int size);
///		static inline void deallocate(unsigned char * pointer);
///
/// These classes exist to provide a constant interface for use with generic progamming.

#ifndef _ALLOCATORS_H_
#define _ALLOCATORS_H_


/// \class New_Delete_Allocator
///
/// Creates/destroys memory using standard new and delete operators.
/// Primarily used for generic programming.
///
class New_Delete_Allocator
{
public:
	static inline unsigned char * allocate(size_t size) { return new unsigned char[size]; }
	static inline void deallocate(unsigned char * pointer) { delete []pointer; pointer = 0; }
};

/// \class Malloc_Free_Allocator
///
/// Creates/destroys memory using malloc/free.
/// Primarily used for generic programming.
///
class Malloc_Free_Allocator
{
public:
	static inline unsigned char * allocate(size_t size) { return static_cast<unsigned char *>(malloc(size)); }
	static inline void deallocate(unsigned char * pointer) { free(pointer); pointer = 0; }
};

#endif // _ALLOCATORS_H_

