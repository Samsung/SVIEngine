#ifndef __SVI_FALLEFFECTOR_H_
#define __SVI_FALLEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIFallEffector : public SVITransitionEffector {
	public:
		SVIFallEffector(SVIGLSurface *surface);
		virtual ~SVIFallEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat rotationAngle;
		SVIVector3 scaleValue;

	};

}
#endif
