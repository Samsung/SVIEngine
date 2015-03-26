#include "SVIMoveInEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIMoveInEffector::SVIMoveInEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	slideWidth(0),
	slideHeight(0){
	}

	void SVIMoveInEffector::setAnimation(){

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

		addPositionAnimation(fromAniSet, originPoint, fromPoint, fromAniSet->mDuration, fromAniSet->mOffset);
		addPositionAnimation(toAniSet, toPoint, originPoint, toAniSet->mDuration, toAniSet->mOffset);

		addZPositionAnimation(fromAniSet, 0.01f, 0.01f, fromAniSet->mDuration, fromAniSet->mOffset);
		addZPositionAnimation(toAniSet, -0.01f, -0.01f, toAniSet->mDuration, toAniSet->mOffset);

		SVIKeyFrameAnimation* toOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY, mSVIGLSurface);
		toOpaAni->mDuration = toAniSet->mDuration;
		toOpaAni->mOffset = toAniSet->mOffset;
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.2f, 1.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
		toAniSet->addAnimation(toOpaAni);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}