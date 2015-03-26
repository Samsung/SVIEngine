#ifndef __SVI_QUADEASEOUT_INTERPOLATOR_H_
#define __SVI_QUADEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuadEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIQuadEaseOutInterpolator() { snprintf(mName, 64,"QuadEaseOutInterpolator"); }
		virtual ~SVIQuadEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(-1 * (input /= 1) * (input - 2) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuadEaseOutInterpolator* cloneObj = new SVIQuadEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUADEASEOUT_INTERPOLATOR_H_