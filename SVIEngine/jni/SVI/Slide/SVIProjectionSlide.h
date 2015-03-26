#pragma once

#include "../Particle/SVIParticleEffect.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI{
	class SVITexture;
	class SVIBeamedSlide;
	class SVIProjectionSlide;
	
	class SVI3DScene;

	typedef std::list<SVIParticleEffect*>		ParticleEffectList;
	typedef std::vector<SVIProjectionSlide*>		ProjectionSlideList;
	typedef ProjectionSlideList::iterator		ProjectionSlideListIter;

	

	//geometry outfit for poly generation
	struct SVISlideOutfit{
	public:				
		//matrix transforms (translation + rotation)
		SVIMatrix					mLocalTransform;
		SVIMatrix					mGlobalTransform;
		
		//basic properties for geometry
		SVIVector3					mOrigin;
		SVIVector3					mPosition;
		SVIVector3					mPivot;
		SVIVector3					mSize;
		SVIVector3					mScale;
		SVIVector4					mColor;

		//2012-06-21 masterkeaton27@gmail.com
		SVIVector3					mSwipeRatio;

		//2011-11-21 masterkeaton27@gmail.com
		SVIVector3					mDeformRatio;
		
		//2012-01-02 phyuzion@gmail.com
		SVIVector3					mRTERatio;

		//2011-07-22 masterkeaton27@gmail.com
		SVIFloat						mGlobalOpacity;

		//2011-08-08 masterkeaton27@gmail.com
		SVIFloat						mLocalOpacity;

		SVIVector2					mTextureScale;
		SVIInt						mZOrderType;
		SVIInt						mSwipeType;
		SVIInt						mDeformType;
		
        SVIInt						mFilterType;
		SVIInt						mFilterTexture0;
		
		SVIInt						mDeformDirection;
		SVIFloat						mCornerRadius;
		SVIFloat						mBorderWidth;
		SVIVector4					mBorderColor;
		//2011-06-15 masterkeaton27@gmail.com
		//Shadow properties
		SVIFloat						mShadowRadius;
		SVIVector4					mShadowColor;
		SVIVector2					mShadowOffset;
		SVIQuaternion				mRotation; //quaternion rotation
		SVIBool						mNeedUpdate;
		SVIBool						mIsOrthogonal;
		SVIBool						mIsCustomRTE;
		SVIBool						mIsHoldScale;

        //2012/12/08 rareboy0112@facebook.com
        //tesselation
        SVIBool                      mTesselation;
        std::vector<SVIVector2>      mTesselationPointList;
        //2012/12/08 rareboy0112@facebook.com
        //add morphing type
        SVIInt                       mMorphingType;
        SVIFloat                     mMorphingTime;
        SVIFloat                     mMorphingRadius;
        SVIInt                       mMorphingVertexCount;
        SVIBool                      mChangeShape;
        SVIVector4                   mMorphingColor;

		//2011-08-26 masterkeaton27@gmail.com
		//light properties
		SVIInt						mLightType;
		SVIVector2					mLightCenter;
		SVIFloat						mLightAngle;
		SVIVector4					mLightColor;
		SVIFloat						mLightOpacity;
		SVIFloat						mLightRadius;
		SVIFloat						mLightPower;
        
		//2012-11-27 masterkeaton27@gmail.com
		SVIVector4                   mLightDirection;
		SVIVector4                   mViewDirection;
		

		SVIInt						mBlendType;

		//2011.12.09 jongchae.moon
		SVIBool						mEnableAA;
		//

		//2011-11-03 masterkeaton27@gmail.com
		//RT effect type
		SVIInt						mRTEffectType;

		//2011-08-29 masterkeaton27@gmail.com
		//texture sprite properties
		SVIRect						mTextureBackFaceSpriteRegion;
		SVIRect						mTextureSpriteRegion;
		SVIRect						mTextureScaleFitRegion;

		//2012-03-22 masterkeaton27@gmail.com
		SVIBool						mHoldDeformTouchDown;
		SVIVector2					mHoldDeformTouchDownPos;

		//2012-01-10 d.sedukhin
        //touch processing
        SVIBool                      mTouchHappened;

		//2012/03/30 rareboy0112@facebook.com
		SVIBool						mIsBackFaceImageFlip;

		//2012-04-12 masterkeaton27@gmail.com
		SVIBool						mIsBackFaceImageClear;


        SVIBool                      mUseNormalMap;

        std::vector<SVIVector3>      mPathPoints;
	public:
       	 SVIBool isUseNormalMap(){ return mUseNormalMap; }
		 SVIInt  getFilterType(){return mFilterType;}
		SVIBool isOrthogonal(){return mIsOrthogonal;}
		SVIBool isCustomRTE(){return mIsCustomRTE;}
		SVIBool isRound(){ return mCornerRadius <= 0.01f ? SVIFALSE : SVITRUE;}
		SVIBool hasBorder(){return mBorderWidth <= 0.01f ? SVIFALSE: SVITRUE;}
		SVIBool hasShadow(){return mShadowColor.a < 0.001f || mShadowRadius < 0.001f ? SVIFALSE : SVITRUE;}
		SVIBool hasSpotLight();
		SVIBool hasLinearLight();
		SVIBool bindChangeProperties(const SVISlideOutfit& other);

	public:
		void calculrate();
		void normalize();
		void setNeedUpdate(){mNeedUpdate = SVITRUE;}
		void setIdentity();
	};


	class SVICaptureData;
	class SVIGLSurface;
	class SVISlideTextureContainer{
	public:
		SVISlideTextureContainer(SVIGLSurface *surface);
		~SVISlideTextureContainer();
	private:
		SVISlideTextureContainer(const SVISlideTextureContainer& rhs);
		void operator=(const SVISlideTextureContainer& rhs);
	public:
		void setTexture(SVITexture * pTexture);
		void setBackFaceTexture(SVITexture * pTexture);
		
        void setNormalTexture(SVITexture * pTexture);
        void setSpecularTexture(SVITexture * pTexture);
        
        void setCaptureTexture(SVICaptureData * pCaptureData);
		void setBackFaceCaptureTexture(SVICaptureData * pCaptureData);
	public:	
		SVIBool hasTexture();
        SVIBool hasNormalTexture();
        SVIBool hasSpecularTexture();

        SVIBool hasBackFaceTexture();
        SVIBool hasCaptureData();
		SVIBool hasBackFaceCaptureData();
		
		SVIBool isEmpty();
	public:	
		SVITexture * getTexture(){return mTexture;}
		SVITexture * getBackFaceTexture(){return mBackFaceTexture;}
		
        SVITexture * getNormalTexture(){return mNormalTexture;}
        SVITexture * getSpecularTexture(){return mSpecularTexture;}
        
        SVICaptureData * getCaptureTexture(){return mCaptureData;}
		SVICaptureData * getBackFaceCaptureTexture(){return mBackFaceCaptureData;}

		SVISize getSize();
		SVIUInt getTextureID();
        SVIUInt getNormalID();
        SVIUInt getSpecularID();
		SVIUInt getBackFaceTextureID();
		SVIUInt getBackFaceCaptureID();
	public:	
		void release();

	private:
		SVITexture * mTexture;
		SVITexture * mBackFaceTexture;
		
        //2012-12-10 masterkeaton27@gmail.com
        SVITexture * mNormalTexture;
        SVITexture * mSpecularTexture;
        
        SVICaptureData * mCaptureData;
		SVICaptureData * mBackFaceCaptureData;

	SVIGLSurface* 	mSVIGLSurface;
	};

	class SVIRenderPatch;
	class SVIFrameFilm;
	class SVIProjectionSlide{
	public:
		

		SVIProjectionSlide(SVIGLSurface *surface);
		virtual ~SVIProjectionSlide();
	private:
		SVIProjectionSlide(const SVIProjectionSlide& rhs);
		void operator=(const SVIProjectionSlide& rhs);

	public:
		SVIBool initialize(SVIProjectionSlide* parent, SVIBeamedSlide* slide, SVIImage* image, int type);
		void addSubSlide(SVIProjectionSlide* pChild);
		SVIBool removeSubSlide(SVIProjectionSlide* pChild);
		void removeSubSlide(SVIUInt id);
		void removeFromSuperSlide();
		SVIBool insertSubSlide(SVIInt index, SVIProjectionSlide* newSlide);
		SVIBool replaceSubSlide(SVIProjectionSlide* oldSlide, SVIProjectionSlide* newSlide);
		SVIInt getSlideIndex(SVIProjectionSlide* subSlide);

		void detachAll();
		virtual void invalidateLocalOrigin();
		virtual void invalidate(SVIBool bFirstSet = SVIFALSE);
		virtual void invalidate(SVISlideOutfit & newOutfit);
		virtual void initialize();
		virtual void finalize();
		SVIInt getSlideType() { return mSlideType; }

	public:
		void	setRenderTarget(SVIBool value){ mIsRenderTarget = value;}
		void	setAbsoluteRoot(){mIsAbsoluteRoot = SVITRUE;}
		void	setClip(SVIBool isClip){mIsClip = isClip;}
		void	setHidden(SVIBool isHidden){ mIsHidden = isHidden;}

		void	setTexture(SVITexture * texture);
        void	setNormalTexture(SVITexture * texture);
        void	setSpecularTexture(SVITexture * texture);
		void	setBackFaceTexture(SVITexture * texture);
		void	setCaptureTexture(SVICaptureData * pData);
		
        void	setImage(SVIImage* image);
        void	setNormalImage(SVIImage* image);
        void	setSpecularImage(SVIImage* image);
		void	setBackFaceImage(SVIImage* image);

		void	setBackFaceImageFlip(SVIBool isFlip);
		void	setBackFaceImageClear(SVIBool isClear);

		// 2011/11/30 rareboy0112@facebook.com
		void	set3DScene(SVI3DScene * pScene);

		void	setParent(SVIProjectionSlide* parent){mSuperSlide = parent;}
		void	setLink(SVIBeamedSlide* pLinked);
		void	setDepthLevel(SVIUInt level){mDepthLevel = level;}
		void	setSideLevel(SVIUInt level){mSideLevel = level;}
		void	setRenderPatch(SVIRenderPatch* pPatch){mLinkedRenderPatch = pPatch;}

		//2013-06-21 masterkeaton27@gmail.com
        void    addPathPoint(const SVIVector3& point );
        void    clearPathPoints();

		//void	setRTTextureId(SVIInt id){mRTTextureId = id;}
		SVIBool	setOutfit(SVISlideOutfit& outfit);

	public:
		const SVISlideOutfit&		getOutfit(){return mOutfit;}
		const SVIBeamedSlide*		getLink(){ return mBeamedSlide; }
		const SVIProjectionSlide*	getParent(){ return mSuperSlide;}
		SVIUInt				        getSubCount(){return mSubSlideList.size();}
		SVIUInt				        getId(){ return mId; }
		SVIUInt				        getDepthLevel(){return mDepthLevel;}
		SVIUInt				        getSideLevel(){return mSideLevel;}
		const SVIProjectionSlide*	getSub(SVIUInt index){return mSubSlideList[index];}
		SVIRenderPatch*				getRenderPatch(){return mLinkedRenderPatch;}
		SVI3DScene*					get3DScene(){return mAttachedScene;}
		
		SVISlideTextureContainer*	getTextureContainer(){return mTextureContainer;}
		//SVITexture*					getTexture(){return mTexture;}
		//SVIInt						getRTTextureId(){return mRTTextureId;}

		SVIGLSurface*				getSVIGLSurface() {return mSVIGLSurface; }
	public:	
		SVIBool						isTexturedSlide();
		SVIBool						isColoredSlide();
        SVIBool                      isUseNormalMap();
		//2011-08-05 masterkeaton27@gmail.com
		SVIBool						isOrthogonal(){return mOutfit.isOrthogonal();}
		SVIBool						isCustomRTE(){return mOutfit.isCustomRTE();}
		SVIBool						isClip(){return mIsClip;}
		SVIBool						isHidden(){return mIsHidden;}
		SVIBool						isAbsoluteRoot(){return mIsAbsoluteRoot;}
		SVIBool						isTransparent();
		SVIBool						isRoundChanged(){return mIsRoundChanged;}
		SVIBool						isTopMost();
		SVIBool						isBottom();
		SVIBool						isRenderTarget();
		SVIBool						hasClipParent();
		SVIBool						hasZOrderedParent(SVIUInt zOrderType);
		SVIUInt						hasZOrderedParent();
		SVIBool						has3DScene(){return (mAttachedScene != NULL) ? SVITRUE : SVIFALSE;}
	public:
		SVIBool						isGroupHead();

		SVIBool isNeedInvalidate() { return !mIsInvalidated; }
		void resetInvalidate() { mIsInvalidated = SVIFALSE; }

	public:
		//2011-10-18 masterkeaton27@gmail.com
		void bake();
		void rebake();

		//2012-03-22 masterkeaton27@gmail.com
		//this is walk-around function to solve touch down skipping bug.
		void setHoldDeformDown(float x, float y);

		// 2012/06/12 rareboy0112@facebook.com
		// add particle effect
		void				addParticleEffect(SVIParticleEffect* particleEffect);
		SVIBool				removeParticleEffect(SVIParticleEffect* particleEffect);
		void				updateParticleEffect();
		ParticleEffectList* getParticleEffectList() { return mParticleEffectList; }

	protected:
		//2011-10-10 masterkeaton27@gmail.com
		void buildTextureScale();
		void buildColors();
		void buildTransform();
		void buildOrthogonal(SVIBool value);
        
		SVIBool buildOutfit();
		SVIBool buildGeometryOutfit();

		void updateOutfit(SVIBool * pIsColorUpdated,SVIBool * pIsTransformUpdated);


		ProjectionSlideListIter findSlideIter(SVIInt index);
		SVIBool isNeedResetAll();

	protected:

		//2011-11-18 masterkeaton27@gmail.com
		SVISlideTextureContainer*	mTextureContainer;

		//SVITexture*					mTexture;
		//SVIInt						mRTTextureId;

		//slide outfit
		SVISlideOutfit				mOutfit;

	protected:
		static SVIUInt				mUniqueId;
		
		//unique number for identification
		SVIUInt						mId;
	
		//linked beamed slide
		//2011-06-27 optimization test
		SVIRenderPatch*				mLinkedRenderPatch;
		SVIBeamedSlide*				mBeamedSlide;
		SVIInt						mSlideType;

		//basic information for z sorting
		SVIUInt						mDepthLevel;
		SVIUInt						mSideLevel;
		SVIUInt						mSubTotalSlideCount;
				
		//slide hirerachy information
		ProjectionSlideList			mSubSlideList;		
		SVIProjectionSlide*			mSuperSlide;

	//2011-10-18 masterkeaton27@gmail.com
	//saved previous properties
	protected: 
		SVIProjectionSlide*			mSavedParent;
		SVISlideOutfit				mSavedOutfit;

	protected:
		//2011-11-07 masterkeaton27@gmail.com
		SVI3DScene *					mAttachedScene;

	//2011-06-16
	protected: 
		//2011-06-16 masterkeaton27@gmail.com
		//own frame buffer
		SVIBool						mIsAbsoluteRoot;
		
		SVIBool						mIsHidden;
		SVIBool						mIsClip;
		SVIBool						mIsRenderTarget;
		SVIBool						mIsRoundChanged;

		SVIBool						mIsInvalidated;	

		SVIBool						mIsBackFaceImageFlip;
		//2012-04-12 masterkeaton27@gmail.com
		SVIBool						mIsBackFaceImageClear;

		// 2012/06/11 rareboy0112@facebook.com
		// add particle value
		ParticleEffectList*			mParticleEffectList;

		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support

	};
};
