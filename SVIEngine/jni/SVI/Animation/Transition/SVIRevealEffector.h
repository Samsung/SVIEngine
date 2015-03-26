#ifndef __SVI_REVEALEFFECTOR_H_
#define __SVI_REVEALEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIRevealEffector : public SVITransitionEffector {
	public:
		SVIRevealEffector(SVIGLSurface *surface);
		virtual ~SVIRevealEffector() {}

	protected:
		void setAnimation();
	};

} 
#endif