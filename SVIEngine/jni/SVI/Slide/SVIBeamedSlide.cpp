#include "SVIBeamedSlide.h"
#include "SVIProjectionSlide.h"
#include "SVIRenderPartManager.h"
#include "../Render/SVITexture.h"
#include "../Animation/Transition/SVITransitionEffector.h"
#include "../Animation/SVIBasicAnimation.h"

namespace SVI {
	static const SVIBool	DEBUG = SVIFALSE;

	SVIBeamedSlide::SVIBeamedSlide(SVIGLSurface *surface) :
		SVIBaseSlide(surface){
		mSlide = NULL;
		mProjectionSlide = NULL;
		mSlideType = BASE_SLIDE;
		mAnimation = NULL;
	}

	SVIBeamedSlide::~SVIBeamedSlide() {
		if( mAnimation != NULL ) {
			mAnimation->setBeamedSlide(NULL);
			removeAnimation();
		}
		if( getParent() != NULL )
			removeFromSuperSlide();

		if( mSubSlideList.size() > 0 ) {
			BaseSlideListIter iter = mSubSlideList.begin();
			SVIBeamedSlide* slide = NULL;
			for(iter; iter != mSubSlideList.end(); iter++) {
				slide = (SVIBeamedSlide*)(*iter);
				slide->setParent(NULL);
			}
			mSubSlideList.clear();
		}

		SVI_SVIFE_DELETE(mProjectionSlide);
	}
		
	SVIBool SVIBeamedSlide::initialize(SVIBeamedSlide* parent, SVISlide* slide, SVIImage* image, SlideType type) {
		if( slide == NULL ) {
			return SVIFALSE;
		}

		SVIBool	ret = SVIFALSE;

		if( mProjectionSlide == NULL ) {
			SVIBaseSlide* baseSlide = (SVIBaseSlide*)slide;
			copyBaseProperties(*baseSlide);

			mSlide = slide;

			SVIProjectionSlide* projectionSlideParent = NULL;
			if( parent != NULL ) {
				parent->addSubSlide(this);
				projectionSlideParent = parent->mProjectionSlide;
			}			

			mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);
			if( mProjectionSlide != NULL ) {			
				ret = mProjectionSlide->initialize(projectionSlideParent, this, image, type);
			}
		}

		if( !ret )
			DEBUG_CHECK_LOGE("SVIBeamedSlide::initialize() failed");

