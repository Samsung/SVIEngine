#include "SVIBoxEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIBoxEffector::SVIBoxEffector(SVIGLSurface *surface):SVITransitionEffector(surface){
	}

	void SVIBoxEffector::setAnimation(){
        SVIPoint originPoint = mFromSlide->getPosition();

        slideWidth = mFromSlide->getRegion().getSize().mWidth;
		slideHeight = mFromSlide->getRegion().getSize().mHeight;


        SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);

        SVIFloat fromPointX = originPoint.mX - (mTextureWidth * 1.1f);
        SVIFloat toPointX = originPoint.mX + (mTextureWidth * 1.1f);        
        SVIFloat rotAngle = 90.0f;

        switch(mDirectionType){
        case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
            mFromNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));
		    mToNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.5f));
            fromPointX = mFromNslide[0][0]->getPosition().mX-slideWidth;
            toPointX = mToNslide[0][0]->getPosition().mX+slideWidth;
            rotAngle = 90.0f;
            break;

        case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
            mFromNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.5f));
		    mToNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));
            fromPointX = mFromNslide[0][0]->getPosition().mX+slideWidth;
            toPointX = mToNslide[0][0]->getPosition().mX-slideWidth;
            rotAngle = -90.0f;
            break;
		default:
			mFromNslide[0][0]->setPivotPoint(SVIPoint(0.0f, 0.5f));
			mToNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));
			fromPointX = mFromNslide[0][0]->getPosition().mX+slideWidth;
			toPointX = mToNslide[0][0]->getPosition().mX-slideWidth;
			rotAngle = -90.0f;
			break;
        }


        // position
        addPositionAnimation(fromAniSet, mFromNslide[0][0]->getPosition(), SVIPoint(fromPointX, mFromNslide[0][0]->getPosition().mY), mFullTimeDuration, fromAniSet->mOffset);
        addPositionAnimation(toAniSet, SVIPoint(toPointX, mToNslide[0][0]->getPosition().mY), mToNslide[0][0]->getPosition(), mFullTimeDuration, toAniSet->mOffset);


        // opacity
		SVIKeyFrameAnimation* fromOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromOpaAni->mDuration = fromAniSet->mDuration;
		fromOpaAni->mOffset = fromAniSet->mOffset;
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.7f, 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
		fromAniSet->addAnimation(fromOpaAni);

        SVIKeyFrameAnimation* toOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toOpaAni->mDuration = toAniSet->mDuration;
		toOpaAni->mOffset = toAniSet->mOffset;
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.3f, 1.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
		toAniSet->addAnimation(toOpaAni);


        // roation
        addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -1*rotAngle, 0.0f), mFullTimeDuration, fromAniSet->mOffset);
        addRotationAnimation(toAniSet, SVIVector3(0.0f, rotAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), mFullTimeDuration, toAniSet->mOffset);


        // z position        
        SVIFloat zPosition = (slideWidth*0.5f)*abs(sin(rotAngle*3.1416/180));
        addZPosAni(fromAniSet, zPosition, mFullTimeDuration, fromAniSet->mOffset);
        addZPosAni(toAniSet, zPosition, mFullTimeDuration, toAniSet->mOffset);


        // add to slide
        mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}    

    void SVIBoxEffector::addZPosAni(SVIAnimationSet* aniSet, SVIFloat zPosition, SVIInt duration, SVIInt offset){
        SVIFloat time[3] = {0.0f, 0.5f, 1.0f};

        SVIKeyFrameAnimation* zPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		zPosAni->mDuration = duration;
		zPosAni->mOffset = offset;
		zPosAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
        zPosAni->addKeyProperty(SVIKeyFrameProperty(time[1], zPosition));
		zPosAni->addKeyProperty(SVIKeyFrameProperty(time[2], 0.0f));
		aniSet->addAnimation(zPosAni);
    }    
}
