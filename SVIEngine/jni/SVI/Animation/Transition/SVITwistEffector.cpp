#include "SVITwistEffector.h"
#include "SVITransitionEffector.h"
#include "../../Render/SVITexture.h"
#include "../../Render/SVIFrameRenderer.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVITwistEffector::SVITwistEffector(SVIGLSurface *surface) :
		SVITransitionEffector(surface){
	}

	void SVITwistEffector::setAnimation(){
		
		mSVIGLSurface->getSlideCapture()->setBackFaceCaptureSlideToSlide(mToSlide->getProjectionSlide(),mFromNslide[0][0]->getProjectionSlide());//Multi-Instance-Support
		
		SVISlideTextureContainer * pContainer = mFromNslide[0][0]->getProjectionSlide()->getTextureContainer();
		SVICaptureData * pCaptureData = pContainer != NULL ? pContainer->getBackFaceCaptureTexture() : NULL;
		
		if (pCaptureData != NULL){
			SVIVector2 & size = pCaptureData->getSize();
			SVIRect tempRect = SVIRect(0.0f, 0.0f, size.x, size.y);
			mFromNslide[0][0]->setBackFaceTextureRegion(tempRect);
		}


		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::LINEAR);
		fromAniSet->shareAnimationInfo(true);
		
		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::LINEAR);
		toAniSet->shareAnimationInfo(true);

		addDeformAnimation(fromAniSet, SVIVector3(-2.0f, 0.5f, 1.0f), SVIVector3(2.0f, 0.5f, 1.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		addOpacityAnimation(toAniSet, 0.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
		
		mToNslide[0][0]->addAnimation(toAniSet);
		mFromNslide[0][0]->addAnimation(fromAniSet);
	}
}