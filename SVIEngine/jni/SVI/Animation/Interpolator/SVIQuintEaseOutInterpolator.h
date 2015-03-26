#ifndef __SVI_QUINTEASEOUT_INTERPOLATOR_H_
#define __SVI_QUINTEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuintEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIQuintEaseOutInterpolator() { snprintf(mName, 64,"QuintEaseOutInterpolator"); }
		virtual ~SVIQuintEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * ((input = input / 1 - 1) * input * input * input * input + 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuintEaseOutInterpolator* cloneObj = new SVIQuintEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUINTEASEOUT_INTERPOLATOR_H_