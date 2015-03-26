#ifndef __SVI_CIRCULAREASEINOUT_INTERPOLATOR_H_
#define __SVI_CIRCULAREASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVICircularEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVICircularEaseInOutInterpolator() { snprintf(mName, 64,"CircularEaseInOutInterpolator"); }
		virtual ~SVICircularEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;

			if((input /= 1 * 0.5) < 1) {
				r = (float) (-1 * 0.5f * (sqrt(1 - input * input) - 1) + 0);
			}else{
				r = (float) (1 * 0.5f * (sqrt(1 - (input -= 2) * input) + 1) + 0);
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVICircularEaseInOutInterpolator* cloneObj = new SVICircularEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_CIRCULAREASEINOUT_INTERPOLATOR_H_