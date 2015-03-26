#ifndef __SVI_SPRITE_ANIMATION_H_
#define __SVI_SPRITE_ANIMATION_H_

#include "../BaseType/SVIBaseTypes.h"
#include "SVIAnimation.h"

namespace SVI {

	class SVISpriteAnimation : public SVIAnimation {
	public:
		enum	PlayType { PLAY_ALL, PLAY_PARTIAL };
	private:
		SVISpriteAnimation(PlayType type, SVIGLSurface* saGLSurface);

	public:
		virtual ~SVISpriteAnimation();

		static SVISpriteAnimation* create(PlayType type, SVIGLSurface* saGLSurface);
		void setImage(SVIImage* image);
		void setFrameInfo(SVIInt frameWidth, SVIInt frameHeight);
		void setPlayInterval(SVIInt fromIndex, SVIInt toIndex);

		void update(SVIUInt& time);			
		SVIAnimation* clone();
		virtual void print();

	protected:
		void copy(const SVISpriteAnimation* rhs);
		void calculateAndApply(const SVIFloat& ratio);

	private:
		void initialize();

	private:
		PlayType	mPlayType;
		SVIImage*	mImage;
		SVIInt		mFrameWidth;
		SVIInt		mFrameHeight;
		SVIInt		mFromIndex;
		SVIInt		mToIndex;
		SVIInt		mCurrentPlayingIndex;
		SVIInt		mRowCount;
		SVIInt		mColumnCount;

		SVIBool		mInitialized;
	};
};

#endif //__SVI_SPRITE_ANIMATION_H_