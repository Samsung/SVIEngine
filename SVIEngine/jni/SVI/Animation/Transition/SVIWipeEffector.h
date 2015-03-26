#ifndef __SVI_WIPEEFFECTOR_H_
#define __SVI_WIPEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIWipeEffector : public SVITransitionEffector {
	public:
		SVIWipeEffector(SVIGLSurface *surface);
		virtual ~SVIWipeEffector() {}

	protected:
		void setAnimation();
	};

} 
#endif