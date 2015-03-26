#include "SVISmoothEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVISmoothEffector::SVISmoothEffector(SVIGLSurface *surface)
		:SVITransitionEffector(surface){
	}

	void SVISmoothEffector::setAnimation(){

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		fromAniSet->shareAnimationInfo(true);

		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mFullTimeDuration, mGlovalOffsetDuration);
		
		mFromNslide[0][0]->addAnimation(fromAniSet);
	}
}