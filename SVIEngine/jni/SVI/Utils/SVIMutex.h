#ifndef __SVI_MUTEX_H_
#define __SVI_MUTEX_H_

#include <pthread.h>
#include "../BaseType/SVIBaseTypes.h"

namespace SVI {

#define SVIAutoLock(lockObj)		SVIMutex::SVIAutolock lock(lockObj, (SVIChar*)__PRETTY_FUNCTION__);

	class SVIMutex {
	public:
		enum {
			PRIVATE = 0,
			SHARED = 1
		};
    
		SVIMutex(const SVIChar* name);
		SVIMutex(int type, const SVIChar* name = NULL);
		~SVIMutex();

		SVIInt    lock();
		void     unlock();

		SVIInt    tryLock();

		SVIChar*	 getName() { return mName; }	

		class SVIAutolock {
		public:
			SVIAutolock(SVIMutex& mutex);
			SVIAutolock(SVIMutex* mutex);
			SVIAutolock(SVIMutex& mutex, SVIChar* funcName);
			SVIAutolock(SVIMutex* mutex, SVIChar* funcName);
			~SVIAutolock();
		private:
			SVIMutex*	mLock;
		};

	private:
		pthread_mutex_t mMutex;
		SVIChar			mName[128];
	};
	
	typedef SVIMutex::SVIAutolock AutoMutex;

};

#endif // __SVI_MUTEX_H_
