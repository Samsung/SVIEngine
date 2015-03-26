#ifndef __SVI_KEYFRAME_PARTICLES_H_
#define __SVI_KEYFRAME_PARTICLES_H_

#include "SVIParticles.h"

namespace SVI {

	// Creation Date	: 2012/06/30 rareboy0112@facebook.com
	// Description		: particles contain class(use SVIKeyFrameParticleEffect)
	// Last Modified	: 2012/06/30 rareboy0112@facebook.com
	class SVIKeyFrameParticles : public SVIParticles {
	public:
		
	public:
		SVIKeyFrameParticles(SVIGLSurface* surface);
		
		virtual void			update(SVIUInt drawTime) {}
		virtual void			update(SVIUInt drawTime, SVIUInt effectStartTime, SVIUInt effectDuration);
		virtual void			updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration);
		
		virtual SVIParticles*	clone();

		virtual void			validate();

		void					setKeyTime(SVIFloat keyTime);

		void					copy(const SVIKeyFrameParticles* rhs);

		void					setGeneratorPosition(SVIVector3 pos);

	protected:
		SVIFloat		mKeyTime;
			
		SVIVector3	mGeneratorPosition;
	};

} // end namespace SVI

#endif //__SVI_KEYFRAME_PARTICLES_H_
