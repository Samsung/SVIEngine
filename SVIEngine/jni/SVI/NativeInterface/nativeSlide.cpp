#include "nativeSlide.h"
#include "../Animation/SVIAnimation.h"
#include "../Animation/SVIAnimationSet.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVIKeyFrameAnimation.h"
#include "../SVICores.h"
#include "../Slide/SVISlide.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../Slide/SVIProjectionSlide.h"
#include "../Slide/SVIRenderPartManager.h"
#include "../Render/SVI3DScene.h"
#include "../Render/SVISceneNode.h"
#include "../Render/SVIModelManager.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Particle/SVIParticleEffectManager.h"

#include "nativeSlide.h"
#include "../GLSurface/SVIGLSurface.h"
using namespace SVI;

static const SVIBool DEBUG = SVIFALSE;



JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetRootSlideSVISlide(JNIEnv * env, jclass jobject, jint slide) {
/*
#if USE_MESSVIGE_QUEUE
	SVIMessage msg;
	msg.mSlide = NULL;
	msg.mParam1 = (SVIInt)slide;
	msg.mMessage = SVIMessageHandler::SM_SET_ROOT_SLIDE;
	
	SVIMessageHandler::getInstance()->sendMessage(msg);
#else
	SVIRenderPartManager::getInstance()->setRootSlide((SVISlide*)slide);
#endif
*/
	SVIMutex::SVIAutolock slideLock(mLockAddingSlides);
	SVISlide* rootSlide = (SVISlide*)slide;
	if(rootSlide != NULL){
		rootSlide->getSVIGLSurface()->getRenderPartManager()->setRootSlide(rootSlide);//Multi-Instance-Support
	}
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeDisplaySlideTreesSVISlide(JNIEnv * env, jclass jobject, jint slide) {
	LOGI("SVISlideJNILib_displaySlideTrees : slide=%d", slide);
	((SVISlide*)slide)->displaySlideTrees();
}

JNIEXPORT jint JNICALL Java_com_github_sviengine_slide_SVISlide_nativeInitializeSVISlide
 	(JNIEnv * env, jclass thisz, jint surfaceNativeHandle, jint type, jint parent, jfloat x, jfloat y, jfloat width, jfloat height, jfloatArray colorData, jobject jbitmap) {

	SVIMutex::SVIAutolock slideLock(mLockAddingSlides);

	SVIGLSurface*surface = (SVIGLSurface*)surfaceNativeHandle;
	
	SVISlide* slide = NULL;
	SVIImage* newImage = NULL;
	SVIColor color;
	SVIFloat* data = env->GetFloatArrayElements(colorData, NULL);
	
	color.mR = data[0];
	color.mG = data[1];
	color.mB = data[2];
	color.mA = data[3];
	env->ReleaseFloatArrayElements(colorData, data, 0);

	slide = new SVISlide(surface, (SVIInt)type);
	if (slide == NULL) {
		LOGE("Error creating SVISlide");
		return -1;
	}

	if(jbitmap != NULL) {
		void* addr = NULL;
		AndroidBitmapInfo info;
		AndroidBitmap_getInfo(env, jbitmap, &info);
		if( info.format == ANDROID_BITMAP_FORMAT_RGB_565  ||
			info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
			info.format == ANDROID_BITMAP_FORMAT_RGBA_4444  ||
			info.format == ANDROID_BITMAP_FORMAT_A_8) {
			AndroidBitmap_lockPixels(env, jbitmap, &addr);
			if( addr != NULL ) {

				newImage = new SVIImage();

				newImage->mWidth = info.width;
				newImage->mHeight = info.height;
				newImage->mStride = info.stride;

				if( info.format == ANDROID_BITMAP_FORMAT_RGB_565 ) {
					newImage->mImageFormat = SVIImage::RGB_565;
				}else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
					newImage->mImageFormat = SVIImage::RGBA_8888;
				}else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_4444 ) {
					newImage->mImageFormat = SVIImage::RGBA_4444;
				}else if( info.format == ANDROID_BITMAP_FORMAT_A_8 ) {
					newImage->mImageFormat = SVIImage::A_8;
				}

				newImage->mBits = (SVIByte*)addr;

				AndroidBitmap_unlockPixels(env, jbitmap);
			}
		}
	}
	
	if ( SVIFALSE == slide->initialize( NULL, SVIRect(x, y, width, height), color, newImage) ) {
		LOGE("Error SVISlide INITIALIZE");
		return -1;
	}

	SVISlide* parentSlide = (SVISlide*)parent;
	if( parentSlide != NULL )
		parentSlide->addModelSlide(slide);

	return (jint)slide;
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeDestroySVISlide
	(JNIEnv * env, jclass jobject, jint slideHandle) {
	SVISlide* slide = (SVISlide*)slideHandle;

	if( slide != NULL )  {
		slide->destroySlide();
	}
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeInitRegionSVISlide
  	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat width, jfloat height) {
	((SVISlide*)slide)->initRegion(SVIRect(x, y, width, height));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeInitRotationSVISlide
  (JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z, jfloat w) {
	((SVISlide*)slide)->initRotation(SVIVector4((SVIFloat)x, (SVIFloat)y, (SVIFloat)z, (SVIFloat)w));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeInitScaleSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z, jfloat w) {
	((SVISlide*)slide)->initScale(SVIVector4((SVIFloat)x, (SVIFloat)y, (SVIFloat)z, (SVIFloat)w));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeInitOpacitySVISlide
  	(JNIEnv * env, jclass jobject, jint slide, jfloat opacity) {
	((SVISlide*)slide)->initOpacity(opacity);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeInitBackgroundColorSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat r, jfloat g, jfloat b, jfloat a) {
		SVIColor bgColor(r, g, b, a);
		((SVISlide*)slide)->initBgColor(bgColor);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeInitZPositionSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat zPosition) {
		((SVISlide*)slide)->initZPosition(zPosition);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetRegionSVISlide
  	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat width, jfloat height) {
	((SVISlide*)slide)->setRegion(SVIRect(x, y, width, height));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBoundsSVISlide
 	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat width, jfloat height) {
	((SVISlide*)slide)->setBounds(SVIRect(x, y, width, height));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetPositionSVISlide
  	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y) {
	((SVIBaseSlide*)slide)->setPosition(SVIPoint(x, y));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetPivotPointSVISlide
 	 (JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y) {
	((SVISlide*)slide)->setPivotPoint(SVIPoint(x, y));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetZPositionSVISlide
  (JNIEnv * env, jclass jobject, jint slide, jfloat position) {
	((SVISlide*)slide)->setZPosition(position);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetRotationSVISlide
  (JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z, jfloat w) {
	((SVISlide*)slide)->setRotation(SVIVector4((SVIFloat)x, (SVIFloat)y, (SVIFloat)z, (SVIFloat)w));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetSwipeRatioSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z) {
		((SVISlide*)slide)->setSwipeRatio(SVIVector3((SVIFloat)x, (SVIFloat)y, (SVIFloat)z));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetDeformRatioSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z) {
		((SVISlide*)slide)->setDeformRatio(SVIVector3((SVIFloat)x, (SVIFloat)y, (SVIFloat)z));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetRTERatioSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z) {
		((SVISlide*)slide)->setRTERatio(SVIVector3((SVIFloat)x, (SVIFloat)y, (SVIFloat)z));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetScaleSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z) {
	((SVISlide*)slide)->setScale(SVIVector3((SVIFloat)x, (SVIFloat)y, (SVIFloat)z));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBackgroundColorSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVISlide*)slide)->setBgColor(SVIColor(r, g, b, a));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetOpacitySVISlide
  	(JNIEnv * env, jclass jobject, jint slide, jfloat opacity) {
	((SVISlide*)slide)->setOpacity(opacity);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetCornerRadiusSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat cornerRadius) {
	((SVISlide*)slide)->setCornerRadius(cornerRadius);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBorderWidthSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat borderWidth) {
	((SVISlide*)slide)->setBorderWidth(borderWidth);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBorderColorSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVISlide*)slide)->setBorderColor(SVIColor(r, g, b, a));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetShadowRadiusSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat shadowRadius) {
	((SVISlide*)slide)->setShadowRadius(shadowRadius);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetShadowColorSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat r, jfloat g, jfloat b, jfloat a) {
	((SVISlide*)slide)->setShadowColor(SVIColor(r, g, b, a));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetShadowOpacitySVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat shadowOpacity) {
		((SVISlide*)slide)->setShadowOpacity(shadowOpacity);

}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetShadowOffsetSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y) {
	((SVISlide*)slide)->setShadowOffset(SVIPoint(x, y));
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightAngleSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat lightAngle) {
		((SVISlide*)slide)->setLightAngle(lightAngle);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightRadiusSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat lightRadius) {
		((SVISlide*)slide)->setLightRadius(lightRadius);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightOpacitySVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat lightOpacity) {
		((SVISlide*)slide)->setLightOpacity(lightOpacity);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightPowerSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat lightPower) {
		((SVISlide*)slide)->setLightPower(lightPower);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightOffsetSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y) {
		((SVISlide*)slide)->setLightOffset(SVIPoint(x, y));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightColorSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat r, jfloat g, jfloat b, jfloat a) {
		((SVISlide*)slide)->setLightColor(SVIColor(r, g, b, a));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightDirectionSVISlide
    (JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z, jfloat w) {
        ((SVISlide*)slide)->setLightDirection(SVIVector4(x, y, z, w));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetViewDirectionSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z, jfloat w) {
		((SVISlide*)slide)->setViewDirection(SVIVector4(x, y, z, w));
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetLightTypeSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jint type) {
		((SVISlide*)slide)->setLightType((SVIInt)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetScaleToFitTypeSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jint type) {
		((SVISlide*)slide)->setImageScaleType((SlideImageScaleType)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetTextureRegionSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat width, jfloat height) {
		((SVISlide*)slide)->setTextureRegion(SVIRect(x, y, width, height));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBackFaceTextureRegionSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat width, jfloat height) {
		((SVISlide*)slide)->setBackFaceTextureRegion(SVIRect(x, y, width, height));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetTextureScaleToFitRegionSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat width, jfloat height) {
		((SVISlide*)slide)->setImageScaleType(MATRIX);
		((SVISlide*)slide)->setTextureScaleFitRegion(SVIRect(x, y, width, height));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetRotationFromSVISlide
  (JNIEnv * env, jclass jobject, jint slide, jfloat x, jfloat y, jfloat z, jfloat w, jfloat x2, jfloat y2, jfloat z2, jfloat w2) {
	((SVISlide*)slide)->setRotationFrom(SVIVector4((SVIFloat)x, (SVIFloat)y, (SVIFloat)z, (SVIFloat)w),
													SVIVector4((SVIFloat)x2, (SVIFloat)y2, (SVIFloat)z2, (SVIFloat)w2));

}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetClipSubSlidesSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jboolean clipSubSlide) {
	((SVISlide*)slide)->setClipSubSlide(clipSubSlide);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetHiddenSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jboolean hidden) {
	((SVISlide*)slide)->setHidden(hidden);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetZOrderTypeSVISlide
  	(JNIEnv * env, jclass jobject, jint slide, jint type) {
	((SVISlide*)slide)->setZOrderType((SlideZOrderType)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetSwipeTypeSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jint type) {
		((SVISlide*)slide)->setSwipeType((SlideSwipeType)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetDeformTypeSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jint type) {
		((SVISlide*)slide)->setDeformType((SlideDeformType)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetMorphingTypeSVISlide
    (JNIEnv * env, jclass jobject, jint slide, jint type) {
        ((SVISlide*)slide)->setMorphingType((SlideMorphingType)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetMorphingRadiusSVISlide
    (JNIEnv * env, jclass jobject, jint slide, jfloat radius) {
        ((SVISlide*)slide)->setMorphingRadius(radius);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetMorphingVertexCountSVISlide
    (JNIEnv * env, jclass jobject, jint slide, jint vertexCount) {

        ((SVISlide*)slide)->setMorphingVertexCount(vertexCount);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetMorphingColorSVISlide
    (JNIEnv * env, jclass jobject, jint slide, jfloat r, jfloat g, jfloat b, jfloat a) {
        ((SVISlide*)slide)->setMorphingColor(SVIColor(r, g, b, a));
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetFilterTypeSVISlide
    (JNIEnv * env, jclass jobject, jint slide, jint type) {
        ((SVISlide*)slide)->setFilterType((SlideFilterType)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetDeformDirectionSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jint type) {
		((SVISlide*)slide)->setDeformDirection((SlideDeformDirection)type);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetOrthogonalSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jboolean orthogonal) {
	((SVISlide*)slide)->setOrthogonal(orthogonal);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetUseNormalMapSVISlide
    (JNIEnv * env, jclass jobject, jint slide, jboolean useNormalMap) {
        ((SVISlide*)slide)->setUseNormalMap(useNormalMap);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetCustomRTESVISlide
	(JNIEnv * env, jclass jobject, jint slide, jboolean customRTE) {
		((SVISlide*)slide)->setCustomRTE(customRTE);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetHoldOpacitySVISlide
	(JNIEnv * env, jclass jobject, jint slide, jboolean holdOpacity) {
		((SVISlide*)slide)->setHoldOpacity(holdOpacity);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetHoldScaleSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jboolean holdScale) {
		((SVISlide*)slide)->setHoldScale(holdScale);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetCaptureSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jboolean capture) {
		((SVISlide*)slide)->setCapture(capture);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBlendTypeSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jint blendType) {
		((SVISlide*)slide)->setBlendType((SlideBlendType)blendType);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetRTEffectTypeSVISlide
	(JNIEnv * env, jclass jobject, jint slide, jint rtEffectType) {
		((SVISlide*)slide)->setRTEffectType((SlideRTEffectType)rtEffectType);
}

//2011.12.09 jongchae.moon
JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetAntiAliasing
	(JNIEnv * env, jclass jobject, jint slide, jboolean enableAA) {
		((SVISlide*)slide)->setAntiAliasing(enableAA);
}
//


JNIEXPORT jfloatArray JNICALL Java_com_github_sviengine_slide_SVISlide_nativeGetRegionSVISlide
	(JNIEnv * env, jclass jobject, jint slide) {

	jfloatArray jarr;
	jarr = env->NewFloatArray(4);

	SVIRect rect = ((SVISlide*)slide)->getRegion();
//	SVIFloat data[4] = {rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth, rect.mSize.mHeight};

	SVIFloat* data;
	data = env->GetFloatArrayElements(jarr, NULL);
	data[0] = rect.mOrigin.mX;
	data[1] = rect.mOrigin.mY;
	data[2] = rect.mSize.mWidth;
	data[3] = rect.mSize.mHeight;

	env->SetFloatArrayRegion(jarr, 0, 4, data);
	env->ReleaseFloatArrayElements(jarr, data, 0);
	
	return jarr;
}



JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeAddSlideIndexSVISlide
	(JNIEnv* env, jclass jobject, jint parentSlide, jint childSlide, jint index) {	
	((SVISlide*)parentSlide)->addModelSlide((SVISlide*)childSlide, index);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeAddSlideSVISlide
	(JNIEnv* env, jclass jobject, jint parentSlide, jint childSlide) {	
	SVIMutex::SVIAutolock slideLock(mLockAddingSlides);
	((SVISlide*)parentSlide)->addModelSlide((SVISlide*)childSlide);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeRemoveSlideSVISlide
	(JNIEnv* env, jclass jobject, jint parentSlide, jint removeSlide) {
	((SVISlide*)parentSlide)->removeModelSlide((SVISlide*)removeSlide);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeRemoveAllSlidesSVISlide
	(JNIEnv* env, jclass jobject, jint viewGroupSlide) {
	((SVISlide*)viewGroupSlide)->removeAllSlides();
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeRemoveSlideAtSVISlide
	(JNIEnv* env, jclass jobject, jint viewGroupSlide, jint index) {
	((SVISlide*)viewGroupSlide)->removeSlideAt(index);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeRemoveSlidesSVISlide
	(JNIEnv* env, jclass jobject, jint viewGroupSlide, jint start, jint count) {
	((SVISlide*)viewGroupSlide)->removeSlides(start, count);
}
  	
JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBitmapSVISlide
	(JNIEnv * env, jclass thisz, jint slide, jobject jbitmap) {

		SVIImage* newImage = NULL;

		if(jbitmap != NULL) {
			void* addr = NULL;
			AndroidBitmapInfo info;
			AndroidBitmap_getInfo(env, jbitmap, &info);


            if (info.width == 0 || info.height == 0){
                LOGE("wrong bitmap Size W(%d), H(%d)",info.width, info.height);
                return;
            }

			if( info.format == ANDROID_BITMAP_FORMAT_RGB_565  ||
				info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
				info.format == ANDROID_BITMAP_FORMAT_RGBA_4444  ||
				info.format == ANDROID_BITMAP_FORMAT_A_8) {
					AndroidBitmap_lockPixels(env, jbitmap, &addr);
					if( addr != NULL ) {

						newImage = new SVIImage();
						newImage->mWidth = info.width;
						newImage->mHeight = info.height;
						newImage->mStride = info.stride;

						if( info.format == ANDROID_BITMAP_FORMAT_RGB_565 ) {
							newImage->mImageFormat = SVIImage::RGB_565;
						}
						else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
							newImage->mImageFormat = SVIImage::RGBA_8888;
						}else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_4444 ) {
							newImage->mImageFormat = SVIImage::RGBA_4444;
						}else if( info.format == ANDROID_BITMAP_FORMAT_A_8 ) {
							newImage->mImageFormat = SVIImage::A_8;
						}

						newImage->mBits = (SVIByte*)addr;

						AndroidBitmap_unlockPixels(env, jbitmap);
					}
			}
		}

		((SVISlide*)slide)->setImage(newImage);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetImageSVISlide
	(JNIEnv * env, jclass thisz, jint slideHandle, jint imageHandle) {

		SVISlide *slide = (SVISlide*) slideHandle;
		SVIImage *image = (SVIImage *)imageHandle;

		if( slide != NULL )
			slide->setImage(image);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetNormalImageSVISlide
    (JNIEnv * env, jclass thisz, jint slideHandle, jint imageHandle) {

        SVISlide *slide = (SVISlide*) slideHandle;
        SVIImage *image = (SVIImage *)imageHandle;

        if( slide != NULL )
            slide->setNormalImage(image);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetSpecularImageSVISlide
    (JNIEnv * env, jclass thisz, jint slideHandle, jint imageHandle) {

        SVISlide *slide = (SVISlide*) slideHandle;
        SVIImage *image = (SVIImage *)imageHandle;

        if( slide != NULL )
            slide->setSpecularImage(image);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBackFaceImageSVISlide
	(JNIEnv * env, jclass thisz, jint slideHandle, jint imageHandle) {

		SVISlide *slide = (SVISlide*) slideHandle;
		SVIImage *image = (SVIImage *)imageHandle;

		if( slide != NULL )
			slide->setBackFaceImage(image);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetTesselationPointListSVISlide
    (JNIEnv * env, jclass thisz, jint slideHandle, jobjectArray pointList) {

        SVISlide *slide = (SVISlide*) slideHandle;

        if( slide != NULL)
        {
            jfieldID fid;

            std::vector<SVIVector2> tempPointList;
            jint count = env->GetArrayLength(pointList);

            if (count > 500) {
                LOGI("TesselationPointList: too much point list(%d)", count);
            }

            for(int i = 0; i < count; ++i)
            {
                jobject vector2Object = env->GetObjectArrayElement(pointList, i);
                SVIVector2 point;
                jclass vector2Class = env->GetObjectClass(vector2Object);
                fid = env->GetFieldID(vector2Class, "mX", "F");
                point.x = env->GetFloatField(vector2Object, fid);
                fid = env->GetFieldID(vector2Class, "mY", "F");
                point.y = env->GetFloatField(vector2Object, fid);
                tempPointList.push_back(point);

                env->DeleteLocalRef( vector2Class );
                env->DeleteLocalRef( vector2Object );
            }

            slide->setTesselationPointList(tempPointList);
        }
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBackFaceImageFlipSVISlide
	(JNIEnv * env, jclass thisz, jint slideHandle, jint isFlip) {

		SVISlide *slide = (SVISlide*) slideHandle;

		SVIBool flag = SVITRUE;
		if(isFlip == 0)
			flag = SVIFALSE;

		if( slide != NULL )
			slide->setBackFaceImageFlip(flag);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetBackFaceImageClearSVISlide
	(JNIEnv * env, jclass thisz, jint slideHandle, jint isClear) {

		SVISlide *slide = (SVISlide*) slideHandle;

		SVIBool flag = SVITRUE;
		if(isClear == 0)
			flag = SVIFALSE;

		if( slide != NULL )
			slide->setBackFaceImageClear(flag);
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeStartAnimationSVISlide
  	(JNIEnv * env, jclass jobject, jint slide, jint animation) {
	((SVISlide*)slide)->startAnimation((SVI::SVIAnimation*)animation);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeStopAnimationSVISlide
  	(JNIEnv * env, jclass jobject, jint slide) {
	((SVISlide*)slide)->stopAnimation();
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeRunExplicitAnimationSVISlide
  	(JNIEnv * env, jclass jobject, jint slideHandle, jint animation) {
	((SVISlide*)slideHandle)->startAnimation((SVI::SVIAnimation*)animation);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetImplicitAnimationSVISlide
  	(JNIEnv * env, jclass jobject, jint slideHandle, jint type, jint intepolator, jint duration, jint repeatCnt, jint offset, jint autoReverse) {

	((SVISlide*)slideHandle)->setImplicitAnimation(type, intepolator, duration, repeatCnt, offset, autoReverse);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetExplicitAnimationSVISlide
  	(JNIEnv * env, jclass jobject, jint slideHandle, jint animation) {
	((SVISlide*)slideHandle)->setExplicitAnimation((SVI::SVIAnimation*)animation);
}

JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetProxySVISlide
  	(JNIEnv * env, jclass jobject, jint slideHandle, jint proxy, jint listenerID) {
	((SVISlide*)slideHandle)->setImplicitAnimationListenerInfo((SVIInt)proxy, (SVIUInt) listenerID);
}

JNIEXPORT jint JNICALL Java_com_github_sviengine_slide_SVISlide_nativeIsSlideAnimating
	(JNIEnv * env, jclass jobject, jint slideHandle) {
	return ((SVISlide*)slideHandle)->isAnimating() ? 1 : 0;
}

/*
 * Class:     com_github_sviengine_slide_SVISlide
 * Method:    nativeAddPathPoint
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeAddPathPoint
	(JNIEnv * env, jclass jobject, jint slideHandle, jfloat x, jfloat y , jfloat z){
    ((SVISlide*)slideHandle)->addPathPoint(SVIVector3(x,y,z));
}


/*
 * Class:     com_github_sviengine_slide_SVISlide
 * Method:    nativeClearPathPoints
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeClearPathPoints
    (JNIEnv * env, jclass jobject, jint slideHandle){
    ((SVISlide*)slideHandle)->clearPathPoints();
}


JNIEXPORT jint JNICALL Java_com_github_sviengine_slide_SVISlide_nativeLoad3dModelFileSVISlide
  (JNIEnv *env, jclass clazz, jint parentSlide, jstring fileName) {
#if 0	// DEPRECATED
	const char *utf8;

	utf8 = env->GetStringUTFChars(fileName, NULL);

	LOGI("nativeSlide: loadModelFileFromAssetManager(%s)", utf8);
	SVISceneNode *sceneNode = SVIModelManager::getInstance()->loadModelFileFromAssetManager(utf8, SVITRUE);

	if(sceneNode == NULL)
		return -1;

	// add scene-node
	SVIProjectionSlide *projectionSlide = ((SVISlide*)parentSlide)->getBeamedSlide()->getProjectionSlide();

	SVISize size = ((SVISlide*)parentSlide)->getBeamedSlide()->getRegion().getSize();
	SVI3DScene * pLocal3DScene = new SVI3DScene(size.mWidth, size.mHeight);
	projectionSlide->set3DScene(pLocal3DScene);
	pLocal3DScene->setParent((SVISlide*)parentSlide);

	/// The current allocation of the slide is only one 3d model node.
	/*if(!projectionSlide->has3DScene()) {
		LOGE("3D: Create New Scene");
		SVISize size = ((SVISlide*)parentSlide)->getBeamedSlide()->getRegion().getSize();
		SVI3DScene * pLocal3DScene = new SVI3DScene(size.mWidth, size.mHeight);
		projectionSlide->set3DScene(pLocal3DScene);
		pLocal3DScene->setParent((SVISlide*)parentSlide);
	}*/

	projectionSlide->get3DScene()->addSceneNode(sceneNode);
	
	return (jint)sceneNode;
#endif 
	return 1;
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeRemoveSceneNodeSVISlide
	(JNIEnv *env, jclass clazz, jint parentSlide) {

#if 0	// DEPRECATED	
	// remove scene-node
	SVIProjectionSlide *projectionSlide = ((SVISlide*)parentSlide)->getBeamedSlide()->getProjectionSlide();

	if(!projectionSlide->has3DScene()) {
		LOGE("not exist scene node");
		return;
	}
	else {
		projectionSlide->set3DScene(NULL);
	}
#endif	
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetModelPositionSVISlide
  	(JNIEnv * env, jclass jclazz, jint slide, jfloat x, jfloat y, jfloat z) {
	((SVIBaseSlide*)slide)->setModelPosition(SVIVector3(x, y, z));
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetModelRotationSVISlide
  	(JNIEnv * env, jclass jclazz, jint slide, jfloat x, jfloat y, jfloat z) {
	((SVISlide*)slide)->setModelRotation(SVIVector4((SVIFloat)x, (SVIFloat)y, (SVIFloat)z, (SVIFloat)0.0f));
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeSetModelScaleSVISlide
  	(JNIEnv * env, jclass jclazz, jint slide, jfloat x, jfloat y, jfloat z) {
	((SVIBaseSlide*)slide)->setModelScale(SVIVector3(x, y, z));
}


JNIEXPORT void JNICALL Java_com_github_sviengine_slide_SVISlide_nativeAddParticleEffectSVISlide
(JNIEnv *env, jclass clazz, jint slideHandle, jint effectHandle) {
	LOGI("nativeAddParticleEffectSVISlide - start method !!!");
	if(effectHandle != -1) {
		SVIParticleEffect* newEffect = ( (SVIParticleEffect*)effectHandle )->clone();
		((SVISlide*)slideHandle)->addParticleEffect( newEffect );
	}
}
