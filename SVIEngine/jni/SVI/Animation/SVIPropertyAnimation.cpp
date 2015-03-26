#include "SVIPropertyAnimation.h"
#include "../Slide/SVIBeamedSlide.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Mathematics/SVIQuaternion.h"
#include "../Particle/SVIParticles.h"

namespace SVI {

const static SVIBool DEBUG = SVIFALSE;

#define SETTING_PROPERTY_VALUE(PropertyType, DataType)	\
	case PropertyType :									\
	{													\
		if( mFromValue == NULL ) 						\
			mFromValue = (SVIPVoid*)new DataType();		\
														\
		if( mToValue == NULL ) 							\
			mToValue = (SVIPVoid*)new DataType();		\
														\
		mFrom.get( *((DataType*)mFromValue) );			\
		mTo.get( *((DataType*)mToValue) );				\
	} break


#define FREE_PROPERTY_VALUE(PropertyType, DataType)		\
	case PropertyType :									\
	{													\
		DataType* from = (DataType*)mFromValue;			\
		DataType* to = (DataType*)mToValue;				\
		SVI_SVIFE_DELETE(from);							\
		SVI_SVIFE_DELETE(to);								\
	} break

#define CALCULATE_CURRENT_VALUE(PropertyType, Name, ratio)		\
	case PropertyType :											\
		calculate##Name(ratio);									\
	break

	const SVIChar* SVIPropertyAnimation::mPropertyAnimationTypeString[] = {
		"NONE=0",
		"REGION",
		"BOUND",
		"POSITION",
		"PIVOT_POINT",
		"ZPOSITION",
		"ROTATION",
		"SCALE",
		"BG_COLOR",
		"OPACITY",
		"CORNER_RADIUS",
		"BORDER_WIDTH",
		"BORDER_COLOR",
		"SHADOW_RADIUS",
		"SHADOW_COLOR",
		"SHADOW_OPACITY",
		"SHADOW_OFFSET",
		"TEXTURE_REGION",
		"BACKFACE_TEXTURE_REGION",
		"LIGHT_RADIUS",
		"LIGHT_COLOR",
		"LIGHT_OPACITY",
		"LIGHT_ANGLE",
		"LIGHT_OFFSET",
		"LIGHT_POWER",
		"TEXTURE_SCALEFIT_REGION",
		"ROTATION_FROM",
		"SWIPE_RATIO", // 2012-06-21 masterkeaton27@gmail.com
		"DEFORM_RATIO",
		"RTE_RATIO",
		"MODEL_POSITION",
		"MODEL_ROTATION",
		"MODEL_SCALE",
        "LIGHT_DIRECTION",
        "MORPHING_TIME",
		"PROPERTY_ANI_COUNT"
	};

	const SVIChar* SVIPropertyAnimation::getPropertyTypeString(PropertyAnimationType type) {
		return mPropertyAnimationTypeString[type];
	}

	SVIPropertyAnimation::SVIPropertyAnimation(SVIGLSurface* saGLSurface): SVIAnimation(saGLSurface){ 
		mPropertyAnimationType = NONE; 
		mFromValue = NULL;
		mToValue = NULL;
	}

	SVIPropertyAnimation::SVIPropertyAnimation(PropertyAnimationType type, SVIGLSurface* saGLSurface): SVIAnimation(saGLSurface) {
		mPropertyAnimationType = type;
		initData();
	}

	SVIPropertyAnimation::~SVIPropertyAnimation() {
		deInitData();
		mPropertyAnimationType = NONE;
	}

	void SVIPropertyAnimation::updateInfo(SVIUInt time) {
		if( getStatus() == IDLE ) {

			initData();
			startAnimation(time);
			if( mListener != NULL )
				mListener->onAnimationStart(this);
		}

		if( getStatus() == ANIMATING ) {

			mTimeRatio = mInterpolator->getTimeInterpolation(time);
			if( mListener != NULL ) {
				SVIUInt currentCount = mInterpolator->getCurrentCount();
				// currentCount is elapsedTime / duration. So current means current animation index.
				// and RepeatCount means total count of playing animation.
				// Default RepeatCount is 1, because animation must play once.
				if( mRepeatCount > 1 && mCurrentCount != currentCount && mRepeatCount != currentCount ) {
					//LOGE("Current repeat count %d, Interpolator repeatCount %d", mCurrentCount, currentCount);
					mListener->onAnimationRepeat(this);
				}
				mCurrentCount = currentCount;
			}
		}
	}

