#ifndef __SVI_BRICKCUBEEFFECTOR_H_
#define __SVI_BRICKCUBEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIBrickCubeEffector : public SVITransitionEffector {
	public:
		SVIBrickCubeEffector(SVIGLSurface *surface);
		virtual ~SVIBrickCubeEffector() {}

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	protected:
		void setAnimation();
	};

} 
#endif