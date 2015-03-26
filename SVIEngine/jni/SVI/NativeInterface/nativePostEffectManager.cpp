#include "nativePostEffectManager.h"

#include "../Slide/SVIRenderPartManager.h"
#include "../Render/SVIPostEffectRenderer.h"
#include "../Render/SVIRenderer.h"

using namespace SVI;

extern JNIEnv*  gEnv;
extern JavaVM* g_VM;

//2013-02-27 masterkeaton27@gmail.com
void nativeRippleFinishedCallback(){
    jclass jNativesCls; 
    jmethodID jMethod;
    JNIEnv *env = NULL;

    if (gEnv != NULL) {
        gEnv->GetJavaVM(&g_VM);		
        g_VM->AttachCurrentThread (&env, NULL);

        if( env != NULL ) {
            jNativesCls = (jclass)env->FindClass("com/github/sviengine/posteffect/SVIPostEffectManager");
            if(jNativesCls == NULL) {
                LOGE("nativeRippleFinishedCallback : NativesCls == NULL");
                return;
            }

            jMethod = env->GetStaticMethodID(jNativesCls, "onRippleFinished", "()V");

            if (jMethod == NULL) {
                LOGE("nativeRippleFinishedCallback : jMethod == NULL");
                return;
            }

            env->CallStaticVoidMethod( jNativesCls, jMethod);
        } else {
            LOGE("Failed AttachCurrentThread function in nativeRippleFinishedCallback");
        }
    }
}

/*
 * Class:     com_github_sviengine_posteffect_SVIPostEffectManager
 * Method:    nativeResetRipple
 * Signature: (FFF)V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_posteffect_SVIPostEffectManager_nativeResetRipple
  (JNIEnv * pEnv, jclass jObject, jint natSurfaceHandle){
      SVIMessage msg;
      msg.mSlide = NULL;
      msg.mMessage = SVIMessageHandler::SM_SET_RIPPLE_RESET;
	SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
	surface->getMessageHandler()->sendMessage(msg);
}

/*
 * Class:     com_github_sviengine_posteffect_SVIPostEffectManager
 * Method:    nativeSetRipple
 * Signature: (FFF)V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_posteffect_SVIPostEffectManager_nativeSetRipple
  (JNIEnv * pEnv, jclass jObject, jint natSurfaceHandle,  jfloat x, jfloat y, jfloat intensity){
      
      SVIMessage msg;
      msg.mSlide = NULL;
      msg.mMessage = SVIMessageHandler::SM_SET_RIPPLE_WAVE;
      msg.mParam1 = (SVIInt)(new SVIVector3(x,y,intensity));

      	SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
	surface->getMessageHandler()->sendMessage(msg);
}


/*
 * Class:     com_github_sviengine_posteffect_SVIPostEffectManager
 * Method:    nativeSetRippleInfo
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_posteffect_SVIPostEffectManager_nativeSetRippleInfo
  (JNIEnv * pEnv, jclass jObject, jint natSurfaceHandle,  jfloat ratio, jfloat threshhold){
      SVIMessage msg;
      msg.mSlide = NULL;
      msg.mMessage = SVIMessageHandler::SM_SET_RIPPLE_INFO;
      msg.mParam1 = (SVIInt)(new SVIVector3(ratio, threshhold, 0.0f));

 	SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
	surface->getMessageHandler()->sendMessage(msg);
}



/*
 * Class:     com_github_sviengine_posteffect_SVIPostEffectManager
 * Method:    nativeSetRipple
 * Signature: (FFF)V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_posteffect_SVIPostEffectManager_nativeSetRippleEnvImage
  (JNIEnv * pEnv, jclass jObject, jint natSurfaceHandle, jint imageHandle){


    SVIMessage msg;
    msg.mSlide = NULL;
    msg.mMessage = SVIMessageHandler::SM_SET_RIPPLE_ENVMAP;
    msg.mParam1 = (SVIInt)imageHandle;

    SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
    surface->getMessageHandler()->sendMessage(msg);
}


/*
* Class:     com_github_sviengine_posteffect_SVIPostEffectManager
* Method:    nativeEnable
* Signature: (I)V
*/
JNIEXPORT void JNICALL Java_com_github_sviengine_posteffect_SVIPostEffectManager_nativeEnable
    (JNIEnv * pEnv, jclass jObject, jint natSurfaceHandle,  jint enable){

    /*
    SVIMessage msg;
    msg.mSlide = NULL;
    msg.mMessage = SVIMessageHandler::SM_SET_USEPOSTEFFECT;
    msg.mParam1 = (SVIInt)enable;

    SVIMessageHandler::getInstance()->sendMessage(msg);
    */
    SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
    surface->getRenderer()->mUsePostEffect = enable;
}

/*
* Class:     com_github_sviengine_posteffect_SVIPostEffectManager
* Method:    nativeSetType
* Signature: (I)V
*/
JNIEXPORT void JNICALL Java_com_github_sviengine_posteffect_SVIPostEffectManager_nativeSetType
    (JNIEnv * pEnv, jclass jObject, jint natSurfaceHandle,  jint type){

        /*
    SVIMessage msg;
    msg.mSlide = NULL;
    msg.mMessage = SVIMessageHandler::SM_SET_POSTEFFECT_TYPE;
    msg.mParam1 = (SVIInt)type;

    SVIMessageHandler::getInstance()->sendMessage(msg);*/

    SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;

    if (type == POST_EFFECT_BLOOM){
        surface->getRenderer()->mUsePostRippleEffect= SVIFALSE;
    }else if (type == POST_EFFECT_RIPPLE){
        surface->getRenderer()->mUsePostRippleEffect = SVITRUE;
    }
}


