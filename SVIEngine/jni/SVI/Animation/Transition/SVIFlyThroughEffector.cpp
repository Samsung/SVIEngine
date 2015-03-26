#include "SVIFlyThroughEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIFlyThroughEffector::SVIFlyThroughEffector(SVIGLSurface *surface):SVITransitionEffector(surface){};

	void SVIFlyThroughEffector::setAnimation(){

		SVIVector3 fromRotationValue;
		SVIVector3 toRotationValue;


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

		SVIKeyFrameAnimation* opaFromAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		opaFromAni->mDuration = fromAniSet->mDuration;
		opaFromAni->mOffset = fromAniSet->mOffset;
		opaFromAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));
		opaFromAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 0.0f));
		opaFromAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.01f));
		fromAniSet->addAnimation(opaFromAni);


		SVIKeyFrameAnimation* opaToAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		opaToAni->mDuration = toAniSet->mDuration;
		opaToAni->mOffset = toAniSet->mOffset;
		opaToAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.5f));
		opaToAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 1.0f));
		opaToAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
		toAniSet->addAnimation(opaToAni);


		addScaleAnimation(fromAniSet, SVIVector3(1.0f, 1.0f, 1.0f), SVIVector3(2.0f, 2.0f, 2.0f), fromAniSet->mDuration, fromAniSet->mOffset);
		addScaleAnimation(toAniSet, SVIVector3(0.5f, 0.5f, 0.5f), SVIVector3(1.0f, 1.0f, 1.0f), toAniSet->mDuration, toAniSet->mOffset);


		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}