#include "SVISwitchEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVISwitchEffector::SVISwitchEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rotationAngle(30.0f),
    slideWidth(0),
    slideHeight(0){
        mIsUseParent = SVITRUE;
	}

	void SVISwitchEffector::setAnimation(){

		SVIVector3 fromRotationValue;
		SVIVector3 toRotationValue;

        SVIAnimationSet* fromParentAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromParentAniSet->mDuration = mFullTimeDuration;
		fromParentAniSet->mRepeatCount = mRepeatCount;
		fromParentAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromParentAniSet->mAutoReverse = mAutoReverse;
		fromParentAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		fromParentAniSet->shareAnimationInfo(true);

        SVIAnimationSet* toParentAniSet = new SVIAnimationSet(mSVIGLSurface);
		toParentAniSet->mDuration = mFullTimeDuration;
		toParentAniSet->mRepeatCount = mRepeatCount;
		toParentAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toParentAniSet->mAutoReverse = mAutoReverse;
		toParentAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		toParentAniSet->shareAnimationInfo(true);

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		fromAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		toAniSet->shareAnimationInfo(true);

        SVIPoint originPoint = mFromSlide->getPosition();

        slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
		slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

        fromRotationValue = SVIVector3(0.0f, 0.0f, 0.0f);
        toRotationValue = SVIVector3(rotationAngle, 0.0f, -rotationAngle);
        
        SVIFloat parentChangePointX = originPoint.mX + slideWidth*0.1;
        SVIFloat parentRotValue = 15.0f;


        switch(mDirectionType){
		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
            parentChangePointX = originPoint.mX - slideWidth*0.1;
            parentRotValue = -15.0f;
            break;

		case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
            parentChangePointX = originPoint.mX + slideWidth*0.1;
            parentRotValue = 15.0f;
            break;
        }


        // POSITION
        SVIKeyFrameAnimation* fromParentPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		fromParentPosAni->mDuration = fromParentAniSet->mDuration;
		fromParentPosAni->mRepeatCount = fromParentAniSet->mRepeatCount;
		fromParentPosAni->mAutoReverse = fromParentAniSet->mAutoReverse;
		fromParentPosAni->mOffset = fromParentAniSet->mOffset;
        fromParentPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, originPoint));
        fromParentPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(parentChangePointX, originPoint.mY)));
		fromParentPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, originPoint));
		fromParentAniSet->addAnimation(fromParentPosAni);

        SVIKeyFrameAnimation* toParentPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		toParentPosAni->mDuration = fromParentAniSet->mDuration;
		toParentPosAni->mRepeatCount = fromParentAniSet->mRepeatCount;
		toParentPosAni->mAutoReverse = fromParentAniSet->mAutoReverse;
		toParentPosAni->mOffset = fromParentAniSet->mOffset;
        toParentPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, originPoint));
        toParentPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(parentChangePointX, originPoint.mY)));
		toParentPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, originPoint));
        toParentAniSet->addAnimation(toParentPosAni);


        SVIKeyFrameAnimation* fromPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		fromPosAni->mDuration = fromAniSet->mDuration;
		fromPosAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromPosAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromPosAni->mOffset = fromAniSet->mOffset;
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, originPoint));
        fromPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(originPoint.mX, slideHeight*0.15)));
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, originPoint));
		fromAniSet->addAnimation(fromPosAni);

        SVIKeyFrameAnimation* toPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		toPosAni->mDuration = toAniSet->mDuration;
		toPosAni->mRepeatCount = toAniSet->mRepeatCount;
		toPosAni->mAutoReverse = toAniSet->mAutoReverse;
		toPosAni->mOffset = toAniSet->mOffset;
		toPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, originPoint));
        toPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(originPoint.mX, slideHeight*0.85)));
		toPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, originPoint));
		toAniSet->addAnimation(toPosAni);


        // ROTATION
        addParentRotAni(fromParentAniSet, parentRotValue);
        addParentRotAni(toParentAniSet, parentRotValue);
        addChildRotAni(fromAniSet, 30.0f);
		addChildRotAni(toAniSet, 30.0f);


        // SCALE
        addScaleAnimation(fromAniSet, SVIVector3(1.0f, 1.0f, 1.0f), SVIVector3(0.5f, 0.5f, 0.1f), mFullTimeDuration, mGlovalOffsetDuration);
        addScaleAnimation(toAniSet, SVIVector3(0.5f, 0.5f, 1.0f), SVIVector3(1.0f, 1.0f, 0.1f), mFullTimeDuration, mGlovalOffsetDuration);


        // Z POSITION
		addZPositionAnimation(fromParentAniSet, -0.01f, 0.01f, mFullTimeDuration, mGlovalOffsetDuration);
		addZPositionAnimation(toParentAniSet, 0.01f, -0.01f, mFullTimeDuration, mGlovalOffsetDuration);


        // ADD
        mFromNParentSlide->addAnimation(fromParentAniSet);
        mToNParentSlide->addAnimation(toParentAniSet);
		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}

    void SVISwitchEffector::addParentRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle){

		SVIKeyFrameAnimation* rotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		rotateAni->mDuration = aniSet->mDuration;
		rotateAni->mRepeatCount = aniSet->mRepeatCount;
		rotateAni->mAutoReverse = aniSet->mAutoReverse;
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
		y = SVI_DEGTORAD(0.0f);
        z = SVI_DEGTORAD(-rotAngle);

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

    void SVISwitchEffector::addChildRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle){

		SVIKeyFrameAnimation* rotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		rotateAni->mDuration = aniSet->mDuration;
		rotateAni->mRepeatCount = aniSet->mRepeatCount;
		rotateAni->mAutoReverse = aniSet->mAutoReverse;
		rotateAni->mOffset = aniSet->mOffset;

		SVIFloat x, y, z;
		SVIQuaternion q;

		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(rotAngle);
		y = SVI_DEGTORAD(0.0f);
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