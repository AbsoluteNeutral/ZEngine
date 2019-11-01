
#ifndef _ZG_ALLOCATOR_H
#define _ZG_ALLOCATOR_H

#include "Logging.h"

//safe delete and free
#define DeleteS(x)   { if(x) { delete x; x = nullptr;} } 
#define DeleteA(x)   { if(x) { delete [] x; x = nullptr;} }
#define FreeSafe(x)  { free(x); x = nullptr; }

// __________________________________________________________________ Using Raw New
template<typename T>
T* Allocate(size_t arraynum_) 
{
	T* obj = nullptr;
	if (arraynum_ == 1) 
	{
		try {
			obj = new T;
		}
		catch (std::exception& e) {
			Warn("Size failed to allocate ONE");
			PopUp_Error("Allocation Failed Hard yo!");
			throw e.what();
		}
	}
	else 
	{
		try {
			obj = new T[arraynum_];
		}
		catch (std::exception& e) {
			Warn("Failed to Allocate Array of %d, Total: %d Bytes", arraynum_, sizeof(T) * arraynum_);
			PopUp_Error("Allocation Failed Hard yo!");
			throw e.what();
		}
	}
	return obj;
}

template<typename T>
void DeallocateOne(T* ptr)
{
	DeleteS(ptr);
}
template<typename T>
void Deallocate(T* ptr) 
{
	DeleteA(ptr);
}

// _____________________________________________________________________________ Placement New
template<typename T>
T* AllocateMalloc(size_t arraynum_) 
{
	void *obj = nullptr;
	try {
		obj = malloc(sizeof(T) * arraynum_);
	}
	catch (std::exception& e) {
		Warn("Failed to Allocate Array of %d, Total: %d Bytes", arraynum_, sizeof(T) * arraynum_);
		ErrorMsg("Allocation Failed Hard yo!");
		throw e.what();
	}
	return static_cast<T*>(obj);
}

template<typename T>
void DeallocateMalloc(T* ptr)
{
	ptr->~T();
	FreeSafe(ptr);
}
template<typename T>
void DeallocateMalloc(T* ptr, size_t arraysize_)
{
	T* b = ptr;
	T* e = ptr + arraysize_;
	while (b != e) 
	{
		b->~T();
		++b;
	}
	FreeSafe(ptr);
}

#endif //_ZG_ALLOCATOR_H
