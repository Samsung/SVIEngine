#ifndef __SVI_PARTICLE_KEYFRAME_PROPERTY_H_
#define __SVI_PARTICLE_KEYFRAME_PROPERTY_H_

#include "SVIParticlePropertyAnimation.h"

namespace SVI {

	class SVIParticleKeyFrameProperty {
	public:

		SVIParticleKeyFrameProperty();
		SVIParticleKeyFrameProperty(const SVIParticleKeyFrameProperty& rhs);
		SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIFloat& rhs);
		SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIPoint& rhs);
		SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVISize& rhs);
		SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIColor& rhs);
		SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIRect& rhs);
		SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIVector3& rhs);
		SVIParticleKeyFrameProperty(const SVIFloat& keyTime, const SVIVector4& rhs);

		static bool compare(const SVIParticleKeyFrameProperty& first, const SVIParticleKeyFrameProperty& second) {
			return first.mKeyTime < second.mKeyTime;
		}

	private:

		SVIFloat				mKeyTime;
		SVIUnionProperty		mProperty;

		friend class SVIParticleKeyFrameAnimation;
	};

	/*struct ParticleKeyFrameSort {
		bool operator() ( const SVIParticleKeyFrameProperty& a, const SVIParticleKeyFrameProperty& b ) const {
			return a.mKeyTime < b.mKeyTime;
		}
	};*/

} // end namepsace SVI

#endif //__SVI_PARTICLE_KEYFRAME_PROPERTY_H_