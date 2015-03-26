#ifndef __SVI_BREAKEFFECTOR_H_
#define __SVI_BREAKEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIBreakEffector : public SVITransitionEffector {
	public:
		SVIBreakEffector(SVIGLSurface *surface);
		virtual ~SVIBreakEffector() {}

	protected:
		void setAnimation();
	private:
		void setOffsetDuration(SVIInt fullTimeDuration);

		SVIInt rowCount;
		SVIInt columnCount;

	};

} 
#endif