#include "SVIScaleEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIScaleEffector::SVIScaleEffector(SVIGLSurface *surface) :
	scaleValue(0.2f,0.2f,1.0f),
	slideWidth(0),
	slideHeight(0),
	SVITransitionEffector(surface){
	}

	void SVIScaleEffector::setAnimation(){

		slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount) * 0.6f;
		slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount) * 0.6f;

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
			fromPoint = SVIPoint(originPoint.mX - slideWidth, originPoint.mY);
			toPoint = SVIPoint(originPoint.mX + slideWidth, originPoint.mY);
			break;

		case SVITransitionAnimation::RIGHT:
			fromPoint = SVIPoint(originPoint.mX + slideWidth, originPoint.mY);
			toPoint = SVIPoint(originPoint.mX -slideWidth, originPoint.mY);
			break;

		case SVITransitionAnimation::UP:
			fromPoint = SVIPoint(originPoint.mX, originPoint.mY - slideHeight);
			toPoint = SVIPoint(originPoint.mX, originPoint.mY + slideHeight);
			break;

		case SVITransitionAnimation::DOWN:
			fromPoint = SVIPoint(originPoint.mX, originPoint.mY + slideHeight);
			toPoint = SVIPoint(originPoint.mX, originPoint.mY - slideHeight);
			break;

		}

		addPositionAnimation(fromAniSet, originPoint, fromPoint, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addPositionAnimation(toAniSet, toPoint, originPoint, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addOpacityAnimation(toAniSet, 0.0f, 1.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addScaleAnimation(fromAniSet, SVIVector3(1.0f, 1.0f, 1.0f), scaleValue, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addScaleAnimation(toAniSet, scaleValue, SVIVector3(1.0f, 1.0f, 1.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}