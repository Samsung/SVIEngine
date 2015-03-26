#include "SVIZoomINEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIZoomINEffector::SVIZoomINEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface){
		
	}

	void SVIZoomINEffector::setAnimation(){

		SVIPoint originPoint = mFromNslide[0][0]->getPosition();

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::SINEEASEINOUT);
		fromAniSet->shareAnimationInfo(true);

		SVIKeyFrameAnimation* offsetAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY, mSVIGLSurface);
		offsetAni->mDuration = fromAniSet->mDuration;
		offsetAni->mRepeatCount = fromAniSet->mRepeatCount;
		offsetAni->mAutoReverse = fromAniSet->mAutoReverse;
		offsetAni->mOffset = fromAniSet->mOffset;
		offsetAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));
		offsetAni->addKeyProperty(SVIKeyFrameProperty(0.9f, 1.0f));
		offsetAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));

		fromAniSet->addAnimation(offsetAni);

		
		addScaleAnimation(fromAniSet,SVIVector3(1.0f, 1.0f, 1.0f), SVIVector3(2.0f, 2.0f, 2.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		mFromNslide[0][0]->addAnimation(fromAniSet);
	}
}
