#ifndef __SVI_FLIP2EFFECTOR_H_
#define __SVI_FLIP2EFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIFlip2Effector : public SVITransitionEffector {
	public:
		SVIFlip2Effector(SVIGLSurface *surface);
		virtual ~SVIFlip2Effector() {}

	protected:
		void setAnimation();
        void addChildScaleAni(SVIAnimationSet* aniSet, SVIVector3 scaleValue);
        void addParentRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle);
	private:
		SVIFloat rotationAngle;
        SVIFloat slideWidth;
		SVIFloat slideHeight;
	};

} 
#endif