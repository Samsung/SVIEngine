#ifndef __SVI_CIRCULAREASEOUT_INTERPOLATOR_H_
#define __SVI_CIRCULAREASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVICircularEaseOutInterpolator : public SVIInterpolator {

	public :

		SVICircularEaseOutInterpolator() { snprintf(mName, 64,"CircularEaseOutInterpolator"); }
		virtual ~SVICircularEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * sqrt(1 - (input = input / 1 - 1) * input) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVICircularEaseOutInterpolator* cloneObj = new SVICircularEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_CIRCULAREASEOUT_INTERPOLATOR_H_