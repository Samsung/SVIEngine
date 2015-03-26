#include "SVICoverEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVICoverEffector::SVICoverEffector(SVIGLSurface *surface):SVITransitionEffector(surface){
	}

	void SVICoverEffector::setAnimation(){

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
		fromRotateAni->mOffset = fromAniSet->mOffset;
		fromRotateAni->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
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
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		fromAniSet->addAnimation(fromRotateAni);

		SVIKeyFrameAnimation* fromZpositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		fromZpositionAni->mDuration = fromAniSet->mDuration;
		fromZpositionAni->mOffset = fromAniSet->mOffset;
		fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.01f));
		fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 40.01f));
		fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.01f));
		fromAniSet->addAnimation(fromZpositionAni);



		SVIKeyFrameAnimation* toRotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		toRotateAni->mDuration = fromAniSet->mDuration;
		toRotateAni->mOffset = fromAniSet->mOffset;
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
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

		SVIKeyFrameAnimation* posAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		posAni->mDuration = toAniSet->mDuration;
		posAni->mOffset = toAniSet->mOffset;
		posAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(originPoint.mX + 1000.0f, originPoint.mY + 100.0f)));
		//posAni->addKeyProperty(SVIKeyFrameProperty(0.1f, SVIPoint(originPoint.mX + 1000.0f, originPoint.mY + 100.0f)));
		posAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(originPoint.mX, originPoint.mY)));
		posAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(originPoint.mX, originPoint.mY)));
		toAniSet->addAnimation(posAni);

		SVIKeyFrameAnimation* toZpositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		toZpositionAni->mDuration = toAniSet->mDuration;
		toZpositionAni->mOffset = toAniSet->mOffset;
		toZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, -500.0f));
		toZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 40.0f));
		toZpositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
		toAniSet->addAnimation(toZpositionAni);

		SVIKeyFrameAnimation* toOpacityAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
		toOpacityAni->mDuration = fromAniSet->mDuration;
		toOpacityAni->mOffset = fromAniSet->mOffset;
		toOpacityAni->setInterpolator(SVIAnimation::ACCELERATE);
		toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIFloat(0.0f)));
		toOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.6f, SVIFloat(1.0f)));
		toOpacityAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIFloat(1.0f)));
		toAniSet->addAnimation(toOpacityAni);





		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}
