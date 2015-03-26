#include "SVICube4PiecesEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"
#include "../../Render/SVITexture.h"
#include "../../Render/SVIFrameRenderer.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVICube4PiecesEffector::SVICube4PiecesEffector(SVIGLSurface *surface) :SVITransitionEffector(surface),
	rowCount(1),
	columnCount(4),
	rotationAngle(180.0f) {
		mRowCount = rowCount;
		mColumnCount = columnCount;
	}

	void SVICube4PiecesEffector::setAnimation(){

		switch(mDirectionType){

		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::RIGHT:
			mFromNslide[0][0]->setBackFaceImageFlip(SVIFALSE);
			mFromNslide[3][0]->setBackFaceImageFlip(SVIFALSE);
			
			break;

		case SVITransitionAnimation::UP:
		case SVITransitionAnimation::DOWN:
			mFromNslide[1][0]->setBackFaceImageFlip(SVIFALSE);
			mFromNslide[2][0]->setBackFaceImageFlip(SVIFALSE);
			break;

		}
		
		for(SVIInt i = 0; i< mColumnCount; i++){

			mToSlide->getSVIGLSurface()->getSlideCapture()->setBackFaceCaptureSlideToSlide(mToSlide->getProjectionSlide(),mFromNslide[i][0]->getProjectionSlide());

			SVISlideTextureContainer * pContainer = mFromNslide[0][0]->getProjectionSlide()->getTextureContainer();
			SVICaptureData * pCaptureData = pContainer != NULL ? pContainer->getBackFaceCaptureTexture() : NULL;

			if (pCaptureData != NULL){
				SVIVector2 & size = pCaptureData->getSize();
				SVIFloat width = size.x;
				SVIFloat height = size.y / mColumnCount;
				SVIRect tempRect = SVIRect(0.0f, height * i, width, height);
				mFromNslide[i][0]->setBackFaceTextureRegion(tempRect);
			}
		}

		SVIAnimationSet* onePannelAni = new SVIAnimationSet(mSVIGLSurface);
		onePannelAni->mDuration = mFullTimeDuration;
		//onePannelAni->mDuration = mFullTimeDuration * 0.8f;
		onePannelAni->mRepeatCount = mRepeatCount;
		onePannelAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		onePannelAni->mAutoReverse = mAutoReverse;
		//onePannelAni->setInterpolator(SVIAnimation::LINEAR);
		onePannelAni->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

		SVIAnimationSet* twoPannelAni = new SVIAnimationSet(mSVIGLSurface);
		twoPannelAni->mDuration = mFullTimeDuration;
		//twoPannelAni->mDuration = mFullTimeDuration * 0.8f;
		twoPannelAni->mRepeatCount = mRepeatCount;
		twoPannelAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		twoPannelAni->mAutoReverse = mAutoReverse;
		//twoPannelAni->setInterpolator(SVIAnimation::LINEAR);
		twoPannelAni->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

		SVIAnimationSet* threePannelAni = new SVIAnimationSet(mSVIGLSurface);
		threePannelAni->mDuration = mFullTimeDuration;
		//threePannelAni->mDuration = mFullTimeDuration * 0.8f;
		threePannelAni->mRepeatCount = mRepeatCount;
		threePannelAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		threePannelAni->mAutoReverse = mAutoReverse;
		//threePannelAni->setInterpolator(SVIAnimation::LINEAR);
		threePannelAni->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

		SVIAnimationSet* fourPannelAni = new SVIAnimationSet(mSVIGLSurface);
		fourPannelAni->mDuration = mFullTimeDuration;
		//fourPannelAni->mDuration = mFullTimeDuration * 0.8f;
		fourPannelAni->mRepeatCount = mRepeatCount;
		fourPannelAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fourPannelAni->mAutoReverse = mAutoReverse;
		//fourPannelAni->setInterpolator(SVIAnimation::LINEAR);
		fourPannelAni->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

		switch(mDirectionType){
		
		case SVITransitionAnimation::LEFT:
			addRotationAnimation(onePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(twoPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(threePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(fourPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		case SVITransitionAnimation::RIGHT:
			addRotationAnimation(onePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(twoPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(threePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(fourPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		case SVITransitionAnimation::UP:
			addRotationAnimation(onePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(twoPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(threePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(fourPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		case SVITransitionAnimation::DOWN:
			addRotationAnimation(onePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(twoPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(threePannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addRotationAnimation(fourPannelAni, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			break;

		}				



		/*
		SVIKeyFrameAnimation* scaleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SCALE);
		scaleAni->mDuration = parentAniSet->mDuration;
		scaleAni->mRepeatCount = parentAniSet->mRepeatCount;
		scaleAni->mAutoReverse = parentAniSet->mAutoReverse;
		scaleAni->mOffset = parentAniSet->mOffset;
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.15f, SVIVector3(0.8f, 0.8f, 0.8f)));
		//scaleAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.7f, 0.7f, 0.7f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.85f, SVIVector3(0.8f, 0.8f, 0.8f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
		parentAniSet->addAnimation(scaleAni);
		*/



		/*
		addZPositionAnimationTime(onePannelAni, -0.01f, 300.0f, 7.01f);
		addZPositionAnimationTime(twoPannelAni, -0.01f, 300.0f, 7.01f);
		addZPositionAnimationTime(threePannelAni, -0.015f, 300.0f, 7.01f);
		addZPositionAnimationTime(fourPannelAni, -0.01f, 300.0f, 7.015f);
		*/


		
		mFromNslide[0][0]->addAnimation(onePannelAni);
		//mFromNslide[0][0]->setOrthogonal(SVITRUE);
		mFromNslide[1][0]->addAnimation(twoPannelAni);
		//mFromNslide[1][0]->setOrthogonal(SVITRUE);
		mFromNslide[2][0]->addAnimation(threePannelAni);
		//mFromNslide[2][0]->setOrthogonal(SVITRUE);
		mFromNslide[3][0]->addAnimation(fourPannelAni);
		//mFromNslide[3][0]->setOrthogonal(SVITRUE);




		SVIAnimationSet* toAniSet1 = new SVIAnimationSet(mSVIGLSurface);
		toAniSet1->mDuration = mFullTimeDuration;
		toAniSet1->mRepeatCount = mRepeatCount;
		toAniSet1->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet1->mAutoReverse = mAutoReverse;
		toAniSet1->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

		SVIAnimationSet* toAniSet2 = new SVIAnimationSet(mSVIGLSurface);
		toAniSet2->mDuration = mFullTimeDuration;
		toAniSet2->mRepeatCount = mRepeatCount;
		toAniSet2->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet2->mAutoReverse = mAutoReverse;
		toAniSet2->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

		SVIAnimationSet* toAniSet3 = new SVIAnimationSet(mSVIGLSurface);
		toAniSet3->mDuration = mFullTimeDuration;
		toAniSet3->mRepeatCount = mRepeatCount;
		toAniSet3->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet3->mAutoReverse = mAutoReverse;
		toAniSet3->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

		SVIAnimationSet* toAniSet4 = new SVIAnimationSet(mSVIGLSurface);
		toAniSet4->mDuration = mFullTimeDuration;
		toAniSet4->mRepeatCount = mRepeatCount;
		toAniSet4->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet4->mAutoReverse = mAutoReverse;
		toAniSet4->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

		addOpacityAnimation(toAniSet1, 0.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addOpacityAnimation(toAniSet2, 0.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addOpacityAnimation(toAniSet3, 0.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addOpacityAnimation(toAniSet4, 0.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		mToNslide[0][0]->addAnimation(toAniSet1);
		mToNslide[1][0]->addAnimation(toAniSet2);
		mToNslide[2][0]->addAnimation(toAniSet3);
		mToNslide[3][0]->addAnimation(toAniSet4);

		


		SVIAnimationSet* parentAniSet = new SVIAnimationSet(mSVIGLSurface);
		parentAniSet->mDuration = mFullTimeDuration;
		parentAniSet->mRepeatCount = mRepeatCount;
		parentAniSet->mOffset =  mGlovalOffsetDuration + mOffsetDuration;
		parentAniSet->mAutoReverse = mAutoReverse;
		parentAniSet->setInterpolator(SVIAnimation::LINEAR);


		SVIKeyFrameAnimation* scaleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SCALE,mSVIGLSurface);
		scaleAni->mDuration = parentAniSet->mDuration;
		scaleAni->mRepeatCount = parentAniSet->mRepeatCount;
		scaleAni->mAutoReverse = parentAniSet->mAutoReverse;
		scaleAni->mOffset = parentAniSet->mOffset;
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.15f, SVIVector3(0.8f, 0.8f, 0.8f)));
		//scaleAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.7f, 0.7f, 0.7f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.85f, SVIVector3(0.8f, 0.8f, 0.8f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
		parentAniSet->addAnimation(scaleAni);
		

		mParentSlide->addAnimation(parentAniSet);

	}

	void SVICube4PiecesEffector::addZPositionAnimationTime(SVIAnimationSet* aniSet, SVIFloat preZPosition, SVIFloat midZPosition, SVIFloat nextZPosition)
	{
		SVIKeyFrameAnimation* zPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		zPosAni->mDuration = aniSet->mDuration;
		zPosAni->mRepeatCount = aniSet->mRepeatCount;
		zPosAni->mAutoReverse = aniSet->mAutoReverse;
		zPosAni->mOffset = aniSet->mOffset;

		// from Z-Position
		zPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preZPosition));
		// mid Z-Position
		zPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, midZPosition));
		// to Z-Position
		zPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextZPosition));
		
		aniSet->addAnimation(zPosAni);
	}
}