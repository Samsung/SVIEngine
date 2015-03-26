#ifndef __SVI_WAVEEFFECTOR_H_
#define __SVI_WAVEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIWaveEffector : public SVITransitionEffector {
	public:
		SVIWaveEffector(SVIGLSurface *surface);
		virtual ~SVIWaveEffector() {}

	protected:
		void setAnimation();



	};

} 
#endif