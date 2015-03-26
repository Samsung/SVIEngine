#ifndef __SVI_ANIMATION_SET_HEADER_
#define __SVI_ANIMATION_SET_HEADER_

#include "SVIAnimation.h"
#include <vector>

namespace SVI {
	typedef std::vector<SVIAnimation*>	AnimationSet;

	class SVIAnimationSet : public SVIAnimation {
	
	public:
		SVIAnimationSet(SVIGLSurface* saGLSurface);
		virtual ~SVIAnimationSet();

		void setBeamedSlide(SVIBeamedSlide* slide);
		virtual void update(SVIUInt& time);

		virtual SVIBool addAnimation(SVIAnimation* animation);
		void removeAnimation(SVIAnimation* animation);
		SVIAnimation* clone();
		void forceEnd();
		virtual SVIBool isEnd(SVIUInt& time);
		virtual void print();
		void clearAnimations();
		void shareAnimationInfo(SVIBool flag) { mShareAnimationInfo = flag; }

	protected :
		void calculateAndApply(const SVIFloat& ratio);
		void copy(const SVIAnimationSet* rhs);
		void shareAnimationInfo();
		SVIBool isExist(SVIAnimation* animation);

	protected:
		AnimationSet		mAnimationList;
		SVIBool				mShareAnimationInfo;

	private:
		SVIUInt				mCurrentRepeatCount;
	};

}; //end of SVI namespace

#endif //__SVI_ANIMATION_SET_HEADER_
