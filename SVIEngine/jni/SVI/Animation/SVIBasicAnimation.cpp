#include "SVIBasicAnimation.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Slide/SVIBeamedSlide.h"

namespace SVI {

	SVIBasicAnimation::SVIBasicAnimation(SVIGLSurface* saGLSurface):SVIPropertyAnimation(saGLSurface) {
		mClassType = BASIC;
	}

	SVIBasicAnimation::SVIBasicAnimation(PropertyAnimationType type, SVIGLSurface* saGLSurface):SVIPropertyAnimation(saGLSurface) {
		mPropertyAnimationType = type; // this is dummy code to avoid compile error : declaration of "XXX" shadows a parameter.
		//SVIPropertyAnimation::SVIPropertyAnimation(mPropertyAnimationType);
		mClassType = BASIC;
	}

	SVIBasicAnimation::~SVIBasicAnimation() {
	}

	SVIBasicAnimation* SVIBasicAnimation::create(PropertyAnimationType type, SVIGLSurface* saGLSurface) {
		SVIBasicAnimation* animation = new SVIBasicAnimation(type, saGLSurface);
		
		return animation;
	}

	void SVIBasicAnimation::update(SVIUInt& time) {
		updateInfo(time);

		if( getStatus() == ANIMATING ) {
			calculateAndApply(mTimeRatio);

			if( isEnd(time) ) {
				setStatus(END);
				if( !mIsContainedInAniSet && mSlide != NULL ) {
					mSlide->removeAnimation();
				}
			}
		}
	}

	void SVIBasicAnimation::copy(const SVIBasicAnimation* rhs) {
		SVIPropertyAnimation::copy(rhs);
	}

	SVIAnimation* SVIBasicAnimation::clone() {
		SVIBasicAnimation* cloneAnimation = new SVIBasicAnimation(mSVIGLSurface);
		cloneAnimation->copy(this);
		return (SVIAnimation*)cloneAnimation;
	}

	void SVIBasicAnimation::print() {
		LOGI("AniClassType : BasicAnimation, PropertyType : %d", mPropertyAnimationType);
	}
}