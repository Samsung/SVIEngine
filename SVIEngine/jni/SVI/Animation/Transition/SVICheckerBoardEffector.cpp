#include "SVICheckerBoardEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVICheckerBoardEffector::SVICheckerBoardEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rowCount(8),
		columnCount(5),
		rotationAngle(180.0f){
			mRowCount = rowCount;
			mColumnCount =columnCount;
	}

	void SVICheckerBoardEffector::setAnimation(){

		setOffsetDuration(mFullTimeDuration);

		SVIInt **mOffsetMap;


		mOffsetMap = new SVIInt *[mColumnCount];
		for(int i =0; i<mColumnCount; i++){
			mOffsetMap[i] = new SVIInt[mRowCount];
		}

		SVIInt rightOffsetMap[5][8] =  
		{
			{0, 3, 1, 3, 4, 6, 5, 7},
			{1, 2, 2, 4, 5, 7, 8, 6},
			{0, 1, 3, 5, 6, 5, 6, 8},
			{1, 2, 4, 6, 4, 6, 7, 6},
			{0, 4, 2, 4, 5, 7, 5, 8}
		};

		SVIInt leftOffsetMap[5][8] =  
		{
			{7, 5, 6, 4, 3, 1, 3, 0},
			{6, 8, 7, 5, 4, 2, 2, 1},
			{8, 6, 5, 6, 5, 3, 1, 0},
			{6, 7, 6, 4, 6, 4, 2, 1},
			{8, 5, 7, 5, 4, 2, 4, 0}
		};

		SVIInt downOffsetMap[8][5] =
		{
			{0, 1, 0, 1, 0},
			{3, 2, 1, 2, 4},
			{1, 2, 3, 4, 2},
			{3, 4, 5, 6, 4},
			{4, 5, 6, 4, 5},
			{6, 7, 5, 6, 7},
			{5, 8, 6, 7, 5},
			{7, 6, 8, 6, 8}
		};

		SVIInt upOffsetMap[8][5] = 
		{
			{7, 6, 8, 6, 8},
			{5, 8, 6, 7, 5},
			{6, 7, 5, 6, 7},
			{4, 5, 6, 4, 5},
			{3, 4, 5, 6, 4},
			{1, 2, 3, 4, 2},
			{3, 2, 1, 2, 4},
			{0, 1, 0, 1, 0}
		};



		for(int i=0; i< mColumnCount;i++){
			for(int j = 0; j < mRowCount; j++){
				if(mDirectionType == SVITransitionAnimation::LEFT){
					mOffsetMap[i][j] = leftOffsetMap[i][j];
				}else if(mDirectionType == SVITransitionAnimation::RIGHT){
					mOffsetMap[i][j] = rightOffsetMap[i][j];
				}else if(mDirectionType == SVITransitionAnimation::DOWN){
					mOffsetMap[i][j] = downOffsetMap[i][j];
				}else if(mDirectionType == SVITransitionAnimation::UP){
					mOffsetMap[i][j] = upOffsetMap[i][j];
				}
			}
		}

		for(SVIInt i=0; i<mColumnCount; i++){
			for(SVIInt j=0; j<mRowCount; j++){

				SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
				fromAniSet->mDuration = mFullTimeDuration;
				fromAniSet->mRepeatCount = mRepeatCount;
				fromAniSet->mAutoReverse = mAutoReverse;
				fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

				SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
				toAniSet->mDuration = mFullTimeDuration;
				toAniSet->mRepeatCount = mRepeatCount;
				toAniSet->mAutoReverse = mAutoReverse;
				toAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
				
				SVIInt fromOffset = mOffsetDuration*mOffsetMap[i][j] + mGlovalOffsetDuration;
				SVIInt toOffset = mOffsetDuration*mOffsetMap[i][j] + mGlovalOffsetDuration;

				switch(mDirectionType){

				case SVITransitionAnimation::LEFT:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mSlideDuration, fromOffset);
					addRotationAnimation(toAniSet, SVIVector3(0.0f, -rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, toOffset);
					break;

				case SVITransitionAnimation::RIGHT:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mSlideDuration, fromOffset);
					addRotationAnimation(toAniSet, SVIVector3(0.0f, rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, toOffset);
					break;

				case SVITransitionAnimation::UP:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mSlideDuration, fromOffset);
					addRotationAnimation(toAniSet, SVIVector3(rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, toOffset);
					break;

				case SVITransitionAnimation::DOWN:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mSlideDuration, fromOffset);
					addRotationAnimation(toAniSet, SVIVector3(-rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, toOffset);
					break;
				}

				
				addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mSlideDuration, fromOffset);
				addZPositionAnimation(toAniSet, 0.01f, -0.01f, mSlideDuration, toOffset);

				mFromNslide[i][j]->addAnimation(fromAniSet);
				mToNslide[i][j]->addAnimation((toAniSet));

			}
		}

		SVI_SVIFE_DELETE_ARRAY(mOffsetMap);
	}

	void SVICheckerBoardEffector::setOffsetDuration(SVIInt fullTimeDuration){
		fullTimeDuration = mFullTimeDuration;
		mOffsetDuration = fullTimeDuration * 0.05f;

		mSlideDuration = fullTimeDuration - (mOffsetDuration * 8);
	}


}
