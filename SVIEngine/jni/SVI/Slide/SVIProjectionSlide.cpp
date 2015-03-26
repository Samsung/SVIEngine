#include "../SVICores.h"

#include "SVIBaseSlide.h"
#include "SVIBeamedSlide.h"
#include "SVIProjectionSlide.h"

#include "../BaseType/SVIExtendTypes.h"
#include "../Render/SVITexture.h"
#include "../Render/SVITextureManager.h"
#include "../Render/SVIRenderer.h"
#include "../Render/SVIFrameRenderer.h"
#include "../Render/SVI3DScene.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Particle/SVIParticleEffectManager.h"

#include "../NativeInterface/nativeSVIImage.h"

namespace SVI{
	#define SVI_MAX_PROJECTION_SLIDE_CHILD_COUNT 64

	SVIUInt			SVIProjectionSlide::mUniqueId = 0;

	static const SVIBool DEBUG = SVIFALSE;

	SVIBool SVISlideOutfit::hasSpotLight(){
		return (mLightType == SPOT_LIGHT && mLightRadius > 0.001f && mLightColor.a > 0.001f) ? SVITRUE : SVIFALSE;
	}
	SVIBool SVISlideOutfit::hasLinearLight(){
		return (mLightType == LINEAR_LIGHT&& mLightRadius > 0.001f && mLightColor.a > 0.001f) ? SVITRUE : SVIFALSE;
	}

	SVIBool SVISlideOutfit::bindChangeProperties(const SVISlideOutfit& other){
		SVIBool result = SVIFALSE;
		if (other.mLocalTransform != mLocalTransform){
			mLocalTransform = other.mLocalTransform;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LOCAL TRANSFORM ");
			result = SVITRUE;
		}

		if (other.mGlobalTransform != mGlobalTransform){
			mGlobalTransform = other.mGlobalTransform;
			DEBUG_CHECK_LOGI("CHANGE_CHECK GLOBAL TRANSFORM ");
			result = SVITRUE;
		}

		if (other.mPosition != mPosition){
			mPosition = other.mPosition;
			DEBUG_CHECK_LOGI("CHANGE_CHECK POSITION ");
			result = SVITRUE;
		}

		if (other.mOrigin != mOrigin){
			mOrigin = other.mOrigin;
			DEBUG_CHECK_LOGI("CHANGE_CHECK ORIGIN ");
			result = SVITRUE;
		}

		if (other.mPivot != mPivot){
			mPivot = other.mPivot;
			DEBUG_CHECK_LOGI("CHANGE_CHECK PIVOT ");
			result = SVITRUE;
		}

		if (other.mScale != mScale){
			mScale = other.mScale;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SCALE ");
			result = SVITRUE;
		}

		if (other.mColor != mColor){
			mColor = other.mColor;
			result = SVITRUE;
			DEBUG_CHECK_LOGI("CHANGE_CHECK COLOR ");
		}

		if (other.mSize != mSize){
			mSize = other.mSize;
			DEBUG_CHECK_LOGI("CHANGE_CHECK w=%f,h=%f",mSize.x,mSize.y);
			result = SVITRUE;
		}else {
			DEBUG_CHECK_LOGI("CHANGE_CHECK SVIME w=%f,h=%f",other.mSize.x,other.mSize.y);
		}

		if (other.mRotation != mRotation){
			mRotation = other.mRotation;
			DEBUG_CHECK_LOGI("CHANGE_CHECK ROT x=%f,y=%f,z=%f,w=%f",mRotation.x,mRotation.y,mRotation.z,mRotation.w);
			result = SVITRUE;
		}

		if (other.mCornerRadius != mCornerRadius){
			mCornerRadius = other.mCornerRadius;
			DEBUG_CHECK_LOGI("CHANGE_CHECK CORNER RADIUS r=%f",mCornerRadius);
			result = SVITRUE;
		}

		if (other.mBorderWidth != mBorderWidth){
			mBorderWidth = other.mBorderWidth;
			DEBUG_CHECK_LOGI("CHANGE_CHECK BORDER WIDTH w=%f",mBorderWidth);
			result = SVITRUE;
		}

		if (other.mBorderColor != mBorderColor){
			mBorderColor = other.mBorderColor;
			DEBUG_CHECK_LOGI("CHANGE_CHECK BORDER COLOR r=%f,g=%f,b=%f,a=%f",
				mBorderColor.x,mBorderColor.y,mBorderColor.z,mBorderColor.w);
			result = SVITRUE;
		}


		if (other.mShadowOffset != mShadowOffset){
			mShadowOffset = other.mShadowOffset;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SHADOW Offset x=%f,y=%f",
				mShadowOffset.x,mShadowOffset.y);
			result = SVITRUE;
		}

		if (other.mShadowRadius != mShadowRadius){
			mShadowRadius = other.mShadowRadius;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SHADOW radius =%f",mShadowRadius);
			result = SVITRUE;
		}

