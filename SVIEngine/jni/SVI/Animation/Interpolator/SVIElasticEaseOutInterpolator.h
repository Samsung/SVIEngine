#ifndef __SVI_ELASTICEASEOUT_INTERPOLATOR_H_
#define __SVI_ELASTICEASEOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIElasticEaseOutInterpolator : public SVIInterpolator {

	public :

		SVIElasticEaseOutInterpolator() { snprintf(mName, 64,"ElasticEaseOutInterpolator"); }
		virtual ~SVIElasticEaseOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat s;
			SVIFloat p = 0.0f;
			SVIFloat a = 0.0f;
			if(input == 0) {
				SVIFloat timeRatio = (SVIFloat)(0);
				postProcess(time, timeRatio);
				return timeRatio;
			}
			if((input /= 1) == 1) {
				SVIFloat timeRatio = (SVIFloat)(0+1);
				postProcess(time, timeRatio);
				return timeRatio;
			}
			if(p == 0) {
				p = 1 * 0.3f;
			}
			if(a == 0 || (1 > 0 && a < 1) || (1 < 0)) {
				a = 1;
				s = p / 4;
			} else {
				s = (SVIFloat) (p / SVI_PI * 2 * asin(1 / a));
			}

			SVIFloat timeRatio = (SVIFloat) (a * pow(2, -10 * input) * sin((input * 1 - s) * SVI_PI * 2 / p) + 1 + 0);

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIElasticEaseOutInterpolator* cloneObj = new SVIElasticEaseOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_ELASTICEASEOUT_INTERPOLATOR_H_