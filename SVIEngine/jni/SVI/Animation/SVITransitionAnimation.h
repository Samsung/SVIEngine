#ifndef __SVI_TRANSITIONANIMATION_H_
#define __SVI_TRANSITIONANIMATION_H_

#include "../Slide/SVISlide.h"
#include "SVIAnimation.h"
#include <vector>

namespace SVI {
	
	class SVITransitionEffector;
	
	class SVITransitionAnimation : public SVIAnimation {
	public:
		enum TransitionType	{

			BLIND,					//0
			BREAK,					//1
			CURTAIN,				//2
			DOMINOFLIP,				//3
			TUMBLE,					//4
			WIZZLE,					//5
			ROTATE,					//6
            ROTATE_CUBE,			//7
			SIDEWINDOW,				//8
			SLIDE,					//9
			WAVE,					//10
			ZOOMIN,					//11
			EXPLOID,				//12
			FLIP,					//13
			SCALE,					//14
			SMOOTH,					//15
			TWIST,					//16
			BOX,					//17
			CHECKER_BOARD,			//18
			COVER,					//19
			FLASH,					//20
			FLIP_2,					//21
			FLY_THROUGH,			//22
			GALLERY,				//23
			HONEYCOMB,				//24
			REVEAL,					//25
			SHAPE,					//26
			SHRED,					//27
			SPLIT,					//28
			SWITCH,					//29
			UNCOVER,				//30
			WIPE,					//31
			CENTER_BLIND,			//32
			FADE_THROUGH_COLOR,		//33
			FALL,					//34
			MOVE_IN,				//35
			REVOLVING_DOOR,			//36
			SWAP,					//37
			SWOOSH,					//38
			TWIRL,					//39
			BRICK_CUBE,				//40
			CUBE_2PIECES,			//41
			CUBE_4PIECES,			//42
            FOLDING_SCREEN,         //43
			GALLERY_2,				//44

		};				



		
									
									
		enum DirectionType {
			LEFT,
			RIGHT,
			UP,
			DOWN
		};

		SVITransitionAnimation(SVIGLSurface* saGLSurface);
		static SVITransitionAnimation* createAnimation(TransitionType type, SVIGLSurface* saGLSurface);
		
		// @Override method
		void setBeamedSlide(SVIBeamedSlide* slide);
		void update(SVIUInt& time);
		void calculateAndApply(const SVIFloat& ratio){};
		SVIAnimation* clone();
		virtual void print();

		void stopCaptureSlide();

		// @Virtual method
		virtual ~SVITransitionAnimation();

		virtual void forceEnd();

		SVITransitionEffector* getEffector() { return mEffector; }
		DirectionType getDirectionType() { return mDirectionType; }
		void setDirectionType(DirectionType type) { mDirectionType = type; } 
		
		void prepareTransitionAnimation(SVIBeamedSlide* slide);
		
	protected :
		void copy(const SVITransitionAnimation* rhs);

		//2012-04-09 masterkeaton27@gmail.com
		void processEnd();
	public:
		SVISize					mPrevWindowSize;
		SVISize					mCurWindowSize;

	private:
		TransitionType			mTransitionType;
		SVIBeamedSlide*			mFromSlide;
		SVIBeamedSlide*			mToSlide;
		SVITransitionEffector*	mEffector;
		SVIBool					mIsTransition;
		DirectionType			mDirectionType;
	};

}
#endif
