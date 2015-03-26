#include "SVIFallEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIFallEffector::SVIFallEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rotationAngle(90.0f),
	scaleValue(1.0f,0.0f,0.0f){
	}

	void SVIFallEffector::setAnimation(){

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		//fromAniSet->setInterpolator(SVIAnimation::CUBICEASEIN);
		fromAniSet->setInterpolator(SVIAnimation::ACCELERATE);
		fromAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::ACCELERATE);
		toAniSet->shareAnimationInfo(true);

		
		switch(mDirectionType){
		
		case SVITransitionAnimation::LEFT:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.5f));
			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), fromAniSet->mDuration, fromAniSet->mOffset);
			//addScaleAnimation(fromAniSet, SVIVector3(1.0f, 1.0f, 1.0f), SVIVector3(0.0f,1.0f,0.0f));
			break;

		case SVITransitionAnimation::RIGHT:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));
			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), fromAniSet->mDuration, fromAniSet->mOffset);
			//addScaleAnimation(fromAniSet, SVIVector3(1.0f, 1.0f, 1.0f), SVIVector3(0.0f,1.0f,0.0f));
			break;

		case SVITransitionAnimation::UP:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 0.0f));
			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), fromAniSet->mDuration, fromAniSet->mOffset);
			//addScaleAnimation(fromAniSet, SVIVector3(1.0f, 1.0f, 1.0f), SVIVector3(1.0f,0.0f,0.0f));
			break;

		case SVITransitionAnimation::DOWN:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 1.0f));
			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), fromAniSet->mDuration, fromAniSet->mOffset);
			//addScaleAnimation(fromAniSet, SVIVector3(1.0f, 1.0f, 1.0f), SVIVector3(1.0f,0.0f,0.0f));
			break;

		}

		addZPositionAnimation(fromAniSet, -0.01f, -0.01f, fromAniSet->mDuration, fromAniSet->mOffset);
		addZPositionAnimation(toAniSet, 0.01f, 0.01f, fromAniSet->mDuration, fromAniSet->mOffset);


		//addOpacityAnimation(fromAniSet, 1.0f, 0.0f);

		SVIKeyFrameAnimation* fromOpacityAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromOpacityAni->mDuration = fromAniSet->mDuration;
		fromOpacityAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIFloat(1.0f)));
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.4f, SVIFloat(1.0f)));
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIFloat(0.0f)));
		fromAniSet->addAnimation(fromOpacityAni);


		mFromNslide[0][0]->addAnimation(fromAniSet);
		//mFromNslide[0][0]->setOrthogonal(SVITRUE);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}
