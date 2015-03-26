#include "SVIRotateCubeEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIRotateCubeEffector::SVIRotateCubeEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface),
	rotationAngle(90.0f){
	}

	void SVIRotateCubeEffector::setAnimation(){

		SVIPoint originPoint = mFromNslide[0][0]->getPosition();

		slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount) * 0.5f;
		slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount) * 0.5f;


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
			mFromNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.5f));
			
			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(fromAniSet,SVIPoint(originPoint.mX + slideWidth, originPoint.mY), SVIPoint(originPoint.mX - slideWidth, originPoint.mY), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

			addRotationAnimation(toAniSet, SVIVector3(0.0f, -rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(toAniSet, SVIPoint(originPoint.mX + slideWidth, originPoint.mY), SVIPoint(originPoint.mX - slideWidth, originPoint.mY), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		case SVITransitionAnimation::RIGHT:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.5f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));

			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX - slideWidth, originPoint.mY), SVIPoint(originPoint.mX + slideWidth, originPoint.mY), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

			addRotationAnimation(toAniSet, SVIVector3(0.0f, rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(toAniSet, SVIPoint(originPoint.mX - slideWidth, originPoint.mY), SVIPoint(originPoint.mX + slideWidth, originPoint.mY), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		case SVITransitionAnimation::UP:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 1.0f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 0.0f));
			
			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX, originPoint.mY + slideHeight), SVIPoint(originPoint.mX, originPoint.mY - slideHeight), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

			addRotationAnimation(toAniSet, SVIVector3(rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(toAniSet, SVIPoint(originPoint.mX , originPoint.mY + slideHeight), SVIPoint(originPoint.mX , originPoint.mY - slideHeight), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		case SVITransitionAnimation::DOWN:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 0.0f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 1.0f));
			
			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX, originPoint.mY - slideHeight), SVIPoint(originPoint.mX, originPoint.mY + slideHeight), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

			addRotationAnimation(toAniSet, SVIVector3(-rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addPositionAnimation(toAniSet, SVIPoint(originPoint.mX , originPoint.mY - slideHeight), SVIPoint(originPoint.mX , originPoint.mY + slideHeight), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		}

		addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addZPositionAnimation(toAniSet, 0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}