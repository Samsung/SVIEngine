#include "SVIWipeEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIWipeEffector::SVIWipeEffector(SVIGLSurface *surface):SVITransitionEffector(surface){
	}

	void SVIWipeEffector::setAnimation(){

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

		switch(mDirectionType){

		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
			addSwipeAnimation(toAniSet, SVIVector3(1.0f, 0.01f, 0.0f), SVIVector3(-0.3f, 0.3f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
			addSwipeAnimation(toAniSet, SVIVector3(0.0f, 0.01f, 1.0f), SVIVector3(1.0f, 0.3f, 1.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		}


		addZPositionAnimation(toAniSet, -0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		
		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}
}