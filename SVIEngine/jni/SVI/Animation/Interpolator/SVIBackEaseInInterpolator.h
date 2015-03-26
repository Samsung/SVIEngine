#ifndef __SVI_BACKEASEIN_INTERPOLATOR_H_
#define __SVI_BACKEASEIN_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIBackEaseInInterpolator : public SVIInterpolator {

	public :

		SVIBackEaseInInterpolator() { 
			snprintf(mName, 64,"BackEaseInInterpolator");
			mS = 1.5f;//1.70158f;
		}
		virtual ~SVIBackEaseInInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(1 * (input /= 1) * input * ((mS + 1) * input - mS) + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIBackEaseInInterpolator* cloneObj = new SVIBackEaseInInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat		mS;

	};
};

#endif //__SVI_BACKEASEIN_INTERPOLATOR_H_