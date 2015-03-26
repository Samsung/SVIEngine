#ifndef __SVI_ACCELERATE_DECELERATE_INTERPOLATOR_H_
#define __SVI_ACCELERATE_DECELERATE_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIAccelerateDecelerateInterpolator : public SVIInterpolator {

	public :
		SVIAccelerateDecelerateInterpolator() { snprintf(mName, 64,"AccelerateDecelerateInterpolator"); }
		virtual ~SVIAccelerateDecelerateInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(cos((input + 1) * SVI_PI) * 0.5f) + 0.5f;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIAccelerateDecelerateInterpolator* cloneObj = new SVIAccelerateDecelerateInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	};
};

#endif //__SVI_ACCELERATE_DECELERATE_INTERPOLATOR_H_