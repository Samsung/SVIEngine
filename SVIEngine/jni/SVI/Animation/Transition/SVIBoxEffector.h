#ifndef __SVI_BOXEFFECTOR_H_
#define __SVI_BOXEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIBoxEffector : public SVITransitionEffector {
	public:
		SVIBoxEffector(SVIGLSurface *surface);
		virtual ~SVIBoxEffector() {}

	protected:
		void setAnimation();
		void addZPosAni(SVIAnimationSet* aniSet, SVIFloat zPosition, SVIInt duration, SVIInt offset);
	private:
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	};

}
#endif
