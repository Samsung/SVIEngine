#ifndef __SVI_GALLERY2EFFECTOR_H_
#define __SVI_GALLERY2EFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIGallery2Effector : public SVITransitionEffector {
	public:
		SVIGallery2Effector(SVIGLSurface *surface);
		virtual ~SVIGallery2Effector() {}

	protected:
		void setAnimation();
        void addRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle);
        void addZPosAni(SVIAnimationSet* aniSet, SVIFloat zPosition);
	private:
		SVIFloat rotationAngle;
	};

} 
#endif