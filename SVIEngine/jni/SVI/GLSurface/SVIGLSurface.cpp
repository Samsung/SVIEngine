#include "../SVIDefines.h"
#include "../SVICores.h"

#include "../Render/SVIFrameRenderer.h"
#include "../Render/SVIPostEffectRenderer.h"
#include "../Slide/SVIRenderPartManager.h"
#include "../Particle/SVIParticleEffectManager.h"
#include "../Render/SVIParticleRenderer.h"
#include "../Render/SVIViewport.h"
#include "../Render/SVITextureManager.h"
#include "../Render/SVIProgramManager.h"
#include "../Render/SVIDebugRenderer.h"
#include "../Render/SVIRenderer.h"
#include "../Render/SVIModelManager.h"

using namespace SVI;

SVIGLSurface::SVIGLSurface(jobject obj) {		
	mJavaObject = obj;
	mSVISlideCapture = NULL;
	mSVIPostEffectRenderer = NULL;
	mSVIRenderPartManager = NULL;
	mSVIMessageHandler = NULL;
	mSVIParticleEffectManager = NULL;
	mSVIModelManager = NULL;
	mSVIParticleRenderer = NULL;
	mSVIViewport = NULL;
	mSVIProgramManager = NULL;
	mSVIDebugRenderer = NULL; 
	mSVICmnShdrVars = NULL;
	mSVIRippleRenderer = NULL;
	mSVIRenderer = NULL;
	mSVITextureManager = NULL;

	g_VM = 0;
	jGetMethod = 0;
	jReqRenderMethod = 0;
	jNatiUpdateMethod = 0;
}

SVIGLSurface::~SVIGLSurface(){
	SVI_SVIFE_DELETE(mJavaObject);
	SVI_SVIFE_DELETE(mSVIPostEffectRenderer);
	
	SVI_SVIFE_DELETE(mSVIMessageHandler);//Delete SVIRenderPartManager after SVIMessageHandler.
	SVI_SVIFE_DELETE(mSVIRenderPartManager );
	
	SVI_SVIFE_DELETE(mSVIParticleEffectManager );
	SVI_SVIFE_DELETE(mSVIModelManager );
	SVI_SVIFE_DELETE(mSVIParticleRenderer);
	SVI_SVIFE_DELETE(mSVIViewport);
	SVI_SVIFE_DELETE(mSVIProgramManager );
	SVI_SVIFE_DELETE(mSVIDebugRenderer); 
	SVI_SVIFE_DELETE(mSVICmnShdrVars );
	SVI_SVIFE_DELETE(mSVIRippleRenderer);
	SVI_SVIFE_DELETE(mSVIRenderer );
	SVI_SVIFE_DELETE(mSVITextureManager );
	SVI_SVIFE_DELETE(mSVISlideCapture);
	g_VM = 0;
	jGetMethod = 0;
	jReqRenderMethod = 0;
	jNatiUpdateMethod = 0;
	mJavaObject = 0;
}

SVISlideCapture* SVIGLSurface::getSlideCapture()
{
	if (mSVISlideCapture == NULL) 
		mSVISlideCapture = new SVISlideCapture(this);
	return mSVISlideCapture;
}

SVIPostEffectRenderer* SVIGLSurface::getPostEffectRenderer()
{
	if (mSVIPostEffectRenderer == NULL) 
		mSVIPostEffectRenderer = new SVIPostEffectRenderer(this);
	return mSVIPostEffectRenderer;
}

SVIRenderPartManager* SVIGLSurface::getRenderPartManager()
{
	if (mSVIRenderPartManager == NULL) 
		mSVIRenderPartManager = new SVIRenderPartManager(this);
	return mSVIRenderPartManager;
}

SVIMessageHandler* SVIGLSurface::getMessageHandler()
{
	if (mSVIMessageHandler == NULL) 
		mSVIMessageHandler = new SVIMessageHandler(this);
	return mSVIMessageHandler;
}

SVIParticleEffectManager* SVIGLSurface::getParticleEffectManager()
{
	if (mSVIParticleEffectManager == NULL) 
		mSVIParticleEffectManager = new SVIParticleEffectManager(this);
	return mSVIParticleEffectManager;
}

