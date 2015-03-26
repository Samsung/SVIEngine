#ifndef __SVI_BACKEASEINOUT_INTERPOLATOR_H_
#define __SVI_BACKEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIBackEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIBackEaseInOutInterpolator() { 
			snprintf(mName, 64,"BackEaseInOutInterpolator");
			mS = 1.5f;//1.70158f;
		}
		virtual ~SVIBackEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat s2 = mS;
			SVIFloat r;
			if((input /= (1 * 0.5f)) < 1) {
				r = 1 * 0.5f * (input * input * (((s2 *= (1.525f)) + 1) * input - mS)) + 0;
			} else {
				r = 1.0f/2.0f * ((input -= 2.0f) * input * (((s2 *= (1.525f)) + 1.0f) * input + mS) + 2.0f) + 0;
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIBackEaseInOutInterpolator* cloneObj = new SVIBackEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat		mS;

	};
};

#endif //__SVI_BACKEASEINOUT_INTERPOLATOR_H_