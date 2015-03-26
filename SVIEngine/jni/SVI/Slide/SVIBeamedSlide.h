#ifndef __SVI_BEAMED_SLIDE_H_
#define __SVI_BEAMED_SLIDE_H_

#include "../SVICores.h"
#include "SVIBaseSlide.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI{

	class SVISlide;
	class SVIProjectionSlide;
	
	typedef std::vector<SVIInt>						ChangePropertyList;

	class SVIBeamedSlide : public SVIBaseSlide{
	public:

		enum SlideType { BASE_SLIDE=0, IMAGE_SLIDE, TEXT_SLIDE };
		SVIBeamedSlide(SVIGLSurface* surface);
		virtual ~SVIBeamedSlide();


		SVIBool initialize(SVIBeamedSlide* parent, SVISlide* slide, SVIImage* image, SlideType type);
		SVIProjectionSlide* getProjectionSlide() { return mProjectionSlide; }
		void setImage(SVIImage* image);
        void setNormalImage(SVIImage* image);
        void setSpecularImage(SVIImage* image);

		void setBackFaceImage(SVIImage* image);
		void setBackFaceImageFlip(SVIBool isFlip);
		void setBackFaceImageClear(SVIBool isClear);

		SVIInt getSlideType();

		virtual void addChangePropery(PropertyChangeType type);
		void clearChangePropertyList();
		SVIInt getChangePropertyListSize();
		ChangePropertyList&	getChangePropertyList();

		virtual SVIBool addAnimation(SVIAnimation* animation);
		virtual void removeAnimation();
		void stopAnimation();

		SVIAnimation* getAnimation() { return mAnimation; }

		void setRegion(const SVIRect& region);

		SVIBool addSubSlide(SVIBeamedSlide* slide);
		SVIBool removeSubSlide(SVIBeamedSlide* slide);
		SVIBool insertSubSlide(SVIInt index, SVIBeamedSlide* newSlide);
		SVIBool replaceSubSlide(SVIBeamedSlide* oldSlide, SVIBeamedSlide* newSlide);

		void resetAllProperties();
		
		// 2011/09/07 rareboy0112@facebook.com
		// The method is called when the start of the transition is animated.
		SVIBeamedSlide* deepCopy(SVIBool withSubSlide);
		// 2011/09/07 rareboy0112@facebook.com
		// The method is called a transition animation has finished.
		void removeTransitionBeamedSlide();
		SVIBool isAnimating();

		// 2012/03/22 marx.kim
		//workaround code : in case of using math based pte, touch down information can be ignored
		//when user move finger fast on the touch screen.
		//so, especially add property information to message queue to handle touch down information
		//for math based pte.
		void deformRatioTouchDown(float x, float y);


        //2013-06-21 masterkeaton27@gmail.com
        void addPathPoint(const SVIVector3& point);
        void clearPathPoints();

	private:
		// 2011/09/07 rareboy0112@facebook.com
		// The method is called from the getTransitionBeamedSlide method.
		void copySubSlideList(SVIBeamedSlide* parent);
		// 2011/09/07 rareboy0112@facebook.com
		// The method is called from the removeTransitionBeamedSlide method.
		void removeSubSlideList(SVIBeamedSlide* slide);
		
	private:
		SVISlide*				mSlide;
		SVIProjectionSlide*		mProjectionSlide;
		SVIAnimation*			mAnimation;
		ChangePropertyList		mChangePropertyList;
		SlideType				mSlideType;


	friend class SVITransitionEffector;
	};
};

#endif