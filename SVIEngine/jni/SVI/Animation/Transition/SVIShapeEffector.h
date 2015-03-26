#ifndef __SVI_SHAPEEFFECTOR_H_
#define __SVI_SHAPEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIShapeEffector : public SVITransitionEffector {
	public:
		SVIShapeEffector(SVIGLSurface *surface);
		virtual ~SVIShapeEffector() {}

	protected:
		void setAnimation();
	};

} 
#endif