#include "SVIFlashEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIFlashEffector::SVIFlashEffector(SVIGLSurface *surface):SVITransitionEffector(surface){};

	void SVIFlashEffector::setAnimation(){
		
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

		SVIKeyFrameAnimation* lightFromAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::LIGHT_RADIUS,mSVIGLSurface);
		lightFromAni->mDuration = fromAniSet->mDuration;
		lightFromAni->mOffset = fromAniSet->mOffset;
		lightFromAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.3f));
		lightFromAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 5.0f));
		lightFromAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 5.0f));

		fromAniSet->addAnimation(lightFromAni);


		SVIKeyFrameAnimation* opaToAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		opaToAni->mDuration = fromAniSet->mDuration;
		opaToAni->mOffset = fromAniSet->mOffset;
		opaToAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 100.0f));
		opaToAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 100.0f));
		opaToAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));

		toAniSet->addAnimation(opaToAni);

		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, fromAniSet->mDuration, fromAniSet->mOffset);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f, fromAniSet->mDuration, fromAniSet->mOffset);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}