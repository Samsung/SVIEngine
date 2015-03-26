#ifndef __SVI_ANTICIPATE_OVERSHOOT_INTERPOLATOR_H_
#define __SVI_ANTICIPATE_OVERSHOOT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIAnticipateOvershootInterpolator : public SVIInterpolator {

	public :
		SVIAnticipateOvershootInterpolator() { 
			snprintf (mName,64, "AnticipateOvershoot"); 
			mTension = 2.0f * 1.5f;
		}
		virtual ~SVIAnticipateOvershootInterpolator() {}


		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);
			// a(t, s) = t * t * ((s + 1) * t - s)
			// o(t, s) = t * t * ((s + 1) * t + s)
			// f(t) = 0.5 * a(t * 2, tension * extraTension), when t < 0.5
			// f(t) = 0.5 * (o(t * 2 - 2, tension * extraTension) + 2), when t <= 1.0
			SVIFloat timeRatio = 0.0f;
			if (input < 0.5f) 
				timeRatio = 0.5f * a(input * 2.0f, mTension);
			else 
				timeRatio = 0.5f * (o(input * 2.0f - 2.0f, mTension) + 2.0f);

			postProcess(time, timeRatio);

			return timeRatio;


		}

		SVIInterpolator* clone() {
			SVIAnticipateOvershootInterpolator* cloneObj = new SVIAnticipateOvershootInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat a(SVIFloat t, SVIFloat s) {
			return t * t * ((s + 1) * t - s);
		}

		SVIFloat o(SVIFloat t, SVIFloat s) {
			return t * t * ((s + 1) * t + s);
		}

	private:
		SVIFloat		mTension;

	};

};

#endif //__SVI_ANTICIPATE_OVERSHOOT_INTERPOLATOR_H_