#include "nativeGLWallpaper.h"
#include "../SVICores.h"
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include "../Slide/SVIRenderPartManager.h"
#include "../Animation/SVIUnionProperty.h"
#include "../Mathematics/SVIVector3.h"
#include "../Mathematics/SVIVector4.h"
#include "../GLSurface/SVIGLSurface.h"

using namespace SVI;

JNIEnv*  gEnv;
JavaVM* g_VM;
static jclass jWGlobalCls;
static jmethodID jWSendMethod;

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

static void initJVM(JNIEnv*  env) {
	gEnv = env;

	jclass jNativesCls;
	env->GetJavaVM(&g_VM);
    g_VM->AttachCurrentThread (&env, NULL);

	jNativesCls = (jclass)env->FindClass("com/github/sviengine/glwallpaperservice/SVIGLWallpaperService");
	jWGlobalCls=(jclass)env->NewGlobalRef(jNativesCls);
	env->DeleteLocalRef(jNativesCls);
	////Changed from Static to Non-Static call back for Multi-Instance-Support
	jWSendMethod = env->GetMethodID(jWGlobalCls, "requestRenderView", "()V"); // _Bonnie
}

void requestRenderGLWallpaper() {
	if( jWSendMethod != 0 ) {
		JNIEnv *env;
		g_VM->AttachCurrentThread (&env, NULL);
		//Changed from Static to Non-Static call back for Multi-Instance-Support
		env->CallVoidMethod( jWGlobalCls, jWSendMethod); //_Bonnie
		//if call this function to detach from VM, program died. i don't know why...
		//g_VM->DetachCurrentThread(); 
	}
}

static void deInitJVM() {

}

//Function to Pass SVIGLSurface native handle back to Java
JNIEXPORT jint JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeGetHandle (JNIEnv *env, jclass thiz, jobject obj) {//Multi-Support-Instance
	DEBUG_CHECK_LOGI("nativeGetHandle");
	SVIGLSurface * saGLSurface =new SVIGLSurface(obj) ;
	DEBUG_CHECK_LOGI("nativeGetHandle end");
	return (jint)saGLSurface;
}
JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeInitSVIGLSurface (JNIEnv *env, jclass thiz, jint enableTestBed,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeInitialize");
	initJVM(env);
	DEBUG_CHECK_LOGI("nativeInitialize #1");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->create();

	saGLSurface->getRenderPartManager()->setExternalMode(SVIFALSE);
	saGLSurface->getRenderPartManager()->setWallpaperMode(SVITRUE);

	if( enableTestBed != 0 )
		saGLSurface->getRenderPartManager()->testBedEnable(SVITRUE);
	else
		saGLSurface->getRenderPartManager()->testBedEnable(SVIFALSE);

	DEBUG_CHECK_LOGI("nativeInitialize end");
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeResizeSVIGLSurface (JNIEnv *env, jclass thiz, jint w, jint h, jint rotation,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeResize w=%d h=%d rotation:%d", w, h, rotation);

	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->resize(w, h, rotation);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeRenderSVIGLSurface (JNIEnv *env, jclass thiz, jint updateMsg,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeRender");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->drawFrame(updateMsg == 1 ? SVITRUE : SVIFALSE);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeResumeSVIGLSurface (JNIEnv *env, jclass thiz,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativeResume");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->resume();
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativePauseSVIGLSurface(JNIEnv *env, jclass thiz,jint nativeHandle) {
	DEBUG_CHECK_LOGI("nativePause");
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->pause();
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeSetAssetManager
  (JNIEnv *env, jclass clazz, jobject assetManager,jint nativeHandle) {
	
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	// get assetmanager native
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);

	if(mgr == NULL) {
		LOGE("nativeSetAssetManager: load AssetManager fail !!!");
	}

	LOGI("nativeSetAssetManager: setAssetManager");
	//saGLSurface->getModelManager()->setAssetManager(mgr);
	saGLSurface->getRenderPartManager()->setAssetManager(mgr);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeSwapAnimationSVIGLSurface(JNIEnv *env, jclass thiz,jint nativeHandle) {
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	//2013-04-20 rareboy0112@facebook.com
    //no need this method.
    //saGLSurface->getRenderPartManager()->swapAnimationSlide();
	saGLSurface->getMessageHandler()->swapMessage();
}	

//2012-01-02 masterkeaton27@gmail.com
JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeEnableShowFPS(JNIEnv * env, jclass thiz, jboolean enable,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->showFPSEnable(enable);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeEnableLogFPS(JNIEnv * env, jclass thiz, jboolean enable,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->logFPSEnable(enable);
}

//2012-01-13 masterkeaton27@gmail.com
//enable translucent
JNIEXPORT void JNICALL Java_com_github_sviengine_glwallpaperservice_SVIWallpaperNative_nativeEnableTranslucent(JNIEnv * env, jclass thiz, jboolean enable,jint nativeHandle){
	SVIGLSurface * saGLSurface =(SVIGLSurface*)nativeHandle;//Multi-Instance-Support
	saGLSurface->getRenderPartManager()->enableTranslucent(enable);
}