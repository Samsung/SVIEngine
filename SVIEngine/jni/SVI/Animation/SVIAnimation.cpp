#include "SVIAnimation.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../Slide/SVIRenderPartManager.h"
#include "./Interpolator/SVILinearInterpolator.h"
#include "./Interpolator/SVIAccelerateIterpolator.h"
#include "./Interpolator/SVIDecelerateInterpolator.h"
#include "./Interpolator/SVIAccelerateDecelerateInterpolator.h"
#include "./Interpolator/SVIAnticipateInterpolator.h"
#include "./Interpolator/SVIAnticipateOvershootInterpolator.h"
#include "./Interpolator/SVIBounceInterpolator.h"
#include "./Interpolator/SVIOvershootInterpolator.h"
#include "./Interpolator/SVICycleInterpolator.h"
#include "./Interpolator/SVIBackEaseInInterpolator.h"
#include "./Interpolator/SVIBackEaseInOutInterpolator.h"
#include "./Interpolator/SVIBackEaseOutInterpolator.h"
#include "./Interpolator/SVIBounceEaseInInterpolator.h"
#include "./Interpolator/SVIBounceEaseInOutInterpolator.h"
#include "./Interpolator/SVIBounceEaseOutInterpolator.h"
#include "./Interpolator/SVICircularEaseInInterpolator.h"
#include "./Interpolator/SVICircularEaseInOutInterpolator.h"
#include "./Interpolator/SVICircularEaseOutInterpolator.h"
#include "./Interpolator/SVICubicEaseInInterpolator.h"
#include "./Interpolator/SVICubicEaseInOutInterpolator.h"
#include "./Interpolator/SVICubicEaseOutInterpolator.h"
#include "./Interpolator/SVIElasticEaseInInterpolator.h"
#include "./Interpolator/SVIElasticEaseInOutInterpolator.h"
#include "./Interpolator/SVIElasticEaseOutInterpolator.h"
#include "./Interpolator/SVIExponentialEaseInInterpolator.h"
#include "./Interpolator/SVIExponentialEaseInOutInterpolator.h"
#include "./Interpolator/SVIExponentialEaseOutInterpolator.h"
#include "./Interpolator/SVIQuadEaseInInterpolator.h"
#include "./Interpolator/SVIQuadEaseInOutInterpolator.h"
#include "./Interpolator/SVIQuadEaseOutInterpolator.h"
#include "./Interpolator/SVIQuartEaseInInterpolator.h"
#include "./Interpolator/SVIQuartEaseInOutInterpolator.h"
#include "./Interpolator/SVIQuartEaseOutInterpolator.h"
#include "./Interpolator/SVIQuintEaseInInterpolator.h"
#include "./Interpolator/SVIQuintEaseInOutInterpolator.h"
#include "./Interpolator/SVIQuintEaseOutInterpolator.h"
#include "./Interpolator/SVISineEaseInInterpolator.h"
#include "./Interpolator/SVISineEaseInOutInterpolator.h"
#include "./Interpolator/SVISineEaseOutInterpolator.h"
#include "./Interpolator/SVIStrongEaseInInterpolator.h"
#include "./Interpolator/SVIStrongEaseInOutInterpolator.h"
#include "./Interpolator/SVIStrongEaseOutInterpolator.h"



namespace SVI {

	static const SVIBool DEBUG = SVIFALSE;

	SVIAnimation::SVIAnimation(SVIGLSurface* saGLSurface) {
		mSVIGLSurface = saGLSurface;
		
		mTimeRatio = -1.0f;
		mDuration = 0;
		mRepeatCount = 1;
		mOffset = 0;
		mAutoReverse = SVIFALSE;
		mSlide = NULL;

		mInterpolatorType = LINEAR;
		mInterpolator = NULL;
		mListener = NULL;

		mStatus = IDLE;
		mCurrentCount = 0;
		mClassType = NONE;
		mID = 0;

		mIsContainedInAniSet = SVIFALSE;
	}

	SVIAnimation::~SVIAnimation() {
		if(mSlide != NULL){			
			mSlide->removeAnimation();
			mSlide = NULL;
		}
		
		mSVIGLSurface->getRenderPartManager()->removeAnimationFromAnimationList(this);
		
		SVI_SVIFE_DELETE(mInterpolator);
		mSVIGLSurface = NULL;
	}