	void SVIPropertyAnimation::initData() {
		switch(mPropertyAnimationType) {
			SETTING_PROPERTY_VALUE(REGION, SVIRect);
			SETTING_PROPERTY_VALUE(BOUND, SVIRect);
			SETTING_PROPERTY_VALUE(POSITION, SVIPoint);
			SETTING_PROPERTY_VALUE(PIVOT_POINT, SVIPoint);
			SETTING_PROPERTY_VALUE(ZPOSITION, SVIFloat);
			SETTING_PROPERTY_VALUE(ROTATION, SVIVector4);
			SETTING_PROPERTY_VALUE(SCALE, SVIVector3);
			SETTING_PROPERTY_VALUE(BG_COLOR, SVIColor);
			SETTING_PROPERTY_VALUE(OPACITY, SVIFloat);
			SETTING_PROPERTY_VALUE(CORNER_RADIUS,SVIFloat);
			SETTING_PROPERTY_VALUE(BORDER_WIDTH, SVIFloat);
			SETTING_PROPERTY_VALUE(BORDER_COLOR, SVIColor);
			SETTING_PROPERTY_VALUE(SHADOW_OPACITY, SVIFloat);
			SETTING_PROPERTY_VALUE(SHADOW_RADIUS, SVIFloat);
			SETTING_PROPERTY_VALUE(SHADOW_COLOR, SVIColor);
			SETTING_PROPERTY_VALUE(SHADOW_OFFSET, SVIPoint);
			SETTING_PROPERTY_VALUE(LIGHT_OPACITY, SVIFloat);
			SETTING_PROPERTY_VALUE(LIGHT_RADIUS, SVIFloat);
			SETTING_PROPERTY_VALUE(LIGHT_COLOR, SVIColor);
			SETTING_PROPERTY_VALUE(LIGHT_OFFSET, SVIPoint);
			SETTING_PROPERTY_VALUE(LIGHT_ANGLE, SVIFloat);
			SETTING_PROPERTY_VALUE(LIGHT_POWER, SVIFloat);			
			SETTING_PROPERTY_VALUE(TEXTURE_REGION, SVIRect);
			SETTING_PROPERTY_VALUE(BACKFACE_TEXTURE_REGION, SVIRect);
			SETTING_PROPERTY_VALUE(TEXTURE_SCALEFIT_REGION, SVIRect);	
			SETTING_PROPERTY_VALUE(SWIPE_RATIO, SVIVector3);
			SETTING_PROPERTY_VALUE(DEFORM_RATIO, SVIVector3);
			SETTING_PROPERTY_VALUE(RTE_RATIO, SVIVector3);
			SETTING_PROPERTY_VALUE(MODEL_POSITION, SVIVector3);
			SETTING_PROPERTY_VALUE(MODEL_ROTATION, SVIVector4);
			SETTING_PROPERTY_VALUE(MODEL_SCALE, SVIVector3);
            SETTING_PROPERTY_VALUE(LIGHT_DIRECTION, SVIVector4);
			SETTING_PROPERTY_VALUE(VIEW_DIRECTION, SVIVector4);
            SETTING_PROPERTY_VALUE(MORPHING_TIME, SVIFloat);

        default:
            break;
		}

	}

