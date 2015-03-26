#include "SVITumbleEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVITumbleEffector::SVITumbleEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface),
	rotationAngle(180.0f),
	slideWidth(0),
	slideHeight(0){
	}

	void SVITumbleEffector::setAnimation(){

		SVIVector3 fromRotationValue;
		SVIVector3 toRotationValue;

		slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
		slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

		SVIPoint originPoint = mSizeSlide->getPosition();

		SVIPoint fromPoint;
		SVIPoint toPoint;

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
			fromRotationValue = SVIVector3( 0.0f, 0.0f, -rotationAngle);
			toRotationValue = SVIVector3(0.0f, 0.0f, rotationAngle);


			fromPoint = SVIPoint(originPoint.mX - slideWidth, originPoint.mY);
			toPoint = SVIPoint(originPoint.mX + slideWidth, originPoint.mY);
			break;
		
		case SVITransitionAnimation::RIGHT:
			fromRotationValue = SVIVector3( 0.0f, 0.0f, rotationAngle);
			toRotationValue = SVIVector3(0.0f, 0.0f, -rotationAngle);

			fromPoint = SVIPoint(originPoint.mX + slideWidth, originPoint.mY);
			toPoint = SVIPoint(originPoint.mX -slideWidth, originPoint.mY);
			break;


		case SVITransitionAnimation::UP:
			fromRotationValue = SVIVector3( 0.0f, 0.0f, rotationAngle);
			toRotationValue = SVIVector3(0.0f, 0.0f, -rotationAngle);

			fromPoint = SVIPoint(originPoint.mX, originPoint.mY - slideHeight);
			toPoint = SVIPoint(originPoint.mX, originPoint.mY + slideHeight);
			break;

		case SVITransitionAnimation::DOWN:
			fromRotationValue = SVIVector3( 0.0f, 0.0f, -rotationAngle);
			toRotationValue = SVIVector3(0.0f, 0.0f, rotationAngle);

			fromPoint = SVIPoint(originPoint.mX, originPoint.mY + slideHeight);
			toPoint = SVIPoint(originPoint.mX, originPoint.mY - slideHeight);
			break;

		}

		addPositionAnimation(fromAniSet, originPoint, fromPoint, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addPositionAnimation(toAniSet, toPoint, originPoint, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), fromRotationValue, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addRotationAnimation(toAniSet, toRotationValue, SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addOpacityAnimation(toAniSet, 0.0f, 1.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}