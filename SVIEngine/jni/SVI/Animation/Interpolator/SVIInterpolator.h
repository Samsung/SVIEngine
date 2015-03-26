#ifndef __SVI_INTERPOLATOR_H_
#define __SVI_INTERPOLATOR_H_

#include "../../SVICores.h"

namespace SVI {
	class SVIInterpolator {
	public :
		SVIInterpolator();
		virtual ~SVIInterpolator() {}

		void setRepeatCount(SVIUInt count) { mRepeatCount = count; }
		void setAutoReverse(SVIBool autoReverse) { mAutoReverse = autoReverse; }
		void setStartTime(SVIUInt time, SVIUInt duration, SVIUInt offset);

		void forceEnd() { mForceEnd = SVITRUE; }

		SVIBool isEnd(SVIUInt& time);

		void copy(SVIInterpolator& rhs);

		SVIUInt getCurrentCount() { return mCurrentCount; }
		SVIUInt getRepeatCount() { return mRepeatCount; }
		
		virtual SVIFloat getTimeInterpolation(SVIUInt& time) = 0;
		virtual SVIInterpolator* clone() = 0;

	protected:
		void print(SVIUInt& time, SVIFloat& ratio);
		void preProcess(const SVIUInt& time, SVIFloat& input);
		void postProcess(const SVIUInt& time, SVIFloat& timeRatio);

	private:	
		void checkAutoReverse(SVIFloat& ratio) const;
		void checkRepeatCount(SVIFloat& ratio);
		void checkCurrentCount(const SVIUInt& time);

	protected:
		SVIUInt		mStartTime;
		SVIUInt		mEndTime;
		SVIUInt		mDuration;
		SVIUInt		mOffset;
		SVIUInt		mRepeatCount;
		SVIBool		mAutoReverse;
		SVIBool		mForceEnd;
		SVIBool		mForceEndDone;

	private:
		SVIUInt		mCurrentCount;
		SVIUInt		mForceEndCount;

	public:
		SVIChar mName[64];
	};

};

#endif //__SVI_INTERPOLATOR_H_