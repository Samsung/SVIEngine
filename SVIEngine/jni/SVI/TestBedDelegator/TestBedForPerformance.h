#ifndef __TEST_BED_FOR_PERFORMANCE_H_
#define __TEST_BED_FOR_PERFORMANCE_H_

#include "../SVICores.h"
#include "TestBedDelegator.h"

namespace SVI {
	class TestBedForPerformance : public TestBedDelegator{
	public:
		

		TestBedForPerformance(SVIInt w, SVIInt h);
		virtual ~TestBedForPerformance();

		virtual void runTestBed();
		virtual void traversalBeamedTree();
		virtual void resize(SVIInt w, SVIInt h, SVIInt rotation);
		virtual void restore();

	protected:
		void buildSlideTree();
		SVISlide* createSlide(SVISlide* parent, const SVIRect& frame, const SVIColor& color, SVIImage* image);
		void traversalSubSlide(SVIBaseSlide* node, int level, int id);

	private:
		void testBasicAnimation();
		void testKeyFrameAnimation();
		void testRotateAnimation(SVIInt w, SVIInt h, SVIInt rotation);
		SVIFloat getFromAngle(SVIInt oldRotation, SVIInt newRotation);

	private:
		SVISlide* mMainSlide;
		SVIInt mCurrentRowCount;
		SVIInt mCurrentColumnCount;

		SVIInt mLeftMargin;
		SVIInt mRightMargin;
		SVIInt mTopMargin;
		SVIInt mBottomMargin;

		SVIFloat mIconWidth;
		SVIFloat mIconHeight;

		SVIFloat mMainSlideWidth;
		SVIFloat mMainSlideHeight;
		SVIFloat mXOffset;
		SVIFloat mYOffset;
	};
};

#endif //__TEST_BED_FOR_PERFORMANCE_H_