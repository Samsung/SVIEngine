#ifndef __SVI_QUINTEASEINOUT_INTERPOLATOR_H_
#define __SVI_QUINTEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuintEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIQuintEaseInOutInterpolator() { snprintf(mName, 64,"QuintEaseInOutInterpolator"); }
		virtual ~SVIQuintEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if((input /= 1 * 0.5f) < 1) {
				r = 1 * 0.5f * input * input * input * input * input + 0;
			}else{
				r = 1 * 0.5f * ((input -= 2) * input * input * input * input + 2) + 0;
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuintEaseInOutInterpolator* cloneObj = new SVIQuintEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUINTEASEINOUT_INTERPOLATOR_H_