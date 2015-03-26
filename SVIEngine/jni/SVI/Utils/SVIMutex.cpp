#include "SVIMutex.h"
#include "../SVICores.h"

namespace SVI {

	static const SVIBool	DEBUG = SVIFALSE;

	SVIMutex::SVIMutex(const SVIChar* name) {
		if( pthread_mutex_init(&mMutex, NULL) == -1 )
			LOGE("Mutex initialization failed name: %s", name);

		memset(mName, 0x00, sizeof(mName));
		if( name != NULL ) {
			snprintf(mName, 128, "%s", name);
		} else {
			sprintf(mName, "Default");
		}
	}

	SVIMutex::SVIMutex(int type, const SVIChar* name) {
		if (type == SHARED) {
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
			pthread_mutex_init(&mMutex, &attr);
			pthread_mutexattr_destroy(&attr);
		} else {
			pthread_mutex_init(&mMutex, NULL);
		}
	}

	SVIMutex::~SVIMutex() {
		pthread_mutex_destroy(&mMutex);
	}

	SVIInt SVIMutex::lock() {
		return pthread_mutex_lock(&mMutex);
	}

	void SVIMutex::unlock() {
		pthread_mutex_unlock(&mMutex);
	}

	SVIInt SVIMutex::tryLock() {
		return pthread_mutex_trylock(&mMutex);
	}

	SVIMutex::SVIAutolock::SVIAutolock(SVIMutex& mutex) : mLock(&mutex)  { 
		DEBUG_CHECK_LOGI("AutoLock Lock : %s", mLock->getName());
		mLock->lock(); 
		DEBUG_CHECK_LOGI("AutoLock Locked: %s", mLock->getName());
	}

	SVIMutex::SVIAutolock::SVIAutolock(SVIMutex* mutex) : mLock(mutex) { 
		DEBUG_CHECK_LOGI("AutoLock Lock : %s", mLock->getName());
		mLock->lock(); 
		DEBUG_CHECK_LOGI("AutoLock Locked : %s", mLock->getName());
	}

	SVIMutex::SVIAutolock::SVIAutolock(SVIMutex& mutex, SVIChar* funcName) : mLock(&mutex)  { 
		DEBUG_CHECK_LOGI("AutoLock Lock : %s", mLock->getName());
		mLock->lock(); 
		DEBUG_CHECK_LOGI("AutoLock Locked: %s", mLock->getName());
	}

	SVIMutex::SVIAutolock::SVIAutolock(SVIMutex* mutex, SVIChar* funcName) : mLock(mutex) { 
		DEBUG_CHECK_LOGI("AutoLock Lock : %s, %s", mLock->getName(), funcName);
		mLock->lock(); 
		DEBUG_CHECK_LOGI("AutoLock Locked : %s, %s", mLock->getName(), funcName);
	}

	SVIMutex::SVIAutolock::~SVIAutolock() { 
		DEBUG_CHECK_LOGI("AutoLock unLock %s", mLock->getName());
		mLock->unlock(); 
		DEBUG_CHECK_LOGI("AutoLock unLocked %s", mLock->getName());
	}

} //namespace SVI