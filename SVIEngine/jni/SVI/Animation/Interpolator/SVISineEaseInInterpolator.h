#ifndef __SVI_SINEEASEIN_INTERPOLATOR_H_
#define __SVI_SINEEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVISineEaseInInterpolator : public SVIInterpolator {

	public :

		SVISineEaseInInterpolator() { snprintf(mName, 64,"SineEaseInInterpolator"); }
		virtual ~SVISineEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(-1 * cos(input / 1 * SVI_PI * 0.5f) + 1 + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVISineEaseInInterpolator* cloneObj = new SVISineEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_SINEEASEIN_INTERPOLATOR_H_