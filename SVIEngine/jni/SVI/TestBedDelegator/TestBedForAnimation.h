#ifndef __TEST_BED_FOR_ANIMATION_H_
#define __TEST_BED_FOR_ANIMATION_H_

#include "../SVICores.h"
#include "TestBedDelegator.h"

namespace SVI {
	class TestBedForAnimation : public TestBedDelegator{
	public:
		

		TestBedForAnimation(SVIInt w, SVIInt h);
		virtual ~TestBedForAnimation();

		virtual void runTestBed();
		virtual void traversalBeamedTree();
		virtual void resize(SVIInt w, SVIInt h, SVIInt rotation);
		virtual void restore();

	protected:
		void buildSlideTree();
		SVISlide* createSlide(SVISlide* parent, const SVIRect& frame, const SVIColor& color, SVIImage* image);

	private:
		void testBasicAnimation();
		void testKeyFrameAnimation();
	};
};

#endif //__TEST_BED_FOR_ANIMATION_H_