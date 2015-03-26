#include "SVIParticlePropertyAnimation.h"
#include "../Slide/SVIBeamedSlide.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Mathematics/SVIQuaternion.h"
#include "../Particle/SVIParticles.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Particle/SVIKeyFrameParticleEffect.h"

namespace SVI {

const static SVIBool DEBUG = SVIFALSE;

#define SETTING_PROPERTY_VALUE(PropertyType, DataType)	\
	case PropertyType :									\
	{													\
		if( mFromValue == NULL ) 						\
			mFromValue = (SVIPVoid*)new DataType();		\
														\
		if( mToValue == NULL ) 							\
			mToValue = (SVIPVoid*)new DataType();		\
														\
		mFrom.get( *((DataType*)mFromValue) );			\
		mTo.get( *((DataType*)mToValue) );				\
	} break


#define FREE_PROPERTY_VALUE(PropertyType, DataType)		\
	case PropertyType :									\
	{													\
		DataType* from = (DataType*)mFromValue;			\
		DataType* to = (DataType*)mToValue;				\
		SVI_SVIFE_DELETE(from);							\
		SVI_SVIFE_DELETE(to);								\
	} break

#define CALCULATE_CURRENT_VALUE(PropertyType, Name, ratio)		\
	case PropertyType :											\
		calculate##Name(ratio);									\
	break

	const SVIChar* SVIParticlePropertyAnimation::mParticlePropertyAnimationTypeString[] = {
		"PARTICLE_NONE=0",
		"PARTICLE_SCALE",
		"PARTICLE_COLOR",
		"PARTICLE_POSITION",
		"PARTICLE_EFFECT_SCALE",
		"PARTICLE_EFFECT_COLOR",
		"PARTICLE_EFFECT_POSITION",
		"PARTICLE_EFFECT_GENERATOR",
		"PARTICLE_PROPERTY_ANI_COUNT"
	};

	const SVIChar* SVIParticlePropertyAnimation::getPropertyTypeString(ParticlePropertyAnimationType type) {
		return mParticlePropertyAnimationTypeString[type];
	}

	SVIParticlePropertyAnimation::SVIParticlePropertyAnimation(SVIGLSurface* saGLSurface):SVIAnimation(saGLSurface) { 
		mParticlePropertyAnimationType = PARTICLE_NONE; 
		mFromValue = NULL;
		mToValue = NULL;
		mParticleEffect = NULL;
		mParticles = NULL;
	}

	SVIParticlePropertyAnimation::SVIParticlePropertyAnimation(ParticlePropertyAnimationType type, SVIGLSurface* saGLSurface):SVIAnimation(saGLSurface) {
		mParticlePropertyAnimationType = type;
		initData();
	}

	SVIParticlePropertyAnimation::~SVIParticlePropertyAnimation() {
		deInitData();
		mParticlePropertyAnimationType = PARTICLE_NONE;
	}

	void SVIParticlePropertyAnimation::updateInfo(SVIUInt time) {
		if( getStatus() == IDLE ) {

			initData();
			startAnimation(time);
			if( mListener != NULL )
				mListener->onAnimationStart(this);
		}

		if( getStatus() == ANIMATING ) {

			mTimeRatio = mInterpolator->getTimeInterpolation(time);
			if( mListener != NULL ) {
				SVIUInt currentCount = mInterpolator->getCurrentCount();
				// currentCount is elapsedTime / duration. So current means current animation index.
				// and RepeatCount means total count of playing animation.
				// Default RepeatCount is 1, because animation must play once.
				if( mRepeatCount > 1 && mCurrentCount != currentCount && mRepeatCount != currentCount ) {
					//LOGE("Current repeat count %d, Interpolator repeatCount %d", mCurrentCount, currentCount);
					mListener->onAnimationRepeat(this);
				}
				mCurrentCount = currentCount;
			}
		}
	}

	void SVIParticlePropertyAnimation::initData() {
		switch(mParticlePropertyAnimationType) {
			SETTING_PROPERTY_VALUE(PARTICLE_SCALE, SVIVector3);
			SETTING_PROPERTY_VALUE(PARTICLE_COLOR, SVIColor);
			SETTING_PROPERTY_VALUE(PARTICLE_POSITION, SVIVector3);
			SETTING_PROPERTY_VALUE(PARTICLE_EFFECT_SCALE, SVIVector3);
			SETTING_PROPERTY_VALUE(PARTICLE_EFFECT_COLOR, SVIColor);
			SETTING_PROPERTY_VALUE(PARTICLE_EFFECT_POSITION, SVIVector3);
			SETTING_PROPERTY_VALUE(PARTICLE_EFFECT_GENERATOR, SVIVector3);
		}

	}

