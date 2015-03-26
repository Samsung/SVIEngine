#include "TestBedForPerformance.h"
#include "../Slide/SVISlide.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIKeyFrameAnimation.h"
#include "../Slide/SVIRenderPartManager.h"

#define PORTRAIT_COLUMN_COUNT	(5)
#define PORTRAIT_ROW_COUNT		(5)

#define ICON_WIDTH				(60.0f)
#define ICON_HEIGHT				(60.0f)

#define LANDSCAPE_COLUMN_COUNT	PORTRAIT_ROW_COUNT
#define LANDSCAPE_ROW_COUNT		PORTRAIT_COLUMN_COUNT

//2011-07-03 masterkeaton27@gmail.comtest flag
#define USING_IMAGE 1

namespace SVI {

	static const SVIBool DEBUG = SVIFALSE;

	TestBedForPerformance::TestBedForPerformance(SVIInt w, SVIInt h) {
		mRootSlide = NULL;
		mMainSlide = NULL;
		mWidth = w;
		mHeight = h;

		mCurrentRowCount = 0;
		mCurrentColumnCount = 0;

		mLeftMargin = 30;
		mRightMargin = 30;
		mTopMargin = 30;
		mBottomMargin = 30;

		mMainSlideWidth = 500;
		mMainSlideHeight = 500;
		mXOffset = 0;
		mYOffset = 0;

		mIconWidth = ICON_WIDTH;
		mIconHeight = ICON_HEIGHT;

		buildSlideTree();
	}

	TestBedForPerformance::~TestBedForPerformance() {
		SVI_SVIFE_DELETE(mRootSlide);
	}

	void TestBedForPerformance::traversalBeamedTree() {
		SVIBeamedSlide* root = mRootSlide->getBeamedSlide();
		traversalSubSlide(root, 0, 0);
	}

	void TestBedForPerformance::runTestBed() {
		testKeyFrameAnimation();
		
		//testBasicAnimation();
	}

	void TestBedForPerformance::resize(SVIInt w, SVIInt h, SVIInt rotation) {
		mNewRotation = rotation;
		DEBUG_CHECK_LOGI("old:%d, new:%d", mRotation, mNewRotation);
		testRotateAnimation(w, h, rotation);
		TestBedDelegator::resize(w, h, rotation);
	}

	void TestBedForPerformance::restore() {
	}

	void TestBedForPerformance::testBasicAnimation() {
		SVIUInt repeatCount = 2;
		SVIBool autoReverse = SVITRUE;
		SVIUInt duration = 500;
		SVIAnimation::InterpolatorType interpolatorType = SVIAnimation::DECELERATE;

		SVIInt offset = 50;
		SVIInt count = 0;
		BaseSlideList& subSlideLists = mMainSlide->getSubSlides();
		BaseSlideList::iterator iter = subSlideLists.begin();
		SVISlide* subSlide = NULL;

		for(iter; iter != subSlideLists.end(); iter++) {
			subSlide = (SVISlide*)(*iter);
			subSlide->getBeamedSlide()->setPivotPoint(SVIPoint(0.0f, 0.5f));

			SVIAnimationSet* aniSet = new SVIAnimationSet(mRootSlide->getSVIGLSurface());
			aniSet->mDuration = duration;
			aniSet->mRepeatCount = repeatCount;
			aniSet->mAutoReverse = autoReverse;
			aniSet->setInterpolator(interpolatorType);
			aniSet->setListener(mRootSlide->getSVIGLSurface()->getRenderPartManager());
			aniSet->mOffset = offset * count;

			//test rotation animation
			{
				SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::ROTATION, mRootSlide->getSVIGLSurface());

				DEBUG_CHECK_LOGE("start animation %d", count);

				SVIQuaternion q;
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 3.141529/2.0f, 0.0f));
				animation->mTo = SVIVector4(q.x, q.y, q.z, q.w);
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
				animation->mFrom = SVIVector4(q.x, q.y, q.z, q.w);
				animation->mDuration = duration;
				animation->mRepeatCount = repeatCount;
				animation->mAutoReverse = autoReverse;
				animation->mOffset = offset * count;