	void SVIPropertyAnimation::deInitData() {
		switch(mPropertyAnimationType) {
			FREE_PROPERTY_VALUE(REGION, SVIRect);
			FREE_PROPERTY_VALUE(BOUND, SVIRect);
			FREE_PROPERTY_VALUE(POSITION, SVIPoint);
			FREE_PROPERTY_VALUE(PIVOT_POINT, SVIPoint);
			FREE_PROPERTY_VALUE(ZPOSITION, SVIFloat);
			FREE_PROPERTY_VALUE(ROTATION, SVIVector4);
			FREE_PROPERTY_VALUE(SCALE, SVIVector3);
			FREE_PROPERTY_VALUE(BG_COLOR, SVIColor);
			FREE_PROPERTY_VALUE(OPACITY, SVIFloat);
			FREE_PROPERTY_VALUE(CORNER_RADIUS,SVIFloat);
			FREE_PROPERTY_VALUE(BORDER_WIDTH, SVIFloat);
			FREE_PROPERTY_VALUE(BORDER_COLOR, SVIColor);
			FREE_PROPERTY_VALUE(SHADOW_OPACITY, SVIFloat);
			FREE_PROPERTY_VALUE(SHADOW_RADIUS, SVIFloat);
			FREE_PROPERTY_VALUE(SHADOW_COLOR, SVIColor);
			FREE_PROPERTY_VALUE(SHADOW_OFFSET, SVIPoint);
			FREE_PROPERTY_VALUE(LIGHT_OPACITY, SVIFloat);
			FREE_PROPERTY_VALUE(LIGHT_RADIUS, SVIFloat);
			FREE_PROPERTY_VALUE(LIGHT_COLOR, SVIColor);
			FREE_PROPERTY_VALUE(LIGHT_OFFSET, SVIPoint);
			FREE_PROPERTY_VALUE(LIGHT_ANGLE, SVIFloat);
			FREE_PROPERTY_VALUE(LIGHT_POWER, SVIFloat);
			FREE_PROPERTY_VALUE(TEXTURE_REGION, SVIRect);
			FREE_PROPERTY_VALUE(BACKFACE_TEXTURE_REGION, SVIRect);			
			FREE_PROPERTY_VALUE(TEXTURE_SCALEFIT_REGION, SVIRect);		
			FREE_PROPERTY_VALUE(SWIPE_RATIO, SVIVector3);
			FREE_PROPERTY_VALUE(DEFORM_RATIO, SVIVector3);
			FREE_PROPERTY_VALUE(RTE_RATIO, SVIVector3);
			FREE_PROPERTY_VALUE(MODEL_POSITION, SVIVector3);
			FREE_PROPERTY_VALUE(MODEL_ROTATION, SVIVector4);
			FREE_PROPERTY_VALUE(MODEL_SCALE, SVIVector3);
            FREE_PROPERTY_VALUE(LIGHT_DIRECTION, SVIVector4);
			FREE_PROPERTY_VALUE(VIEW_DIRECTION, SVIVector4);
			FREE_PROPERTY_VALUE(MORPHING_TIME, SVIFloat);

        default:
            break;
		}

		mFromValue = NULL;
		mToValue = NULL;
	}

