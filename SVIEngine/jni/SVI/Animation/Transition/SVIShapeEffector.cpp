#include "SVIShapeEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIShapeEffector::SVIShapeEffector(SVIGLSurface *surface):SVITransitionEffector(surface){
	}

	void SVIShapeEffector::setAnimation(){

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		fromAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		toAniSet->shareAnimationInfo(true);

		SVIKeyFrameAnimation* toSwipeAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SWIPE_RATIO,mSVIGLSurface);
		toSwipeAni->mDuration = toAniSet->mDuration;
		toSwipeAni->mRepeatCount = toAniSet->mRepeatCount;
		toSwipeAni->mAutoReverse = toAniSet->mAutoReverse;
		toSwipeAni->mOffset = toAniSet->mOffset;
		toSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.01f, 1.0f)));
		toSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 0.3f, 1.0f)));
		toAniSet->addAnimation(toSwipeAni);


		addZPositionAnimation(toAniSet, -0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		
		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}
}