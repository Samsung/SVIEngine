#ifndef __SVI_TRANSITIONEFFECTOR_H_
#define __SVI_TRANSITIONEFFECTOR_H_

#include "../SVITransitionAnimation.h"

namespace SVI {
	
	class SVIBeamedSlide;
	class SVIAnimationSet;
	class SVIGLSurface;
	// Creation Date	: 2011/11/16 phyuzion@gmail.com
	// Description		: interface TransitionEffector
	// Last Modified	: 2011/11/16 phyuzion@gmail.com
	class SVITransitionEffector {
	public:	
		static SVITransitionEffector* createEffector(SVIGLSurface *surface, SVITransitionAnimation::TransitionType type);

		SVIBool initialize(SVITransitionAnimation* transAni, SVIBeamedSlide *sizeSlide, SVIBeamedSlide *parentSlide, SVIBeamedSlide *fromSlide, SVIBeamedSlide *toSlide, SVIInt duration, SVIInt offset, SVIInt repeatCount, SVIInt transitionType);
		virtual void updateToSlideInfo(SVIBeamedSlide* toSlide);
		void cancelAnimation();
		void removeNslide();
		// @Virtual method
		virtual ~SVITransitionEffector();
		
	protected:
		SVITransitionEffector(SVIGLSurface *surface);
		void setRowColumnCount();
		void prepareEffect(SVIInt transitionType);
		void makeNslide();
		void makeBrickSlide();

		virtual void setAnimation() = 0;


		void addRotationAnimation(SVIAnimationSet* aniSet, SVIVector3 preAngle, SVIVector3 nextAngle, SVIInt duration, SVIInt offset);
		void addScaleAnimation(SVIAnimationSet* aniSet, SVIVector3 preScale, SVIVector3 nextScale, SVIInt duration, SVIInt offset);
		void addRegionAnimation(SVIAnimationSet* aniSet, SVIRect preRect, SVIRect nextRect, SVIInt duration, SVIInt offset);
		void addTextureRegionAnimation(SVIAnimationSet* aniSet, SVIRect preRect, SVIRect nextRect, SVIInt duration, SVIInt offset);
		void addPositionAnimation(SVIAnimationSet* aniSet, SVIPoint prePosition, SVIPoint nextPosition, SVIInt duration, SVIInt offset);
		void addZPositionAnimation(SVIAnimationSet* aniSet, SVIFloat preZPosition, SVIFloat nextZPosition, SVIInt duration, SVIInt offset);
		void addOpacityAnimation(SVIAnimationSet* aniSet, SVIClampf preOpacity, SVIClampf nextOpacity, SVIInt duration, SVIInt offset);
		void addScaleToFitRegionAnimation(SVIAnimationSet* aniSet, SVIRect preRect, SVIRect nextRect, SVIInt duration, SVIInt offset);
		void addDeformAnimation(SVIAnimationSet* aniSet, SVIVector3 preDeform, SVIVector3 nextDeform, SVIInt duration, SVIInt offset);
		void addColorAnimation(SVIAnimationSet* aniSet, SVIColor preColor, SVIColor nextColor, SVIInt duration, SVIInt offset);

		//2012-06-21 masterkeaton27@gmail.com
		//new shader effect property for mobile office
		void addSwipeAnimation(SVIAnimationSet* aniSet, SVIVector3 preSwipe, SVIVector3 nextSwipe, SVIInt duration, SVIInt offset);
	protected:
		SVIInt mFullTimeDuration;
		SVIInt mGlovalOffsetDuration;
		SVIInt mOffsetDuration;
		SVIInt mSlideDuration;
		SVIInt mTransitionType;

		SVIUInt mRepeatCount;
		SVIBool mAutoReverse;

		SVIBool mIsUseParent;
        SVIBool mIsUseClone;

		SVIBool mIsUseFakeParent;
		SVIFloat mTextureWidth;
		SVIFloat mTextureHeight;

		SVIInt mRowCount;
		SVIInt mColumnCount;

		SVIBeamedSlide *mFromSlide;
		SVIBeamedSlide *mToSlide;

        SVIBeamedSlide *mFromCloneSlide;
		SVIBeamedSlide *mToCloneSlide;

		SVIBeamedSlide *mFromNParentSlide;
		SVIBeamedSlide *mToNParentSlide;

		SVIBeamedSlide *mFakeParentSlide;
		SVIBeamedSlide* **mFromNslide;
		SVIBeamedSlide* **mToNslide;

		SVIBeamedSlide *mSizeSlide;
		SVIBeamedSlide *mParentSlide;

		SVITransitionAnimation *mTransitaionAni;
		SVITransitionAnimation::DirectionType	mDirectionType;
		
		SVISize mPrevWindowSize;
		SVISize mCurWindowSize;

		SVIFloat mRandomValue;
		SVIGLSurface* mSVIGLSurface;

	};

} // end namespace SVI
#endif //__SVI_TRANSITIONANIMATION_H_
