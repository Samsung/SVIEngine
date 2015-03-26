#include "SVITwirlEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVITwirlEffector::SVITwirlEffector(SVIGLSurface *surface):SVITransitionEffector(surface){};

	void SVITwirlEffector::setAnimation(){

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

		addRotAni(fromAniSet);
		addRotAni(toAniSet);

		addScaleAni(fromAniSet);
		addScaleAni(toAniSet);

		addOpacityAnimation(fromAniSet, 1.0f, 0.0f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);

		

		mFromNslide[0][0]->addAnimation(fromAniSet);
		mToNslide[0][0]->addAnimation(toAniSet);
	}


	void SVITwirlEffector::addRotAni(SVIAnimationSet* aniSet){

		SVIKeyFrameAnimation* rotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		rotateAni->mDuration = aniSet->mDuration;
		rotateAni->mRepeatCount = aniSet->mRepeatCount;
		rotateAni->mAutoReverse = aniSet->mAutoReverse;
		rotateAni->mOffset = aniSet->mOffset;

		SVIFloat x, y, z;
		SVIQuaternion q;

		for(SVIInt i=0; i<=8; i++){
			x = SVI_DEGTORAD(0.0f);
			y = SVI_DEGTORAD(0.0f);
			z = SVI_DEGTORAD(i*180.0f);
			
			q.setIdentity();
			q.setEuler(SVIVector3(x, y, z));
			rotateAni->addKeyProperty(SVIKeyFrameProperty(i* 0.125f, SVIVector4(q.x, q.y, q.z, q.w)));
		}
		

		aniSet->addAnimation(rotateAni);

	}

	void SVITwirlEffector::addScaleAni(SVIAnimationSet* aniSet){

		SVIKeyFrameAnimation* scaleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SCALE,mSVIGLSurface);
		scaleAni->mDuration = aniSet->mDuration;
		scaleAni->mRepeatCount = aniSet->mRepeatCount;
		scaleAni->mAutoReverse = aniSet->mAutoReverse;
		scaleAni->mOffset = aniSet->mOffset;

		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.4f, SVIVector3(0.3f, 0.3f, 0.3f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.6f, SVIVector3(0.3f, 0.3f, 0.3f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 1.0f, 1.0f)));

		aniSet->addAnimation(scaleAni);

	}
}