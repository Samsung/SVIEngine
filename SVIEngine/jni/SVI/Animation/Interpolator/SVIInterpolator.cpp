#include "SVIInterpolator.h"

namespace SVI {
	static const SVIBool DEBUG = SVIFALSE;

	SVIInterpolator::SVIInterpolator() {
		mStartTime = 0;
		mEndTime = 0;
		mDuration = 0;
		mOffset = 0;
		mRepeatCount = 1;
		mCurrentCount = 0;
		mAutoReverse = SVIFALSE;
		mForceEnd = SVIFALSE;
		mForceEndDone = SVIFALSE;
		mForceEndCount = 0;
	}

	void SVIInterpolator::setStartTime(SVIUInt time, SVIUInt duration, SVIUInt offset) { 
		mStartTime = time + offset; 
		mDuration = duration;
		mOffset = offset;
		mEndTime = mStartTime + ( mDuration * mRepeatCount );
		DEBUG_CHECK_LOGI("SVIInterpolator::setStartTime() mStartTime:%d, mDuration:%d, mOffset:%d, mEndTime:%d", mStartTime, mDuration, mOffset, mEndTime);
	}

	SVIBool SVIInterpolator::isEnd(SVIUInt& time) { 
		SVIBool ret = SVIFALSE;

		DEBUG_CHECK_LOGI("SVIInterpolator::isEnd()time:%d mEndtime:%d, mCurrentCount:%d, mRepeatCount:%d", time, mEndTime, mCurrentCount, mRepeatCount);

		if( time >= mEndTime && mCurrentCount >= mRepeatCount )
			ret = SVITRUE;
		else if( mForceEndDone ) {
			mForceEndCount++;
			if( mForceEndCount > 2 )
				ret = SVITRUE;
		}

		return ret;
	}

	void SVIInterpolator::copy(SVIInterpolator& rhs) {
		mStartTime = rhs.mStartTime;
		mEndTime = rhs.mEndTime;
		mDuration = rhs.mDuration;
		mOffset = rhs.mOffset;
		mRepeatCount = rhs.mRepeatCount;
		mCurrentCount = rhs.mCurrentCount;
		mAutoReverse = rhs.mAutoReverse;
	}

	void SVIInterpolator::print(SVIUInt& time, SVIFloat& ratio) {
		DEBUG_CHECK_LOGI("type:%s time:%d, ratio:%f, count:%d, repeatcount:%d", mName, time, ratio, mCurrentCount, mRepeatCount);
	}

	void SVIInterpolator::preProcess(const SVIUInt& time, SVIFloat& input) {
		if( mForceEnd ) {
			input = 1.0f;
			mForceEndDone = SVITRUE;

			//LOGE("Interpolator mForceEnd TRUE ");
			return;
		}

		SVIInt elapsedTime = (SVIInt)(time - mStartTime);

		if( mDuration < 0.0f )
			input = 0.0f; 
		else {
			if( mDuration != 0 )
				input = (SVIFloat)(elapsedTime / (SVIFloat)mDuration);
			else
				input = 1.0f;

			//DEBUG_CHECK_LOGI("SVIInterpolator::preProcess() #1 input:%f, elapsedTime:%d", input, elapsedTime);
			checkRepeatCount(input);
			checkAutoReverse(input);
			//DEBUG_CHECK_LOGI("SVIInterpolator::preProcess() #2 input:%f, elapsedTime:%d", input, elapsedTime);
			if( input < 0.0f )
				input = 0.0f;
			else if( input > 1.0f )
				input = 1.0f;
		}
	}

	void SVIInterpolator::postProcess(const SVIUInt& time, SVIFloat& timeRatio) {
		checkCurrentCount(time);

		//print(time, timeRatio);
	}

	void SVIInterpolator::checkAutoReverse(SVIFloat& ratio) const { 
		if( !mAutoReverse )
			return;

		if( 0.0f < ratio && ratio <= 0.5f )
			ratio *= 2.0f;
		else if( 0.5f < ratio && ratio <= 1.0f )
			ratio = 1.0f - (ratio - 0.5f)*2.0f;
		else if( ratio > 1.0f )
			ratio = 0.0f;
	}

	void SVIInterpolator::checkRepeatCount(SVIFloat& ratio) {
		ratio = ratio - (1.0f * mCurrentCount );
	}

	void SVIInterpolator::checkCurrentCount(const SVIUInt& time) {
		if( mDuration > 0 ) {
			SVIInt elapsedTime = (SVIInt)(time - mStartTime);
			if( elapsedTime > 0 )
				mCurrentCount = (SVIUInt)(elapsedTime / mDuration);
			else
				mCurrentCount = 0;

		} else {
			mCurrentCount = mRepeatCount;
		}
	}
}