#ifndef __SVI_BOUNCEEASEOUT_INTERPOLATOR_H_
#define __SVI_BOUNCEEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIBounceEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIBounceEaseOutInterpolator() { snprintf(mName, 64,"BounceEaseOutInterpolator"); }
		virtual ~SVIBounceEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if ((input/=1) < (1/2.75f)) {
				r = 1*(7.5625f*input*input) + 0;
			} else if (input < (2/2.75)) {
				r = 1*(7.5625f*(input-=(1.5f/2.75f))*input + .75f) + 0;
			} else if (input < (2.5/2.75)) {
				r = 1*(7.5625f*(input-=(2.25f/2.75f))*input + .9375f) + 0;
			} else {
				r = 1*(7.5625f*(input-=(2.625f/2.75f))*input + .984375f) + 0;
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIBounceEaseOutInterpolator* cloneObj = new SVIBounceEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_BOUNCEEASEOUT_INTERPOLATOR_H_