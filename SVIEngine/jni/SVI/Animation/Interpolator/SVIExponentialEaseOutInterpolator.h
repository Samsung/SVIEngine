#ifndef __SVI_EXPONENTIALEASEOUT_INTERPOLATOR_H_
#define __SVI_EXPONENTIALEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIExponentialEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIExponentialEaseOutInterpolator() { snprintf(mName, 64,"ExponentialEaseOutInterpolator"); }
		virtual ~SVIExponentialEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)((input == 1) ? 0 + 1 : 1 * (-pow(2, -10 * input / 1) + 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIExponentialEaseOutInterpolator* cloneObj = new SVIExponentialEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_EXPONENTIALEASEOUT_INTERPOLATOR_H_