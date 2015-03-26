#ifndef __SVI_STRONGEASEIN_INTERPOLATOR_H_
#define __SVI_STRONGEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIStrongEaseInInterpolator : public SVIInterpolator {

	public :

		SVIStrongEaseInInterpolator() { snprintf(mName, 64,"StrongEaseInInterpolator"); }
		virtual ~SVIStrongEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * (input /= 1) * input * input * input * input + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIStrongEaseInInterpolator* cloneObj = new SVIStrongEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_STRONGEASEIN_INTERPOLATOR_H_