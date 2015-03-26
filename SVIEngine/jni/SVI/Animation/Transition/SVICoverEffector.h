#ifndef __SVI_COVEREFFECTOR_H_
#define __SVI_COVEREFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVICoverEffector : public SVITransitionEffector {
	public:
		SVICoverEffector(SVIGLSurface *surface);
		virtual ~SVICoverEffector() {}

	protected:
		void setAnimation();
	private:

	};

}
#endif
