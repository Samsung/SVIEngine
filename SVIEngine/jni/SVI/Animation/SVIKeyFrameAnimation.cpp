#include "SVIKeyFrameAnimation.h"
#include "./Interpolator/SVIInterpolator.h"
#include "../Slide/SVIBeamedSlide.h"

namespace SVI {
static const SVIBool DEBUG = SVIFALSE;

#define TYPE_CHECK_FOR_KEYFRAMES(type)						            \
	{														            \
		for(SVIUInt i=0; i<mKeyFrameList.size(); i++) {			        \
			if( mKeyFrameList[i].mProperty.mPropertyType != type ){	    \
				mTypeCheckSuccess = SVIFALSE;				            \
				LOGE("KeyFrameList %dth is wrong type!!!", (int)type);	\
			}												            \
		}													            \
	}														            \

	SVIKeyFrameAnimation::SVIKeyFrameAnimation(SVIGLSurface* saGLSurface):SVIPropertyAnimation(saGLSurface) {
	
		mFromKey = NULL;
		mToKey = NULL;
		mTypeChecked = SVIFALSE;
		mTypeCheckSuccess = SVIFALSE;

		mClassType = KEYFRAME;
	}

	SVIKeyFrameAnimation::SVIKeyFrameAnimation(PropertyAnimationType type, SVIGLSurface* saGLSurface):SVIPropertyAnimation(saGLSurface) {
		mFromKey = NULL;
		mToKey = NULL;
		mTypeChecked = SVIFALSE;
		mTypeCheckSuccess = SVIFALSE;
		mPropertyAnimationType = type;
		

		mClassType = KEYFRAME;
	}

	SVIKeyFrameAnimation::SVIKeyFrameAnimation(const SVIKeyFrameAnimation* rhs):SVIPropertyAnimation(rhs->mSVIGLSurface) {
		mFromKey = NULL;
		mToKey = NULL;
		mTypeChecked = SVIFALSE;
		mTypeCheckSuccess = SVIFALSE;
	}

	SVIKeyFrameAnimation::~SVIKeyFrameAnimation() {
		mKeyFrameList.clear();
	}

	SVIKeyFrameAnimation* SVIKeyFrameAnimation::create(PropertyAnimationType type, SVIGLSurface* saGLSurface) {
		SVIKeyFrameAnimation* keyFrameAnimation = new SVIKeyFrameAnimation(type, saGLSurface);
		return keyFrameAnimation;
	}

	void SVIKeyFrameAnimation::update(SVIUInt& time) {
		SVIPropertyAnimation::updateInfo(time);

		if( getStatus() == ANIMATING ) {
			if( !mTypeChecked )
				checkType();

			if( findKeyFrame() ) {
				DEBUG_CHECK_LOGI("FromKey:%f, ToKey:%f", mFromKey->mKeyTime, mToKey->mKeyTime);
				calculateAndApply();
			} else {
				LOGE("Can not find out key-frame!!!");
				return;
			}

			if( isEnd(time) ) {
				setStatus(END);
				if( !mIsContainedInAniSet && mSlide != NULL ) {
					mSlide->removeAnimation();
				}
			}
		}
	}

	void SVIKeyFrameAnimation::addKeyProperty(const SVIKeyFrameProperty& keyFrameProperty) {
		mKeyFrameList.push_back(keyFrameProperty);
	}

	SVIAnimation* SVIKeyFrameAnimation::clone() {
		SVIKeyFrameAnimation* cloneAnimation = new SVIKeyFrameAnimation(mSVIGLSurface);
		cloneAnimation->copy(this);

		return (SVIAnimation*)cloneAnimation;
	}

