#include "SVIRevealEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIRevealEffector::SVIRevealEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface){
	}

	void SVIRevealEffector::setAnimation(){

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

		
		SVIKeyFrameAnimation* fromSwipeAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SWIPE_RATIO,mSVIGLSurface);
		fromSwipeAni->mDuration = toAniSet->mDuration;
		fromSwipeAni->mRepeatCount = toAniSet->mRepeatCount;
		fromSwipeAni->mAutoReverse = toAniSet->mAutoReverse;
		fromSwipeAni->mOffset = toAniSet->mOffset;
		switch(mDirectionType){
		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
			fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 0.3f, 1.0f)));
			fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.0f, 0.0f, 1.0f)));
			fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(0.0f, 0.0f, 1.0f)));
			break;

		case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
			fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.0f, 0.0f)));
			fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(1.0f, 0.3f, 0.0f)));
			fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 0.3f, 0.0f)));
			break;

		}
		//right to left, appearing
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 0.3f, 0.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.0f, 0.0f, 0.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(0.0f, 0.0f, 0.0f)));
		//right to left, disappearing
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 0.3f, 1.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.0f, 0.0f, 1.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(0.0f, 0.0f, 1.0f)));
		//left to right, appearing
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.0f, 1.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(1.0f, 0.3f, 1.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 0.3f, 1.0f)));
		//left to right, disappearing
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.0f, 0.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(1.0f, 0.3f, 0.0f)));
		//fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 0.3f, 0.0f)));
		fromAniSet->addAnimation(fromSwipeAni);

		SVIKeyFrameAnimation* fromOpacityAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromOpacityAni->mDuration = fromAniSet->mDuration;
		fromOpacityAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromOpacityAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromOpacityAni->mOffset = fromAniSet->mOffset;
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));			
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 1.0f));
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.51f, 0.0f));
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
		fromAniSet->addAnimation(fromOpacityAni);
		


		SVIKeyFrameAnimation* toSwipeAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SWIPE_RATIO,mSVIGLSurface);
		toSwipeAni->mDuration = toAniSet->mDuration;
		toSwipeAni->mRepeatCount = toAniSet->mRepeatCount;
		toSwipeAni->mAutoReverse = toAniSet->mAutoReverse;
		toSwipeAni->mOffset = toAniSet->mOffset;
		switch(mDirectionType){
		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
			toSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.0f, 1.0f)));
			toSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.0f, 0.0f, 1.0f)));
			toSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 0.3f, 1.0f)));
			break;

		case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
			toSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 0.3f, 0.0f)));
			toSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(1.0f, 0.3f, 0.0f)));
			toSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(0.0f, 0.0f, 0.0f)));
			break;

		}
		toAniSet->addAnimation(toSwipeAni);

		SVIKeyFrameAnimation* toOpacityAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toOpacityAni->mDuration = fromAniSet->mDuration;
		toOpacityAni->mRepeatCount = fromAniSet->mRepeatCount;
		toOpacityAni->mAutoReverse = fromAniSet->mAutoReverse;
		toOpacityAni->mOffset = fromAniSet->mOffset;
		toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));			
		toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.49f, 0.0f));
		toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 1.0f));
		toOpacityAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
		toAniSet->addAnimation(toOpacityAni);

		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}
}