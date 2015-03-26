#ifndef __TEST_BED_DELEGATOR_H_
#define __TEST_BED_DELEGATOR_H_

#include "../SVICores.h"
#include "../Slide/SVISlide.h"

namespace SVI {
	class TestBedDelegator {
	public:
		

		enum { ROTATION_NOTDEFINED=-1, ROTATION_0 = 0, ROTATION_90, ROTATION_180, ROTATION_270 };

		TestBedDelegator();
		virtual ~TestBedDelegator();

		SVISlide* getRootSlide();
		virtual void runTestBed() {}
		void traversalBeamedTree();
		virtual void restore() {}
		virtual void configurationChanged(SVIInt rotation) { }
		virtual void resize(SVIInt w, SVIInt h, SVIInt rotation)  {
			mWidth = w;
			mHeight = h;
			mRotation = rotation;
		}

	protected:
		virtual void traversalSubSlide(SVIBaseSlide* node, SVIInt level, SVIInt id);
		
	protected:
		SVISlide*		mSavedSubSlide;
		SVISlide*		mRootSlide;
		SVIInt			mWidth;
		SVIInt			mHeight;
		SVIInt			mRotation;
		SVIInt			mNewRotation;
	};
};

#endif //__TEST_BED_DELEGATOR_H_