//#include "nativeGLExternalSurface.h"
//#include "../SVICores.h"
//#include <android/asset_manager_jni.h>
//#include <android/bitmap.h>
//#include "../Slide/SVIRenderPartManager.h"
//#include "../Animation/SVIUnionProperty.h"
//#include "../Mathematics/SVIVector3.h"
//#include "../Mathematics/SVIVector4.h"
//
//using namespace SVI;
//
//JNIEnv*  gEnv;
//JavaVM* g_VM;
//static jclass jExGlobalCls;
//static jmethodID jExSendMethod;
//
//static const SVIBool DEBUG = SVIFALSE;
//
//static void printGLString(const char *name, GLenum s) {
//    const char *v = (const char *) glGetString(s);
//    DEBUG_CHECK_LOGI("GL %s = %s\n", name, v);
//}
//
//static void checkGlError(const char* op) {
//     GLint error;
//     for (error = glGetError(); error; error = glGetError()) {
//        DEBUG_CHECK_LOGI("after %s() glError (0x%x)\n", op, error);
//    }
//}
//
//static void initJVM(JNIEnv*  env) {
//	gEnv = env;
//
//	jclass jNativesCls;
//	env->GetJavaVM(&g_VM);
//	jNativesCls = (jclass)env->FindClass("com/github/sviengine/glexternalsurface/SVIGLExternalSurface");
//	jExGlobalCls=(jclass)env->NewGlobalRef(jNativesCls);
//	env->DeleteLocalRef(jNativesCls);
//	jExSendMethod = env->GetStaticMethodID(jExGlobalCls, "requestRenderView", "()V");
//}
//
//void requestRenderGLExternalSurface() {
//	if( jExSendMethod != 0 ) {
//		JNIEnv *env;
//		g_VM->AttachCurrentThread (&env, NULL);
//		env->CallStaticVoidMethod( jExGlobalCls, jExSendMethod);
//		//if call this function to detach from VM, program died. i don't know why...
//		//g_ExVM->DetachCurrentThread();
//	}
//}
//
//static void deInitJVM() {
//
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeInitSVIGLExternalSurface (JNIEnv *env, jclass thiz, jint surfaceNativeHandle, jint enableTestBed) {
//	DEBUG_CHECK_LOGI("nativeInitialize");
//	initJVM(env);
//	DEBUG_CHECK_LOGI("nativeInitialize #1");
//
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//
//
//	surface->getRenderPartManager()->create();
//	surface->getRenderPartManager()->setWallpaperMode(SVIFALSE);
//	surface->getRenderPartManager()->setExternalMode(SVITRUE);
//
//
//	if( enableTestBed != 0 )
//		surface->getRenderPartManager()->testBedEnable(SVITRUE);
//	else
//		surface->getRenderPartManager()->testBedEnable(SVIFALSE);
//
//	DEBUG_CHECK_LOGI("nativeInitialize end");
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeResizeSVIGLExternalSurface (JNIEnv *env, jclass thiz, jint surfaceNativeHandle, jint w, jint h, jint rotation) {
//	DEBUG_CHECK_LOGI("nativeResize w=%d h=%d rotation:%d", w, h, rotation);
//
//	printGLString("Version", GL_VERSION);
//	printGLString("Vendor", GL_VENDOR);
//	printGLString("Renderer", GL_RENDERER);
//	printGLString("Extensions", GL_EXTENSIONS);
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//
//	surface->getRenderPartManager()->resize(w, h, rotation);
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeRenderSVIGLExternalSurface (JNIEnv *env, jclass thiz, jint surfaceNativeHandle,jint updateMsg) {
//	DEBUG_CHECK_LOGI("nativeRender");
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//	surface->getRenderPartManager()->drawFrame(updateMsg == 1 ? SVITRUE : SVIFALSE);
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeResumeSVIGLExternalSurface (JNIEnv *env, jclass thiz, jint surfaceNativeHandle) {
//	DEBUG_CHECK_LOGI("nativeResume");
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//	surface->getRenderPartManager()->resume();
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativePauseSVIGLExternalSurface(JNIEnv *env, jclass thiz, jint surfaceNativeHandle) {
//	DEBUG_CHECK_LOGI("nativePause");
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//	surface->getRenderPartManager()->pause();
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeSetAssetManager
//  (JNIEnv *env, jclass clazz, jint surfaceNativeHandle, jobject assetManager) {
//
//	// get assetmanager native
//	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
//
//	if(mgr == NULL) {
//		LOGE("nativeSetAssetManager: load AssetManager fail !!!");
//	}
//
//	LOGI("nativeSetAssetManager: setAssetManager");
//	//SVIModelManager::getInstance()->setAssetManager(mgr);
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//	surface->getRenderPartManager()->setAssetManager(mgr);
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeSwapAnimationSVIGLExternalSurface(JNIEnv *env, jclass thiz,jint surfaceNativeHandle) {
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//	surface->getRenderPartManager()->swapAnimationSlide();
//	surface->getMessageHandler()->swapMessage();
//}
//
////2012-01-02 masterkeaton27@gmail.com
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeEnableShowFPS(JNIEnv * env, jclass thiz,jint surfaceNativeHandle, jboolean enable){
//SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//	surface->getRenderPartManager()->showFPSEnable(enable);
//}
//
//JNIEXPORT void JNICALL Java_com_github_sviengine_glexternalsurface_SVIGLExternalSurface_nativeEnableLogFPS(JNIEnv * env, jclass thiz, jint surfaceNativeHandle, jboolean enable){
//	SVIGLSurface *surface = (SVIGLSurface*) surfaceNativeHandle;
//	surface->getRenderPartManager()->logFPSEnable(enable);
//}
