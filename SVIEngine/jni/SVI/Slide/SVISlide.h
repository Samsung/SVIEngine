#ifndef __SVI_SLIDE_H_
#define __SVI_SLIDE_H_

#include "SVIBeamedSlide.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVIPropertyAnimation.h"
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIAnimationListenerProxy.h"
#include "../Particle/SVIParticleEffect.h"
#include "SVIRenderPartManager.h"
#include <list>
#include <map>
#include "../GLSurface/SVIGLSurface.h"
namespace SVI {

	class SVIBeamedSlide;
	class SVIAnimationListenerProxy;
	class SVISlideForInitProperty;

	#define USE_QUEUE_FOR_SLIDE_PROPRETY 1

	class SVISlide : public SVIBaseSlide {

		typedef std::map<SVIInt, SVIInt>				InitPropertyMap;

		public:
			enum ImplicitAnimationSet {	
				ISSETPROPERTY = 0,
				INTERPOLATORTYPE = 1,
				DURATION = 2, 
				REPEATCOUNT = 3,
				OFFSET = 4,
				AUTOREVERSE = 5,
				SET_COUNT
			};

			enum RotationType {
				COUNTERCLOCKWISE_90 = 0,
				CLOCKWISE_90 = 1,
				CLOCKWISE_180 = 2,
			};

			SVISlide(SVIGLSurface* surface);
			SVISlide(SVIGLSurface* surface, SVIInt type);
			virtual ~SVISlide();
			virtual SVIBool initialize(SVISlide* parent, const SVIRect& region, const SVIColor& bgColor, SVIImage* image/*, SVIBool text*/);

			/* debug function */
			void displaySlideTrees();
			void printSubSlides(SVIInt depth);
			
			/**
			* Restores currently used textures to the present gl context.
			*/
			void restoreImages();
			
			/* */
			virtual void addChangePropery(PropertyChangeType type);
			SVIBeamedSlide* getBeamedSlide();
			SVIInt getSlideType();			
			void setSurface(SVIGLSurface* saGLSurface);//Multi-Instance-Support
			/* initProperty functions is for setting from value of implicit animation */
			void initRegion(const SVIRect& region);
			void initPosition(const SVIPoint& position);
			void initPivotPoint(const SVIPoint& pivotPoint);
			void initZPosition(const SVIFloat& zPosition);
			void initRotation(const SVIVector4& rotation);
			void initSwipeRatio(const SVIVector3& ratio);
			void initDeformRatio(const SVIVector3& ratio);
			void initRTERatio(const SVIVector3& ratio);
			void initModelPosition(const SVIVector3& position);
			void initModelRotation(const SVIVector4& rotation);
			void initModelScale(const SVIVector3& scale);
			void initScale(const SVIVector3& scale);
			void initBgColor(const SVIColor& color);
			void initOpacity(const SVIFloat& opacity);
			void initCornerRadius(const SVIFloat& radius);
			void initBorderWidth(const SVIFloat& width);
			void initBorderColor(const SVIColor& color);
			void initShadowColor(const SVIColor& color);
			void initShadowRadius(const SVIFloat& radius);
			void initShadowOffset(const SVIPoint& offset);
			void initShadowOpacity(const SVIFloat& opacity);
			void initLightOpacity(const SVIFloat& opacity);
			void initLightColor(const SVIColor& color);
			void initLightOffset(const SVIPoint& offset);
			void initLightRadius(const SVIFloat& radius);
			void initLightAngle(const SVIFloat& angle);
			void initLightPower(const SVIFloat& power);
			void initTextureRegion(const SVIRect& region);
			void initBackFaceTextureRegion(const SVIRect& region);
			void initTextureScaleFitRegion(const SVIRect& region);
            void initLightDirection(const SVIVector4& direction);
			void initViewDirection(const SVIVector4& direction);

			void setRegion(const SVIRect& region);
			//void setBounds(const SVIRect& bounds);
			void setPosition(const SVIPoint& position);
			void setPivotPoint(const SVIPoint& pivotPoint);
			void setZPosition(const SVIFloat& zPosition);
			void setRotation(const SVIVector4& rotation);
			void setModelRotation(const SVIVector4& rotation);
			void setSwipeRatio(const SVIVector3& ratio);
			void setDeformRatio(const SVIVector3& ratio);
			void setRTERatio(const SVIVector3& ratio);
			void setScale(const SVIVector3& scale);
			void setBgColor(const SVIColor& color);
			void setClipSubSlide(SVIBool clipSubSlide);
			void setOpacity(SVIFloat opacity);
			void setCornerRadius(SVIFloat radius);
			void setBorderWidth(SVIFloat width);
			void setBorderColor(const SVIColor& color);
			void setShadowColor(const SVIColor& color);
			void setShadowRadius(SVIFloat radius);
			void setShadowOffset(const SVIPoint& offset);
			void setShadowOpacity(const SVIFloat& opacity);
			void setLightOpacity(const SVIFloat& opacity);
			void setLightColor(const SVIColor& color);
			void setLightOffset(const SVIPoint& offset);
			void setLightType(const SVIInt& type); 
			void setLightRadius(const SVIFloat& radius);
			void setLightAngle(const SVIFloat& angle);
			void setLightPower(const SVIFloat& power);
			void setZOrderType(SlideZOrderType type);
			void setSwipeType(SlideSwipeType type);
			void setDeformType(SlideDeformType type);
            void setFilterType(SlideFilterType type);
			void setDeformDirection(SlideDeformDirection direction);
            //2012/12/08 rareboy0112@facebook.com
            //add morphing method.
            void setMorphingType(SlideMorphingType type);
            void setMorphingRadius(const SVIFloat radius);
            void setMorphingVertexCount(SVIInt gridCount);
            void setMorphingColor(SVIColor morphingColor);
			void setBlendType(SlideBlendType type);
			void setAntiAliasing(SVIBool enableAA);
			void setRTEffectType(SlideRTEffectType type);
			void setImageScaleType(SlideImageScaleType type);
			void setOrthogonal(SVIBool isOrtho);
            void setUseNormalMap(SVIBool isUseNormalMap);
			void setCustomRTE(SVIBool isCustomRTE);
			void setHidden(SVIBool hidden);
			void setHoldOpacity(SVIBool hold);
			void setHoldScale(SVIBool hold);
			void setCapture(SVIBool capture);
			void setTextureRegion(const SVIRect& region);
			void setTextureScaleFitRegion(const SVIRect& region);

