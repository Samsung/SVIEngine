#ifndef __SVI_STRONGEASEOUT_INTERPOLATOR_H_
#define __SVI_STRONGEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIStrongEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIStrongEaseOutInterpolator() { snprintf(mName, 64,"StrongEaseOutInterpolator"); }
		virtual ~SVIStrongEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * ((input = input / 1 - 1) * input * input * input * input + 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIStrongEaseOutInterpolator* cloneObj = new SVIStrongEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_STRONGEASEOUT_INTERPOLATOR_H_