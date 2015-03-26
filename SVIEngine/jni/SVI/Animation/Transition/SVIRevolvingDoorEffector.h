#ifndef __SVI_REVOLVINGDOOREFFECTOR_H_
#define __SVI_REVOLVINGDOOREFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIRevolvingDoorEffector : public SVITransitionEffector {
	public:
		SVIRevolvingDoorEffector(SVIGLSurface *surface);
		virtual ~SVIRevolvingDoorEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat rotationAngle;

	};

}
#endif
