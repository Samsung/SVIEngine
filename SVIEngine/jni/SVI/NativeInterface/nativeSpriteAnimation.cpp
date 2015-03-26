#include "nativeSpriteAnimation.h"
#include "../SVICores.h"
#include "../Animation/SVISpriteAnimation.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_animation_SVISpriteAnimation_nativeCreateSVISpriteAnimation
	(JNIEnv * env, jclass jobject, jint playType, jint imageHandle, jint frameWidth, jint frameHeight, jint surfaceHandle) {
	SVIGLSurface* saGLSurface =(SVIGLSurface*)surfaceHandle;
	
	SVIImage* image = (SVIImage*)imageHandle;
	if( image == NULL ) {
		LOGE("Can not create SVISpriteAnimation instance. Because image handle is NULL !!!");
		return (jint)0;
	}

	SVISpriteAnimation::PlayType type = SVISpriteAnimation::PLAY_ALL;
	if( playType == 1 )
		type = SVISpriteAnimation::PLAY_PARTIAL;
	SVISpriteAnimation* spriteAnimation = SVISpriteAnimation::create(type, saGLSurface);

	spriteAnimation->setImage(image);
	spriteAnimation->setFrameInfo(frameWidth, frameHeight);

	return (jint)spriteAnimation;
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVISpriteAnimation_nativeSetIntervalSVISpriteAnimation(JNIEnv * env, jclass jobject, jint nativeHandle, jint fromIndex, jint toIndex) {
	SVISpriteAnimation* spriteAnimation = (SVISpriteAnimation*)nativeHandle;

	if( spriteAnimation == NULL ) {
		LOGE("SpriteAnimation instance is NULL !!!");
		return;
	}

	spriteAnimation->setPlayInterval(fromIndex, toIndex);
}
		