#ifndef __SVI_EXPLOIDEFFECTOR_H_
#define __SVI_EXPLOIDEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIExploidEffector : public SVITransitionEffector {
	public:
		SVIExploidEffector(SVIGLSurface *surface);
		virtual ~SVIExploidEffector() {}

	protected:
		void setAnimation();

	};

} 
#endif