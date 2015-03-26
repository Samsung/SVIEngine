#ifndef __SVI_ROTATEEFFECTOR_H_
#define __SVI_ROTATEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIRotateEffector : public SVITransitionEffector {
	public:
		SVIRotateEffector(SVIGLSurface *surface);
		virtual ~SVIRotateEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat rotationAngle;

	};

} 
#endif