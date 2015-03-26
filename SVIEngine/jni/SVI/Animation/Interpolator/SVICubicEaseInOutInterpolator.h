#ifndef __SVI_CUBICEASEINOUT_INTERPOLATOR_H_
#define __SVI_CUBICEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVICubicEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVICubicEaseInOutInterpolator() { snprintf(mName, 64,"CubicEaseInOutInterpolator"); }
		virtual ~SVICubicEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if((input /= 1 * 0.5f) < 1) {
				r =  1 * 0.5f * input * input * input + 0;
			} else {
				r = 1 * 0.5f * ((input -= 2) * input * input + 2) + 0;
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVICubicEaseInOutInterpolator* cloneObj = new SVICubicEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_CUBICEASEINOUT_INTERPOLATOR_H_