#ifndef __SVI_WIZZLEEFFECTOR_H_
#define __SVI_WIZZLEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIWizzleEffector : public SVITransitionEffector {
	public:
		SVIWizzleEffector(SVIGLSurface *surface);
		virtual ~SVIWizzleEffector() {}

	protected:
		void setAnimation();
		void setEffectTimeLine();

	private:
		SVIFloat mSetEffectTime;

	};

} 
#endif