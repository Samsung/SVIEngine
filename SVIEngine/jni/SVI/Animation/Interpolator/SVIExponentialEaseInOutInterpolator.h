#ifndef __SVI_EXPONENTIALEASEINOUT_INTERPOLATOR_H_
#define __SVI_EXPONENTIALEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIExponentialEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIExponentialEaseInOutInterpolator() { snprintf(mName, 64,"ExponentialEaseInOutInterpolator"); }
		virtual ~SVIExponentialEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if(input == 0) {
				r = 0;
			}else if(input == 1) {
				r = 0 + 1;
			}else if((input /= 1 * 0.5f) < 1) {
				r = (SVIFloat) (1 * 0.5f * pow(2, 10 * (input - 1)) + 0);
			}else{
				r = (SVIFloat) (1 * 0.5f * (-pow(2, -10 * --input) + 2) + 0);
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIExponentialEaseInOutInterpolator* cloneObj = new SVIExponentialEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_EXPONENTIALEASEINOUT_INTERPOLATOR_H_