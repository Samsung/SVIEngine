#include "SVIFadeThroughColorEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIFadeThroughColorEffector::SVIFadeThroughColorEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface){
	}

	void SVIFadeThroughColorEffector::setAnimation(){

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
		
		SVIKeyFrameAnimation* fromColorAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::BG_COLOR,mSVIGLSurface);
		fromColorAni->mDuration = fromAniSet->mDuration;
		fromColorAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromColorAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromColorAni->mOffset = fromAniSet->mOffset;
		fromColorAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIColor(1.0f, 1.0f, 1.0f, 1.0f)));
		fromColorAni->addKeyProperty(SVIKeyFrameProperty(0.4f, SVIColor(0.0f, 0.0f, 0.0f, 1.0f)));
		fromColorAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIColor(0.0f, 0.0f, 0.0f, 1.0f)));
		fromAniSet->addAnimation(fromColorAni);


		SVIKeyFrameAnimation* toOpacityToAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toOpacityToAni->mDuration = toAniSet->mDuration;
		toOpacityToAni->mRepeatCount = toAniSet->mRepeatCount;
		toOpacityToAni->mAutoReverse = toAniSet->mAutoReverse;
		toOpacityToAni->mOffset = toAniSet->mOffset;
		toOpacityToAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 50.0f));
		toOpacityToAni->addKeyProperty(SVIKeyFrameProperty(0.6f, 50.0f));
		toOpacityToAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
		toAniSet->addAnimation(toOpacityToAni);
		
		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, fromAniSet->mDuration, fromAniSet->mOffset);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f, toAniSet->mDuration, toAniSet->mOffset);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}
