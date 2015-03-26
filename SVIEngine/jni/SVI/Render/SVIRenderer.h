#pragma once
#include "../GLSurface/SVIGLSurface.h"
namespace SVI{
	
#define USE_STENCIL_BUFFER 1

	class SVIRenderScene;
	class SVIProjectionSlide;
	class SVIRenderPatch;
	
	class SVIRenderer{
	public:
		SVIRenderer(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		~SVIRenderer();
		void setRootSlide(SVIProjectionSlide * pRoot);
		void requestRender();
		void renderSlideScene(SVIProjectionSlide * pSlide);

	public:
		void initialize();
		SVIUInt finalize();
		void resize(SVIInt width, SVIInt height);
		void restore();
		void updateScene(SVIFloat fElapsedTime);
		void renderScene();

        //2012-06-07 masterkeaton27@gmail.comfurther purpose.
		void setCamera();
	public:
		static SVIBool isVideoCore();
		static SVIBool isMali();
		static SVIBool isSGX();
		static SVIBool isTegra();
		static SVIBool isAdreno();
        static SVIBool isAdreno320();
        static SVIBool isSGX544();
        static SVIBool isMaliT628();

        static SVIBool hasDepth24Stencil8();

	public:		
		SVIVector2		getViewSize();
		SVIRenderScene*	getScene();
		
		//2011-11-17 masterkeaton27@gmail.com
		SVIVector2	mTouchPosition;

    protected:
        void renderPostEffectScene();
        void renderNormalScene();

	protected:
		static SVIBool mIsVideoCore;
		static SVIBool mIsMali;
		static SVIBool mIsMaliT628;
		static SVIBool mIsSGX;
		static SVIBool mIsTegra;
		static SVIBool mIsAdreno;
		static SVIBool mIsAdreno320;
		static SVIBool mIsSGX544;
		static SVIBool mhasDepth24Stencil8;

	protected:
		SVIBool		mRequestedRestore;
		SVIBool		mRequestedFinalize;
		
        SVIBool		mRequestedResize;
	protected:
		SVIRenderScene*	mBackScene;
		SVIVector2		mViewSize;
		SVIGLSurface* mSVIGLSurface;

	public:
        SVIBool           mUsePostEffect;
        SVIBool           mUsePostRippleEffect;
		SVIBool			mIsDeform;
	};
};
