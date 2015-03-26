#include "nativeSlideManager.h"

#include "../Slide/SVIRenderPartManager.h"
#include "../Render/SVIRenderPatch.h"
#include "../Animation/SVIAnimationListenerProxy.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_slide_SVISlideManager_nativeIsAnimatingSVISlideManager
  (JNIEnv *env, jclass jobject, jint natSurfaceHandle) {

  	SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
	return surface->getRenderPartManager()->isAnimating() ? 1 : 0;
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlideManager_nativeEnableGlobalAnimation
	(JNIEnv *env, jclass jobject, jint natSurfaceHandle,   jint enableAnimation) {

	SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
	surface->getRenderPartManager()->enableAinmation(enableAnimation == 1 ? SVITRUE : SVIFALSE);
}

JNIEXPORT jint JNICALL Java_com_github_sviengine_slide_SVISlideManager_nativeIsEnableGlobalAnimation
	(JNIEnv *env, jclass jobject, jint natSurfaceHandle ) {
	
	SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
	return surface->getRenderPartManager()->isEnableAinmation() ? 1 : 0;
}

JNIEXPORT jfloat JNICALL Java_com_github_sviengine_slide_SVISlideManager_nativeGetFPS
	(JNIEnv *env, jclass jobject, jint natSurfaceHandle ) {
	
	SVIGLSurface *surface = (SVIGLSurface*)natSurfaceHandle;
	return surface->getRenderPartManager() ->getFPS();
}

JNIEXPORT jint JNICALL Java_com_github_sviengine_slide_SVISlideManager_nativeCheckPageStatus
	(JNIEnv *env, jclass jobject, jint slideHandle) {
	if( slideHandle == 0 )
		return -1;

	SVISlide* slide = (SVISlide*)slideHandle;

	SVIBeamedSlide* beamedSlide = slide->getBeamedSlide();
	if( beamedSlide == NULL )
		return -1;

	SVIProjectionSlide* projectionSlide = beamedSlide->getProjectionSlide();
	if( projectionSlide == NULL )
		return -1;

	SVIRenderPatch* renderPatch = projectionSlide->getRenderPatch();
	if( renderPatch == NULL )
		return -1;

	return renderPatch->getPageStatus();
}

