#include "SVIFoldingScreenEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"
#include <math.h>

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIFoldingScreenEffector::SVIFoldingScreenEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rowCount(2),
	columnCount(1),
	rotationAngle(90.0f){
			mRowCount = rowCount;
			mColumnCount =columnCount;
	}

	void SVIFoldingScreenEffector::setAnimation(){

        SVIPoint originPoint = mFromSlide->getPosition();

        SVIFloat fromSlideWidth = mFromSlide->getRegion().getSize().mWidth;
		SVIFloat fromSlideHeight = mFromSlide->getRegion().getSize().mHeight;

        SVIFloat slideWidth = mSizeSlide->getRegion().getSize().mWidth / mRowCount;
		SVIFloat slideHeight = mSizeSlide->getRegion().getSize().mHeight / mColumnCount;

        SVIColor whiteColor = SVIColor(1.0f, 1.0f, 1.0f, 1.0f);
        SVIColor grayColor = SVIColor(0.5f, 0.5f, 0.5f, 1.0f);
        SVIColor blackColor = SVIColor(0.0f, 0.0f, 0.0f, 1.0f);

		SVIInt mValue = 1;
		setOffsetDuration(mFullTimeDuration);
		
		for(SVIInt i=0; i<mRowCount; i++){
            SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		    fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		    fromAniSet->mDuration = mFullTimeDuration;
		    fromAniSet->mRepeatCount = mRepeatCount;
		    fromAniSet->mAutoReverse = mAutoReverse;
		    fromAniSet->setInterpolator(SVIAnimation::DECELERATE);
			fromAniSet->shareAnimationInfo(true);

            SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
            toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		    toAniSet->mDuration = mFullTimeDuration;
		    toAniSet->mRepeatCount = mRepeatCount;
		    toAniSet->mAutoReverse = mAutoReverse;
		    toAniSet->setInterpolator(SVIAnimation::DECELERATE);
			toAniSet->shareAnimationInfo(true);

			if (i==0) {
				mFromNslide[0][i]->setPivotPoint(SVIPoint(0.0f, 0.5f));
				mToNslide[0][i]->setPivotPoint(SVIPoint(0.0f, 0.5f));
				mValue = -1;

                // from left
                addPosAni(fromAniSet, true, fromSlideWidth, mValue*rotationAngle, mFromNslide[0][i]->getPosition().mX, mFromNslide[0][i]->getPosition().mY);
                addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, mValue*rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
                addColorAnimation(fromAniSet, whiteColor, grayColor, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

                // to left
                addPositionAnimation(toAniSet, 
                    SVIPoint(mToNslide[0][i]->getPosition().mX+fromSlideWidth, mToNslide[0][i]->getPosition().mY), 
                    SVIPoint(mToNslide[0][i]->getPosition().mX, mToNslide[0][i]->getPosition().mY), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
                addRotationAnimation(toAniSet, SVIVector3(0.0f, mValue*rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
                addColorAnimation(toAniSet, blackColor, whiteColor, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			} else if (i==1) {
				mFromNslide[0][i]->setPivotPoint(SVIPoint(1.0f, 0.5f));
				mToNslide[0][i]->setPivotPoint(SVIPoint(1.0f, 0.5f));
				mValue = 1;

                // from right
                addPositionAnimation(fromAniSet, 
                    SVIPoint(mFromNslide[0][i]->getPosition().mX, mFromNslide[0][i]->getPosition().mY), 
                    SVIPoint(mFromNslide[0][i]->getPosition().mX-fromSlideWidth, mFromNslide[0][i]->getPosition().mY), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
                addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, mValue*rotationAngle, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
                addColorAnimation(fromAniSet, whiteColor, blackColor, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
                
                // to right
                addPosAni(toAniSet, false, fromSlideWidth, mValue*rotationAngle, mToNslide[0][i]->getPosition().mX, mToNslide[0][i]->getPosition().mY);
                addRotationAnimation(toAniSet, SVIVector3(0.0f, mValue*rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
                addColorAnimation(toAniSet, grayColor, whiteColor, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			}
			
            // z postion
            addZPositionAnimation(fromAniSet, -0.01f, 0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);
			addZPositionAnimation(toAniSet, 0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

            // add to slide
            mFromNslide[0][i]->addAnimation(fromAniSet);
			mToNslide[0][i]->addAnimation((toAniSet));
		}
	}

	void SVIFoldingScreenEffector::setOffsetDuration(SVIInt fullTimeDuration){
		fullTimeDuration = mFullTimeDuration;
		mOffsetDuration = fullTimeDuration*0.1f;
		mSlideDuration = fullTimeDuration - mOffsetDuration;
	}

    void SVIFoldingScreenEffector::addPosAni(SVIAnimationSet* aniSet, bool isFrom, SVIFloat fromSlideWidth, SVIFloat angle, SVIFloat positionX, SVIFloat positionY){
        SVIFloat deltaX;

        SVIKeyFrameAnimation* positionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		positionAni->mDuration = aniSet->mDuration;
		positionAni->mOffset = aniSet->mOffset;

        for(int i=0; i<=90; i++){
            if(isFrom == true){
                deltaX = fromSlideWidth/2 * cos(angle*(i/90.0f)*(3.1416f/180));
                positionAni->addKeyProperty(SVIKeyFrameProperty(1.0f*i/90.0f, SVIPoint(positionX + fromSlideWidth*((90-i)/90.0f) - deltaX*2.0f, positionY)));
            } else {
                deltaX = fromSlideWidth/2 * cos(angle*((90-i)/90.0f)*(3.1416f/180));
                positionAni->addKeyProperty(SVIKeyFrameProperty(1.0f*i/90.0f, SVIPoint(positionX - fromSlideWidth*(i/90.0f) + deltaX*2.0f, positionY)));
            }
        }

		aniSet->addAnimation(positionAni);
	}
}