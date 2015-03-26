#include "SVIAnimationListenerProxy.h"
#include "../NativeInterface/nativeAnimation.h"
#include "../Slide/SVIBeamedSlide.h"

namespace SVI {

	SVIAnimationListenerProxy::SVIAnimationListenerProxy() {
		mType = EXPLICIT;
	}

	SVIAnimationListenerProxy::SVIAnimationListenerProxy(SVIInt type) {
		mType = type;
	}

	SVIAnimationListenerProxy::~SVIAnimationListenerProxy() {
	}

	void SVIAnimationListenerProxy::onAnimationEnd(SVIAnimation* animation) {
		if (mType == EXPLICIT) 
			nativeOnAnimationEnd(animation->getID(), (unsigned int)animation);
		else if (mType == IMPLICIT) 
			nativeImplicitOnAnimationEnd(animation->getID(), (unsigned int)animation);
	}

	void SVIAnimationListenerProxy::onAnimationRepeat(SVIAnimation* animation) {
			if (mType == EXPLICIT)
				nativeOnAnimationRepeat(animation->getID(), (unsigned int)animation);
			else if (mType == IMPLICIT)
				nativeImplicitOnAnimationRepeat(animation->getID(), (unsigned int)animation);
	}

	void SVIAnimationListenerProxy::onAnimationStart(SVIAnimation* animation) {
		if (mType == EXPLICIT)
			nativeOnAnimationStart(animation->getID(), (unsigned int)animation);
		else if (mType == IMPLICIT)
			nativeImplicitOnAnimationStart(animation->getID(), (unsigned int)animation);
	}
}