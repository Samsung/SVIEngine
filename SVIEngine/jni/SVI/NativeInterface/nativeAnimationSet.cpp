#include "nativeAnimationSet.h"
#include "../SVICores.h"
#include "../SVIDefines.h"
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIAnimationListenerProxy.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_animation_SVIAnimationSet_nativeCreateSVIAnimationSet (JNIEnv *env, jclass jobject, jint surfaceHandle) {
	//LOGI("SVIAnimationSetJNILib_create");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)surfaceHandle;
	
	jint animationSetAdress = 0;
	SVIAnimationSet* animationSet = NULL;
	
	animationSet = new SVIAnimationSet(saGLSurface);

	if (animationSet == 0) {
		LOGE("Can't create basicAnimation!");
		return -1;
	}
	else {
		animationSetAdress = (jint)animationSet;
		return animationSetAdress;
	}
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimationSet_nativeShareAnimationInfo (JNIEnv *env, jclass jobject, jint aniSetHandle, jint shareInfo) {
	SVIAnimationSet* aniSet = (SVIAnimationSet*)aniSetHandle;

	if( aniSet != NULL )
		aniSet->shareAnimationInfo(shareInfo==1?SVITRUE:SVIFALSE);
}

JNIEXPORT jboolean JNICALL Java_com_github_sviengine_animation_SVIAnimationSet_nativeAddAnimationSVIAnimationSet (JNIEnv *env, jclass jobject, jint aniSet, jint aniHandle) {

	if (aniSet == 0 || aniHandle == 0) {
		LOGE("SVIAnimationSetJNILib_addAnimation : error!! aniSet == 0 || aniHandle == 0");
		return SVIFALSE;
	}

	SVIAnimationSet* animationSet = (SVIAnimationSet*)aniSet;
	SVIAnimation* animation = (SVIAnimation*)aniHandle;

	return (animationSet->addAnimation(animation));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimationSet_nativeRemoveAnimationSVIAnimationSet (JNIEnv *env, jclass jobject, jint aniSet, jint aniHandle) {

	if (aniSet == 0 || aniHandle == 0) {
		LOGE("SVIAnimationSetJNILib_removeAnimation : error!! aniSet == 0 || aniHandle == 0");
		return;
	}
	
	//LOGI("SVIAnimationSetJNILib_removeAnimation");

	SVIAnimationSet* animationSet = (SVIAnimationSet*)aniSet;
	SVIAnimation* animation = (SVIAnimation*)aniHandle;

	animationSet->removeAnimation(animation);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimationSet_nativeSetPropertySVIAnimationSet 
	(JNIEnv *env, jclass jobject, jint aniSet, jint duration, jint repeatCount, jboolean autoReverse, jint offset) {
	//LOGI("SVIAnimationSetJNILib_setAnimationSetProperty");

	SVIAnimationSet* animationSet = (SVIAnimationSet*)aniSet;

	animationSet->mDuration = (SVIUInt)duration;
	animationSet->mRepeatCount = (SVIUInt)repeatCount;
	animationSet->mAutoReverse = (SVIBool)autoReverse;
	animationSet->mOffset = (SVIUInt)offset;
}