	void SVIAnimation::setStatus(AnimationStatus newStatus) {
		DEBUG_CHECK_LOGI("set animation state %d => %d", mStatus, newStatus);
		mStatus = newStatus;
	}

	SVIBool SVIAnimation::isEnd(SVIUInt& time) {
		if( mInterpolator == NULL )
			return SVITRUE;
		return mInterpolator->isEnd(time);
	}

	void SVIAnimation::copy(const SVIAnimation* rhs) {
		mTimeRatio = rhs->mTimeRatio;
		mDuration = rhs->mDuration;
		mRepeatCount = rhs->mRepeatCount;
		mOffset = rhs->mOffset;
		mAutoReverse = rhs->mAutoReverse;
		mID = rhs->mID;

		mInterpolatorType = rhs->mInterpolatorType;
		if( rhs->mInterpolator != NULL ) {
			SVI_SVIFE_DELETE(mInterpolator);
			mInterpolator = rhs->mInterpolator->clone();
		}
		mListener = rhs->mListener;

		mStatus = rhs->mStatus;
		mCurrentCount = rhs->mCurrentCount;

		mSVIGLSurface = rhs->mSVIGLSurface;
	}

	SVIInterpolator* SVIAnimation::createInterpolator(InterpolatorType type) {
		SVIInterpolator* interpolator = NULL;
		switch(mInterpolatorType) {
		case ACCELERATE:
			interpolator = new SVIAccelerateInterpolator();
			break;
		case DECELERATE:
			interpolator = new SVIDecelerateInterpolator();
			break;
		case ACCELERATE_DECELERATE:
			interpolator = new SVIAccelerateDecelerateInterpolator();
			break;
		case ANTICIPATE:
			interpolator = new SVIAnticipateInterpolator();
			break;
		case ANTICIPATE_OVERSHOOT:
			interpolator = new SVIAnticipateOvershootInterpolator();
			break;
		case BOUNCE:
			interpolator = new SVIBounceInterpolator();
			break;
		case OVERSHOOT:
			interpolator = new SVIOvershootInterpolator();
			break;
		case CYCLE:
			interpolator = new SVICycleInterpolator();
			break;
		case BACKEASEIN:
			interpolator = new SVIBackEaseInInterpolator();
			break;
		case BACKEASEINOUT:
			interpolator = new SVIBackEaseInOutInterpolator();
			break;
		case BACKEASEOUT:
			interpolator = new SVIBackEaseOutInterpolator();
			break;
		case BOUNCEEASEIN:
			interpolator = new SVIBounceEaseInInterpolator();
			break;
		case BOUNCEEASEINOUT:
			interpolator = new SVIBounceEaseInOutInterpolator();
			break;
		case BOUNCEEASEOUT:
			interpolator = new SVIBounceEaseOutInterpolator();
			break;
		case CIRCULAREASEIN:
			interpolator = new SVICircularEaseInInterpolator();
			break;
		case CIRCULAREASEINOUT:
			interpolator = new SVICircularEaseInOutInterpolator();
			break;
		case CIRCULAREASEOUT:
			interpolator = new SVICircularEaseOutInterpolator();
			break;
		case CUBICEASEIN:
			interpolator = new SVICubicEaseInInterpolator();
			break;
		case CUBICEASEINOUT:
			interpolator = new SVICubicEaseInOutInterpolator();
			break;
		case CUBICEASEOUT:
			interpolator = new SVICubicEaseOutInterpolator();
			break;
		case ELASTICEASEIN:
			interpolator = new SVIElasticEaseInInterpolator();
			break;
		case ELASTICEASEINOUT:
			interpolator = new SVIElasticEaseInOutInterpolator();
			break;
		case ELASTICEASEOUT:
			interpolator = new SVIElasticEaseOutInterpolator();
			break;
		case EXPONENTIALEASEIN:
			interpolator = new SVIExponentialEaseInInterpolator();
			break;
		case EXPONENTIALEASEINOUT:
			interpolator = new SVIExponentialEaseInOutInterpolator();
			break;
		case EXPONENTIALEASEOUT:
			interpolator = new SVIExponentialEaseOutInterpolator();
			break;
		case QUADEASEIN:
			interpolator = new SVIQuadEaseInInterpolator();
			break;
		case QUADEASEINOUT:
			interpolator = new SVIQuadEaseInOutInterpolator();
			break;
		case QUADEASEOUT:
			interpolator = new SVIQuadEaseOutInterpolator();
			break;
		case QUARTEASEIN:
			interpolator = new SVIQuartEaseInInterpolator();
			break;
		case QUARTEASEINOUT:
			interpolator = new SVIQuartEaseInOutInterpolator();
			break;
		case QUARTEASEOUT:
			interpolator = new SVIQuartEaseOutInterpolator();
			break;
		case QUINTEASEIN:
			interpolator = new SVIQuintEaseInInterpolator();
			break;
		case QUINTEASEINOUT:
			interpolator = new SVIQuintEaseInOutInterpolator();
			break;
		case QUINTEASEOUT:
			interpolator = new SVIQuintEaseOutInterpolator();
			break;
		case SINEEASEIN:
			interpolator = new SVISineEaseInInterpolator();
			break;
		case SINEEASEINOUT:
			interpolator = new SVISineEaseInOutInterpolator();
			break;
		case SINEEASEOUT:
			interpolator = new SVISineEaseOutInterpolator();
			break;
		case STRONGEASEIN:
			interpolator = new SVIStrongEaseInInterpolator();
			break;
		case STRONGEASEINOUT:
			interpolator = new SVIStrongEaseInOutInterpolator();
			break;
		case STRONGEASEOUT:
			interpolator = new SVIStrongEaseOutInterpolator();
			break;
		case LINEAR:
		default:
			interpolator = new SVILinearInterpolator();
			break;
		}

		return interpolator;
	}


