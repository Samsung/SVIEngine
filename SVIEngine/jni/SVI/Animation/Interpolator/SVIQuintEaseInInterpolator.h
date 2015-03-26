#ifndef __SVI_QUINTEASEIN_INTERPOLATOR_H_
#define __SVI_QUINTEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuintEaseInInterpolator : public SVIInterpolator {

	public :

		SVIQuintEaseInInterpolator() { snprintf(mName, 64,"QuintEaseInInterpolator"); }
		virtual ~SVIQuintEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * (input /= 1) * input * input * input * input + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuintEaseInInterpolator* cloneObj = new SVIQuintEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUINTEASEIN_INTERPOLATOR_H_