#include "SVICenterBlindEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVICenterBlindEffector::SVICenterBlindEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rowCount(15),
		columnCount(9),
		rotationAngle(120.0f){
			mRowCount = rowCount;
			mColumnCount =columnCount;
	}

	void SVICenterBlindEffector::setAnimation(){

		setOffsetDuration(mFullTimeDuration);

		SVIInt offset;

		SVIInt leftOffset[15] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5};
		SVIInt rightOffset[15] = {5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		SVIInt upOffset[9] = {5, 4, 3, 2, 1, 0, 1, 2, 3};
		SVIInt downOffset[9] = {3, 2, 1, 0, 1, 2, 3, 4, 5};

		for(SVIInt i=0; i<mColumnCount; i++){
			for(SVIInt j=0; j<mRowCount; j++){


				SVIPoint originPoint = mFromNslide[i][j]->getPosition();

				slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount) * 0.5f;
				slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount) * 0.5f;

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

				switch(mDirectionType){
				case SVITransitionAnimation::LEFT:
					offset = leftOffset[j] * mOffsetDuration;
					break;

				case SVITransitionAnimation::RIGHT:
					offset = rightOffset[j] * mOffsetDuration;
					break;

				case SVITransitionAnimation::UP:
					offset = upOffset[i] * mOffsetDuration;
					break;

				case SVITransitionAnimation::DOWN:
					offset = downOffset[i] * mOffsetDuration;
					break;


				}

				switch(mDirectionType){


				case SVITransitionAnimation::LEFT:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(1.0f, 0.5f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(0.0f, 0.5f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mSlideDuration, offset);
					addPositionAnimation(fromAniSet,SVIPoint(originPoint.mX + slideWidth, originPoint.mY), SVIPoint(originPoint.mX - slideWidth, originPoint.mY), mSlideDuration, offset);

					addRotationAnimation(toAniSet, SVIVector3(0.0f, -rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX + slideWidth, originPoint.mY), SVIPoint(originPoint.mX - slideWidth, originPoint.mY), mSlideDuration, offset);
					break;

				case SVITransitionAnimation::RIGHT:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(0.0f, 0.5f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(1.0f, 0.5f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mSlideDuration, offset);
					addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX - slideWidth, originPoint.mY), SVIPoint(originPoint.mX + slideWidth, originPoint.mY), mSlideDuration, offset);

					addRotationAnimation(toAniSet, SVIVector3(0.0f, rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX - slideWidth, originPoint.mY), SVIPoint(originPoint.mX + slideWidth, originPoint.mY), mSlideDuration, offset);
					break;

				case SVITransitionAnimation::UP:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 1.0f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 0.0f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mSlideDuration, offset);
					addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX, originPoint.mY + slideHeight), SVIPoint(originPoint.mX, originPoint.mY - slideHeight), mSlideDuration, offset);

					addRotationAnimation(toAniSet, SVIVector3(rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX , originPoint.mY + slideHeight), SVIPoint(originPoint.mX , originPoint.mY - slideHeight), mSlideDuration, offset);
					break;

				case SVITransitionAnimation::DOWN:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 0.0f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 1.0f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mSlideDuration, offset);
					addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX, originPoint.mY - slideHeight), SVIPoint(originPoint.mX, originPoint.mY + slideHeight), mSlideDuration, offset);

					addRotationAnimation(toAniSet, SVIVector3(-rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, offset);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX , originPoint.mY - slideHeight), SVIPoint(originPoint.mX , originPoint.mY + slideHeight), mSlideDuration, offset);
					break;


				}


				addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mSlideDuration, offset);
				addZPositionAnimation(toAniSet, 0.01f, -0.01f, mSlideDuration, offset);

				mFromNslide[i][j]->addAnimation(fromAniSet);

				mToNslide[i][j]->addAnimation((toAniSet));

			}
		}

	}

	void SVICenterBlindEffector::setOffsetDuration(SVIInt fullTimeDuration){
		mFullTimeDuration = fullTimeDuration;
		mOffsetDuration = fullTimeDuration*0.03f;
		SVIInt tempCount = 15;
		if(mRowCount<mColumnCount){
			mOffsetDuration = fullTimeDuration * 0.05f;
			tempCount = 9;
		}
		mSlideDuration = fullTimeDuration - (mOffsetDuration*tempCount);
	}
}