	void SVIAnimation::startAnimation(SVIUInt time) { 
		setStatus(START);
		if( mInterpolator != NULL) {
			SVI_SVIFE_DELETE(mInterpolator);
		}

		mInterpolator = createInterpolator(mInterpolatorType);
		
		if( mInterpolator == NULL ) {
			LOGE("Interpolator allocation failed!!!");
			if( mSlide != NULL )
				mSlide->removeAnimation();
			return;
		}

		if( mInterpolator != NULL ) {
			mInterpolator->setRepeatCount(mRepeatCount);
			mInterpolator->setAutoReverse(mAutoReverse);
			mInterpolator->setStartTime(time, mDuration, mOffset);
		}

		setStatus(ANIMATING);
	}

	void SVIAnimation::endAnimation(SVIUInt time) {
		if( mStatus == END ) {
			if( mListener != NULL )
				mListener->onAnimationEnd(this);
		}
	}

	void SVIAnimation::setInterpolator(InterpolatorType type) {
		mInterpolatorType = type;
	}

	void SVIAnimation::updateInfo(SVIUInt time) {
		if( getStatus() == IDLE ) {
			startAnimation(time);

			if( mListener != NULL )
				mListener->onAnimationStart(this);
		}

		if( getStatus() == ANIMATING ) {
			if( mInterpolator != NULL ) {
				mTimeRatio = mInterpolator->getTimeInterpolation(time);
			}

			if( mListener != NULL && mInterpolator != NULL ) {
				SVIUInt currentCount = mInterpolator->getCurrentCount();
				// currentCount is elapsedTime / duration. So current means current animation index.
				// and RepeatCount means total count of playing animation.
				// Default RepeatCount is 1, because animation must play once.
				if( mRepeatCount > 1 && mCurrentCount != currentCount && mRepeatCount != currentCount ) {
					//LOGE("Current repeat count %d, Interpolator repeatCount %d", mCurrentCount, currentCount);
					mListener->onAnimationRepeat(this);
				}
				mCurrentCount = currentCount;
			}
		}

		DEBUG_CHECK_LOGI("SVIAnimation::updateInfo mTimeRatio : %f", mTimeRatio);
	}
	void SVIAnimation::cancel() {
		if( mSlide != NULL ) {
			mSlide->removeAnimation();
			mSlide = NULL;
			setStatus(END);
		}
	}

	void SVIAnimation::forceEnd() {
		if( mInterpolator != NULL )
			mInterpolator->forceEnd();
		
		if( mSlide != NULL ) {
			mSlide->removeAnimation();
			mSlide = NULL;
			setStatus(END);
		}
	}

	SVIGLSurface* SVIAnimation::getSVIGLSurface(){
		return mSVIGLSurface;
	}



}
