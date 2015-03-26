#include "TestBedForAnimation.h"
#include "../Slide/SVISlide.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIKeyFrameAnimation.h"
#include "../Slide/SVIRenderPartManager.h"

namespace SVI {
	static const SVIBool DEBUG = SVIFALSE;
	TestBedForAnimation::TestBedForAnimation(SVIInt w, SVIInt h) {
		mRootSlide = NULL;
		mWidth = w;
		mHeight = h;
		buildSlideTree();
	}

	TestBedForAnimation::~TestBedForAnimation() {
		SVI_SVIFE_DELETE(mRootSlide);
	}

	void TestBedForAnimation::traversalBeamedTree() {
		SVIBeamedSlide* root = mRootSlide->getBeamedSlide();
		traversalSubSlide(root, 0, 0);
	}

	void TestBedForAnimation::runTestBed() {
		testKeyFrameAnimation();
	}

	void TestBedForAnimation::resize(SVIInt w, SVIInt h, SVIInt rotation) {
		TestBedDelegator::resize(w, h, rotation);
	}

	void TestBedForAnimation::restore() {
	}

	void TestBedForAnimation::testBasicAnimation() {
		SVIUInt repeatCount = 3;
		SVIBool autoReverse = SVITRUE;
		SVIUInt duration = 2000;

		static SVIInt count = 0;
		SVIAnimationSet* aniSet = new SVIAnimationSet(mRootSlide->getSVIGLSurface());
		aniSet->mDuration = duration;
		aniSet->mRepeatCount = repeatCount;
		aniSet->mAutoReverse = autoReverse;
		aniSet->setListener(mRootSlide->getSVIGLSurface()->getRenderPartManager());

		//test region animation
		{
			SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::REGION, mRootSlide->getSVIGLSurface());
			animation->mFrom = mRootSlide->getBeamedSlide()->getRegion();

			DEBUG_CHECK_LOGE("start animation %d", count);

			if( count % 2 ) {
				animation->mTo = SVIRect(0.0f, 0.0f, mWidth, mHeight);
				animation->mDuration = duration;
			} else {
				animation->mTo = SVIRect(0.0f, 0.0f, mHeight, mWidth);
				animation->mDuration = duration;
			}
			aniSet->addAnimation(animation);
		}

		//test rotation animation
		{
			SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::ROTATION, mRootSlide->getSVIGLSurface());
			animation->mFrom = mRootSlide->getBeamedSlide()->getRotation();

			DEBUG_CHECK_LOGE("start animation %d", count);

			SVIQuaternion q;
			q.setIdentity();
			if( count % 2 ) {
				q.setEuler(SVIVector3(0.0f, 3.141529/2.0f, 0.0f));
				animation->mTo = SVIVector4(q.x, q.y, q.z, q.w);
				animation->mDuration = duration;
			} else {
				q.setEuler(SVIVector3(0.0f, -3.141529/2.0f, 3.141529/2.0f));
				animation->mTo = SVIVector4(q.x, q.y, q.z, q.w);
				animation->mDuration = duration;
			}
			animation->mRepeatCount = repeatCount;
			animation->mAutoReverse = autoReverse;

