#pragma once

#include "SVIRenderPatch.h"
#include "../GLSurface/SVIGLSurface.h"
namespace SVI{

	enum SVIStaticCaptureType{
		STATIC_CAPTURE_FULLSCENE = 0,
		STATIC_CAPTURE_PORTRAIT,
		STATIC_CAPTURE_LANDSCAPE,
		STATIC_CAPTURE_COUNT
	};

	enum SVICaptureType{
		CAPTURE_FULL = 0,
		CAPTURE_INDV,
		CAPTURE_TYPE_COUNT
	};

	

	class SVICaptureData{
	public:
		SVICaptureData(SVIGLSurface * mSVIGLSurface);//Multi-Instance-Support
		SVICaptureData(SVIProjectionSlide * pOwner);
		virtual ~SVICaptureData();
	public:
		SVIUInt release();
		SVIUInt addRef();

	public:
		void create();
		void create(SVIFloat width, SVIFloat height);
		void create(SVIVector2& offset, SVIVector2& size);
		void restore();
		void destroy();
		void update();

		void bind(SVIBool isFullScene = SVIFALSE);
		void unbind();

	public:
		void					setType(SVIInt type);
		void					setId(SVIInt id);
		void					setFrameBufferId(SVIInt id);
		void					setSize(SVIVector2& size);
		void					setSize(SVIVector2* size);
		void					setOwner(SVIProjectionSlide * pOwner);
		
	public:
		SVIInt					getType(){return mType;}
		SVIUInt					getId(){return mId;}
		SVIUInt					getFrameBufferId(){return mFBId;}
        SVIUInt					getStencilBufferId(){return mSBId;}
        SVIUInt					getDepthBufferId(){return mDBId;}
		SVIVector2&				getSize(){return mSize;}
		SVIProjectionSlide *		getOwner(){return mOwner;}


	public:
		SVIBool					isUploaded(){ return mIsUploaded;}
		SVIBool					isReserved(){ return mReservedLoad;}
		SVIBool					isResized(float width, float height);

	private:
		SVIInt				mType;
		SVIVector2			mSize;
		SVIVector2			mOffset;
		SVIUInt				mId;
		SVIUInt				mFBId;
		SVIUInt				mSBId;
		SVIUInt				mDBId;
		SVIProjectionSlide * mOwner;
		SVIInt				mSavedFrameBufferId;
		SVIInt				mSavedViewport[4];

	private:
		SVIInt				mRefCount;
	private:
		SVIBool				mReservedLoad;
		SVIBool				mIsUploaded;
		SVIBool				mCaptureStarted;
		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support
	};


	class SVISlideCapture{
	public:
		 SVISlideCapture(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		virtual ~SVISlideCapture();

	public:
		void initialize();
		void update();
		void finalize();

	public:
		void start(SVIProjectionSlide * pSlide);
		void stop(SVIProjectionSlide * pSlide);
		
	public:		
		void setCaptureSlideToSlide(SVIProjectionSlide* pSrcSlide, SVIProjectionSlide* pDestSlide);
		void releaseCaptureFromSlide(SVIProjectionSlide* pSlide);

		void setBackFaceCaptureSlideToSlide(SVIProjectionSlide* pSrcSlide, SVIProjectionSlide* pDestSlide);
		void releaseBackFaceCaptureFromSlide(SVIProjectionSlide* pSlide);


		SVICaptureData* renderTraversal(SVIProjectionSlide * pSlide);		
		SVICaptureData* renderBackFaceTraversal(SVIProjectionSlide * pSlide);	

	public:		
		void addReferencedSlide(SVIProjectionSlide* pSlide);
		void removeReferencedSlide(SVIProjectionSlide* pSlide);
		
		void addBackFaceReferencedSlide(SVIProjectionSlide* pSlide);
		void removeBackFaceReferencedSlide(SVIProjectionSlide* pSlide);
		

		void removeCapture(SVICaptureData* pCapture);

	protected:
		SVIBool isExist(SVIProjectionSlide* pSlide);
		SVIBool isExistBackFace(SVIProjectionSlide* pSlide);

		void renderQuad(SVIFloat width, SVIFloat height, SVIInt textureId);
		void renderQuad(SVIVector2& offset, SVIVector2& size, SVIInt textureId);
		
		void removeCapture(SVIProjectionSlide* pSlide);
		SVICaptureData* addCapture(SVIProjectionSlide* pSlide);
		SVICaptureData* addBackFaceCapture(SVIProjectionSlide* pSlide);
	private:
		SVICaptureData** 				mCaptures;
		std::vector<SVICaptureData*> mDynamicCaptures;


		std::vector<SVIProjectionSlide*> mReservedCaptures;
		std::vector<SVIProjectionSlide*> mReferencedSlides;
		std::vector<SVIProjectionSlide*> mReferencedBackFaceSlides;
				
		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support
	};

	
};