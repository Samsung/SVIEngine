#ifndef __SVI_UNCOVEREFFECTOR_H_
#define __SVI_UNCOVEREFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIUncoverEffector : public SVITransitionEffector {
	public:
		SVIUncoverEffector(SVIGLSurface *surface);
		virtual ~SVIUncoverEffector() {}

	protected:
		void setAnimation();
	private:

	};

}
#endif
