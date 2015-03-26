#ifndef __SVI_SINEEASEOUT_INTERPOLATOR_H_
#define __SVI_SINEEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVISineEaseOutInterpolator : public SVIInterpolator {

	public :

		SVISineEaseOutInterpolator() { snprintf(mName, 64,"SineEaseOutInterpolator"); }
		virtual ~SVISineEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * sin(input / 1 * SVI_PI * 0.5f) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVISineEaseOutInterpolator* cloneObj = new SVISineEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_SINEEASEOUT_INTERPOLATOR_H_