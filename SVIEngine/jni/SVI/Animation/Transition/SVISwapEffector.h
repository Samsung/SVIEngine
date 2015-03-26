#ifndef __SVI_SWAPEFFECTOR_H_
#define __SVI_SWAPEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVISwapEffector : public SVITransitionEffector {
	public:
		SVISwapEffector(SVIGLSurface *surface);
		virtual ~SVISwapEffector() {}

	protected:
		void setAnimation();
		void addRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle);

	};

} 
#endif