				aniSet->addAnimation(animation);
			}

			//test opacity color
			{
				SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::BG_COLOR, mRootSlide->getSVIGLSurface());

				animation->mFrom = subSlide->getBgColor();
				animation->mDuration = duration;
				animation->mTo = SVIColor(0.0f, 1.0f, 0.0f, 1.0f);
				animation->mRepeatCount = repeatCount;
				animation->mAutoReverse = autoReverse;
				animation->mOffset = offset * count;
				aniSet->addAnimation(animation);
			}
			count++;

			subSlide->addAnimation(aniSet);
			
			//SVI_SVIFE_DELETE(aniSet);
		}
		
	}

	void TestBedForPerformance::testKeyFrameAnimation() {
		SVIUInt repeatCount = 1;
		SVIBool autoReverse = SVIFALSE;
		SVIUInt duration = 6000;
		SVIAnimation::InterpolatorType interpolatorType = SVIAnimation::DECELERATE;
		
		BaseSlideList& subSlideLists = mMainSlide->getSubSlides();
		BaseSlideList::iterator iter = subSlideLists.begin();
		SVISlide* subSlide = NULL;
		SVIInt count = 0;
		SVIInt offset = 0;

		for(iter; iter != subSlideLists.end(); iter++) {
			subSlide = (SVISlide*)(*iter);

			SVIAnimationSet* aniSet = new SVIAnimationSet(mRootSlide->getSVIGLSurface());
			aniSet->mDuration = duration;
			aniSet->mRepeatCount = repeatCount;
			aniSet->mAutoReverse = autoReverse;
			aniSet->setListener(mRootSlide->getSVIGLSurface()->getRenderPartManager());
			aniSet->mOffset = offset * count;
			
			{
				/*		
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::ZPOSITION);
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, subSlide->getZPosition()));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, 100.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 60.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, 30.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.1f));
				aniSet->addAnimation(keyAni);
				*/
			}
			
			{
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::ZPOSITION, mRootSlide->getSVIGLSurface());
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 90.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
				//keyAni->addKeyProperty(SVIKeyFrameProperty(0.7f, SVIPoint(0.5,0.75)));
				//keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(0.5,1.0)));
				aniSet->addAnimation(keyAni);
			}

			{
				/*
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::SHADOW_RADIUS);
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, subSlide->getShadowRadius()));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 10.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 5.0f));
				aniSet->addAnimation(keyAni);
				*/
			}

			{
				/*
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::SHADOW_OFFSET);
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, subSlide->getShadowOffset()));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(15.0f, 15.0f)));
				aniSet->addAnimation(keyAni);
				*/
			}
				
			{
				
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::POSITION, mRootSlide->getSVIGLSurface());
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(subSlide->getBeamedSlide()->getPosition().mX, subSlide->getBeamedSlide()->getPosition().mY)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIPoint(0.0f, 0.0f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(subSlide->getBeamedSlide()->getPosition().mX, subSlide->getBeamedSlide()->getPosition().mY)));

				aniSet->addAnimation(keyAni);
				
			}
			
			{
				
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::ROTATION, mRootSlide->getSVIGLSurface());
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				SVIQuaternion q;
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(3.141592f*0.5f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(3.141529f*0.5f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));

				aniSet->addAnimation(keyAni);				
			}
			
			/*
			{
				
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::BG_COLOR);
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIColor(1.0f, 1.0f, 1.0f, 0.0f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIColor(1.0f, 1.0f, 0.0f, 0.3f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIColor(0.0f, 1.0f, 1.0f, 0.5f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, SVIColor(1.0f, 0.0f, 1.0f, 0.8f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, subSlide->getBgColor()));

				aniSet->addAnimation(keyAni);				
			}*/

			{
				/*
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::CORNER_RADIUS);
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 30.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0));

				aniSet->addAnimation(keyAni);
				*/
				
			}

			{
				/*
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::SHADOW_COLOR);
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIColor(0.0f, 0.0f, 0.0f, 0.6f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIColor(1.0f, 0.0f, 0.0f, 0.6f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.6f, SVIColor(0.0f, 1.0f, 0.0f, 0.6f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIColor(0.0f, 0.0f, 1.0f, 0.6f)));

				aniSet->addAnimation(keyAni);
				*/
			}

			count++;
			subSlide->addAnimation(aniSet);
			//SVI_SVIFE_DELETE(aniSet);
		}
	}

	void TestBedForPerformance::testRotateAnimation(SVIInt w, SVIInt h, SVIInt rotation) {
		SVIUInt repeatCount = 1;
		SVIBool autoReverse = SVIFALSE;
		SVIUInt duration = 500;
		SVIAnimation::InterpolatorType interpolatorType = SVIAnimation::DECELERATE;
		SVIInt count = 0;
		SVIInt offset = 0;

		{
			SVIAnimationSet* aniSet = new SVIAnimationSet(mRootSlide->getSVIGLSurface());
			aniSet->mDuration = duration;
			aniSet->mRepeatCount = repeatCount;
			aniSet->mAutoReverse = autoReverse;
			aniSet->setListener(mRootSlide->getSVIGLSurface()->getRenderPartManager());

			{
				SVIFloat fromAngle = getFromAngle(mRotation, mNewRotation);
				SVIFloat toAngle = 0.0f;

				if( fromAngle != toAngle ) {
					SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::ROTATION, mRootSlide->getSVIGLSurface());

					DEBUG_CHECK_LOGE("start animation %d", count);

					SVIQuaternion q;
					q.setIdentity();
					q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
					animation->mTo = SVIVector4(q.x, q.y, q.z, q.w);
					q.setIdentity();
					q.setEuler(SVIVector3(0.0f, 0.0f, fromAngle));
					animation->mFrom = SVIVector4(q.x, q.y, q.z, q.w);
					animation->mDuration = duration;
					animation->mRepeatCount = repeatCount;
					animation->mAutoReverse = autoReverse;
					animation->setInterpolator(interpolatorType);

					aniSet->addAnimation(animation);
				}
			}


			{
				
				SVIBasicAnimation* animation = SVIBasicAnimation::create(SVIBasicAnimation::REGION, mRootSlide->getSVIGLSurface());

				DEBUG_CHECK_LOGE("start animation %d", count);

				//SVIFloat xOffset = (w - mWidth) / 2.0f;
				//SVIFloat yOffset = (h - mHeight) / 2.0f;
				SVIFloat slideW = mMainSlide->getBounds().getSize().mWidth;
				SVIFloat slideH = mMainSlide->getBounds().getSize().mHeight;
				SVIFloat xOffset = (w - slideW) / 2.0f;
				SVIFloat yOffset = (h - slideH) / 2.0f;
				animation->mFrom = SVIRect(SVIPoint(xOffset, yOffset), SVISize(slideW+1.0f, slideH+1.0f));
				animation->mTo = SVIRect(SVIPoint(xOffset, yOffset), SVISize(slideW, slideH));
				animation->mDuration = duration;
				animation->mRepeatCount = repeatCount;
				animation->mAutoReverse = autoReverse;
				animation->setInterpolator(interpolatorType);

				aniSet->addAnimation(animation);

				mXOffset = xOffset;
				mYOffset = yOffset;
				
			}

			mMainSlide->addAnimation(aniSet);
			//SVI_SVIFE_DELETE(aniSet);
		}

		SVIInt rowCount = PORTRAIT_ROW_COUNT;
		SVIInt columnCount = PORTRAIT_COLUMN_COUNT;
		SVIInt counter = 0;

		if( w > h ) {
			columnCount = LANDSCAPE_COLUMN_COUNT;
			rowCount = LANDSCAPE_ROW_COUNT;
		}

		SVISlide* temp = NULL;
		SVIFloat fromWidth = w / (SVIFloat)mCurrentColumnCount;
		SVIFloat fromHeight = h / (SVIFloat)mCurrentRowCount;
		SVIFloat toWidth = w / (SVIFloat)columnCount;
		SVIFloat toHeight = h / (SVIFloat)rowCount;
		SVIFloat r, g, b;
		
		SVIFloat fromLeftMargin = mLeftMargin;
		SVIFloat fromRightMargin = mRightMargin;
		SVIFloat fromTopMargin = mTopMargin;
		SVIFloat fromBottomMargin = mBottomMargin;

		SVIFloat toLeftMargin = (toWidth - mIconWidth) / 2.0f;
		SVIFloat toRightMargin = toLeftMargin;
		SVIFloat toTopMargin = (toHeight - mIconHeight) / 2.0f;
		SVIFloat toBottomMargin = toTopMargin;


		BaseSlideList& subSlideLists = mMainSlide->getSubSlides();
		BaseSlideList::iterator iter = subSlideLists.begin();
		SVISlide* subSlide = NULL;
		count = 0;


		SVIInt fromXIdx, fromYIdx;
		SVIInt toXIdx, toYIdx;
		SVIInt fromX, fromY;
		SVIInt toX, toY;

		DEBUG_CHECK_LOGI("mColumnCount:%d, mRowCount:%d, columnCount:%d rowCount:%d", mCurrentColumnCount, mCurrentRowCount, columnCount, rowCount);
		DEBUG_CHECK_LOGI("fromWidth:%f, fromHeight:%f, toWidth:%f, toHeight:%f", fromWidth, fromHeight, toWidth, toHeight);

		for(iter; iter != subSlideLists.end(); iter++) {
			subSlide = (SVISlide*)(*iter);

			fromXIdx = count % mCurrentColumnCount;
			fromYIdx = count / (SVIFloat)mCurrentColumnCount;

			fromX = (mXOffset*-1.0f) + fromXIdx * fromWidth;
			fromY = (mYOffset*-1.0f) + fromYIdx * fromHeight;

			toXIdx = count % columnCount;
			toYIdx = count / (SVIFloat)columnCount;

			toX = (mXOffset*-1.0f) + toXIdx * toWidth;
			toY = (mYOffset*-1.0f) + toYIdx * toHeight;

			SVIAnimationSet* aniSet = new SVIAnimationSet(mRootSlide->getSVIGLSurface());
			aniSet->mDuration = duration;
			aniSet->mRepeatCount = repeatCount;
			aniSet->mAutoReverse = autoReverse;
			aniSet->setListener(mRootSlide->getSVIGLSurface()->getRenderPartManager());
			aniSet->mOffset = offset * count;

			{
				SVIBasicAnimation* basicAni = SVIBasicAnimation::create(SVIKeyFrameAnimation::REGION, mRootSlide->getSVIGLSurface());
				basicAni->mDuration = duration;
				basicAni->mAutoReverse = autoReverse;
				basicAni->mRepeatCount = repeatCount;
				basicAni->mOffset = offset * count;
				basicAni->setInterpolator(interpolatorType);

				
				basicAni->mFrom = SVIRect( (fromX+fromLeftMargin), (fromY+fromTopMargin), mIconWidth, mIconHeight);
				basicAni->mTo = SVIRect( (toX+toLeftMargin), (toY+toTopMargin), mIconWidth, mIconHeight);
				aniSet->addAnimation(basicAni);

				DEBUG_CHECK_LOGI("count: %d, [fx:%d, fy:%d], [tx:%d, ty:%d] ", count, fromX, fromY, toX, toY);
			}

			{
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::BG_COLOR, mRootSlide->getSVIGLSurface());
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIColor(1.0f, 1.0f, 1.0f, 1.0f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIColor(1.0f, 1.0f, 1.0f, 0.3f)));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIColor(1.0f, 1.0f, 1.0f, 1.0f)));

				aniSet->addAnimation(keyAni);
			}

			{
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::ZPOSITION, mRootSlide->getSVIGLSurface());
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, subSlide->getZPosition()));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, 100.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, 60.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, 30.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.1f)); //to do: z sorting function has some problem. it needs to check.
				aniSet->addAnimation(keyAni);
			}
			
			{
				SVIKeyFrameAnimation* keyAni = SVIKeyFrameAnimation::create(SVIKeyFrameAnimation::ROTATION, mRootSlide->getSVIGLSurface());
				keyAni->mDuration = duration;
				keyAni->mAutoReverse = autoReverse;
				keyAni->mRepeatCount = repeatCount;
				keyAni->mOffset = offset * count;
				keyAni->setInterpolator(interpolatorType);

				SVIQuaternion q;
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 3.141592f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.3f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.5f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, -3.141529f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(0.8f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setIdentity();
				q.setEuler(SVIVector3(0.0f, 0.0f, 0.0f));
				keyAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));

				aniSet->addAnimation(keyAni);
			}
			
			count++;
			subSlide->addAnimation(aniSet);
		}

		mLeftMargin = toLeftMargin;
		mRightMargin = toRightMargin;
		mTopMargin = toTopMargin;
		mBottomMargin = toBottomMargin;

		mCurrentRowCount = rowCount;
		mCurrentColumnCount = columnCount;
	}

	void TestBedForPerformance::buildSlideTree() {
		SVIRenderPartManager* renderMgr = mRootSlide->getSVIGLSurface()->getRenderPartManager();
		//Create root slide [Level 0]
		SVIInt bgImageIdx = renderMgr->getImageCount() - 1;
		if( mRootSlide == NULL ) {
			DEBUG_CHECK_LOGI("bgImageIdx : %d", bgImageIdx);
			mRootSlide = createSlide(NULL, 
				SVIRect(0.0f, 0.0f, 50.0f,50.0f),//(SVIFloat)mWidth, SVIFloat(mHeight)),
				SVIColor(1.0f, 1.0f, 1.0f, 1.0f), NULL);
		}

		

		if( mMainSlide == NULL ) {
			mXOffset = (mWidth - mMainSlideWidth) / 2.0f;
			mYOffset = (mHeight - mMainSlideHeight) / 2.0f;
			mMainSlide = createSlide(mRootSlide, 
				SVIRect(mXOffset, mYOffset, (SVIFloat)mMainSlideWidth, (SVIFloat)mMainSlideHeight),
#if USING_IMAGE
				SVIColor(1.0f, 1.0f, 1.0f, 1.0f),renderMgr->getImage(bgImageIdx));
#else
				SVIColor(1.0f, 1.0f, 1.0f, 1.0f),NULL);
#endif
		}

		

		SVIInt imageCnt = renderMgr->getImageCount()-1;
		SVIInt rowCount = PORTRAIT_ROW_COUNT;
		SVIInt columnCount = PORTRAIT_COLUMN_COUNT;
		SVIInt count = 0;

		if( mWidth > mHeight ) {
			columnCount = LANDSCAPE_COLUMN_COUNT;
			rowCount = LANDSCAPE_ROW_COUNT;
		}

		mCurrentRowCount = rowCount;
		mCurrentColumnCount = columnCount;

		SVIFloat x, y;
		SVIFloat width = mWidth / columnCount;
		SVIFloat height = mHeight / rowCount;
		SVIFloat r, g, b;

		mLeftMargin = (width - mIconWidth) / 2.0f;
		mRightMargin = mLeftMargin;
		mTopMargin = (height - mIconHeight) / 2.0f;
		mBottomMargin = mTopMargin;
		SVIInt slideCount = rowCount * columnCount;
		for(int i=0; i<slideCount; i++) {
			SVIInt imgIdx = count % (imageCnt);
			//x = 800.0f*0.5f;//(mXOffset*-1.0f) + (count%columnCount) * width;
			//y = 1280.0f*0.5f;//(mYOffset*-1.0f) + (count/columnCount) * height;
			x = (mXOffset*-1.0f) + (count%columnCount) * width;
			y = (mYOffset*-1.0f) + (count/columnCount) * height;
			createSlide(mMainSlide, SVIRect( (x+mLeftMargin), (y+mTopMargin), mIconWidth, mIconHeight),
				//SVIRect( x, y, mIconWidth, mIconHeight),
#if USING_IMAGE
				SVIColor(1.0f, 1.0f, 1.0f, 1.0f), renderMgr->getImage(imgIdx));
#else
				SVIColor(1.0f, 1.0f, 1.0f, 1.0f), NULL);
