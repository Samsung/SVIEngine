#ifndef __SVI_PARTICLE_PROPERTY_ANIMATION_H_
#define __SVI_PARTICLE_PROPERTY_ANIMATION_H_

#include "SVIAnimation.h"
#include "SVIUnionProperty.h"

namespace SVI {

	class SVIParticles;
	class SVIParticleEffect;

	class SVIParticlePropertyAnimation : public SVIAnimation{
	public:
		enum ParticlePropertyAnimationType {	
			PARTICLE_NONE = 0,
			PARTICLE_SCALE = 1,
			PARTICLE_COLOR = 2,
			PARTICLE_POSITION = 3,
			PARTICLE_EFFECT_SCALE = 4,
			PARTICLE_EFFECT_COLOR = 5,
			PARTICLE_EFFECT_POSITION = 6,
			PARTICLE_EFFECT_GENERATOR = 7,
			PARTICLE_PROPERTY_ANI_COUNT
		};

		static const SVIChar* mParticlePropertyAnimationTypeString[];

		SVIParticlePropertyAnimation(SVIGLSurface* saGLSurface);
		SVIParticlePropertyAnimation(ParticlePropertyAnimationType type, SVIGLSurface* saGLSurface);
		virtual ~SVIParticlePropertyAnimation();

		virtual void update(SVIUInt time) {}
		
		virtual SVIAnimation* clone() { 
			LOGE("Critical error!!! this function should not be called!!!");
			return NULL; 
		}

		SVIUInt getPropertyType() { return (SVIUInt)mParticlePropertyAnimationType; }
		static const SVIChar* getPropertyTypeString(ParticlePropertyAnimationType type);

		void setParticleEffect(SVIParticleEffect* effect) { mParticleEffect = effect; }
		void setParticles(SVIParticles* particles) { mParticles = particles; }

	protected :
		virtual void updateInfo(SVIUInt time);

		virtual void calculateAndApply(const SVIFloat& ratio);

		virtual void initData();
		void deInitData();

		void copy(const SVIParticlePropertyAnimation* rhs);
	private:

		void calculateParticleScale(const SVIFloat &ratio);
		void calculateParticleColor(const SVIFloat &ratio);
		void calculateParticlePosition(const SVIFloat &ratio);
		void calculateParticleEffectScale(const SVIFloat &ratio);
		void calculateParticleEffectColor(const SVIFloat &ratio);
		void calculateParticleEffectPosition(const SVIFloat &ratio);
		void calculateParticleEffectGenerator(const SVIFloat &ratio);

		template<typename T>
		T calculateCurrentValue(const T& from, const T& to, const SVIFloat& ratio);

	public:
		SVIUnionProperty					mFrom;
		SVIUnionProperty					mTo;

	protected:
		ParticlePropertyAnimationType	mParticlePropertyAnimationType;

		SVIPVoid							mFromValue; 
		SVIPVoid							mToValue;

		SVIParticles*					mParticles;
		SVIParticleEffect*				mParticleEffect;

	};


	template<typename T>
	T SVIParticlePropertyAnimation::calculateCurrentValue(const T& from, const T& to, const SVIFloat& ratio) {

		T ret;

		if( ratio == 0.0f )
			ret = from;
		else if( ratio == 1.0f )
			ret = to;
		else {
			T distance = to - from;
			ret = from + (T)(distance * ratio);
		}

		return ret;
	}

} // end namespace SVI

#endif //__SVI_PARTICLE_PROPERTY_ANIMATION_H_
