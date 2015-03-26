#ifndef __SVI_ANIMATION_H_
#define __SVI_ANIMATION_H_

#include "../SVICores.h"
#include "../GLSurface/SVIGLSurface.h"


namespace SVI {

	class SVIBeamedSlide;
	class SVIInterpolator;
	class SVIAnimationListener;

	class SVIAnimation {
	public:
		SVIAnimation(SVIGLSurface* saGLSurface);
		virtual ~SVIAnimation();

		// status change sequence
		// start animation status transition : IDLE => START => ANIMATING => END
		// end animation status transition : ANIMATING => END => IDLE
		// cancel animation status transition : ANIMATING => END => IDLE
		enum AnimationStatus {IDLE = 0, START, ANIMATING, END };
		enum InterpolatorType { LINEAR = 0, 
                                ACCELERATE, 
                                DECELERATE, 
                                ACCELERATE_DECELERATE, 
                                ANTICIPATE, 
                                ANTICIPATE_OVERSHOOT, 
                                BOUNCE, 
                                OVERSHOOT, 
                                CYCLE,
								BACKEASEIN,
								BACKEASEINOUT,
								BACKEASEOUT,
								BOUNCEEASEIN,
								BOUNCEEASEINOUT,
								BOUNCEEASEOUT,
								CIRCULAREASEIN,
								CIRCULAREASEINOUT,
								CIRCULAREASEOUT,
								CUBICEASEIN,
								CUBICEASEINOUT,
								CUBICEASEOUT,
								ELASTICEASEIN,
								ELASTICEASEINOUT,
								ELASTICEASEOUT,
								EXPONENTIALEASEIN,
								EXPONENTIALEASEINOUT,
								EXPONENTIALEASEOUT,
								QUADEASEIN,
								QUADEASEINOUT,
								QUADEASEOUT,
								QUARTEASEIN,
								QUARTEASEINOUT,
								QUARTEASEOUT,
								QUINTEASEIN,
								QUINTEASEINOUT,
								QUINTEASEOUT,
								SINEEASEIN,
								SINEEASEINOUT,
								SINEEASEOUT,
								STRONGEASEIN,
								STRONGEASEINOUT,
								STRONGEASEOUT };
		
		enum AnimationType		{ NONE, BASIC, KEYFRAME, TRANSITION, SPRITE, ANIMATION_SET};


		SVIGLSurface* getSVIGLSurface();
		void setInterpolator(InterpolatorType type);
		void setListener(SVIAnimationListener* listener) { mListener = listener; }
		SVIAnimationListener* getListener() { return mListener; }
		void setID(SVIUInt id) { mID = id; }
		void setIsContainedInAniSet(SVIBool isContained ) { mIsContainedInAniSet = isContained; }
		SVIBool isContainedInAniSet() { return mIsContainedInAniSet; }
		SVIUInt getID() { return mID; }
		virtual void setBeamedSlide(SVIBeamedSlide* slide) { mSlide = slide; }
		SVIBeamedSlide* getBeamedSlide() { return mSlide; }
		virtual void update(SVIUInt& time)=0;
		void cancel();
		virtual void forceEnd();

		//endAnimation function will be called before delete animation object from RenderPartManager.
		void endAnimation(SVIUInt time);

		virtual void updateInfo(SVIUInt time);
		virtual void calculateAndApply(const SVIFloat& ratio) = 0;
		virtual SVIAnimation* clone() = 0;
		virtual void print()=0;

		AnimationType getClassType() { return mClassType; }

		void startAnimation(SVIUInt time);

	protected:
		//startAnimatio function will be called before rendering animation from updateInfo.

		void setStatus(AnimationStatus newStatus);
		inline AnimationStatus getStatus() { return mStatus; }
		virtual SVIBool isEnd(SVIUInt& time);
		void copy(const SVIAnimation* rhs);

	private:
		SVIInterpolator* createInterpolator(InterpolatorType type);
		SVIAnimation(const SVIAnimation& rhs){copy(&rhs);}
		void operator=(const SVIAnimation& rhs){copy(&rhs);}


	protected:
		AnimationType				mClassType;
		SVIBeamedSlide*				mSlide;
		SVIAnimationListener*		mListener;
		SVIInterpolator*				mInterpolator;
		InterpolatorType			mInterpolatorType;
		SVIFloat						mTimeRatio;
		SVIUInt						mCurrentCount;
		SVIUInt						mID;
		SVIBool						mIsContainedInAniSet;

		SVIGLSurface*				mSVIGLSurface;


	private:
		AnimationStatus				mStatus;

	public :
		SVIUInt						mDuration;
		SVIUInt						mRepeatCount;
		SVIBool						mAutoReverse;
		SVIUInt						mOffset;
	};

	class SVIAnimationListener {
		public:
			virtual void onAnimationEnd(SVIAnimation* animation) = 0;
			virtual void onAnimationRepeat(SVIAnimation* animation) = 0;
			virtual void onAnimationStart(SVIAnimation* animation) = 0;
	};

}
#endif //__SVI_ANIMATION_H_
