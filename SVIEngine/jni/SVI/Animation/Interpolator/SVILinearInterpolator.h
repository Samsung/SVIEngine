#ifndef __SVI_LINEAR_INTERPOLATOR_H_
#define __SVI_LINEAR_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVILinearInterpolator : public SVIInterpolator{

	public :
		SVILinearInterpolator() { snprintf(mName, 64, "Linear"); }
		virtual ~SVILinearInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = input;

			postProcess(time, timeRatio);
			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVILinearInterpolator* cloneObj = new SVILinearInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	};

};

#endif //__SVI_LINEAR_INTERPOLATOR_H_