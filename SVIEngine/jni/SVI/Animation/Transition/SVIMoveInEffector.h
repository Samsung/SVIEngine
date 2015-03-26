#ifndef __SVI_MOVEINEFFECTOR_H_
#define __SVI_MOVEINEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIMoveInEffector : public SVITransitionEffector {
	public:
		SVIMoveInEffector(SVIGLSurface *surface);
		virtual ~SVIMoveInEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	};

} 
#endif