	void SVIPropertyAnimation::calculateAndApply(const SVIFloat& ratio) {

		DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateAndApply animation type:%d, ratio:%f", mPropertyAnimationType, ratio);
		switch(mPropertyAnimationType) {
			CALCULATE_CURRENT_VALUE(REGION, Region, ratio);
			CALCULATE_CURRENT_VALUE(BOUND, Bound, ratio);
			CALCULATE_CURRENT_VALUE(POSITION, Position, ratio);
			CALCULATE_CURRENT_VALUE(PIVOT_POINT, PivotPoint, ratio);
			CALCULATE_CURRENT_VALUE(ZPOSITION, ZPosition, ratio);
			CALCULATE_CURRENT_VALUE(ROTATION, Rotation, ratio);
			CALCULATE_CURRENT_VALUE(SCALE, Scale, ratio);
			CALCULATE_CURRENT_VALUE(BG_COLOR, BgColor, ratio);
			CALCULATE_CURRENT_VALUE(OPACITY, Opacity, ratio);
			CALCULATE_CURRENT_VALUE(CORNER_RADIUS, CornerRadius, ratio);
			CALCULATE_CURRENT_VALUE(BORDER_WIDTH, BorderWidth, ratio);
			CALCULATE_CURRENT_VALUE(BORDER_COLOR, BorderColor, ratio);
			CALCULATE_CURRENT_VALUE(SHADOW_RADIUS, ShadowRadius, ratio);
			CALCULATE_CURRENT_VALUE(SHADOW_COLOR, ShadowColor, ratio);
			CALCULATE_CURRENT_VALUE(SHADOW_OPACITY, ShadowOpacity, ratio);
			CALCULATE_CURRENT_VALUE(SHADOW_OFFSET, ShadowOffset, ratio);
			CALCULATE_CURRENT_VALUE(LIGHT_OPACITY, LightOpacity, ratio);
			CALCULATE_CURRENT_VALUE(LIGHT_RADIUS, LightRadius, ratio);
			CALCULATE_CURRENT_VALUE(LIGHT_COLOR, LightColor, ratio);
			CALCULATE_CURRENT_VALUE(LIGHT_OFFSET, LightOffset, ratio);
			CALCULATE_CURRENT_VALUE(LIGHT_ANGLE, LightAngle, ratio);
			CALCULATE_CURRENT_VALUE(LIGHT_POWER, LightPower, ratio);
			CALCULATE_CURRENT_VALUE(TEXTURE_REGION, TextureRegion, ratio);
			CALCULATE_CURRENT_VALUE(BACKFACE_TEXTURE_REGION, BackFaceTextureRegion, ratio);			
			CALCULATE_CURRENT_VALUE(TEXTURE_SCALEFIT_REGION, TextureScaleFitRegion, ratio);
			CALCULATE_CURRENT_VALUE(SWIPE_RATIO, SwipeRatio, ratio);
			CALCULATE_CURRENT_VALUE(DEFORM_RATIO, DeformRatio, ratio);
			CALCULATE_CURRENT_VALUE(RTE_RATIO, RTERatio, ratio);
			CALCULATE_CURRENT_VALUE(MODEL_POSITION, ModelPosition, ratio);
			CALCULATE_CURRENT_VALUE(MODEL_ROTATION, ModelRotation, ratio);
			CALCULATE_CURRENT_VALUE(MODEL_SCALE, ModelScale, ratio);
            CALCULATE_CURRENT_VALUE(LIGHT_DIRECTION, LightDirection, ratio);
			CALCULATE_CURRENT_VALUE(VIEW_DIRECTION, ViewDirection, ratio);
			CALCULATE_CURRENT_VALUE(MORPHING_TIME, MorphingTime, ratio);

        default:
            break;
		}
	}
	
	void SVIPropertyAnimation::calculateRegion(const SVIFloat& ratio) {
		SVIRect& from = *(SVIRect*)(mFromValue);
		SVIRect& to = *(SVIRect*)(mToValue);

		SVIFloat dx = calculateCurrentValue(from.mOrigin.mX, to.mOrigin.mX, ratio);
		SVIFloat dy = calculateCurrentValue(from.mOrigin.mY, to.mOrigin.mY, ratio);
		SVIFloat dw = calculateCurrentValue(from.mSize.mWidth, to.mSize.mWidth, ratio);
		SVIFloat dh = calculateCurrentValue(from.mSize.mHeight, to.mSize.mHeight, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateRegion slide:%x, ratio:%f, dx: %f, dy:%f, dw:%f, dh:%f", mSlide, ratio, dx, dy, dw, dh);
			mSlide->setRegion(SVIRect(dx, dy, dw, dh));
		}
	}

	void SVIPropertyAnimation::calculateBound(const SVIFloat& ratio) {
		SVIRect& from = *(SVIRect*)(mFromValue);
		SVIRect& to = *(SVIRect*)(mToValue);

		SVIFloat dx = calculateCurrentValue(from.mOrigin.mX, to.mOrigin.mX, ratio);
		SVIFloat dy = calculateCurrentValue(from.mOrigin.mY, to.mOrigin.mY, ratio);
		SVIFloat dw = calculateCurrentValue(from.mSize.mWidth, to.mSize.mWidth, ratio);
		SVIFloat dh = calculateCurrentValue(from.mSize.mHeight, to.mSize.mHeight, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateBound slide:%x, ratio:%f, dx: %f, dy:%f, dw:%f, dh:%f", mSlide, ratio, dx, dy, dw, dh);
			mSlide->setBounds(SVIRect(dx, dy, dw, dh));
		}
	}

