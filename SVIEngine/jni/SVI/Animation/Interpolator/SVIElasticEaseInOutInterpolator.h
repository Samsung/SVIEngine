#ifndef __SVI_ELASTICEASEINOUT_INTERPOLATOR_H_
#define __SVI_ELASTICEASEINOUT_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIElasticEaseInOutInterpolator : public SVIInterpolator {

	public :

		SVIElasticEaseInOutInterpolator() { snprintf(mName, 64,"ElasticEaseInOutInterpolator"); }
		virtual ~SVIElasticEaseInOutInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat r;
			SVIFloat s;
			SVIFloat p = 0.0f;
			SVIFloat a = 0.0f;
			if(input == 0) {
				SVIFloat timeRatio = (SVIFloat)(0);
				postProcess(time, timeRatio);
				return timeRatio;
			}
			if((input /= 1 * 0.5) == 2) {
				SVIFloat timeRatio = (SVIFloat)(0+1);
				postProcess(time, timeRatio);
				return timeRatio;
			}
			if(p == 0) {
				p = 1 * (0.3f * 1.5f);
			}
			if(a == 0 || (1 > 0 && a < 1) || (1 < 0)) {
				a = 1;
				s = p / 4.0f;
			} else {
				s = (SVIFloat) (p / SVI_PI * 2 * asin(1 / a));
			}
			if(input < 1) {
				r = (SVIFloat) (-0.5 * (a * pow(2, 10 * (input -= 1)) * sin((input * 1 - s) * SVI_PI * 2 / p)) + 0);
			}else{
				r = (SVIFloat) (a * pow(2, -10 * (input -= 1)) * sin((input * 1 - s) * SVI_PI * 2 / p) * .5 + 1 + 0);
			}


			SVIFloat timeRatio = r;

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIElasticEaseInOutInterpolator* cloneObj = new SVIElasticEaseInOutInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}
	};
};

#endif //__SVI_ELASTICEASEINOUT_INTERPOLATOR_H_