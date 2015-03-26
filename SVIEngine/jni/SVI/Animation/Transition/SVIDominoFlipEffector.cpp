#include "SVIDominoFlipEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIDominoFlipEffector::SVIDominoFlipEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface),
	rowCount(15),
		columnCount(9),
		rotationAngle(180.0f){
			mRowCount = rowCount;
			mColumnCount =columnCount;
	}

	void SVIDominoFlipEffector::setAnimation(){

		setOffsetDuration(mFullTimeDuration);

		SVIInt slideDuration;

		for(SVIInt i=0; i<mColumnCount; i++){
			for(SVIInt j=0; j<mRowCount; j++){

				switch(mDirectionType){

				case SVITransitionAnimation::LEFT:
					slideDuration = mRowCount + mColumnCount -2 -i -j;
					break;

				case SVITransitionAnimation::RIGHT:
					slideDuration = i + j;
					break;

				case SVITransitionAnimation::UP:
					slideDuration = mRowCount + mColumnCount -2 -i -j;
					break;

				case SVITransitionAnimation::DOWN:
					slideDuration = i+j;
					break;
				}


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


				fromAniSet->mOffset = mGlovalOffsetDuration;
				toAniSet->mOffset = mGlovalOffsetDuration;

				SVIInt offset = slideDuration*mOffsetDuration + mGlovalOffsetDuration;;


				switch(mDirectionType){

				case SVITransitionAnimation::LEFT:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mSlideDuration, offset);
					addRotationAnimation(toAniSet, SVIVector3(0.0f, -rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					break;

				case SVITransitionAnimation::RIGHT:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mSlideDuration, offset);
					addRotationAnimation(toAniSet, SVIVector3(0.0f, rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					break;

				case SVITransitionAnimation::UP:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mSlideDuration, offset);
					addRotationAnimation(toAniSet, SVIVector3(rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					break;

				case SVITransitionAnimation::DOWN:
					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mSlideDuration, offset);
					addRotationAnimation(toAniSet, SVIVector3(-rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					break;
				}

				
				addZPositionAnimation(fromAniSet, -0.1f, 0.1f, mSlideDuration, offset);
				addZPositionAnimation(toAniSet, 0.1f, -0.1f, mSlideDuration, offset);

				mFromNslide[i][j]->addAnimation(fromAniSet);
				mToNslide[i][j]->addAnimation((toAniSet));

			}
		}
	}

	void SVIDominoFlipEffector::setOffsetDuration(SVIInt fullTimeDuration){
		fullTimeDuration = mFullTimeDuration;
		mOffsetDuration = fullTimeDuration * 0.02f;
		
		mSlideDuration = fullTimeDuration - (mOffsetDuration*(mRowCount + mColumnCount));
	}


}
