#include "SVIGallery2Effector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"
#include <math.h>

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIGallery2Effector::SVIGallery2Effector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rotationAngle(30.0f){
		mIsUseParent = SVITRUE;
        mIsUseClone = SVITRUE;
	}

	void SVIGallery2Effector::setAnimation(){

        SVIPoint originPoint = mFromSlide->getPosition();
        SVIFloat slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
		SVIFloat slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

        SVIFloat fromSlideWidth = mFromSlide->getRegion().getSize().mWidth;

        SVIFloat time[3] = {0.0f, 0.5f, 1.0f};


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

        SVIAnimationSet* fromMirrorAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromMirrorAniSet->mDuration = mFullTimeDuration;
		fromMirrorAniSet->mRepeatCount = mRepeatCount;
		fromMirrorAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromMirrorAniSet->mAutoReverse = mAutoReverse;
		fromMirrorAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		fromMirrorAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toMirrorAniSet = new SVIAnimationSet(mSVIGLSurface);
		toMirrorAniSet->mDuration = mFullTimeDuration;
		toMirrorAniSet->mRepeatCount = mRepeatCount;
		toMirrorAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toMirrorAniSet->mAutoReverse = mAutoReverse;
		toMirrorAniSet->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		toMirrorAniSet->shareAnimationInfo(true);


        SVIFloat fromPointX = originPoint.mX - (mTextureWidth * 1.1f);
        SVIFloat toPointX = originPoint.mX + (mTextureWidth * 1.1f);
        SVIFloat rotAngle = 15.0f;


		switch(mDirectionType){
		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
            fromPointX = originPoint.mX - (mTextureWidth * 1.1f);
            toPointX = originPoint.mX + (mTextureWidth * 1.1f);
            rotAngle = 30.0f;
            break;

		case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
            fromPointX = originPoint.mX + (mTextureWidth * 1.1f);
            toPointX = originPoint.mX - (mTextureWidth * 1.1f);
            rotAngle = -30.0f;
            break;
        }


        // position
        addPositionAnimation(fromAniSet, originPoint, SVIPoint(fromPointX, originPoint.mY), fromAniSet->mDuration, fromAniSet->mOffset);
        addPositionAnimation(toAniSet, SVIPoint(toPointX, originPoint.mY), originPoint, toAniSet->mDuration, toAniSet->mOffset);

        SVIFloat mirrorSlideY = originPoint.mY + slideHeight*1.01f;
        addPositionAnimation(fromMirrorAniSet, SVIPoint(originPoint.mX, mirrorSlideY), SVIPoint(fromPointX, mirrorSlideY), fromMirrorAniSet->mDuration, fromMirrorAniSet->mOffset);
        addPositionAnimation(toMirrorAniSet, SVIPoint(toPointX, mirrorSlideY), SVIPoint(originPoint.mX, mirrorSlideY), toMirrorAniSet->mDuration, toMirrorAniSet->mOffset);


        // Opacity
		SVIKeyFrameAnimation* fromOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromOpaAni->mDuration = fromAniSet->mDuration;
		fromOpaAni->mOffset = fromAniSet->mOffset;
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(time[0], 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(time[1], 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(time[2], 0.0f));
		fromAniSet->addAnimation(fromOpaAni);

        SVIKeyFrameAnimation* fromCloneOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromCloneOpaAni->mDuration = fromAniSet->mDuration;
		fromCloneOpaAni->mOffset = fromAniSet->mOffset;
		fromCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.8f));
		fromCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(time[1], 0.8f));
		fromCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(time[2], 0.0f));
		fromMirrorAniSet->addAnimation(fromCloneOpaAni);


		SVIKeyFrameAnimation* toOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toOpaAni->mDuration = toAniSet->mDuration;
		toOpaAni->mOffset = toAniSet->mOffset;
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(time[1], 1.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(time[2], 1.0f));
		toAniSet->addAnimation(toOpaAni);

        SVIKeyFrameAnimation* toCloneOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toCloneOpaAni->mDuration = toAniSet->mDuration;
		toCloneOpaAni->mOffset = toAniSet->mOffset;
		toCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
		toCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(time[1], 0.8f));
		toCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(time[2], 0.8f));
		toMirrorAniSet->addAnimation(toCloneOpaAni);



        // Rotation
        addRotAni(fromAniSet, rotAngle);
		addRotAni(toAniSet, rotAngle);
        addRotAni(fromMirrorAniSet, rotAngle);
		addRotAni(toMirrorAniSet, rotAngle);


        // z position        
        SVIFloat zPosition = (fromSlideWidth*0.5f)*abs(sin(rotAngle*3.1416/180));
        addZPosAni(fromAniSet, zPosition);
        addZPosAni(toAniSet, zPosition);
        addZPosAni(fromMirrorAniSet, zPosition);
        addZPosAni(toMirrorAniSet, zPosition);


        // Mirror slide rotation
        SVIFloat x, y, z;
		SVIQuaternion q;

		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(180.0f);
		z = SVI_DEGTORAD(180.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));

        mFromCloneSlide->setRotation(SVIVector4(q.x, q.y, q.z, q.w));
        mToCloneSlide->setRotation(SVIVector4(q.x, q.y, q.z, q.w));        


        // add
		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);        

        mFromCloneSlide->addAnimation(fromMirrorAniSet);
        mToCloneSlide->addAnimation(toMirrorAniSet);
	}

    void SVIGallery2Effector::addRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle){
        SVIFloat time[3] = {0.0f, 0.5f, 1.0f};

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
		rotateAni->addKeyProperty(SVIKeyFrameProperty(time[0], SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(rotAngle);
        z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(time[1], SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(time[2], SVIVector4(q.x, q.y, q.z, q.w)));
		

		aniSet->addAnimation(rotateAni);

	}


    void SVIGallery2Effector::addZPosAni(SVIAnimationSet* aniSet, SVIFloat zPosition){
        SVIFloat time[3] = {0.0f, 0.5f, 1.0f};

        SVIKeyFrameAnimation* zPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		zPosAni->mDuration = aniSet->mDuration;
		zPosAni->mRepeatCount = aniSet->mRepeatCount;
		zPosAni->mAutoReverse = aniSet->mAutoReverse;
		zPosAni->mOffset = aniSet->mOffset;
		zPosAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
        zPosAni->addKeyProperty(SVIKeyFrameProperty(time[1], zPosition));
		zPosAni->addKeyProperty(SVIKeyFrameProperty(time[2], 0.0f));
		aniSet->addAnimation(zPosAni);
    }
}