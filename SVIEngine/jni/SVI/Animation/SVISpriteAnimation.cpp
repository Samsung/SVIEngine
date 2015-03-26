#include "SVISpriteAnimation.h"
#include "../Slide/SVIBeamedSlide.h"

namespace SVI {
	SVISpriteAnimation::SVISpriteAnimation(PlayType type, SVIGLSurface* saGLSurface): SVIAnimation(saGLSurface){
		mPlayType = type;
		mImage = NULL;
		mFrameWidth = 0;
		mFrameHeight = 0;
		mFromIndex = -1;
		mToIndex = -1;
		mCurrentPlayingIndex = -1;
		mRowCount = 0;
		mColumnCount = 0;

		mInitialized = SVIFALSE;

		mClassType = SPRITE;
	}

	SVISpriteAnimation::~SVISpriteAnimation() {
		mImage = NULL;
	}

	SVISpriteAnimation* SVISpriteAnimation::create(PlayType type, SVIGLSurface* saGLSurface) {
		SVISpriteAnimation* newAnimation = new SVISpriteAnimation(type, saGLSurface);
		return newAnimation;
	}

	void SVISpriteAnimation::setImage(SVIImage* image) {
		mImage = image;
	}

	void SVISpriteAnimation::setFrameInfo(SVIInt frameWidth, SVIInt frameHeight) {
		mFrameWidth = frameWidth;
		mFrameHeight = frameHeight;
	}

	void SVISpriteAnimation::setPlayInterval(SVIInt fromIndex, SVIInt toIndex) {
		mFromIndex = fromIndex;
		mToIndex = toIndex;
	}

	void SVISpriteAnimation::update(SVIUInt& time ) {
		updateInfo(time);

		if( getStatus() == ANIMATING ) {
			calculateAndApply(mTimeRatio);

			if( isEnd(time) ) {
				setStatus(END);
				if( !mIsContainedInAniSet && mSlide != NULL ) {
					mSlide->removeAnimation();
				}
			}
		}
	}

	SVIAnimation* SVISpriteAnimation::clone() {
		SVISpriteAnimation* cloneAnimation = new SVISpriteAnimation(this->mPlayType, mSVIGLSurface);
		cloneAnimation->copy(this);
		return (SVIAnimation*)cloneAnimation;
	}
	
	void SVISpriteAnimation::calculateAndApply(const SVIFloat& ratio) {
		if( !mInitialized )
			initialize();

		if( !mInitialized )
			return;

		SVIInt totalPlayFrameCount = mToIndex - mFromIndex;
		
		mCurrentPlayingIndex = mFromIndex + (SVIInt)(totalPlayFrameCount * ratio);

		SVIInt xIndex = mCurrentPlayingIndex % mColumnCount;
		SVIInt yIndex = mCurrentPlayingIndex / mColumnCount;

		SVIRect textureRegion;
		textureRegion.mOrigin.mX = xIndex * mFrameWidth;
		textureRegion.mOrigin.mY = yIndex * mFrameHeight;
		textureRegion.mSize.mWidth = mFrameWidth;
		textureRegion.mSize.mHeight = mFrameHeight;

		mSlide->setTextureRegion(textureRegion);
	}

	void SVISpriteAnimation::initialize() {
		
		if( mFrameWidth <= 0 || mFrameHeight <=0 ||
			(mImage->mWidth % mFrameWidth) ||  
			(mImage->mHeight % mFrameHeight) ) {
			LOGE("Frame width or height is wrong!!!");

			return;
		}

		mColumnCount = mImage->mWidth / mFrameWidth;
		mRowCount = mImage->mHeight / mFrameHeight;

		if( mPlayType == PLAY_ALL) {
			mFromIndex = 0;
			mToIndex = (mRowCount * mColumnCount)-1;

			mInitialized = SVITRUE;
		} else {
			if( mFromIndex < 0 || mToIndex >= (mRowCount*mColumnCount) ) {
				LOGE("Frame Interval is invalid value!!!");
				return;
			}

			mInitialized = SVITRUE;
		}

		mSlide->setImage(mImage);
	}

	void SVISpriteAnimation::copy(const SVISpriteAnimation* rhs) {
		if( rhs == NULL )
			return;

		SVIAnimation::copy(rhs);

		mPlayType = rhs->mPlayType;
		mImage = rhs->mImage;
		mFrameWidth = rhs->mFrameWidth;
		mFrameHeight = rhs->mFrameHeight;
		mFromIndex = rhs->mFromIndex;
		mToIndex = rhs->mToIndex;
		mCurrentPlayingIndex = rhs->mCurrentPlayingIndex;
		mRowCount = rhs->mRowCount;
		mColumnCount = rhs->mColumnCount;
	}

	void SVISpriteAnimation::print() {
		LOGI("AniClassType : SpriteAnimation");
	}
}
