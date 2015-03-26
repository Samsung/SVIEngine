#ifndef __SVI_PARTICLE_KEYFRAME_ANIMATION_H_
#define __SVI_PARTICLE_KEYFRAME_ANIMATION_H_

#include "SVIParticlePropertyAnimation.h"
#include "SVIParticleKeyFrameProperty.h"
#include <vector>

namespace SVI {

	class SVIParticles;

	typedef	std::vector<SVIParticleKeyFrameProperty>		ParticleKeyFramePropertyList;

	class SVIParticleKeyFrameAnimation : public SVIParticlePropertyAnimation {
	public:
		virtual ~SVIParticleKeyFrameAnimation();

		static SVIParticleKeyFrameAnimation* create(ParticlePropertyAnimationType type, SVIGLSurface* saGLSurface);

		void update(SVIUInt& time);
		void addKeyProperty(const SVIParticleKeyFrameProperty& keyFrameProperty);
		void print();
		virtual SVIAnimation* clone();
		void keyReset();

	protected :
		
		void calculateAndApply(const SVIFloat& ratio) {}

		void calculateAndApply();
		void copy(const SVIParticleKeyFrameAnimation* rhs);

	private:
		SVIParticleKeyFrameAnimation(SVIGLSurface* saGLSurface);
		SVIParticleKeyFrameAnimation(ParticlePropertyAnimationType type, SVIGLSurface* saGLSurface);
		SVIParticleKeyFrameAnimation(const SVIParticleKeyFrameAnimation* rhs);
		SVIBool findKeyFrame();
		void checkType();


		ParticleKeyFramePropertyList		mKeyFrameList;
		SVIParticleKeyFrameProperty*			mFromKey;
		SVIParticleKeyFrameProperty*			mToKey;
		SVIBool								mTypeChecked;
		SVIBool								mTypeCheckSuccess;
	};

} // end namespace SVI

#endif //__SVI_PARTICLE_KEYFRAME_ANIMATION_H_