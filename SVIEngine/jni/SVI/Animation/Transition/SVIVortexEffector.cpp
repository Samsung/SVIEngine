#include "SVIVortexEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIVortexEffector::SVIVortexEffector(SVIGLSurface *surface):SVITransitionEffector(surface){
	}

	void SVIVortexEffector::setAnimation(){
		SVIFloat x, y, z;

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

		SVIPoint originPoint = SVIPoint(mFromSlide->getPosition().mX + (mFromSlide->getRegion().getSize().mWidth * 0.5f ), mFromSlide->getPosition().mY);


		mFromNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));
		mToNslide[0][0]->setPivotPoint(SVIPoint(1.0f, 0.5f));
		addPositionAnimation(fromAniSet, originPoint, originPoint);
		addPositionAnimation(toAniSet, originPoint, originPoint);


		//addRotationAnimation(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -135.0f, 0.0f));

		SVIKeyFrameAnimation* fromRotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		fromRotateAni->mDuration = fromAniSet->mDuration;
		fromRotateAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromRotateAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromRotateAni->mOffset = fromAniSet->mOffset;
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(-10.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIVector4(q.x, q.y, q.z, q.w)));
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(-180.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		fromRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		fromAniSet->addAnimation(fromRotateAni);


		//addRotationAnimation(toAniSet, SVIVector3(0.0f, 135.0f, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f));
		SVIKeyFrameAnimation* toRotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION , mSVIGLSurface);
		toRotateAni->mDuration = toAniSet->mDuration;
		toRotateAni->mRepeatCount = toAniSet->mRepeatCount;
		toRotateAni->mAutoReverse = toAniSet->mAutoReverse;
		toRotateAni->mOffset = toAniSet->mOffset;
		x = SVI_DEGTORAD(0);
		y = SVI_DEGTORAD(135.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(0.8f, SVIVector4(q.x, q.y, q.z, q.w)));
		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		toRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		toAniSet->addAnimation(toRotateAni);






		addDeformAnimation(fromAniSet, SVIVector3(0.0f, -1.0f, 0.0f), SVIVector3(0.6f, -1.0f, 0.0f));
		addDeformAnimation(toAniSet, SVIVector3(0.5f, 1.0f, 0.0f), SVIVector3(0.0f, 1.0f, 0.0f));


		//addOpacityAnimation(fromAniSet,0.0f,1.0f);
		//addOpacityAnimation(fromAniSet,0.3f,0.0f);
		//addOpacityAnimation(fromAniSet,1.0f,0.0f);
		

		//addZPositionAnimation(fromAniSet, -0.01f, 0.01f);
	//	addZPositionAnimation(toAniSet, 0.01f, -0.01f);
		
		//addZPositionAnimation(toAniSet, mFromSlide->getRegion().getSize().mWidth, 0.0f);

		mFromNslide[0][0]->addAnimation(fromAniSet);

		mToNslide[0][0]->addAnimation(toAniSet);
	}
}
