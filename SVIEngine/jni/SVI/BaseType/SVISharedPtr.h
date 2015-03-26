#ifndef __SVI_SHAREDPTR_H_
#define __SVI_SHAREDPTR_H_

#include "../SVIDefines.h"
#include "SVIBaseTypes.h"
#include <assert.h>

namespace SVI {

	template <typename T> class SVISharedPtr;

	// Creation Date	: 2011/11/25 rareboy0112@facebook.com
	// Description		: Interface for managing the reference count
	// Last Modified	: 2011/11/29 rareboy0112@facebook.com
	template <typename T> class IRefCnt
	{
	protected:
		// 순수 가상 함수 설정
		virtual void addRef() = 0;
		virtual void release() = 0;
		virtual T* getPtr() const = 0;
		virtual int getCount() const = 0;
	
		friend class SVISharedPtr<T>;
	};


	// Creation Date	: 2011/11/25 rareboy0112@facebook.com
	// Description		: The implementation class IRefCnt.
	// Last Modified	: 2011/11/29 rareboy0112@facebook.com
	template <typename T> class IRefCntImpl : public IRefCnt<T>
	{
	private:
		int mCount;

	protected:
		virtual void addRef() { 
			++mCount;
		}
	
		virtual void release()
		{
			assert(mCount >= 0);
			--mCount;
			if (mCount <= 0) {
				destroy();
			}
		}
	
		virtual T* getPtr() const { 
			return ((T*)this); 
		}

		virtual void destroy() { 
			if (getPtr() != NULL)
				delete getPtr();
		}

		virtual int getCount() const {
			return mCount;
		}
	
		// Ctor:
		IRefCntImpl() { 
			mCount = 0; 
		}
	};


	// Creation Date	: 2011/11/25 rareboy0112@facebook.com
	// Description		: shared pointer class
	// Last Modified	: 2011/11/29 rareboy0112@facebook.com
	template <typename T> class SVISharedPtr
	{
	private:
		IRefCnt<T> *mRefCnt;

		class RefCnt : public IRefCntImpl<T>
		{
		private:
			T *mData;
		protected:
			virtual T* getPtr() const { 
				return mData; 
			}

			virtual void destroy() { 
				delete this; 
			}

		public:
			RefCnt(T *ptr) { 
				mData = ptr; 
			}

			virtual ~RefCnt() { 
				IRefCntImpl<T>::destroy(); 
			}
		};

		void assign(void *ptr)
		{
			if (ptr == NULL) 
				assign((IRefCnt<T>*)NULL);
			else 
				assign(new RefCnt(static_cast<T*>(ptr)));
		}

		void assign(IRefCnt<T> *refCnt)
		{
			if (refCnt != NULL) 
				refCnt->addRef();
		
			IRefCnt<T> *oldRef = mRefCnt;
			mRefCnt = refCnt;
		
			if (oldRef != NULL) 
				oldRef->release();
		}

	public:
		// Ctor:
		SVISharedPtr() { 
			mRefCnt = NULL; 
		}
	
		SVISharedPtr(T *ptr) { 
			mRefCnt = NULL; 
			assign(ptr); 
		}
	
		SVISharedPtr(const SVISharedPtr &sp) { 
			mRefCnt = NULL; 
			assign(sp.mRefCnt);
		}
	
		// Dtor:
		virtual ~SVISharedPtr() { 
			assign((IRefCnt<T>*)NULL); 
		}
	
		T* getPtr() const { 
			return (mRefCnt == NULL) ? NULL : mRefCnt->getPtr(); 
		}

		int getCount() const {
			return mRefCnt->getCount();
		}

		SVISharedPtr& operator = (const SVISharedPtr &sp) {
			assign(sp.mRefCnt); return *this; 
		}

		SVISharedPtr& operator = (T *ptr) { 
			assign(ptr); return *this; 
		}
	
		T* operator ->() const { 
			assert(getPtr() != NULL); 
			return getPtr(); 
		}
	
		operator T* () const { 
			return getPtr(); 
		}

		bool operator !() { 
			return getPtr() == NULL; 
		}
	
		bool operator ==(const SVISharedPtr &sp) { 
			return getPtr() == sp.getPtr(); 
		}
	
		bool operator ==(const T *t) { 
			return getPtr() == t; 
		}
	
		bool operator !=(const SVISharedPtr &sp)	{ 
			return getPtr() != sp.getPtr(); 
		}
	};

} // end namespace SVI

#endif // #ifndef __SVI_SHAREDPTR_H_