#ifndef __SVI_ROTATECUBEEFFECTOR_H_
#define __SVI_ROTATECUBEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIRotateCubeEffector : public SVITransitionEffector {
	public:
		SVIRotateCubeEffector(SVIGLSurface *surface);
		virtual ~SVIRotateCubeEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat rotationAngle;
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	};

} 
#endif