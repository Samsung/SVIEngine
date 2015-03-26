#include "TestBedDelegator.h"
#include "../Slide/SVISlide.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIKeyFrameAnimation.h"
#include "../Slide/SVIRenderPartManager.h"

namespace SVI {
	static const SVIBool DEBUG = SVIFALSE;

	TestBedDelegator::TestBedDelegator() {
		mRootSlide = NULL;
		mWidth = 0;
		mHeight = 0;
		mRotation = ROTATION_NOTDEFINED;
		mNewRotation = ROTATION_NOTDEFINED;

		LOGE("TestBedDelegator::TestBedDelegator()");
	}

	TestBedDelegator::~TestBedDelegator() {
		SVI_SVIFE_DELETE(mRootSlide);
	}

	SVISlide* TestBedDelegator::getRootSlide() { 
		return mRootSlide; 
	}

	void TestBedDelegator::traversalBeamedTree() {
		SVIBeamedSlide* root = mRootSlide->getBeamedSlide();
		traversalSubSlide(root, 0, 0);
	}


	void TestBedDelegator::traversalSubSlide(SVIBaseSlide* node, SVIInt level, SVIInt id) {
		if( node != NULL ) {
			SVIBaseSlide* childNode = NULL;
			BaseSlideList& subSlideList = node->getSubSlides();

			SVIChar levelString[32];
			memset(levelString, 0x00, sizeof(levelString));
			SVIRect frame = node->getRegion();
			LOGI("level : %d, nodeidx : %d, frame:(%f,%f,%f,%f)", level, id, frame.mOrigin.mX,frame.mOrigin.mY,
				frame.mSize.mWidth, frame.mSize.mHeight);



			int childIdx = 0;
			for(BaseSlideListIter iter = subSlideList.begin(); iter != subSlideList.end(); iter++) {
				childNode = (*iter);
				if( childNode != NULL ) 
					traversalSubSlide(childNode, level+1, childIdx++);
			}

		}
	}

}