#include "nativeParticles.h"
#include "../SVICores.h"
#include "../Particle/SVIParticles.h"
#include "../Particle/SVIParticleEffectManager.h"
#include "../Animation/SVIAnimation.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeCreateParticles
(JNIEnv *env, jclass clazz, jint surfaceNativeHandle) {

    jint nativeAdress = 0;
    SVIParticles* particles = NULL;

    SVIGLSurface* surface = (SVIGLSurface*) surfaceNativeHandle;

    particles = new SVIParticles(surface); 

    if (particles == 0) {
        LOGE("Can't create particles!");
        return -1;
    }
    else {
        nativeAdress = (jint)particles;
        return nativeAdress;
    }
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetTextureFile
(JNIEnv *env, jclass clazz, jint particlesHandle, jstring textureFileName, jint imageHandle) {
	SVIImage *image = (SVIImage *)imageHandle;

	const char *utf8;
	utf8 = env->GetStringUTFChars(textureFileName, NULL);
	std::string fileName = utf8;

	((SVIParticles*)particlesHandle)->setTextureFileName(fileName, image);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetParticleCount
(JNIEnv *env, jclass clazz, jint particlesHandle, jint particleCount) {
	((SVIParticles*)particlesHandle)->setParticleCount(particleCount);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetMaxDuration
(JNIEnv *env, jclass clazz, jint particlesHandle, jint duration) {
	((SVIParticles*)particlesHandle)->setMaxDuration(duration);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetRandomDuration
(JNIEnv *env, jclass clazz, jint particlesHandle, jint duration) {
	((SVIParticles*)particlesHandle)->setRandomDuration(duration);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetMaxSize
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setMaxSize( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetRandomSize
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setRandomSize( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetDefaultPosition
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setDefaultPosition( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetRandomPosition
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setRandomPosition( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetMaxColor
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVIParticles*)particlesHandle)->setMaxColor( SVIColor(r, g, b, a) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetRandomColor
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVIParticles*)particlesHandle)->setRandomColor( SVIColor(r, g, b, a) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetDefaultGravity
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setDefaultGravity( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetRandomGravity
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setRandomGravity( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetDefaultForce
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setDefaultForce( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetRandomForce
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->setRandomForce( SVIVector3(x, y, z) );
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetMaxMass
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat mass) {
	((SVIParticles*)particlesHandle)->setMaxMass(mass);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetRandomMass
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat mass) {
	((SVIParticles*)particlesHandle)->setRandomMass(mass);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetScaleKeyFrameAnimationProperty
(JNIEnv *env, jclass clazz, jint particlesHandle, jint duration, jint interpolatorType, jint reset) {
	((SVIParticles*)particlesHandle)->setScaleKeyFrameAnimationProperty(duration, (SVIAnimation::InterpolatorType)interpolatorType, reset);
}
	
JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetColorKeyFrameAnimationProperty
(JNIEnv *env, jclass clazz, jint particlesHandle, jint duration, jint interpolatorType, jint reset) {
	((SVIParticles*)particlesHandle)->setColorKeyFrameAnimationProperty(duration, (SVIAnimation::InterpolatorType)interpolatorType, reset);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeSetPositionKeyFrameAnimationProperty
(JNIEnv *env, jclass clazz, jint particlesHandle, jint duration, jint interpolatorType, jint reset) {
	((SVIParticles*)particlesHandle)->setPositionKeyFrameAnimationProperty(duration, (SVIAnimation::InterpolatorType)interpolatorType, reset);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeAddScaleKeyFrame
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat keyTime, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->addScaleKeyFrame( keyTime, SVIVector3(x, y, z) );
}
	
JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeAddColorKeyFrame
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat keyTime, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVIParticles*)particlesHandle)->addColorKeyFrame( keyTime, SVIColor(r, g, b, a) );
}
	
JNIEXPORT void JNICALL Java_com_github_sviengine_particle_SVIParticles_nativeAddPositionKeyFrame
(JNIEnv *env, jclass clazz, jint particlesHandle, jfloat keyTime, jfloat x, jfloat y, jfloat z) {
	((SVIParticles*)particlesHandle)->addPositionKeyFrame( keyTime, SVIVector3(x, y, z) );
}
