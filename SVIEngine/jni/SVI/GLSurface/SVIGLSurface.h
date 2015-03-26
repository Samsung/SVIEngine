#pragma once

#include <jni.h>

namespace SVI {

	class SVISlideCapture;
	class SVIPostEffectRenderer;
	class SVIRenderPartManager;
	class SVIMessageHandler;
	class SVIParticleEffectManager;
	class SVIModelManager;
	class SVIViewport;
	class SVIProgramManager;
	class SVIDebugRenderer;
	class SVICommonShaderVariables;
	class SVIRippleRenderer;
	class SVIRenderer;
	class SVIParticleRenderer;
	class SVITextureManager;


	class SVIGLSurface
	{
		static const SVIBool DEBUG = SVIFALSE;

	public:
		JavaVM* g_VM;

		jmethodID jGetMethod;
		jmethodID jReqRenderMethod;
		jmethodID jNatiUpdateMethod;

		jobject mJavaObject;
		
		SVISlideCapture* mSVISlideCapture;
		SVIPostEffectRenderer* mSVIPostEffectRenderer;
		SVIRenderPartManager* mSVIRenderPartManager;
		SVIMessageHandler* mSVIMessageHandler;
		SVIParticleEffectManager* mSVIParticleEffectManager;
		SVIModelManager* mSVIModelManager;
		SVIParticleRenderer* mSVIParticleRenderer;
		SVIViewport*		mSVIViewport;
		SVIProgramManager* mSVIProgramManager;
		SVIDebugRenderer* mSVIDebugRenderer;
		SVICommonShaderVariables* mSVICmnShdrVars;
		SVIRippleRenderer* mSVIRippleRenderer;
		SVIRenderer*		mSVIRenderer;
		SVITextureManager* mSVITextureManager;
		

		SVIGLSurface(jobject obj);
		~SVIGLSurface();

		SVISlideCapture* getSlideCapture();
		SVIPostEffectRenderer* getPostEffectRenderer();
		SVIRenderPartManager* getRenderPartManager();
		SVIMessageHandler* getMessageHandler();
		SVIParticleEffectManager* getParticleEffectManager();
		SVIModelManager* getModelManager();
		SVIParticleRenderer* getParticleRenderer();
		SVIViewport* getViewport();
		SVIProgramManager* getProgramManager();
		SVIDebugRenderer* getDebugRenderer();
		SVICommonShaderVariables* getCommonShaderVariables();
		SVIRippleRenderer* getRippleRenderer();
		SVIRenderer* getRenderer();
		SVITextureManager* getTextureManager();		

		void initJVM(JNIEnv*  env, jclass obj);
		void requestRenderGLSurface(jobject obj);
		void nativeUpdate(jobject obj);
		void deInitJVM();
	};
}