	void SVIPropertyAnimation::calculatePosition(const SVIFloat& ratio) {
		SVIPoint& from = *(SVIPoint*)(mFromValue);
		SVIPoint& to = *(SVIPoint*)(mToValue);

		SVIPoint dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculatePosition slide:%x, ratio:%f, dx: %f, dy:%f", mSlide, ratio, dPoint.mX, dPoint.mY);
			mSlide->setPosition(dPoint);
		}
	}


	void SVIPropertyAnimation::calculateSwipeRatio(const SVIFloat& ratio){
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateSwipeRatio slide:%x, ratio:%f, dx: %f, dy:%f, dz:%f", mSlide, ratio, dPoint.x, dPoint.y, dPoint.z);
			mSlide->setSwipeRatio(dPoint);
		}
	}

	void SVIPropertyAnimation::calculateDeformRatio(const SVIFloat& ratio){
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateDeformRatio slide:%x, ratio:%f, dx: %f, dy:%f, dz:%f", mSlide, ratio, dPoint.x, dPoint.y, dPoint.z);
			mSlide->setDeformRatio(dPoint);
		}
	}

	void SVIPropertyAnimation::calculateRTERatio(const SVIFloat& ratio){
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateRTERatio slide:%x, ratio:%f, dx: %f, dy:%f, dz:%f", mSlide, ratio, dPoint.x, dPoint.y, dPoint.z);
			mSlide->setRTERatio(dPoint);
		}
	}

	void SVIPropertyAnimation::calculatePivotPoint(const SVIFloat& ratio) {
		SVIPoint& from = *(SVIPoint*)(mFromValue);
		SVIPoint& to = *(SVIPoint*)(mToValue);

		SVIPoint dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculatePivotPoint slide:%x, ratio:%f, dx: %f, dy:%f", mSlide, ratio, dPoint.mX, dPoint.mY);
			mSlide->setPivotPoint(dPoint);
		}
	}

	void SVIPropertyAnimation::calculateZPosition(const SVIFloat& ratio) {
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dPosition = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateZPosition slide:%x, ratio:%f, dZPosition", mSlide, ratio, dPosition);
			mSlide->setZPosition(dPosition);
		}
	}

	void SVIPropertyAnimation::calculateRotation(const SVIFloat& ratio) {

		SVIVector4& from = *(SVIVector4*)(mFromValue);
		SVIVector4& to = *(SVIVector4*)(mToValue);

		SVIQuaternion fromQ(from);
		SVIQuaternion toQ(to);

		SVIQuaternion current;
		current = fromQ.Slerp(toQ,ratio);

		SVIVector4 dVector = SVIVector4(current.x,current.y,current.z,current.w);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateRotation slide:%x, ratio:%f, x:%f, y:%f, z:%f, w:%f", mSlide, ratio, current.x,current.y,current.z,current.w);
			mSlide->setRotation(dVector);
		}		
	}

	void SVIPropertyAnimation::calculateScale(const SVIFloat& ratio) {
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIFloat dx = calculateCurrentValue(from.xyz[0], to.xyz[0], ratio);
		SVIFloat dy = calculateCurrentValue(from.xyz[1], to.xyz[1], ratio);
		SVIFloat dz = calculateCurrentValue(from.xyz[2], to.xyz[2], ratio);
		
		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateScale slide:%x, ratio:%f, dx:%f, dy:%f, dz:%f", mSlide, ratio, 
				dx, dy, dz);
			mSlide->setScale(SVIVector3(dx, dy, dz));
		}
	}

	void SVIPropertyAnimation::calculateBgColor(const SVIFloat& ratio) {
		SVIColor& from = *(SVIColor*)(mFromValue);
		SVIColor& to = *(SVIColor*)(mToValue);

		SVIColor dColor = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateBgColor slide:%x, ratio:%f, dR:%f, dG:%f, dB:%f, dA:%f", mSlide, ratio, dColor.mR, dColor.mG, dColor.mB, dColor.mA);
			mSlide->setBgColor(dColor);
		}
	}

	void SVIPropertyAnimation::calculateOpacity(const SVIFloat& ratio){
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dA = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateOpacity slide:%x, ratio:%f, dA: %f", mSlide, ratio, dA);
			mSlide->setOpacity(dA);
		}

	}

	void SVIPropertyAnimation::calculateCornerRadius(const SVIFloat& ratio) {
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dRadius = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateCornerRadius slide:%x, ratio:%f, dRadius: %f", mSlide, ratio, dRadius);
			mSlide->setCornerRadius(dRadius);
		}
	}

	void SVIPropertyAnimation::calculateBorderWidth(const SVIFloat& ratio) {
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dWidth = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateBorderWidth slide:%x, ratio:%f, dWidth: %f", mSlide, ratio, dWidth);
			mSlide->setBorderWidth(dWidth);
		}
	}

	void SVIPropertyAnimation::calculateBorderColor(const SVIFloat& ratio) {
		SVIColor& from = *(SVIColor*)(mFromValue);
		SVIColor& to = *(SVIColor*)(mToValue);

		SVIColor dColor = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateBorderColor slide:%x, ratio:%f, dR:%f, dG:%f, dB:%f, dA:%f", mSlide, ratio, dColor.mR, dColor.mG, dColor.mB, dColor.mA);
			mSlide->setBorderColor(dColor);
		}
	}

	void SVIPropertyAnimation::calculateShadowOpacity(const SVIFloat& ratio) {
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dA = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateShadowOpacity slide:%x, ratio:%f, dA: %f", mSlide, ratio, dA);
			mSlide->setShadowOpacity(dA);
		}
	}

	void SVIPropertyAnimation::calculateShadowRadius(const SVIFloat& ratio) {
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dRadius = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateShadowRadius slide:%x, ratio:%f, dRadius: %f", mSlide, ratio, dRadius);
			mSlide->setShadowRadius(dRadius);
		}
	}

	void SVIPropertyAnimation::calculateShadowColor(const SVIFloat& ratio) {
		SVIColor& from = *(SVIColor*)(mFromValue);
		SVIColor& to = *(SVIColor*)(mToValue);

		SVIColor dColor = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateShadowColor slide:%x, ratio:%f, dR:%f, dG:%f, dB:%f, dA:%f", mSlide, ratio, dColor.mR, dColor.mG, dColor.mB, dColor.mA);
			mSlide->setShadowColor(dColor);
		}
	}

	void SVIPropertyAnimation::calculateShadowOffset(const SVIFloat& ratio) {
		SVIPoint& from = *(SVIPoint*)(mFromValue);
		SVIPoint& to = *(SVIPoint*)(mToValue);

		SVIPoint dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateShadowOffset slide:%x, ratio:%f, dx: %f, dy:%f", mSlide, ratio, dPoint.mX, dPoint.mY);
			mSlide->setShadowOffset(dPoint);
		}
	}

	void SVIPropertyAnimation::calculateLightOpacity(const SVIFloat& ratio){
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dA = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::setLightOpacity slide:%x, ratio:%f, dRadius: %f", mSlide, ratio, dA);
			mSlide->setLightOpacity(dA);
		}
	}
	void SVIPropertyAnimation::calculateLightRadius(const SVIFloat& ratio){
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dRadius = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::setLightRadius slide:%x, ratio:%f, dRadius: %f", mSlide, ratio, dRadius);
			mSlide->setLightRadius(dRadius);
		}
	}
	void SVIPropertyAnimation::calculateLightColor(const SVIFloat& ratio){
		SVIColor& from = *(SVIColor*)(mFromValue);
		SVIColor& to = *(SVIColor*)(mToValue);

		SVIColor dColor = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateLightColor slide:%x, ratio:%f, dR:%f, dG:%f, dB:%f, dA:%f", mSlide, ratio, dColor.mR, dColor.mG, dColor.mB, dColor.mA);
			mSlide->setLightColor(dColor);
		}
	}
	void SVIPropertyAnimation::calculateLightOffset(const SVIFloat& ratio){
		SVIPoint& from = *(SVIPoint*)(mFromValue);
		SVIPoint& to = *(SVIPoint*)(mToValue);
		SVIPoint dPoint = calculateCurrentValue(from, to, ratio);
		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateLightOffset slide:%x, ratio:%f, dx: %f, dy:%f", mSlide, ratio, dPoint.mX, dPoint.mY);
			mSlide->setLightOffset(dPoint);
		}
	}
	void SVIPropertyAnimation::calculateLightAngle(const SVIFloat& ratio){
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dTheta = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::setLightAngle slide:%x, ratio:%f, dRadius: %f", mSlide, ratio, dTheta);
			mSlide->setLightAngle(dTheta);
		}
	}

	void SVIPropertyAnimation::calculateLightPower(const SVIFloat& ratio){
		SVIFloat& from = *(SVIFloat*)(mFromValue);
		SVIFloat& to = *(SVIFloat*)(mToValue);

		SVIFloat dTheta = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::setLightPower slide:%x, ratio:%f, dRadius: %f", mSlide, ratio, dTheta);
			mSlide->setLightPower(dTheta);
		}
	}

	void SVIPropertyAnimation::calculateBackFaceTextureRegion(const SVIFloat& ratio){
		SVIRect& from = *(SVIRect*)(mFromValue);
		SVIRect& to = *(SVIRect*)(mToValue);

		SVIFloat dx = calculateCurrentValue(from.mOrigin.mX, to.mOrigin.mX, ratio);
		SVIFloat dy = calculateCurrentValue(from.mOrigin.mY, to.mOrigin.mY, ratio);
		SVIFloat dw = calculateCurrentValue(from.mSize.mWidth, to.mSize.mWidth, ratio);
		SVIFloat dh = calculateCurrentValue(from.mSize.mHeight, to.mSize.mHeight, ratio);

		if( mSlide != NULL ) {
			//DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateTextureRegion slide:%x, ratio:%f, dx: %f, dy:%f, dw:%f, dh:%f", mSlide, ratio, dx, dy, dw, dh);
			mSlide->setBackFaceTextureRegion(SVIRect(dx, dy, dw, dh));
		}
	}

	void SVIPropertyAnimation::calculateTextureRegion(const SVIFloat& ratio){
		SVIRect& from = *(SVIRect*)(mFromValue);
		SVIRect& to = *(SVIRect*)(mToValue);

		SVIFloat dx = calculateCurrentValue(from.mOrigin.mX, to.mOrigin.mX, ratio);
		SVIFloat dy = calculateCurrentValue(from.mOrigin.mY, to.mOrigin.mY, ratio);
		SVIFloat dw = calculateCurrentValue(from.mSize.mWidth, to.mSize.mWidth, ratio);
		SVIFloat dh = calculateCurrentValue(from.mSize.mHeight, to.mSize.mHeight, ratio);

		if( mSlide != NULL ) {
			//DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateTextureRegion slide:%x, ratio:%f, dx: %f, dy:%f, dw:%f, dh:%f", mSlide, ratio, dx, dy, dw, dh);
			mSlide->setTextureRegion(SVIRect(dx, dy, dw, dh));
		}
	}

	void SVIPropertyAnimation::calculateTextureScaleFitRegion(const SVIFloat& ratio){
		SVIRect& from = *(SVIRect*)(mFromValue);
		SVIRect& to = *(SVIRect*)(mToValue);

		SVIFloat dx = calculateCurrentValue(from.mOrigin.mX, to.mOrigin.mX, ratio);
		SVIFloat dy = calculateCurrentValue(from.mOrigin.mY, to.mOrigin.mY, ratio);
		SVIFloat dw = calculateCurrentValue(from.mSize.mWidth, to.mSize.mWidth, ratio);
		SVIFloat dh = calculateCurrentValue(from.mSize.mHeight, to.mSize.mHeight, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateTextureRegion slide:%x, ratio:%f, dx: %f, dy:%f, dw:%f, dh:%f", mSlide, ratio, dx, dy, dw, dh);
			mSlide->setTextureScaleFitRegion(SVIRect(dx, dy, dw, dh));
		}
	}


	void SVIPropertyAnimation::calculateModelPosition(const SVIFloat& ratio){
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateModelPosition slide:%x, ratio:%f, dx: %f, dy:%f, dz:%f", mSlide, ratio, dPoint.x, dPoint.y, dPoint.z);
			mSlide->setModelPosition(dPoint);
		}
	}


	void SVIPropertyAnimation::calculateModelRotation(const SVIFloat& ratio) {

		SVIVector4& from = *(SVIVector4*)(mFromValue);
		SVIVector4& to = *(SVIVector4*)(mToValue);

		SVIQuaternion fromQ(from);
		SVIQuaternion toQ(to);

		SVIQuaternion current;
		current = fromQ.Slerp(toQ,ratio);

		SVIVector4 dVector = SVIVector4(current.x,current.y,current.z,current.w);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateModelRotation slide:%x, ratio:%f, x:%f, y:%f, z:%f, w:%f", mSlide, ratio, current.x,current.y,current.z,current.w);
			mSlide->setModelRotation(dVector);
		}		
	}


	void SVIPropertyAnimation::calculateModelScale(const SVIFloat& ratio){
		SVIVector3& from = *(SVIVector3*)(mFromValue);
		SVIVector3& to = *(SVIVector3*)(mToValue);

		SVIVector3 dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateModelScale slide:%x, ratio:%f, dx: %f, dy:%f, dz:%f", mSlide, ratio, dPoint.x, dPoint.y, dPoint.z);
			mSlide->setModelScale(dPoint);
		}
	}

    void SVIPropertyAnimation::calculateLightDirection(const SVIFloat &ratio){

        SVIVector4& from = *(SVIVector4*)(mFromValue);
        SVIVector4& to = *(SVIVector4*)(mToValue);

        SVIVector4 dPoint = calculateCurrentValue(from, to, ratio);

        if( mSlide != NULL ) {
            DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateLightDirection slide:%x, ratio:%f, dx: %f, dy:%f, dz:%f, dz:%f", mSlide, ratio, dPoint.x, dPoint.y, dPoint.z, dPoint.w);
            mSlide->setLightDirection(dPoint);
        }
    }

	void SVIPropertyAnimation::calculateViewDirection(const SVIFloat &ratio){

		SVIVector4& from = *(SVIVector4*)(mFromValue);
		SVIVector4& to = *(SVIVector4*)(mToValue);

		SVIVector4 dPoint = calculateCurrentValue(from, to, ratio);

		if( mSlide != NULL ) {
			DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateViewDirection slide:%x, ratio:%f, dx: %f, dy:%f, dz:%f, dz:%f", mSlide, ratio, dPoint.x, dPoint.y, dPoint.z, dPoint.w);
			mSlide->setViewDirection(dPoint);
		}
	}

    void SVIPropertyAnimation::calculateMorphingTime(const SVIFloat& ratio) {
        SVIFloat& from = *(SVIFloat*)(mFromValue);
        SVIFloat& to = *(SVIFloat*)(mToValue);

        SVIFloat dRadius = calculateCurrentValue(from, to, ratio);

        if( mSlide != NULL ) {
            DEBUG_CHECK_LOGI("SVIPropertyAnimation::calculateMorphingTime slide:%x, ratio:%f, dRadius: %f", mSlide, ratio, dRadius);
            mSlide->setMorphingTime(dRadius);
        }
    }


	void SVIPropertyAnimation::copy(const SVIPropertyAnimation* rhs) {
		SVIAnimation::copy(rhs);

		mPropertyAnimationType = rhs->mPropertyAnimationType;
		mFrom = rhs->mFrom;
		mTo = rhs->mTo;
		deInitData();
		initData();
	}
	
}
