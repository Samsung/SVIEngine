#include "nativeAnimation.h"
#include "../SVICores.h"
#include "../SVIDefines.h"
#include "../Animation/SVIAnimationListenerProxy.h"
#include "../Animation/SVIAnimation.h"
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVIKeyFrameAnimation.h"
#include "../Animation/SVITransitionAnimation.h"
#include "../Animation/SVISpriteAnimation.h"

using namespace SVI;

void nativeImplicitOnAnimationEnd(unsigned int listenerID, unsigned int animation) {
	JavaVM* g_VM = ((SVIAnimation*)animation)->getSVIGLSurface()->g_VM;
	
    jclass jNativesCls; 
    jmethodID jMethod;
    JNIEnv *env = NULL;

    if (g_VM != NULL) {
        g_VM->AttachCurrentThread (&env, NULL);

        if( env != NULL ) {
            jNativesCls = (jclass)env->FindClass("com/github/sviengine/slide/SVISlideManager");
            if(jNativesCls == NULL) {
                LOGE("nativeImplicitOnAnimationEnd : NativesCls == NULL");
                return;
            }

            jMethod = env->GetStaticMethodID(jNativesCls, "onSVISlideImplicitAnimationEnd", "(I)V");

            if (jMethod == NULL) {
                LOGE("nativeImplicitOnAnimationEnd : jMethod == NULL");
                return;
            }

            env->CallStaticVoidMethod( jNativesCls, jMethod, listenerID);
        } else {
            LOGE("Failed AttachCurrentThread function in nativeImplicitOnAnimationEnd");
        }
    }
}

void nativeImplicitOnAnimationRepeat(unsigned int listenerID, unsigned int animation) {
	JavaVM* g_VM = ((SVIAnimation*)animation)->getSVIGLSurface()->g_VM;
	
    jclass jNativesCls; 
    jmethodID jMethod;
    JNIEnv *env = NULL;

    if (g_VM != NULL) {
        g_VM->AttachCurrentThread (&env, NULL);

		if( env != NULL ) {
			jNativesCls = (jclass)env->FindClass("com/github/sviengine/slide/SVISlideManager");
			if(jNativesCls == NULL) {
				LOGE("nativeImplicitOnAnimationRepeat : NativesCls == NULL");
				return;
			}

			jMethod = env->GetStaticMethodID(jNativesCls, "onSVISlideImplicitAnimationRepeat", "(I)V");

			if (jMethod == NULL) {
				LOGE("nativeImplicitOnAnimationRepeat : jMethod == NULL");
				return;
			}

			env->CallStaticVoidMethod( jNativesCls, jMethod, listenerID);
		} else {
			LOGE("Failed AttachCurrentThread function in nativeImplicitOnAnimationRepeat");
		}
    }
}

void nativeImplicitOnAnimationStart(unsigned int listenerID, unsigned int animation) {
	JavaVM* g_VM = ((SVIAnimation*)animation)->getSVIGLSurface()->g_VM;
	
    jclass jNativesCls; 
    jmethodID jMethod;
    JNIEnv *env = NULL;

    if (g_VM != NULL) {
        g_VM->AttachCurrentThread (&env, NULL);

		if( env != NULL ) {
			jNativesCls = (jclass)env->FindClass("com/github/sviengine/slide/SVISlideManager");
			if(jNativesCls == NULL) {
				LOGE("nativeImplicitOnAnimationStart : NativesCls == NULL");
				return;
			}

			jMethod = env->GetStaticMethodID(jNativesCls, "onSVISlideImplicitAnimationStart", "(I)V");

			if (jMethod == NULL) {
				LOGE("nativeImplicitOnAnimationStart : jMethod == NULL");
				return;
			}

			env->CallStaticVoidMethod( jNativesCls, jMethod, listenerID);
		} else {
			LOGE("Failed AttachCurrentThread function in nativeImplicitOnAnimationStart");
		}
    }
}

void nativeOnAnimationEnd(unsigned int listenerID, unsigned int animation) {
	JavaVM* g_VM = ((SVIAnimation*)animation)->getSVIGLSurface()->g_VM;
	
    jclass jNativesCls; 
    jmethodID jMethod;
    JNIEnv *env = NULL;

    if (g_VM != NULL) {
        g_VM->AttachCurrentThread (&env, NULL);

		if( env != NULL ) {
			jNativesCls = (jclass)env->FindClass("com/github/sviengine/animation/SVIAnimation");
			if(jNativesCls == NULL) {
				LOGE("nativeOnAnimationEnd : NativesCls == NULL");
				return;
			}

			jMethod = env->GetStaticMethodID(jNativesCls, "onSVIAnimationEnd", "(I)V");

			if (jMethod == NULL) {
				LOGE("nativeOnAnimationEnd : jMethod == NULL");
				return;
			}

			//LOGE("nativeOnAnimationEnd2");
			env->CallStaticVoidMethod( jNativesCls, jMethod, listenerID);
		} else {
			LOGE("Failed AttachCurrentThread function in nativeOnAnimationEnd");
		}
    }
}

