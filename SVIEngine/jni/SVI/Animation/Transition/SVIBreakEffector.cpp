#include "SVIBreakEffector.h"
#include "SVITransitionEffector.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI{

	static const SVIBool DEBUG = SVIFALSE;

	SVIBreakEffector::SVIBreakEffector(SVIGLSurface *surface) :
	SVITransitionEffector(surface),
	rowCount(15),
	columnCount(9){
		mRowCount = rowCount;
		mColumnCount = columnCount;
	}

	void SVIBreakEffector::setAnimation(){

		SVIFloat slideWidth = (mSizeSlide->getRegion().getSize().mWidth / mRowCount);
		SVIFloat slideHeight = (mSizeSlide->getRegion().getSize().mHeight / mColumnCount);

		SVIInt **mOffsetMap;

		mOffsetMap = new SVIInt *[mColumnCount];
		for (int i=0; i<mColumnCount; i++) {
			mOffsetMap[i] = new SVIInt[mRowCount];			
		}

		SVIInt rowOffsetMapUp[9][15] = {
			{8, 9, 9, 9, 8, 9, 9, 8, 9, 9, 8, 8, 9, 9, 9},
			{7, 8, 6, 7, 8, 8, 9, 9, 8, 9, 5, 6, 6, 4, 8},
			{6, 8, 7, 8, 6, 6, 5, 5, 7, 5, 4, 8, 9, 3, 8},
			{4, 4, 3, 6, 5, 7, 2, 5, 7, 3, 4, 7, 4, 3, 4},
			{0, 2, 2, 3, 4, 7, 3, 4, 4, 2, 3, 3, 6, 3, 4},
			{1, 0, 0, 1, 5, 1, 0, 3, 3, 3, 3, 2, 4, 6, 3},
			{1, 1, 0, 1, 4, 1, 1, 4, 4, 1, 2, 2, 0, 3, 0},
			{2, 1, 0, 0, 2, 0, 3, 4, 0, 0, 0, 9, 0, 1, 0},
			{0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 2, 0, 0, 0, 0}			
		};
		SVIInt rowOffsetMapDown[9][15] = {
			{0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0},
			{2, 0, 1, 2, 0, 3, 0, 0, 0, 1, 0, 0, 1, 0, 1},
			{3, 2, 3, 1, 2, 3, 3, 1, 1, 0, 2, 2, 0, 1, 1},
			{4, 1, 0, 4, 2, 4, 2, 2, 1, 0, 2, 3, 1, 4, 4},
			{6, 5, 5, 6, 4, 7, 4, 4, 4, 5, 5, 4, 5, 3, 4},
			{6, 6, 6, 4, 4, 5, 5, 6, 5, 4, 8, 8, 6, 6, 5},
			{8, 7, 8, 7, 7, 6, 8, 6, 7, 6, 8, 8, 8, 7, 6},
			{9, 8, 7, 8, 8, 9, 8, 7, 7, 6, 9, 9, 8, 9, 9},
			{9, 8, 9, 9, 9, 9, 8, 9, 9, 7, 9, 9, 8, 9, 8}			
		};
		SVIInt rowOffsetMapLeft[9][15] = {
			{8, 7, 6, 5, 5, 4, 3, 2, 2, 1, 0, 0, 0, 0, 0},
			{9, 8, 6, 6, 4, 3, 4, 2, 2, 1, 1, 1, 0, 0, 0},
			{7, 7, 5, 5, 5, 4, 4, 3, 3, 2, 2, 2, 0, 1, 0},
			{7, 6, 7, 6, 5, 5, 2, 2, 1, 2, 2, 0, 1, 1, 0},
			{8, 6, 5, 6, 4, 7, 5, 5, 3, 2, 2, 0, 1, 0, 0},
			{8, 6, 5, 5, 2, 6, 5, 3, 3, 3, 2, 2, 0, 1, 1},
			{8, 9, 9, 8, 7, 6, 6, 4, 4, 3, 3, 2, 1, 0, 0},
			{8, 9, 8, 6, 5, 5, 5, 4, 4, 2, 1, 1, 1, 1, 0},
			{9, 9, 6, 8, 6, 6, 6, 5, 7, 2, 2, 2, 0, 0, 1}			
		};
		SVIInt rowOffsetMapRight[9][15] = {
			{0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 7, 8},
			{0, 0, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, 7, 8, 8},
			{0, 2, 3, 2, 2, 3, 3, 3, 4, 4, 5, 6, 7, 8, 8},
			{1, 1, 2, 4, 2, 7, 3, 5, 6, 6, 6, 7, 6, 7, 7},
			{0, 1, 2, 3, 4, 7, 6, 6, 7, 7, 6, 6, 6, 8, 9},
			{0, 0, 0, 2, 2, 3, 4, 3, 4, 7, 8, 8, 6, 8, 9},
			{0, 0, 1, 4, 4, 3, 4, 4, 4, 6, 8, 8, 8, 9, 9},
			{0, 0, 1, 3, 4, 4, 5, 6, 5, 6, 9, 9, 9, 8, 8},
			{0, 1, 1, 3, 6, 5, 5, 6, 6, 7, 7, 9, 9, 9, 9}			
		};

		SVIInt columnOffsetMapUp[15][9] = {
			{9, 9, 9, 7, 9, 9, 9, 9, 8},
			{9, 7, 9, 9, 7, 8, 9, 9, 8},
			{8, 7, 7, 7, 8, 8, 9, 8, 9},
			{8, 6, 7, 6, 6, 5, 7, 5, 6},
			{7, 5, 6, 5, 5, 6, 4, 4, 6},
			{7, 5, 5, 7, 7, 5, 5, 3, 5},
			{6, 4, 3, 2, 4, 4, 3, 3, 2},
			{4, 4, 3, 2, 3, 3, 4, 4, 5},
			{3, 3, 3, 1, 2, 3, 4, 4, 7},
			{3, 3, 3, 0, 2, 2, 2, 6, 4},
			{1, 1, 2, 2, 2, 2, 2, 9, 2},
			{0, 1, 2, 0, 0, 1, 2, 1, 1},
			{0, 1, 0, 1, 1, 1, 1, 1, 0},
			{0, 0, 0, 1, 1, 0, 1, 0, 0},
			{0, 1, 1, 0, 0, 0, 0, 1, 0}					
		};
		SVIInt columnOffsetMapDown[15][9] = {
			{0, 1, 1, 0, 0, 0, 0, 1, 0},
			{0, 0, 0, 1, 1, 0, 1, 0, 0},
			{0, 1, 0, 1, 1, 1, 1, 1, 0},
			{0, 1, 2, 0, 0, 1, 2, 1, 1},
			{1, 1, 2, 2, 2, 2, 2, 9, 2},
			{3, 3, 3, 0, 2, 2, 2, 6, 4},
			{3, 3, 3, 1, 2, 3, 4, 4, 7},
			{4, 4, 3, 2, 3, 3, 4, 4, 5},
			{6, 4, 3, 2, 4, 4, 3, 3, 2},
			{7, 5, 5, 7, 7, 5, 5, 3, 5},
			{7, 5, 6, 5, 5, 6, 4, 4, 6},
			{8, 6, 7, 6, 6, 5, 7, 5, 6},
			{8, 7, 7, 7, 8, 8, 9, 8, 9},
			{9, 7, 9, 9, 7, 8, 9, 9, 8},
			{9, 9, 9, 7, 9, 9, 9, 9, 8}	
		};
		SVIInt columnOffsetMapLeft[15][9] = {
			{8, 7, 5, 3, 2, 1, 1, 1, 0},
			{8, 8, 2, 5, 5, 0, 2, 0, 0},
			{8, 7, 7, 5, 5, 3, 2, 1, 0},
			{7, 6, 6, 5, 4, 1, 1, 1, 0},
			{9, 8, 8, 4, 2, 2, 0, 0, 0},
			{9, 9, 8, 6, 2, 1, 1, 0, 1},
			{8, 7, 6, 2, 4, 3, 1, 2, 0},
			{8, 7, 6, 4, 5, 3, 2, 1, 1},
			{0, 9, 7, 7, 5, 3, 2, 2, 1},
			{9, 9, 7, 5, 3, 3, 0, 0, 0},
			{8, 8, 7, 4, 3, 2, 0, 1, 0},
			{9, 8, 7, 5, 4, 3, 2, 1, 0},
			{9, 8, 5, 4, 4, 2, 2, 1, 0},
			{9, 8, 9, 7, 6, 6, 3, 1, 0},
			{9, 9, 9, 7, 5, 3, 3, 3, 1}					
		};
		SVIInt columnOffsetMapRight[15][9] = {
			{0, 0, 2, 3, 4, 5, 7, 7, 8},
			{0, 0, 2, 5, 5, 6, 7, 8, 8},
			{0, 1, 1, 0, 5, 5, 8, 8, 8},
			{0, 0, 0, 6, 6, 7, 7, 8, 9},
			{0, 0, 2, 4, 5, 6, 7, 9, 9},
			{0, 1, 1, 7, 7, 6, 8, 9, 9},
			{1, 1, 3, 2, 5, 5, 6, 8, 9},
			{0, 0, 3, 2, 4, 5, 7, 9, 9},
			{0, 0, 3, 1, 4, 5, 6, 7, 7},
			{1, 1, 3, 6, 6, 7, 8, 8, 8},
			{1, 0, 2, 4, 5, 7, 8, 9, 8},
			{0, 3, 5, 6, 6, 8, 8, 9, 9},
			{0, 1, 0, 2, 6, 6, 8, 8, 9},
			{0, 0, 1, 2, 3, 6, 9, 9, 9},
			{0, 1, 1, 2, 4, 6, 8, 9, 9}					
		};


		for (int i=0; i<mColumnCount; i++) {
			for (int j=0; j<mRowCount; j++) {
				if(mTextureWidth<mTextureHeight) {
					if (mDirectionType == SVITransitionAnimation::UP)
						mOffsetMap[i][j] = columnOffsetMapUp[i][j];
					else if (mDirectionType == SVITransitionAnimation::DOWN)
						mOffsetMap[i][j] = columnOffsetMapDown[i][j];
					else if (mDirectionType == SVITransitionAnimation::LEFT)
						mOffsetMap[i][j] = columnOffsetMapLeft[i][j];
					else if (mDirectionType == SVITransitionAnimation::RIGHT)
						mOffsetMap[i][j] = columnOffsetMapRight[i][j];
				} else {
					if (mDirectionType == SVITransitionAnimation::UP)
						mOffsetMap[i][j] = rowOffsetMapUp[i][j];
					else if (mDirectionType == SVITransitionAnimation::DOWN)
						mOffsetMap[i][j] = rowOffsetMapDown[i][j];
					else if (mDirectionType == SVITransitionAnimation::LEFT)
						mOffsetMap[i][j] = rowOffsetMapLeft[i][j];
					else if (mDirectionType == SVITransitionAnimation::RIGHT)
						mOffsetMap[i][j] = rowOffsetMapRight[i][j];
				}
			}
		}


		setOffsetDuration(mFullTimeDuration);

		for (int i=0; i<mColumnCount; i++) {

			for (int j=0; j<mRowCount; j++) {
				SVIAnimationSet* fromAniSet = new SVIAnimationSet(mSVIGLSurface);
				fromAniSet->mOffset = mGlovalOffsetDuration;
				fromAniSet->mDuration = mFullTimeDuration;
				fromAniSet->mRepeatCount = mRepeatCount;
				fromAniSet->mAutoReverse = mAutoReverse;

				SVIKeyFrameAnimation* fromOpacityAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY, mSVIGLSurface);
				fromOpacityAni->mDuration = mSlideDuration;
				fromOpacityAni->mOffset = mOffsetDuration*mOffsetMap[i][j] + mGlovalOffsetDuration;
				fromOpacityAni->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
				fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 1.0f));			
				fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(0.9f, 1.0f));
				fromOpacityAni->addKeyProperty(SVIKeyFrameProperty(1.0f, 0.0f));
				fromAniSet->addAnimation(fromOpacityAni);

				SVIKeyFrameAnimation* fromPositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION, mSVIGLSurface);
				fromPositionAni->mDuration = mSlideDuration;
				fromPositionAni->mOffset = mOffsetDuration*mOffsetMap[i][j] + mGlovalOffsetDuration;
				fromPositionAni->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
				fromPositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIPoint(mFromNslide[i][j]->getPosition().mX, mFromNslide[i][j]->getPosition().mY)));
				fromPositionAni->addKeyProperty(SVIKeyFrameProperty(0.9f, SVIPoint(mFromNslide[i][j]->getPosition().mX, mFromNslide[i][j]->getPosition().mY + slideHeight*(mColumnCount-i))));
				fromPositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIPoint(mFromNslide[i][j]->getPosition().mX, mFromNslide[i][j]->getPosition().mY + slideHeight*(mColumnCount-i) + 10.0f)));
				fromAniSet->addAnimation(fromPositionAni);

				SVIKeyFrameAnimation* fromRotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION, mSVIGLSurface);
				fromRotateAni->mDuration = mSlideDuration;
				fromRotateAni->mOffset = mOffsetDuration*mOffsetMap[i][j] + mGlovalOffsetDuration;
				fromRotateAni->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
				SVIQuaternion q;
				q.setIdentity();				
				fromRotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
				q.setEuler(SVIVector3(80.0f*i, 50.0f*j, 0));
				fromRotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
				fromAniSet->addAnimation(fromRotateAni);

				SVIKeyFrameAnimation* fromZpositionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION, mSVIGLSurface);
				fromZpositionAni->mDuration = mSlideDuration;
				fromZpositionAni->mOffset = mOffsetDuration*mOffsetMap[i][j] + mGlovalOffsetDuration;
				fromZpositionAni->setInterpolator(SVIAnimation::ACCELERATE_DECELERATE);
				fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, 0.0f));
				fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.2f, -0.1f * j));
				fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(0.7f, -1.0f));
				fromZpositionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, -2.0f));
				fromAniSet->addAnimation(fromZpositionAni);

				mFromNslide[i][j]->addAnimation(fromAniSet);
			}
		}

		SVI_SVIFE_DELETE_ARRAY(mOffsetMap);
	}

	void SVIBreakEffector::setOffsetDuration(SVIInt fullTimeDuration) {
		fullTimeDuration = mFullTimeDuration;
		mOffsetDuration = fullTimeDuration*0.05f;
		mSlideDuration = fullTimeDuration - (mOffsetDuration*9);
	}

}
