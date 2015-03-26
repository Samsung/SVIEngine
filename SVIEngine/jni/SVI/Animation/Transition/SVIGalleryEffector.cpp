#include "SVIGalleryEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIGalleryEffector::SVIGalleryEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rotationAngle(30.0f){
		mIsUseParent = SVITRUE;
        mIsUseClone = SVITRUE;
	}

	void SVIGalleryEffector::setAnimation(){

        SVIPoint originPoint = mFromSlide->getPosition();
        SVIFloat slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
		SVIFloat slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

        SVIFloat time[4] = {0.0f, 0.3f, 0.7f, 1.0f};


		SVIAnimationSet* fromParentAni = new SVIAnimationSet(mSVIGLSurface);
		fromParentAni->mDuration = mFullTimeDuration;
		fromParentAni->mRepeatCount = mRepeatCount;
		fromParentAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromParentAni->mAutoReverse = mAutoReverse;
		fromParentAni->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		fromParentAni->shareAnimationInfo(true);

		SVIAnimationSet* toParentAni = new SVIAnimationSet(mSVIGLSurface);
		toParentAni->mDuration = mFullTimeDuration;
		toParentAni->mRepeatCount = mRepeatCount;
		toParentAni->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toParentAni->mAutoReverse = mAutoReverse;
		toParentAni->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
		toParentAni->shareAnimationInfo(true);

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
        SVIPoint parentPivotPoint = SVIPoint(1.0f, 0.5f);
        SVIFloat parentRotValue = 30.0f;


        switch(mDirectionType){
		case SVITransitionAnimation::LEFT:
		case SVITransitionAnimation::UP:
            fromPointX = originPoint.mX - (mTextureWidth * 1.1f);
            toPointX = originPoint.mX + (mTextureWidth * 1.1f);
            parentPivotPoint = SVIPoint(1.0f, 0.5f);
            parentRotValue = 30.0f;
            break;

		case SVITransitionAnimation::RIGHT:
		case SVITransitionAnimation::DOWN:
            fromPointX = originPoint.mX + (mTextureWidth * 1.1f);
            toPointX = originPoint.mX - (mTextureWidth * 1.1f);
            parentPivotPoint = SVIPoint(0.0f, 0.5f);
            parentRotValue = -30.0f;
            break;
        }


        // position
		SVIKeyFrameAnimation* fromPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		fromPosAni->mDuration = fromAniSet->mDuration;
		fromPosAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromPosAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromPosAni->mOffset = fromAniSet->mOffset;
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(time[0], originPoint));
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(time[1], originPoint));
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(time[2], SVIPoint(fromPointX, originPoint.mY)));
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(time[3], SVIPoint(fromPointX, originPoint.mY)));
		fromAniSet->addAnimation(fromPosAni);

        SVIKeyFrameAnimation* toPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		toPosAni->mDuration = toAniSet->mDuration;
		toPosAni->mRepeatCount = toAniSet->mRepeatCount;
		toPosAni->mAutoReverse = toAniSet->mAutoReverse;
		toPosAni->mOffset = toAniSet->mOffset;
        toPosAni->addKeyProperty(SVIKeyFrameProperty(time[0], SVIPoint(toPointX, originPoint.mY)));
        toPosAni->addKeyProperty(SVIKeyFrameProperty(time[1], SVIPoint(toPointX, originPoint.mY)));
        toPosAni->addKeyProperty(SVIKeyFrameProperty(time[2], originPoint));
        toPosAni->addKeyProperty(SVIKeyFrameProperty(time[3], originPoint));
		toAniSet->addAnimation(toPosAni);


        SVIFloat mirrorSlideY = originPoint.mY + slideHeight*1.01f;

        SVIKeyFrameAnimation* fromClonePosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		fromClonePosAni->mDuration = fromAniSet->mDuration;
		fromClonePosAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromClonePosAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromClonePosAni->mOffset = fromAniSet->mOffset;
        fromClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[0], SVIPoint(originPoint.mX, mirrorSlideY)));
		fromClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[1], SVIPoint(originPoint.mX, mirrorSlideY)));
		fromClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[2], SVIPoint(fromPointX, mirrorSlideY)));
		fromClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[3], SVIPoint(fromPointX, mirrorSlideY)));
		fromMirrorAniSet->addAnimation(fromClonePosAni);

        SVIKeyFrameAnimation* toClonePosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		toClonePosAni->mDuration = toAniSet->mDuration;
		toClonePosAni->mRepeatCount = toAniSet->mRepeatCount;
		toClonePosAni->mAutoReverse = toAniSet->mAutoReverse;
		toClonePosAni->mOffset = toAniSet->mOffset;
        toClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[0], SVIPoint(toPointX, mirrorSlideY)));
        toClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[1], SVIPoint(toPointX, mirrorSlideY)));
        toClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[2], SVIPoint(originPoint.mX, mirrorSlideY)));
        toClonePosAni->addKeyProperty(SVIKeyFrameProperty(time[3], SVIPoint(originPoint.mX, mirrorSlideY)));
		toMirrorAniSet->addAnimation(toClonePosAni);



        // Opacity
		SVIKeyFrameAnimation* fromOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromOpaAni->mDuration = fromAniSet->mDuration;
		fromOpaAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromOpaAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromOpaAni->mOffset = fromAniSet->mOffset;
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.8f, 1.0f));
		fromOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
		fromAniSet->addAnimation(fromOpaAni);

        SVIKeyFrameAnimation* fromCloneOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromCloneOpaAni->mDuration = fromAniSet->mDuration;
		fromCloneOpaAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromCloneOpaAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromCloneOpaAni->mOffset = fromAniSet->mOffset;
		fromCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.8f));
		fromCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(0.8f, 0.8f));
		fromCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
		fromMirrorAniSet->addAnimation(fromCloneOpaAni);


		SVIKeyFrameAnimation* toOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toOpaAni->mDuration = toAniSet->mDuration;
		toOpaAni->mRepeatCount = toAniSet->mRepeatCount;
		toOpaAni->mAutoReverse = toAniSet->mAutoReverse;
		toOpaAni->mOffset = toAniSet->mOffset;
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(0.2f, 1.0f));
		toOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 1.0f));
		toAniSet->addAnimation(toOpaAni);

        SVIKeyFrameAnimation* toCloneOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toCloneOpaAni->mDuration = toAniSet->mDuration;
		toCloneOpaAni->mRepeatCount = toAniSet->mRepeatCount;
		toCloneOpaAni->mAutoReverse = toAniSet->mAutoReverse;
		toCloneOpaAni->mOffset = toAniSet->mOffset;
		toCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
		toCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(0.2f, 0.8f));
		toCloneOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.8f));
		toMirrorAniSet->addAnimation(toCloneOpaAni);


        
		// Parent Rotation
		addRotAni(fromParentAni, parentRotValue);
		addRotAni(toParentAni, parentRotValue);
        mFromNParentSlide->setPivotPoint(parentPivotPoint);
        mToNParentSlide->setPivotPoint(parentPivotPoint);

		SVIKeyFrameAnimation* fromParentZAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		fromParentZAni->mDuration = fromParentAni->mDuration;
		fromParentZAni->mRepeatCount = fromParentAni->mRepeatCount;
		fromParentZAni->mAutoReverse = fromParentAni->mAutoReverse;
		fromParentZAni->mOffset = fromParentAni->mOffset;
		fromParentZAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
		fromParentZAni->addKeyProperty(SVIKeyFrameProperty(time[1], (originPoint.mX + originPoint.mY) * 0.05f));
		fromParentZAni->addKeyProperty(SVIKeyFrameProperty(time[2], (originPoint.mX + originPoint.mY) * 0.05f));
		fromParentZAni->addKeyProperty(SVIKeyFrameProperty(time[3], 0.0f));
		fromParentAni->addAnimation(fromParentZAni);

		SVIKeyFrameAnimation* toParentZAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		toParentZAni->mDuration = toParentAni->mDuration;
		toParentZAni->mRepeatCount = toParentAni->mRepeatCount;
		toParentZAni->mAutoReverse = toParentAni->mAutoReverse;
		toParentZAni->mOffset = toParentAni->mOffset;
		toParentZAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
		toParentZAni->addKeyProperty(SVIKeyFrameProperty(time[1], (originPoint.mX + originPoint.mY) * 0.05f));
		toParentZAni->addKeyProperty(SVIKeyFrameProperty(time[2], (originPoint.mX + originPoint.mY) * 0.05f));
		toParentZAni->addKeyProperty(SVIKeyFrameProperty(time[3], 0.0f));
		toParentAni->addAnimation(toParentZAni);



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
		mFromNParentSlide->addAnimation(fromParentAni);
		mToNParentSlide->addAnimation(toParentAni);

		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);        

        mFromCloneSlide->addAnimation(fromMirrorAniSet);
        mToCloneSlide->addAnimation(toMirrorAniSet);
	}

    void SVIGalleryEffector::addRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle){
        SVIFloat time[4] = {0.0f, 0.3f, 0.7f, 1.0f};

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
		y = SVI_DEGTORAD(rotAngle);
        z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(time[2], SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(time[3], SVIVector4(q.x, q.y, q.z, q.w)));
		

		aniSet->addAnimation(rotateAni);

	}
}