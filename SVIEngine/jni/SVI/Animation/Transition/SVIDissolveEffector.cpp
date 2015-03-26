#include "SVIDissolveEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIDissolveEffector::SVIDissolveEffector(SVIGLSurface *surface):SVITransitionEffector(surface){
	}

	void SVIDissolveEffector::setAnimation(){

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::ACCELERATE);
		fromAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::ACCELERATE);
		toAniSet->shareAnimationInfo(true);

		addDeformAnimation(toAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(1.0f, 0.0f, 0.0f),mSlideDuration,mOffsetDuration);
		//addOpacityAnimation(fromAniSet, 0.0f, 0.0f);
		//addOpacityAnimation(toAniSet, 0.0f, 0.0f);
		//addScaleAnimation(fromAniSet,SVIVector3(0.7f,0.7f,1.0f), SVIVector3(0.5f,0.5f,0.5f));
		
		addZPositionAnimation(fromAniSet, 0.01f, 0.01f,fromAniSet->mDuration,fromAniSet->mOffset);
		addZPositionAnimation(toAniSet, -0.01f, -0.01f,toAniSet->mDuration,toAniSet->mOffset);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}
