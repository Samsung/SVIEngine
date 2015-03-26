#include "SVIWaveEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIWaveEffector::SVIWaveEffector(SVIGLSurface *surface):
		SVITransitionEffector(surface){
	}

	void SVIWaveEffector::setAnimation(){

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

		SVIKeyFrameAnimation* fromMagnifyAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::RTE_RATIO, mSVIGLSurface);
		fromMagnifyAni->mDuration = fromAniSet->mDuration;
		fromMagnifyAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromMagnifyAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromMagnifyAni->mOffset = fromAniSet->mOffset;
		fromMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.01f, 1.0f)));
		fromMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.25f, SVIVector3(0.3f, 0.6f, 50.0f)));
		fromMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.5f, 0.7f, 100.0f)));
		fromMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.75f, SVIVector3(0.3f, 0.6f, 50.0f)));
		fromMagnifyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(0.0f, 0.01f, 1.0f)));

		SVIKeyFrameAnimation* toMagnifyAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::RTE_RATIO, mSVIGLSurface);
		toMagnifyAni->mDuration = toAniSet->mDuration;
		toMagnifyAni->mRepeatCount = toAniSet->mRepeatCount;
		toMagnifyAni->mAutoReverse = toAniSet->mAutoReverse;
		toMagnifyAni->mOffset = fromAniSet->mOffset;
		toMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.01f, 1.0f)));
		toMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.25f, SVIVector3(0.3f, 0.6f, 50.0f)));
		toMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.5f, 0.7f, 100.0f)));
		toMagnifyAni->addKeyProperty(SVIKeyFrameProperty(0.75f, SVIVector3(0.3f, 0.6f, 50.0f)));
		toMagnifyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(0.0f, 0.01f, 1.0f)));

		fromAniSet->addAnimation(fromMagnifyAni);
		toAniSet->addAnimation(toMagnifyAni);


		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		//addOpacityAnimation(toAniSet, 0.0f, 1.0f);

		addZPositionAnimation(fromAniSet, -0.1f, 0.1f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addZPositionAnimation(toAniSet, 0.1f, -0.1f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}