#include "SVIRotateEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIRotateEffector::SVIRotateEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface),
	rotationAngle(90.0f){
	}

	void SVIRotateEffector::setAnimation(){

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
			mFromNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.0f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.0f));
			break;

		case SVITransitionAnimation::RIGHT:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 1.0f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 1.0f));
			break;

		case SVITransitionAnimation::UP:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 1.0f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 1.0f));
			break;

		case SVITransitionAnimation::DOWN:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.0f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.0f));
			break;

		}


		addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3( 0.0f, 0.0f, rotationAngle), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addRotationAnimation(toAniSet, SVIVector3(0.0f, 0.0f, -rotationAngle), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addOpacityAnimation(toAniSet, 0.0f, 1.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}