#include "SVIThrowEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIThrowEffector::SVIThrowEffector(SVIGLSurface *surface) :	
		SVITransitionEffector(surface),
		rotationAngle(180.0f),
		rowCount(2),
		columnCount(2){
			mRowCount = rowCount;
			mColumnCount = columnCount;
	}

	void SVIThrowEffector::setAnimation(){
		setOffsetDuration(mFullTimeDuration);

		slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
		slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

		//SVIPoint* toStartPoint = new SVIPoint[4];
		//SVIPoint* toPassPoint = new SVIPoint[4];
		SVIPoint toStartPoint;
		SVIPoint toPassPoint;

		SVIVector3 angleVector = SVIVector3(-50, 0, 0);
		/*
		if (mDirectionType == SVITransitionAnimation::UP) {
			angleVector = SVIVector3(-50, 0, 0);
		} else if (mDirectionType == SVITransitionAnimation::DOWN) {
			angleVector = SVIVector3(50, 0, 0);
		} else if (mDirectionType == SVITransitionAnimation::LEFT) {
			angleVector = SVIVector3(0, 50, 0);
		} else if (mDirectionType == SVITransitionAnimation::RIGHT) {
			angleVector = SVIVector3(0, -50, 0);
		}
		
		if (i==0 && j==0) {
			toStartPoint = SVIPoint(slideWidth/2.0f, slideHeight);
			toPassPoint = SVIPoint(slideWidth/2.0f, -slideHeight*0.2f);
		} else if (i==0 && j==1) {
			toStartPoint = SVIPoint(slideWidth/2.0f+slideWidth, slideHeight);
			toPassPoint = SVIPoint(slideWidth/2.0f+slideWidth, -slideHeight*0.2f);
		} else if (i==1 && j==0) {
			toStartPoint = SVIPoint(slideWidth/2.0f, slideHeight*2);
			toPassPoint = SVIPoint(slideWidth/2.0f, slideHeight*0.8f);
		} else if (i==1 && j==1) {
			toStartPoint = SVIPoint(slideWidth/2.0f+slideWidth, slideHeight*2);
			toPassPoint = SVIPoint(slideWidth/2.0f+slideWidth, slideHeight*0.8f);
		}
		*/
			


		for (SVIInt i=0; i<mRowCount; i++) {
			for (SVIInt j=0; j<mColumnCount; j++) {
				mFromNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 1.0f));
				mToNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 1.0f));
			}
		}
	

		for (SVIInt i=0; i<mColumnCount; i++) {

			SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
			fromAniSet->mOffset = mGlovalOffsetDuration;
			fromAniSet->mDuration = mSlideDuration;
			fromAniSet->mRepeatCount = mRepeatCount;
			fromAniSet->mAutoReverse = mAutoReverse;
			fromAniSet->setInterpolator(SVIAnimation::ACCELERATE);
		
			SVIKeyFrameAnimation* fromZPositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION, mSVIGLSurface);
			fromZPositionAni->mDuration = fromAniSet->mDuration;
			fromZPositionAni->mRepeatCount = fromAniSet->mRepeatCount;
			fromZPositionAni->mAutoReverse = fromAniSet->mAutoReverse;
			fromZPositionAni->mOffset = fromAniSet->mOffset;
			fromZPositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
			fromZPositionAni->addKeyProperty(SVIKeyFrameProperty(0.2f, 100.0f));
			fromZPositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 100.0f));
			fromAniSet->addAnimation(fromZPositionAni);	
		
			SVIKeyFrameAnimation* fromRotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION, mSVIGLSurface);
			fromRotateAni->mDuration = fromAniSet->mDuration;
			fromRotateAni->mRepeatCount = fromAniSet->mRepeatCount;
			fromRotateAni->mAutoReverse = fromAniSet->mAutoReverse;
			fromRotateAni->mOffset = fromAniSet->mOffset;
			SVIFloat x, y, z;
			x = SVI_DEGTORAD(angleVector.x);
			y = SVI_DEGTORAD(angleVector.y);
			z = SVI_DEGTORAD(angleVector.z);
			SVIQuaternion q;
			q.setIdentity();
			q.setEuler(SVIVector3(0, y, z));
			fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
			q.setEuler(SVIVector3(x, y, z));
			fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.2f, SVIVector4(q.x, q.y, q.z, q.w)));
			fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector4(q.x, q.y, q.z, q.w)));	
			q.setEuler(SVIVector3(0, y, z));
			fromRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
			fromAniSet->addAnimation(fromRotateAni);

			mFromNslide[0][i]->addAnimation(fromAniSet);
		}

		for (SVIInt i=0; i<mColumnCount; i++) {

			SVIAnimationSet* fromAniSet2 = new SVIAnimationSet(mSVIGLSurface);
			fromAniSet2->mOffset = mGlovalOffsetDuration;
			fromAniSet2->mDuration = mSlideDuration;
			fromAniSet2->mRepeatCount = mRepeatCount;
			fromAniSet2->mAutoReverse = mAutoReverse;
			fromAniSet2->setInterpolator(SVIAnimation::ACCELERATE);

			SVIKeyFrameAnimation* fromZPositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION, mSVIGLSurface);
			fromZPositionAni->mDuration = fromAniSet2->mDuration;
			fromZPositionAni->mRepeatCount = fromAniSet2->mRepeatCount;
			fromZPositionAni->mAutoReverse = fromAniSet2->mAutoReverse;
			fromZPositionAni->mOffset = fromAniSet2->mOffset;
			fromZPositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
			fromZPositionAni->addKeyProperty(SVIKeyFrameProperty(0.2f, 100.0f));
			fromZPositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 100.0f));
			fromAniSet2->addAnimation(fromZPositionAni);

			mFromNslide[1][i]->addAnimation(fromAniSet2);
		}

		for(SVIInt i=0; i<mRowCount; i++) {
			for (SVIInt j=0; j<mColumnCount; j++) {		
				if (i==0 && j==0) {
					toStartPoint = SVIPoint(slideWidth/2.0f, slideHeight);
					toPassPoint = SVIPoint(slideWidth/2.0f, -slideHeight*0.2f);
				} else if (i==0 && j==1) {
					toStartPoint = SVIPoint(slideWidth/2.0f+slideWidth, slideHeight);
					toPassPoint = SVIPoint(slideWidth/2.0f+slideWidth, -slideHeight*0.2f);
				} else if (i==1 && j==0) {
					toStartPoint = SVIPoint(slideWidth/2.0f, slideHeight*2);
					toPassPoint = SVIPoint(slideWidth/2.0f, slideHeight*0.8f);
				} else if (i==1 && j==1) {
					toStartPoint = SVIPoint(slideWidth/2.0f+slideWidth, slideHeight*2);
					toPassPoint = SVIPoint(slideWidth/2.0f+slideWidth, slideHeight*0.8f);
				}

				SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
				toAniSet->mOffset = mGlovalOffsetDuration;
				toAniSet->mDuration = mSlideDuration;
				toAniSet->mRepeatCount = mRepeatCount;
				toAniSet->mAutoReverse = mAutoReverse;
				toAniSet->setInterpolator(SVIAnimation::ACCELERATE);


				SVIKeyFrameAnimation* toZPositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION, mSVIGLSurface);
				toZPositionAni->mDuration = toAniSet->mDuration;
				toZPositionAni->mRepeatCount = toAniSet->mRepeatCount;
				toZPositionAni->mAutoReverse = toAniSet->mAutoReverse;
				toZPositionAni->mOffset = toAniSet->mOffset;
				toZPositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 100.0f));
				toZPositionAni->addKeyProperty(SVIKeyFrameProperty(0.65f, 100.0f));
				toZPositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
				toAniSet->addAnimation(toZPositionAni);

				SVIKeyFrameAnimation* toOpacityAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY, mSVIGLSurface);
				toOpacityAni->mDuration = toAniSet->mDuration;
				toOpacityAni->mRepeatCount = toAniSet->mRepeatCount;
				toOpacityAni->mAutoReverse = toAniSet->mAutoReverse;
				toOpacityAni->mOffset = toAniSet->mOffset;
				toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
				toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.3f, 0.0f));
				toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.4f, 1.0f));
				toOpacityAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
				toAniSet->addAnimation(toOpacityAni);

				SVIKeyFrameAnimation* toPositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION, mSVIGLSurface);
				toPositionAni->mDuration = toAniSet->mDuration;
				toPositionAni->mRepeatCount = toAniSet->mRepeatCount;
				toPositionAni->mAutoReverse = toAniSet->mAutoReverse;
				toPositionAni->mOffset = toAniSet->mOffset;
				toPositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, toStartPoint));
				toPositionAni->addKeyProperty(SVIKeyFrameProperty(0.3f, toStartPoint));
				toPositionAni->addKeyProperty(SVIKeyFrameProperty(0.65f, toPassPoint));
				toPositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, toStartPoint));
				toAniSet->addAnimation(toPositionAni);

				//addScaleAnimation(toAniSet2, SVIVector3(0.5f, 0.5f, 0.0f), SVIVector3(1.0f, 1.0f, 0.0f));

				mToNslide[i][j]->addAnimation(toAniSet);
			}
		}
		
	}
	void SVIThrowEffector::setOffsetDuration(SVIInt fullTimeDuration){
		fullTimeDuration = mFullTimeDuration;
		mOffsetDuration = fullTimeDuration*0.1f;
		mSlideDuration = fullTimeDuration - mOffsetDuration;
	}
}
