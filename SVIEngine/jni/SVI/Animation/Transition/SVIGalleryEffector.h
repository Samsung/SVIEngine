#ifndef __SVI_GALLERYEFFECTOR_H_
#define __SVI_GALLERYEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIGalleryEffector : public SVITransitionEffector {
	public:
		SVIGalleryEffector(SVIGLSurface *surface);
		virtual ~SVIGalleryEffector() {}

	protected:
		void setAnimation();
        void addRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle);
	private:
		SVIFloat rotationAngle;
	};

} 
#endif