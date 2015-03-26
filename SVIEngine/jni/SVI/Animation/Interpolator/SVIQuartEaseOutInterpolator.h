#ifndef __SVI_QUARTEASEOUT_INTERPOLATOR_H_
#define __SVI_QUARTEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuartEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIQuartEaseOutInterpolator() { snprintf(mName, 64,"QuartEaseOutInterpolator"); }
		virtual ~SVIQuartEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(-1 * ((input = input / 1 - 1) * input * input * input - 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuartEaseOutInterpolator* cloneObj = new SVIQuartEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUARTEASEOUT_INTERPOLATOR_H_