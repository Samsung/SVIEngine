#include "../SVICores.h"
#include "SVIRenderer.h"
#include "../Slide/SVIProjectionSlide.h"
#include "SVIRenderScene.h"

#include "../Slide/SVIRenderPartManager.h"
#include "../Render/SVIProgramManager.h"

//2012-06-05 masterkeaton27@gmail.com
#include "../Render/SVIParticleRenderer.h"

#include <time.h>
#include "SVIViewport.h"
#include "SVIDebugRenderer.h"
#include "SVITextureManager.h"
#include "SVIModelManager.h"

#ifdef SVI_USING_POST_PROCESS
#include "SVIFrameRenderer.h"
#include "SVIRenderPatch.h"
#include "SVIPostEffectRenderer.h" // 2012-12-14 masterkeaton27@gmail.com
#endif

//#define USE_PARTICLE_RENDERER
//#define LOGGING_TIME

namespace SVI{

		SVIBool SVIRenderer::mIsVideoCore = 0;
		SVIBool SVIRenderer::mIsMali = 0;
		SVIBool SVIRenderer::mIsMaliT628=0;
		SVIBool SVIRenderer::mIsSGX = 0;
		SVIBool SVIRenderer::mIsTegra = 0;
		SVIBool SVIRenderer::mIsAdreno = 0;
        SVIBool SVIRenderer::mIsAdreno320 = 0;
        SVIBool SVIRenderer::mIsSGX544 = 0;
		SVIBool SVIRenderer::mhasDepth24Stencil8=0;

long GetTickCount(){
    struct timeval t;
    gettimeofday(&t, NULL);

    //time is measured as millisecond.
    SVIUInt currentTime = (SVIUInt)((t.tv_sec % SEC_A_DAY)*1000 + (t.tv_usec / 1000));
    return currentTime;
}

void SVIRenderer::requestRender(){
	if (mBackScene == NULL) return;
	mBackScene->requestBuild();
}

SVIRenderer::SVIRenderer(SVIGLSurface* saGLSurface){//Multi-Instance-Support
	if(saGLSurface == NULL) {
		LOGE("Invalid argument to the SVIRenderer::SVIRenderer(SVIGLSurface *surface) \n");
		exit(0); // To check for errors till stabilization.. To be removed later. 
	} else {
		mSVIGLSurface = saGLSurface;
	}

	mTouchPosition = SVIVector2(1.0f,0.0f);
	mBackScene = NULL;
	mRequestedRestore = SVIFALSE;
	mRequestedResize = SVIFALSE;
	mRequestedFinalize = SVIFALSE;
	mIsDeform = SVIFALSE;

	mUsePostEffect = SVIFALSE;
	mUsePostRippleEffect = SVIFALSE;

}

void SVIRenderer::setRootSlide(SVIProjectionSlide * pRoot){
	if( pRoot != NULL ) {
		//2011-05-25
		pRoot->initialize();//this may cause performance issue
		//2011-06-28 masterkeaton27@gmail.com
		//root flag for invisible rendering
		pRoot->setAbsoluteRoot();
	}
	//2012-05-11 masterkeaton27@gmail.com
	if (mBackScene == NULL){
		mBackScene = new SVIRenderScene(mSVIGLSurface);	
	}
	SVIBool bBuildResult = mBackScene->buildRenderPatches(pRoot);
	//swapScene();	
}


SVIVector2 SVIRenderer::getViewSize(){return mViewSize;}
SVIRenderScene * SVIRenderer::getScene(){return mBackScene;}

SVIUInt getCurrentTime(){
	struct timeval t;
	gettimeofday(&t, NULL);
	//time is measured as millisecond.
	SVIUInt currentTime = (SVIUInt)((t.tv_sec % 86400)*1000 + (t.tv_usec / 1000));

    return currentTime;
}


SVIBool SVIRenderer::isVideoCore(){
	return mIsVideoCore;
}
SVIBool SVIRenderer::isMali(){
	return mIsMali;
}

SVIBool SVIRenderer::isMaliT628(){
    return mIsMaliT628;
}

SVIBool SVIRenderer::isSGX(){
	return mIsSGX;
}

SVIBool SVIRenderer::isTegra(){
	return mIsTegra;
}

SVIBool SVIRenderer::isAdreno(){
	return mIsAdreno;
}

SVIBool SVIRenderer::isAdreno320(){
    return mIsAdreno320;
}

SVIBool SVIRenderer::isSGX544(){
    return mIsSGX544;
}

//2013-06-13 masterkeaton27@gmail.com
//check extenstion for depth stencil
SVIBool SVIRenderer::hasDepth24Stencil8(){
    return mhasDepth24Stencil8;
}

void SVIRenderer::initialize(){
	mIsVideoCore = SVIFALSE;
	mIsMali = SVIFALSE;
	mIsMaliT628 = SVIFALSE;
	mIsSGX = SVIFALSE;
	mIsTegra = SVIFALSE;
	mIsAdreno = SVIFALSE;
	mIsAdreno320 = SVIFALSE;
	mIsSGX544 = SVIFALSE;
	mhasDepth24Stencil8 = SVIFALSE;
	
    long start,end,duration;
    //2012-05-17 masterkeaton27@gmail.com
	//get GPU information from chipset
	SVIPChar pVendorName = (SVIPChar)glGetString(GL_VENDOR);
    SVIPChar pRendererName = (SVIPChar)glGetString(GL_RENDERER);
    SVIPChar pVersionName = (SVIPChar)glGetString(GL_VERSION);
    SVIPChar pExtensionName = (SVIPChar)glGetString(GL_EXTENSIONS);

    LOGI("-----------------------------------");
    LOGI("GPU Vendor: %s",pVendorName);
    LOGI("GPU Renderer: %s",pRendererName);
    LOGI("GPU Version: %s",pVersionName);
    if (strstr(pRendererName,"VideoCore") > 0){
        mIsVideoCore = SVITRUE;
    }else if (strstr(pRendererName,"Adreno") > 0 && strstr(pRendererName,"320") > 0){
        mIsAdreno320 = SVITRUE;
        mIsAdreno = SVITRUE;
    }else if (strstr(pRendererName,"Adreno") > 0){
        mIsAdreno = SVITRUE;
    }else if (strstr(pRendererName,"Mali") > 0 && strstr(pRendererName,"T628") > 0){
        mIsMaliT628 = SVITRUE;
    }else if (strstr(pRendererName,"Mali") > 0){
        mIsMali = SVITRUE;
    }else if (strstr(pRendererName,"SGX") > 0  && strstr(pRendererName,"544") > 0){
        mIsSGX544 = SVITRUE;
    }else if (strstr(pRendererName,"SGX") > 0){
        mIsSGX = SVITRUE;
    }else if (strstr(pRendererName,"Tegra") > 0){
        mIsTegra = SVITRUE;
    }
    SVIVector2 viewSize = getViewSize();		
    LOGI("Allocated Resolution: %f, %f",viewSize.x, viewSize.y);
    LOGI("-----------------------------------");


    //LOGI("GPU Version: %s",pExtensionName);

    //2013-06-13 masterkeaton27@gmail.com
    //check extenstion for depth stencil
    if (strstr(pExtensionName,"GL_OES_packed_depth_stencil ") > 0){
        mhasDepth24Stencil8 = SVITRUE;
        LOGI("Extension GL_OES_packed_depth_stencil (GL_DEPTH24_STENCIL8_OES) enabled");
        LOGI("-----------------------------------");
    }
    

    //LOGI("Available Extensions:\n%s",pExtensionName);
    //LOGI("-----------------------------------");

#ifdef LOGGING_TIME
    start = GetTickCount();
    end = GetTickCount();
    duration = end - start;
    if (duration > 0) LOGI("TRACK: Get GL Information time %d", duration);
#endif

#ifdef USE_PARTICLE_RENDERER
	//2012-06-05 masterkeaton27@gmail.com
	mSVIGLSurface->getParticleRenderer()->initialize();
#endif
    mSVIGLSurface->getProgramManager()->create();
    mSVIGLSurface->getViewport()->initialize();
    mSVIGLSurface->getDebugRenderer()->initialize();
    mSVIGLSurface->getTextureManager()->initialize();//Multi-Instance-Support
    
    mSVIGLSurface->getSlideCapture()->initialize();//Multi-Instance-Support
    
    //2012-12-14 masterkeaton27@gmail.com
    if (mUsePostEffect){
        mSVIGLSurface->getPostEffectRenderer()->initialize(viewSize.x, viewSize.y);//Multi-Instance-Support
    }
        

    SVI_SVIFE_DELETE(mBackScene);
    mBackScene = new SVIRenderScene(mSVIGLSurface);	

#ifdef LOGGING_TIME
    end = GetTickCount();
    duration = end - start;
    if (duration > 0) LOGI("TRACK: Renderer initialization time %d", duration);
#endif
}

SVIRenderer::~SVIRenderer(){
	SVI_SVIFE_DELETE(mBackScene);
}


SVIUInt SVIRenderer::finalize(){
	
	/*
	mSVIGLSurface->getTextureManager()finalize();//Multi-Instance-Support
	
	SVIProgramManager::getInstance()->destroy();
	mSVIGLSurface->getSlideCapture()->finalize();//Multi-Instance-Support

	mSVIGLSurface->getViewport()->finalize();
	SVI_SVIFE_DELETE(mBackScene);	
	*/

	//mSVIGLSurface->getTextureManager()finalize();//Multi-Instance-Support
	//SVIProgramManager::getInstance()->destroy();
	
	//2012-05-11 masterkeaton27@gmail.com
	//delete patches.
    	SVI_SVIFE_DELETE(mBackScene);	

    //SVIRippleRenderer::getInstance()->finalize();

	mRequestedFinalize = SVITRUE;

	return 0; //return finalized memory size
}

void SVIRenderer::resize(SVIInt width, SVIInt height){
	mSVIGLSurface->getViewport()->resize(width,height);

	mSVIGLSurface->getCommonShaderVariables()->setScreenSize(width,height);
	mSVIGLSurface->getCommonShaderVariables()->setTime(0.0f);

	mViewSize.x = width;
	mViewSize.y = height;

    mRequestedResize = SVITRUE;
}

void SVIRenderer::restore(){
	mRequestedRestore = SVITRUE;	
}

//2012-06-07 masterkeaton27@gmail.comfurther purpose.
void SVIRenderer::setCamera(){
	static SVIMatrix matWorld; 
	static SVIMatrix matView; 

	matWorld.setIdentity();
	matView.setIdentity();

#ifdef USE_PARTICLE_RENDERER
    static SVIMatrix matProjection = mSVIGLSurface->getViewport()->getProjection();
	mSVIGLSurface->getParticleRenderer()->setViewMatrix(&matView);
	mSVIGLSurface->getParticleRenderer()->setWorldMatrix(&matWorld);
	mSVIGLSurface->getParticleRenderer()->setProjectionMatrix(&matProjection);
#endif
}

void SVIRenderer::updateScene(SVIFloat fElapsedTime){
    long end,duration;

	if (mRequestedFinalize){		
		mRequestedFinalize = SVIFALSE;
	}

	if (mRequestedRestore){
		mSVIGLSurface->getViewport()->restore();
#ifdef USE_PARTICLE_RENDERER
		//2012-06-05 masterkeaton27@gmail.comparticle renderer added.
		mSVIGLSurface->getParticleRenderer()->finalize();
		mSVIGLSurface->getParticleRenderer()->initialize();
#endif
        mSVIGLSurface->getSlideCapture()->finalize();//Multi-Instance-Support
        mSVIGLSurface->getProgramManager()->restore();
		mSVIGLSurface->getModelManager()->restore();//Multi-Instance-Support

#ifdef LOGGING_TIME
        long start = 0;
        if (mRequestedRestore){
            start = GetTickCount();
        }
        end = GetTickCount();
        duration = end - start;
        if (duration > 0) LOGI("TRACK: program manager restore time %d", duration);
#endif

		mSVIGLSurface->getTextureManager()->restore();//Multi-Instance-Support
#ifdef LOGGING_TIME
        start = GetTickCount();
        end = GetTickCount();
        duration = end - start;
        if (duration > 0) LOGI("TRACK: GL Texture restore time %d", duration);
#endif
		if( mBackScene != NULL )
			mBackScene->requestBuild();
		       
        mRequestedRestore = SVIFALSE;
	}
    
    if (mRequestedResize){
        if (mUsePostEffect || mSVIGLSurface->getPostEffectRenderer()->getActivated()){//Multi-Instance-Support
            mSVIGLSurface->getPostEffectRenderer()->restore(mViewSize.x,mViewSize.y);//Multi-Instance-Support
            if (mUsePostRippleEffect){
                mSVIGLSurface->getRippleRenderer()->reset(mViewSize.x,mViewSize.y);
            }
        }    
#ifdef LOGGING_TIME
        if (mRequestedResize){
            start = GetTickCount();
        }

        end = GetTickCount();
        duration = end - start;
        if (duration > 5){
            LOGI("TRACK: Post Effect Renderer restore cost (%d ms)", duration);
        }        
#endif
        mRequestedResize = SVIFALSE;
    }

	SVIFloat fDeltaTime = 1.0f / 60.0f;
	mSVIGLSurface->getCommonShaderVariables()->addTimeDelta(fDeltaTime);

	mSVIGLSurface->getDebugRenderer()->update();
	mSVIGLSurface->getViewport()->update();	
		
	setCamera();

	if( mBackScene != NULL )
		mBackScene->updateFrame();	
#ifdef LOGGING_TIME
    start = GetTickCount();
    end = GetTickCount();
    duration = end - start;
    if (duration > 5){
        LOGI("TRACK: Scene UpdateFrame Cost(%d ms)", duration);
    }
#endif

#ifdef USE_PARTICLE_RENDERER
	mSVIGLSurface->getParticleRenderer()->update();
#endif
	mSVIGLSurface->getProgramManager()->update();
	mSVIGLSurface->getSlideCapture()->update();//Multi-Instance-Support

    if (mUsePostEffect){
        SVIVector2 viewSize = getViewSize();		
        mSVIGLSurface->getPostEffectRenderer()->update(viewSize.x, viewSize.y);//Multi-Instance-Support
    }

	mSVIGLSurface->getTextureManager()->update();//Multi-Instance-Support
#ifdef LOGGING_TIME
    start = GetTickCount();
    end = GetTickCount();
    duration = end - start;
    if (duration > 10){
        LOGI("TRACK: GL Texture Uploading Cost (%d ms)", duration);
    }
#endif
}

void SVIRenderer::renderPostEffectScene(){

    //2013-03-28 masterkeaton27@gmail.com
    //ripple rendering optimization.
    SVIBool isRippleAnimating = mSVIGLSurface->getRippleRenderer()->isAnimating();

    long end,duration;

    if (isRippleAnimating){
        mSVIGLSurface->getPostEffectRenderer()->beginFrame();//Multi-Instance-Support
        if( mBackScene != NULL )
            mBackScene->renderFrame();			
#ifdef USE_PARTICLE_RENDERER
        //2012-06-05 masterkeaton27@gmail.com
        mSVIGLSurface->getParticleRenderer()->defferedRender();
#endif
        mSVIGLSurface->getDebugRenderer()->render();
        mSVIGLSurface->getPostEffectRenderer()->endFrame();//Multi-Instance-Support
    }
#ifdef LOGGING_TIME
    long start;
    start = GetTickCount();
    end = GetTickCount();
    duration = end - start;
    if (duration > 5) LOGI("TRACK: Rendering Post Off-Scene - Frame Cost (%d ms)", duration);
#endif


#if USE_STENCIL_BUFFER
    SVIBool isTranslucent = mSVIGLSurface->getRenderPartManager()->isTranslucent();//Multi-Instance-Support
    float color = isTranslucent ? 0.0f : SVI_CLEAR_COLOR;
    glClearColor(color, color, color, isTranslucent ? 0.0f : 1.0f);

    glClearStencil(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
#endif
        
    if (isRippleAnimating){    
    	mSVIGLSurface->getPostEffectRenderer()->drawPostRippleEffect();//Multi-Instance-Support
    }else{
        if( mBackScene != NULL )
            mBackScene->renderFrame();			
#ifdef USE_PARTICLE_RENDERER
        //2012-06-05 masterkeaton27@gmail.com
        mSVIGLSurface->getParticleRenderer()->defferedRender();
#endif
    }

#if USE_STENCIL_BUFFER
    glDisable(GL_STENCIL_TEST);
#endif
#ifdef LOGGING_TIME
    start = GetTickCount();
    end = GetTickCount();
    duration = end - start;
    if (duration > 5) LOGI("TRACK: Rendering Front-Scene - Frame Cost (%d ms)", duration);
#endif
}

void SVIRenderer::renderNormalScene(){
    long end,duration;

#if USE_STENCIL_BUFFER
    SVIBool isTranslucent = mSVIGLSurface->getRenderPartManager()->isTranslucent();//Multi-Instance-Support
    float color = isTranslucent ? 0.0f : SVI_CLEAR_COLOR;
    glClearColor(color, color, color, isTranslucent ? 0.0f : 1.0f);

    glClearStencil(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
#endif

    if( mBackScene != NULL )
        mBackScene->renderFrame();			

#ifdef USE_PARTICLE_RENDERER
    //2012-06-05 masterkeaton27@gmail.com
    mSVIGLSurface->getParticleRenderer()->defferedRender();
#endif

    
    
#if USE_STENCIL_BUFFER
    glDisable(GL_STENCIL_TEST);
#endif
#ifdef LOGGING_TIME
    log start;
    start = GetTickCount();
    end = GetTickCount();
    duration = end - start;
    if (duration > 5) LOGI("TRACK: Rendering Front-Scene - Frame Cost (%d ms)", duration);
#endif
}

void SVIRenderer::renderScene(){		
    long end,duration;

    if (mUsePostEffect){
        renderPostEffectScene();
    }else{
        renderNormalScene();
    } 
#ifdef LOGGING_TIME
    long start;
    start = GetTickCount();
    end = GetTickCount();
    duration = end - start;
    if (duration > 10){
        LOGI("TRACK: Rendering Frame Cost (%d ms)", duration);
    }   
#endif
}

}
