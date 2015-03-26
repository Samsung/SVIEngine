#ifndef __SVI_TWISTEFFECTOR_H_
#define __SVI_TWISTEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVITwistEffector : public SVITransitionEffector {
	public:
		SVITwistEffector(SVIGLSurface *surface);
		virtual ~SVITwistEffector() {}

	protected:
		void setAnimation();

	};

} 
#endif