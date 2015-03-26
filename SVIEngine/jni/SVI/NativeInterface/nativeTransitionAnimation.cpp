#include "nativeTransitionAnimation.h"
#include "../SVICores.h"
#include "../SVIDefines.h"
#include "../Animation/SVITransitionAnimation.h"
#include "../Animation/SVIAnimationListenerProxy.h"

using namespace SVI;

static const SVIBool DEBUG = SVIFALSE;

JNIEXPORT jint JNICALL Java_com_github_sviengine_animation_SVITransitionAnimation_nativeCreateSVITransitionAnimation__II
	(JNIEnv *env, jclass jobject, jint type, jint surfaceHandle) {
	
	DEBUG_CHECK_LOGI("%s : into Create SVITransitionAnimation Method", "nativeTransitionAnimation");
	SVIGLSurface* saGLSurface =(SVIGLSurface*)surfaceHandle;

	jint transAnimationAddress = 0;
	SVITransitionAnimation* transAni = NULL;
	transAni = (SVITransitionAnimation *)SVITransitionAnimation::createAnimation((SVITransitionAnimation::TransitionType)type, saGLSurface);
	
	if (transAni == 0) {
		LOGE("Can't create basicAnimation!");
		return -1;
	}
	else {
		transAnimationAddress = (jint)transAni;
		DEBUG_CHECK_LOGI("%s : transAnimationAddress(%d), type(%d)", "nativeCreateSVITransitionAnimation", transAnimationAddress, type);
		return transAnimationAddress;
	}
}




JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVITransitionAnimation_nativeSetDirectionTypeSVITransitionAnimation
  (JNIEnv *env, jclass jobject, jint aniHandle, jint type) {

	SVITransitionAnimation* tansAni = (SVITransitionAnimation*) aniHandle;
	
	tansAni->setDirectionType( SVITransitionAnimation::DirectionType(type) );
}
