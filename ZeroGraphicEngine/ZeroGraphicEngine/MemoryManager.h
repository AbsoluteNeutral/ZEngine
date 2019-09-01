/*****************************************************************************/
/*!
\file			MemoryManager.h
\brief			MemoryManager, ALlocate, Dellocate
*/
/*****************************************************************************/
#ifndef _MEMORYMANAGER_H
#define _MEMORYMANAGER_H

//class ITCHY_API Mem_Exception
//{
//public:
//	Mem_Exception(const std::string& Message) 
//		:message_(Message) 
//	{};
//
//	virtual ~Mem_Exception() {}
//	virtual const char *what(void) const {
//		return message_.c_str();
//	}
//private:
//	std::string message_;
//};

//class ITCHY_API MemoryManager {
//public:
//	MemoryManager();
//	~MemoryManager();
//
//	template<typename T>
//	T* Allocate(size_t arraynum_){
//#ifdef USE_RAWNEW
//		T* obj = nullptr;
//		try {
//			obj = new T[arraynum_];
//		}
//		catch (std::exception& e) {
//			throw e.what();
//		}
//		return obj;
//#else
//		void *obj = nullptr;
//		//++NumOfAllocation;
//		//TotalSizeAdded += arraynum_ * sizeof(T);
//		try {
//			//object = static_cast<T*>(malloc(sizeof(T) * arraynum_));
//			obj = malloc(sizeof(T) * arraynum_);
//		}
//		catch (std::exception& e) {
//			throw e.what();
//		}
//		return static_cast<T*>(object);
//#endif
//
//	}
//	template<typename T>
//	void Free(T* ptr, size_t arraysize_ = 1) {
//#ifdef USE_RAWNEW
//		if (arraysize_ > 1) { DeleteA(ptr); }
//		else { DeleteS(ptr); }
//#else
//		T* b = ptr;
//		T* e = ptr + arraysize_;
//		while (b != e) {
//			b->~T();
//			++b;
//		}
//		//++NumOfDeAllocation;
//		//ptr->~T();
//		free(ptr);
//#endif
//	}
////	void dump();
////private:
////	unsigned NumOfAllocation;
////	unsigned NumOfDeAllocation;
////	unsigned TotalSizeAdded;
//};
//
//extern MemoryManager MemoryMgr;

#endif //_MEMORYMANAGER_H
