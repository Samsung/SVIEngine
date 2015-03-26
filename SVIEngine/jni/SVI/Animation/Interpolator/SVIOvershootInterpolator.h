#ifndef __SVI_OVERSHOOT_INTERPOLATOR_H_
#define __SVI_OVERSHOOT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIOvershootInterpolator : public SVIInterpolator {

	public :
		SVIOvershootInterpolator() { 
			snprintf(mName, 64, "Overshoot"); 
			mTension = 2.0f;
		}
		virtual ~SVIOvershootInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			// _o(t) = t * t * ((tension + 1) * t + tension)
			// o(t) = _o(t - 1) + 1
			SVIFloat timeRatio = 0.0f;

			input -= 1.0f;
			timeRatio = input * input * ((mTension + 1) * input + mTension) + 1.0f;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIOvershootInterpolator* cloneObj = new SVIOvershootInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat bounce(const SVIFloat& t) {
			return t * t * 8.0f;
		}

	private:
		SVIFloat		mTension;

	};

};

#endif //__SVI_OVERSHOOT_INTERPOLATOR_H_