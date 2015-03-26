#ifndef __SVI_CUBICEASEIN_INTERPOLATOR_H_
#define __SVI_CUBICEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVICubicEaseInInterpolator : public SVIInterpolator {

	public :

		SVICubicEaseInInterpolator() { snprintf(mName, 64,"CubicEaseInInterpolator"); }
		virtual ~SVICubicEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * (input /= 1) * input * input + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVICubicEaseInInterpolator* cloneObj = new SVICubicEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_CUBICEASEIN_INTERPOLATOR_H_