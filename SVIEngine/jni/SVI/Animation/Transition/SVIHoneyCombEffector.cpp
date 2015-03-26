#include "SVIHoneyCombEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIHoneyCombEffector::SVIHoneyCombEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rowCount(11),
		columnCount(5){

			mRowCount = rowCount;
			mColumnCount = columnCount;
			mIsUseParent = SVITRUE;
	}

	void SVIHoneyCombEffector::setAnimation(){

		SVIInt **mOffsetMap;


		mOffsetMap = new SVIInt *[mColumnCount];
		for(int i =0; i<mColumnCount; i++){
			mOffsetMap[i] = new SVIInt[mRowCount];
		}

		SVIInt rowOffsetMap[5][11] =  {
			{3,2,3,2,1,1,2,1,2,4,3},
			{3,3,1,0,1,0,1,0,2,3,3},
			{3,2,1,1,0,0,1,1,0,2,4},
			{4,3,2,1,1,1,0,2,1,3,3},
			{3,3,1,2,0,1,1,1,2,3,3}

		};

		SVIInt columnOffsetMap[11][5] = {
			{3,3,3,4,3},
			{2,3,2,3,3},
			{3,1,1,2,1},
			{2,0,1,1,2},
			{1,1,0,1,0},
			{1,0,0,1,1},
			{2,1,1,0,1},
			{1,0,1,2,1},
			{2,2,0,1,2},
			{4,3,2,3,3},
			{3,3,4,3,3}
		};

		for(int i=0; i< mColumnCount;i++){
			for(int j = 0; j < mRowCount; j++){
				if(mTextureWidth<mTextureHeight){
					mOffsetMap[i][j] = columnOffsetMap[i][j];
				}else{
					mOffsetMap[i][j] = rowOffsetMap[i][j];
				}
			}
		}

		SVIAnimationSet* fromParentAniSet = new SVIAnimationSet(mSVIGLSurface);

		fromParentAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromParentAniSet->mDuration = mFullTimeDuration;
		fromParentAniSet->mRepeatCount = mRepeatCount;
		fromParentAniSet->mAutoReverse = mAutoReverse;
		fromParentAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		fromParentAniSet->shareAnimationInfo(true);

		addZPositionAnimation(fromParentAniSet, -0.01f, -mToSlide->getRegion().mSize.mWidth * 0.5f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		addRotationAnimation(fromParentAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3( 0.0f, 0.0f, 90.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		mFromNParentSlide->addAnimation(fromParentAniSet);


		SVIAnimationSet* toParentAniSet = new SVIAnimationSet(mSVIGLSurface);

		toParentAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toParentAniSet->mDuration = mFullTimeDuration;
		toParentAniSet->mRepeatCount = mRepeatCount;
		toParentAniSet->mAutoReverse = mAutoReverse;
		toParentAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		toParentAniSet->shareAnimationInfo(true);

		addZPositionAnimation(toParentAniSet, mToSlide->getRegion().mSize.mWidth * 0.5f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);


		addRotationAnimation(toParentAniSet, SVIVector3(0.0f, 0.0f, -90.0f), SVIVector3( 0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		mToNParentSlide->addAnimation(toParentAniSet);


		setOffsetDuration(mFullTimeDuration);


		SVIFloat cornerRad = mFromNslide[0][0]->getRegion().getSize().mWidth;


		for(int i=0; i<mColumnCount; i++){
			for(int j=0; j<mRowCount; j++){

				mFromNslide[i][j]->setBorderColor(SVIColor(1.0f, 1.0f, 1.0f,1.0f));
				mToNslide[i][j]->setBorderColor(SVIColor(1.0f, 1.0f, 1.0f,1.0f));


				SVIInt offset = mOffsetDuration*mOffsetMap[i][j];
				SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);

				fromAniSet->mOffset = mGlovalOffsetDuration;
				fromAniSet->mDuration = mFullTimeDuration;

				fromAniSet->mRepeatCount = mRepeatCount;
				fromAniSet->mAutoReverse = mAutoReverse;
				fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

				addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mSlideDuration, offset);

				

				SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);

				toAniSet->mOffset = mGlovalOffsetDuration;
				toAniSet->mDuration = mFullTimeDuration;

				toAniSet->mRepeatCount = mRepeatCount;
				toAniSet->mAutoReverse = mAutoReverse;
				toAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
				

				addOpacityAnimation(toAniSet, 0.0f, 1.0f, mSlideDuration, offset);



				SVIKeyFrameAnimation* cornerFromAni= SVIKeyFrameAnimation::create(SVIPropertyAnimation::CORNER_RADIUS,mSVIGLSurface);
				cornerFromAni->mDuration = mFullTimeDuration;

				cornerFromAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
				cornerFromAni->addKeyProperty(SVIKeyFrameProperty(0.2f, cornerRad));
				cornerFromAni->addKeyProperty(SVIKeyFrameProperty(0.8f, cornerRad));
				cornerFromAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));

				fromAniSet->addAnimation(cornerFromAni);

				SVIKeyFrameAnimation* cornerToAni= SVIKeyFrameAnimation::create(SVIPropertyAnimation::CORNER_RADIUS,mSVIGLSurface);
				cornerToAni->mDuration = mFullTimeDuration;

				cornerToAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
				cornerToAni->addKeyProperty(SVIKeyFrameProperty(0.2f, cornerRad));
				cornerToAni->addKeyProperty(SVIKeyFrameProperty(0.8f, cornerRad));
				cornerToAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));

				toAniSet->addAnimation(cornerToAni);



				mFromNslide[i][j]->addAnimation(fromAniSet);
				mToNslide[i][j]->addAnimation((toAniSet));

			}
		}

		SVI_SVIFE_DELETE_ARRAY(mOffsetMap);

	}

	void SVIHoneyCombEffector::setOffsetDuration(SVIInt fullTimeDuration){
		fullTimeDuration = mFullTimeDuration;
		mOffsetDuration = fullTimeDuration*0.20f;
		mSlideDuration = fullTimeDuration - (mOffsetDuration*4);
	}



}
