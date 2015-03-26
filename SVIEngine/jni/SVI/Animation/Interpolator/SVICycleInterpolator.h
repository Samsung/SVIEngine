#ifndef __SVI_CYCLE_INTERPOLATOR_H_
#define __SVI_CYCLE_INTERPOLATOR_H_

#include "SVIInterpolator.h"

namespace SVI {
	class SVICycleInterpolator : public SVIInterpolator {

	public :
		SVICycleInterpolator() { 
			snprintf(mName, 64, "Cycle"); 
			mCycles = 1.0f;
		}
		virtual ~SVICycleInterpolator() {}

		SVIFloat getTimeInterpolation(SVIUInt& time) { 
			SVIFloat input = 0.0f;
			preProcess(time, input);

			SVIFloat timeRatio = (SVIFloat)(sin(2 * mCycles * SVI_PI * input));

			postProcess(time, timeRatio);

			return timeRatio;
		}

		SVIInterpolator* clone() {
			SVICycleInterpolator* cloneObj = new SVICycleInterpolator();
			cloneObj->copy(*this);

			return (SVIInterpolator*)cloneObj;
		}

	private:
		SVIFloat		mCycles;

	};

};

#endif //__SVI_CYCLE_INTERPOLATOR_H_