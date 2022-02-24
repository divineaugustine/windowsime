#pragma once


	
template <class T>
class ComPtr
{
public:
	ComPtr()					: ptr(nullptr)				{								}
	ComPtr(T			 *val)	: ptr(val)					{	if (ptr) ptr->AddRef();		}
	ComPtr(const ComPtr  &val)	: ptr(val.ptr)				{	if (ptr) ptr->AddRef();		}
	ComPtr(		 ComPtr &&val)	: ptr(val.ptr)				{	val.ptr = nullptr;			}
	~ComPtr()												{	reset();					}

	inline operator T*  ()						 const		{	return  ptr;				}
	inline T& operator* ()						 const		{	return *ptr;				}
	inline T* operator->()						 const		{	return  ptr;				}
												 
	inline bool operator!()						 const		{	return  ptr == nullptr;		}
	inline bool operator==(const T *val)		 const		{	return  ptr == val;			}
	inline bool operator!=(const T *val)		 const		{	return  ptr != val;			}
	inline bool operator==(const ComPtr<T> &val) const		{	return  ptr == val.ptr;		}
	inline bool operator!=(const ComPtr<T> &val) const		{	return  ptr != val.ptr;		}
	
	inline ComPtr &operator=(T			   *val)			{	if ( ptr != val)	 { reset();	ptr = val;		if (ptr) ptr->AddRef();	}; return *this;	}
	inline ComPtr &operator=(const ComPtr  &val)			{	if ( ptr != val.ptr) { reset();	ptr = val.ptr;	if (ptr) ptr->AddRef();	}; return *this;	}
	inline ComPtr &operator=(      ComPtr &&val)			{	if ( ptr != val.ptr) { reset();	ptr = val.ptr;  val.ptr = nullptr;		}; return *this;	}

	//! Tries to acquire the given interface; if it does not exist, null is returned
	template <class Q> inline ComPtr<Q> QueryInterface()
	{	
		ComPtr<Q> result;
		return ptr && ptr->QueryInterface(__uuidof(Q), (void**)&result.reset()) >= 0 ? result : ComPtr<Q>();
	}
	
	//! Resets the pointer to null. You can use the returned pointer to obtain a new value, e.g. CoCreateInstance(..., (void**)&comptr.reset());
	T *&reset()
	{
		T *val = ptr;
		ptr = nullptr;
		if (val) val->Release();

		return ptr;
	}

private:

	//not allowed...
	void operator&();

	T* ptr;
};


