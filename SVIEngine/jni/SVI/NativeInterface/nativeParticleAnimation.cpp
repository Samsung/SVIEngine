#include "nativeParticleAnimation.h"
#include "../SVICores.h"
#include "../Animation/SVIParticleAnimation.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Particle/SVIKeyFrameParticleEffect.h"

using namespace SVI;

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIParticleAnimation_nativeSetParticleEffect
(JNIEnv *env, jclass clazz, jint nativleHandle, jint effectHandle) {
	if(effectHandle != -1) {
		SVIParticleEffect* newEffect = ( (SVIParticleEffect*)effectHandle )->clone();
		((SVIParticleAnimation*)nativleHandle)->setParticleEffect(newEffect);
	}
}


JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIParticleAnimation_nativeSetKeyFrameParticleEffect
(JNIEnv *env, jclass clazz, jint nativleHandle, jint effectHandle) {
	if(effectHandle != -1) {
		SVIKeyFrameParticleEffect* newEffect = (SVIKeyFrameParticleEffect*)( (SVIKeyFrameParticleEffect*)effectHandle )->clone();
		((SVIParticleAnimation*)nativleHandle)->setParticleEffect(newEffect);
	}
}


JNIEXPORT jint JNICALL Java_com_github_sviengine_animation_SVIParticleAnimation_nativeCreateSVIParticleAnimation
(JNIEnv *env, jclass clazz, jint surfaceHandle) {
	SVIGLSurface* saGLSurface =(SVIGLSurface*)surfaceHandle;

    jint animationAdress = 0;
    SVIParticleAnimation* particleAnimation = NULL;

    particleAnimation = new SVIParticleAnimation(saGLSurface);

    if (particleAnimation == 0) {
        LOGE("Can't create particleAnimation!");
        return -1;
    }
    else {
        animationAdress = (jint)particleAnimation;
        return animationAdress;
    }
}
