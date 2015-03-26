#include "SVIExploidEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIExploidEffector::SVIExploidEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface)
	{
	}

	void SVIExploidEffector::setAnimation(){

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::SINEEASEINOUT);
		fromAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::SINEEASEINOUT);
		toAniSet->shareAnimationInfo(true);

		addDeformAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(2.0f, 0.0f, 0.0f), fromAniSet->mDuration, fromAniSet->mOffset);
		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, fromAniSet->mDuration, fromAniSet->mOffset);

		addScaleAnimation(toAniSet,SVIVector3(0.7f,0.7f,1.0f), SVIVector3(1.0f,1.0f,1.0f), toAniSet->mDuration, toAniSet->mOffset);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}