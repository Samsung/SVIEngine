#ifndef __SVI_FLIPEFFECTOR_H_
#define __SVI_FLIPEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIFlipEffector : public SVITransitionEffector {
	public:
		SVIFlipEffector(SVIGLSurface *surface);
		virtual ~SVIFlipEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat rotationAngle;

	};

} 
#endif