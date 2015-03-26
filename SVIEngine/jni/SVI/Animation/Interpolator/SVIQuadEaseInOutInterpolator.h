#ifndef __SVI_QUADEASEINOUT_INTERPOLATOR_H_
#define __SVI_QUADEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuadEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIQuadEaseInOutInterpolator() { snprintf(mName, 64,"QuadEaseInOutInterpolator"); }
		virtual ~SVIQuadEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			if((input /= 1 * 0.5f) < 1) {
				r = 1 * 0.5f * input * input + 0;
			}else{
				r = -1 * 0.5f * ((--input) * (input - 2) - 1) + 0;

			}

			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuadEaseInOutInterpolator* cloneObj = new SVIQuadEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUADEASEINOUT_INTERPOLATOR_H_