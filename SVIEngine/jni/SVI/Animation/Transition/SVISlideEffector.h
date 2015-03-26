#ifndef __SVI_SLIDEEFFECTOR_H_
#define __SVI_SLIDEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVISlideEffector : public SVITransitionEffector {
	public:
		SVISlideEffector(SVIGLSurface *surface);
		virtual ~SVISlideEffector() {}

	protected:
		void setAnimation();
	private:
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	};

} 
#endif