		if (other.mShadowColor != mShadowColor){
			mShadowColor = other.mShadowColor;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SHADOW color r=%f,g=%f,b=%f,a=%f",
				mShadowColor.x,mShadowColor.y,mShadowColor.z,mShadowColor.w);
			result = SVITRUE;
		}

		if (other.mTextureScale != mTextureScale){
			mTextureScale = other.mTextureScale;
			DEBUG_CHECK_LOGI("CHANGE_CHECK TEXTURE SCALE color x=%f,y=%f",
				mTextureScale.x,mTextureScale.y);
			result = SVITRUE;
		}

		if (other.mZOrderType != mZOrderType){
			mZOrderType = other.mZOrderType;
			DEBUG_CHECK_LOGI("CHANGE_CHECK TEXTURE SCALE Z ORDER TYPE = %d", mZOrderType);
			result = SVITRUE;
		}

		if (other.mSwipeType != mSwipeType){
			mSwipeType = other.mSwipeType;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SWIPE TYPE = %d", mSwipeType);
			result = SVITRUE;
		}

		if (other.mDeformType != mDeformType){
			mDeformType = other.mDeformType;
			DEBUG_CHECK_LOGI("CHANGE_CHECK DEFORM TYPE = %d", mDeformType);
			result = SVITRUE;
		}

        if (other.mFilterType != mFilterType){
            mFilterType = other.mFilterType;
            DEBUG_CHECK_LOGI("CHANGE_CHECK FILTER TYPE = %d", mFilterType);
            result = SVITRUE;
        }
        

		if (other.mIsOrthogonal != mIsOrthogonal){
			mIsOrthogonal = other.mIsOrthogonal;
			DEBUG_CHECK_LOGI("CHANGE_CHECK ORTHOGONAL TYPE = %s", mIsOrthogonal ? "TRUE" : "FALSE");
			result = SVITRUE;
		}

        if (other.mUseNormalMap != mUseNormalMap){
            mUseNormalMap = other.mUseNormalMap;
            DEBUG_CHECK_LOGI("CHANGE_CHECK NORMALMAP TYPE = %s", mUseNormalMap ? "TRUE" : "FALSE");
            result = SVITRUE;
        }

		if (other.mIsCustomRTE != mIsCustomRTE){
			mIsCustomRTE = other.mIsCustomRTE;
			DEBUG_CHECK_LOGI("CHANGE_CHECK CustomRTE TYPE = %s", mIsCustomRTE ? "TRUE" : "FALSE");
			result = SVITRUE;
		}

		if (other.mIsHoldScale != mIsHoldScale){
			mIsHoldScale = other.mIsHoldScale;
			DEBUG_CHECK_LOGI("CHANGE_CHECK HOLDSCALE TYPE = %s", mIsHoldScale ? "TRUE" : "FALSE");
			result = SVITRUE;
		}

		

		if (other.mGlobalOpacity != mGlobalOpacity){
			mGlobalOpacity = other.mGlobalOpacity;
			DEBUG_CHECK_LOGI("CHANGE_CHECK GLOBAL OPACITY = %f", mGlobalOpacity);
			result = SVITRUE;
		}
		
		if (other.mLocalOpacity != mLocalOpacity){
			mLocalOpacity = other.mLocalOpacity;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LOCAL OPACITY = %f", mLocalOpacity);
			result = SVITRUE;
		}
		
		if (other.mLightType != mLightType){
			mLightType = other.mLightType;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT TYPE = %d", mLightType);
			result = SVITRUE;
		}

		if (other.mLightCenter != mLightCenter){
			mLightCenter = other.mLightCenter;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT CENTER = %.02f,%.02f", mLightCenter.x,mLightCenter.y);
			result = SVITRUE;
		}

		if (other.mLightAngle != mLightAngle){
			mLightAngle = other.mLightAngle;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT ANGLE = %.02f", mLightAngle);
			result = SVITRUE;
		}

		if (other.mLightColor != mLightColor){
			mLightColor = other.mLightColor;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT COLOR = %.02f,  %.02f,  %.02f,  %.02f", 
				mLightColor.x,mLightColor.y,mLightColor.z,mLightColor.w);
			result = SVITRUE;
		}

        if (other.mLightDirection != mLightDirection){
            mLightDirection = other.mLightDirection;
            DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT DIRECTION = %.02f,  %.02f,  %.02f,  %.02f", 
                mLightDirection.x,mLightDirection.y,mLightDirection.z,mLightDirection.w);
            result = SVITRUE;
        }

		if (other.mViewDirection != mViewDirection){
			mViewDirection = other.mViewDirection;
			DEBUG_CHECK_LOGI("CHANGE_CHECK VIEW DIRECTION = %.02f,  %.02f,  %.02f,  %.02f", 
				mViewDirection.x,mViewDirection.y,mViewDirection.z,mViewDirection.w);
			result = SVITRUE;
		}

		if (other.mLightOpacity != mLightOpacity){
			mLightOpacity = other.mLightOpacity;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT OPACITY = %.02f",mLightOpacity);
			result = SVITRUE;
		}

		if (other.mLightRadius != mLightRadius){
			mLightRadius = other.mLightRadius;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT RADIUS = %.02f",mLightRadius);
			result = SVITRUE;
		}

		if (other.mLightPower != mLightPower){
			mLightPower = other.mLightPower;
			DEBUG_CHECK_LOGI("CHANGE_CHECK LIGHT RADIUS = %.02f",mLightPower);
			result = SVITRUE;
		}


		if (other.mBlendType != mBlendType){
			mBlendType = other.mBlendType;
			DEBUG_CHECK_LOGI("CHANGE_CHECK BLEND TYPE = %d",mBlendType);
			result = SVITRUE;
		}

		//2011.12.09 jongchae.moon
		if (other.mEnableAA != mEnableAA){
			mEnableAA = other.mEnableAA;
			DEBUG_CHECK_LOGI("CHANGE_CHECK ANTI-ALIASING = %d",mEnableAA);
			result = SVITRUE;
		}
		//

		//2012-06-21 masterkeaton27@gmail.com
		//added for mobile office 
		if (mSwipeRatio != other.mSwipeRatio){
			mSwipeRatio = other.mSwipeRatio;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SWIPE RATIO = %.02f, %.02f, %.02f",
				mSwipeRatio.x,
				mSwipeRatio.y,
				mSwipeRatio.z
				);
			result = SVITRUE;
		}

		if (mDeformRatio != other.mDeformRatio){
			mDeformRatio = other.mDeformRatio;
			DEBUG_CHECK_LOGI("CHANGE_CHECK DEFORM RATIO = %.02f, %.02f, %.02f",
				mDeformRatio.x,
				mDeformRatio.y,
				mDeformRatio.z
				);
			result = SVITRUE;
		}

		if (mDeformDirection != other.mDeformDirection){
			mDeformDirection = other.mDeformDirection;
			DEBUG_CHECK_LOGI("CHANGE_CHECK DEFORM DIRECTION = %d",
				mDeformDirection);
			result = SVITRUE;
		}
		

		if (mRTERatio != other.mRTERatio){
			mRTERatio = other.mRTERatio;
			DEBUG_CHECK_LOGI("CHANGE_CHECK RTE RATIO = %.02f, %.02f, %.02f",
				mRTERatio.x,
				mRTERatio.y,
				mRTERatio.z
				);
			result = SVITRUE;
		}
				

		if (!(mTextureBackFaceSpriteRegion == other.mTextureBackFaceSpriteRegion)){
			mTextureBackFaceSpriteRegion = other.mTextureBackFaceSpriteRegion;
			DEBUG_CHECK_LOGI("CHANGE_CHECK BACK FACE SPRITE REGION = %.02f, %.02f, %.02f, %.02f",
				mTextureBackFaceSpriteRegion.mOrigin.mX,
				mTextureBackFaceSpriteRegion.mOrigin.mY,
				mTextureBackFaceSpriteRegion.mSize.mWidth,
				mTextureBackFaceSpriteRegion.mSize.mHeight
				);
			result = SVITRUE;
		}

		if (!(mTextureSpriteRegion == other.mTextureSpriteRegion)){
			mTextureSpriteRegion = other.mTextureSpriteRegion;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SPRITE REGION = %.02f, %.02f, %.02f, %.02f",
				mTextureSpriteRegion.mOrigin.mX,
				mTextureSpriteRegion.mOrigin.mY,
				mTextureSpriteRegion.mSize.mWidth,
				mTextureSpriteRegion.mSize.mHeight
			);
			result = SVITRUE;
		}

		if (!(mTextureScaleFitRegion == other.mTextureScaleFitRegion)){
			mTextureScaleFitRegion = other.mTextureScaleFitRegion;
			DEBUG_CHECK_LOGI("CHANGE_CHECK SCALEFIT REGION = %.02f, %.02f, %.02f, %.02f",
				mTextureScaleFitRegion.mOrigin.mX,
				mTextureScaleFitRegion.mOrigin.mY,
				mTextureScaleFitRegion.mSize.mWidth,
				mTextureScaleFitRegion.mSize.mHeight
				);
			result = SVITRUE;
		}
        
        //2013-06-21 masterkeaton27@gmail.com
        if (!(mPathPoints.size() == other.mPathPoints.size())){            
            mPathPoints.clear();

            mPathPoints.assign(other.mPathPoints.begin(), other.mPathPoints.end());
            DEBUG_CHECK_LOGI("CHANGE_CHECK PATH POINT = size %d",mPathPoints.size());
            result = SVITRUE;
        }

		if (!(mRTEffectType == other.mRTEffectType)){
			mRTEffectType = other.mRTEffectType;
			DEBUG_CHECK_LOGI("CHANGE_CHECK RT EFFECT TYPE = %d",mRTEffectType);
			result = SVITRUE;
		}

        if (!(mMorphingType == other.mMorphingType)){
            mMorphingType = other.mMorphingType;
            DEBUG_CHECK_LOGI("CHANGE_CHECK MORPHING TYPE = %d", mMorphingType);
            result = SVITRUE;
        }

        if (!(mMorphingTime == other.mMorphingTime)){
            mMorphingTime = other.mMorphingTime;
            DEBUG_CHECK_LOGI("CHANGE_CHECK MORPHING TIME = %f", mMorphingTime);
            result = SVITRUE;
        }

		//2012-03-22 masterkeaton27@gmail.com
		mHoldDeformTouchDown = other.mHoldDeformTouchDown;
		mHoldDeformTouchDownPos = other.mHoldDeformTouchDownPos;

		mIsBackFaceImageClear = other.mIsBackFaceImageClear;
		mIsBackFaceImageFlip = other.mIsBackFaceImageFlip;
		//LOGE("SVIProjectionSlide::bindChangeProperties x:%f, y:%f", mHoldDeformTouchDownPos.x, mHoldDeformTouchDownPos.y);

		return result;
	}

	//2011-06-17 masterkeaton27@gmail.com

	void SVISlideOutfit::normalize(){		
		mOrigin.x = 0.0f;
		mOrigin.y = 0.0f;
		mOrigin.z = 0.0f;
		mScale.x = 1.0f;
		mScale.y = 1.0f;
		mScale.z = 1.0f;
		mTextureScale.x = 1.0f;
		mTextureScale.y = 1.0f;
		mRotation.setEuler(SVIVector3(0.0f,0.0f,0.0f));
		float fdeltaX = mSize.x * mPivot.x;// * outfit.mScale.x;
		float fdeltaY = mSize.y * mPivot.y;// * outfit.mScale.y;
		mPosition = SVIVector3(fdeltaX ,fdeltaY, 0.0f);
	}

	void SVISlideOutfit::calculrate(){

		SVIMatrix scaleMat, rotationMat, translationMat;
		scaleMat.setIdentity();
		rotationMat.setIdentity();
		translationMat.setIdentity();

		scaleMat.setScaling(mScale); 
		rotationMat.setSVIQuaternion(mRotation);

		translationMat._41 = mPosition.x;
		translationMat._42 = mPosition.y;
		translationMat._43 = mPosition.z;
		translationMat._44 = 1.0f;

		mLocalTransform = rotationMat * scaleMat * translationMat;
	}

	void SVISlideOutfit::setIdentity(){
		mIsHoldScale = SVIFALSE;
		mIsOrthogonal = SVITRUE;
		mIsCustomRTE = SVIFALSE;
        mUseNormalMap = SVIFALSE;

		mTextureScale.x = 1.0f;
		mTextureScale.y = 1.0f;

		mHoldDeformTouchDown = SVIFALSE;
		mHoldDeformTouchDownPos = SVIVector2(0.0f,0.0f);

		mSwipeRatio = SVIVector3(1.0f,1.0f,1.0f);
		mDeformRatio = SVIVector3(0.0f,0.0f,0.0f);
		mRTERatio = SVIVector3(0.0f, 0.0f, 0.0f);

		mShadowColor = SVIVector4(0.0f,0.0f,0.0f,1.0f);
		mShadowOffset.x = 10.0f;
		mShadowOffset.y = 10.0f;
		mShadowRadius = 30.0f;
		
		mCornerRadius = 0.0f;
		mBorderWidth = 0.0f;
		mBorderColor = SVIVector4(0.5f,0.5f,0.5f,1.0f);

		//2011-08-26 masterkeaton27@gmail.com
		//light properties
		mLightType = NO_LIGHT;
		mLightCenter =  SVIVector2(0.5f,0.5f);
		mLightAngle = 0.0f;		
		mLightColor =  SVIVector4(1.0f,1.0f,1.0f,1.0f);
        mLightDirection = SVIVector4(0.0f,0.0f,1.0f,0.0f);
		mViewDirection = SVIVector4(0.0f,0.0f,1.0f,0.0f);
		mLightOpacity = 1.0f;
		mLightRadius = 1.0f;
		mLightPower = 1.0f;
        
		//2012-02-07 masterkeaton27@gmail.com
		mDeformDirection = DEFORM_HORIZONTAL;

		//2011-09-14 masterkeaton27@gmail.com
		mBlendType = BLEND_MULTIPLY;
		mDeformType = DEFORM_NONE;
        mRTEffectType = RT_NONE;

        //2012/12/08 rareboy0112@facebook.com
        mTesselation = SVIFALSE;
        mMorphingType = MORPHING_NONE;
        mMorphingTime = 0.0f;
        mMorphingRadius = 0.0f;
        mMorphingVertexCount = 0;
        mChangeShape = SVIFALSE;
        mMorphingColor = SVIVector4(0.2f, 0.3f, 0.8f, 0.7f);

        //2013-06-26 masterkeaton27@gmail.com
        mFilterType = FILTER_NONE;

		//2012-06-21 masterkeaton27@gmail.com
		mSwipeType = SWIPE_NONE;

		//2011.12.09 jongchae.moon
		mEnableAA = SVIFALSE;
		//

		//2011-08-29 masterkeaton27@gmail.com
		//sprite texture properties
		mTextureBackFaceSpriteRegion = SVIRect(0.0f,0.0f,1.0f,1.0f);
		mTextureSpriteRegion =  SVIRect(0.0f,0.0f,1.0f,1.0f);
		mTextureScaleFitRegion =  SVIRect(0.0f,0.0f,1.0f,1.0f);

        //2013-06-21 masterkeaton27@gmail.com
        mPathPoints.clear();

		mLocalTransform.setIdentity();
		mGlobalTransform.setIdentity();

		mOrigin = SVIVector3(0.0f,0.0f,0.0f);
		mPosition = SVIVector3(0.0f,0.0f,0.0f);
		mPivot = SVIVector3(0.5f,0.5f,0.5f);
		mScale = SVIVector3(1.0f,1.0f,1.0f);
		mSize = SVIVector3(1.0f,1.0f,1.0f);
		mColor = SVIVector4(0.0f,0.0f,0.0f,1.0f);
		mRotation.setIdentity();

		mGlobalOpacity = 1.0f;
		mLocalOpacity = 1.0f;

		mNeedUpdate = SVIFALSE;
	}



	SVISlideTextureContainer::SVISlideTextureContainer(SVIGLSurface *surface) : 
		mTexture(NULL),
		mBackFaceTexture(NULL),

        mNormalTexture(NULL),
        mSpecularTexture(NULL),

		mCaptureData(NULL),
		mBackFaceCaptureData(NULL){
		mSVIGLSurface = surface;
		}

	SVISlideTextureContainer::~SVISlideTextureContainer(){
        release();
		mSVIGLSurface = NULL;
    }

	SVISlideTextureContainer::SVISlideTextureContainer(const SVISlideTextureContainer& rhs){}
	void SVISlideTextureContainer::operator=(const SVISlideTextureContainer& rhs){}
    void SVISlideTextureContainer::setTexture(SVITexture * pTexture){
        if (pTexture == NULL) SVI_SVIFE_DELETE(mTexture);      
        mTexture = pTexture;
    }

    void SVISlideTextureContainer::setBackFaceTexture(SVITexture * pTexture){
        if (pTexture == NULL) SVI_SVIFE_DELETE(mBackFaceTexture);
        mBackFaceTexture = pTexture;
    }

    void SVISlideTextureContainer::setNormalTexture(SVITexture * pTexture){
        if (pTexture == NULL) SVI_SVIFE_DELETE(mNormalTexture);
        mNormalTexture = pTexture;
    }

    void SVISlideTextureContainer::setSpecularTexture(SVITexture * pTexture){
        if (pTexture == NULL) SVI_SVIFE_DELETE(mSpecularTexture);
        mSpecularTexture = pTexture;
    }

    void SVISlideTextureContainer::setCaptureTexture(SVICaptureData * pCaptureData){
        if (pCaptureData == NULL) {
            if (mCaptureData != NULL){
                SVI_SVIFE_RELEASE(mCaptureData);
            } 
			return;
		}
		if (mCaptureData == NULL){
			mCaptureData = pCaptureData;
			SVI_SVIFE_ADDREF(mCaptureData);		
		}else if (mCaptureData != pCaptureData){
			SVI_SVIFE_RELEASE(mCaptureData);
			mCaptureData = pCaptureData;
			SVI_SVIFE_ADDREF(mCaptureData);		
		}else{

		}		
	}

	void SVISlideTextureContainer::setBackFaceCaptureTexture(SVICaptureData * pCaptureData){
		if (pCaptureData == NULL) {
			if (mBackFaceCaptureData != NULL){
				SVI_SVIFE_RELEASE(mBackFaceCaptureData);
			} 
			return;
		}
		if (mBackFaceCaptureData == NULL){
			mBackFaceCaptureData = pCaptureData;
			SVI_SVIFE_ADDREF(mBackFaceCaptureData);		
		}else if (mBackFaceCaptureData != pCaptureData){
			SVI_SVIFE_RELEASE(mBackFaceCaptureData);
			mBackFaceCaptureData = pCaptureData;
			SVI_SVIFE_ADDREF(mBackFaceCaptureData);		
		}else{

		}		
	}

	void SVISlideTextureContainer::release(){
		SVI_SVIFE_RELEASE(mCaptureData);
		SVI_SVIFE_RELEASE(mBackFaceCaptureData);

		if (mTexture != NULL){
			mTexture->setImage(NULL);
			mSVIGLSurface->getTextureManager()->remove(mTexture);
			SVI_SVIFE_DELETE(mTexture);
		}

        if (mNormalTexture != NULL){
            mNormalTexture->setImage(NULL);
            mSVIGLSurface->getTextureManager()->remove(mNormalTexture);
            SVI_SVIFE_DELETE(mNormalTexture);
        }

        if (mSpecularTexture != NULL){
            mSpecularTexture->setImage(NULL);
            mSVIGLSurface->getTextureManager()->remove(mSpecularTexture);
            SVI_SVIFE_DELETE(mSpecularTexture);
        }

		if (mBackFaceTexture != NULL){
			mBackFaceTexture->setImage(NULL);
			mSVIGLSurface->getTextureManager()->remove(mBackFaceTexture);
			SVI_SVIFE_DELETE(mBackFaceTexture);
		}
	}

	SVIBool SVISlideTextureContainer::hasTexture(){
		return mTexture != NULL ? SVITRUE : SVIFALSE;
	}

    SVIBool SVISlideTextureContainer::hasNormalTexture(){
        return mNormalTexture != NULL ? SVITRUE : SVIFALSE;
    }

    SVIBool SVISlideTextureContainer::hasSpecularTexture(){
        return mSpecularTexture != NULL ? SVITRUE : SVIFALSE;
    }

	SVIBool SVISlideTextureContainer::hasCaptureData(){
		return mCaptureData != NULL ? SVITRUE : SVIFALSE;
	}

	SVIBool SVISlideTextureContainer::hasBackFaceCaptureData(){
		return mBackFaceCaptureData != NULL ? SVITRUE : SVIFALSE;
	}

	SVIBool SVISlideTextureContainer::hasBackFaceTexture(){
		return mBackFaceTexture != NULL ? SVITRUE : SVIFALSE;
	}

	SVIBool SVISlideTextureContainer::isEmpty(){
		return (!hasTexture() && !hasCaptureData()) ? SVITRUE : SVIFALSE;
	}

	SVISize SVISlideTextureContainer::getSize(){
		if (hasCaptureData()){
			//return SVISize(mCaptureData->getSize().x,mCaptureData->getSize().y);
		}
		if (mTexture == NULL) return SVISize(1.0f,1.0f);
		return SVISize(mTexture->getWidth(),mTexture->getHeight());		
	}

	SVIUInt SVISlideTextureContainer::getTextureID(){
		if (hasCaptureData()){
			return mCaptureData->getId();
		}
		return mTexture != NULL ? mTexture->getTextureId() : 0;
	}


    SVIUInt SVISlideTextureContainer::getNormalID(){
        return mNormalTexture != NULL ? mNormalTexture->getTextureId() : 0;
    }

    SVIUInt SVISlideTextureContainer::getSpecularID(){
        return mSpecularTexture != NULL ? mSpecularTexture->getTextureId() : 0;
    }

	SVIUInt SVISlideTextureContainer::getBackFaceCaptureID(){
		return hasBackFaceCaptureData() ? mBackFaceCaptureData->getId() : 0;
	}

	SVIUInt SVISlideTextureContainer::getBackFaceTextureID(){
		return mBackFaceTexture != NULL ? mBackFaceTexture->getTextureId() : 0;
	}

	SVIProjectionSlide::SVIProjectionSlide(SVIGLSurface *surface):
	mBeamedSlide(NULL),
		mSuperSlide(NULL),
		mSavedParent(NULL),
	mTextureContainer(NULL),
	mLinkedRenderPatch(NULL),
	mAttachedScene(NULL),
	//mTexture(NULL),
	mDepthLevel(0),
	mSideLevel(0),
	//mRTTextureId(-1),
	mIsRenderTarget(SVIFALSE),
	mIsClip(SVIFALSE),
	mSubTotalSlideCount(0),
	mIsRoundChanged(SVIFALSE),
	mIsAbsoluteRoot(SVIFALSE),
	mSlideType(BASE_SLIDE), 
	mIsInvalidated(SVIFALSE),
	mIsBackFaceImageFlip(SVITRUE),
	mIsBackFaceImageClear(SVIFALSE),
	mParticleEffectList(NULL) {
		memset(&mOutfit,0,sizeof(SVISlideOutfit));
		memset(&mSavedOutfit,0,sizeof(SVISlideOutfit));
		mSavedOutfit = mOutfit;

		mOutfit.setIdentity();
		mId = mUniqueId++;
		mSVIGLSurface=surface;//Multi-Instance-Support
		mTextureContainer = new SVISlideTextureContainer(mSVIGLSurface);
	}


	SVIProjectionSlide::SVIProjectionSlide(const SVIProjectionSlide& rhs){}
	void SVIProjectionSlide::operator=(const SVIProjectionSlide& rhs){}

	void SVIProjectionSlide::updateOutfit(SVIBool * pIsColorUpdated,SVIBool * pIsTransformUpdated){
		if (mBeamedSlide == NULL) return;

		SVIInt changedSize = mBeamedSlide->getChangePropertyListSize();
		if (!changedSize) return;

		

		SVISlideOutfit * pOutfit = &mOutfit;	

		mOutfit.setNeedUpdate();

		ChangePropertyList & changedList = mBeamedSlide->getChangePropertyList();

		SVIBool bNeedUpdateColor = SVITRUE;
		SVIBool bNeedUpdatePosition = SVIFALSE;
		SVIBool bNeedUpdateOrthogonal = SVIFALSE;
		SVIBool bNeedUpdateImageScale = SVIFALSE;

		*pIsColorUpdated = SVIFALSE;
		*pIsTransformUpdated = SVIFALSE;

		
		for (int n = 0; n < changedSize; ++n){
			SVIInt type = changedList[n];
			switch (type){
				//2011-10-10 masterkeaton27@gmail.com
				case IMAGESCALE_TYPE:
					bNeedUpdatePosition = SVITRUE;
				break;

				//2011-08-26 masterkeaton27@gmail.com
				//light properties
				case LIGHT_POWER:
				case LIGHT_ANGLE:
				case LIGHT_COLOR:
				case LIGHT_OFFSET:
				case LIGHT_OPACITY:
				case LIGHT_RADIUS:
				case LIGHT_TYPE:
					pOutfit->mLightPower = mBeamedSlide->getLightPower();
					pOutfit->mLightOpacity = mBeamedSlide->getLightOpacity();
					pOutfit->mLightColor.x = mBeamedSlide->getLightColor().mR;
					pOutfit->mLightColor.y = mBeamedSlide->getLightColor().mG;
					pOutfit->mLightColor.z = mBeamedSlide->getLightColor().mB;
					pOutfit->mLightColor.w = mBeamedSlide->getLightColor().mA * pOutfit->mLightOpacity;
					pOutfit->mLightAngle = mBeamedSlide->getLightAngle();					
					pOutfit->mLightRadius = mBeamedSlide->getLightRadius();
					pOutfit->mLightType = mBeamedSlide->getLightType();
					pOutfit->mLightCenter.x = mBeamedSlide->getLightOffset().mX;
					pOutfit->mLightCenter.y = mBeamedSlide->getLightOffset().mY;
					break;

				//2012-11-24 masterkeaton27@gmail.com
                case LIGHT_DIRECTION:
                    pOutfit->mLightDirection = mBeamedSlide->getLightDirection();
                    break;

				//2012-11-27 masterkeaton27@gmail.com
				case VIEW_DIRECTION:
					pOutfit->mViewDirection = mBeamedSlide->getViewDirection();
					break;

				//2012-01-06 masterkeaton27@gmail.com
				case BACKFACE_TEXTURE_REGION:
					pOutfit->mTextureBackFaceSpriteRegion = mBeamedSlide->getBackFaceTextureRegion();
					bNeedUpdatePosition = SVITRUE;
				break;

				//2011-08-29 masterkeaton27@gmail.com
				case TEXTURE_REGION:
					pOutfit->mTextureSpriteRegion = mBeamedSlide->getTextureRegion();					
					bNeedUpdatePosition = SVITRUE;
				break;

				//2011-10-11 masterkeaton27@gmail.com
				case TEXTURE_SCALEFIT_REGION:
					//pOutfit->mTextureSpriteRegion = mBeamedSlide->getTextureScaleFitRegion();
					bNeedUpdatePosition = SVITRUE;
				break;

				//2011-08-16 masterkeaton27@gmail.com
				//change color properties
				case SHADOW_COLOR: 
				case SHADOW_OPACITY:
				case BORDER_COLOR: 
				case OPACITY: 				
				case BGCOLOR:
					pOutfit->mColor.x = mBeamedSlide->getBgColor().mR;
					pOutfit->mColor.y = mBeamedSlide->getBgColor().mG;
					pOutfit->mColor.z = mBeamedSlide->getBgColor().mB;
					pOutfit->mColor.w = mBeamedSlide->getBgColor().mA;							
					pOutfit->mBorderColor.x = mBeamedSlide->getBorderColor().mR;
					pOutfit->mBorderColor.y = mBeamedSlide->getBorderColor().mG;
					pOutfit->mBorderColor.z = mBeamedSlide->getBorderColor().mB;
					pOutfit->mBorderColor.w = mBeamedSlide->getBorderColor().mA;
					pOutfit->mShadowColor.x = mBeamedSlide->getShadowColor().mR;
					pOutfit->mShadowColor.y = mBeamedSlide->getShadowColor().mG;
					pOutfit->mShadowColor.z = mBeamedSlide->getShadowColor().mB;
					pOutfit->mShadowColor.w = mBeamedSlide->getShadowColor().mA * mBeamedSlide->getShadowOpacity();
					pOutfit->mLocalOpacity = mBeamedSlide->getOpacity();

					bNeedUpdateColor = SVITRUE;
					break;

				case CLIP_SUBSLIDE: 							
					setClip(mBeamedSlide->getClipSubSlide());
					break;

				case HIDDEN: 
					setHidden(mBeamedSlide->getHidden());	
					break;

				case CORNER_RADIUS: 
					//2011-07-12 masterkeaton27@gmail.com
					//corner radius check
					if (pOutfit->mCornerRadius != mBeamedSlide->getCornerRadius()){
						mIsRoundChanged = SVITRUE;
					}else mIsRoundChanged = SVIFALSE;

					pOutfit->mCornerRadius = 
						mBeamedSlide->getCornerRadius();		
					break;

				case BORDER_WIDTH: 
					pOutfit->mBorderWidth = mBeamedSlide->getBorderWidth();	
					break;

				case SHADOW_RADIUS: 
					pOutfit->mShadowRadius = mBeamedSlide->getShadowRadius();
					break;

				case SHADOW_OFFSET: 
					pOutfit->mShadowOffset.x = mBeamedSlide->getShadowOffset().mX;
					pOutfit->mShadowOffset.y = mBeamedSlide->getShadowOffset().mY;
					//LOGE("X %.02f Y %.02f",pOutfit->mShadowOffset.x,pOutfit->mShadowOffset.y);
					//bNeedUpdatePosition = SVITRUE;
					break;			

                case USENORMALMAP:
                    pOutfit->mUseNormalMap = mBeamedSlide->getUseNormalMap();
                    break;

				case ORTHOGONAL:
					pOutfit->mIsOrthogonal = mBeamedSlide->getOrthogonal();
					bNeedUpdateOrthogonal = SVITRUE;
					break;

				case CUSTOM_RTE:
					pOutfit->mIsCustomRTE = mBeamedSlide->getCustomRTE();
					break;

				case HOLD_SCALE:
					pOutfit->mIsHoldScale = mBeamedSlide->getHoldScale();
					bNeedUpdatePosition = SVITRUE;
					break;

				case SWIPE_RATIO:
					//2012-06-21
					pOutfit->mSwipeRatio = mBeamedSlide->getSwipeRatio();
					break;

				case DEFORM_RATIO:
					//2012-01-10 d.sedukhin
					//touch processing
					pOutfit->mTouchHappened = SVITRUE;
					pOutfit->mDeformRatio = mBeamedSlide->getDeformRatio();
					break;

				case RTE_RATIO:
					pOutfit->mRTERatio = mBeamedSlide->getRTERatio();
					break;

				case POSITION: 
				case REGION: 
				case PIVOT_POINT: 					
				case ZPOSITION: 					
				case ROTATION: 
				case SCALE:
					bNeedUpdatePosition = SVITRUE;
					break;

				//2012-02-07
				case DEFORM_DIRECTION : 
					pOutfit->mDeformDirection = (SVIInt)mBeamedSlide->getDeformDirection();
					bNeedUpdatePosition = SVITRUE;
					break;

				//2012-06-21 masterkeaton27@gmail.com
				case SWIPE_TYPE :
					pOutfit->mSwipeType = (SVIInt)mBeamedSlide->getSwipeType();
					bNeedUpdatePosition = SVITRUE;
					break;

				//2011-11-18 masterkeaton27@gmail.com
				case DEFORM_TYPE :
					pOutfit->mDeformType = (SVIInt)mBeamedSlide->getDeformType();
					bNeedUpdatePosition = SVITRUE;
					break;

				//2013-06-26 masterkeaton27@gmail.com
				case FILTER_TYPE:
					pOutfit->mFilterType = (SVIInt)mBeamedSlide->getFilterType();					
					bNeedUpdatePosition = SVITRUE;				
					DEBUG_CHECK_LOGE("ProjectionSlide FILTER_TYPE Filtered");
					break;

				//2012/12/08 rareboy0112@facebook.com
				case TESSELATION :
					pOutfit->mTesselation = SVITRUE;

					//copy tesselation point list
					pOutfit->mTesselationPointList.clear();
					pOutfit->mTesselationPointList.reserve(mBeamedSlide->getTesselationPointList().size());


					std::copy(mBeamedSlide->getTesselationPointList().begin(), mBeamedSlide->getTesselationPointList().end(),
						std::back_inserter(pOutfit->mTesselationPointList));

					bNeedUpdatePosition = SVITRUE;
					break;

				//2012/12/10 rareboy0112@facebook.com
				case MORPHING_TIME :
					pOutfit->mMorphingTime = mBeamedSlide->getMorphingTime();
					pOutfit->mChangeShape = SVITRUE;
					break;

				//2012/12/10 rareboy0112@facebook.com
				case MORPHING_RADIUS :
					pOutfit->mMorphingRadius = mBeamedSlide->getMorphingRadius();
					break;

				//2012/12/21 rareboy0112@facebook.com
				case MORPHING_VERTEX_COUNT :
					pOutfit->mMorphingVertexCount = mBeamedSlide->getMorphingVertexCount();
					break;

				//2012/12/21 rareboy0112@facebook.com
				case MORPHING_COLOR :
					pOutfit->mMorphingColor.x = mBeamedSlide->getMorphingColor().mR;
					pOutfit->mMorphingColor.y = mBeamedSlide->getMorphingColor().mG;
					pOutfit->mMorphingColor.z = mBeamedSlide->getMorphingColor().mB;
					pOutfit->mMorphingColor.w = mBeamedSlide->getMorphingColor().mA;
					break;

				//2012/12/10 rareboy0112@facebook.com
				case MORPHING_TYPE :
					pOutfit->mMorphingType = (SVIInt)mBeamedSlide->getMorphingType();
					break;

				case ZORDER_TYPE : 
					pOutfit->mZOrderType = (SVIInt)mBeamedSlide->getZOrderType();
					bNeedUpdatePosition = SVITRUE;
					break;
					
				//2011-09-14 masterkeaton27@gmail.com
				case BLEND_TYPE :
					pOutfit->mBlendType = mBeamedSlide->getBlendType();
					break;

				//2011.12.09 jongchae.moon
				case ANTI_ALIASING:
					pOutfit->mEnableAA = mBeamedSlide->getAntiAliasing();
					break;
				//

				//2011-11-03 masterkeaton27@gmail.com
				case RT_EEFECT_TYPE :
					pOutfit->mRTEffectType = mBeamedSlide->getRTEffectType();
					
					break;

				// 2011/11/30 rareboy0112@facebook.com
				case MODEL_POSITION :
					if(mAttachedScene) {
						SVIVector3 position = mBeamedSlide->getModelPosition();
						mAttachedScene->getRootSceneNode()->setPosition(position);
					}
					break;

				case MODEL_ROTATION :
					if(mAttachedScene) {
						SVIVector4 rotation = mBeamedSlide->getModelRotation();
						mAttachedScene->getRootSceneNode()->setRotation(rotation);
					}
					break;

				// 2011/12/01 rareboy0112@facebook.com
				case MODEL_SCALE :
					if(mAttachedScene) {
						SVIVector3 scale = mBeamedSlide->getModelScale();
						mAttachedScene->getRootSceneNode()->setScale(scale);
					}
					break;

				// 2012/06/22 rareboy0112@facebook.com
				case PARTICLE_EFFECT :

					if( mBeamedSlide->getParticleEffectList().size() > 0 ) {
						SVIParticleEffect* effect = mBeamedSlide->getParticleEffectList().front();
						if( effect != NULL ) {
							mBeamedSlide->getParticleEffectList().pop_front();
							
							if( mParticleEffectList == NULL ) {
								mParticleEffectList = new ParticleEffectList();
							}
							mParticleEffectList->push_back(effect);
							//addParticleEffect(effect);
						}
					}
					break;
			}
		}

		if (bNeedUpdateOrthogonal){
			buildOrthogonal(pOutfit->mIsOrthogonal);
		}
		
		//2011-07-22 masterkeaton27@gmail.com
		//alpha accumulation
		if (bNeedUpdateColor){
			if (mSuperSlide != NULL){
				const SVISlideOutfit& parentOutfit = mSuperSlide->getOutfit();				
				SVIFloat parentAlpha = parentOutfit.mGlobalOpacity;
				SVIFloat localAlpha = pOutfit->mLocalOpacity;				 
				pOutfit->mGlobalOpacity = parentAlpha * localAlpha;
			}else{
				pOutfit->mGlobalOpacity = pOutfit->mLocalOpacity;
			}		

			//2011-07-22 masterkeaton27@gmail.com
			//hold alpha without parent's alpha efficiency
			if (mBeamedSlide->getHoldOpacity()){
				pOutfit->mGlobalOpacity = pOutfit->mLocalOpacity;
			}
						
			//2011-10-28 masterkeaton27@gmail.com
			//remove alpha operation (SDS requested)
			pOutfit->mBorderColor.a = mBeamedSlide->getBorderColor().mA * pOutfit->mGlobalOpacity;
			pOutfit->mShadowColor.a = mBeamedSlide->getShadowColor().mA * mBeamedSlide->getShadowOpacity() * pOutfit->mColor.w * pOutfit->mGlobalOpacity;
			
			*pIsColorUpdated = SVITRUE;
		}
		
		if (bNeedUpdatePosition){
			*pIsTransformUpdated = SVITRUE;
		}		
	}

	//2011-10-10 masterkeaton27@gmail.com
	//texture scale to fit
	void SVIProjectionSlide::buildTextureScale(){

		if (mTextureContainer == NULL || (mTextureContainer != NULL && mTextureContainer->isEmpty())) return;

		SVISize size = mTextureContainer->getSize();
		
		SVIFloat fSlideWidth = mOutfit.mSize.x * mOutfit.mScale.x;
		SVIFloat fSlideHeight = mOutfit.mSize.y * mOutfit.mScale.y;
		SVIFloat fSlideBase = (fSlideWidth < fSlideHeight) ? fSlideWidth : fSlideHeight;
		SVIFloat fSlideMax = (fSlideWidth > fSlideHeight) ? fSlideWidth : fSlideHeight;
		SVIBool isSlideWider = (fSlideWidth > fSlideHeight) ? SVITRUE : SVIFALSE;

		//texture size cant be zero area.
		SVIFloat fImageX = size.mWidth;
		SVIFloat fImageY = size.mHeight;
		SVIFloat fImageBase = (fImageX < fImageY) ? fImageX : fImageY;
		SVIFloat fImageMax = (fImageX > fImageY) ? fImageX : fImageY;
		SVIBool isImageWider = (fImageX > fImageY) ? SVITRUE : SVIFALSE;

		SVIFloat fXRatio = 1.0f;
		SVIFloat fYRatio = 1.0f;
		SVIFloat fXGap = 0.0f;
		SVIFloat fYGap = 0.0f;

		SVIBool isImageBigger = ((fImageX * fImageY) > (fSlideWidth * fSlideHeight)) ? SVITRUE : SVIFALSE;

		SVIVector2 vOffset = SVIVector2(0.0f,0.0f);
		SVIVector2 vSize = SVIVector2(1.0f,1.0f);
		SVIRect region = mBeamedSlide->getTextureScaleFitRegion();
				
		float slideRatio = isSlideWider ? ((fSlideHeight > fSlideWidth) ? fSlideWidth : fSlideHeight) : 
			((fSlideHeight > fSlideWidth) ? fSlideHeight : fSlideWidth);
		
		float imageRatio = fImageMax;//isImageWider ? ((fImageY > fImageX) ? fImageX : fImageY) : ((fImageY > fImageX) ? fImageY : fImageX);
		switch (mBeamedSlide->getImageScaleType()){

			case MATRIX: 				
				//convert image space to unit space
				//vOffset = SVIVector2(region.mOrigin.mX/fImageX,region.mOrigin.mY/fImageY);
				//vSize = SVIVector2(region.mSize.mWidth/fImageX,region.mSize.mHeight/fImageY);
				vOffset = SVIVector2(region.mOrigin.mX/fSlideWidth,region.mOrigin.mY/fSlideHeight);
				vSize = SVIVector2(region.mSize.mWidth/fSlideWidth,region.mSize.mHeight/fSlideHeight);
				
				//LOGE("MATRIX image %.02f %.02f",fImageX,fImageY);
				//LOGE("MATRIX region origin %.02f %.02f",region.mOrigin.mX,region.mOrigin.mY);
				//LOGE("MATRIX region size %.02f %.02f",region.mSize.mWidth,region.mSize.mHeight);
				//LOGE("MATRIX Offset %.02f %.02f",vOffset.x,vOffset.y);
				//LOGE("MATRIX Size %.02f %.02f",vSize.x,vSize.y);
			break;

			case FIT_XY: 
				vSize = SVIVector2(1.0f,1.0f); 	
				break;

			case FIT_START: 				

				if (fImageX > fSlideWidth) {
					fImageX *= fSlideBase / fImageMax;					
				}else{
					//fImageX = fSlideBase;
				}
				if (fImageY > fSlideHeight) {
					fImageY *= fSlideBase / fImageMax;					
				}else{
					//fImageY = fSlideBase;
				}				
				fXRatio = fImageX / fSlideWidth;
				fYRatio = fImageY / fSlideHeight;
				vSize = SVIVector2(fXRatio,fYRatio);
				break;

			case FIT_CENTER: 
				if (fImageX > fSlideWidth) {
					fImageX *= fSlideBase / fImageMax;					
				}else{
					//fImageX = fSlideBase;
				}
				if (fImageY > fSlideHeight) {
					fImageY *= fSlideBase / fImageMax;					
				}else{
					//fImageY = fSlideBase;
				}				
				fXRatio = fImageX / fSlideWidth;
				fYRatio = fImageY / fSlideHeight;
				fXGap = (fSlideWidth - fImageX)/fSlideWidth;
				fYGap = (fSlideHeight - fImageY)/fSlideHeight;
				vOffset = SVIVector2(fXGap * 0.5f,fYGap * 0.5f); 	
				vSize = SVIVector2(fXRatio,fYRatio);
				break;

			case FIT_END: 
				if (fImageX > fSlideWidth) {
					fImageX *= fSlideBase / fImageMax;					
				}else{
					//fImageX = fSlideBase;
				}
				if (fImageY > fSlideHeight) {
					fImageY *= fSlideBase / fImageMax;					
				}else{
					//fImageY = fSlideBase;
				}				
				fXRatio = fImageX / fSlideWidth;
				fYRatio = fImageY / fSlideHeight;
				fXGap = (fSlideWidth - fImageX)/fSlideWidth;
				fYGap = (fSlideHeight - fImageY)/fSlideHeight;
				vOffset = SVIVector2(fXGap,fYGap); 	
				vSize = SVIVector2(fXRatio,fYRatio);
				break;

			case CENTER: 				
				fXRatio = fImageX / fSlideWidth;
				fYRatio = fImageY / fSlideHeight;
				fXGap = ((fImageX - fSlideWidth) * 0.5f) / fSlideWidth;
				fYGap = ((fImageY - fSlideHeight) * 0.5f) / fSlideHeight;
				vOffset = SVIVector2(-fXGap,-fYGap); 	
				vSize = SVIVector2(fXRatio,fYRatio);
				break;

			case CENTER_CROP: 
				if (fImageX > fSlideWidth) {
					fImageX *= fSlideMax / fImageMax;					
				}else{
					fImageX = fSlideMax;
				}
				if (fImageY > fSlideHeight) {
					fImageY *= fSlideMax / fImageMax;					
				}else{
					fImageY = fSlideMax;
				}				
				fXRatio = fImageX / fSlideWidth;
				fYRatio = fImageY / fSlideHeight;
				fXGap = (fSlideWidth - fImageX)/fSlideWidth;
				fYGap = (fSlideHeight - fImageY)/fSlideHeight;
				vOffset = SVIVector2(fXGap * 0.5f,fYGap * 0.5f); 	
				vSize = SVIVector2(fXRatio,fYRatio);
				break;

			case CENTER_OUTSIDE:
				if (fImageX > fImageY){
					if (fSlideWidth > fSlideHeight){						 
						if (fSlideHeight < fImageY){
							fXRatio = 1.0f;
							fYRatio = fSlideWidth / fImageX;
							fXGap = 0.0f;
							fYGap = (fImageY - fSlideHeight) / fImageY * 0.5f;
						}else{
							fXRatio = fSlideHeight / fImageY;
							fYRatio = 1.0f;
							fXGap = (fImageX * fXRatio - fSlideWidth) / fImageX * fXRatio * 0.5f;
							fYGap = 0.0f;							
						}
					}else{						
						fXRatio = fSlideHeight / fImageY;
						fYRatio = 1.0f;
						fXGap = (fImageX * fXRatio - fSlideWidth) / (fImageX * fXRatio) * 0.5f;		
						fYGap = 0.0f;													
					}
				}else{										
					fXRatio = 1.0f;
					fYRatio = fSlideWidth / fImageX;
					fXGap = 0.0f;
					fYGap = (fImageY * fYRatio - fSlideHeight) / fImageY * fYRatio * 0.5f;								
				}			

				vOffset = SVIVector2(fXGap,fYGap); 	
				vSize = SVIVector2(fXRatio,fYRatio);
				break;

			case CENTER_INSIDE:				
				if (fSlideWidth > fSlideHeight){
					fYRatio = fImageY / fSlideHeight;
					fXRatio = fYRatio;
					fXGap = (fSlideWidth-fImageX) / fSlideWidth * 0.5f;  
					fYGap = 0.0f;
				}else{
					fXRatio = fImageX / fSlideWidth;
					fYRatio = fXRatio;
					fXGap = 0.0f;  
					fYGap = (fSlideHeight-fImageY) / fSlideHeight * 0.5f;
				}				
				vOffset = SVIVector2(fXGap,fYGap); 	
				vSize = SVIVector2(fXRatio,fYRatio);
			break;

			default: return;
		}

		mOutfit.mTextureScaleFitRegion.mOrigin.mX = vOffset.x;
		mOutfit.mTextureScaleFitRegion.mOrigin.mY = vOffset.y;
		mOutfit.mTextureScaleFitRegion.mSize.mWidth = vSize.x;
		mOutfit.mTextureScaleFitRegion.mSize.mHeight = vSize.y;
	}

	void SVIProjectionSlide::buildOrthogonal(SVIBool value){
		for (SVIUInt n = 0; n <mSubSlideList.size(); ++n){
			SVIProjectionSlide * child = mSubSlideList[n];
			child->mOutfit.mIsOrthogonal = value;
            child->buildOrthogonal(value);
        }
    }

    SVIBool SVIProjectionSlide::buildGeometryOutfit(){
        if (mBeamedSlide == NULL) {
            LOGE("SVIProjectionSlide::buildOutfit()-mBeamedSlide == NULL");
            return SVIFALSE;
        }

        mOutfit.mUseNormalMap = mBeamedSlide->getUseNormalMap();
        mOutfit.mIsOrthogonal = mBeamedSlide->getOrthogonal();
        mOutfit.mIsBackFaceImageFlip = mIsBackFaceImageFlip;
        mOutfit.mIsBackFaceImageClear = mIsBackFaceImageClear;

        mOutfit.mSwipeRatio = mBeamedSlide->getSwipeRatio();
		mOutfit.mDeformRatio = mBeamedSlide->getDeformRatio();
		mOutfit.mRTERatio = mBeamedSlide->getRTERatio();

		mOutfit.mRotation = SVIQuaternion(mBeamedSlide->getRotation());

		mOutfit.mPivot.x = mBeamedSlide->getPivotPoint().mX;
		mOutfit.mPivot.y = mBeamedSlide->getPivotPoint().mY;
		mOutfit.mPivot.z = 0.0f;

		mOutfit.mScale = mBeamedSlide->getScale(); // this will lose original w factor

		mOutfit.mSize.x = mBeamedSlide->getRegion().mSize.mWidth;
		mOutfit.mSize.y = mBeamedSlide->getRegion().mSize.mHeight;
		mOutfit.mSize.z = 0.0f;		

		//2011-06-29 masterkeaton27@gmail.com
		mOutfit.mOrigin.x = mBeamedSlide->getRegion().getOrigin().mX;
		mOutfit.mOrigin.y = mBeamedSlide->getRegion().getOrigin().mY;
		mOutfit.mOrigin.z = mBeamedSlide->getZPosition();
		
		float fdeltaX = mOutfit.mSize.x * mOutfit.mPivot.x;
		float fdeltaY = mOutfit.mSize.y * mOutfit.mPivot.y;

		float fPointX = mOutfit.mOrigin.x;
		float fPointY = mOutfit.mOrigin.y;
		float fPointZ = mOutfit.mOrigin.z;

		mOutfit.mTextureSpriteRegion = mBeamedSlide->getTextureRegion();
		mOutfit.mTextureBackFaceSpriteRegion = mBeamedSlide->getBackFaceTextureRegion();

		mOutfit.mLocalOpacity = mBeamedSlide->getOpacity();

		if (mSuperSlide != NULL){
			const SVISlideOutfit& parentOutfit = mSuperSlide->getOutfit();
			fPointX += parentOutfit.mOrigin.x;
			fPointY += parentOutfit.mOrigin.y;
			fPointZ += parentOutfit.mOrigin.z;
			
			//2011-07-22 masterkeaton27@gmail.com
			SVIFloat parentAlpha = parentOutfit.mGlobalOpacity;
			SVIFloat localAlpha = mOutfit.mLocalOpacity;
			mOutfit.mGlobalOpacity = parentAlpha * localAlpha;
		}else{
			mOutfit.mGlobalOpacity = mOutfit.mLocalOpacity;
		}

		//2011-07-22 masterkeaton27@gmail.com
		//hold alpha without parent's alpha efficiency
		if (mBeamedSlide->getHoldOpacity()){
			mOutfit.mGlobalOpacity = mOutfit.mLocalOpacity;
		}
				
		mOutfit.mPosition.x = fdeltaX + fPointX;
		mOutfit.mPosition.y = fdeltaY + fPointY;
		mOutfit.mPosition.z = mBeamedSlide->getZPosition() + fPointZ;
		
		setHidden(mBeamedSlide->getHidden());

		//2011-07-21 masterkeaton27@gmail.com
		//clipping flag for scissor test clipping
		setClip(mBeamedSlide->getClipSubSlide());

		buildTextureScale();
		return SVITRUE;
	}

	void SVIProjectionSlide::bake(){
		mSavedOutfit = mOutfit;
		mSavedParent = mSuperSlide;

		mSuperSlide = NULL;
		mOutfit.mPosition = mOutfit.mPivot * mOutfit.mSize * mOutfit.mScale;
		mOutfit.mRotation = SVIVector3(0.0f,0.0f,0.0f);
		mOutfit.mOrigin = SVIVector3(0.0f,0.0f,0.0f);


		invalidate(mOutfit);
	}

	void SVIProjectionSlide::rebake(){
		mOutfit = mSavedOutfit;
		mSuperSlide = mSavedParent;

		invalidate(mOutfit);
	}


	void SVIProjectionSlide::setHoldDeformDown(float x, float y){
		mOutfit.mHoldDeformTouchDown = SVITRUE;
		mOutfit.mHoldDeformTouchDownPos.x = x;
		mOutfit.mHoldDeformTouchDownPos.y = y;
		//LOGE("SVIProjectionSlide::setHoldDeformDown x:%f, y:%f", mOutfit.mHoldDeformTouchDownPos.x, mOutfit.mHoldDeformTouchDownPos.y);
	}

	SVIBool SVIProjectionSlide::buildOutfit(){
		if (mBeamedSlide == NULL) {
			LOGE("SVIProjectionSlide::buildOutfit()-mBeamedSlide == NULL");
			return SVIFALSE;
		}
		
		SVISlideOutfit* pOutfit = &mOutfit;	

		//2012-01-10 d.sedukhin
		//touch processing
		pOutfit->mTouchHappened = SVIFALSE;

		pOutfit->mRotation = SVIQuaternion(mBeamedSlide->getRotation());

		pOutfit->mPivot.x = mBeamedSlide->getPivotPoint().mX;
		pOutfit->mPivot.y = mBeamedSlide->getPivotPoint().mY;
		pOutfit->mPivot.z = 0.0f;

		pOutfit->mScale = mBeamedSlide->getScale(); // this will lose original w factor

		pOutfit->mSize.x = mBeamedSlide->getRegion().mSize.mWidth;
		pOutfit->mSize.y = mBeamedSlide->getRegion().mSize.mHeight;
		pOutfit->mSize.z = 0.0f;

		
		pOutfit->mOrigin.x = mBeamedSlide->getRegion().getOrigin().mX;
		pOutfit->mOrigin.y = mBeamedSlide->getRegion().getOrigin().mY;
		pOutfit->mOrigin.z = mBeamedSlide->getZPosition();
		
		float fdeltaX = pOutfit->mSize.x * pOutfit->mPivot.x;
		float fdeltaY = pOutfit->mSize.y * pOutfit->mPivot.y;
		float fPointX = pOutfit->mOrigin.x;
		float fPointY = pOutfit->mOrigin.y;
		float fPointZ = pOutfit->mOrigin.z;

		pOutfit->mLocalOpacity = mBeamedSlide->getOpacity();

		if (mSuperSlide != NULL){
			const SVISlideOutfit& parentOutfit = mSuperSlide->getOutfit();
			fPointX += parentOutfit.mOrigin.x;
			fPointY += parentOutfit.mOrigin.y;
			fPointZ += parentOutfit.mOrigin.z;

			//2011-07-22 masterkeaton27@gmail.com
			SVIFloat parentAlpha = parentOutfit.mGlobalOpacity;
			SVIFloat localAlpha = pOutfit->mLocalOpacity;
			pOutfit->mGlobalOpacity = parentAlpha * localAlpha;
		}else{
			pOutfit->mGlobalOpacity = pOutfit->mLocalOpacity;
		}
		
		//2011-07-22 masterkeaton27@gmail.com
		//hold alpha without parent's alpha efficiency
		if (mBeamedSlide->getHoldOpacity()){
			pOutfit->mGlobalOpacity = pOutfit->mLocalOpacity;
		}

		pOutfit->mPosition.x = fdeltaX + fPointX;
		pOutfit->mPosition.y = fdeltaY + fPointY;
		pOutfit->mPosition.z = mBeamedSlide->getZPosition() + fPointZ;

		pOutfit->mCornerRadius = 
			mBeamedSlide->getCornerRadius();					

		pOutfit->mColor.x = mBeamedSlide->getBgColor().mR;
		pOutfit->mColor.y = mBeamedSlide->getBgColor().mG;
		pOutfit->mColor.z = mBeamedSlide->getBgColor().mB;
		pOutfit->mColor.w = mBeamedSlide->getBgColor().mA;
		//pOutfit->mLocalOpacity = mBeamedSlide->getOpacity();
		//pOutfit->mGlobalOpacity = pOutfit->mLocalOpacity;
		
		pOutfit->mBorderWidth = mBeamedSlide->getBorderWidth();			
		
		pOutfit->mBorderColor.x = mBeamedSlide->getBorderColor().mR;
		pOutfit->mBorderColor.y = mBeamedSlide->getBorderColor().mG;
		pOutfit->mBorderColor.z = mBeamedSlide->getBorderColor().mB;
		//2011-10-28 masterkeaton27@gmail.com
		//remove alpha operation
		//pOutfit->mBorderColor.w = mBeamedSlide->getBorderColor().mA * pOutfit->mColor.w * pOutfit->mGlobalOpacity;		
		pOutfit->mBorderColor.w = mBeamedSlide->getBorderColor().mA * pOutfit->mGlobalOpacity;		
		pOutfit->mShadowColor.x = mBeamedSlide->getShadowColor().mR;
		pOutfit->mShadowColor.y = mBeamedSlide->getShadowColor().mG;
		pOutfit->mShadowColor.z = mBeamedSlide->getShadowColor().mB;
		pOutfit->mShadowColor.w = mBeamedSlide->getShadowColor().mA * 
			mBeamedSlide->getShadowOpacity() * pOutfit->mColor.w * pOutfit->mGlobalOpacity;
		pOutfit->mShadowRadius = mBeamedSlide->getShadowRadius();
		pOutfit->mShadowOffset.x = mBeamedSlide->getShadowOffset().mX;
		pOutfit->mShadowOffset.y = mBeamedSlide->getShadowOffset().mY;


		pOutfit->mLightOpacity = mBeamedSlide->getLightOpacity();
		pOutfit->mLightColor.x = mBeamedSlide->getLightColor().mR;
		pOutfit->mLightColor.y = mBeamedSlide->getLightColor().mG;
		pOutfit->mLightColor.z = mBeamedSlide->getLightColor().mB;
		pOutfit->mLightColor.w = mBeamedSlide->getLightColor().mA * pOutfit->mLightOpacity;

		//2012-11-27 masterkeaton27@gmail.com
        pOutfit->mLightDirection = mBeamedSlide->getLightDirection();
		pOutfit->mViewDirection = mBeamedSlide->getViewDirection();

		pOutfit->mLightAngle = mBeamedSlide->getLightAngle();		
		pOutfit->mLightRadius = mBeamedSlide->getLightRadius();
		pOutfit->mLightPower = mBeamedSlide->getLightPower();
		pOutfit->mLightType = mBeamedSlide->getLightType();
		pOutfit->mLightCenter.x = mBeamedSlide->getLightOffset().mX;
		pOutfit->mLightCenter.y = mBeamedSlide->getLightOffset().mY;
		
		//2011-08-29 masterkeaton27@gmail.com
		pOutfit->mTextureSpriteRegion = mBeamedSlide->getTextureRegion();

		//2012-01-06 masterkeaton27@gmail.com
		pOutfit->mTextureBackFaceSpriteRegion = mBeamedSlide->getBackFaceTextureRegion();

		//2011-06-23 masterkeaton27@gmail.com
		//hidden
		setHidden(mBeamedSlide->getHidden());		

		//2011-07-21 masterkeaton27@gmail.com
		//clipping flag for scissor test clipping
		setClip(mBeamedSlide->getClipSubSlide());		

		pOutfit->mZOrderType = (SVIInt)mBeamedSlide->getZOrderType();
		pOutfit->mSwipeType = (SVIInt)mBeamedSlide->getSwipeType();
		pOutfit->mDeformType = (SVIInt)mBeamedSlide->getDeformType();

        //2013-06-26 masterkeaton27@gmail.comfilter type added.
        pOutfit->mFilterType = (SVIInt)mBeamedSlide->getFilterType();

        //2012/12/08 rareboy0112@facebook.com
        pOutfit->mTesselation = mBeamedSlide->getTesselation();
        if(pOutfit->mTesselation == SVITRUE)
        {
            //copy tesselation point list
            pOutfit->mTesselationPointList.clear();
            pOutfit->mTesselationPointList.reserve(mBeamedSlide->getTesselationPointList().size());
            std::copy(mBeamedSlide->getTesselationPointList().begin(), mBeamedSlide->getTesselationPointList().end(),
                std::back_inserter(pOutfit->mTesselationPointList));
        }
        pOutfit->mMorphingType = (SVIInt)mBeamedSlide->getMorphingType();

		pOutfit->mDeformDirection = (SVIInt)mBeamedSlide->getDeformDirection();

		//2011-09-14 masterkeaton27@gmail.com
		pOutfit->mBlendType = (SVIInt)mBeamedSlide->getBlendType();

		//2011.12.09 jongchae.moon
		pOutfit->mEnableAA = mBeamedSlide->getAntiAliasing();
		//

		//2011-11-03 masterkeaton27@gmail.com
		pOutfit->mRTEffectType = (SVIInt)mBeamedSlide->getRTEffectType();

		pOutfit->mSwipeRatio = mBeamedSlide->getSwipeRatio();
		pOutfit->mDeformRatio = mBeamedSlide->getDeformRatio();
		pOutfit->mRTERatio = mBeamedSlide->getRTERatio();

        pOutfit->mUseNormalMap = mBeamedSlide->getUseNormalMap();
		pOutfit->mIsOrthogonal = mBeamedSlide->getOrthogonal();
		pOutfit->mIsCustomRTE = mBeamedSlide->getCustomRTE();

		pOutfit->mIsBackFaceImageFlip = mIsBackFaceImageFlip;
		pOutfit->mIsBackFaceImageClear = mIsBackFaceImageClear;
		buildTextureScale();
		return SVITRUE;
	}

	
	SVIBool SVIProjectionSlide::initialize(SVIProjectionSlide* parent, SVIBeamedSlide* slide, SVIImage* image, int type) {
		if( slide == NULL ) {
			DEBUG_CHECK_LOGE("SVIProjectionSlide::initialize() : slide is NULL");
			return SVIFALSE;
		}

		//2011-07-01 masterkeaton27@gmail.com- blocked for setimage issue
		//if( type == IMAGE_SLIDE && image == NULL ) 
		//	return SVIFALSE;

		mBeamedSlide = slide;
		mSlideType = (SlideType)type;

		//2011-07-01 masterkeaton27@gmail.comchanged for setimage issue
		//if( mSlideType != BASE_SLIDE && image != NULL ) {
		if( image != NULL ) {
			//2011-07-01 masterkeaton27@gmail.com
			if (mSlideType == TEXT_SLIDE){
				image->mImageType = SVIImage::IMAGE_TEXT;
				DEBUG_CHECK_LOGE("Initialize - Text Slide");
			}else{
				image->mImageType = SVIImage::IMAGE_RAW;
				DEBUG_CHECK_LOGE("Initialize - Image Slide");
			}
			setImage(image);
		}

		if( parent != NULL )
			parent->addSubSlide(this);


		//2011-05-26 masterkeaton27@gmail.com
		//invalidate myself

		//2011-06-16 masterkeaton27@gmail.com
		//first set changed for initial mesh state issues
		invalidate(SVITRUE);

        return SVITRUE;
    }

    void SVIProjectionSlide::setBackFaceTexture(SVITexture * texture){
        mTextureContainer->setBackFaceTexture(texture);
    }

    void SVIProjectionSlide::setCaptureTexture(SVICaptureData * pData){
        mTextureContainer->setCaptureTexture(pData);
    }

    void SVIProjectionSlide::setTexture(SVITexture * texture){
        mTextureContainer->setTexture(texture);
    }

    void SVIProjectionSlide::setNormalTexture(SVITexture * texture){
        mTextureContainer->setNormalTexture(texture);
    }

    void SVIProjectionSlide::setSpecularTexture(SVITexture * texture){
        mTextureContainer->setSpecularTexture(texture);
    }


	void SVIProjectionSlide::setBackFaceImage(SVIImage* image){
        SVITexture * pTexture = mTextureContainer->getBackFaceTexture();

        if( image != NULL ) {

            if( pTexture == NULL ){
                pTexture = new SVITexture(mSVIGLSurface);
                mTextureContainer->setBackFaceTexture(pTexture);
            }

            if (mSlideType == TEXT_SLIDE){
                image->mImageType = SVIImage::IMAGE_TEXT;
                image->mAlphaType = SVIImage::PREMULTIPLIED;
            }else{
                image->mImageType = SVIImage::IMAGE_RAW;
                image->mAlphaType = SVIImage::NORMAL;
            }
            pTexture->setImage(image);
            mSVIGLSurface->getTextureManager()->add(pTexture);
        }else{
            if (pTexture != NULL){
                pTexture->setImage(NULL);
				mSVIGLSurface->getTextureManager()->remove(pTexture);
				mTextureContainer->setBackFaceTexture(NULL);
			}
		}
	}


	void SVIProjectionSlide::setBackFaceImageFlip(SVIBool isFlip) {
		mIsBackFaceImageFlip = isFlip;
	}

	void SVIProjectionSlide::setBackFaceImageClear(SVIBool isClear){
		mIsBackFaceImageClear = isClear;
	}

	void SVIProjectionSlide::set3DScene(SVI3DScene * pScene) {
		SVI_SVIFE_DELETE(mAttachedScene);
		mAttachedScene = pScene;
	}

    //2013-06-21 masterkeaton27@gmail.com
    void SVIProjectionSlide::addPathPoint(const SVIVector3& point ){
        mOutfit.mPathPoints.push_back(point);
    }

    //2013-06-21 masterkeaton27@gmail.com
    void SVIProjectionSlide::clearPathPoints(){
        std::vector<SVIVector3> swapVertices;
        swapVertices.reserve(0);
        mOutfit.mPathPoints.swap(swapVertices);
        mOutfit.mPathPoints.clear();
    }


    void SVIProjectionSlide::setImage(SVIImage* image) {
		//Thread protect new image setting.
		// BONNIE .. Need to check whether this lock is needed or not.
    	//SVIMutex::SVIAutolock lock(mSVIGLSurface->getTextureManager()->mLockForBitmapPixels);

        SVITexture * pTexture = mTextureContainer->getTexture();
               

        if( image != NULL && image->mBits != NULL) {                        
            if( pTexture == NULL ){
                pTexture = new SVITexture(mSVIGLSurface);
                mTextureContainer->setTexture(pTexture);
            }

            if (mSlideType == TEXT_SLIDE){
                image->mImageType = SVIImage::IMAGE_TEXT;
                image->mAlphaType = SVIImage::PREMULTIPLIED;				
            }else{
                image->mImageType = SVIImage::IMAGE_RAW;
                image->mAlphaType = SVIImage::NORMAL;
            }
            pTexture->setImage(image);
            mSVIGLSurface->getTextureManager()->add(pTexture);
        }else{
            if (pTexture != NULL){
                pTexture->setImage(NULL);
                mSVIGLSurface->getTextureManager()->remove(pTexture);
                mTextureContainer->setTexture(NULL);
            }
        }
    }


    void SVIProjectionSlide::setNormalImage(SVIImage* image) {
        SVITexture * pTexture = mTextureContainer->getNormalTexture();

        

        if( image != NULL ) {
            if( pTexture == NULL ){
                pTexture = new SVITexture(mSVIGLSurface);
                mTextureContainer->setNormalTexture(pTexture);
            }

            if (mSlideType == TEXT_SLIDE){
                image->mImageType = SVIImage::IMAGE_TEXT;
                image->mAlphaType = SVIImage::PREMULTIPLIED;				
            }else{
                image->mImageType = SVIImage::IMAGE_RAW;
                image->mAlphaType = SVIImage::NORMAL;
            }
            pTexture->setImage(image);
            mSVIGLSurface->getTextureManager()->add(pTexture);
        }else{
            if (pTexture != NULL){
               pTexture->setImage(NULL);
                mSVIGLSurface->getTextureManager()->remove(pTexture);
                mTextureContainer->setNormalTexture(NULL);
            }
        }
    }

    void SVIProjectionSlide::setSpecularImage(SVIImage* image) {
        SVITexture * pTexture = mTextureContainer->getSpecularTexture();
               
        if( image != NULL ) {
            if( pTexture == NULL ){
                pTexture = new SVITexture(mSVIGLSurface);
                mTextureContainer->setSpecularTexture(pTexture);
            }

            if (mSlideType == TEXT_SLIDE){
                image->mImageType = SVIImage::IMAGE_TEXT;
                image->mAlphaType = SVIImage::PREMULTIPLIED;				
            }else{
                image->mImageType = SVIImage::IMAGE_RAW;
                image->mAlphaType = SVIImage::NORMAL;
            }
            pTexture->setImage(image);
            mSVIGLSurface->getTextureManager()->add(pTexture);
        }else{
            if (pTexture != NULL){
                pTexture->setImage(NULL);
                mSVIGLSurface->getTextureManager()->remove(pTexture);
                mTextureContainer->setSpecularTexture(NULL);
            }
        }
    }

	SVIBool	SVIProjectionSlide::isGroupHead(){

		//2011-07-21 masterkeaton27@gmail.com
		//need more flag identifier
		return mIsClip;
	}

	SVIBool SVIProjectionSlide::isTexturedSlide(){
		return (mSlideType == IMAGE_SLIDE) ? SVITRUE : SVIFALSE;
	}

	SVIBool SVIProjectionSlide::isColoredSlide(){
		return (mSlideType == BASE_SLIDE) ? SVITRUE : SVIFALSE;
	}

	SVIBool SVIProjectionSlide::isUseNormalMap(){
	    return mOutfit.isUseNormalMap();
	}

	//2011-07-22 masterkeaton27@gmail.com
	//build opacity
	void SVIProjectionSlide::buildColors(){
		if (mOutfit.mNeedUpdate == SVIFALSE){
			DEBUG_CHECK_LOGI("Cancel build transform");
			return;
		}

		if (mSuperSlide != NULL){
			const SVISlideOutfit& parentOutfit = mSuperSlide->getOutfit();
			//2011-07-22 masterkeaton27@gmail.com
			//alpha accumulation
			SVIFloat parentAlpha = parentOutfit.mGlobalOpacity;
			SVIFloat localAlpha = mOutfit.mLocalOpacity;
			mOutfit.mGlobalOpacity = parentAlpha * localAlpha;
		}else{
			mOutfit.mGlobalOpacity = mOutfit.mLocalOpacity;
			
		}

		//2011-07-22 masterkeaton27@gmail.com
		//hold alpha without parent's alpha efficiency
		if (mBeamedSlide->getHoldOpacity()){
			mOutfit.mGlobalOpacity = mOutfit.mLocalOpacity;
		}


		/*
		//build sub colors
		if (mSubSlideList.empty() == SVIFALSE){
			for (int n = 0; n < mSubSlideList.size(); ++n){
				SVIProjectionSlide * subSlide = mSubSlideList[n];
				subSlide->buildColors();
			}			
		}*/
	}

	void SVIProjectionSlide::buildTransform(){
		//CHECK_PERFORMANCE("depth : %d, subCount:%d", mDepthLevel, mSubSlideList.size());
		if (mOutfit.mNeedUpdate == SVIFALSE){
			//DEBUG_CHECK_LOGI("Cancel build transform");
			LOGE("Cancel build transform");
			return;
		}

		//2011-07-07 masterkeaton27@gmail.com
		//renew geometry informations. to build transform trees.
		buildGeometryOutfit();		

		//build my transform
		mOutfit.calculrate();
		
		if (mSuperSlide == NULL){ // top most transform			
			mOutfit.mGlobalTransform = mOutfit.mLocalTransform;	
		}else{ // child case
			const SVISlideOutfit & parentOutfit = mSuperSlide->getOutfit();
			// holdScale issue fix
			mOutfit.mLocalTransform._41 -= 	(parentOutfit.mOrigin.x + parentOutfit.mPivot.x * parentOutfit.mSize.x);
			mOutfit.mLocalTransform._42 -= 	(parentOutfit.mOrigin.y + parentOutfit.mPivot.y * parentOutfit.mSize.y);
			mOutfit.mLocalTransform._43 -= parentOutfit.mOrigin.z;

			mOutfit.mGlobalTransform = mOutfit.mLocalTransform * parentOutfit.mGlobalTransform;	
			
			if (mOutfit.mIsHoldScale){
				mOutfit.mGlobalTransform._11 = mOutfit.mScale.x;
				mOutfit.mGlobalTransform._22 = mOutfit.mScale.y;
				mOutfit.mGlobalTransform._33 = mOutfit.mScale.z;
			}
		}
	}

	void SVIProjectionSlide::addSubSlide(SVIProjectionSlide* pChild){		
		if( pChild == NULL ) {
			LOGE("SVIProjectionSlide::addSubSlide : child is NULL");
			return;
		}
		
		pChild->setParent(this);
		pChild->setDepthLevel(mDepthLevel+1);
		//2011-06-22 masterkeaton27@gmail.comside level bug fixed.
		pChild->setSideLevel(mSubTotalSlideCount++);
		
		//2011-11-18 masterkeaton27@gmail.com
		//texture process changed.
		//2011-08-07 masterkeaton27@gmail.com
		//follow code was main reason of texture dangling pointer problem.
		//fixed with pointer duplication check in add function.
		if( pChild->getTextureContainer()->getTexture() != NULL ) {
			mSVIGLSurface->getTextureManager()->add(pChild->getTextureContainer()->getTexture());
		}
		if( pChild->getTextureContainer()->getBackFaceTexture() != NULL ) {
			mSVIGLSurface->getTextureManager()->add(pChild->getTextureContainer()->getBackFaceTexture());
		}

		mSubSlideList.push_back(pChild);

		invalidate(SVITRUE);
		pChild->invalidate(SVITRUE);
	}

	SVIBool SVIProjectionSlide::removeSubSlide(SVIProjectionSlide* pChild){
		std::vector<SVIProjectionSlide*>::iterator iter = mSubSlideList.begin();

		while (iter != mSubSlideList.end()){
			SVIProjectionSlide* slide = *iter;
			if (slide == pChild){
				//check this deletion for safety check
				(*iter)->finalize();
				(*iter)->setParent(NULL);
				mSubSlideList.erase(iter);

				//2011.08.08 marx.kim : add code to synchronize between projection and renderPatchList.
				invalidate(SVITRUE);
				return SVITRUE;
			}
			iter++;
		}		

		return SVIFALSE;
	}

	void SVIProjectionSlide::removeSubSlide(SVIUInt id){
		std::vector<SVIProjectionSlide*>::iterator iter = mSubSlideList.begin();
		while (iter != mSubSlideList.end()){
			SVIProjectionSlide* slide = *iter;
			if (slide->getId() == id){
				//check this deletion for safety check
				(*iter)->finalize();
				mSubSlideList.erase(iter);
				

				//2011.08.08 marx.kim : add code to synchronize between projection and renderPatchList.
				invalidate(SVITRUE);
				return;
			}
			iter++;
		}		
	}

	//2011-07-08 masterkeaton27@gmail.com
	//transparent checking
	SVIBool SVIProjectionSlide::isTransparent(){
		if (mBeamedSlide == NULL) return SVITRUE;

		//2011-09-14 masterkeaton27@gmail.com
		//add blend check
		if (mBeamedSlide->getBlendType() == BLEND_MULTIPLY && 
			(mBeamedSlide->getBgColor().mA < 0.01f || 
			mBeamedSlide->getOpacity() < 0.01f)) return SVITRUE;

		if (mBeamedSlide->getBlendType() == BLEND_ADD && 
			mBeamedSlide->getOpacity() < 0.01f) return SVITRUE;
		return SVIFALSE;
	}

	SVIBool	SVIProjectionSlide::hasClipParent(){
		if (mSuperSlide == NULL) return SVIFALSE;
		SVIProjectionSlide * pParent = (SVIProjectionSlide *)mSuperSlide->getParent();
		if (pParent == NULL) return SVIFALSE;
		while (pParent != NULL){
			if (pParent->isClip()) return SVITRUE;
			pParent = (SVIProjectionSlide *)pParent->getParent();
		}
		return SVIFALSE;
	}


	SVIUInt	SVIProjectionSlide::hasZOrderedParent(){
		SVIProjectionSlide * pParent = (SVIProjectionSlide *)mSuperSlide;
		if (pParent == NULL) return SVIFALSE;
		while (pParent != NULL){
			if (pParent->isTopMost()) return TOPMOST;
			else if (pParent->isBottom()) return BOTTOM;
			pParent = (SVIProjectionSlide *)pParent->getParent();
		}
		return NORMAL;
	}

	SVIBool	SVIProjectionSlide::hasZOrderedParent(SVIUInt zOrderType){
		SVIBool isTopMostCheck = zOrderType == TOPMOST ? SVITRUE : SVIFALSE;
		SVIBool isBottomCheck = zOrderType == BOTTOM ? SVITRUE : SVIFALSE;

		SVIProjectionSlide * pParent = (SVIProjectionSlide *)mSuperSlide;
		if (pParent == NULL) return SVIFALSE;
		while (pParent != NULL){
			
			if (isTopMostCheck && pParent->isTopMost()) return SVITRUE;
			else if (isBottomCheck && pParent->isBottom()) return SVITRUE;
			
			pParent = (SVIProjectionSlide *)pParent->getParent();
		}
		return SVIFALSE;
	}

	SVIBool	SVIProjectionSlide::isTopMost(){
		return (mOutfit.mZOrderType == TOPMOST) ? SVITRUE : SVIFALSE;
	}

	SVIBool SVIProjectionSlide::isBottom(){
		return (mOutfit.mZOrderType == BOTTOM) ? SVITRUE : SVIFALSE;
	}

	SVIBool SVIProjectionSlide::isRenderTarget(){
		return (mTextureContainer != NULL && mTextureContainer->hasCaptureData()) ? SVITRUE : SVIFALSE;
		//return mIsRenderTarget;
	}

	void SVIProjectionSlide::removeFromSuperSlide() {
		if( mSuperSlide != NULL ) 
			mSuperSlide->removeSubSlide(this);
	}

	SVIBool SVIProjectionSlide::insertSubSlide(SVIInt index, SVIProjectionSlide* newSlide) {
		if( newSlide == NULL )
			return SVIFALSE;

		SVIBool ret = SVIFALSE;
		SVIInt count = mSubSlideList.size();

		if( index < 0 || index > count ) {
			LOGE("insertSubSlide is failed : because index is out of range.!!!");
			return SVIFALSE;
		}

		if( index == count ) {
			mSubSlideList.push_back(newSlide);
			ret = SVITRUE;
		} else {
			//2011.08.08 marx.kim : modify code to fix bug.
			if( count == 0 ) {
				mSubSlideList.push_back(newSlide);
				ret = SVITRUE;
			} else {
				ProjectionSlideListIter iter = findSlideIter(index);

				if( iter != mSubSlideList.end() ) {
					mSubSlideList.insert(iter, newSlide);
					ret = SVITRUE;
				} 
			}
		}
		
		if( ret ){

			newSlide->setParent(this);
			newSlide->setDepthLevel(mDepthLevel+1);
			newSlide->setSideLevel(index);

			//2011-11-18 masterkeaton27@gmail.com
			//texture process changed.
			//2011-08-07 masterkeaton27@gmail.com
			//follow code was main reason of texture dangling pointer problem.
			//fixed with pointer duplication check in add function.
			if( newSlide->getTextureContainer()->getTexture() != NULL ) {
				mSVIGLSurface->getTextureManager()->add(newSlide->getTextureContainer()->getTexture());
			}
			if( newSlide->getTextureContainer()->getBackFaceTexture() != NULL ) {
				mSVIGLSurface->getTextureManager()->add(newSlide->getTextureContainer()->getBackFaceTexture());
			}

			invalidate(SVITRUE);
			newSlide->invalidate(SVITRUE);
		} else {
			LOGE("SVIProjectionSlide::insertSubSlide failed index:%d newSlide:%x", index, (SVIUInt)newSlide);
		}

		return ret;
	}

	ProjectionSlideListIter SVIProjectionSlide::findSlideIter(SVIInt index) {
		ProjectionSlideListIter iter = mSubSlideList.end();
		if( index < 0 || (SVIUInt)index > mSubSlideList.size() )
			return iter;

		SVIInt counter = 0;

		for(iter = mSubSlideList.begin(); iter != mSubSlideList.end(); iter++) {
			if( counter == index )
				break;
			counter++;
		}

		return iter;
	}

	SVIBool SVIProjectionSlide::replaceSubSlide(SVIProjectionSlide* oldSlide, SVIProjectionSlide* newSlide) {
		if( oldSlide == NULL || newSlide == NULL ) {
			DEBUG_CHECK_LOGE("SVIProjectionSlide : oldSlide or newSlide is NULL!!!");
			return SVIFALSE;
		}

		if( oldSlide->getParent() == NULL || oldSlide->getParent() != this  ||
			newSlide->getParent() != NULL) {
				DEBUG_CHECK_LOGE("SVIProjectionSlide : oldSlide or newSlide has some problem to replace!!!");
				return SVIFALSE;
		}

		SVIInt index = getSlideIndex(oldSlide);
		if( index < 0 ) {
			DEBUG_CHECK_LOGE("SVIProjectionSlide : can not find out oldSlide in subSlides!!!");
			return SVIFALSE;
		}

		if( !insertSubSlide(index, newSlide) ) {
			DEBUG_CHECK_LOGE("SVIProjectionSlide : failed to insert new subSlide!!!");
			return SVIFALSE;
		}

		if( !removeSubSlide(oldSlide) ) {
			DEBUG_CHECK_LOGE("SVIProjectionSlide : failed to remove new subSlide!!!");
			return SVIFALSE;
		}

		return SVITRUE;
	}

	SVIInt SVIProjectionSlide::getSlideIndex(SVIProjectionSlide* subSlide) {
		int index = -1;
		ProjectionSlideListIter iter = mSubSlideList.begin();

		SVIProjectionSlide* slide = NULL;

		int indexCounter = 0;
		for(iter; iter != mSubSlideList.end(); iter++ ) {
			slide = (SVIProjectionSlide*)(*iter);
			if( subSlide == slide ) {
				index = indexCounter;
				break;
			}
			indexCounter++;
		}

		return index;
	}

	SVIBool SVIProjectionSlide::setOutfit(SVISlideOutfit& outfit){
		//bind outfit properties if something changed.
		if (mOutfit.bindChangeProperties(outfit)){
			return SVITRUE;
		}
		return SVIFALSE;
	}

	void SVIProjectionSlide::invalidateLocalOrigin(){
						
		float fdeltaX = mOutfit.mSize.x * mOutfit.mPivot.x;// * outfit.mScale.x;
		float fdeltaY = mOutfit.mSize.y * mOutfit.mPivot.y;// * outfit.mScale.y;
		float fdeltaZ = mOutfit.mPosition.z;

		float fPointX = mOutfit.mOrigin.x;
		float fPointY = mOutfit.mOrigin.y;
		float fPointZ = mOutfit.mOrigin.z;

		if (mSuperSlide != NULL){
			const SVISlideOutfit& parentOutfit = mSuperSlide->getOutfit();
			fPointX += parentOutfit.mOrigin.x;
			fPointY += parentOutfit.mOrigin.y;
			fPointZ += parentOutfit.mOrigin.z;
		}		

		mOutfit.mPosition = SVIVector3(fdeltaX + fPointX,fdeltaY + fPointY,fdeltaZ);
		
		//build sub transforms
		if (mSubSlideList.empty() == SVIFALSE){
			for (SVIUInt n = 0; n < mSubSlideList.size(); ++n){
				SVIProjectionSlide * subSlide = mSubSlideList[n];				
				subSlide->invalidateLocalOrigin();
			}			
		}
	}

	void SVIProjectionSlide::invalidate(SVISlideOutfit & newOutfit){
		//mOutfit.setNeedUpdate();
		//setOutfit(newOutfit);

		//build my transform
		mOutfit.calculrate();
		mOutfit.mGlobalTransform = mOutfit.mLocalTransform;	
		//buildTransform();	
		//2011-08-11 masterkeaton27@gmail.com
		//change traversal 
		if (mSubSlideList.empty() == SVIFALSE){
			for (SVIUInt n = 0; n < mSubSlideList.size(); ++n){
				SVIProjectionSlide * subSlide = mSubSlideList[n];
				subSlide->invalidate(SVITRUE);
			}
		}
	}

	void SVIProjectionSlide::invalidate(SVIBool bFirstSet){
		SVIBool bTransformChanged = SVIFALSE;
		SVIBool bColorChanged = SVIFALSE;
		SVIBool bIsRenderRequested = SVIFALSE;
		if( isNeedResetAll() )
			bFirstSet = SVITRUE;

		//2011-06-16 masterkeaton27@gmail.com
		//invalidate previous property which updated one
		if (!bFirstSet){
			//2011-08-09 masterkeaton27@gmail.com
			//reduce for loop count
			//bTransformChanged = updateTransformOutfit();
			//bColorChanged = updateColorOutfit();
			updateOutfit(&bColorChanged,&bTransformChanged);

			mOutfit.setNeedUpdate();
			buildTransform();
			buildColors();
			
			if (bTransformChanged || bColorChanged) bIsRenderRequested = SVITRUE;
		}else{
			if (buildOutfit()){
				//update all properties for initialize object
				mOutfit.setNeedUpdate();
				buildTransform();
				bIsRenderRequested = SVITRUE;
			}
		}
		

		//LOGE("%d: <%d> Opacity %.02f",mDepthLevel,(int)this,mOutfit.mLocalOpacity);

		//2011-08-11 masterkeaton27@gmail.com
		//change traversal 
		if (mSubSlideList.empty() == SVIFALSE){
			for (SVIUInt n = 0; n < mSubSlideList.size(); ++n){
				SVIProjectionSlide * subSlide = mSubSlideList[n];
				subSlide->setDepthLevel(mDepthLevel+1);
				subSlide->invalidate(bFirstSet);
			}			
		}

		if (bIsRenderRequested) 
			mSVIGLSurface->getRenderer()->requestRender();


		mIsInvalidated = SVITRUE;
	}

	void SVIProjectionSlide::initialize(){
		invalidate();
	}

	void SVIProjectionSlide::finalize(){
		//2011-10-28 masterkeaton27@gmail.com		
    	(mSVIGLSurface)->getSlideCapture()->stop(this);
		
		//2011-06-27 masterkeaton27@gmail.com
		//this may cause thread critical problem.
		if (mLinkedRenderPatch != NULL) {
			mLinkedRenderPatch->setOwner(NULL);
			//2012-05-11 masterkeaton27@gmail.com
			//delete undeleted renderpatch
			SVI_SVIFE_DELETE(mLinkedRenderPatch);
			//LOGE("mLinkedRenderPatch deleted");
		}
		
		mLinkedRenderPatch = NULL;
	}



	SVIProjectionSlide::~SVIProjectionSlide(){

        //2013-06-21 masterkeaton27@gmail.com
        clearPathPoints();

        if( getParent() != NULL )
            removeFromSuperSlide();
        SVIProjectionSlide* slide = NULL;
        if( mSubSlideList.size() > 0 ) {
            for(SVIUInt i=0; i<mSubSlideList.size(); i++) {
                slide = mSubSlideList[i];
                slide->setParent(NULL);
            }

            mSubSlideList.clear();
        }

        SVI_SVIFE_DELETE(mTextureContainer);
		
		// 2012/06/22 rareboy0112@facebook.com
		// delete particle effect list
		if(mParticleEffectList != NULL) {
			ParticleEffectList::iterator iter = mParticleEffectList->begin();
			while( !(mParticleEffectList->empty()) ) {
				SVIParticleEffect* effect = *iter;
				iter = mParticleEffectList->erase(iter);
				SVI_SVIFE_DELETE(effect);
			}
			mParticleEffectList->clear();
			SVI_SVIFE_DELETE(mParticleEffectList);
		}

		finalize();
		mSVIGLSurface = NULL;
	}		


	//2011-07-06 marx.kim
	// this function is called from invalidate function to check whether ChangeList contains RESET_ALL or not.
	SVIBool SVIProjectionSlide::isNeedResetAll() {
		SVIBool ret = SVIFALSE;
		ChangePropertyList & changedList = mBeamedSlide->getChangePropertyList();
		for(SVIUInt i=0; i<changedList.size(); ++i) {
			if( changedList[i] == RESET_ALL) {
				ret = SVITRUE;
				break;
			}
				
		}
		return ret;
	}


	void SVIProjectionSlide::addParticleEffect(SVIParticleEffect* particleEffect) {
		if( mParticleEffectList == NULL ) {
			mParticleEffectList = new ParticleEffectList();
		}

		ParticleEffectList::iterator iter = mParticleEffectList->begin();

		SVIBool isEffect = SVIFALSE;
		SVIParticleEffect* temp = NULL;
		for(iter; iter != mParticleEffectList->end(); ++iter) {
			temp = (SVIParticleEffect*)(*iter);
			if( particleEffect == temp ) {
				isEffect = SVITRUE;
			}
		}

		if(isEffect == SVIFALSE) {
			mParticleEffectList->push_back(particleEffect);
		}
	}


	SVIBool SVIProjectionSlide::removeParticleEffect(SVIParticleEffect* particleEffect) {
		if( mParticleEffectList != NULL ) {
			ParticleEffectList::iterator iter = mParticleEffectList->begin();

			SVIParticleEffect* temp = NULL;
			for(iter; iter != mParticleEffectList->end(); ++iter) {
				temp = (SVIParticleEffect*)(*iter);
				if( particleEffect == temp ) {
					iter = mParticleEffectList->erase(iter);
					SVI_SVIFE_DELETE(temp);

					return SVITRUE;
				}
			}
		}

		return SVIFALSE;
	}


	// 2012/06/13 rareboy0112@facebook.com
	void SVIProjectionSlide::updateParticleEffect() {
		if( mParticleEffectList != NULL && mParticleEffectList->size() > 0 ) {
			SVIUInt drawTime = mSVIGLSurface->getParticleEffectManager()->getDrawTime();

			ParticleEffectList::iterator iter = mParticleEffectList->begin();
			while( iter != mParticleEffectList->end() ) {
				

				if( (*iter)->getStatus() == SVIParticleEffect::END ) {
					LOGI("SVIProjectionSlide: updateParticleEffect - END");
					SVIParticleEffect* effect = *iter;
					iter = mParticleEffectList->erase(iter);
					SVI_SVIFE_DELETE(effect);
					
				}else{
					(*iter)->update(drawTime);
					++iter;
					mSVIGLSurface->getRenderPartManager()->requestRender();
				}			
			}
		}
	}
}
