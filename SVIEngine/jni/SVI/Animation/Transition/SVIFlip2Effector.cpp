#include "SVIFlip2Effector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIFlip2Effector::SVIFlip2Effector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rotationAngle(-180.0f),
    slideWidth(0),
    slideHeight(0){
        mIsUseParent = SVITRUE;
	}

	void SVIFlip2Effector::setAnimation(){

		SVIVector3 fromRotationValue;
		SVIVector3 toRotationValue;

        SVIAnimationSet* fromParentAni = new SVIAnimationSet(mSVIGLSurface);
		fromParentAni->mDuration = mFullTimeDuration;
		fromParentAni->mRepeatCount = mRepeatCount;
		fromParentAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromParentAni->mAutoReverse = mAutoReverse;
		fromParentAni->setInterpolator(SVIAnimation::ACCELERATE);
		fromParentAni->shareAnimationInfo(true);

		SVIAnimationSet* toParentAni = new SVIAnimationSet(mSVIGLSurface);
		toParentAni->mDuration = mFullTimeDuration;
		toParentAni->mRepeatCount = mRepeatCount;
		toParentAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toParentAni->mAutoReverse = mAutoReverse;
		toParentAni->setInterpolator(SVIAnimation::ACCELERATE);
		toParentAni->shareAnimationInfo(true);

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::ACCELERATE);
		fromAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::ACCELERATE);
		toAniSet->shareAnimationInfo(true);


        SVIPoint originPoint = mFromSlide->getPosition();

        slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
		slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

        fromRotationValue = SVIVector3(rotationAngle, 0.0f, 0.0f);
        toRotationValue = SVIVector3(-rotationAngle, 0.0f, 0.0f);

        SVIPoint parentPivotPoint = SVIPoint(1.0f, 0.5f);
        SVIFloat parentRotValue = 45.0f;


        switch(mDirectionType){
		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
            parentPivotPoint = SVIPoint(1.0f, 0.5f);
            parentRotValue = 45.0f;
            break;

		case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
            parentPivotPoint = SVIPoint(0.0f, 0.5f);
            parentRotValue = -45.0f;
            break;
        }


        // position
        SVIKeyFrameAnimation* fromPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		fromPosAni->mDuration = fromAniSet->mDuration;
		fromPosAni->mOffset = fromAniSet->mOffset;
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, originPoint));
        fromPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(originPoint.mX, originPoint.mY-slideHeight*0.025)));
        fromPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, originPoint));
		fromAniSet->addAnimation(fromPosAni);

        SVIKeyFrameAnimation* toPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		toPosAni->mDuration = fromAniSet->mDuration;
		toPosAni->mOffset = fromAniSet->mOffset;
		toPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, originPoint));
        toPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(originPoint.mX, originPoint.mY+slideHeight*0.025)));
        toPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, originPoint));
		toAniSet->addAnimation(toPosAni);
        

        // parent rotate
        mFromNParentSlide->setPivotPoint(parentPivotPoint);
        mToNParentSlide->setPivotPoint(parentPivotPoint);
        addParentRotAni(fromParentAni, parentRotValue);
        addParentRotAni(toParentAni, parentRotValue);


        // rotation              
		addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), fromRotationValue, fromAniSet->mDuration, fromAniSet->mOffset);
		addRotationAnimation(toAniSet, toRotationValue, SVIVector3(0.0f, 0.0f, 0.0f), toAniSet->mDuration, toAniSet->mOffset);


        // scale        
        addChildScaleAni(fromAniSet, SVIVector3(0.9f, 0.9f, 1.0f));
        addChildScaleAni(toAniSet, SVIVector3(0.9f, 0.9f, 1.0f));
        

        // z position
		addZPositionAnimation(fromParentAni, -0.01f, 0.01f, fromParentAni->mDuration, fromParentAni->mOffset);
		addZPositionAnimation(toParentAni, 0.01f, -0.01f, toParentAni->mDuration, toParentAni->mOffset);


        // add
        mFromNParentSlide->addAnimation(fromParentAni);
        mToNParentSlide->addAnimation(toParentAni);

		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}

    void SVIFlip2Effector::addChildScaleAni(SVIAnimationSet* aniSet, SVIVector3 scaleValue){

		SVIKeyFrameAnimation* scaleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SCALE,mSVIGLSurface);
		scaleAni->mDuration = aniSet->mDuration;
		scaleAni->mOffset = aniSet->mOffset;
        
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
        scaleAni->addKeyProperty(SVIKeyFrameProperty(0.5f, scaleValue));
        scaleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 1.0f, 1.0f)));

		aniSet->addAnimation(scaleAni);
	}

    void SVIFlip2Effector::addParentRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle){

		SVIKeyFrameAnimation* rotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		rotateAni->mDuration = aniSet->mDuration;
		rotateAni->mOffset = aniSet->mOffset;
        
		SVIFloat x, y, z;
		SVIQuaternion q;

		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(rotAngle);
        z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		

		aniSet->addAnimation(rotateAni);

	}
}