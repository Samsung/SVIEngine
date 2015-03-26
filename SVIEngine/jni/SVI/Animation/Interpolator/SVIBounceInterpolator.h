#ifndef __SVI_BOUNCE_INTERPOLATOR_H_
#define __SVI_BOUNCE_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIBounceInterpolator : public SVIInterpolator {

	public :
		SVIBounceInterpolator() { 
			snprintf (mName, 64, "Bounce"); 
		}
		virtual ~SVIBounceInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			// _b(t) = t * t * 8
			// bs(t) = _b(t) for t < 0.3535
			// bs(t) = _b(t - 0.54719) + 0.7 for t < 0.7408
			// bs(t) = _b(t - 0.8526) + 0.9 for t < 0.9644
			// bs(t) = _b(t - 1.0435) + 0.95 for t <= 1.0
			// b(t) = bs(t * 1.1226)
			SVIFloat timeRatio = 0.0f;
			input *= 1.1226f;
			if (input < 0.3535f) timeRatio = bounce(input);
			else if (input < 0.7408f) timeRatio = bounce(input - 0.54719f) + 0.7f;
			else if (input < 0.9644f) timeRatio = bounce(input - 0.8526f) + 0.9f;
			else timeRatio = bounce(input - 1.0435f) + 0.95f;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIBounceInterpolator* cloneObj = new SVIBounceInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat bounce(const SVIFloat& t) {
			return t * t * 8.0f;
		}
	};

};

#endif //__SVI_BOUNCE_INTERPOLATOR_H_