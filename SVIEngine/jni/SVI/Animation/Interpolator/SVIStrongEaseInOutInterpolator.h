#ifndef __SVI_STRONGEASEINOUT_INTERPOLATOR_H_
#define __SVI_STRONGEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIStrongEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIStrongEaseInOutInterpolator() { snprintf(mName, 64,"StrongEaseInOutInterpolator"); }
		virtual ~SVIStrongEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if((input /= 1 * 0.5f) < 1) {
				r = 1 * 0.5f * input * input * input * input * input + 0;
			}else{
				r = 1 * 0.5f * ((input -= 2) * input * input * input * input + 2) + 0;
			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIStrongEaseInOutInterpolator* cloneObj = new SVIStrongEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_STRONGEASEINOUT_INTERPOLATOR_H_