	void SVIParticlePropertyAnimation::deInitData() {
		switch(mParticlePropertyAnimationType) {
			FREE_PROPERTY_VALUE(PARTICLE_SCALE, SVIVector3);
			FREE_PROPERTY_VALUE(PARTICLE_COLOR, SVIColor);
			FREE_PROPERTY_VALUE(PARTICLE_POSITION, SVIVector3);
			FREE_PROPERTY_VALUE(PARTICLE_EFFECT_SCALE, SVIVector3);
			FREE_PROPERTY_VALUE(PARTICLE_EFFECT_COLOR, SVIColor);
			FREE_PROPERTY_VALUE(PARTICLE_EFFECT_POSITION, SVIVector3);
			FREE_PROPERTY_VALUE(PARTICLE_EFFECT_GENERATOR, SVIVector3);
		}

		mFromValue = NULL;
		mToValue = NULL;
	}

	void SVIParticlePropertyAnimation::calculateAndApply(const SVIFloat& ratio) {

		DEBUG_CHECK_LOGI("SVIParticlePropertyAnimation::calculateAndApply animation type:%d, ratio:%f", mParticlePropertyAnimationType, ratio);
		switch(mParticlePropertyAnimationType) {
			CALCULATE_CURRENT_VALUE(PARTICLE_SCALE, ParticleScale, ratio);
			CALCULATE_CURRENT_VALUE(PARTICLE_COLOR, ParticleColor, ratio);
			CALCULATE_CURRENT_VALUE(PARTICLE_POSITION, ParticlePosition, ratio);
			CALCULATE_CURRENT_VALUE(PARTICLE_EFFECT_SCALE, ParticleEffectScale, ratio);
			CALCULATE_CURRENT_VALUE(PARTICLE_EFFECT_COLOR, ParticleEffectColor, ratio);
			CALCULATE_CURRENT_VALUE(PARTICLE_EFFECT_POSITION, ParticleEffectPosition, ratio);
			CALCULATE_CURRENT_VALUE(PARTICLE_EFFECT_GENERATOR, ParticleEffectGenerator, ratio);
		}
	}


	void SVIParticlePropertyAnimation::calculateParticleScale(const SVIFloat& ratio) {
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mParticles != NULL ) {
			mParticles->setChangeScale(dPoint);
		}
	}


	void SVIParticlePropertyAnimation::calculateParticleColor(const SVIFloat& ratio) {
		SVIColor& from = *(SVIColor*)(mFromValue);
		SVIColor& to = *(SVIColor*)(mToValue);

		SVIColor dColor = calculateCurrentValue(from, to, ratio);

		if( mParticles != NULL ) {
			mParticles->setChangeColor(dColor);
		}
	}


	void SVIParticlePropertyAnimation::calculateParticlePosition(const SVIFloat& ratio) {
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mParticles != NULL ) {
			mParticles->setChangePosition(dPoint);
		}
	}


	void SVIParticlePropertyAnimation::calculateParticleEffectScale(const SVIFloat& ratio) {
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mParticleEffect != NULL ) {
			mParticleEffect->setScale(dPoint);
		}
	}


	void SVIParticlePropertyAnimation::calculateParticleEffectColor(const SVIFloat &ratio) {
		SVIColor& from = *(SVIColor*)(mFromValue);
		SVIColor& to = *(SVIColor*)(mToValue);

		SVIColor dColor = calculateCurrentValue(from, to, ratio);

		if( mParticleEffect != NULL ) {
			mParticleEffect->setColor(dColor);
		}
	}


	void SVIParticlePropertyAnimation::calculateParticleEffectPosition(const SVIFloat& ratio) {
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mParticleEffect != NULL ) {
			mParticleEffect->setPosition(dPoint);
		}
	}


	void SVIParticlePropertyAnimation::calculateParticleEffectGenerator(const SVIFloat& ratio) {
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mParticleEffect != NULL ) {
			( (SVIKeyFrameParticleEffect*)mParticleEffect )->setGeneratorPosition(dPoint);
		}
	}


	void SVIParticlePropertyAnimation::copy(const SVIParticlePropertyAnimation* rhs) {
		SVIAnimation::copy(rhs);

		mParticlePropertyAnimationType = rhs->mParticlePropertyAnimationType;
		mFrom = rhs->mFrom;
		mTo = rhs->mTo;
		mParticles = rhs->mParticles;
		mParticleEffect = rhs->mParticleEffect;
		deInitData();
		initData();
	}
	
}