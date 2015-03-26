#include "SVIUncoverEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIUncoverEffector::SVIUncoverEffector(SVIGLSurface *surface):SVITransitionEffector(surface){
	}

	void SVIUncoverEffector::setAnimation(){

		SVIVector3 fromRotationValue;
		SVIVector3 toRotationValue;
		SVIFloat x, y, z;

		SVIPoint originPoint = mSizeSlide->getPosition();
		originPoint.mY += mSizeSlide->getRegion().getSize().mHeight / 2;

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

		mFromNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 1.0f));

		SVIKeyFrameAnimation* fromRotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		fromRotateAni->mDuration = fromAniSet->mDuration;
		fromRotateAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromRotateAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromRotateAni->mOffset = fromAniSet->mOffset;
		fromRotateAni->setInterpolator(SVIAnimation::ACCELERATE);
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		x = SVI_DEGTORAD(-10.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector4(q.x, q.y, q.z, q.w)));
		x = SVI_DEGTORAD(-10.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		fromAniSet->addAnimation(fromRotateAni);

		SVIKeyFrameAnimation* posAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		posAni->mDuration = fromAniSet->mDuration;
		posAni->mRepeatCount = fromAniSet->mRepeatCount;
		posAni->mAutoReverse = fromAniSet->mAutoReverse;
		posAni->mOffset = fromAniSet->mOffset;
		posAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(originPoint.mX, originPoint.mY)));
		posAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(originPoint.mX, originPoint.mY)));
		//posAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(originPoint.mX - 1000.0f, originPoint.mY - 100.0f)));
		posAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(originPoint.mX - 1000.0f, originPoint.mY - 100.0f)));
		fromAniSet->addAnimation(posAni);

		SVIKeyFrameAnimation* fromZpositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		fromZpositionAni->mDuration = fromAniSet->mDuration;
		fromZpositionAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromZpositionAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromZpositionAni->mOffset = fromAniSet->mOffset;
		fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, -0.01f));
		fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 40.0f));
		fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, -500.0f));
		fromAniSet->addAnimation(fromZpositionAni);

		SVIKeyFrameAnimation* fromOpacityAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		fromOpacityAni->mDuration = fromAniSet->mDuration;
		fromOpacityAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromOpacityAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromOpacityAni->mOffset = fromAniSet->mOffset;
		fromOpacityAni->setInterpolator(SVIAnimation::ACCELERATE);
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIFloat(1.0f)));
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.6f, SVIFloat(1.0f)));
		fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIFloat(0.0f)));
		fromAniSet->addAnimation(fromOpacityAni);





		SVIKeyFrameAnimation* toRotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		toRotateAni->mDuration = fromAniSet->mDuration;
		toRotateAni->mRepeatCount = fromAniSet->mRepeatCount;
		toRotateAni->mAutoReverse = fromAniSet->mAutoReverse;
		toRotateAni->mOffset = fromAniSet->mOffset;
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		/*x = SVI_DEGTORAD(-30.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(0.1f, SVIVector4(q.x, q.y, q.z, q.w)));*/
		x = SVI_DEGTORAD(-10.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector4(q.x, q.y, q.z, q.w)));
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		mToNslide[0][0]->setPivotPoint(SVIPoint(0.5f, 1.0f));
		toAniSet->addAnimation(toRotateAni);

		SVIKeyFrameAnimation* toZpositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		toZpositionAni->mDuration = toAniSet->mDuration;
		toZpositionAni->mRepeatCount = toAniSet->mRepeatCount;
		toZpositionAni->mAutoReverse = toAniSet->mAutoReverse;
		toZpositionAni->mOffset = toAniSet->mOffset;
		toZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.01f));
		//toZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.1f, 40.0f));
		toZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 40.01f));
		toZpositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.01f));
		toAniSet->addAnimation(toZpositionAni);



		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}
