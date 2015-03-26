#ifndef __SVI_CUBICEASNOUT_INTERPOLATOR_H_
#define __SVI_CUBICEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVICubicEaseOutInterpolator : public SVIInterpolator {

	public :

		SVICubicEaseOutInterpolator() { snprintf(mName, 64,"CubicEaseOutInterpolator"); }
		virtual ~SVICubicEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * ((input = input / 1 - 1) * input * input + 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVICubicEaseOutInterpolator* cloneObj = new SVICubicEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_CUBICEASEOUT_INTERPOLATOR_H_