#ifndef __SVI_QUARTEASEIN_INTERPOLATOR_H_
#define __SVI_QUARTEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuartEaseInInterpolator : public SVIInterpolator {

	public :

		SVIQuartEaseInInterpolator() { snprintf(mName, 64,"QuartEaseInInterpolator"); }
		virtual ~SVIQuartEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * (input /= 1) * input * input * input + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuartEaseInInterpolator* cloneObj = new SVIQuartEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUARTEASEIN_INTERPOLATOR_H_