#endif
			count++;
		}
	}

	SVISlide* TestBedForPerformance::createSlide(SVISlide* parent, const SVIRect& frame, const SVIColor& color, SVIImage* image){
		SVISlide* slide = NULL;
		slide = new SVISlide(parent->getSVIGLSurface());
		if( !slide->initialize(parent, frame, color, image) )
			DEBUG_CHECK_LOGE("create slide failed");

		return slide;
	}

	void TestBedForPerformance::traversalSubSlide(SVIBaseSlide* node, int level, int id) {
		if( node != NULL ) {
			SVIBaseSlide* childNode = NULL;
			BaseSlideList& subSlideList = node->getSubSlides();

			SVIRect frame = node->getRegion();
			DEBUG_CHECK_LOGE("level : %d, nodeidx : %d, frame:(%f,%f,%f,%f)", level, id, frame.mOrigin.mX,frame.mOrigin.mY,
				frame.mSize.mWidth, frame.mSize.mHeight);



			int childIdx = 0;
			for(BaseSlideListIter iter = subSlideList.begin(); iter != subSlideList.end(); iter++) {
				childNode = (*iter);
				if( childNode != NULL ) 
					traversalSubSlide(childNode, level+1, childIdx++);
			}

		}
	}

	SVIFloat TestBedForPerformance::getFromAngle(SVIInt oldRotation, SVIInt newRotation){
		SVIFloat fromAngle = 0.0f;
		SVIFloat from, to;

		switch(oldRotation) {
		case ROTATION_0:
			from = 0.0f;
			break;
		case ROTATION_90:
			from = SVI_90;
			break;
		case ROTATION_180:
			from = SVI_PI;
			break;
		case ROTATION_270:
			from = SVI_270;
			break;
		default:
			from = 0.0f;
			break;
		}

		switch(newRotation) {
		case ROTATION_0:
			to = 0.0f;
			break;
		case ROTATION_90:
			to = SVI_90;
			break;
		case ROTATION_180:
			to = SVI_PI;
			break;
		case ROTATION_270:
			to = SVI_270;
			break;
		default:
			to = 0.0f;
			break;
		}

		fromAngle = from - to;

		if( fromAngle == SVI_270 )
			fromAngle = -SVI_90;
		else if (fromAngle == -SVI_270 )
			fromAngle = SVI_90;

		DEBUG_CHECK_LOGI("TestBedForPerformance::getFromAngle() from:%d, to:%d, angle:%f", oldRotation, newRotation, fromAngle);

		return fromAngle;
	}

}