void nativeOnAnimationRepeat(unsigned int listenerID, unsigned int animation) {
	JavaVM* g_VM = ((SVIAnimation*)animation)->getSVIGLSurface()->g_VM;
	
    jclass jNativesCls; 
    jmethodID jMethod;
    JNIEnv *env = NULL;

    if (g_VM != NULL) {
        g_VM->AttachCurrentThread (&env, NULL);

		if( env != NULL ) {
			jNativesCls = (jclass)env->FindClass("com/github/sviengine/animation/SVIAnimation");
			if(jNativesCls == NULL) {
				LOGE("nativeOnAnimationRepeat : NativesCls == NULL");
				return;
			}

			jMethod = env->GetStaticMethodID(jNativesCls, "onSVIAnimationRepeat", "(I)V");

			if (jMethod == NULL) {
				LOGE("nativeOnAnimationRepeat : jMethod == NULL");
				return;
			}

			env->CallStaticVoidMethod( jNativesCls, jMethod, listenerID);
		} else {
			LOGE("Failed AttachCurrentThread function in nativeOnAnimationRepeat");
		}
    }
}


void nativeOnAnimationStart(unsigned int listenerID, unsigned int animation) {
	JavaVM* g_VM = ((SVIAnimation*)animation)->getSVIGLSurface()->g_VM;
	
	jclass jNativesCls; 
	jmethodID jMethod;
	JNIEnv *env = NULL;

	if (g_VM != NULL) {
		g_VM->AttachCurrentThread (&env, NULL);

		if( env != NULL ) {
			jNativesCls = (jclass)env->FindClass("com/github/sviengine/animation/SVIAnimation");
			if(jNativesCls == NULL) {
				LOGE("nativeOnAnimationStart : NativesCls == NULL");
				return;
			}

			jMethod = env->GetStaticMethodID(jNativesCls, "onSVIAnimationStart", "(I)V");

			if (jMethod == NULL) {
				LOGE("nativeOnAnimationStart : jMethod == NULL");
				return;
			}

			env->CallStaticVoidMethod( jNativesCls, jMethod, listenerID);
		} else {
			LOGE("Failed AttachCurrentThread function in nativeOnAnimationStart");
		}
    }
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimation_nativeSetInterpolatorSVIAnimation
    (JNIEnv *env, jclass jobject, jint aniHandle, jint interpolatorType) {

    if (aniHandle == 0) {
        LOGE("SVIAnimationJNILib_setInterpolator : aniHandle is null, error!!!");
        return;
    }

    SVIAnimation* animation = (SVIAnimation*)aniHandle;

    animation->setInterpolator((SVIAnimation::InterpolatorType)interpolatorType);
}
					  
JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimation_nativeSetAnimationPropertySVIAnimation
    (JNIEnv *env, jclass jobject, jint aniHandle, jint duration, jint repeatCount, jboolean autoReverse, jint offset) {

    if (aniHandle == 0) {
        LOGE("SVIAnimationJNILib_setAnimationProperty : aniHandle is null, error!!!");
        return;
    }

    SVIAnimation* animation = (SVIAnimation*)aniHandle;
    animation->mDuration = (SVIUInt)duration;
    animation->mRepeatCount = (SVIUInt)repeatCount;
    animation->mAutoReverse = (SVIBool)autoReverse;
    animation->mOffset = (SVIUInt)offset;
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimation_nativeSetListenerSVIAnimation
    (JNIEnv *env, jclass jobject, jint aniHandle, jint listener) {

    if (aniHandle == 0) {
        LOGE("SVIAnimationJNILib_setListener : aniHandle is null, error!!!");
        return;
    }

    SVIAnimation* animation = (SVIAnimation*)aniHandle;
    SVIAnimationListenerProxy* proxy = (SVIAnimationListenerProxy*)listener;

    animation->setListener(proxy);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimation_nativeDeleteSVIAnimation
    (JNIEnv *env, jclass jobject, jint aniHandle, jint classType) {

	SVIAnimation* animation = (SVIAnimation*)aniHandle;
	if( animation->getClassType() == SVIAnimation::ANIMATION_SET ) {
		SVIAnimationSet* aniSet = (SVIAnimationSet*)animation;
		aniSet->clearAnimations();
	} 
	SVI_SVIFE_DELETE(animation);
}

JNIEXPORT jint JNICALL Java_com_github_sviengine_animation_SVIAnimation_nativeCreteProxySVIAnimation
	(JNIEnv *env, jclass jobject, jint proxyType) {

		jint proxyAddress = 0;
		SVIAnimationListenerProxy* pAnimationListenerProxy = NULL;

		pAnimationListenerProxy = new SVIAnimationListenerProxy(proxyType);

		if (pAnimationListenerProxy == 0) {
			LOGE("Can't create animationListenerProxy!");
			return -1;
		}
		else {
			proxyAddress = (jint)pAnimationListenerProxy;
			return proxyAddress;
		}
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimation_nativeDeleteProxySVIAnimation
	(JNIEnv *env, jclass jobject, jint proxyHandle) {

		SVIAnimationListenerProxy* proxy = (SVIAnimationListenerProxy*)proxyHandle;

		SVI_SVIFE_DELETE(proxy);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIAnimation_nativeSetListenerID
	(JNIEnv *env, jclass jobject, jint aniHandle, jint listenerID) {
	if( aniHandle == 0 )
		return;

	SVIAnimation* animation = (SVIAnimation*)aniHandle;
	animation->setID((SVIUInt)listenerID);
}