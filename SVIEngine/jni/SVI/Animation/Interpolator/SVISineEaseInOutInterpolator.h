#ifndef __SVI_SINEEASEINOUT_INTERPOLATOR_H_
#define __SVI_SINEEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVISineEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVISineEaseInOutInterpolator() { snprintf(mName, 64,"SineEaseInOutInterpolator"); }
		virtual ~SVISineEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(-1 * 0.5f * (cos(SVI_PI * input / 1) - 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVISineEaseInOutInterpolator* cloneObj = new SVISineEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_SINEEASEINOUT_INTERPOLATOR_H_