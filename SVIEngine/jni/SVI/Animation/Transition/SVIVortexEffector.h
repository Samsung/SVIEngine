#ifndef __SVI_VORTEXEFFECTOR_H_
#define __SVI_VORTEXEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIVortexEffector : public SVITransitionEffector {
	public:
		SVIVortexEffector(SVIGLSurface *surface);
		virtual ~SVIVortexEffector() {}

	protected:
		void setAnimation();
		
	private:
		void setOffsetDuration(SVIInt fullTimeDuration);
	private:
			SVIInt rowCount;
			SVIInt columnCount;
	};

} 
#endif