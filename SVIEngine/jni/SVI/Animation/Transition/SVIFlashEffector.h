#ifndef __SVI_FLASHEFFECTOR_H_
#define __SVI_FLASHEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIFlashEffector : public SVITransitionEffector {
	public:
		SVIFlashEffector(SVIGLSurface *surface);
		virtual ~SVIFlashEffector() {}

	protected:
		void setAnimation();

	};

} 
#endif