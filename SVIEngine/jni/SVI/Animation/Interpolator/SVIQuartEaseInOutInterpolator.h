#ifndef __SVI_QUARTEASEINOUT_INTERPOLATOR_H_
#define __SVI_QUARTEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuartEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIQuartEaseInOutInterpolator() { snprintf(mName, 64,"QuartEaseInOutInterpolator"); }
		virtual ~SVIQuartEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if((input /= 1 * 0.5f) < 1) {
				r = 1 * 0.5f * input * input * input * input + 0;
			}else{
				r = -1 * 0.5f * ((input -= 2) * input * input * input - 2) + 0;
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuartEaseInOutInterpolator* cloneObj = new SVIQuartEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUARTEASEINOUT_INTERPOLATOR_H_