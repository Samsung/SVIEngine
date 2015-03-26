#include "SVISlideEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVISlideEffector::SVISlideEffector(SVIGLSurface *surface):
		SVITransitionEffector(surface),
	slideWidth(0),
	slideHeight(0){
	}

	void SVISlideEffector::setAnimation(){

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

		addPositionAnimation(fromAniSet, originPoint, fromPoint, mFullTimeDuration, mGlovalOffsetDuration);
		addPositionAnimation(toAniSet, toPoint, originPoint, mFullTimeDuration, mGlovalOffsetDuration);


		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mFullTimeDuration, mGlovalOffsetDuration);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f, mFullTimeDuration, mGlovalOffsetDuration);



		SVIKeyFrameAnimation* fromOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromOpaAni->mDuration = fromAniSet->mDuration;
		fromOpaAni->mOffset = fromAniSet->mOffset;
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.8f, 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
		fromAniSet->addAnimation(fromOpaAni);

		SVIKeyFrameAnimation* toOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toOpaAni->mDuration = toAniSet->mDuration;
		toOpaAni->mOffset = toAniSet->mOffset;
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.2f, 1.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
		toAniSet->addAnimation(toOpaAni);



		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}