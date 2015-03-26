#include "SVIAnimationSet.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../SVICores.h"

#include "SVIBasicAnimation.h"
#include "SVIKeyFrameAnimation.h"

namespace SVI {
	static const SVIBool DEBUG = SVIFALSE;

	SVIAnimationSet::SVIAnimationSet(SVIGLSurface * saGLSurface):SVIAnimation(saGLSurface) {
		mClassType = ANIMATION_SET;
		mShareAnimationInfo = SVIFALSE;
		mCurrentRepeatCount = 0;
	}

	SVIAnimationSet::~SVIAnimationSet() {
		AnimationSet::iterator iter;
		for(iter=mAnimationList.begin(); iter!=mAnimationList.end(); iter++) 
			SVI_SVIFE_DELETE(*iter);
		mAnimationList.clear();
	}

	void SVIAnimationSet::setBeamedSlide(SVIBeamedSlide* slide) {
		AnimationSet::iterator iter;
		mSlide = slide;
		for(iter=mAnimationList.begin(); iter!=mAnimationList.end(); iter++)
			(*iter)->setBeamedSlide(slide);

	}

	SVIBool SVIAnimationSet::isEnd(SVIUInt& time) {
		return mAnimationList.empty() || SVIAnimation::isEnd(time);
	}

	void SVIAnimationSet::update(SVIUInt& time) {
		if( getStatus() == IDLE ) 
			shareAnimationInfo();

		updateInfo(time);
		
		
		if( mAnimationList.empty() )
			LOGE("Animation is empty... status is %d", getStatus());
		if( getStatus() == ANIMATING ) {
		
			DEBUG_CHECK_LOGE("SVIAnimationSet::update() animation count:%d", mAnimationList.size());
			
			for(SVIUInt i=0; i < mAnimationList.size(); ++i) {
				mAnimationList[i]->update(time);
			}

			SVIUInt currentRepeatCount = mInterpolator->getCurrentCount();

			//phyuzion@gmail.com 2013-06-24
			//for Animation Set handle
			if(mRepeatCount > 1 && currentRepeatCount != mCurrentRepeatCount && mRepeatCount != currentRepeatCount){
				for(SVIUInt i=0; i < mAnimationList.size(); ++i) {
					mAnimationList[i]->startAnimation(time);
				}
			}
			mCurrentRepeatCount = currentRepeatCount;
	
			if( isEnd(time) ) {
				setStatus(END);
				if( !mIsContainedInAniSet && mSlide != NULL ) {
					mSlide->removeAnimation();
				}
			}
		}	
	}

	void SVIAnimationSet::calculateAndApply(const SVIFloat& ratio) {
		// nothing to do...
	}

	SVIBool SVIAnimationSet::addAnimation(SVIAnimation* animation) {
		if( isExist(animation) )
			return SVIFALSE;

		animation->setIsContainedInAniSet(SVITRUE);
		mAnimationList.push_back(animation);

		return SVITRUE;
	}

	void SVIAnimationSet::removeAnimation(SVIAnimation* animation) {
		LOGE("test is Erase");

		AnimationSet::iterator iter;

		for(iter=mAnimationList.begin(); iter!=mAnimationList.end(); iter++) {
			if( (*iter) == animation ) {
				mAnimationList.erase(iter);
				break;
			}
		}
	}

	SVIBool SVIAnimationSet::isExist(SVIAnimation* animation) {
		SVIBool ret = SVIFALSE;
		AnimationSet::iterator iter;

		for (unsigned i=0; i<mAnimationList.size(); i++){
			if( mAnimationList[i] == animation ) {
				ret = SVITRUE;
				break;
			}
		}

		return ret;
	}

	void SVIAnimationSet::copy(const SVIAnimationSet* rhs) {
		SVIAnimation::copy(rhs);
		SVIAnimation* cloneAnimation = NULL;
		for(SVIUInt i=0; i<rhs->mAnimationList.size(); i++) {
			cloneAnimation = rhs->mAnimationList[i]->clone();
			cloneAnimation->setIsContainedInAniSet(SVITRUE);
			mAnimationList.push_back(cloneAnimation);
		}
	}

	SVIAnimation* SVIAnimationSet::clone() {
		SVIAnimationSet* cloneAnimation = new SVIAnimationSet(mSVIGLSurface);
		cloneAnimation->copy(this);
		cloneAnimation->shareAnimationInfo(mShareAnimationInfo);
		return (SVIAnimation*)cloneAnimation;
	}

	void SVIAnimationSet::forceEnd() {
		SVIAnimation::forceEnd();
		AnimationSet::iterator iter;
		for(iter=mAnimationList.begin(); iter!=mAnimationList.end(); iter++)
			(*iter)->forceEnd();

	}

	void SVIAnimationSet::print() {
		AnimationSet::iterator iter;
		LOGI("AniClassType AnimationSet count:%d", mAnimationList.size());
		for(iter=mAnimationList.begin(); iter!=mAnimationList.end(); iter++) {
			(*iter)->print();
		}
	}

	/*
	 * This function may be needed when Java VM delete animation instance of AnimationSet prior to delete AnimationSet
	 * So that, Animation instance that is contained in AnimationSet should not be deleted.
	 * This is the reason why this function exist.
	 */
	void SVIAnimationSet::clearAnimations() {
	 	mAnimationList.clear();
	}

	void SVIAnimationSet::shareAnimationInfo() {
		if( !mShareAnimationInfo )
			return;

		AnimationSet::iterator iter;
		for(iter=mAnimationList.begin(); iter!=mAnimationList.end(); iter++) {
			(*iter)->setInterpolator(mInterpolatorType);
			(*iter)->mDuration = mDuration;
			(*iter)->mRepeatCount = mRepeatCount;
			(*iter)->mAutoReverse = mAutoReverse;
			(*iter)->mOffset = mOffset;
		}
	}
}