	SVIBool SVIKeyFrameAnimation::findKeyFrame() {

		SVIInt frameCount = mKeyFrameList.size();

		if( frameCount < 2 )
			return SVIFALSE;

		SVIKeyFrameProperty* fromKey = NULL;
		SVIKeyFrameProperty* toKey = NULL;

		if( mTimeRatio <= 0.0f ) {
			fromKey = &mKeyFrameList[0];
			toKey = &mKeyFrameList[1];
		} else if( mTimeRatio >= 1.0f ) {
			fromKey = &mKeyFrameList[0];
			toKey = &mKeyFrameList[frameCount-1];
		} else {
			for(int i=0; i<frameCount; ++i) {
				if( mKeyFrameList[i].mKeyTime <= mTimeRatio ) {
					fromKey = &mKeyFrameList[i];
					toKey = &mKeyFrameList[i+1];
				}
			}
		}

		if( mFromKey != fromKey || mToKey != toKey ) {
			mFromKey = fromKey;
			mToKey = toKey;

			mFrom = mFromKey->mProperty;
			mTo = mToKey->mProperty;

			SVIPropertyAnimation::initData();
		}

		return SVITRUE;
	}

	void SVIKeyFrameAnimation::calculateAndApply() {
		if( mFromKey == NULL || mToKey == NULL ) {
			LOGE("FromKey or ToKey is not defined!!!");
			return;
		}

		//calculate ratio between fromkey and tokey and apply newratio to PropertyAnimation
		SVIFloat gapTime = mToKey->mKeyTime - mFromKey->mKeyTime;
		SVIFloat newRatio = (mTimeRatio - mFromKey->mKeyTime) / gapTime;

		DEBUG_CHECK_LOGI("KeyFrame TimeRatio : %f, newRatio : %f", mTimeRatio, newRatio);
		SVIPropertyAnimation::calculateAndApply(newRatio);
	}

	void SVIKeyFrameAnimation::checkType() {

		if( !mTypeChecked ) {
			mTypeCheckSuccess = SVITRUE;
			switch(mPropertyAnimationType) {
			case TEXTURE_SCALEFIT_REGION:
			case BACKFACE_TEXTURE_REGION:
			case TEXTURE_REGION:
			case REGION:		//SVIRect
			case BOUND:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::RECT);
				break;
			case POSITION:		//SVIPoint
			case PIVOT_POINT:
			case LIGHT_OFFSET:
			case SHADOW_OFFSET:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::POINT);
				break;
			case ZPOSITION:		//SVIFloat
			case OPACITY:
			case LIGHT_POWER:
			case LIGHT_ANGLE:
			case LIGHT_RADIUS:
			case LIGHT_OPACITY:
			case CORNER_RADIUS:
			case BORDER_WIDTH:
			case SHADOW_RADIUS:
			case SHADOW_OPACITY:
            case MORPHING_TIME:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::FLOAT);
				break;
			case ROTATION:		//SVIVector4
			case MODEL_ROTATION:
            case LIGHT_DIRECTION:
			case VIEW_DIRECTION:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::VECTOR4);
				break;

			case SWIPE_RATIO:
			case DEFORM_RATIO:
			case RTE_RATIO:
			case SCALE:			//SVIVector3
			case MODEL_POSITION:
			case MODEL_SCALE:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::VECTOR3);
				break;

			case SHADOW_COLOR:
			case LIGHT_COLOR:
			case BG_COLOR:		//SVIColor
			case BORDER_COLOR:
				TYPE_CHECK_FOR_KEYFRAMES(SVIUnionProperty::COLOR);
				break;

            default:
                break;
			}

			mTypeChecked = SVITRUE;

			if( !mTypeCheckSuccess )
				LOGE("One of KeyFrameList is wrong type!!! please check it!!! ");
		}
	}

	void SVIKeyFrameAnimation::copy(SVIKeyFrameAnimation* rhs) {
		SVIPropertyAnimation::copy(rhs);
		mKeyFrameList = rhs->mKeyFrameList;
	}

	void SVIKeyFrameAnimation::print() {
		LOGI("AniClassType : KeyFrameAnimation, PropertyType : %d", mPropertyAnimationType);
	}

}
