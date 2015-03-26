#include "SVIWizzleEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIWizzleEffector::SVIWizzleEffector(SVIGLSurface *surface):
		SVITransitionEffector(surface),
	mSetEffectTime(0){
	}

	void SVIWizzleEffector::setAnimation(){

		setEffectTimeLine();

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

		SVIKeyFrameAnimation* fromWizzleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::RTE_RATIO, mSVIGLSurface);
		fromWizzleAni->mDuration = fromAniSet->mDuration;
		fromWizzleAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromWizzleAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromWizzleAni->mOffset = fromAniSet->mOffset;
		fromWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(40.0f, 0.0f, 0.0f)));
		fromWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.25f, SVIVector3(40.0f, 0.005f, mSetEffectTime * 1.0f)));
		fromWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(40.0f, 0.01f, mSetEffectTime * 2.0f)));
		fromWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.75f, SVIVector3(40.0f, 0.005f, mSetEffectTime * 3.0f)));
		fromWizzleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(40.0f, 0.0f, mSetEffectTime * 4.0f)));

		SVIKeyFrameAnimation* toWizzleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::RTE_RATIO, mSVIGLSurface);
		toWizzleAni->mDuration = toAniSet->mDuration;
		toWizzleAni->mRepeatCount = toAniSet->mRepeatCount;
		toWizzleAni->mAutoReverse = toAniSet->mAutoReverse;
		toWizzleAni->mOffset = fromAniSet->mOffset;
		toWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(40.0f, 0.00f, 0.0f)));
		toWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.25f, SVIVector3(40.0f, 0.005f, mSetEffectTime * 1.0f)));
		toWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(40.0f, 0.01f, mSetEffectTime * 2.0f)));
		toWizzleAni->addKeyProperty(SVIKeyFrameProperty(0.75f, SVIVector3(40.0f, 0.005f, mSetEffectTime * 3.0f)));
		toWizzleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(40.0f, 0.0f, mSetEffectTime * 4.0f)));

		fromAniSet->addAnimation(fromWizzleAni);
		toAniSet->addAnimation(toWizzleAni);
		

		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
	

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}

	void SVIWizzleEffector::setEffectTimeLine(){
		mSetEffectTime = mFullTimeDuration * 0.000125f;
	}
}