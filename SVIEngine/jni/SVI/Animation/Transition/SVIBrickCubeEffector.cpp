#include "SVIBrickCubeEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"
#include "../../Render/SVITexture.h"
#include "../../Render/SVIFrameRenderer.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIBrickCubeEffector::SVIBrickCubeEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rowCount(28),
	columnCount(1){
		mRowCount = rowCount;
		mColumnCount = columnCount;
	}

	void SVIBrickCubeEffector::setAnimation(){
		srand(time(NULL));

		for(int i = 0; i<mRowCount; i++){

			SVIInt offset = mFullTimeDuration * 0.05f * (rand() % 10);

			SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
			fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
			fromAniSet->mDuration = mFullTimeDuration;
			fromAniSet->mRepeatCount = mRepeatCount;
			fromAniSet->mAutoReverse = mAutoReverse;
			fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

			SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
			toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
			toAniSet->mDuration = mFullTimeDuration;
			toAniSet->mRepeatCount = mRepeatCount;
			toAniSet->mAutoReverse = mAutoReverse;
			toAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);


			addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, 90.0f, 0.0f), mFullTimeDuration * 0.5f, offset + mGlovalOffsetDuration);
			addOpacityAnimation(toAniSet, 0.0f, 0.0f, toAniSet->mDuration, toAniSet->mOffset);

			mToNslide[0][i]->addAnimation(toAniSet);
			mFromNslide[0][i]->addAnimation(fromAniSet);
		}
		
	}
}