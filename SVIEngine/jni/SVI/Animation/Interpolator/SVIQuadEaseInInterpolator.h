#ifndef __SVI_QUADEASEIN_INTERPOLATOR_H_
#define __SVI_QUADEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIQuadEaseInInterpolator : public SVIInterpolator {

	public :

		SVIQuadEaseInInterpolator() { snprintf(mName, 64,"QuadEaseInInterpolator"); }
		virtual ~SVIQuadEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * (input /= 1) * input + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIQuadEaseInInterpolator* cloneObj = new SVIQuadEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_QUADEASEIN_INTERPOLATOR_H_