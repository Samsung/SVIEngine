#include "SVISplitEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVISplitEffector::SVISplitEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rowCount(2),
	columnCount(1) {
		mRowCount = rowCount;
		mColumnCount = columnCount;
	}

	void SVISplitEffector::setAnimation(){

		for(SVIInt i=0; i<mColumnCount; i++){
			for(SVIInt j=0; j<mRowCount; j++){

				SVIRect  originRect = mFromNslide[i][j]->getRegion();
				SVIPoint originPosition = mFromNslide[i][j]->getPosition();

				slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
				slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

				mFromNslide[i][j]->setImageScaleType(MATRIX);
				mFromNslide[i][j]->setTextureScaleFitRegion(SVIRect(0.0f, 0.0f, slideWidth, slideHeight));


						
				SVIKeyFrameAnimation* fromSwipeAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SWIPE_RATIO,mSVIGLSurface);
				
				fromSwipeAni->mDuration = mFullTimeDuration;
				fromSwipeAni->mRepeatCount = mRepeatCount;
				fromSwipeAni->mAutoReverse = mAutoReverse;
				fromSwipeAni->mOffset = mGlovalOffsetDuration;
				
				if(j == 0)	//left
				{
					fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 0.0f, 1.0f)));
					fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(-0.5f, 0.3f, 1.0f)));
				}
				else if(j == 1)	//right
				{
					fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.0f, 0.0f)));
					fromSwipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.2f, 0.3f, 0.0f)));
				}
				
				
				mFromNslide[i][j]->addAnimation(fromSwipeAni);
			}
		}
	}
}