			aniSet->addAnimation(animation);
		}

		//test scale animation
		{
			SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::SCALE, mRootSlide->getSVIGLSurface());
			animation->mFrom = mRootSlide->getBeamedSlide()->getScale();

			DEBUG_CHECK_LOGE("start animation %d", count);

			if( count % 2 ) {
				animation->mTo = SVIVector3(1.0f, 1.0f, 1.0f);
				animation->mDuration = duration;
			} else {
				//animation->mTo = SVIVector3((mHeight/mWidth), (mWidth/mHeight), 1.0f);
				animation->mTo = SVIVector3(0.5f, 0.5f, 1.0f);
				animation->mDuration = duration;
			}
			animation->mRepeatCount = repeatCount;
			animation->mAutoReverse = autoReverse;
			aniSet->addAnimation(animation);
		}

		//test opacity color
		{
			SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::BG_COLOR, mRootSlide->getSVIGLSurface());
			animation->mFrom = mRootSlide->getBeamedSlide()->getBgColor();

			DEBUG_CHECK_LOGE("start animation %d", count);

			if( count % 2 ) {
				animation->mTo = SVIColor(1.0f, 1.0f, 1.0f, 1.0f);
				animation->mDuration = duration;
			} else {
				animation->mTo = SVIColor(1.0f, 0.0f, 1.0f, 0.3f);
				animation->mDuration = duration;
			}
			animation->mRepeatCount = repeatCount;
			animation->mAutoReverse = autoReverse;
			aniSet->addAnimation(animation);
		}


		count++;

		if( !mRootSlide->addAnimation(aniSet) )
			LOGE("addAnimation failed!!!!");

		SVI_SVIFE_DELETE(aniSet);
	}

	void TestBedForAnimation::testKeyFrameAnimation() {
		SVIUInt repeatCount = 1;
		SVIBool autoReverse = SVIFALSE;
		SVIUInt duration = 3000;
		SVIAnimation::InterpolatorType interpolatorType = SVIAnimation::DECELERATE;
		SVIAnimationSet* aniSet = new SVIAnimationSet(mRootSlide->getSVIGLSurface());
		aniSet->mDuration = duration;
		aniSet->mRepeatCount = repeatCount;
		aniSet->mAutoReverse = autoReverse;
		aniSet->setListener(mRootSlide->getSVIGLSurface()->getRenderPartManager());

		{
			SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::SCALE, mRootSlide->getSVIGLSurface());
			keyAni->mDuration = duration;
			keyAni->mAutoReverse = autoReverse;
			keyAni->mRepeatCount = repeatCount;
			keyAni->setInterpolator(interpolatorType);

			keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector3(1.0f, 1.0f, 1.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIVector3(0.1f, 0.1f, 1.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector3(0.7f, 0.7f, 1.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, SVIVector3(0.3f, 0.3f, 1.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector3(1.0f, 1.0f, 1.0f)));

			aniSet->addAnimation(keyAni);
		}

		{
			SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::POSITION, mRootSlide->getSVIGLSurface());
			keyAni->mDuration = duration;
			keyAni->mAutoReverse = autoReverse;
			keyAni->mRepeatCount = repeatCount;
			keyAni->setInterpolator(interpolatorType);

			keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(mWidth/2.0f, mHeight/2.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIPoint(0.0f, 0.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(400.0f, 300.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, SVIPoint(0.0f, 300.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(mWidth/2.0f, mHeight/2.0f)));

			aniSet->addAnimation(keyAni);
		}
		/*
		{
			SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::ROTATION);
			keyAni->mDuration = duration;
			keyAni->mAutoReverse = autoReverse;
			keyAni->mRepeatCount = repeatCount;
			keyAni->setInterpolator(interpolatorType);

			SVIQuaternion q;
			q.setIdentity();
			q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
			q.setIdentity();
			q.setEuler(SVIVector3(0.0f, 3.141592f, 0.0f));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIVector4(q.x, q.y, q.z, q.w)));
			q.setIdentity();
			q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector4(q.x, q.y, q.z, q.w)));
			q.setIdentity();
			q.setEuler(SVIVector3(3.141529f, 0.0f, 0.0f));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, SVIVector4(q.x, q.y, q.z, q.w)));
			q.setIdentity();
			q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
			keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));

			aniSet->addAnimation(keyAni);
		}*/

		if( !mRootSlide->addAnimation(aniSet) )
			LOGE("addAnimation failed!!!!");
		SVI_SVIFE_DELETE(aniSet);
		/*
		{
			SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::POSITION);
			keyAni->mDuration = duration;
			keyAni->mAutoReverse = autoReverse;
			keyAni->mRepeatCount = repeatCount;
			keyAni->setInterpolator(interpolatorType);

			keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(100.0f, 100.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIPoint(100.0f, 0)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(0, 100.0f)));
			keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(200.0f, 200.0f)));

			if( !mSavedSubSlide->addAnimation(keyAni) )
				LOGE("addAnimation failed!!!!");

			SVI_SVIFE_DELETE(keyAni);
		}
		*/
		//mSavedSubSlide->getBeamedSlide()->setClipSubSlide(SVITRUE);

	}

	void TestBedForAnimation::buildSlideTree() {
		DEBUG_CHECK_LOGE("buildSlideTree() 1");

		SVIRenderPartManager* renderMgr = mRootSlide->getSVIGLSurface()->getRenderPartManager();
		//Create root slide [Level 0]
		if( mRootSlide == NULL ) {
			mRootSlide = createSlide(NULL, 
			SVIRect(0.0f, 0.0f, (SVIFloat)mWidth, SVIFloat(mHeight)),
			SVIColor(1.0f, 1.0f, 1.0f, 1.0f),NULL);// renderMgr->getImage(3));
		}

		SVISlide* temp = NULL;
		SVIFloat yoffset  = 0;
		// [Level 1] [root - child 0 ]
		temp = createSlide(mRootSlide, SVIRect(210.f, -40.0f, 400.0f, 400.0f),
			SVIColor(1.0f, 0.0f, 0.0f, 1.0f), NULL);

		//mSavedSubSlide = temp;

		yoffset += 200.0f;


		// [Level 2] [root - child 0 - child 0]
		temp = createSlide(temp, SVIRect(40.f, 40.f, 100.0f, 100.0f),
			SVIColor(0.0f, 1.0f, 1.0f, 1.0f), NULL);
		mSavedSubSlide = temp;
		mSavedSubSlide->setClipSubSlide(SVITRUE);

		// [Level 3] [root - child 0 - child 0 - child 0]
		temp = createSlide(temp, SVIRect(10.f, 10.f, 60.0f, 60.0f),
			SVIColor(1.0f, 0.0f, 1.0f, 1.0f), NULL);

		// [Level 3] [root - child 0 - child 0 - child 0]
		temp = createSlide(temp, SVIRect(10.f, 10.f, 40.0f, 40.0f),
			SVIColor(0.0f, 1.0f, 0.0f, 1.0f), NULL);

		// [Level 3] [root - child 0 - child 0 - child 0]
		temp = createSlide(temp, SVIRect(10.f, 10.f, 20.0f, 20.0f),
			SVIColor(1.0f, 1.0f, 0.0f, 1.0f), NULL);

		// [Level 3] [root - child 0 - child 0 - child 0]
		temp = createSlide(temp, SVIRect(10.f, 10.f, 10.0f, 10.0f),
			SVIColor(0.0f, 1.0f, 1.0f, 1.0f), NULL);



		// [Level 1] [root - child 1 ]
		createSlide(mRootSlide, SVIRect(10.f, yoffset + 50.0f, 100.0f, 100.f),
			SVIColor(0.0f, 1.0f, 0.0f, 1.0f), renderMgr->getImage(1));
		yoffset += 50.0f + 100.0f;

		// [Level 1] [root - child 2 ]
		createSlide(mRootSlide, SVIRect(10.f, yoffset + 30.0f, 100.0f, 100.f),
			SVIColor(0.0f, 0.0f, 1.0f, 1.0f), renderMgr->getImage(2));
		yoffset += 30.0f + 100.0f;
		DEBUG_CHECK_LOGE("buildSlideTree() 2");

		temp = createSlide(NULL, SVIRect(10.f, yoffset + 30.0f, 100.0f, 100.f),
			SVIColor(0.5f, 0.5f, 0.5f, 1.0f), NULL);

		SVISlide* savedtemp = temp;

		

#if 0
		SVISlide* tempSub = createSlide(temp, SVIRect(10.f, 10, 80.0f, 80.f),
			SVIColor(1.0f, 0.0f, 0.5f, 1.0f), NULL);
		//temp->addSubSlide(tempSub);

		temp = tempSub;
		tempSub = createSlide(temp, SVIRect(10.f, 10, 60.0f, 60.f),
			SVIColor(0.5f, 1.0f, 0.0f, 1.0f), NULL);
		//temp->addSubSlide(tempSub);
#else
		SVISlide* tempSub = createSlide(NULL, SVIRect(10.f, 10, 80.0f, 80.f),
			SVIColor(1.0f, 0.0f, 0.5f, 1.0f), NULL);
		savedtemp->addSlide(tempSub);

		temp = tempSub;
		tempSub = createSlide(NULL, SVIRect(10.f, 10.0f, 60.0f, 60.f),
			SVIColor(0.5f, 1.0f, 0.0f, 1.0f), NULL);
		temp->addSlide(tempSub);

		temp = tempSub;
		tempSub = createSlide(NULL, SVIRect(10.f, 10.0f, 40.0f, 40.f),
			SVIColor(0.5f, 0.0f, 5.0f, 1.0f), NULL);
		temp->addSlide(tempSub);

		temp = tempSub;
		tempSub = createSlide(NULL, SVIRect(10.f, 10.0f, 20.0f, 20.f),
			SVIColor(1.0f, 0.3f, 0.7f, 1.0f), NULL);
		temp->addSlide(tempSub);

		temp = tempSub;
		tempSub = createSlide(NULL, SVIRect(10.f, 10.0f, 10.0f, 10.f),
			SVIColor(0.1f, 0.3f, 1.7f, 1.0f), NULL);
		temp->addSlide(tempSub);

		temp = tempSub;
		tempSub = createSlide(NULL, SVIRect(5.0f, 5.0f, 5.0f, 5.f),
			SVIColor(0.0f, 1.0f, 0.7f, 1.0f), NULL);
		temp->addSlide(tempSub);

		mRootSlide->addSlide(savedtemp);
		mSavedSubSlide = savedtemp;

#endif
		traversalBeamedTree();
		DEBUG_CHECK_LOGE("buildSlideTree() 3");

	}

	SVISlide* TestBedForAnimation::createSlide(SVISlide* parent, const SVIRect& frame, const SVIColor& color, SVIImage* image){
		SVISlide* slide = NULL;
		slide = new SVISlide(parent->getSVIGLSurface());
		if( !slide->initialize(parent, frame, color, image) )
			DEBUG_CHECK_LOGE("create slide failed");

		return slide;
	}

}