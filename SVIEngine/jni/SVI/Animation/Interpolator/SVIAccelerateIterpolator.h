#ifndef __SVI_ACCELERATE_INTERPOLATOR_H_
#define __SVI_ACCELERATE_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIAccelerateInterpolator : public SVIInterpolator {

	public :
		SVIAccelerateInterpolator() { snprintf (mName, 64, "Accelerate"); }
		virtual ~SVIAccelerateInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) {
			SVIFloat input = 0.0f;
			preProcess(time, input);
			SVIFloat timeRatio = input * input;
			postProcess(time, timeRatio);
			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIAccelerateInterpolator* cloneObj = new SVIAccelerateInterpolator();
			cloneObj->copy(*this);

			return cloneObj;
		}

	};

};

#endif //__SVI_ACCELERATE_INTERPOLATOR_H_