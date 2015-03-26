#ifndef __SVI_PROPERTY_ANIMATION_H_
#define __SVI_PROPERTY_ANIMATION_H_

#include "SVIAnimation.h"
#include "SVIUnionProperty.h"

namespace SVI {

	class SVIPropertyAnimation : public SVIAnimation{
	public:
		/// Java side code for easy and were numbered.
		enum PropertyAnimationType {	
			NONE = 0,
			REGION = 1,
			BOUND = 2,
			POSITION = 3,
			PIVOT_POINT = 4,
			ZPOSITION = 5,
			ROTATION = 6,
			SCALE = 7,
			BG_COLOR = 8,
			OPACITY = 9,
			CORNER_RADIUS = 10,
			BORDER_WIDTH = 11,
			BORDER_COLOR = 12,
			SHADOW_RADIUS = 13,
			SHADOW_COLOR = 14,
			SHADOW_OPACITY = 15,
			SHADOW_OFFSET = 16,
			TEXTURE_REGION = 17,
			BACKFACE_TEXTURE_REGION = 18,
			LIGHT_RADIUS = 19,
			LIGHT_COLOR = 20,
			LIGHT_OPACITY = 21,
			LIGHT_ANGLE = 22,
			LIGHT_OFFSET = 23,
			LIGHT_POWER = 24,
			TEXTURE_SCALEFIT_REGION = 25,
			ROTATION_FROM = 26,
			SWIPE_RATIO = 27,
			DEFORM_RATIO = 28,
			RTE_RATIO = 29,
			MODEL_POSITION = 30,
			MODEL_ROTATION = 31,
			MODEL_SCALE = 32,
            LIGHT_DIRECTION = 33,
			VIEW_DIRECTION = 34,
            MORPHING_TIME = 35, //2012/12/10 rareboy0112@facebook.com
			PROPERTY_ANI_COUNT
		};

		static const SVIChar* mPropertyAnimationTypeString[];

		SVIPropertyAnimation(SVIGLSurface* saGLSurface);
		SVIPropertyAnimation(PropertyAnimationType type, SVIGLSurface* saGLSurface);
		virtual ~SVIPropertyAnimation();

		virtual void update(SVIUInt time) {}
		virtual SVIAnimation* clone() { 
			LOGE("Critical error!!! this function should not be called!!!");
			return NULL; 
		}

		SVIUInt getPropertyType() { return (SVIUInt)mPropertyAnimationType; }
		static const SVIChar* getPropertyTypeString(PropertyAnimationType type);

	protected :
		virtual void updateInfo(SVIUInt time);
		virtual void calculateAndApply(const SVIFloat& ratio);

		virtual void initData();
		void deInitData();

		void copy(const SVIPropertyAnimation* rhs);
	private:

		void calculateRegion(const SVIFloat& ratio);
		void calculateBound(const SVIFloat& ratio);
		void calculatePosition(const SVIFloat& ratio);
		void calculatePivotPoint(const SVIFloat& ratio);
		void calculateZPosition(const SVIFloat& ratio);
		void calculateRotation(const SVIFloat& ratio);
		void calculateSwipeRatio(const SVIFloat& ratio);
		void calculateDeformRatio(const SVIFloat& ratio);
		void calculateRTERatio(const SVIFloat& ratio);
		void calculateScale(const SVIFloat& ratio);
		void calculateBgColor(const SVIFloat& ratio);
		void calculateOpacity(const SVIFloat& ratio);
		void calculateCornerRadius(const SVIFloat& ratio);
		void calculateBorderWidth(const SVIFloat& ratio);
		void calculateBorderColor(const SVIFloat& ratio);
		void calculateShadowOpacity(const SVIFloat& ratio);
		void calculateShadowRadius(const SVIFloat& ratio);
		void calculateShadowColor(const SVIFloat& ratio);
		void calculateShadowOffset(const SVIFloat& ratio);

		void calculateLightOpacity(const SVIFloat& ratio);
		void calculateLightRadius(const SVIFloat& ratio);
		void calculateLightColor(const SVIFloat& ratio);
		void calculateLightOffset(const SVIFloat& ratio);
		void calculateLightAngle(const SVIFloat& ratio);
		void calculateLightPower(const SVIFloat& ratio);

		void calculateBackFaceTextureRegion(const SVIFloat& ratio);
		void calculateTextureRegion(const SVIFloat& ratio);
		void calculateTextureScaleFitRegion(const SVIFloat& ratio);
		void calculateModelPosition(const SVIFloat &ratio);
		void calculateModelRotation(const SVIFloat& ratio);
		void calculateModelScale(const SVIFloat &ratio);

        void calculateLightDirection(const SVIFloat &ratio);
		void calculateViewDirection(const SVIFloat &ratio);

        //2012/12/10 rareboy0112@facebook.com
        void calculateMorphingTime(const SVIFloat &ratio);

		template<typename T>
		T calculateCurrentValue(const T& from, const T& to, const SVIFloat& ratio);

	public:
		SVIUnionProperty			mFrom;
		SVIUnionProperty			mTo;

	protected:
		PropertyAnimationType	mPropertyAnimationType;

		SVIPVoid					mFromValue; 
		SVIPVoid					mToValue;

	};


	template<typename T>
	T SVIPropertyAnimation::calculateCurrentValue(const T& from, const T& to, const SVIFloat& ratio) {

		T ret;

		if( ratio == 0.0f )
			ret = from;
		else if( ratio == 1.0f )
			ret = to;
		else {
			T distance = to - from;
			ret = from + (T)(distance * ratio);
		}

		return ret;
	}
};
#endif //__SVI_PROPERTY_ANIMATION_H_
