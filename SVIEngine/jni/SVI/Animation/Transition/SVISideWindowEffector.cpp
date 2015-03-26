#include "SVISideWindowEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVISideWindowEffector::SVISideWindowEffector(SVIGLSurface *surface) :
	SVITransitionEffector(surface),
	rowCount(2),
	columnCount(1),
	rotationAngle(150.0f){
			mRowCount = rowCount;
			mColumnCount =columnCount;
	}

	void SVISideWindowEffector::setAnimation(){

		SVIInt mValue = 1;
		setOffsetDuration(mFullTimeDuration);

		
		for(SVIInt i=0; i<mRowCount; i++){

			if(i==0){
				mFromNslide[0][i]->setPivotPoint(SVIPoint(0.0f, 0.5f));
				mToNslide[0][i]->setPivotPoint(SVIPoint(1.0f, 0.5f));
				mValue = -1;
			}else{
				mFromNslide[0][i]->setPivotPoint(SVIPoint(1.0f, 0.5f));
				mToNslide[0][i]->setPivotPoint(SVIPoint(0.0f, 0.5f));
				mValue = 1;
			}
			
			SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
			fromAniSet->mOffset = mGlovalOffsetDuration;
			fromAniSet->mDuration = mFullTimeDuration;
			fromAniSet->mRepeatCount = mRepeatCount;
			fromAniSet->mAutoReverse = mAutoReverse;
			fromAniSet->setInterpolator(SVIAnimation::ACCELERATE);
			fromAniSet->shareAnimationInfo(true);
			
			addRotationAnimation(fromAniSet,SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, mValue*rotationAngle, 0.0f), mFullTimeDuration, mGlovalOffsetDuration);
			addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mFullTimeDuration, mGlovalOffsetDuration);
			mFromNslide[0][i]->addAnimation(fromAniSet);

			SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
			toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
			toAniSet->mDuration = mFullTimeDuration;
			toAniSet->mRepeatCount = mRepeatCount;
			toAniSet->mAutoReverse = mAutoReverse;
			toAniSet->setInterpolator(SVIAnimation::ACCELERATE);
			toAniSet->shareAnimationInfo(true);

			addZPositionAnimation(toAniSet, (mToSlide->getRegion().mSize.mWidth + mToSlide->getRegion().mSize.mHeight) * 0.25f, -0.01f, mFullTimeDuration, mGlovalOffsetDuration);
			addOpacityAnimation(toAniSet, 0.5f, 1.0f, mFullTimeDuration, mGlovalOffsetDuration);

			mToNslide[0][i]->addAnimation((toAniSet));

		}

	}

	void SVISideWindowEffector::setOffsetDuration(SVIInt fullTimeDuration){
		fullTimeDuration = mFullTimeDuration;
		mOffsetDuration = fullTimeDuration*0.1f;
		mSlideDuration = fullTimeDuration - mOffsetDuration;
	}



}