#ifndef __SVI_SWOOSHEFFECTOR_H_
#define __SVI_SWOOSHEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVISwooshEffector : public SVITransitionEffector {
	public:
		SVISwooshEffector(SVIGLSurface *surface);
		virtual ~SVISwooshEffector() {}

	protected:
		void setAnimation();
	};

} 
#endif