SVIModelManager* SVIGLSurface::getModelManager()
{
	if (mSVIModelManager == NULL) 
		mSVIModelManager = new SVIModelManager(this);
	return mSVIModelManager;
}

SVIParticleRenderer* SVIGLSurface::getParticleRenderer()
{
	if (mSVIParticleRenderer == NULL) 
		mSVIParticleRenderer = new SVIParticleRenderer(this);
	return mSVIParticleRenderer;
}

SVIViewport* SVIGLSurface::getViewport()
{
	if (mSVIViewport == NULL) 
		mSVIViewport = new SVIViewport(this);
	return mSVIViewport;
}

SVIProgramManager* SVIGLSurface::getProgramManager()
{
	if (mSVIProgramManager == NULL) 
		mSVIProgramManager = new SVIProgramManager(this);
	return mSVIProgramManager;
}

SVIDebugRenderer* SVIGLSurface::getDebugRenderer()
{
	if (mSVIDebugRenderer == NULL) 
		mSVIDebugRenderer = new SVIDebugRenderer(this);
	return mSVIDebugRenderer;
}

SVICommonShaderVariables* SVIGLSurface::getCommonShaderVariables() {
	if (mSVICmnShdrVars == NULL) 
		mSVICmnShdrVars = new SVICommonShaderVariables();
	return mSVICmnShdrVars;
}

SVIRippleRenderer* SVIGLSurface::getRippleRenderer() {
	if (mSVIRippleRenderer == NULL) 
		mSVIRippleRenderer = new SVIRippleRenderer(this);
	return mSVIRippleRenderer;
}

SVIRenderer* SVIGLSurface::getRenderer(){
	if (mSVIRenderer == NULL) 
		mSVIRenderer = new SVIRenderer(this);
	return mSVIRenderer;
}

SVITextureManager* SVIGLSurface::getTextureManager() {
	if (mSVITextureManager == NULL) {
		mSVITextureManager = new SVITextureManager(this);
	}
	return mSVITextureManager;
}

void SVIGLSurface::initJVM(JNIEnv*  env, jclass clz) {
	env->GetJavaVM(&g_VM);
	g_VM->AttachCurrentThread (&env, NULL);

	jclass jWeakRefCls = (jclass)env->FindClass("java/lang/ref/WeakReference");
	if (jWeakRefCls == 0) {
		LOGE("Find class failed - java/lang/ref/WeakReference");
	}

	jGetMethod = env->GetMethodID(jWeakRefCls, "get",  "()Ljava/lang/Object;");
	if (jGetMethod == 0){
		LOGE("get method failed - get");
	}

	jclass jSurfaceCls = clz;
	
	jReqRenderMethod = env->GetMethodID(jSurfaceCls, "requestRenderView", "()V"); 
	if (jReqRenderMethod == 0){
		LOGE("get method failed - requestRenderView");
	}

	jNatiUpdateMethod = env->GetMethodID(jSurfaceCls, "nativeUpdate", "()V");
	if (jNatiUpdateMethod == 0){
		LOGE("get method failed - nativeUpdate");
	}

	LOGE("JVM Initialized (2013-09-16 GMT+8)");
}

void SVIGLSurface::requestRenderGLSurface(jobject obj) {
	if( g_VM != 0 ) {
		JNIEnv *env;
		g_VM->AttachCurrentThread (&env, NULL);		

		jobject surface = env->CallObjectMethod(obj, jGetMethod);
		env->CallVoidMethod( surface, jReqRenderMethod);
		//if call this function to detach from VM, program died. i don't know why...
		//g_VM->DetachCurrentThread(); 
	}
}

void SVIGLSurface::nativeUpdate(jobject obj) {
	if( g_VM != 0 ) {
		JNIEnv *env;
		g_VM->AttachCurrentThread (&env, NULL);		
		if( env != NULL ) {
	
			jobject surface = env->CallObjectMethod(obj, jGetMethod);
			env->CallVoidMethod( surface, jNatiUpdateMethod);
		//if call this function to detach from VM, program died. i don't know why...
		//g_VM->DetachCurrentThread(); 
		}
		else {
			LOGE("Failed AttachCurrentThread function in nativeUpdate");
		}
	}
}

void SVIGLSurface::deInitJVM() {

}

