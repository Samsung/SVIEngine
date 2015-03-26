#ifndef __SVI_TWIRLEFFECTOR_H_
#define __SVI_TWIRLEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVITwirlEffector : public SVITransitionEffector {
	public:
		SVITwirlEffector(SVIGLSurface *surface);
		virtual ~SVITwirlEffector() {}

	protected:
		void setAnimation();
		void addRotAni(SVIAnimationSet* aniSet);
		void addScaleAni(SVIAnimationSet* aniSet);

	};

} 
#endif