#ifndef __SVI_BACKEASEOUT_INTERPOLATOR_H_
#define __SVI_BACKEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIBackEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIBackEaseOutInterpolator() { 
			snprintf(mName, 64,"BackEaseOutInterpolator");
			mS = 1.5f;//1.70158f;
		}
		virtual ~SVIBackEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * ((input = input / 1 - 1) * input * ((mS + 1) * input + mS) + 1) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIBackEaseOutInterpolator* cloneObj = new SVIBackEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat		mS;

	};
};

#endif //__SVI_BACKEASEOUT_INTERPOLATOR_H_