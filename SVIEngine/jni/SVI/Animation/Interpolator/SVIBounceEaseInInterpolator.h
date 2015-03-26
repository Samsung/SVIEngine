#ifndef __SVI_BOUNCEEASEIN_INTERPOLATOR_H_
#define __SVI_BOUNCEEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIBounceEaseInInterpolator : public SVIInterpolator {

	public :

		SVIBounceEaseInInterpolator() { 
			snprintf(mName, 64,"BounceEaseInInterpolator");
			mS = 1.5f;//1.70158f;
		}
		virtual ~SVIBounceEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 - easeOut(1-input, 0, 1, 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIBounceEaseInInterpolator* cloneObj = new SVIBounceEaseInInterpolator();
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

	private:
		SVIFloat		mS;

	};
};

#endif //__SVI_BOUNCEEASEIN_INTERPOLATOR_H_