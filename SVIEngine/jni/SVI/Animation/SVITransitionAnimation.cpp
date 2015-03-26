#include "SVITransitionAnimation.h"
#include "./Transition/SVITransitionEffector.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Slide/SVISlide.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../Slide/SVIProjectionSlide.h"
#include "../Slide/SVIRenderPartManager.h"
#include "../Render/SVITexture.h"
#include "../SVICores.h"
#include "../Render/SVIRenderer.h"
#include "../Render/SVIFrameRenderer.h"
#include "../Render/SVIRenderScene.h"
#include "../Render/SVITextureManager.h"

#include "SVIBasicAnimation.h"
#include "SVIKeyFrameAnimation.h"

namespace SVI {

	static const SVIBool DEBUG = SVIFALSE;

	SVITransitionAnimation::SVITransitionAnimation(SVIGLSurface* saGLSurface):SVIAnimation(saGLSurface),
	mFromSlide(NULL), 
	mToSlide(NULL),
	mEffector(NULL),
	mIsTransition(false),
	mDirectionType(LEFT){
		mClassType = TRANSITION;
	}



	SVITransitionAnimation::~SVITransitionAnimation() {
		DEBUG_CHECK_LOGE("SVITransition remove : %x,  mFrom %x , mTo %x", this, mFromSlide, mToSlide);

		stopCaptureSlide();

		if(mEffector!=NULL){
			mEffector->cancelAnimation();
			mEffector->removeNslide();
			SVI_SVIFE_DELETE(mEffector);
		}

	}


	



	SVITransitionAnimation* SVITransitionAnimation::createAnimation(TransitionType type, SVIGLSurface* saGLSurface) {
		SVITransitionAnimation* transAni = new SVITransitionAnimation(saGLSurface);
		
		transAni->mTransitionType = type;
		return transAni;
	}



	void SVITransitionAnimation::stopCaptureSlide(){

		if(mToSlide != NULL){
			// PLM P130802-8000 mToSlide->mSVIGLSurface is coming NULL some time so getting from mFromSlide surface.
			mSVIGLSurface->getSlideCapture()->stop(mToSlide->getProjectionSlide());//Multi-Instance-Support
		}

		if(mFromSlide != NULL) {
			mSVIGLSurface->getSlideCapture()->stop(mFromSlide->getProjectionSlide());//Multi-Instance-Support
			mFromSlide->removeAnimation();
			mFromSlide->removeTransitionBeamedSlide();
			mFromSlide->removeFromSuperSlide();
			SVI_SVIFE_DELETE(mFromSlide);
		}
	}

	
	void SVITransitionAnimation::setBeamedSlide(SVIBeamedSlide* slide) {
		DEBUG_CHECK_LOGE("%s(%x) : into setBeamedSlide method", "SVITransitionAnimation", this);
		if( mFromSlide == NULL ) {
			LOGE("mFromSlide pointer is null!!! SVITransitionAnimation::setBeamedSlide()");
			return;
		}
		mSlide = slide;
		mToSlide = mSlide;

		//slide instance can be null, when slide that has animation is deleted.
		if( mSlide == NULL )
			return;
		
		mEffector = SVITransitionEffector::createEffector(mSVIGLSurface, mTransitionType);

		SVIProjectionSlide* toProjectionSlide = mToSlide->getProjectionSlide();


		toProjectionSlide->invalidate(SVITRUE);

		mSVIGLSurface->getTextureManager()->update();//Multi-Instance-Support

		mEffector->initialize(this, mToSlide, (SVIBeamedSlide*)mToSlide->getParent(), mFromSlide, mToSlide, mDuration, mOffset, mRepeatCount, mTransitionType);

		mSVIGLSurface->getSlideCapture()->stop(mToSlide->getProjectionSlide());//Multi-Instance-Support
		mSVIGLSurface->getSlideCapture()->stop(mFromSlide->getProjectionSlide());//Multi-Instance-Support

		mToSlide->setHidden(SVITRUE);
		mFromSlide->setHidden(SVITRUE);

		mToSlide->getProjectionSlide()->invalidate(SVITRUE);
		mFromSlide->getProjectionSlide()->invalidate(SVITRUE);
		
		mIsTransition = true;
	}


	void SVITransitionAnimation::update(SVIUInt& time) {
		updateInfo(time);

		if( getStatus() == ANIMATING ) {
			if( !mIsTransition ) {
				LOGE("transition didn't occur!!!");
				if(mToSlide != NULL) {
					mToSlide->removeAnimation();
				}
				return;
			}
			else {
				if( SVIAnimation::isEnd(time) ) {
					processEnd();
				}
			}
		}
	}

	//2012-04-09 masterkeaton27@gmail.com
	//to avoid illusion of previous transition scene.
	void SVITransitionAnimation::processEnd(){
		setStatus(END);

		if(mEffector!= NULL)
			mEffector->updateToSlideInfo(mToSlide);

		mIsTransition = false;

		if( !mIsContainedInAniSet && mToSlide != NULL ) {
			mToSlide->removeAnimation();
		}

		if(mToSlide != NULL){
			mToSlide->setHidden(SVIFALSE);	
		}
	}

	void SVITransitionAnimation::copy(const SVITransitionAnimation* rhs) {
		SVIAnimation::copy(rhs);
		mTransitionType = rhs->mTransitionType;

		mPrevWindowSize = rhs->mPrevWindowSize;
		mCurWindowSize = rhs->mCurWindowSize;
		mDirectionType = rhs->mDirectionType;
	}


	SVIAnimation* SVITransitionAnimation::clone() {

		SVITransitionAnimation* cloneAnimation = new SVITransitionAnimation(mSVIGLSurface);
		cloneAnimation->copy(this);

		return (SVIAnimation*)cloneAnimation;
	}

	void SVITransitionAnimation::print() {
		LOGI("AniClassType : TransitionAnimation");
	}

	void SVITransitionAnimation::forceEnd(){

		if( mInterpolator != NULL )
			mInterpolator->forceEnd();
		
		//2012-04-09 masterkeaton27@gmail.com
		//to avoid illusion of previous transition scene.
		processEnd();				
		
		//stopCaptureSlide();

		//if(mEffector!=NULL){			
		//	mEffector->cancelAnimation();
		//	mEffector->removeNslide();			
		//}
	}


	void SVITransitionAnimation::prepareTransitionAnimation(SVIBeamedSlide* slide) {
		SVIAnimation* oldAnimation = slide->getAnimation();
		if( oldAnimation != NULL ) {
			// 2011/09/16 rareboy0112@facebook.com
			// When stopped in the middle of a transition animation processing tasks.
			if(oldAnimation->getClassType() == SVIAnimation::TRANSITION) {
				((SVITransitionAnimation *)oldAnimation)->getEffector()->updateToSlideInfo(slide);
			}
		}
		// make From Slide to display before status of current slide using copy slide tree.
		mFromSlide = slide->deepCopy(true);
		mFromSlide->getProjectionSlide()->invalidate(SVITRUE);
		(mSVIGLSurface->getRenderer())->getScene()->renderSpecifyTraversal(mFromSlide->getProjectionSlide());
	}

} // end namespace SVI
