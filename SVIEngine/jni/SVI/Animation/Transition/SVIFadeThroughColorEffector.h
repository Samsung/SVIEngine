#ifndef __SVI_FADETHROUGHCOLOREFFECTOR_H_
#define __SVI_FADETHROUGHCOLOREFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIFadeThroughColorEffector : public SVITransitionEffector {
	public:
		SVIFadeThroughColorEffector(SVIGLSurface *surface);
		virtual ~SVIFadeThroughColorEffector() {}

	protected:
		void setAnimation();
	private:

	};

}
#endif