		return ret;
	}


	SVIBeamedSlide* SVIBeamedSlide::deepCopy(SVIBool withSubSlide) {
		SVIBeamedSlide* copySlide = new SVIBeamedSlide(mSVIGLSurface);

		// copy self
		SVIBaseSlide* baseSlide = (SVIBaseSlide*)this;
		copySlide->copyBaseProperties(*baseSlide);

		copySlide->mSlide = mSlide;
		copySlide->mSlideType = mSlideType;

		SVIBeamedSlide* parent = (SVIBeamedSlide *)(mSuperSlide);
		SVIProjectionSlide* projectionSlideParent = NULL;
		if( parent != NULL ) {
			parent->addSubSlide(copySlide);
			projectionSlideParent = parent->mProjectionSlide;
		}	

		copySlide->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);
		if(copySlide->mProjectionSlide != NULL) {
			if( mProjectionSlide->getTextureContainer()->getTexture() ) {
				copySlide->mProjectionSlide->initialize(projectionSlideParent, copySlide, 
					mProjectionSlide->getTextureContainer()->getTexture()->getImage(), copySlide->mSlideType);
			}
			else {
				copySlide->mProjectionSlide->initialize(projectionSlideParent, copySlide, NULL, copySlide->mSlideType);
			}

			//2011-11-18 masterkeaton27@gmail.com
			if (mProjectionSlide->getTextureContainer()->getBackFaceTexture()){
				copySlide->setBackFaceImage(mProjectionSlide->getTextureContainer()->getBackFaceTexture()->getImage());				
			}
		}
		
		copySlide->mSubSlideList.clear();

		if( withSubSlide )
			copySubSlideList(copySlide);

		return copySlide;
	}


	void SVIBeamedSlide::copySubSlideList(SVIBeamedSlide* parent) {
		// copy sub slide
		BaseSlideList newSlideList;
		BaseSlideList::iterator iter = mSubSlideList.begin();
		for(iter; iter != mSubSlideList.end(); ++iter) {
			SVIBeamedSlide* oldSubSlide = (SVIBeamedSlide *)*iter;
			if(oldSubSlide->getProjectionSlide() != NULL) {
				SVIBeamedSlide* newSubSlide = new SVIBeamedSlide(mSVIGLSurface);
				if( oldSubSlide->getProjectionSlide()->getTextureContainer()->getTexture() ) {
					newSubSlide->initialize(parent, (SVISlide *)oldSubSlide, 
						oldSubSlide->getProjectionSlide()->getTextureContainer()->getTexture()->getImage(), (SVIBeamedSlide::SlideType)oldSubSlide->getSlideType());
				}
				else {
					newSubSlide->initialize(parent, (SVISlide *)oldSubSlide, NULL, (SVIBeamedSlide::SlideType)oldSubSlide->getSlideType());
				}

				//2011-11-18 masterkeaton27@gmail.com
				if (oldSubSlide->getProjectionSlide()->getTextureContainer()->getBackFaceTexture()){
					newSubSlide->setBackFaceImage(oldSubSlide->getProjectionSlide()->getTextureContainer()->getBackFaceTexture()->getImage());
				}

				newSubSlide->mSubSlideList.clear();
				oldSubSlide->copySubSlideList(newSubSlide);
				newSlideList.push_back(newSubSlide);
			}
		}
		
		parent->mSubSlideList = newSlideList;
	}


	void SVIBeamedSlide::removeTransitionBeamedSlide() {
		removeSubSlideList(this);
	}


	void SVIBeamedSlide::removeSubSlideList(SVIBeamedSlide* slide) {
		// remove sub slide
		BaseSlideList::iterator iter = slide->mSubSlideList.begin();
		SVIBeamedSlide* subSlide = NULL;
		for(; !slide->mSubSlideList.empty(); ) {
			if(iter != slide->mSubSlideList.end()){
			subSlide = (SVIBeamedSlide *)*iter;
			subSlide->removeSubSlideList(subSlide);
			iter = slide->mSubSlideList.erase(iter);
			SVI_SVIFE_DELETE(subSlide);
				}
		}
		slide->mSubSlideList.clear();
	}


    //2012-12-10 masterkeaton27@gmail.comnormal & specular image 
    void SVIBeamedSlide::setNormalImage(SVIImage* image){
       
        if (mProjectionSlide == NULL ){
            return;
        }

        mProjectionSlide->setNormalImage(image);
    }

    void SVIBeamedSlide::setSpecularImage(SVIImage* image){
      
        if (mProjectionSlide == NULL ){
            return;
        }
       
        mProjectionSlide->setSpecularImage(image);
    }

	void SVIBeamedSlide::setImage(SVIImage* image) {
		//if( image == NULL || mProjectionSlide == NULL )
		//	return;
		//2011-08-05 masterkeaton27@gmail.com
		//set image to null for unload.
		if (mProjectionSlide == NULL ){
			return;
		}

		if (image != NULL){			
			mTextureSpriteRegion = SVIRect(0.0f,0.0f,(SVIFloat)image->mWidth,(SVIFloat)image->mHeight);
			mTextureScaleFitRegion = SVIRect(0.0f,0.0f,getRegion().getSize().mWidth,getRegion().getSize().mHeight);
		} else {
			mTextureSpriteRegion = SVIRect(0.0f,0.0f,1.0f,1.0f);
			mTextureScaleFitRegion = SVIRect(0.0f,0.0f,1.0f,1.0f);
		}
		mProjectionSlide->setImage(image);
	}

	void SVIBeamedSlide::setBackFaceImage(SVIImage* image){
		if (mProjectionSlide == NULL ){
			return;
		}		

		if (image != NULL){			
			mTextureBackFaceSpriteRegion = SVIRect(0.0f,0.0f,(SVIFloat)image->mWidth,(SVIFloat)image->mHeight);
		}else {
			mTextureBackFaceSpriteRegion = SVIRect(0.0f,0.0f,1.0f,1.0f);
		}

		mProjectionSlide->setBackFaceImage(image);
	}

	void SVIBeamedSlide::setBackFaceImageFlip(SVIBool isFlip) {
		if (mProjectionSlide == NULL ){
			return;
		}		

		mProjectionSlide->setBackFaceImageFlip(isFlip);
	}

	void SVIBeamedSlide::setBackFaceImageClear(SVIBool isClear) {
		if (mProjectionSlide == NULL ){
			return;
		}		
		mProjectionSlide->setBackFaceImageClear(isClear);
	}

	SVIInt SVIBeamedSlide::getSlideType() {
		return mProjectionSlide->getSlideType();
	}
	
	void SVIBeamedSlide::addChangePropery(PropertyChangeType type) {

#if 1
		if( mChangePropertyList.empty() ) 
			mSVIGLSurface->getRenderPartManager()->addUpdatedSlide(this);//Multi-Instance-Support
#else
		if( mChangePropertyList.empty() ) {
			mRequestedInvalidation = SVITRUE;
			if( !isNeedInvalidate() )
				mRequestedInvalidation = SVIFALSE;

			if( mRequestedInvalidation )
				mSVIGLSurface->getRenderPartManager()->addUpdatedSlide(this);//Multi-Instance-Support
		}
#endif
		mChangePropertyList.push_back((SVIInt)type);
		
	}

	void SVIBeamedSlide::clearChangePropertyList() {
		mChangePropertyList.clear();
	}

	SVIInt SVIBeamedSlide::getChangePropertyListSize() {
		return mChangePropertyList.size();
	}

	ChangePropertyList&	SVIBeamedSlide::getChangePropertyList() {
		return mChangePropertyList;
	}

	SVIBool SVIBeamedSlide::addAnimation(SVIAnimation* animation) {

		if( animation == NULL )
			return SVIFALSE;

		animation->setBeamedSlide(this);

		if( mAnimation != NULL ) {
			mSVIGLSurface->getRenderPartManager()->removeAnimation(mAnimation);//Multi-Instance-Support
			mAnimation->setBeamedSlide(NULL);
			mAnimation = NULL;
		}

		mAnimation = animation;
		mSVIGLSurface->getRenderPartManager()->addAnimation(mAnimation);//Multi-Instance-Support

		return SVITRUE;
	}

	void SVIBeamedSlide::removeAnimation() {
		if( mAnimation != NULL ) {
			mSVIGLSurface->getRenderPartManager()->removeAnimation(mAnimation);//Multi-Instance-Support
			mAnimation->setBeamedSlide(NULL);
			mAnimation = NULL;
		}
	}

	void SVIBeamedSlide::stopAnimation() {
		if( mAnimation != NULL ) {
			mAnimation->setBeamedSlide(NULL);
			mAnimation->forceEnd();
		}
	}

	void SVIBeamedSlide::setRegion(const SVIRect& region) {
		SVIBaseSlide::setRegion(region);
		updateGeometryByRegion();
	}

	SVIBool SVIBeamedSlide::addSubSlide(SVIBeamedSlide* slide) {
		if( slide == NULL ) {
			LOGE("SVIBeamedSlide::addSubSlide is NULL!!!");
			return SVIFALSE;
		}

		if( !SVIBaseSlide::addSubSlide(slide) ) {
			LOGE("SVIBeamedSlide::addSubSlide failed to add beamed slide");
			return SVIFALSE;
		}

		if( mProjectionSlide != NULL && slide->getProjectionSlide() != NULL )
			mProjectionSlide->addSubSlide(slide->getProjectionSlide());

		return SVITRUE;
	}

	SVIBool SVIBeamedSlide::removeSubSlide(SVIBeamedSlide* slide) {
		if( slide == NULL )
			return SVIFALSE;

		SVIProjectionSlide* projectionSlide = slide->getProjectionSlide();
		if( !mProjectionSlide->removeSubSlide(projectionSlide) ) {
			LOGE("SVIBeamedSlide::removeSubSlide failed to remove projection slide!!!");
			return SVIFALSE;
		}

		if( !SVIBaseSlide::removeSubSlide(slide) )
			return SVIFALSE;

		return SVITRUE;
	}

	SVIBool SVIBeamedSlide::insertSubSlide(SVIInt index, SVIBeamedSlide* newSlide) {
		if( newSlide == NULL ) {//|| newSlide->getParent() != NULL ) {
			//LOGI("SVIBeamedSlide::insertSubSlide newSlide is NULL or parent is not NULL %x", newSlide->getParent());
			LOGI("SVIBeamedSlide::insertSubSlide newSlide is NULL !!");
			return SVIFALSE;
		}

		if( !SVIBaseSlide::insertSubSlide(index, newSlide) ) {
			LOGE("SVIBaseSlide::insertSubSlide failed!!!");
			return SVIFALSE;
		}

		if( !mProjectionSlide->insertSubSlide(index, newSlide->getProjectionSlide()) ){
			LOGE("ProjectionSlide::insertSubSlide failed!!!");
			return SVIFALSE;
		}

		return SVITRUE;
	}

	SVIBool SVIBeamedSlide::replaceSubSlide(SVIBeamedSlide* oldSlide, SVIBeamedSlide* newSlide) {

		if( oldSlide == NULL || oldSlide->getParent() == NULL || 
			newSlide == NULL || newSlide->getParent() != NULL ) {
			DEBUG_CHECK_LOGE("SVIBeamedSlide::replaceSubSlide : old or new is NULL");
			return SVIFALSE;
		}


		if( !mProjectionSlide->replaceSubSlide(oldSlide->getProjectionSlide(), newSlide->getProjectionSlide()) ) {
			DEBUG_CHECK_LOGE("SVIBeamedSlide::replaceSubSlide : failed to replace projection slide!!!");
			return SVIFALSE;
		}

		if( !SVIBaseSlide::replaceSubSlide(oldSlide, newSlide) ) {
			DEBUG_CHECK_LOGE("SVIBeamedSlide::replaceSubSlide : failed to replace beamed slide!!!");
			return SVIFALSE;
		}

		return SVITRUE;
	}

	void SVIBeamedSlide::resetAllProperties() {
		if( mSlide != NULL ) {
			SVIBaseSlide* baseSlide = (SVIBaseSlide*)mSlide;
			copyBaseProperties(*baseSlide);
			addChangePropery(RESET_ALL);
		}
	}

	SVIBool SVIBeamedSlide::isAnimating() {
		return mAnimation != NULL ? SVITRUE : SVIFALSE;
	}

	void SVIBeamedSlide::deformRatioTouchDown(float x, float y) {
		//2012.03.22 marx.kim
		//to do : this function will call function of projection slide to set deform touch down information.
		//LOGI("SVIBeamedSlide::deformRatioTouchDown x:%f, y:%f", x, y);
		if( mProjectionSlide != NULL )
			 mProjectionSlide->setHoldDeformDown(x,y);
	}
    
    //2013-06-21 masterkeaton27@gmail.com
    void SVIBeamedSlide::addPathPoint(const SVIVector3& point){
        if( mProjectionSlide != NULL )
            mProjectionSlide->addPathPoint(point);
		mSVIGLSurface->getRenderPartManager()->addUpdatedSlide(this);
	}

    void SVIBeamedSlide::clearPathPoints(){
        if( mProjectionSlide != NULL )
            mProjectionSlide->clearPathPoints();
		mSVIGLSurface->getRenderPartManager()->addUpdatedSlide(this);
    }

};