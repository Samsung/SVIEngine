#include "SVICube2PiecesEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVICube2PiecesEffector::SVICube2PiecesEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rotationAngle(90.0f),
	rowCount(1),
	columnCount(2){
		mIsUseFakeParent = SVITRUE;
		mRowCount = rowCount;
		mColumnCount = columnCount;
	}

	void SVICube2PiecesEffector::setAnimation(){

		SVIInt slideWidth, slideHeight, slideDuration, offsetDuration;

		offsetDuration = mFullTimeDuration / 6;
		slideDuration = offsetDuration * 5;

		SVIPoint originPoint = mFromSlide->getPosition();

		slideWidth = (mFromSlide->getRegion().mSize.mWidth / mRowCount) * 0.5f;
		slideHeight = (mFromSlide->getRegion().mSize.mHeight / mColumnCount) * 0.5f;

		for(SVIInt i = 0; i< mColumnCount; i++){

			SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
			fromAniSet->mDuration = mFullTimeDuration;
			fromAniSet->mRepeatCount = mRepeatCount;
			fromAniSet->mOffset =  mGlovalOffsetDuration;
			fromAniSet->mAutoReverse = mAutoReverse;
			fromAniSet->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

			SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
			toAniSet->mDuration = mFullTimeDuration;
			toAniSet->mRepeatCount = mRepeatCount;
			toAniSet->mOffset =  mGlovalOffsetDuration;
			toAniSet->mAutoReverse = mAutoReverse;
			toAniSet->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);
			
			SVIInt offset =(offsetDuration * i);

			if(i == 0){
				mFromNslide[i][0]->setPivotPoint(SVIPoint(0.0f, 1.0f));
				mToNslide[i][0]->setPivotPoint(SVIPoint(1.0f, 1.0f));

			}else{
				mFromNslide[i][0]->setPivotPoint(SVIPoint(0.0f, 0.0f));
				mToNslide[i][0]->setPivotPoint(SVIPoint(1.0f, 0.0f));
			}


			SVIKeyFrameAnimation* fromPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
			fromPosAni->mDuration = slideDuration;
			fromPosAni->mOffset = offset;
			fromPosAni->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

			// from point
			fromPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(originPoint.mX - slideWidth, originPoint.mY)));
			// to point
			fromPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(originPoint.mX + slideWidth, originPoint.mY)));

			fromAniSet->addAnimation(fromPosAni);

			SVIKeyFrameAnimation* toPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION,mSVIGLSurface);
			toPosAni->mDuration = slideDuration;
			toPosAni->mOffset = offset;
			toPosAni->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

			// from point
			toPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f,  SVIPoint(originPoint.mX - slideWidth, originPoint.mY)));
			// to point
			toPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(originPoint.mX + slideWidth, originPoint.mY)));

			toAniSet->addAnimation(toPosAni);




			addRotAni(fromAniSet, SVIVector3(0.0f, 0.0f, 0.0f), SVIVector3(0.0f, -rotationAngle, 0.0f), slideDuration, offset);
			addRotAni(toAniSet, SVIVector3(0.0f, rotationAngle, 0.0f), SVIVector3(0.0f, 0.0f, 0.0f), slideDuration, offset);

			addZPositionAnimation(fromAniSet, -0.01f, 0.01f, slideDuration, offset);
			addZPositionAnimation(toAniSet, 0.01f, -0.01f, slideDuration, offset);


			SVIKeyFrameAnimation* fromColorAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::BG_COLOR,mSVIGLSurface);
			fromColorAni->mDuration = slideDuration;
			fromColorAni->mOffset = offset;
			fromColorAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIColor(1.0f, 1.0f, 1.0f, 1.0f)));
			fromColorAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIColor(0.3f, 0.3f, 0.3f, 1.0f)));
			fromAniSet->addAnimation(fromColorAni);

			SVIKeyFrameAnimation* toColorAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::BG_COLOR,mSVIGLSurface);
			toColorAni->mDuration = slideDuration;
			toColorAni->mOffset = offset;
			toColorAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIColor(0.3f, 0.3f, 0.3f, 1.0f)));
			toColorAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIColor(1.0f, 1.0f, 1.0f, 1.0f)));
			toAniSet->addAnimation(toColorAni);


			mFromNslide[i][0]->addAnimation(fromAniSet);

			mToNslide[i][0]->addAnimation(toAniSet);
		}

		SVIAnimationSet* parentAniSet = new SVIAnimationSet(mSVIGLSurface);
		parentAniSet->mDuration = mFullTimeDuration;
		parentAniSet->mRepeatCount = mRepeatCount;
		parentAniSet->mOffset =  mGlovalOffsetDuration + mOffsetDuration;
		parentAniSet->mAutoReverse = mAutoReverse;
		parentAniSet->setInterpolator(SVIAnimation::LINEAR);
		parentAniSet->shareAnimationInfo(true);


		SVIKeyFrameAnimation* scaleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SCALE,mSVIGLSurface);
		scaleAni->mDuration = parentAniSet->mDuration;
		scaleAni->mOffset = parentAniSet->mOffset;
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.15f, SVIVector3(0.8f, 0.8f, 0.8f)));
		//scaleAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.7f, 0.7f, 0.7f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.85f, SVIVector3(0.8f, 0.8f, 0.8f)));
		scaleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
		parentAniSet->addAnimation(scaleAni);
		

		mFakeParentSlide->addAnimation(parentAniSet);
	}


	void SVICube2PiecesEffector::addRotAni(SVIAnimationSet* aniSet, SVIVector3 preAngle, SVIVector3 nextAngle, SVIInt duration, SVIInt offset) {

		SVIKeyFrameAnimation* rotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION,mSVIGLSurface);
		rotateAni->mDuration = duration;
		rotateAni->mOffset = offset;
		rotateAni->setInterpolator(SVIAnimation::ANTICIPATE_OVERSHOOT);

		SVIFloat x, y, z;

		// from angle
		x = SVI_DEGTORAD(preAngle.x);
		y = SVI_DEGTORAD(preAngle.y);
		z = SVI_DEGTORAD(preAngle.z);
		SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));

		// to angle
		x = SVI_DEGTORAD(nextAngle.x);
		y = SVI_DEGTORAD(nextAngle.y);
		z = SVI_DEGTORAD(nextAngle.z);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));

		aniSet->addAnimation(rotateAni);
	}

}