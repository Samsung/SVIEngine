#include <algorithm>
#include "SVIParticleKeyFrameAnimation.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../Particle/SVIParticles.h"

namespace SVI {
static const SVIBool DEBUG = SVIFALSE;

#define TYPE_CHECK_FOR_KEYFRAMES(type)						            \
	{														            \
		for(SVIUInt i=0; i<mKeyFrameList.size(); i++) {			        \
			if( mKeyFrameList[i].mProperty.mPropertyType != type ){	    \
				mTypeCheckSuccess = SVIFALSE;				            \
				LOGE("KeyFrameList %dth is wrong type!!!", (SVIInt)type);\
			}												            \
		}													            \
	}														            \

	SVIParticleKeyFrameAnimation::SVIParticleKeyFrameAnimation(SVIGLSurface* saGLSurface):SVIParticlePropertyAnimation(saGLSurface) {
		mFromKey = NULL;
		mToKey = NULL;
		mTypeChecked = SVIFALSE;
		mTypeCheckSuccess = SVIFALSE;

		mClassType = KEYFRAME;
	}

	SVIParticleKeyFrameAnimation::SVIParticleKeyFrameAnimation(ParticlePropertyAnimationType type, SVIGLSurface* saGLSurface):SVIParticlePropertyAnimation(saGLSurface) {
		mFromKey = NULL;
		mToKey = NULL;
		mTypeChecked = SVIFALSE;
		mTypeCheckSuccess = SVIFALSE;
		mParticlePropertyAnimationType = type;
		
		mClassType = KEYFRAME;
	}

	SVIParticleKeyFrameAnimation::SVIParticleKeyFrameAnimation(const SVIParticleKeyFrameAnimation* rhs):SVIParticlePropertyAnimation(rhs->mSVIGLSurface) {
		mFromKey = NULL;
		mToKey = NULL;
		mTypeChecked = SVIFALSE;
		mTypeCheckSuccess = SVIFALSE;
	}

	SVIParticleKeyFrameAnimation::~SVIParticleKeyFrameAnimation() {
		mKeyFrameList.clear();
	}

	SVIParticleKeyFrameAnimation* SVIParticleKeyFrameAnimation::create(ParticlePropertyAnimationType type, SVIGLSurface* saGLSurface) {
		SVIParticleKeyFrameAnimation* keyFrameAnimation = new SVIParticleKeyFrameAnimation(type, saGLSurface);
		return keyFrameAnimation;
	}

	void SVIParticleKeyFrameAnimation::update(SVIUInt& time) {
		SVIParticlePropertyAnimation::updateInfo(time);

		if( getStatus() == ANIMATING ) {
			if( !mTypeChecked )
				checkType();

			if( findKeyFrame() ) {
				DEBUG_CHECK_LOGI("FromKey:%f, ToKey:%f", mFromKey->mKeyTime, mToKey->mKeyTime);
				calculateAndApply();
			} else {
				LOGE("Can not find out key-frame!!!");
				return;
			}

			if( isEnd(time) ) {
				setStatus(END);
				if( !mIsContainedInAniSet && mSlide != NULL ) {
					mSlide->removeAnimation();
				}
			}
		}
	}


	void SVIParticleKeyFrameAnimation::addKeyProperty(const SVIParticleKeyFrameProperty& keyFrameProperty) {
		ParticleKeyFramePropertyList::iterator iter = mKeyFrameList.begin();
		for(iter; iter != mKeyFrameList.end(); ++iter) {
			if( (*iter).mKeyTime == keyFrameProperty.mKeyTime ) {
				(*iter).mProperty = keyFrameProperty.mProperty;
				return;
			}
		}

		mKeyFrameList.push_back(keyFrameProperty);
		std::sort( mKeyFrameList.begin(), mKeyFrameList.end(), SVIParticleKeyFrameProperty::compare );
	}

	SVIAnimation* SVIParticleKeyFrameAnimation::clone() {
		SVIParticleKeyFrameAnimation* cloneAnimation = new SVIParticleKeyFrameAnimation(mSVIGLSurface);
		cloneAnimation->copy(this);

		return (SVIAnimation*)cloneAnimation;
	}


	void SVIParticleKeyFrameAnimation::keyReset() {
		mKeyFrameList.clear();
	}


	SVIBool SVIParticleKeyFrameAnimation::findKeyFrame() {

		SVIInt frameCount = mKeyFrameList.size();

		if( frameCount < 2 )
			return SVIFALSE;

		SVIParticleKeyFrameProperty* fromKey = NULL;
		SVIParticleKeyFrameProperty* toKey = NULL;

		if( mTimeRatio <= 0.0f ) {
			fromKey = &mKeyFrameList[0];
			toKey = &mKeyFrameList[1];
		} else if( mTimeRatio >= 1.0f ) {
			fromKey = &mKeyFrameList[0];
			toKey = &mKeyFrameList[frameCount-1];
		} else {
			for(int i=0; i<frameCount; ++i) {
				if( mKeyFrameList[i].mKeyTime <= mTimeRatio ) {
					fromKey = &mKeyFrameList[i];
					toKey = &mKeyFrameList[i+1];
				}
			}
		}

		if( mFromKey != fromKey || mToKey != toKey ) {
			mFromKey = fromKey;
			mToKey = toKey;

			mFrom = mFromKey->mProperty;
			mTo = mToKey->mProperty;

			SVIParticlePropertyAnimation::initData();
		}

		return SVITRUE;
	}

	void SVIParticleKeyFrameAnimation::calculateAndApply() {
		if( mFromKey == NULL || mToKey == NULL ) {
			LOGE("FromKey or ToKey is not defined!!!");
			return;
		}

		//calculate ratio between fromkey and tokey and apply newratio to PropertyAnimation
		SVIFloat gapTime = mToKey->mKeyTime - mFromKey->mKeyTime;
		SVIFloat newRatio = (mTimeRatio - mFromKey->mKeyTime) / gapTime;

		DEBUG_CHECK_LOGI("KeyFrame TimeRatio : %f, newRatio : %f", mTimeRatio, newRatio);
		SVIParticlePropertyAnimation::calculateAndApply(newRatio);
	}

	void SVIParticleKeyFrameAnimation::checkType() {

		if( !mTypeChecked ) {
			mTypeCheckSuccess = SVITRUE;
			switch(mParticlePropertyAnimationType) {
			case PARTICLE_SCALE:
			case PARTICLE_POSITION:
			case PARTICLE_EFFECT_SCALE:
			case PARTICLE_EFFECT_POSITION:
			case PARTICLE_EFFECT_GENERATOR:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::VECTOR3);
				break;

			case PARTICLE_COLOR:
			case PARTICLE_EFFECT_COLOR:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::COLOR);
				break;
			}

			mTypeChecked = SVITRUE;

			if( !mTypeCheckSuccess )
				LOGE("One of KeyFrameList is wrong type!!! please check it!!! ");
		}
	}

	void SVIParticleKeyFrameAnimation::copy(const SVIParticleKeyFrameAnimation* rhs) {
		SVIParticlePropertyAnimation::copy(rhs);
		mKeyFrameList = rhs->mKeyFrameList;
	}

	void SVIParticleKeyFrameAnimation::print() {
		LOGI("AniClassType : ParticleKeyFrameAnimation, PropertyType : %d", mParticlePropertyAnimationType);
	}

}
