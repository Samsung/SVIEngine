#ifndef __SVI_TUMBLEEFFECTOR_H_
#define __SVI_TUMBLEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVITumbleEffector : public SVITransitionEffector {
	public:
		SVITumbleEffector(SVIGLSurface *surface);
		virtual ~SVITumbleEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat rotationAngle;
		SVIFloat slideWidth;
		SVIFloat slideHeight;
	};

} 
#endif