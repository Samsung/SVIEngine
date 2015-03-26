#include "SVIBlindEffector.h"
#include "SVITransitionEffector.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIBlindEffector::SVIBlindEffector(SVIGLSurface *surface) :
	SVITransitionEffector(surface),
	rowCount(15),
		columnCount(9){
			mRowCount = rowCount;
			mColumnCount =columnCount;
	}

	void SVIBlindEffector::setAnimation(){


		for(SVIInt i=0; i<mColumnCount; i++){
			for(SVIInt j=0; j<mRowCount; j++){

				SVIRect  originRect = mFromNslide[i][j]->getRegion();
				SVIRect	originTexRect = mFromNslide[i][j]->getTextureRegion();
				SVIPoint originPosition = mFromNslide[i][j]->getPosition();

				slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
				slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

				SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
				fromAniSet->mDuration = mFullTimeDuration;
				fromAniSet->mOffset = mOffsetDuration;
				fromAniSet->mRepeatCount = mRepeatCount;
				fromAniSet->setInterpolator(SVIAnimation::LINEAR);

				switch(mDirectionType){

				case SVITransitionAnimation::LEFT:
					
					addRegionAnimation(fromAniSet, originRect, SVIRect(originRect.getOrigin().mX,originRect.getOrigin().mY, 0.0f, originRect.getSize().mHeight), mFullTimeDuration, mOffsetDuration);
					addTextureRegionAnimation(fromAniSet, originTexRect,SVIRect(originTexRect.getOrigin().mX, originTexRect.getOrigin().mY, 0.0f, 1.0f), mFullTimeDuration, mOffsetDuration);
					break;

				case SVITransitionAnimation::RIGHT:
					addRegionAnimation(fromAniSet, originRect, SVIRect(originRect.getOrigin().mX + slideWidth,originRect.getOrigin().mY, 0.0f, originRect.getSize().mHeight), mFullTimeDuration, mOffsetDuration);
					addTextureRegionAnimation(fromAniSet, originTexRect,SVIRect(originTexRect.getOrigin().mX + (1.0/mRowCount), originTexRect.getOrigin().mY, 0.0f, 1.0f), mFullTimeDuration, mOffsetDuration);
					break;

				case SVITransitionAnimation::UP:
					addRegionAnimation(fromAniSet, originRect, SVIRect(originRect.getOrigin().mX,originRect.getOrigin().mY, originRect.mSize.mWidth, 0.0f), mFullTimeDuration, mOffsetDuration);
					addTextureRegionAnimation(fromAniSet, originTexRect,SVIRect(originTexRect.getOrigin().mX, originTexRect.getOrigin().mY, 1.0f, 0.0f), mFullTimeDuration, mOffsetDuration);
					break;

				case SVITransitionAnimation::DOWN:
					addRegionAnimation(fromAniSet, originRect, SVIRect(originRect.getOrigin().mX,originRect.getOrigin().mY +slideHeight, originRect.getSize().mWidth, 0.0f), mFullTimeDuration, mOffsetDuration);
					addTextureRegionAnimation(fromAniSet, originTexRect,SVIRect(originTexRect.getOrigin().mX, originTexRect.getOrigin().mY + (1.0/mColumnCount), 1.0f, 0.0f), mFullTimeDuration, mOffsetDuration);
					break;

				}
				
				mFromNslide[i][j]->addAnimation(fromAniSet);

			}
		}

	}
}
