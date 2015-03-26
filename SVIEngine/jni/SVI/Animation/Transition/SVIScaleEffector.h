#ifndef __SVI_SCAILEFFECTOR_H_
#define __SVI_SCAILEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIScaleEffector : public SVITransitionEffector {
	public:
		SVIScaleEffector(SVIGLSurface *surface);
		virtual ~SVIScaleEffector() {}

	protected:
		void setAnimation();
	private:
		SVIVector3 scaleValue;
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	};

} 
#endif