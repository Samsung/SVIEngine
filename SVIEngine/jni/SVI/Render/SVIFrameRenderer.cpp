#include "../SVICores.h"
#include "SVIRenderer.h"
#include "SVIRenderScene.h"
#include "SVIRenderPatch.h"
#include "SVIFrameRenderer.h"
#include "SVIProgramManager.h"
#include "../Slide/SVIProjectionSlide.h"
#include "SVIViewport.h"
#include "SVI3DScene.h"
#include "SVIModelManager.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

#define SSVIMPLE_SCALE 1.0f
		
	////////////////////////////////////////////////////////////////////////////////////
	SVIUInt SVICaptureData::addRef(){
		//0211-11-18 masterkeaton27@gmail.com
		//should change with InterlockedIncrement after work
		
		DEBUG_CHECK_LOGE("SVICaptureData<%d> - add ref %d",(SVIInt)this,mRefCount+1);
		return mRefCount++;
	}
	
	SVIUInt SVICaptureData::release(){
		//0211-11-18 masterkeaton27@gmail.com
		//should change with InterlockedDecrement after work
		mRefCount--;

		DEBUG_CHECK_LOGE("SVICaptureData<%d> - released %d",(SVIInt)this,mRefCount);
		if (mRefCount == 0){
			mSVIGLSurface->getSlideCapture()->removeCapture(this);//Multi-Instance-Support
			DEBUG_CHECK_LOGE("SVICaptureData<%d> - freed %d",(SVIInt)this,mRefCount);
		}
		return mRefCount;
	}

	

	SVICaptureData::SVICaptureData(SVIGLSurface * saGLSurface){
		mSVIGLSurface=saGLSurface;//Multi-Instance-Support
		
		mType = CAPTURE_FULL;
		mSize = mSVIGLSurface->getRenderer()->getViewSize();
		mDBId = mFBId = mId = mSBId = 0;
		mOwner = NULL;
		mRefCount = 0;
		mCaptureStarted = SVIFALSE;
		mIsUploaded = SVIFALSE;
		mReservedLoad = SVIFALSE;
		mSavedFrameBufferId =0;
		//SVI_SVIFE_ADDREF(this);
	}

	SVICaptureData::SVICaptureData(SVIProjectionSlide * pOwner){
		mType = CAPTURE_FULL;
		SVISlideOutfit outpit = pOwner->getOutfit();
		SVIVector3 size = outpit.mSize * outpit.mScale;
		mSize.x = size.x;
		mSize.y = size.y;
		mDBId = mFBId = mId = mSBId= 0;
		mOwner = pOwner;
		mRefCount = 0;
		mCaptureStarted = SVIFALSE;
		mIsUploaded = SVIFALSE;
		mReservedLoad = SVIFALSE;
		mSavedFrameBufferId =0;
		mSVIGLSurface = pOwner->getSVIGLSurface();
		//SVI_SVIFE_ADDREF(this);
	}

	SVICaptureData::~SVICaptureData(){
		destroy();
		mSVIGLSurface = NULL;
	}

	void SVICaptureData::create(){
		create(mSize.x,mSize.y);
	}


    static void checkFBGlError(const char* op) {
        GLint error;
        for (error = glGetError(); error; error = glGetError()) {
            LOGE("after %s() glError (0x%x) - %d\n", op, error, error);
        }
    }

    void SVICaptureData::create(SVIFloat width, SVIFloat height){		
		if (width < 1 && height < 1) return;

        int addRes = 64;
        int filterWidth = (int)(width * SSVIMPLE_SCALE);
        int filterHeight = (int)(height * SSVIMPLE_SCALE);

		glGenTextures(1, (GLuint*)&mId);
		glBindTexture(GL_TEXTURE_2D, mId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//SVI_TEXTURE_FILTER_MIN);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//SVI_TEXTURE_FILTER_MAG);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,             
			filterWidth, filterHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		
		//2011-10-28 masterkeaton27@gmail.com
		static GLint stencilBufferBit;
		glGetIntegerv(GL_STENCIL_BITS, &stencilBufferBit);

		static GLint depthBufferBit;
		glGetIntegerv(GL_DEPTH_BITS, &depthBufferBit);


        //2013-01-10 masterkeaton27@gmail.com
        //special treatment for adreno 320
        //2013-06-12 masterkeaton27@gmail.com
        //special treatment for mali T628
        //2013-06-13 masterkeaton27@gmail.com
        //checking depth 24 stencil 8
        if (mSVIGLSurface->getRenderer()->hasDepth24Stencil8()){
            glGenRenderbuffers(1, &mSBId);
            glBindRenderbuffer(GL_RENDERBUFFER, mSBId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, filterWidth, filterHeight);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            mDBId = mSBId;
        }else{
		    //2012-05-17 masterkeaton27@gmail.com
		    //skip stencil buffer process under broadcom gpu
		    if (stencilBufferBit > 0 && !mSVIGLSurface->getRenderer()->isVideoCore()){
			    glGenRenderbuffers(1,(GLuint*)&mSBId);
			    glBindRenderbuffer(GL_RENDERBUFFER, mSBId);			
			    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, filterWidth, filterHeight);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
            }

            if (depthBufferBit > 0){
                glGenRenderbuffers(1,(GLuint*)&mDBId);
                glBindRenderbuffer(GL_RENDERBUFFER, mDBId);

                GLenum internalFormat = GL_DEPTH_COMPONENT16;
                if ( depthBufferBit == 24 ) internalFormat = GL_DEPTH_COMPONENT24_OES;

                glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, filterWidth, filterHeight);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
            }
        }

		mSize.x = width;
		mSize.y = height;

		glGenFramebuffers(1, (GLuint*)&mFBId);
	}

	void SVICaptureData::create(SVIVector2& offset, SVIVector2& size){
		mOffset.x = offset.x;
		mOffset.y = offset.y;
		mSize.x = size.x;
		mSize.y = size.y;
		create(size.x,size.y);
	}

	void SVICaptureData::restore(){
		destroy();
		create(mSize.x,mSize.y);
	}

	void SVICaptureData::destroy(){
        if (mFBId != 0 ){
            glDeleteFramebuffers(1, &mFBId);	
            mFBId = 0;
        }
        if (mSBId != 0){
            glDeleteRenderbuffers(1,&mSBId);
            mSBId = 0;

            //2013-06-13 masterkeaton27@gmail.com
            if (mSVIGLSurface->getRenderer()->hasDepth24Stencil8()){
                mDBId = 0;
            }
        }
        if (mDBId != 0){
            glDeleteRenderbuffers(1,&mDBId);
            mDBId = 0;
        }
        if (mId != 0){
            glDeleteTextures(1, &mId);
            mId = 0;
        }			
	}


	#define SVI_OFFSCREEN_CLEAR_COLOR 0.0f

	void SVICaptureData::bind(SVIBool isFullScene){	
		
        //checkFBGlError("initialize");

		//save previous GL_VIEWPORT and FRAMEBUFFER
		glGetIntegerv(GL_VIEWPORT, mSavedViewport);

        //checkFBGlError("get viewport");

		mSavedFrameBufferId = 0;
		//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mSavedFrameBufferId);
		SVIFloat savedWidth = (float)mSavedViewport[2];
		SVIFloat savedHeight = (float)mSavedViewport[3];

		glBindFramebuffer(GL_FRAMEBUFFER, mFBId);  

        //checkFBGlError("bind FB");

		if (mId != 0) {
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mId, 0); 
        }else{
            //LOGE("glFramebufferTexture2D - GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 failed");
        }

        //checkFBGlError("bind CB");

		//2012-05-17
		//skip stencil buffer process under broadcom gpu
		if (mSBId != 0 && !mSVIGLSurface->getRenderer()->isVideoCore()) {
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mSBId); 
		}else{
            //LOGE("glFramebufferRenderbuffer - GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT/GL_RENDERBUFFER failed");
        }

        //checkFBGlError("bind SB");

		if (mDBId != 0) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDBId);
        }else{
            //LOGE("glFramebufferRenderbuffer - GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT/GL_RENDERBUFFER failed");
        }

        //checkFBGlError("bind DB");

        int status = glCheckFramebufferStatus( GL_FRAMEBUFFER );    


        //checkFBGlError("glCheckFramebufferStatus");

        if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT){
            LOGE("glCheckFramebufferStatus failed - GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
        }else if (status == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS){
            LOGE("glCheckFramebufferStatus failed - GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
        }else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT){
            LOGE("glCheckFramebufferStatus failed - GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
        }else if (status == GL_FRAMEBUFFER_UNSUPPORTED){
            LOGE("glCheckFramebufferStatus failed - GL_FRAMEBUFFER_UNSUPPORTED");
        }

        SVIBool bTriggered = ( status == GL_FRAMEBUFFER_COMPLETE ) ? SVITRUE : SVIFALSE;	

        mCaptureStarted = bTriggered;

        if (mCaptureStarted){
            SVIVector3 size = mSize;
            /*if (mOwner != NULL){
                SVISlideOutfit outfit = mOwner->getOutfit();

            }*/

            glViewport(0, 0, (GLsizei)mSize.x * SSVIMPLE_SCALE, (GLsizei)mSize.y * SSVIMPLE_SCALE);
            mSVIGLSurface->getViewport()->setOrtho(size.x ,size.y,-1000.0f,1000.0f,SVITRUE);

            if (mDBId != 0) glClearDepthf(1.0f);
            if (mSBId != 0) glClearStencil(0.0f);


            SVIBool isTranslucent = mSVIGLSurface->getRenderPartManager()->isTranslucent();//Multi-Instance-Support
            float color = isTranslucent ? 0.0f : SVI_OFFSCREEN_CLEAR_COLOR;
			glClearColor(color, color, color, isTranslucent ? 0.0f : 1.0f);
			glClear( (mSBId != 0 && mDBId != 0) ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT : GL_COLOR_BUFFER_BIT );//
			
		}else{
			DEBUG_CHECK_LOGE("Frame Buffer Capture Failed <%d>", (SVIInt)this);
		}

	}

    void SVICaptureData::unbind(){
		if (mCaptureStarted == SVITRUE){
			glBindFramebuffer(GL_FRAMEBUFFER, mSavedFrameBufferId);  			
			glBindTexture(GL_TEXTURE_2D, 0);
			glViewport(0, 0, mSavedViewport[2], mSavedViewport[3]);    
			mSVIGLSurface->getViewport()->setOrtho(mSavedViewport[2],mSavedViewport[3],-1000.0f,1000.0f);
			mCaptureStarted = SVIFALSE;
		}
	}

	void SVICaptureData::update(){

	}
	
	void SVICaptureData::setType(SVIInt type){
		mType = type;
	}
	
	void SVICaptureData::setId(SVIInt id){
		mId = id;
	}

	void SVICaptureData::setFrameBufferId(SVIInt id){
		mFBId = id;
	}
	
	void SVICaptureData::setSize(SVIVector2& size){
		mSize.x = size.x;
		mSize.y = size.y;
	}

	void SVICaptureData::setSize(SVIVector2* size){
		mSize.x = size->x;
		mSize.y = size->y;
	}

	void SVICaptureData::setOwner(SVIProjectionSlide * pOwner){
		mOwner = pOwner;
	}

	SVIBool	SVICaptureData::isResized(float width, float height){
		return ((SVIInt)width != (SVIInt)mSize.x ||
				(SVIInt)height != (SVIInt)mSize.y) ? SVITRUE : SVIFALSE;
	}

	/*
	//2011-11-06 masterkeaton27@gmail.com
	//reference count check added.
	void	SVICaptureData::addRef(SVIProjectionSlide* pSlide){
		for (int n = 0; n < mRefList.size(); n++){
			if (mRefList[n] == pSlide) return;
		}
		mRefList.push_back(pSlide);
	}

	void	SVICaptureData::releaseRef(SVIProjectionSlide* pSlide){
		std::vector<SVIProjectionSlide*>::iterator iter = mRefList.begin();
		while (iter != mRefList.end()){
			if (pSlide == *iter){
				mRefList.erase(iter);
				return;
			}
			iter++;
		}
	}
	*/

	////////////////////////////////////////////////////////////////////////////////////////////////

	SVISlideCapture::SVISlideCapture(SVIGLSurface* saGLSurface){//Multi-Instance-Support
		if(saGLSurface == NULL) {
			LOGE("Invalid argument to the SVISlideCapture::SVISlideCapture(SVIGLSurface *surface) \n");
			exit(0); // To check for errors till stabilization.. To be removed later. 
		} else {
			mSVIGLSurface = saGLSurface;
		}

		mCaptures = new SVICaptureData*[STATIC_CAPTURE_COUNT];
		for(int i= 0; i < STATIC_CAPTURE_COUNT; i++)
		{
			mCaptures[i] = new SVICaptureData(mSVIGLSurface);
			
		}
		
	}

	SVISlideCapture::~SVISlideCapture() {
		for(int i= 0; i < STATIC_CAPTURE_COUNT; i++)
		{
			delete mCaptures[i];
		}
		delete []mCaptures;
		mSVIGLSurface = NULL;
	}
	
	void SVISlideCapture::initialize(){
		//SVIVector2 viewSize = mSVIGLSurface->getRenderer()->getViewSize();		
        //2013-02-06 masterkeaton27@gmail.com
        //default RT deprecated.
		//mCaptures[STATIC_CAPTURE_FULLSCENE].create(viewSize.x, viewSize.y);
	}

	void SVISlideCapture::update(){
		for (SVIUInt n = 0; n < mReservedCaptures.size(); n++){
			SVIProjectionSlide * pSlide = mReservedCaptures[n];
			renderTraversal(pSlide);
		}
		mReservedCaptures.clear();		
	}

	void SVISlideCapture::finalize(){
		DEBUG_CHECK_LOGE("RESTORE: referenced slide count %d",mReferencedSlides.size());
		DEBUG_CHECK_LOGE("RESTORE: dynamic capture data count %d",mDynamicCaptures.size());

		for (SVIUInt n = 0; n < mReferencedSlides.size(); n++){
			SVISlideTextureContainer * pContainer = mReferencedSlides[n]->getTextureContainer();
			pContainer->setCaptureTexture(NULL);			
		}		
		mReferencedSlides.clear();

		for (SVIUInt n = 0; n < mReferencedBackFaceSlides.size(); n++){
			SVISlideTextureContainer * pContainer = mReferencedBackFaceSlides[n]->getTextureContainer();
			pContainer->setBackFaceCaptureTexture(NULL);			
		}		
		mReferencedBackFaceSlides.clear();


		for (SVIUInt n = 0; n < mDynamicCaptures.size(); n++){
			removeCapture(mDynamicCaptures[n]);			
		}

        //2013-02-06 masterkeaton27@gmail.com
        //default RT deprecated.
		//mCaptures[STATIC_CAPTURE_FULLSCENE].destroy();
	}
	

	void SVISlideCapture::start(SVIProjectionSlide * pSlide){
		if (pSlide == NULL) return;
		mReservedCaptures.push_back(pSlide);		
	}

	void SVISlideCapture::stop(SVIProjectionSlide * pSlide){
		if (pSlide == NULL) return;

		//2011-11-09 masterkeaton27@gmail.com
		SVIRenderPatch * pPatch = pSlide->getRenderPatch();
		if (pPatch == NULL) return;

		SVISlideTextureContainer *pTextureContainer = pSlide->getTextureContainer();
		if(pTextureContainer == NULL)return;

		SVICaptureData* pCaptureData = pSlide->getTextureContainer()->getCaptureTexture();
		if (pCaptureData == NULL ) return;
		pCaptureData->setOwner(NULL);	
		
		removeReferencedSlide(pSlide);
		pSlide->getTextureContainer()->setCaptureTexture(NULL);
	}
			
	SVIBool SVISlideCapture::isExist(SVIProjectionSlide* pSlide){
		//SVICaptureData* pData = pSlide->getTextureContainer()->getCaptureTexture();
		for (SVIUInt n = 0; n < mReferencedSlides.size(); n++){
			if (mReferencedSlides[n] == pSlide) return SVITRUE;
		}
		return SVIFALSE;
	}

	SVIBool SVISlideCapture::isExistBackFace(SVIProjectionSlide* pSlide){
		//SVICaptureData* pData = pSlide->getTextureContainer()->getBackFaceCaptureTexture();
		for (SVIUInt n = 0; n < mReferencedBackFaceSlides.size(); n++){
			if (mReferencedBackFaceSlides[n] == pSlide) return SVITRUE;
		}
		return SVIFALSE;
	}

	SVICaptureData* SVISlideCapture::addCapture(SVIProjectionSlide* pSlide){
		SVICaptureData* pData = pSlide->getTextureContainer()->getCaptureTexture();
		if (pData != NULL){
			
			SVISlideOutfit outpit = pSlide->getOutfit();
			SVIVector3 size = outpit.mSize * outpit.mScale;
			
			//2011-10-24 masterkeaton27@gmail.com
			//avoid useless finalize & initialize
			if (pData->isResized(size.x,size.y)){
				pData->destroy();				
				pData->create(size.x,size.y);
			}			
		}else{
			pData = new SVICaptureData(pSlide);
			pData->create();
			mDynamicCaptures.push_back(pData);			
		}		
		return pData;
	} 

	SVICaptureData* SVISlideCapture::addBackFaceCapture(SVIProjectionSlide* pSlide){
		SVICaptureData* pData = pSlide->getTextureContainer()->getBackFaceCaptureTexture();
		if (pData != NULL){

			SVISlideOutfit outpit = pSlide->getOutfit();
			SVIVector3 size = outpit.mSize * outpit.mScale;

			//2011-10-24 masterkeaton27@gmail.com
			//avoid useless finalize & initialize
			if (pData->isResized(size.x,size.y)){
				pData->destroy();				
				pData->create(size.x,size.y);
			}			
		}else{
			pData = new SVICaptureData(pSlide);
			pData->create();
			mDynamicCaptures.push_back(pData);			
		}		
		return pData;
	} 

	void SVISlideCapture::addReferencedSlide(SVIProjectionSlide* pSlide){
		if (isExist(pSlide)) return;
		mReferencedSlides.push_back(pSlide);
	}

	void SVISlideCapture::removeReferencedSlide(SVIProjectionSlide* pSlide){
		std::vector<SVIProjectionSlide*>::iterator iter = mReferencedSlides.begin();
		while (iter != mReferencedSlides.end()){
			SVIProjectionSlide* pTargetSlide = *iter;
			if (pTargetSlide == pSlide){
				//pSlide->getTextureContainer()->setCaptureTexture(NULL);
				mReferencedSlides.erase(iter);
				return;
			}
			iter++;
		}				
	}

	void SVISlideCapture::addBackFaceReferencedSlide(SVIProjectionSlide* pSlide){
		if (isExistBackFace(pSlide)) return;
		mReferencedBackFaceSlides.push_back(pSlide);
	}

	void SVISlideCapture::removeBackFaceReferencedSlide(SVIProjectionSlide* pSlide){
		std::vector<SVIProjectionSlide*>::iterator iter = mReferencedBackFaceSlides.begin();
		while (iter != mReferencedBackFaceSlides.end()){
			SVIProjectionSlide* pTargetSlide = *iter;
			if (pTargetSlide == pSlide){
				//pSlide->getTextureContainer()->setCaptureTexture(NULL);
				mReferencedBackFaceSlides.erase(iter);
				return;
			}
			iter++;
		}				
	}

	void SVISlideCapture::removeCapture(SVICaptureData* pCapture){
		std::vector<SVICaptureData*>::iterator iter = mDynamicCaptures.begin();
		while (iter != mDynamicCaptures.end()){
			SVICaptureData* pInnerCapture = *iter;
			if (pInnerCapture == pCapture){
				iter = mDynamicCaptures.erase(iter);
				continue;
			}
			iter++;
		}		

		//anyway, free this one
		pCapture->destroy();
		SVI_SVIFE_DELETE(pCapture);
	}

	
	void SVISlideCapture::removeCapture(SVIProjectionSlide * pSlide){
		pSlide->getTextureContainer()->setCaptureTexture(NULL);		
	}

	void SVISlideCapture::renderQuad(SVIFloat width, SVIFloat height, SVIInt textureId){
		SVIVector2 offset(0.0f,0.0f);
		SVIVector2 size(width, height);
		renderQuad(offset,size,textureId);
	}

	void SVISlideCapture::renderQuad(SVIVector2& offset, SVIVector2& size, SVIInt textureId){
		SVIVector2 vertices[4] = {
			SVIVector2(offset.x		 , offset.y),
			SVIVector2(offset.x		 , offset.y + size.y),
			SVIVector2(offset.x + size.x, offset.y + size.y),
			SVIVector2(offset.x + size.x, offset.y)
		};

        float aaCoords[4] = {1.0f, 1.0f, 1.0f, 1.0f};

		SVIProgramParams renderParam;
		renderParam.addElement(EM_TEXTURE);

		SVIProgram * pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParam);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		SVIProgramHandler * pHandler = pProgram->getHandler();

		pProgram->use();
		pHandler->setSampler(HD_U_SVIMPLER_0, textureId);			

		SVIVector2 sideSize = size;
		SVIVector2 sidePivot = offset;
		pHandler->setUniform(HD_U_SIZE, sideSize);
		pHandler->setUniform(HD_U_PIVOT, sidePivot);

        SVIVector4 texInfo = SVIVector4(0.0f,0.0f,1.0f,1.0f);
        pHandler->setUniform(HD_U_TEXTUREINFO, texInfo);

		//set model view projection matrix
		SVIMatrix viewMat;		
		SVIMatrix projMat = mSVIGLSurface->getViewport()->getProjection();
		viewMat.setIdentity();

        pHandler->setAttribute(HD_A_AACOORD, 1, aaCoords);

		pHandler->setUniform(HD_U_PROJ, projMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);
		
		pHandler->setAttribute(HD_A_POSITION, vertices);			

		glDrawArrays( GL_TRIANGLE_FAN , 0, 4);	

		pHandler->releaseAttribute(HD_A_POSITION);

	}

	void SVISlideCapture::setCaptureSlideToSlide(SVIProjectionSlide* pSrcSlide, SVIProjectionSlide* pDestSlide){
		SVISlideTextureContainer *	pSrcContainer = pSrcSlide->getTextureContainer();
		SVISlideTextureContainer *	pDestContainer = pDestSlide->getTextureContainer();
		SVIRenderPatch *				pDestPatch = pDestSlide->getRenderPatch();
		SVICaptureData *				pCaptureData = pSrcContainer->getCaptureTexture();

		//2012-04-19 masterkeaton27@gmail.com
		//to solve white blink issues
		SVIVector3 size = pSrcSlide->getOutfit().mSize;
		if (size.x * size.y <= 1.0f) {
			//size.x * size.y <= 1.0f
			LOGE("Capture Size: Area %f, X %f Y %f", size.x * size.y , size.x , size.y);
			return;
		}

		//2011-11-06 masterkeaton27@gmail.com
		//if system does not have a captured data, 
		//should capture source slide immediately.
		if (pCaptureData == NULL){ 
			//get capture data from source slide.			
			renderTraversal(pSrcSlide);
			pCaptureData = pSrcContainer->getCaptureTexture();
			if (pCaptureData != NULL){
				pDestContainer->setCaptureTexture(pCaptureData);				
			}else return;
		}else{
			pDestContainer->setCaptureTexture(pCaptureData);				
		}		
		addReferencedSlide(pDestSlide);
		pDestPatch->setTextureContainer(pDestContainer);
	}

	void SVISlideCapture::releaseCaptureFromSlide(SVIProjectionSlide* pSlide){
		if (pSlide == NULL) return;		
		pSlide->getTextureContainer()->setCaptureTexture(NULL);		
		removeReferencedSlide(pSlide);
	}


	void SVISlideCapture::setBackFaceCaptureSlideToSlide(SVIProjectionSlide* pSrcSlide, SVIProjectionSlide* pDestSlide){
		SVISlideTextureContainer *	pSrcContainer = pSrcSlide->getTextureContainer();
		SVISlideTextureContainer *	pDestContainer = pDestSlide->getTextureContainer();
		SVIRenderPatch *				pDestPatch = pDestSlide->getRenderPatch();
		SVICaptureData *				pCaptureData = pSrcContainer->getCaptureTexture();

		//2011-11-06 masterkeaton27@gmail.com
		//if system does not have a captured data, 
		//should capture source slide immediately.
		if (pCaptureData == NULL){ 
			//get capture data from source slide.			
			renderTraversal(pSrcSlide);
			pCaptureData = pSrcContainer->getCaptureTexture();
			if (pCaptureData != NULL){
				pDestContainer->setBackFaceCaptureTexture(pCaptureData);				
			}else return;
		}else{
			pDestContainer->setBackFaceCaptureTexture(pCaptureData);				
		}		
		addBackFaceReferencedSlide(pDestSlide);
		pDestPatch->setTextureContainer(pDestContainer);
	}

	void SVISlideCapture::releaseBackFaceCaptureFromSlide(SVIProjectionSlide* pSlide){
		if (pSlide == NULL) return;		
		pSlide->getTextureContainer()->setBackFaceCaptureTexture(NULL);		
		removeBackFaceReferencedSlide(pSlide);
	}

	SVICaptureData* SVISlideCapture::renderTraversal(SVIProjectionSlide * pSlide){

		DEBUG_CHECK_LOGE("SVISlideCapture: renderTraversal");

		SVIRenderScene * pScene = mSVIGLSurface->getRenderer()->getScene();
		if (pScene == NULL) return NULL;
		
		SVIRenderPatch * pPatch = pSlide->getRenderPatch();


		//2012-01-30 masterkeaton27@gmail.com
		if (pPatch == NULL) {
			pScene->buildUnRenderedRenderPatch(pSlide);
			pPatch = pSlide->getRenderPatch();
		}


		SVICaptureData * pData = addCapture(pSlide);
		SVICaptureData * pSavedData = NULL;
		SVISlideTextureContainer * pContainer = pSlide->getTextureContainer();


		if (pContainer){
			pSavedData = pContainer->getCaptureTexture();

			//should increase one ref count to prevent free.
			SVI_SVIFE_ADDREF(pSavedData);
			pContainer->setCaptureTexture(NULL);

			pPatch->setTextureContainer(pContainer);	
		}

		//begin offscreen render
		pSlide->bake();
		pData->bind();

		if (pSlide->has3DScene()){
			DEBUG_CHECK_LOGE("3D: Running Scene");
			SVI3DScene * pLocal3DScene = pSlide->get3DScene();
			pLocal3DScene->setSize(pData->getSize().x, pData->getSize().y);
			pLocal3DScene->run(1.0f/60.0f);
		}else{
			pScene->renderSpecifyTraversal(pSlide);
		}		

		pData->unbind();
		pSlide->rebake();

		

		addReferencedSlide(pSlide);
		if (pContainer){
		pContainer->setCaptureTexture(pData);
		pPatch->setTextureContainer(pContainer);
		}
		//release ref count which one was we've been increased for safety.
		if (pSavedData != NULL) SVI_SVIFE_RELEASE(pSavedData);

		return pData;
	}

	SVICaptureData* SVISlideCapture::renderBackFaceTraversal(SVIProjectionSlide * pSlide){

		DEBUG_CHECK_LOGE("SVISlideCapture: renderTraversal");

		SVIRenderScene * pScene = mSVIGLSurface->getRenderer()->getScene();
		if (pScene == NULL) return NULL;

		SVIRenderPatch * pPatch = pSlide->getRenderPatch();
		
		//2012-01-30 masterkeaton27@gmail.com
		if (pPatch == NULL) {
			pScene->buildUnRenderedRenderPatch(pSlide);
			pPatch = pSlide->getRenderPatch();
		}

		SVICaptureData * pData = addBackFaceCapture(pSlide);

		SVICaptureData * pSavedData = NULL;
		SVISlideTextureContainer * pContainer = pSlide->getTextureContainer();
		if (pContainer){
			pSavedData = pContainer->getBackFaceCaptureTexture();
			//should increase one ref count to prevent free.
			SVI_SVIFE_ADDREF(pSavedData);
			pContainer->setBackFaceCaptureTexture(NULL);
			pPatch->setTextureContainer(pContainer);				
		}

		//begin offscreen render
		pSlide->bake();
		pData->bind();

		if (pSlide->has3DScene()){
			DEBUG_CHECK_LOGE("3D: Running Scene");
			SVI3DScene * pLocal3DScene = pSlide->get3DScene();
			pLocal3DScene->setSize(pData->getSize().x, pData->getSize().y);
			pLocal3DScene->run(1.0f/60.0f);
		}else{
			pScene->renderSpecifyTraversal(pSlide);
		}		

		pData->unbind();
		pSlide->rebake();


		addBackFaceReferencedSlide(pSlide);
		if(pContainer){
		pContainer->setBackFaceCaptureTexture(pData);
		pPatch->setTextureContainer(pContainer);
		}

		//release ref count which one was we've been increased for safety.
		if (pSavedData != NULL) SVI_SVIFE_RELEASE(pSavedData);

		return pData;
	}
};
