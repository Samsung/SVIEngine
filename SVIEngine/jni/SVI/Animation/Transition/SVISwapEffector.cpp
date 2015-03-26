#include "SVISwapEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVISwapEffector::SVISwapEffector(SVIGLSurface *surface):SVITransitionEffector(surface){};

	void SVISwapEffector::setAnimation(){

		SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
		fromAniSet->mDuration = mFullTimeDuration;
		fromAniSet->mRepeatCount = mRepeatCount;
		fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromAniSet->mAutoReverse = mAutoReverse;
		fromAniSet->setInterpolator(SVIAnimation::SINEEASEINOUT);
		fromAniSet->shareAnimationInfo(true);

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::SINEEASEINOUT);
		toAniSet->shareAnimationInfo(true);


		SVIKeyFrameAnimation* fromPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		fromPosAni->mDuration = fromAniSet->mDuration;
		fromPosAni->mRepeatCount = fromAniSet->mRepeatCount;
		fromPosAni->mAutoReverse = fromAniSet->mAutoReverse;
		fromPosAni->mOffset = fromAniSet->mOffset;
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, mToSlide->getPosition()));
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(0.0f, mToSlide->getPosition().mY)));
		fromPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, mToSlide->getPosition()));

		fromAniSet->addAnimation(fromPosAni);


		SVIKeyFrameAnimation* toPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		toPosAni->mDuration = fromAniSet->mDuration;
		toPosAni->mRepeatCount = fromAniSet->mRepeatCount;
		toPosAni->mAutoReverse = fromAniSet->mAutoReverse;
		toPosAni->mOffset = fromAniSet->mOffset;
		toPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, mToSlide->getPosition()));
		toPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(mToSlide->getRegion().getSize().mWidth, mToSlide->getPosition().mY)));
		toPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, mToSlide->getPosition()));

		toAniSet->addAnimation(toPosAni);


		addRotAni(fromAniSet, -30.0f);
		addRotAni(toAniSet, 30.0f);


		addZPositionAnimation(fromAniSet, -0.01f, mToSlide->getRegion().getSize().mWidth, mFullTimeDuration, mGlovalOffsetDuration);
		addZPositionAnimation(toAniSet, mToSlide->getRegion().getSize().mWidth, -0.01f, mFullTimeDuration, mGlovalOffsetDuration);

		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}


	void SVISwapEffector::addRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle){

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
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.4f, SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(rotAngle);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.6f, SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));


		aniSet->addAnimation(rotateAni);

	}


}