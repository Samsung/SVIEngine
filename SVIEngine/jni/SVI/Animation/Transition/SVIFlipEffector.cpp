#include "SVIFlipEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIFlipEffector::SVIFlipEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface),
		rotationAngle(180.0f){
	}

	void SVIFlipEffector::setAnimation(){

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

		switch(mDirectionType){
		case SVITransitionAnimation::LEFT:
			fromRotationValue = SVIVector3( 0.0f, rotationAngle, 0.0f);
			toRotationValue = SVIVector3(0.0f, -rotationAngle, 0.0f);
			break;

		case SVITransitionAnimation::RIGHT:
			fromRotationValue = SVIVector3( 0.0f, -rotationAngle, 0.0f);
			toRotationValue = SVIVector3(0.0f, rotationAngle, 0.0f);
			break;

		case SVITransitionAnimation::UP:
			fromRotationValue = SVIVector3(-rotationAngle, 0.0f, 0.0f);
			toRotationValue = SVIVector3(rotationAngle, 0.0f, 0.0f);
			break;

		case SVITransitionAnimation::DOWN:
			fromRotationValue = SVIVector3(rotationAngle, 0.0f, 0.0f);
			toRotationValue = SVIVector3(-rotationAngle, 0.0f, 0.0f);
			break;

		}


		addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), fromRotationValue, fromAniSet->mDuration, fromAniSet->mOffset);
		addRotationAnimation(toAniSet, toRotationValue, SVIVector3(0.0f, 0.0f, 0.0f), fromAniSet->mDuration, fromAniSet->mOffset);

		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, fromAniSet->mDuration, fromAniSet->mOffset);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f,  fromAniSet->mDuration, fromAniSet->mOffset);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}
