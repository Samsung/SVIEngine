#include "nativeGLSurface.h"
#include "../SVICores.h"
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include "../Slide/SVIRenderPartManager.h"
#include "../Render/SVIRenderer.h"
#include "../Render/SVITextureManager.h"
#include "../Particle/SVIParticleEffectManager.h"
#include "../Animation/SVIUnionProperty.h"
#include "../Mathematics/SVIVector3.h"
#include "../Mathematics/SVIVector4.h"
#include "../GLSurface/SVIGLSurface.h"

using namespace SVI;

static const SVIBool DEBUG = SVIFALSE;

static void printGLString(const char *name, GLenum s) {
	const char *v = (const char *) glGetString(s);
	DEBUG_CHECK_LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
	 GLint error;
	 for (error = glGetError(); error; error = glGetError()) {
		DEBUG_CHECK_LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

//Function to Pass SVIGLSurface native handle back to Java
JNIEXPORT jint JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeGetHandle (JNIEnv *env, jclass thiz, jobject instance) {
	DEBUG_CHECK_LOGI("nativeGetHandle");
	jobject jGlobalInstance=(jobject)env->NewGlobalRef(instance);
	if (jGlobalInstance == 0){
	    LOGE("get global ref failed - java/lang/ref/WeakReference");
	}
	
	SVIGLSurface * saGLSurface =new SVIGLSurface(jGlobalInstance);
	saGLSurface->initJVM(env, thiz);
	LOGI("nativeGetHandle end: %d", saGLSurface);
	return (jint)saGLSurface;
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeInitSVIGLSurface (JNIEnv *env, jclass thiz, jint enableTestBed, jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeInitialize");
	
	DEBUG_CHECK_LOGI("nativeInitialize #1");
    SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support	
    

	saGLSurface->getRenderPartManager()->create();//Using SVIGLSurface object to get RenderPartManager Object
	
	saGLSurface->getRenderPartManager()->setWallpaperMode(SVIFALSE);//Using SVIGLSurface object to get RenderPartManager Object
	saGLSurface->getRenderPartManager()->setExternalMode(SVIFALSE);//Using SVIGLSurface object to get RenderPartManager Object

	if( enableTestBed != 0 )
		saGLSurface->getRenderPartManager()->testBedEnable(SVITRUE);//Using SVIGLSurface object to get RenderPartManager Object
	else
		saGLSurface->getRenderPartManager()->testBedEnable(SVIFALSE);//Using SVIGLSurface object to get RenderPartManager Object

	DEBUG_CHECK_LOGI("nativeInitialize end");
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeDeInitSVIGLSurface (JNIEnv *env, jclass thiz, jint nativeHandle) {
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;
	env->DeleteGlobalRef(saGLSurface->mJavaObject);
	saGLSurface->mJavaObject = NULL;
	SVI_SVIFE_DELETE(saGLSurface);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeResizeSVIGLSurface (JNIEnv *env, jclass thiz, jint w, jint h, jint rotation,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeResize w=%d h=%d rotation:%d", w, h, rotation);

	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->resize(w, h, rotation);//Using SVIGLSurface object to get RenderPartManager Object
}

/*
 * Class:     com_github_sviengine_glsurface_SVIGLSurface
 * Method:    nativeRenderSVIGLSurface
 * Signature: ()V
 * @param updateMsg - The update message.
 * @param isGlContextChanged - true if gl context has changed.
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeRenderSVIGLSurface (JNIEnv *env, jclass thiz, jint updateMsg, jboolean isGlContextChanged,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeRender");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	
	//If gl context has changed, update gl context id and restore textures.
	if(isGlContextChanged){
		saGLSurface->getTextureManager()->incrementGLContextId();//Using SVIGLSurface object to get TextureManager Object
		saGLSurface->getRenderPartManager()->restoreImages();//Using SVIGLSurface object to get RenderPartManager Object
	}

	//Render the frame
	saGLSurface->getRenderPartManager()->drawFrame(updateMsg == 1 ? SVITRUE : SVIFALSE);//Using SVIGLSurface object to get RenderPartManager Object

	//Unlock the bitmap pixel buffer and delete the jni global reference for all uploaded textures.
	saGLSurface->getTextureManager()->unlockBitmapBuffers(env);//Using SVIGLSurface object to get TextureManager Object
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeResumeSVIGLSurface (JNIEnv *env, jclass thiz,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeResume");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->resume();//Using SVIGLSurface object to get RenderPartManager Object
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativePauseSVIGLSurface(JNIEnv *env, jclass thiz,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativePause");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->pause();//Using SVIGLSurface object to get RenderPartManager Object
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeForceEndAllAnimationSVIGLSurface(JNIEnv *env, jclass thiz,jint nativeHandle) {
    DEBUG_CHECK_LOGI("nativeForceEndAllAnimation");
    SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
    saGLSurface->getRenderPartManager()->forceEndAllAnimation();//Using SVIGLSurface object to get RenderPartManager Object
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeSetAssetManager
  (JNIEnv *env, jclass clazz, jobject assetManager,jint nativeHandle) {
  
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	// get assetmanager native
      AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);

      if(mgr == NULL) {
          LOGE("nativeSetAssetManager: load AssetManager fail !!!");
      }

      LOGI("nativeSetAssetManager: setAssetManager");
	//saGLSurface->getModelManager()->setAssetManager(mgr);
	saGLSurface->getRenderPartManager()->setAssetManager(mgr);//Using SVIGLSurface object to get RenderPartManager Object
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeSwapAnimationSVIGLSurface(JNIEnv *env, jclass thiz,jint nativeHandle) {
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->swapAnimationSlide();//Using SVIGLSurface object to get RenderPartManager Object
	saGLSurface->getMessageHandler()->swapMessage();//Using SVIGLSurface object to get MessageHandler Object
}	

//2012-01-02 masterkeaton27@gmail.com
JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeEnableShowFPS(JNIEnv * env, jclass thiz, jboolean enable,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->showFPSEnable(enable);//Using SVIGLSurface object to get RenderPartManager Object
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeEnableLogFPS(JNIEnv * env, jclass thiz, jboolean enable,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->logFPSEnable(enable);//Using SVIGLSurface object to get RenderPartManager Object
}

//2012-01-13 masterkeaton27@gmail.com
//enable translucent
JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeEnableTranslucent(JNIEnv * env, jclass thiz, jboolean enable,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->enableTranslucent(enable);//Using SVIGLSurface object to get RenderPartManager Object
}


//2012-04-03 rareboy0112@facebook.com
//enable update listener
JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeEnableUpdateListener(JNIEnv * env, jclass thiz, jboolean enable,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getRenderPartManager()->enableUpdateListener(enable);//Using SVIGLSurface object to get RenderPartManager Object
}


//2012-03-30 masterkeaton27@gmail.com
/*
 * Class:     com_github_sviengine_slide_SVIGLSurface
 * Method:    nativeGetFirstLoadFinished
 * Signature: (V)F
 */
JNIEXPORT jboolean JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeGetFirstLoadFinished(JNIEnv * env, jclass thiz,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	return saGLSurface->getTextureManager()->getFirstLoadFinished();//Using SVIGLSurface object to get TextureManager Object
}

//2012-06-22 masterkeaton27@gmail.com
/*
 * Class:     com_github_sviengine_glsurface_SVIGLSurface
 * Method:    nativeClearRemainTexture
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_glsurface_SVIGLSurface_nativeClearRemainTexture(JNIEnv * env, jclass thiz,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Creating object of SVIGLSurface for Multi-Instance-Support
	saGLSurface->getTextureManager()->finalize();//Using SVIGLSurface object to get TextureManager Object
	saGLSurface->getParticleEffectManager()->finalize();//Using SVIGLSurface object to get ParticleEffectManager Object
}
