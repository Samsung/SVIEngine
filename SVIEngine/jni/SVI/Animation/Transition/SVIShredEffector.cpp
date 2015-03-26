#include "SVIShredEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIShredEffector::SVIShredEffector(SVIGLSurface *surface):SVITransitionEffector(surface),
	rowCount(15),
		columnCount(9){
			mRowCount = rowCount;
			mColumnCount = columnCount;
			mIsUseParent = SVITRUE;
	}

	void SVIShredEffector::setAnimation(){

		SVIInt **mEffectMap;
		SVIFloat time[5] = {0.0f, 0.3f, 0.5f, 0.7f, 1.0f};




		mEffectMap = new SVIInt *[mColumnCount];
		for(int i =0; i<mColumnCount; i++){
			mEffectMap[i] = new SVIInt[mRowCount];
		}

		SVIInt rowOffsetMap[9][15] =  {
			{1,-1,-1,1,-1,-1,1,-1,1,-1,1,1,-1,1,1},
			{1,-1,1,1,-1,-1,1,-1,1,-1,-1,1,-1,-1,1},
			{1,-1,-1,1,-1,1,1,-1,-1,1,-1,1,1,-1,1},
			{1,-1,1,1,-1,-1,1,1,-1,1,-1,-1,1,-1,-1},
			{1,-1,1,-1,-1,1,1,-1,-1,1,1,-1,1,-1,1},
			{1,1,-1,1,-1,1,-1,-1,1,1,-1,-1,1,-1,1},
			{-1,1,-1,1,-1,-1,1,-1,1,-1,1,-1,1,-1,1},
			{-1,1,-1,1,1,-1,-1,1,1,-1,1,-1,1,-1,1},
			{1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,1,-1,1}
		};

		SVIInt columnOffsetMap[15][9] = {
			{1,-1,-1,1,1,1,-1,1,-1},
			{1,-1,1,-1,1,-1,-1,1,-1},
			{1,-1,1,-1,1,-1,-1,1,-1},
			{1,-1,1,-1,1,1,-1,1,-1},
			{1,-1,1,-1,-1,1,-1,1,-1},
			{1,-1,1,1,-1,1,-1,1,1},
			{1,1,-1,1,1,-1,1,1,-1},
			{-1,1,-1,-1,1,-1,-1,1,-1},
			{-1,-1,1,-1,1,1,-1,1,1},
			{-1,-1,1,1,1,-1,-1,1,1},
			{-1,-1,1,-1,1,-1,1,-1,1},
			{-1,-1,1,-1,1,-1,1,-1,1},
			{1,-1,1,-1,1,-1,1,-1,1},
			{1,-1,1,-1,-1,-1,1,-1,1},
			{1,-1,1,1,-1,1,1,-1,-1}
		};

		for(int i=0; i< mColumnCount;i++){
			for(int j = 0; j < mRowCount; j++){
				if(mTextureWidth<mTextureHeight){
					mEffectMap[i][j] = columnOffsetMap[i][j];
				}else{
					mEffectMap[i][j] = rowOffsetMap[i][j];
				}
			}
		}

		SVIAnimationSet* fromParentAniSet = new SVIAnimationSet(mSVIGLSurface);

		fromParentAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		fromParentAniSet->mDuration = mFullTimeDuration;

		fromParentAniSet->mRepeatCount = mRepeatCount;
		fromParentAniSet->mAutoReverse = mAutoReverse;
		fromParentAniSet->setInterpolator(SVIAnimation::LINEAR);



		SVIAnimationSet* toParentAniSet = new SVIAnimationSet(mSVIGLSurface);

		toParentAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
		toParentAniSet->mDuration = mFullTimeDuration;

		toParentAniSet->mRepeatCount = mRepeatCount;
		toParentAniSet->mAutoReverse = mAutoReverse;
		toParentAniSet->setInterpolator(SVIAnimation::LINEAR);


		addParentAni(fromParentAniSet);
		addParentAni(toParentAniSet);


		SVIFloat zValue = mTextureWidth > mTextureHeight ? mTextureHeight: mTextureWidth;


		
		for(int i=0; i<mColumnCount; i++){
			for(int j=0; j<mRowCount; j++){

				zValue *= mEffectMap[i][j];

				SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
				fromAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
				fromAniSet->mDuration = mFullTimeDuration;
				fromAniSet->mRepeatCount = mRepeatCount;
				fromAniSet->mAutoReverse = mAutoReverse;
				fromAniSet->setInterpolator(SVIAnimation::LINEAR);
				fromAniSet->shareAnimationInfo(true);


				SVIAnimationSet* toAniSet = new SVIAnimationSet(mSVIGLSurface);
				toAniSet->mOffset = mOffsetDuration + mGlovalOffsetDuration;
				toAniSet->mDuration = mFullTimeDuration;
				toAniSet->mRepeatCount = mRepeatCount;
				toAniSet->mAutoReverse = mAutoReverse;
				toAniSet->setInterpolator(SVIAnimation::LINEAR);
				toAniSet->shareAnimationInfo(true);


				SVIKeyFrameAnimation* fromZAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
				fromZAni->mDuration = fromAniSet->mDuration;
				fromZAni->mOffset = fromAniSet->mOffset;
				fromZAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
				fromZAni->addKeyProperty(SVIKeyFrameProperty(time[1], zValue * 0.25));
				fromZAni->addKeyProperty(SVIKeyFrameProperty(time[2], zValue * 4));
				fromZAni->addKeyProperty(SVIKeyFrameProperty(time[3], zValue * 0.25f));
				fromZAni->addKeyProperty(SVIKeyFrameProperty(time[4], 0.0f));

				fromAniSet->addAnimation(fromZAni);


				SVIKeyFrameAnimation* fromOpaAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY,mSVIGLSurface);
				fromOpaAni->mDuration = fromAniSet->mDuration;
				fromOpaAni->mOffset = fromAniSet->mOffset;
				fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));
				fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.45f, 1.0f));
				fromOpaAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 0.0f));
				fromOpaAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));

				fromAniSet->addAnimation(fromOpaAni);



				SVIKeyFrameAnimation* toZAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
				toZAni->mDuration = toAniSet->mDuration;
				toZAni->mOffset = toAniSet->mOffset;
				toZAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
				toZAni->addKeyProperty(SVIKeyFrameProperty(time[1], zValue * 0.25f));
				toZAni->addKeyProperty(SVIKeyFrameProperty(time[2], zValue * 4));
				toZAni->addKeyProperty(SVIKeyFrameProperty(time[3], zValue * 0.25f));
				toZAni->addKeyProperty(SVIKeyFrameProperty(time[4], 0.0f));

				toAniSet->addAnimation(toZAni);
				
				mFromNslide[i][j]->addAnimation(fromAniSet);
				mToNslide[i][j]->addAnimation((toAniSet));

			}
		}
		

		mFromNParentSlide->addAnimation(fromParentAniSet);
		mToNParentSlide->addAnimation(toParentAniSet);

		SVI_SVIFE_DELETE_ARRAY(mEffectMap);
	}


	void SVIShredEffector::addParentAni(SVIAnimationSet* aniSet){

		SVIFloat time[4] = {0.0f, 0.3f, 0.7f, 1.0f};

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
		rotateAni->addKeyProperty(SVIKeyFrameProperty(time[0], SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(15.0f);
		y = SVI_DEGTORAD(-30.0f);
		z = SVI_DEGTORAD(0.0f);

		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(time[1], SVIVector4(q.x, q.y, q.z, q.w)));


		x = SVI_DEGTORAD(15.0f);
		y = SVI_DEGTORAD(-30.0f);
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


		SVIFloat zValue = mTextureWidth > mTextureHeight ? mTextureHeight: mTextureWidth;

		SVIKeyFrameAnimation* zAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION,mSVIGLSurface);
		zAni->mDuration = aniSet->mDuration;
		zAni->mOffset = aniSet->mOffset;
		zAni->addKeyProperty(SVIKeyFrameProperty(time[0], 0.0f));
		zAni->addKeyProperty(SVIKeyFrameProperty(time[1], zValue * 0.5f));
		zAni->addKeyProperty(SVIKeyFrameProperty(time[2], zValue * 0.5f));
		zAni->addKeyProperty(SVIKeyFrameProperty(time[3], 0.0f));

		aniSet->addAnimation(zAni);

	}


}
