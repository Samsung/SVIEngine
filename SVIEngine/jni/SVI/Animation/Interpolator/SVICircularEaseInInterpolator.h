#ifndef __SVI_CIRCULAREASEIN_INTERPOLATOR_H_
#define __SVI_CIRCULAREASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVICircularEaseInInterpolator : public SVIInterpolator {

	public :

		SVICircularEaseInInterpolator() { snprintf(mName, 64,"CircularEaseInInterpolator"); }
		virtual ~SVICircularEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(-1 * (sqrt(1 - (input /= 1) * input) - 1.0f) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVICircularEaseInInterpolator* cloneObj = new SVICircularEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_CIRCULAREASEIN_INTERPOLATOR_H_