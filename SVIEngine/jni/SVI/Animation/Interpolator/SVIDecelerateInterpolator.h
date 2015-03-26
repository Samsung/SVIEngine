#ifndef __SVI_DECELERATE_INTERPOLATOR_H_
#define __SVI_DECELERATE_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIDecelerateInterpolator : public SVIInterpolator {

	public :
		SVIDecelerateInterpolator() { snprintf(mName, 64, "Decelerate"); }
		virtual ~SVIDecelerateInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);
			
			SVIFloat timeRatio = (float)(1.0f - (1.0f - input) * (1.0f - input));

			postProcess(time, timeRatio);

			return timeRatio;

			
		}

		SVIInterpolator* clone() {
			SVIDecelerateInterpolator* cloneObj = new SVIDecelerateInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	};

};

#endif //__SVI_DECELERATE_INTERPOLATOR_H_