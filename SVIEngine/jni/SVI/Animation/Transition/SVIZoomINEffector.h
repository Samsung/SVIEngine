#ifndef __SVI_ZoomINEFFECTOR_H_
#define __SVI_ZoomINEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIZoomINEffector : public SVITransitionEffector {
	public:
		SVIZoomINEffector(SVIGLSurface *surface);
		virtual ~SVIZoomINEffector() {}

	protected:
		void setAnimation();
	};

} 
#endif