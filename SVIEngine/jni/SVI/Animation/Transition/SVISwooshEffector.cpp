#include "SVISwooshEffector.h"
#include "SVITransitionEffector.h"
#include "../../Render/SVITexture.h"
#include "../../Render/SVIFrameRenderer.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVISwooshEffector::SVISwooshEffector(SVIGLSurface *surface):SVITransitionEffector(surface){};

	void SVISwooshEffector::setAnimation(){

		

		SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
		toAniSet->mDuration = mFullTimeDuration;
		toAniSet->mRepeatCount = mRepeatCount;
		toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toAniSet->mAutoReverse = mAutoReverse;
		toAniSet->setInterpolator(SVIAnimation::DECELERATE);

		SVIKeyFrameAnimation* toScaleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SCALE,mSVIGLSurface);
		toScaleAni->mDuration = toAniSet->mDuration;
		toScaleAni->mRepeatCount = toAniSet->mRepeatCount;
		toScaleAni->mAutoReverse = toAniSet->mAutoReverse;
		toScaleAni->mOffset = toAniSet->mOffset;
		toScaleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(0.0f, 0.0f, 0.0f)));
		toScaleAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.3f, 0.3f, 0.3f)));
		toScaleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 1.0f, 1.0f)));

		toAniSet->addAnimation(toScaleAni);



		SVIPoint originPos = mToSlide->getPosition();

		SVIKeyFrameAnimation* toPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
		toPosAni->mDuration = toAniSet->mDuration;
		toPosAni->mRepeatCount = toAniSet->mRepeatCount;
		toPosAni->mAutoReverse = toAniSet->mAutoReverse;
		toPosAni->mOffset = toAniSet->mOffset;
		toPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(originPos.mX/2, originPos.mY/2)));
		toPosAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint((originPos.mX/2) * 3, (originPos.mY/2) * 3)));
		toPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, originPos));

		toAniSet->addAnimation(toPosAni);



		SVIKeyFrameAnimation* rotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		rotateAni->mDuration = toAniSet->mDuration;
		rotateAni->mRepeatCount = toAniSet->mRepeatCount;
		rotateAni->mAutoReverse = toAniSet->mAutoReverse;
		rotateAni->mOffset = toAniSet->mOffset;

		SVIFloat x, y, z;
		SVIQuaternion q;

		x = SVI_DEGTORAD(180.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(-90.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));



		x = SVI_DEGTORAD(0.0f);
		y = SVI_DEGTORAD(0.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));



		toAniSet->addAnimation(rotateAni);


		addZPositionAnimation(toAniSet, -0.01f, -0.01f, mFullTimeDuration, mOffsetDuration + mGlovalOffsetDuration);




		mToNslide[0][0]->addAnimation(toAniSet);
	}



}
