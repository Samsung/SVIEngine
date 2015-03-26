#ifndef __SVI_EXPONENTIALEASEIN_INTERPOLATOR_H_
#define __SVI_EXPONENTIALEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIExponentialEaseInInterpolator : public SVIInterpolator {

	public :

		SVIExponentialEaseInInterpolator() { snprintf(mName, 64,"ExponentialEaseInInterpolator"); }
		virtual ~SVIExponentialEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)((input == 0) ? 0 : 1 * pow(2, 10 * (input / 1 - 1)) + 0 - 1 * 0.001f);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIExponentialEaseInInterpolator* cloneObj = new SVIExponentialEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_EXPONENTIALEASEIN_INTERPOLATOR_H_