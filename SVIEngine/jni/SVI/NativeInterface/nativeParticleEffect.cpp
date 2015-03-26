#include "nativeParticleEffect.h"
#include "../SVICores.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Animation/SVIAnimation.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeCreateParticleEffect
(JNIEnv *env, jclass clazz, jint surfaceNativeHandle) {

    jint nativeAdress = 0;
    SVIParticleEffect* effect = NULL;
	SVIGLSurface *surface = (SVIGLSurface*)surfaceNativeHandle;

    effect = new SVIParticleEffect(surface);

    if (effect == 0) {
        LOGE("Can't create particleEffect!");
        return -1;
    }
    else {
        nativeAdress = (jint)effect;
        return nativeAdress;
    }
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeAddParticles
(JNIEnv *env, jclass clazz, jint effectHandle, jint particlesHandle) {
	if(particlesHandle != -1) {
		((SVIParticleEffect*)effectHandle)->addParticles( (SVIParticles*)particlesHandle );
	}
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeSetDuration
(JNIEnv *env, jclass clazz, jint effectHandle, jint duration) {
	((SVIParticleEffect*)effectHandle)->setDuration(duration);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeSetPosition
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticleEffect*)effectHandle)->setPosition( SVIVector3(x, y, z) );
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeSetScale
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticleEffect*)effectHandle)->setScale( SVIVector3(x, y, z) );
}
	
JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeSetColor
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVIParticleEffect*)effectHandle)->setColor( SVIColor(r, g, b, a) );
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeSetScaleKeyFrameAnimationProperty
(JNIEnv *env, jclass clazz, jint effectHandle, jint duration, jint interpolatorType, jint reset) {
	((SVIParticleEffect*)effectHandle)->setScaleKeyFrameAnimationProperty(duration, (SVIAnimation::InterpolatorType)interpolatorType, reset);
}
	
JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeSetColorKeyFrameAnimationProperty
(JNIEnv *env, jclass clazz, jint effectHandle, jint duration, jint interpolatorType, jint reset) {
	((SVIParticleEffect*)effectHandle)->setColorKeyFrameAnimationProperty(duration, (SVIAnimation::InterpolatorType)interpolatorType, reset);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeSetPositionKeyFrameAnimationProperty
(JNIEnv *env, jclass clazz, jint effectHandle, jint duration, jint interpolatorType, jint reset) {
	((SVIParticleEffect*)effectHandle)->setPositionKeyFrameAnimationProperty(duration, (SVIAnimation::InterpolatorType)interpolatorType, reset);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeAddScaleKeyFrame
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat keyTime, jfloat x, jfloat y, jfloat z) {
	((SVIParticleEffect*)effectHandle)->addScaleKeyFrame( keyTime, SVIVector3(x, y, z) );
}
	
JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeAddColorKeyFrame
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat keyTime, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVIParticleEffect*)effectHandle)->addColorKeyFrame( keyTime, SVIColor(r, g, b, a) );
}
	
JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticleEffect_nativeAddPositionKeyFrame
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat keyTime, jfloat x, jfloat y, jfloat z) {
	((SVIParticleEffect*)effectHandle)->addPositionKeyFrame( keyTime, SVIVector3(x, y, z) );
}
