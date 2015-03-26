#ifndef __SVI_FLYTHROUGHEPEFFECTOR_H_
#define __SVI_FLYTHROUGHEPEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIFlyThroughEffector : public SVITransitionEffector {
	public:
		SVIFlyThroughEffector(SVIGLSurface *surface);
		virtual ~SVIFlyThroughEffector() {}

	protected:
		void setAnimation();

	};

} 
#endif