            //2012/12/07 rareboy0112@facebook.com
            void setTesselationPointList(const std::vector<SVIVector2>& pointList);

			//2012-11-27 masterkeaton27@gmail.com
			//new properties for planner normap mapping
            void setLightDirection(const SVIVector4& direction);
			void setViewDirection(const SVIVector4& direction);

            //2013-06-21 masterkeaton27@gmail.com
            //path point api for path polygon
            void addPathPoint(const SVIVector3& point);
            void clearPathPoints();

			void setRotationFrom(const SVIVector4& rotationFrom, const SVIVector4& rotationTo);

			void setImage(SVIImage* image); 
			SVIImage* getImage();

            void setNormalImage(SVIImage* image); 
            SVIImage* getNormalImage();

            void setSpecularImage(SVIImage* image); 
            SVIImage* getSpecularImage();

			void setBackFaceImage(SVIImage* image);
			SVIImage* getBackFaceImage();

			void setBackFaceImageFlip(SVIBool isFlip);
			SVIBool getBackFaceImageFlip();

			void setBackFaceImageClear(SVIBool isClear);
			SVIBool getBackFaceImageClear();


			void setImplicitAnimationListenerInfo(SVIInt proxy, SVIUInt listenerID);			
			
			/* slide list management function */
			void addModelSlide(SVISlide* slide, SVIInt index);	
			void addModelSlide(SVISlide* slide);	
			void removeModelSlide(SVISlide* slide);
			void addSlide(SVISlide* slide, SVIInt index);
			void addSlide(SVISlide* slide);
			void removeSlide(SVISlide* slide);
			void removeAllSlides();
			void removeSlideAt(SVIInt index);
			void removeSlides(SVIInt start, SVIInt count);

			/* default animation function */
			virtual SVIBool addAnimation(SVIAnimation* animation);
		    virtual void removeAnimation();
			
			void addParticleEffect(SVIParticleEffect* effect);

			/* slide implicit anmation function */
			void setExplicitAnimation(SVIAnimation* animation);
			void setImplicitAnimation(SVIInt type, SVIInt interpolatorType, SVIInt duration, SVIInt repeatCnt, SVIInt offset, SVIInt autoReverse);
			
			void startAnimation(SVIAnimation* animation);
			void stopAnimation();
			void stopAnimationDirectly();
			SVIBool isAnimating();

			void runSlideAnimation();
			void slideProc(SVIMessage& msg);

			void destroySlide();
		protected:
			void addImplicitAnimation(SVIPropertyAnimation::PropertyAnimationType propertyAniType);
			void setAnimationFromAndTo(SVIPropertyAnimation::PropertyAnimationType& propertyAniType, SVIBasicAnimation*& animation);
			void runExplicitAnimation();
		private:
			SVISlide(const SVISlide& rhs);
			void operator=(const SVISlide& rhs);
		protected:
			SVIBeamedSlide* 	mBeamedSlide;
			SVIAnimation*	mExplicitAnimation;
			SVIBool			mIsExplicitSet;
			SVIInt 			mImplicitAnimationInfo[SVIPropertyAnimation::PROPERTY_ANI_COUNT][SET_COUNT];
			SVIAnimationListenerProxy* mImplicitProxy;
			SVIUInt			mImplicitListenerID;
			SVIImage* 		mImage;
            SVIImage* 		mNormalImage;
            SVIImage* 		mSpecularImage;
			SVIImage* 		mBackFaceImage;
			SVIBool			mIsBackFaceImageFlip;
			SVIBool			mIsBackFaceImageClear;
			SVIInt			mSlideType;
			SVIInt 			mRotationType;

			SVIMutex*		mLock;

			InitPropertyMap	mInitPropertyMap;
			SVISlideForInitProperty*		mInitPropertySlide;
			SVIAnimationSet*				mImplicitAnimation;

		public:
			SVIVector4				mRotationFrom;

			
		friend class SVITransitionAnimation;
	};

	class SVISlideForInitProperty : public SVIBaseSlide {
	public:
		SVISlideForInitProperty(SVIGLSurface* surface) : SVIBaseSlide(surface)
		{
		}
		virtual ~SVISlideForInitProperty() {}

		virtual SVIBool		addAnimation(SVIAnimation* animation) { return SVITRUE; }
		virtual void		removeAnimation() {}

		friend class SVISlide;
	};
}

#endif //__SVI_SLIDE_H_
