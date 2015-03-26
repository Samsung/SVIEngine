#ifndef __SVI_KEYFRAME_PARTICLE_EFFECT_H_
#define __SVI_KEYFRAME_PARTICLE_EFFECT_H_

#include "../SVICores.h"
#include "SVIParticleEffect.h"
#include "SVIKeyFrameParticles.h"
#include <list>
#include <string>

namespace SVI {

	// Creation Date	: 2012/06/30 rareboy0112@facebook.com
	// Description		: able keyframe create particle effect class
	// Last Modified	: 2012/07/02 rareboy0112@facebook.com
	class SVIKeyFrameParticleEffect : public SVIParticleEffect {
	
	public:
		SVIKeyFrameParticleEffect(SVIGLSurface* surface);
		virtual ~SVIKeyFrameParticleEffect();

		virtual void				update(SVIUInt drawTime);
		virtual void				updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration);
		
		virtual SVIParticleEffect*	clone();

		void						addParticles(SVIFloat keyTime, SVIParticles* particles);

		void						setGeneratorPosition(SVIVector3 pos);

		void						setGeneratorPositionKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset);

		void						addGeneratorPositionKeyFrame(SVIFloat keyTime, SVIVector3 pos);

	protected:
		void						copy(const SVIKeyFrameParticleEffect* rhs);

		SVIParticleKeyFrameAnimation*	mGeneratorPositionKeyFrameAnimation;
		SVIVector3						mGeneratorPosition;
	private:   
		SVIKeyFrameParticleEffect(const SVIKeyFrameParticleEffect& rhs);
		void operator=(const SVIKeyFrameParticleEffect& rhs){copy(&rhs);}
	};

}; // end namespace SVI

#endif //__SVI_KEYFRAME_PARTICLE_EFFECT_H_
