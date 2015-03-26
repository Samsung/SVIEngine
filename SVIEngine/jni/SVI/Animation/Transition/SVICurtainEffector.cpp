#include "SVICurtainEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVICurtainEffector::SVICurtainEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface),
	rowCount(15),
		columnCount(9),
		rotationAngle(120.0f){
			mRowCount = rowCount;
			mColumnCount =columnCount;
	}

	void SVICurtainEffector::setAnimation(){

		setOffsetDuration(mFullTimeDuration);

		SVIInt slideCount;

		for(SVIInt i=0; i<mColumnCount; i++){
			for(SVIInt j=0; j<mRowCount; j++){


				SVIPoint originPoint = mFromNslide[i][j]->getPosition();

				slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount) * 0.5f;
				slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount) * 0.5f;

				switch(mDirectionType){
				case SVITransitionAnimation::LEFT:
					slideCount = mRowCount - 1 - j;
					break;

				case SVITransitionAnimation::RIGHT:
					slideCount = j;
					break;

				case SVITransitionAnimation::UP:
					slideCount = mColumnCount - 1 - i;
					break;

				case SVITransitionAnimation::DOWN:
					slideCount = i;
					break;

				}


				SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
				fromAniSet->mOffset = mGlovalOffsetDuration;
				fromAniSet->mDuration = mFullTimeDuration;
				fromAniSet->mRepeatCount = mRepeatCount;
				fromAniSet->mAutoReverse = mAutoReverse;
				fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

				SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
				toAniSet->mOffset =mGlovalOffsetDuration;
				toAniSet->mDuration = mFullTimeDuration;
				toAniSet->mRepeatCount = mRepeatCount;
				toAniSet->mAutoReverse = mAutoReverse;
				toAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

				SVIInt offset = slideCount*offsetDuration + mGlovalOffsetDuration;



				switch(mDirectionType){


				case SVITransitionAnimation::LEFT:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(1.0f, 0.5f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(0.0f, 0.5f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, rotationAngle, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(fromAniSet,SVIPoint(originPoint.mX + slideWidth, originPoint.mY), SVIPoint(originPoint.mX - slideWidth, originPoint.mY), mSlideDuration, mOffsetDuration);

					addRotationAnimation(toAniSet, SVIVector3(0.0f, -rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX + slideWidth, originPoint.mY), SVIPoint(originPoint.mX - slideWidth, originPoint.mY), mSlideDuration, mOffsetDuration);
					break;

				case SVITransitionAnimation::RIGHT:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(0.0f, 0.5f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(1.0f, 0.5f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX - slideWidth, originPoint.mY), SVIPoint(originPoint.mX + slideWidth, originPoint.mY), mSlideDuration, mOffsetDuration);

					addRotationAnimation(toAniSet, SVIVector3(0.0f, rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX - slideWidth, originPoint.mY), SVIPoint(originPoint.mX + slideWidth, originPoint.mY), mSlideDuration, mOffsetDuration);
					break;

				case SVITransitionAnimation::UP:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 1.0f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 0.0f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(-rotationAngle, 0.0f, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX, originPoint.mY + slideHeight), SVIPoint(originPoint.mX, originPoint.mY - slideHeight), mSlideDuration, mOffsetDuration);

					addRotationAnimation(toAniSet, SVIVector3(rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX , originPoint.mY + slideHeight), SVIPoint(originPoint.mX , originPoint.mY - slideHeight), mSlideDuration, mOffsetDuration);
					break;

				case SVITransitionAnimation::DOWN:
					mFromNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 0.0f));
					mToNslide[i][j]->setPivotPoint(SVIPoint(0.5f, 1.0f));

					addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(rotationAngle, 0.0f, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(fromAniSet, SVIPoint(originPoint.mX, originPoint.mY - slideHeight), SVIPoint(originPoint.mX, originPoint.mY + slideHeight), mSlideDuration, mOffsetDuration);

					addRotationAnimation(toAniSet, SVIVector3(-rotationAngle, 0.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mSlideDuration, mOffsetDuration);
					addPositionAnimation(toAniSet, SVIPoint(originPoint.mX , originPoint.mY - slideHeight), SVIPoint(originPoint.mX , originPoint.mY + slideHeight), mSlideDuration, mOffsetDuration);
					break;


				}


				addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mSlideDuration, mOffsetDuration);
				addZPositionAnimation(toAniSet, 0.01f, -0.01f, mSlideDuration, mOffsetDuration);

				mFromNslide[i][j]->addAnimation(fromAniSet);

				mToNslide[i][j]->addAnimation((toAniSet));

			}
		}

	}

	void SVICurtainEffector::setOffsetDuration(SVIInt fullTimeDuration){
		offsetDuration = fullTimeDuration*0.03f;
		SVIInt tempCount = mRowCount;
		if(mRowCount<mColumnCount){
			tempCount = mColumnCount;
		}
		mSlideDuration = fullTimeDuration - (offsetDuration*tempCount);
	}
}
