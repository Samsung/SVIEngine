#include "SVISlide.h"
#include "SVIBeamedSlide.h"
#include "SVIProjectionSlide.h"
#include "../Animation/SVIAnimation.h" 
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVITransitionAnimation.h"
#include "../Render/SVIFrameRenderer.h"

#include "../NativeInterface/nativeSlide.h"

#include "../Render/SVITexture.h"
#include "../Render/SVITextureManager.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

	static const SVIBool	DEBUG = SVIFALSE;

	SVISlide::SVISlide(SVIGLSurface* surface) :
		SVIBaseSlide(surface){
		mBeamedSlide = NULL;
		mSpecularImage = mNormalImage = mBackFaceImage = mImage = NULL;
		mIsBackFaceImageFlip = SVITRUE;
		mIsBackFaceImageClear = SVIFALSE;
		mSlideType = BASE_SLIDE;
		mImplicitProxy = NULL;
		mImplicitListenerID = 0;
		mExplicitAnimation = NULL;
		mImplicitAnimation = NULL;
		mIsExplicitSet = SVIFALSE;
		for (SVIInt i=0; i<SVIPropertyAnimation::PROPERTY_ANI_COUNT; i++) {
			for (SVIInt j=0; j<SET_COUNT;j++) {
				mImplicitAnimationInfo[i][j] = 0;
			}	
		}

		mLock = new SVIMutex("SlideLock");
		mInitPropertySlide = new SVISlideForInitProperty(mSVIGLSurface);
	}

	SVISlide::SVISlide(SVIGLSurface* surface, SVIInt type) :
		SVIBaseSlide(surface) {
		mBeamedSlide = NULL;
		mSpecularImage = mNormalImage = mBackFaceImage = mImage = NULL;
		mIsBackFaceImageFlip = SVITRUE;
		mIsBackFaceImageClear= SVIFALSE;
		mSlideType = type;
		mImplicitProxy = NULL;
		mImplicitListenerID = 0;
		mExplicitAnimation = NULL;
		mImplicitAnimation = NULL;
		mIsExplicitSet = SVIFALSE;
		for (SVIInt i=0; i<SVIPropertyAnimation::PROPERTY_ANI_COUNT; i++) {
			for (SVIInt j=0; j<SET_COUNT;j++) {
				mImplicitAnimationInfo[i][j] = 0;
			}	
		}

		mLock = new SVIMutex("SlideLock");

		mInitPropertySlide = new SVISlideForInitProperty(surface);
		mRotationType =0;
	}

    SVISlide::SVISlide(const SVISlide& rhs):SVIBaseSlide(mSVIGLSurface) {}
    void SVISlide::operator=(const SVISlide& rhs){}
    SVISlide::~SVISlide() {		
          mSVIGLSurface->getRenderPartManager()->removeAllRelatedToSlide(this);//Multi-Instance-Support

        mExplicitAnimation = NULL;

        if( getParent() != NULL )
            removeFromSuperSlide();

        if( mSubSlideList.size() > 0 ) {

            BaseSlideList::iterator iter = mSubSlideList.begin();
            SVISlide* slide = NULL;
            for(iter; iter != mSubSlideList.end(); iter++) {
                slide = (SVISlide*)(*iter);
                slide->setParent(NULL);
            }

			mSubSlideList.clear();
		}

		SVI_SVIFE_DELETE(mLock);
		SVI_SVIFE_DELETE(mBeamedSlide);
		SVI_SVIFE_DELETE(mInitPropertySlide);
	}
	
	SVIBool SVISlide::initialize(SVISlide* parent, const SVIRect& region, const SVIColor& bgColor, SVIImage* image/*, SVIBool text*/) {
		//Thread protect with slide tree modification.
		SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

		SVIBool ret = SVIFALSE;
		if( mBeamedSlide == NULL ) {
			mRegion = region;
			mBgColor = bgColor;
			updateGeometryByRegion();

			SVIBeamedSlide* beamedSlideParent = NULL;
			if( parent != NULL ) {
				parent->addSubSlide(this);
				beamedSlideParent = parent->getBeamedSlide();
			}

			mBeamedSlide = new SVIBeamedSlide(mSVIGLSurface);
			if( mBeamedSlide != NULL ) {
				ret = mBeamedSlide->initialize(beamedSlideParent, 
					this,
					image, 
					(SVIBeamedSlide::SlideType)mSlideType);
			}
		}
	
		return ret;
	}

	void SVISlide::displaySlideTrees() {
		LOGI("SlideTrees::ParentSlide=%d", (SVIUInt)this);
		this->printSubSlides(0);
	}
	
	void SVISlide::printSubSlides(SVIInt depth) {
		SVIInt i;
		char tmp[400] = {'\0'};
		char str[500] = {'\0'};
		
		if( mSubSlideList.size() > 0 ) {
			BaseSlideListIter iter = mSubSlideList.begin();
			SVISlide* slide = NULL;
			for(iter; iter != mSubSlideList.end(); iter++) {
				slide = (SVISlide*)(*iter);

				memset(tmp, '\0', 100);
				for(i=0; i<depth+1; i++) {
					strcat(tmp, "\t");
				}
				sprintf(str, "SlideTrees::%s %d, depth=%d, slideType=%d", tmp, (SVIUInt)slide, depth+1, slide->getSlideType());
				LOGI("%s", str);
				slide->printSubSlides(depth+1);
			}
		}
	}

	/**
	* Restores currently used textures to the present gl context.
	*/
	void SVISlide::restoreImages() {
		//Add texture again for upload.
		SVIBeamedSlide* beamedSlide = getBeamedSlide();
		if(beamedSlide != NULL){	
			SVIProjectionSlide* projectionSlide = beamedSlide->getProjectionSlide();
			if(projectionSlide != NULL){
				SVISlideTextureContainer* textureContainer = projectionSlide->getTextureContainer();

				if(textureContainer != NULL){
					SVITexture* texture = textureContainer->getTexture();
					if(texture != NULL){
						mSVIGLSurface->getTextureManager()->add(texture);
					}					
					
					//2013-05-29 masterkeaton27@gmail.com
					//not only texture but also following textures are need to re-upload 
                    SVITexture* backtexture = textureContainer->getBackFaceTexture();
                    if(backtexture != NULL){
                        mSVIGLSurface->getTextureManager()->add(backtexture);
                    }

                    SVITexture* normaltexture = textureContainer->getNormalTexture();
                    if(normaltexture != NULL){
                        mSVIGLSurface->getTextureManager()->add(normaltexture);
                    }

                    SVITexture* speculartexture = textureContainer->getSpecularTexture();
                    if(speculartexture != NULL){
                        mSVIGLSurface->getTextureManager()->add(speculartexture);
                    }
				}
			}
		}

		//Do for all descendants
		if( mSubSlideList.size() > 0 ) {
			BaseSlideListIter iter = mSubSlideList.begin();
			SVISlide* slide = NULL;
			for(iter; iter != mSubSlideList.end(); iter++) {
				slide = (SVISlide*)(*iter);
				slide->restoreImages();
			}
		}
	}
	
	void SVISlide::addChangePropery(PropertyChangeType type) {
		DEBUG_CHECK_LOGE("SVISlide::addChangePropery");
	}

	SVIBeamedSlide* SVISlide::getBeamedSlide() {
		return mBeamedSlide;
	}

	SVIInt SVISlide::getSlideType() {
		if( mBeamedSlide == NULL )
			return -1;
		return mBeamedSlide->getSlideType();
	}

	//2011-10-20 insu.song
	//remove direct property set code

	void SVISlide::initRegion(const SVIRect& region) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mRegion = region;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::REGION, SVIPropertyAnimation::REGION));
	}
	void SVISlide::initPosition(const SVIPoint& position) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mPosition = position;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::POSITION, SVIPropertyAnimation::POSITION));
	}
	void SVISlide::initPivotPoint(const SVIPoint& pivotPoint) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mPivotPoint = pivotPoint;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::PIVOT_POINT, SVIPropertyAnimation::PIVOT_POINT));
	}
	void SVISlide::initZPosition(const SVIFloat& zPosition) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mZPosition = zPosition;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::ZPOSITION, SVIPropertyAnimation::ZPOSITION));
	}
	void SVISlide::initRotation(const SVIVector4& rotation) {
		SVIFloat x, y, z;
		x = SVI_DEGTORAD(rotation.x);
		y = SVI_DEGTORAD(rotation.y);
		z = SVI_DEGTORAD(rotation.z);
		SVI::SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVI::SVIVector3(x, y, z));

		SVIAutoLock(mLock);
		mInitPropertySlide->mRotation = SVI::SVIVector4(q.x, q.y, q.z, q.w);
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::ROTATION, SVIPropertyAnimation::ROTATION));
	}

	void SVISlide::initDeformRatio(const SVIVector3& ratio) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mDeformRatio = ratio;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::DEFORM_RATIO, SVIPropertyAnimation::DEFORM_RATIO));
	}

	void SVISlide::initSwipeRatio(const SVIVector3& ratio) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mSwipeRatio = ratio;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::SWIPE_RATIO, SVIPropertyAnimation::SWIPE_RATIO));
	}

	void SVISlide::initRTERatio(const SVIVector3& ratio) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mRTERatio = ratio;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::RTE_RATIO, SVIPropertyAnimation::RTE_RATIO));
	}

	void SVISlide::initModelPosition(const SVIVector3& position) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mModelPosition = position;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::MODEL_POSITION, SVIPropertyAnimation::MODEL_POSITION));
	}
	
	void SVISlide::initModelRotation(const SVIVector4& rotation) {
		SVIFloat x, y, z;
		x = SVI_DEGTORAD(rotation.x);
		y = SVI_DEGTORAD(rotation.y);
		z = SVI_DEGTORAD(rotation.z);
		SVI::SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVI::SVIVector3(x, y, z));

		SVIAutoLock(mLock);
		mInitPropertySlide->mModelRotation = SVI::SVIVector4(q.x, q.y, q.z, q.w);
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::MODEL_ROTATION, SVIPropertyAnimation::MODEL_ROTATION));
	}

	void SVISlide::initModelScale(const SVIVector3& scale) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mModelScale = scale;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::MODEL_SCALE, SVIPropertyAnimation::MODEL_SCALE));
	}

	void SVISlide::initScale(const SVIVector3& scale) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mScale = scale;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::SCALE, SVIPropertyAnimation::SCALE));
	}
	void SVISlide::initBgColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mBgColor = color;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::BG_COLOR, SVIPropertyAnimation::BG_COLOR));
	}
	void SVISlide::initOpacity(const SVIFloat& opacity) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mOpacity= opacity;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::OPACITY, SVIPropertyAnimation::OPACITY));
	}
	void SVISlide::initCornerRadius(const SVIFloat& radius) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mCornerRadius = radius;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::CORNER_RADIUS, SVIPropertyAnimation::CORNER_RADIUS));
	}
	void SVISlide::initBorderWidth(const SVIFloat& width) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mBorderWidth = width;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::BORDER_WIDTH, SVIPropertyAnimation::BORDER_WIDTH));
	}
	void SVISlide::initBorderColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mBorderColor = color;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::BORDER_COLOR, SVIPropertyAnimation::BORDER_COLOR));
	}
	void SVISlide::initShadowColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mShadowColor = color;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::SHADOW_COLOR, SVIPropertyAnimation::SHADOW_COLOR));
	}
	void SVISlide::initShadowRadius(const SVIFloat& radius) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mShadowRadius = radius;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::SHADOW_RADIUS, SVIPropertyAnimation::SHADOW_RADIUS));
	}
	void SVISlide::initShadowOffset(const SVIPoint& offset) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mShadowOffset = offset;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::SHADOW_OFFSET, SVIPropertyAnimation::SHADOW_OFFSET));
	}
	void SVISlide::initShadowOpacity(const SVIFloat& opacity) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mShadowOpacity = opacity;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::SHADOW_OPACITY, SVIPropertyAnimation::SHADOW_OPACITY));
	}
	void SVISlide::initLightOpacity(const SVIFloat& opacity) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mLightOpacity = opacity;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::LIGHT_OPACITY, SVIPropertyAnimation::LIGHT_OPACITY));
	}
	void SVISlide::initLightColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mLightColor = color;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::LIGHT_COLOR, SVIPropertyAnimation::LIGHT_COLOR));
	}

    void SVISlide::initLightDirection(const SVIVector4& direction) {
        SVIAutoLock(mLock);
        mInitPropertySlide->mLightDirection = direction;
        mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::LIGHT_DIRECTION, SVIPropertyAnimation::LIGHT_DIRECTION));
    }

	void SVISlide::initViewDirection(const SVIVector4& direction) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mViewDirection = direction;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::VIEW_DIRECTION, SVIPropertyAnimation::VIEW_DIRECTION));
	}

	void SVISlide::initLightOffset(const SVIPoint& offset) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mLightOffset = offset;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::LIGHT_OFFSET, SVIPropertyAnimation::LIGHT_OFFSET));
	}
	void SVISlide::initLightRadius(const SVIFloat& radius) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mLightRadius = radius;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::LIGHT_RADIUS, SVIPropertyAnimation::LIGHT_RADIUS));
	}
	void SVISlide::initLightAngle(const SVIFloat& angle) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mLightAngle = angle;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::LIGHT_ANGLE, SVIPropertyAnimation::LIGHT_ANGLE));
	}
	void SVISlide::initLightPower(const SVIFloat& power) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mLightPower = power;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::LIGHT_POWER, SVIPropertyAnimation::LIGHT_POWER));
	}
	void SVISlide::initTextureRegion(const SVIRect& region) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mTextureSpriteRegion = region;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::TEXTURE_REGION, SVIPropertyAnimation::TEXTURE_REGION));
	}

	void SVISlide::initBackFaceTextureRegion(const SVIRect& region) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mTextureBackFaceSpriteRegion = region;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::BACKFACE_TEXTURE_REGION, SVIPropertyAnimation::BACKFACE_TEXTURE_REGION));
	}

	void SVISlide::initTextureScaleFitRegion(const SVIRect& region) {
		SVIAutoLock(mLock);
		mInitPropertySlide->mTextureScaleFitRegion= region;
		mInitPropertyMap.insert(std::pair<SVIInt,SVIInt>(SVIPropertyAnimation::TEXTURE_SCALEFIT_REGION, SVIPropertyAnimation::TEXTURE_SCALEFIT_REGION));
	}

	void SVISlide::setRegion(const SVIRect& region) {
		SVIAutoLock(mLock);
		SVIBaseSlide::setRegion(region);
	}

	void SVISlide::setPosition(const SVIPoint& position) {
		SVIAutoLock(mLock);
		SVIBaseSlide::setPosition(position);
	}

	void SVISlide::setDeformRatio(const SVIVector3& ratio) {
		SVIAutoLock(mLock);
		SVIBaseSlide::setDeformRatio(ratio);
	}

	void SVISlide::setSwipeRatio(const SVIVector3& ratio) {
		SVIAutoLock(mLock);
		SVIBaseSlide::setSwipeRatio(ratio);
	}

	void SVISlide::setRTERatio(const SVIVector3& ratio) {
		SVIAutoLock(mLock);
		SVIBaseSlide::setRTERatio(ratio);
	}

	void SVISlide::setPivotPoint(const SVIPoint& anchorPoint) {
		SVIAutoLock(mLock);
		SVIBaseSlide::setPivotPoint(anchorPoint);
	}

	void SVISlide::setZPosition(const SVIFloat& zPosition) {
		SVIAutoLock(mLock);
		mZPosition = zPosition;
	}

	void SVISlide::setRotation(const SVIVector4& rotation) {
		SVIFloat x, y, z;

		x = SVI_DEGTORAD(rotation.x);
		y = SVI_DEGTORAD(rotation.y);
		z = SVI_DEGTORAD(rotation.z);
		SVI::SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVI::SVIVector3(x, y, z));

		SVIAutoLock(mLock);
		mRotation = SVI::SVIVector4(q.x, q.y, q.z, q.w);
	}

	void SVISlide::setModelRotation(const SVIVector4& rotation) {
		SVIFloat x, y, z;

		x = SVI_DEGTORAD(rotation.x);
		y = SVI_DEGTORAD(rotation.y);
		z = SVI_DEGTORAD(rotation.z);
		SVI::SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVI::SVIVector3(x, y, z));

		SVIAutoLock(mLock);
		mModelRotation = SVI::SVIVector4(q.x, q.y, q.z, q.w);
	}

	void SVISlide::setScale(const SVIVector3& scale) {
		SVIAutoLock(mLock);
		mScale = scale;
	}

	void SVISlide::setBgColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mBgColor = color;
	}

	void SVISlide::setClipSubSlide(SVIBool clipSubSlide) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_CLIP_SUBSLIDE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mClipSubSlide = clipSubSlide;
	}

	void SVISlide::setOpacity(SVIFloat opacity) {
		SVIAutoLock(mLock);
		mOpacity = opacity;
	}

	void SVISlide::setCornerRadius(SVIFloat radius) {
		SVIAutoLock(mLock);
		mCornerRadius = radius;
	}

	void SVISlide::setBorderWidth(SVIFloat width) {
		SVIAutoLock(mLock);
		mBorderWidth = width;
	}

	void SVISlide::setBorderColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mBorderColor = color;
	}

	void SVISlide::setShadowColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mShadowColor = color;
	}

	void SVISlide::setShadowRadius(SVIFloat radius) {
		SVIAutoLock(mLock);
		mShadowRadius = radius;
	}

	void SVISlide::setShadowOffset(const SVIPoint& offset) {
		SVIAutoLock(mLock);
		mShadowOffset = offset;
	}

	void SVISlide::setShadowOpacity(const SVIFloat& opacity) {
		SVIAutoLock(mLock);
		mShadowOpacity = opacity;
	}

	void SVISlide::setLightOpacity(const SVIFloat& opacity) {
		SVIAutoLock(mLock);
		mLightOpacity = opacity;
	}

	void SVISlide::setLightColor(const SVIColor& color) {
		SVIAutoLock(mLock);
		mLightColor = color;
	}

    void SVISlide::setLightDirection(const SVIVector4& direction) {
        SVIAutoLock(mLock);
        mLightDirection = direction;
    }

    void SVISlide::setTesselationPointList(const std::vector<SVIVector2>& pointList)
    {
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_TESSELATION;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);

        SVIAutoLock(mLock);
        mTesselationPointList = pointList;
    }

	void SVISlide::setViewDirection(const SVIVector4& direction) {
		SVIAutoLock(mLock);
		mViewDirection = direction;
	}

	void SVISlide::setLightOffset(const SVIPoint& offset) {
		SVIAutoLock(mLock);
		mLightOffset = offset;
	}

	void SVISlide::setLightType(const SVIInt& type) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mLightType = type;
	}

	void SVISlide::setLightRadius(const SVIFloat& radius) {
		SVIAutoLock(mLock);
		mLightRadius = radius;
	}

	void SVISlide::setLightAngle(const SVIFloat& angle) {
		SVIAutoLock(mLock);
		mLightAngle = angle;
	}

	void SVISlide::setLightPower(const SVIFloat& power) {
		SVIAutoLock(mLock);
		mLightPower = power;
	}

	void SVISlide::setZOrderType(SlideZOrderType type) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_ZORDER_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mZOrderType = type;
	}

	void SVISlide::setSwipeType(SlideSwipeType type){
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_SWIPE_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mSwipeType = type;
	}


	void SVISlide::setDeformType(SlideDeformType type){
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_DEFORM_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mDeformType = type;
	}

    //2013-06-26 masterkeaton27@gmail.com
    //filter type added for image process from shader
    void SVISlide::setFilterType(SlideFilterType type){
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_FILTER_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);

        SVIAutoLock(mLock);
        mFilterType = type;
        LOGE("SVISlide mFilterType %d", mFilterType);
    }

    //2013-06-21 masterkeaton27@gmail.com
    //path point api for path polygon
    void SVISlide::addPathPoint(const SVIVector3& point){
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mParam1 = (int)(new SVIVector3(point));
        msg.mMessage = SVIMessageHandler::SM_ADD_PATH_POINT;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);
    }

    void SVISlide::clearPathPoints(){
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_CLEAR_PATH_POINTS;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);
    }

	void SVISlide::setDeformDirection(SlideDeformDirection direction){
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_DEFORM_DIRECTION;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mDeformDirection = direction;
	}

    void SVISlide::setMorphingType(SlideMorphingType type){
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_MORPHING_TYPE;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);

        SVIAutoLock(mLock);
        mMorphingType = type;
    }

    void SVISlide::setMorphingRadius(const SVIFloat radius)
    {
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_MORPHING_RADIUS;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);

        SVIAutoLock(mLock);
        mMorphingRadius = radius;
    }

    void SVISlide::setMorphingVertexCount(SVIInt vertexCount)
    {
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_MORPHING_GRID_COUNT;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);

        SVIAutoLock(mLock);
        mMorphingVertexCount = vertexCount;
    }

    void SVISlide::setMorphingColor(SVIColor morphingColor)
    {
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_MORPHING_COLOR;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);

        SVIAutoLock(mLock);
        mMorphingColor = morphingColor;
    }

	void SVISlide::setRTEffectType(SlideRTEffectType type) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_RT_EFFECT_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mRTEffectType = type;
	}


	void SVISlide::setBlendType(SlideBlendType type) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_BLEND_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mBlendType = type;
	}

	//2011.12.09 jongchae.moon
	void SVISlide::setAntiAliasing(SVIBool enableAA) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_ANTI_ALIASING;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mEnableAA = enableAA;
	}
	//

	void SVISlide::setImageScaleType(SlideImageScaleType type) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_IMAGESCALE_TYPE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mImageScaleType = type;
	}

	void SVISlide::setOrthogonal(SVIBool isOrtho) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_ORTHOGONAL;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mOrthogonal = isOrtho;
	}

    void SVISlide::setUseNormalMap(SVIBool isUseNormalMap) {
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_USENORMALMAP;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

        SVIAutoLock(mLock);
        mUseNormalMap = isUseNormalMap;
    }

	void SVISlide::setCustomRTE(SVIBool isCustomRTE) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_CUSTOM_RTE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mCustomRTE = isCustomRTE;
	}

	void SVISlide::setHidden(SVIBool hidden) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_HIDDEN;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mHidden = hidden;
	}

	void SVISlide::setHoldOpacity(SVIBool hold) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_HOLD_OPACITY;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mHoldOpacity = hold;
	}

	void SVISlide::setHoldScale(SVIBool hold) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_HOLD_SCALE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mHoldScale = hold;
	}

	void SVISlide::setCapture(SVIBool capture) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_CAPTURE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
        mCapture = capture;
    }

    void SVISlide::setTextureRegion(const SVIRect& region) {
        SVIAutoLock(mLock);
        mTextureSpriteRegion = region;
    }

    void SVISlide::setTextureScaleFitRegion(const SVIRect& region) {
        SVIAutoLock(mLock);
        mTextureScaleFitRegion = region;
    }

    void SVISlide::setRotationFrom(const SVIVector4& rotationFrom, const SVIVector4& rotationTo) {
        SVIAutoLock(mLock);

        SVIFloat x, y, z;
        mRotationType = (SVIInt)rotationTo.w;

		x = SVI_DEGTORAD(rotationTo.x);
		y = SVI_DEGTORAD(rotationTo.y);
		z = SVI_DEGTORAD(rotationTo.z);
		SVI::SVIQuaternion q;

		q.setIdentity();
		q.setEuler(SVI::SVIVector3(x, y, z));
		mRotation = SVI::SVIVector4(q.x, q.y, q.z, q.w);

		x = SVI_DEGTORAD(rotationFrom.x);
		y = SVI_DEGTORAD(rotationFrom.y);
		z = SVI_DEGTORAD(rotationFrom.z);
		q.setIdentity();
		q.setEuler(SVI::SVIVector3(x, y, z));
		mRotationFrom = SVI::SVIVector4(q.x, q.y, q.z, q.w);
    }

    void SVISlide::setImage(SVIImage* image) {
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_IMAGE;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

        SVIAutoLock(mLock);
        mImage = image;	
    }

    SVIImage* SVISlide::getImage() {
        return mImage;
    }


    void SVISlide::setNormalImage(SVIImage* image){
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_NORMAL_IMAGE;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

        SVIAutoLock(mLock);
        mNormalImage = image;	            
    } 
    SVIImage* SVISlide::getNormalImage(){return mNormalImage;}

    void SVISlide::setSpecularImage(SVIImage* image){
        SVIMessage msg;
        msg.mSlide = (SVISlide*)(this);
        msg.mMessage = SVIMessageHandler::SM_SET_SPECULAR_IMAGE;
        mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

        SVIAutoLock(mLock);
        mSpecularImage = image;	
    } 
    SVIImage* SVISlide::getSpecularImage(){return mSpecularImage;}


	void SVISlide::setBackFaceImage(SVIImage* image){
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_BACKFACE_IMAGE;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		mBackFaceImage = image;	
	}
	
	SVIImage* SVISlide::getBackFaceImage(){
		return mBackFaceImage;
	}
	
	
	void SVISlide::setBackFaceImageFlip(SVIBool isFlip) {
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_BACKFACE_IMAGE_FLIP;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		mIsBackFaceImageFlip = isFlip;
	}

	void SVISlide::setBackFaceImageClear(SVIBool isClear){
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_SET_BACKFACE_IMAGE_CLEAR;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support
		mIsBackFaceImageClear = isClear;
	}

	SVIBool SVISlide::getBackFaceImageClear(){
		return mIsBackFaceImageClear;
	}

	SVIBool SVISlide::getBackFaceImageFlip() {
		return mIsBackFaceImageFlip;
	}


	void SVISlide::setImplicitAnimationListenerInfo(SVIInt proxy, SVIUInt listenerID) {
		SVIAutoLock(mLock);
		mImplicitProxy = (SVIAnimationListenerProxy*)proxy;
		mImplicitListenerID = listenerID;
	}
	
	//function that has mutex lock object is called from Main Thread.
	void SVISlide::addModelSlide(SVISlide* slide, SVIInt index) {
		//Thread protect with slide tree modification.
		SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

		if( slide == NULL ) {
			DEBUG_CHECK_LOGE("SVISlide::addSlide : condition error!!");
		}
		
		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_INSERT_SLIDE;
		msg.mParam1 = (SVIInt)slide;
		msg.mParam2 = (SVIInt)index;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		if( !SVIBaseSlide::insertSubSlide(index, slide) ) {
			DEBUG_CHECK_LOGE("SVISlide::addSlide : insertSubSlide error!!");
		}
	}
	
	void SVISlide::addModelSlide(SVISlide* slide) {
		//Thread protect with slide tree modification.
		SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

		if( slide == NULL ) {
			LOGE("SVISlide::addSubSlide : slide is NULL !!!");
		}

		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_ADD_SLIDE;
		msg.mParam1 = (SVIInt)slide;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		if( !SVIBaseSlide::addSubSlide(slide) ) {
			LOGE("SVISlide::addSubSlide : failed to add slide!!!");
		}
	}
	
	void SVISlide::removeModelSlide(SVISlide* slide) {
		//Thread protect with slide tree modification.
		SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

		if( slide == NULL ) {
			DEBUG_CHECK_LOGE("SVISlide::removeSlide : condition error!!");
		}

		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_REMOVE_SLIDE;
		msg.mParam1 = (SVIInt)slide;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support
		
		SVIAutoLock(mLock);
		if( !SVIBaseSlide::removeSubSlide(slide) ) { 
			LOGE("SVISlide::removeSlide : SVIBaseSlide::removeSubSlide() !!!");
		}
	}
	
	//function that does not have mutex lock object is called from Render Thread.
	void SVISlide::addSlide(SVISlide* slide, SVIInt index) {
		if( slide == NULL ) {
			DEBUG_CHECK_LOGE("SVISlide::addSlide : condition error!!");
		}
		if( mBeamedSlide != NULL  && slide != NULL){
			mBeamedSlide->insertSubSlide(index, slide->getBeamedSlide());
		}
	}
	
	void SVISlide::addSlide(SVISlide* slide) {
		if( slide == NULL ) {
			LOGE("SVISlide::addSubSlide : slide is NULL !!!");
		}

		if( mBeamedSlide != NULL && slide != NULL && slide->getBeamedSlide() != NULL) {
			if( !mBeamedSlide->addSubSlide(slide->getBeamedSlide()) ) {
				LOGE("SVISlide::addSubSlide : failed to add beamed slide!!!");
			}
		}
	}
	
	void SVISlide::removeSlide(SVISlide* slide) {
		SVIBeamedSlide* beamedSlide = slide->getBeamedSlide();

		if( mBeamedSlide != NULL ) {
			if( !mBeamedSlide->removeSubSlide(beamedSlide) ) {
				LOGE("SVISlide::removeSlide :  !mBeamedSlide->removeSubSlide(beamedSlide) !!!");
			}
		}
	}

	//At this time, don not support functions for removeAll, removeAt, removeSlides...
	//Above functions will be provided next time after finishing refactoring...
	//2011.10.25 (marx.kim)

	void SVISlide::removeAllSlides() {
		DEBUG_CHECK_LOGE("SVISlide::removeAllSlides : viewGroup's slide=%d", (SVIUInt)this);
		//Thread protect with slide tree modification.
		SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

		DEBUG_CHECK_LOGE("SVISlide::removeAllSlides : viewGroup's slide=%d", this);

			while( mSubSlideList.size() > 0 ) {
				this->removeSlideAt(0);
		}
	}

	void SVISlide::removeSlideAt(SVIInt index) {
		DEBUG_CHECK_LOGE("SVISlide::removeSlideAt : viewGroup's slide=%d, index=%d", (SVIUInt)this, index);
		//Thread protect with slide tree modification.
		SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

		DEBUG_CHECK_LOGE("SVISlide::removeSlideAt : viewGroup's slide=%d, index=%d", this, index);
		
		BaseSlideListIter iter = mSubSlideList.end();
		SVISlide* slide = NULL;
		SVIInt counter = 0;
		
		if( index < 0 || (SVIUInt)index > mSubSlideList.size() ) {
			DEBUG_CHECK_LOGE("SVISlide::removeSlideAt : index error!!");	
			return;
		}
		
		for(iter = mSubSlideList.begin(); iter != mSubSlideList.end(); iter++) {
			if( counter == index ) {
				slide = (SVISlide*)(*iter);
				this->removeSlide(slide);
				break;
			}	
		}
	}

	void SVISlide::removeSlides(SVIInt start, SVIInt count) {
		DEBUG_CHECK_LOGE("SVISlide::removeSlideAt : viewGroup's slide=%d, start=%d, count=%d", (SVIUInt)this, start, count);
		//Thread protect with slide tree modification.
		SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

		DEBUG_CHECK_LOGE("SVISlide::removeSlideAt : viewGroup's slide=%d, start=%d, count=%d", this, start, count);
		SVIInt i;

		for (i = start; i<start+count; i++) {
			this->removeSlideAt(i);
		}	
	}

	SVIBool SVISlide::addAnimation(SVIAnimation* animation) { 
		if( animation == NULL || mBeamedSlide == NULL )
			return SVIFALSE;

		return mBeamedSlide->addAnimation(animation);
	}

	void SVISlide::removeAnimation() {
		if( mBeamedSlide == NULL ) {
			LOGE("SVISlide::removeAnimation : mBeamedSlide=NULL");
			return;
		}	

		mBeamedSlide->removeAnimation();
	}

	void SVISlide::addParticleEffect(SVIParticleEffect* effect) {
		if(effect == NULL) {
			return;
		}
		
		mParticleEffectList.push_back(effect);

		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mMessage = SVIMessageHandler::SM_ADD_PARTICLE_EFFECT;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support
	}

	void SVISlide::startAnimation(SVIAnimation* animation) {
		if( mBeamedSlide != NULL ) {
			if (!mBeamedSlide->addAnimation(animation)) {
				LOGE("SVISlide::startAnimation : addAnimation failed!");
				return;
			}
		}
	}

	void SVISlide::stopAnimation() {
		SVIMessage msg;
		msg.mMessage = SVIMessageHandler::SM_STOP_ANIMATION;
		msg.mSlide = this;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support
	}

	void SVISlide::stopAnimationDirectly() {
		if( isAnimating() ) {
			if( mBeamedSlide != NULL )
				mBeamedSlide->stopAnimation();
		}
	}

	void SVISlide::setExplicitAnimation(SVIAnimation* explicitAnimation) {
		// In case of explicit animation, user can use one explicit animation instance many times to set animation.
		// So should clone instance for multiple using.
		SVIAnimation* newAnimation = explicitAnimation->clone();

		DEBUG_CHECK_LOGI("setExplicitAnimation new:%x, clone:%x", (SVIUInt)explicitAnimation, (SVIUInt)newAnimation);
		SVIMessage msg;
		msg.mMessage = SVIMessageHandler::SM_ADD_EXPLICIT_ANIMATION;
		msg.mSlide = this;
		msg.mParam1 = (SVIInt)newAnimation;
		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support

		SVIAutoLock(mLock);
		//if mExplicitAnimation is not NULL, SVI is not handled explicit animation yet.
		//So should delete current instance to prevent memory leak.
		if( mExplicitAnimation != NULL )
			SVI_SVIFE_DELETE(mExplicitAnimation);

		mExplicitAnimation = newAnimation;
	}

	void SVISlide::setImplicitAnimation(SVIInt type, SVIInt interpolatorType, SVIInt duration, SVIInt repeatCnt, SVIInt offset, SVIInt autoReverse) {

		if( type >= SVIPropertyAnimation::PROPERTY_ANI_COUNT || type <= SVIPropertyAnimation::NONE ) {
			LOGE("Invalid property type is entered SVISlide::setImplicitAnimation %d", (SVIInt)type);
			return;
		}

		if( duration > 0 ) {
			mImplicitAnimationInfo[type][ISSETPROPERTY] = SVITRUE;
			mImplicitAnimationInfo[type][INTERPOLATORTYPE] = interpolatorType;
			mImplicitAnimationInfo[type][DURATION] = duration;
			mImplicitAnimationInfo[type][REPEATCOUNT] = repeatCnt;
			mImplicitAnimationInfo[type][OFFSET] = offset;
			mImplicitAnimationInfo[type][AUTOREVERSE] = autoReverse;
		}

		SVIMessage msg;
		msg.mSlide = (SVISlide*)(this);
		msg.mParam1 = duration;
		msg.mParam2 = type;

		switch (type) {
		case SVIPropertyAnimation::REGION:
			msg.mMessage = SVIMessageHandler::SM_SET_REGION;
			break;
		case SVIPropertyAnimation::BOUND:
			msg.mMessage = SVIMessageHandler::SM_SET_BOUND;
			break;
		case SVIPropertyAnimation::POSITION:
			msg.mMessage = SVIMessageHandler::SM_SET_POSITION;
			break;
		case SVIPropertyAnimation::PIVOT_POINT:
			msg.mMessage = SVIMessageHandler::SM_SET_PIVOT_POINT;
			break;
		case SVIPropertyAnimation::ZPOSITION:
			msg.mMessage = SVIMessageHandler::SM_SET_ZPOSITION;
			break;
		case SVIPropertyAnimation::ROTATION:
			msg.mMessage = SVIMessageHandler::SM_SET_ROTATION;
			break;		
		case SVIPropertyAnimation::SWIPE_RATIO:
			msg.mMessage = SVIMessageHandler::SM_SET_SWIPE_RATIO;
			break;
		case SVIPropertyAnimation::DEFORM_RATIO:
			msg.mMessage = SVIMessageHandler::SM_SET_DEFORM_RATIO;
			break;
		case SVIPropertyAnimation::RTE_RATIO:
			msg.mMessage = SVIMessageHandler::SM_SET_RTE_RATIO;
			break;
		case SVIPropertyAnimation::SCALE:
			msg.mMessage = SVIMessageHandler::SM_SET_SCALE;
			break;
		case SVIPropertyAnimation::BG_COLOR:
			msg.mMessage = SVIMessageHandler::SM_SET_BG_COLOR;
			break;
		case SVIPropertyAnimation::OPACITY:
			msg.mMessage = SVIMessageHandler::SM_SET_OPACITY;
			break;
		case SVIPropertyAnimation::CORNER_RADIUS:
			msg.mMessage = SVIMessageHandler::SM_SET_CORNER_RADIUS;
			break;
		case SVIPropertyAnimation::BORDER_WIDTH:
			msg.mMessage = SVIMessageHandler::SM_SET_BORDER_WIDTH;
			break;
		case SVIPropertyAnimation::BORDER_COLOR:
			msg.mMessage = SVIMessageHandler::SM_SET_BORDER_COLOR;
			break;
		case SVIPropertyAnimation::SHADOW_RADIUS:
			msg.mMessage = SVIMessageHandler::SM_SET_SHADOW_RADIUS;
			break;
		case SVIPropertyAnimation::SHADOW_COLOR:
			msg.mMessage = SVIMessageHandler::SM_SET_SHADOW_COLOR;
			break;
		case SVIPropertyAnimation::SHADOW_OPACITY:
			msg.mMessage = SVIMessageHandler::SM_SET_SHADOW_OPACITY;
			break;
		case SVIPropertyAnimation::SHADOW_OFFSET:
			msg.mMessage = SVIMessageHandler::SM_SET_SHADOW_OFFSET;
			break;	
		case SVIPropertyAnimation::TEXTURE_REGION:
			msg.mMessage = SVIMessageHandler::SM_SET_TEXTURE_REGION;
			break;	
		case SVIPropertyAnimation::BACKFACE_TEXTURE_REGION:
			msg.mMessage = SVIMessageHandler::SM_SET_BACKFACE_TEXTURE_REGION;
			break;	
		case SVIPropertyAnimation::LIGHT_RADIUS:
			msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_RADIUS;
			break;	
		case SVIPropertyAnimation::LIGHT_COLOR:
			msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_COLOR;
			break;	
        case SVIPropertyAnimation::LIGHT_DIRECTION:
            msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_DIRECTION;
            break;	
		case SVIPropertyAnimation::VIEW_DIRECTION:
			msg.mMessage = SVIMessageHandler::SM_SET_VIEW_DIRECTION;
			break;	
		case SVIPropertyAnimation::LIGHT_OPACITY:
			msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_OPACITY;
			break;	
		case SVIPropertyAnimation::LIGHT_ANGLE:
			msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_ANGLE;
			break;	
		case SVIPropertyAnimation::LIGHT_POWER:
			msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_POWER;
			break;	
		case SVIPropertyAnimation::LIGHT_OFFSET:
			msg.mMessage = SVIMessageHandler::SM_SET_LIGHT_OFFSET;
			break;	
		case SVIPropertyAnimation::TEXTURE_SCALEFIT_REGION:
			msg.mMessage = SVIMessageHandler::SM_SET_IMAGESCALE_REGION;
			break;	
		case SVIPropertyAnimation::ROTATION_FROM:
			msg.mMessage = SVIMessageHandler::SM_SET_ROTATION_FROM;
			break;
		case SVIPropertyAnimation::MODEL_POSITION:
			msg.mMessage = SVIMessageHandler::SM_SET_MODEL_POSITION;
			break;
		case SVIPropertyAnimation::MODEL_ROTATION:
			msg.mMessage = SVIMessageHandler::SM_SET_MODEL_ROTATION;
			break;
		case SVIPropertyAnimation::MODEL_SCALE:
			msg.mMessage = SVIMessageHandler::SM_SET_MODEL_SCALE;
			break;

        default:
            break;
		}

		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support
	}	

	SVIBool SVISlide::isAnimating() {
		if( mBeamedSlide == NULL )
			return SVIFALSE;

		return mBeamedSlide->isAnimating();
	}

	void SVISlide::addImplicitAnimation(SVIPropertyAnimation::PropertyAnimationType propertyAniType) {
		if( mBeamedSlide == NULL )
			return;

		if( mImplicitAnimation == NULL )
			mImplicitAnimation = new SVIAnimationSet(mSVIGLSurface);

		SVIBasicAnimation* animation = NULL;

		DEBUG_CHECK_LOGI("SVISlide::addImplicitAnimation aniType:%s", SVIPropertyAnimation::getPropertyTypeString(propertyAniType));
		if( propertyAniType != SVIPropertyAnimation::ROTATION_FROM ) {
			animation = SVIBasicAnimation::create(propertyAniType, mSVIGLSurface);
			setAnimationFromAndTo(propertyAniType, (SVIBasicAnimation*&)animation);		
			animation->setInterpolator((SVIAnimation::InterpolatorType)mImplicitAnimationInfo[propertyAniType][INTERPOLATORTYPE]);
			animation->mDuration = mImplicitAnimationInfo[propertyAniType][DURATION];
			animation->mRepeatCount = mImplicitAnimationInfo[propertyAniType][REPEATCOUNT];
			animation->mOffset = mImplicitAnimationInfo[propertyAniType][OFFSET];
			animation->mAutoReverse = (SVIBool)mImplicitAnimationInfo[propertyAniType][AUTOREVERSE];
			mImplicitAnimation->setInterpolator((SVIAnimation::InterpolatorType)mImplicitAnimationInfo[propertyAniType][INTERPOLATORTYPE]);
			mImplicitAnimation->addAnimation(animation);
		} else {
			//SVIAutoLock(mLock);
			animation = SVIBasicAnimation::create(SVIBasicAnimation::ROTATION, mSVIGLSurface);
			animation->mFrom = mRotationFrom;
			animation->mTo = mRotation;

			animation->setInterpolator((SVIAnimation::InterpolatorType)mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][INTERPOLATORTYPE]);
			animation->mDuration = mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][DURATION];
			animation->mRepeatCount = mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][REPEATCOUNT];
			animation->mOffset = mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][OFFSET];
			animation->mAutoReverse = (SVIBool)mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][AUTOREVERSE];
			mImplicitAnimation->addAnimation(animation);


			SVIInt statusBarHeight = 48;
			SVIInt titleBarHeight = 56;			
			SVIRect rect = mBeamedSlide->getRegion();
			SVIRect sRect = mRegion;

			DEBUG_CHECK_LOGI("From region (%f, %f, %f, %f)", rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth, rect.mSize.mHeight);
			DEBUG_CHECK_LOGI("To region (%f, %f, %f, %f)", sRect.mOrigin.mX, sRect.mOrigin.mY, sRect.mSize.mWidth, sRect.mSize.mHeight);

			SVIBasicAnimation* animation1 = SVIBasicAnimation::create(SVIBasicAnimation::POSITION, mSVIGLSurface);
			SVIPoint fromPosition;

			if (mRotationType == CLOCKWISE_90) {
				fromPosition = SVIPoint(rect.mSize.mHeight/2.0f, rect.mSize.mWidth/2.0f);
			}	
			else if (mRotationType == COUNTERCLOCKWISE_90){
				fromPosition = SVIPoint(rect.mSize.mHeight/2.0f+statusBarHeight, rect.mSize.mWidth/2.0f);	
			}
			else {
				fromPosition = SVIPoint(rect.mSize.mWidth/2.0f, rect.mSize.mHeight/2.0f+statusBarHeight);
			}			
			animation1->mFrom = fromPosition;
			animation1->mTo = mPosition;
			DEBUG_CHECK_LOGI("From position (%f, %f) type:%d", fromPosition.mX, fromPosition.mY, mRotationType);
			DEBUG_CHECK_LOGI("To position (%f, %f) type:%d", mPosition.mX, mPosition.mY, mRotationType);

			animation->setInterpolator((SVIAnimation::InterpolatorType)mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][INTERPOLATORTYPE]);
			animation1->mDuration = mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][DURATION];
			animation1->mRepeatCount = mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][REPEATCOUNT];
			animation1->mOffset = mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][OFFSET];
			animation1->mAutoReverse = (SVIBool)mImplicitAnimationInfo[SVIPropertyAnimation::ROTATION_FROM][AUTOREVERSE];
			mImplicitAnimation->addAnimation(animation1);
		}

		if (animation->mDuration > mImplicitAnimation->mDuration) {
			mImplicitAnimation->mDuration = animation->mDuration; 
		}

		if (animation->mRepeatCount > mImplicitAnimation->mRepeatCount) {
			mImplicitAnimation->mRepeatCount = animation->mRepeatCount; 
		}

		if (animation->mOffset > mImplicitAnimation->mOffset) {
			mImplicitAnimation->mOffset = animation->mOffset; 
		}

	}

	void SVISlide::setAnimationFromAndTo(SVIPropertyAnimation::PropertyAnimationType& propertyAniType, SVIBasicAnimation*& animation) {
		if( mBeamedSlide == NULL )
			return;

		SVIBool needInitProperty = SVIFALSE;
		InitPropertyMap::iterator iter = mInitPropertyMap.find((SVIInt)propertyAniType);
		if( iter != mInitPropertyMap.end() )
			needInitProperty = SVITRUE;

		switch(propertyAniType) {
		case SVIPropertyAnimation::REGION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mRegion;
			else
				animation->mFrom = mBeamedSlide->getRegion();

			animation->mTo = mRegion;
			break;
		case SVIPropertyAnimation::BOUND:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mBounds;
			else
				animation->mFrom = mBeamedSlide->getBounds();

			animation->mTo = mBounds;
			break;
		case SVIPropertyAnimation::POSITION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mPosition;
			else
				animation->mFrom = mBeamedSlide->getPosition();

			animation->mTo = mPosition;
			break;
		case SVIPropertyAnimation::PIVOT_POINT:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mPivotPoint;
			else
				animation->mFrom = mBeamedSlide->getPivotPoint();

			animation->mTo = mPivotPoint;
			break;
		case SVIPropertyAnimation::ZPOSITION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mZPosition;
			else
				animation->mFrom = mBeamedSlide->getZPosition();

			animation->mTo = mZPosition;
			break;
		case SVIPropertyAnimation::ROTATION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mRotation;
			else
				animation->mFrom = mBeamedSlide->getRotation();

			animation->mTo = mRotation;
			break;
		case SVIPropertyAnimation::SCALE:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mScale;
			else
				animation->mFrom = mBeamedSlide->getScale();
			
			animation->mTo = mScale;
			break;
		case SVIPropertyAnimation::BG_COLOR:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mBgColor;
			else
				animation->mFrom = mBeamedSlide->getBgColor();

			animation->mTo = mBgColor;
			break;
		case SVIPropertyAnimation::OPACITY:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mOpacity;
			else
				animation->mFrom = mBeamedSlide->getOpacity();

			animation->mTo = mOpacity;
			break;
		case SVIPropertyAnimation::CORNER_RADIUS:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mCornerRadius;
			else
				animation->mFrom = mBeamedSlide->getCornerRadius();

			animation->mTo = mCornerRadius;
			break;
		case SVIPropertyAnimation::BORDER_WIDTH:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mBorderWidth;
			else
				animation->mFrom = mBeamedSlide->getBorderWidth();

			animation->mTo = mBorderWidth;
			break;
		case SVIPropertyAnimation::BORDER_COLOR:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mBorderColor;
			else
				animation->mFrom = mBeamedSlide->getBorderColor();

			animation->mTo = mBorderColor;
			break;
		case SVIPropertyAnimation::SHADOW_RADIUS:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mShadowRadius;
			else
				animation->mFrom = mBeamedSlide->getShadowRadius();

			animation->mTo = mShadowRadius;
			break;
		case SVIPropertyAnimation::SHADOW_COLOR:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mShadowColor;
			else
				animation->mFrom = mBeamedSlide->getShadowColor();

			animation->mTo = mShadowColor;
			break;
		case SVIPropertyAnimation::SHADOW_OPACITY:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mShadowOpacity;
			else
				animation->mFrom = mBeamedSlide->getShadowOpacity();

			animation->mTo = mShadowOpacity;
			break;
		case SVIPropertyAnimation::SHADOW_OFFSET:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mShadowOffset;
			else
				animation->mFrom = mBeamedSlide->getShadowOffset();

			animation->mTo = mShadowOffset;
			break;
		case SVIPropertyAnimation::TEXTURE_REGION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mTextureSpriteRegion;
			else
				animation->mFrom = mBeamedSlide->getTextureRegion();

			animation->mTo = mTextureSpriteRegion;
			break;

		case SVIPropertyAnimation::BACKFACE_TEXTURE_REGION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mTextureBackFaceSpriteRegion;
			else
				animation->mFrom = mBeamedSlide->getBackFaceTextureRegion();
			animation->mTo = mTextureBackFaceSpriteRegion;
			break;

		case SVIPropertyAnimation::LIGHT_RADIUS:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mLightRadius;
			else
				animation->mFrom = mBeamedSlide->getLightRadius();

			animation->mTo = mLightRadius;
			break;
		case SVIPropertyAnimation::LIGHT_COLOR:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mLightColor;
			else
				animation->mFrom = mBeamedSlide->getLightColor();

			animation->mTo = mLightColor;
			break;

        case SVIPropertyAnimation::LIGHT_DIRECTION:
            if( needInitProperty )
                animation->mFrom = mInitPropertySlide->mLightDirection;
            else
                animation->mFrom = mBeamedSlide->getLightDirection();

            animation->mTo = mLightDirection;
            break;

		case SVIPropertyAnimation::VIEW_DIRECTION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mViewDirection;
			else
				animation->mFrom = mBeamedSlide->getViewDirection();

			animation->mTo = mViewDirection;
			break;

		case SVIPropertyAnimation::LIGHT_OPACITY:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mLightOpacity;
			else
				animation->mFrom = mBeamedSlide->getLightOpacity();

			animation->mTo = mLightOpacity;
			break;
		case SVIPropertyAnimation::LIGHT_ANGLE:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mLightAngle;
			else
				animation->mFrom = mBeamedSlide->getLightAngle();

			animation->mTo = mLightAngle;
			break;
		case SVIPropertyAnimation::LIGHT_OFFSET:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mLightOffset;
			else
				animation->mFrom = mBeamedSlide->getLightOffset();

			animation->mTo = mLightOffset;
			break;
		case SVIPropertyAnimation::LIGHT_POWER:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mLightPower;
			else
				animation->mFrom = mBeamedSlide->getLightPower();

			animation->mTo = mLightPower;
			break;
		case SVIPropertyAnimation::TEXTURE_SCALEFIT_REGION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mTextureScaleFitRegion;
			else
				animation->mFrom = mBeamedSlide->getTextureScaleFitRegion();

			animation->mTo = mTextureScaleFitRegion;
			break;

		case SVIPropertyAnimation::DEFORM_RATIO:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mDeformRatio;
			else
				animation->mFrom = mBeamedSlide->getDeformRatio();

			animation->mTo = mDeformRatio;
			break;

		case SVIPropertyAnimation::SWIPE_RATIO:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mSwipeRatio;
			else
				animation->mFrom = mBeamedSlide->getSwipeRatio();

			animation->mTo = mSwipeRatio;
			break;

		case SVIPropertyAnimation::RTE_RATIO:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mRTERatio;
			else
				animation->mFrom = mBeamedSlide->getRTERatio();

			animation->mTo = mRTERatio;
			break;

		case SVIPropertyAnimation::MODEL_POSITION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mModelPosition;
			else
				animation->mFrom = mBeamedSlide->getModelPosition();

			animation->mTo = mModelPosition;
			break;

		case SVIPropertyAnimation::MODEL_ROTATION:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mModelRotation;
			else
				animation->mFrom = mBeamedSlide->getModelRotation();

			animation->mTo = mModelRotation;
			break;

		case SVIPropertyAnimation::MODEL_SCALE:
			if( needInitProperty )
				animation->mFrom = mInitPropertySlide->mModelScale;
			else
				animation->mFrom = mBeamedSlide->getModelScale();

			animation->mTo = mModelScale;
			break;

        default:
            break;
		}
	}
	
	void SVISlide::destroySlide() {
		SVIMessage msg;
		msg.mSlide = NULL;
		msg.mParam1 = (SVIInt)this;
		msg.mMessage = SVIMessageHandler::SM_FREE_SLIDE;

		mSVIGLSurface->getMessageHandler()->sendMessage(msg);//Multi-Instance-Support
	}

	void SVISlide::runSlideAnimation() {
		if( mBeamedSlide == NULL )
			return;

		if( mIsExplicitSet ) {
			SVIAutoLock(mLock);
			DEBUG_CHECK_LOGI("Type:%d, duration:%d, repeatCount:%d listener:%x", mExplicitAnimation->getClassType(), 
				mExplicitAnimation->mDuration, 
				mExplicitAnimation->mRepeatCount, 
				(SVIUInt)mExplicitAnimation->getListener());
			mBeamedSlide->addAnimation(mExplicitAnimation);
			mExplicitAnimation = NULL;
			mIsExplicitSet = SVIFALSE;
		} else {
			if( mImplicitAnimation != NULL ) {
				SVIAutoLock(mLock);
				if (mImplicitProxy != NULL) {
					mImplicitAnimation->setListener((SVIAnimationListener*)mImplicitProxy);
					mImplicitAnimation->setID(mImplicitListenerID);
					mImplicitProxy = NULL;
				}
				mBeamedSlide->addAnimation(mImplicitAnimation);
				mImplicitAnimation = NULL;
			}
		}
	}

	void SVISlide::runExplicitAnimation() {
	}

	void SVISlide::slideProc(SVIMessage& msg) {

		if( mBeamedSlide == NULL )
			return;

		SVIInt duration = msg.mParam1;

		//If explicit animation is set, implicit animation can't be enable.
		if( mIsExplicitSet )
			duration = 0;

		SVIAutoLock(mLock);
		DEBUG_CHECK_LOGI("SVISlide::slideProc Slide:%x, Msg:%s duration:%d", (SVIUInt)this, SVIMessageHandler::getMessageString(msg.mMessage), duration);
		//LOGI("SVISlide::slideProc Slide:%x, Msg:%s duration:%d", this, SVIMessageHandler::getMessageString(msg.mMessage), duration);
		switch(msg.mMessage){
		case SVIMessageHandler::SM_ADD_EXPLICIT_ANIMATION :
			{
				if( mExplicitAnimation != NULL ) {
					if( mExplicitAnimation->getClassType() == SVIAnimation::TRANSITION ) {
						SVITransitionAnimation* ani = (SVITransitionAnimation*)mExplicitAnimation;
						ani->prepareTransitionAnimation(mBeamedSlide);
					}
					mIsExplicitSet = SVITRUE;
				}
			}
			break;
		case SVIMessageHandler::SM_STOP_ANIMATION:
			
			if( mBeamedSlide != NULL ){
				mBeamedSlide->stopAnimation();
			}
			break;
		case SVIMessageHandler::SM_ADD_SLIDE:
			{
				SVISlide* subSlide = (SVISlide*)msg.mParam1;

				DEBUG_CHECK_LOGI("Parent:%x, Child :%x", (SVIUInt)this, (SVIUInt)subSlide);
				if( subSlide != NULL )
					addSlide(subSlide);
				break;
			}
		case SVIMessageHandler::SM_INSERT_SLIDE:
			{
				SVISlide* subSlide = (SVISlide*)msg.mParam1;
				SVIInt index = msg.mParam2;
				if( subSlide != NULL )
					addSlide(subSlide, index);

				DEBUG_CHECK_LOGI("SM_INSERT_SLIDE : subSlide count:%d index:%d", getSubSlideCount(), index);
				break;
			}
			break;
		case SVIMessageHandler::SM_REMOVE_SLIDE:
			{
				SVISlide* subSlide = (SVISlide*)msg.mParam1;

				if( subSlide != NULL )
					removeSlide(subSlide);
				break;
			}
		case SVIMessageHandler::SM_SET_REGION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setRegion(getRegion());
			}
			break;
		case SVIMessageHandler::SM_SET_BOUND:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setBounds(getBounds());
			}
			break;	
		case SVIMessageHandler::SM_SET_POSITION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setPosition(getPosition());
			}
			break;
		case SVIMessageHandler::SM_SET_PIVOT_POINT:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setPivotPoint(getPivotPoint());
			}
			break;
		case SVIMessageHandler::SM_SET_ZPOSITION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setZPosition(getZPosition());
			}
			break;
		case SVIMessageHandler::SM_SET_ROTATION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setRotation(getRotation());
			}
			break;

		case SVIMessageHandler::SM_SET_SWIPE_RATIO:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setSwipeRatio(getSwipeRatio());
			}
			break;

		case SVIMessageHandler::SM_SET_DEFORM_RATIO:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setDeformRatio(getDeformRatio());
			}
			break;

		case SVIMessageHandler::SM_SET_DEFORM_TOUCH_DOWN:
			{
				SVIVector3 *deformRatio = (SVIVector3*)msg.mParam1;
				mBeamedSlide->deformRatioTouchDown(deformRatio->x, deformRatio->y);
				SVI_SVIFE_DELETE(deformRatio);
			}
			break;
		case SVIMessageHandler::SM_SET_RTE_RATIO:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setRTERatio(getRTERatio());
			}
			break;

		case SVIMessageHandler::SM_SET_SCALE:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setScale(getScale());
			}
			break;
		case SVIMessageHandler::SM_SET_BG_COLOR:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setBgColor(getBgColor());
			}
			break;
		case SVIMessageHandler::SM_SET_OPACITY:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setOpacity(getOpacity());
			}
			break;
		case SVIMessageHandler::SM_SET_CORNER_RADIUS:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setCornerRadius(getCornerRadius());
			}
			break;
		case SVIMessageHandler::SM_SET_BORDER_WIDTH:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setBorderWidth(getBorderWidth());
			}
			break;
		case SVIMessageHandler::SM_SET_BORDER_COLOR:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setBorderColor(getBorderColor());
			}
			break;
		case SVIMessageHandler::SM_SET_SHADOW_RADIUS:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setShadowRadius(getShadowRadius());
			}
			break;
		case SVIMessageHandler::SM_SET_SHADOW_COLOR:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setShadowColor(getShadowColor());
			}
			break;
		case SVIMessageHandler::SM_SET_SHADOW_OPACITY:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setShadowOpacity(getShadowOpacity());
			}
			break;
		case SVIMessageHandler::SM_SET_SHADOW_OFFSET:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setShadowOffset(getShadowOffset());
			}
			break;
		case SVIMessageHandler::SM_SET_TEXTURE_REGION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setTextureRegion(getTextureRegion());			
			}
			break;

		case SVIMessageHandler::SM_SET_BACKFACE_TEXTURE_REGION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setBackFaceTextureRegion(getBackFaceTextureRegion());			
			}
			break;

		case SVIMessageHandler::SM_SET_CLIP_SUBSLIDE:
			{
				mBeamedSlide->setClipSubSlide(getClipSubSlide());
			}
			break;
		case SVIMessageHandler::SM_SET_HIDDEN:
			{
				mBeamedSlide->setHidden(getHidden());
			}
			break;
		case SVIMessageHandler::SM_SET_ZORDER_TYPE:
			{
				mBeamedSlide->setZOrderType(getZOrderType());
			}
			break;

		//2012-06-21 masterkeaton27@gmail.com: swipe shader effect
		case SVIMessageHandler::SM_SET_SWIPE_TYPE:
			{
				mBeamedSlide->setSwipeType(getSwipeType());
			}
			break;

		case SVIMessageHandler::SM_SET_DEFORM_TYPE:
			{
				mBeamedSlide->setDeformType(getDeformType());
			}
			break;

        //2013-06-26 masterkeaton27@gmail.com: add filter type
        case SVIMessageHandler::SM_SET_FILTER_TYPE:
            {
			DEBUG_CHECK_LOGE("SVISlide Filter setted %d", getFilterType());
			mBeamedSlide->setFilterType(getFilterType());
            }
            break;

		case SVIMessageHandler::SM_SET_DEFORM_DIRECTION:
			{
				mBeamedSlide->setDeformDirection(getDeformDirection());
			}	
			break;

		case SVIMessageHandler::SM_SET_ORTHOGONAL:
			{
				mBeamedSlide->setOrthogonal(getOrthogonal());
			}
			break;

        case SVIMessageHandler::SM_SET_USENORMALMAP:
            {
                mBeamedSlide->setUseNormalMap(getUseNormalMap());
            }
            break;

		case SVIMessageHandler::SM_SET_CUSTOM_RTE:
			{
				mBeamedSlide->setCustomRTE(getCustomRTE());
			}
			break;
		case SVIMessageHandler::SM_SET_HOLD_OPACITY:
			{
				mBeamedSlide->setHoldOpacity(getHoldOpacity());
			}
			break;

		case SVIMessageHandler::SM_SET_HOLD_SCALE:
			{
				mBeamedSlide->setHoldScale(getHoldScale());
			}
			break;
		case SVIMessageHandler::SM_SET_CAPTURE:
			{
				SVIBool isCapture = getCapture();
				mBeamedSlide->setCapture(getCapture());
				SVIProjectionSlide * pProjectionSlide = mBeamedSlide->getProjectionSlide();
				if (isCapture){
					pProjectionSlide->getSVIGLSurface()->getSlideCapture()->start(pProjectionSlide);
				}else{
					pProjectionSlide->getSVIGLSurface()->getSlideCapture()->stop(pProjectionSlide);
				}			
				//LOGE("SM_SET_CAPTURE msg process <%s>",isCapture ? "SVITRUE" : "SVIFALSE");
			}
			break;
		case SVIMessageHandler::SM_SET_BLEND_TYPE:
			{
				mBeamedSlide->setBlendType(getBlendType());
			}
			break;

		//2011.12.09 jongchae.moon
		case SVIMessageHandler::SM_SET_ANTI_ALIASING:
			{
				mBeamedSlide->setAntiAliasing(getAntiAliasing());
			}
			break;
		//

		case SVIMessageHandler::SM_SET_RT_EFFECT_TYPE:
			{
				mBeamedSlide->setRTEffectType(getRTEffectType());
			}
			break;

		case SVIMessageHandler::SM_SET_LIGHT_TYPE:
			{
				mBeamedSlide->setLightType(getLightType());
			}
			break;	

		case SVIMessageHandler::SM_SET_IMAGESCALE_TYPE:
			{
				mBeamedSlide->setImageScaleType(getImageScaleType());
			}
			break;

		case SVIMessageHandler::SM_SET_IMAGESCALE_REGION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setTextureScaleFitRegion(getTextureScaleFitRegion());
			}
			break;
		case SVIMessageHandler::SM_SET_LIGHT_RADIUS:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setLightRadius(getLightRadius());
			}
			break;
		case SVIMessageHandler::SM_SET_LIGHT_COLOR:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setLightColor(getLightColor());
			}
			break;
        case SVIMessageHandler::SM_SET_LIGHT_DIRECTION:
            {
                if( duration > 0 )
                    addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
                else
                    mBeamedSlide->setLightDirection(getLightDirection());
            }
            break;

		case SVIMessageHandler::SM_SET_VIEW_DIRECTION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setViewDirection(getViewDirection());
			}
			break;

		case SVIMessageHandler::SM_SET_LIGHT_OPACITY:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setLightOpacity(getLightOpacity());
			}
			break;
		case SVIMessageHandler::SM_SET_LIGHT_ANGLE:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setLightAngle(getLightAngle());
			}
			break;
		case SVIMessageHandler::SM_SET_LIGHT_POWER:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setLightPower(getLightPower());
			}
			break;
		case SVIMessageHandler::SM_SET_LIGHT_OFFSET: 
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setLightOffset(getLightOffset());
			}
			break;

		case SVIMessageHandler::SM_SET_IMAGE:
			{
                //2013-04-03 masterkeaton27@gmail.com
				mBeamedSlide->setImage(mImage);
				if( mImage != NULL && mImage->mBits != NULL) {
				    mSVIGLSurface->getRenderPartManager()->setImageHandled(SVITRUE);//Multi-Instance-Support
					//LOGE("Slide:%x, mImage:%x, Bitmap:%x", this, mImage, mImage->mBits);
                }
                
			}
			break;

        //2012-12-10 masterkeaton27@gmail.comnormal map
        case SVIMessageHandler::SM_SET_NORMAL_IMAGE:
            {
                //2013-04-03 masterkeaton27@gmail.com
                mBeamedSlide->setNormalImage(mNormalImage);                               
            }
            break;

        case SVIMessageHandler::SM_SET_SPECULAR_IMAGE:
            {
                //2013-04-03 masterkeaton27@gmail.com
                mBeamedSlide->setSpecularImage(mSpecularImage);                                
            }
            break;

		//2011-11-18 masterkeaton27@gmail.com
		case SVIMessageHandler::SM_SET_BACKFACE_IMAGE:
			{
                //2013-04-03 masterkeaton27@gmail.com
                mBeamedSlide->setBackFaceImage(mBackFaceImage);                
			}
			break;

		//2012/03/30 rareboy0112@facebook.com
		case SVIMessageHandler::SM_SET_BACKFACE_IMAGE_FLIP:
			{
				mBeamedSlide->setBackFaceImageFlip(mIsBackFaceImageFlip);
			}
			break;

		//2012-04-12 masterkeaton27@gmail.com
		case SVIMessageHandler::SM_SET_BACKFACE_IMAGE_CLEAR:
			{
				mBeamedSlide->setBackFaceImageClear(mIsBackFaceImageClear);
			}
			break;

		case SVIMessageHandler::SM_SET_ROTATION_FROM:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
			}
			break;

		case SVIMessageHandler::SM_SET_MODEL_POSITION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setModelPosition(getModelPosition());
			}
			break;

		case SVIMessageHandler::SM_SET_MODEL_ROTATION:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setModelRotation(getModelRotation());
			}
			break;

		case SVIMessageHandler::SM_SET_MODEL_SCALE:
			{
				if( duration > 0 )
					addImplicitAnimation((SVIPropertyAnimation::PropertyAnimationType) msg.mParam2);
				else
					mBeamedSlide->setModelScale(getModelScale());
			}
			break;

		case SVIMessageHandler::SM_ADD_PARTICLE_EFFECT:
			{
				SVIParticleEffect* effect = mParticleEffectList.front();
				mBeamedSlide->addParticleEffect(effect);
				mParticleEffectList.pop_front();
			}
            break;

        //2013-06-21 masterkeaton27@gmail.com
        case SVIMessageHandler::SM_ADD_PATH_POINT:
            {
                SVIVector3 * pPoint = (SVIVector3 *)msg.mParam1;
                mBeamedSlide->addPathPoint(*pPoint);
                SVI_SVIFE_DELETE(pPoint);
            }
            break;

        //2013-06-21 masterkeaton27@gmail.com
        case SVIMessageHandler::SM_CLEAR_PATH_POINTS:
            {
                mBeamedSlide->clearPathPoints();
            }
            break;

        case SVIMessageHandler::SM_SET_TESSELATION:
            {
                mBeamedSlide->setTesselationPointList(mTesselationPointList);
            }
            break;

        case SVIMessageHandler::SM_SET_MORPHING_TYPE:
            {
                mBeamedSlide->setMorphingType(getMorphingType());
            }
            break;

        case SVIMessageHandler::SM_SET_MORPHING_RADIUS:
            {
                mBeamedSlide->setMorphingRadius(getMorphingRadius());
            }
            break;

        case SVIMessageHandler::SM_SET_MORPHING_GRID_COUNT:
            {
                mBeamedSlide->setMorphingVertexCount(getMorphingVertexCount());
            }
            break;

        case SVIMessageHandler::SM_SET_MORPHING_COLOR:
            {
                mBeamedSlide->setMorphingColor(getMorphingColor());
            }
            break;
        }

		mBeamedSlide->getProjectionSlide()->resetInvalidate();
	}

};
