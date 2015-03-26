#ifndef __SVI_ANTICIPATE_INTERPOLATOR_H_
#define __SVI_ANTICIPATE_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVIAnticipateInterpolator : public SVIInterpolator {

	public :
		SVIAnticipateInterpolator() { 
			snprintf (mName, 64, "Anticipate"); 
			mTension = 2.0f;
		}
		virtual ~SVIAnticipateInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (float)(input * input * ((mTension + 1.0f) * input - mTension));

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVIAnticipateInterpolator* cloneObj = new SVIAnticipateInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat		mTension;

	};

};

#endif //__SVI_DECELERATE_INTERPOLATOR_H_