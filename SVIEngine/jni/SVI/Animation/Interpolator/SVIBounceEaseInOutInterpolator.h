#ifndef __SVI_BOUNCEEASEINOUT_INTERPOLATOR_H_
#define __SVI_BOUNCEEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIBounceEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIBounceEaseInOutInterpolator() { 
			snprintf(mName, 64,"BounceEaseInOutInterpolator");
			mS = 1.5f;//1.70158f;
		}
		virtual ~SVIBounceEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if(input < 1 * 0.5f) {
				r =  easeIn(input * 2, 0, 1, 1) * 0.5f + 0;
			} else {
				r =  easeOut(input * 2 - 1, 0, 1, 1) * 0.5f + 1 * 0.5f + 0;
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIBounceEaseInOutInterpolator* cloneObj = new SVIBounceEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

		SVIFloat easeOut(SVIFloat t, SVIFloat b, SVIFloat c, SVIFloat d) {
			if ((t/=d) < (1/2.75f)) {
				return c*(7.5625f*t*t) + b;
			} else if (t < (2/2.75f)) {
				return c*(7.5625f*(t-=(1.5f/2.75f))*t + .75f) + b;
			} else if (t < (2.5/2.75)) {
				return c*(7.5625f*(t-=(2.25f/2.75f))*t + .9375f) + b;
			} else {
				return c*(7.5625f*(t-=(2.625f/2.75f))*t + .984375f) + b;
			}
		}

		SVIFloat easeIn(SVIFloat t, SVIFloat b, SVIFloat c, SVIFloat d) {
			return c - easeOut(d-t, 0, c, d) + b;
		}



	private:
		SVIFloat		mS;

	};
};

#endif //__SVI_BOUNCEEASEINOUT_INTERPOLATOR_H_