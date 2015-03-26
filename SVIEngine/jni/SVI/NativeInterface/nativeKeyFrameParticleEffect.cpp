#include "nativeKeyFrameParticleEffect.h"
#include "../SVICores.h"
#include "../Particle/SVIKeyFrameParticleEffect.h"
#include "../Animation/SVIAnimation.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_particle_SVIKeyFrameParticleEffect_nativeCreateKeyFrameParticleEffect
(JNIEnv *env, jclass clazz) {

    jint nativeAdress = 0;
    SVIKeyFrameParticleEffect* effect = NULL;

    effect = new SVIKeyFrameParticleEffect();

    if (effect == 0) {
        LOGE("Can't create keyFrameParticleEffect!");
        return -1;
    }
    else {
        nativeAdress = (jint)effect;
        return nativeAdress;
    }
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIKeyFrameParticleEffect_nativeAddParticles
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat keyTime, jint particlesHandle) {
	if(particlesHandle != -1) {
		((SVIKeyFrameParticleEffect*)effectHandle)->addParticles( keyTime, (SVIParticles*)particlesHandle );
	}
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIKeyFrameParticleEffect_nativeSetGeneratorPositionKeyFrameAnimationProperty
(JNIEnv *env, jclass clazz, jint effectHandle, jint duration, jint interpolatorType, jint reset) {
	((SVIKeyFrameParticleEffect*)effectHandle)->setGeneratorPositionKeyFrameAnimationProperty(duration, (SVIAnimation::InterpolatorType)interpolatorType, reset);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIKeyFrameParticleEffect_nativeAddGeneratorPositionKeyFrame
(JNIEnv *env, jclass clazz, jint effectHandle, jfloat keyTime, jfloat x, jfloat y, jfloat z) {
	((SVIKeyFrameParticleEffect*)effectHandle)->addGeneratorPositionKeyFrame( keyTime, SVIVector3(x, y, z) );
}
