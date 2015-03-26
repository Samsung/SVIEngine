#ifndef __SVI_SMOOTHEFFECTOR_H_
#define __SVI_SMOOTHEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVISmoothEffector : public SVITransitionEffector {
	public:
		SVISmoothEffector(SVIGLSurface *surface);
		virtual ~SVISmoothEffector() {}

	protected:
		void setAnimation();
	};

} 
#endif