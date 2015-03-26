#include "SVIParticleEffectManager.h"
#include "../Render/SVITextureManager.h"
#include "../Animation/SVIParticleKeyFrameAnimation.h"

namespace SVI {

SVIParticleEffectManager::SVIParticleEffectManager(SVIGLSurface* saGLSurface){
		mSVIGLSurface=saGLSurface;
	}


SVIBool SVIParticleEffectManager::isExistTexture(std::string textureFileName) {
	ParticleTextureNameList::iterator iter = mTextureFileNames.begin();

	for(iter; iter != mTextureFileNames.end(); ++iter) {
		if( (*iter) == textureFileName ) {
			return SVITRUE;
		}
	}

	return SVIFALSE;
}


void SVIParticleEffectManager::addTexture(std::string textureFileName, SVIImage *image) {
	if(isExistTexture(textureFileName) == SVIFALSE) {
		mTextureFileNames.push_back(textureFileName);

		mSVIGLSurface->getTextureManager()->add((const SVIPChar)textureFileName.c_str(), image);//Multi-Instance-Support
	}
}


void SVIParticleEffectManager::update(SVIUInt drawTime) {
	// TODO : update processing
	mDrawTime = drawTime;
}


SVIUInt SVIParticleEffectManager::getDrawTime() {
	return mDrawTime;
}

//2012-06-23 masterkeaton27@gmail.com
void SVIParticleEffectManager::finalize(){
	mTextureFileNames.clear();
}


//SVIParticleEffectInfo* SVIParticleEffectManager::parsingParticleEffectInfo(JNIEnv *env, jobject particleEffectInfo) {
//	jfieldID fid;
//	jclass effectClass, infoClass, vectorClass, colorClass;
//	jobject obj;
//	
//	SVIParticleEffectInfo* effectInfo = new SVIParticleEffectInfo();
//
//	effectClass = env->GetObjectClass(particleEffectInfo);
//
//	// get mDuration
//	fid = env->GetFieldID(effectClass, "mDuration", "I");
//	effectInfo->mDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mScaleKeyFrameDuration
//	fid = env->GetFieldID(effectClass, "mScaleKeyFrameDuration", "I");
//	effectInfo->mScaleKeyFrameDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mScaleKeyFrameInterpolaterType
//	fid = env->GetFieldID(effectClass, "mScaleKeyFrameInterpolaterType", "I");
//	effectInfo->mScaleKeyFrameInterpolaterType = env->GetIntField(particleEffectInfo, fid);
//	// get mColorKeyFrameDuration
//	fid = env->GetFieldID(effectClass, "mColorKeyFrameDuration", "I");
//	effectInfo->mColorKeyFrameDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mColorKeyFrameInterpolaterType
//	fid = env->GetFieldID(effectClass, "mColorKeyFrameInterpolaterType", "I");
//	effectInfo->mColorKeyFrameInterpolaterType = env->GetIntField(particleEffectInfo, fid);
//	// get mPositionKeyFrameDuration
//	fid = env->GetFieldID(effectClass, "mPositionKeyFrameDuration", "I");
//	effectInfo->mPositionKeyFrameDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mPositionKeyFrameInterpolaterType
//	fid = env->GetFieldID(effectClass, "mPositionKeyFrameInterpolaterType", "I");
//	effectInfo->mPositionKeyFrameInterpolaterType = env->GetIntField(particleEffectInfo, fid);
//
//	// get mPosition
//	fid = env->GetFieldID(effectClass, "mPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//	obj = env->GetObjectField(particleEffectInfo, fid);
//	vectorClass = env->GetObjectClass(obj);
//	fid = env->GetFieldID(vectorClass, "mX", "F");
//	effectInfo->mPosition.x = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mY", "F");
//	effectInfo->mPosition.y = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mZ", "F");
//	effectInfo->mPosition.z = env->GetFloatField(obj, fid);
//
//	// get mScale
//	fid = env->GetFieldID(effectClass, "mScale", "Lcom/github/sviengine/basetype/SVIVector3;");
//	obj = env->GetObjectField(particleEffectInfo, fid);
//	vectorClass = env->GetObjectClass(obj);
//	fid = env->GetFieldID(vectorClass, "mX", "F");
//	effectInfo->mScale.x = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mY", "F");
//	effectInfo->mScale.y = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mZ", "F");
//	effectInfo->mScale.z = env->GetFloatField(obj, fid);
//
//	// get mColor
//	fid = env->GetFieldID(effectClass, "mColor", "Lcom/github/sviengine/basetype/SVIColor;");
//	obj = env->GetObjectField(particleEffectInfo, fid);
//	colorClass = env->GetObjectClass(obj);
//	fid = env->GetFieldID(colorClass, "mR", "F");
//	effectInfo->mColor.mR = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(colorClass, "mG", "F");
//	effectInfo->mColor.mG = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(colorClass, "mB", "F");
//	effectInfo->mColor.mB = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(colorClass, "mA", "F");
//	effectInfo->mColor.mA = env->GetFloatField(obj, fid);
//
//
//	jobjectArray keyList;
//	jint count = 0;
//
//	// get mScaleKeyFrameInfoListObject
//	fid = env->GetFieldID(effectClass, "mScaleKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticleEffectInfo$ScaleKeyFrameInfo;");
//	keyList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	if(keyList != NULL) {
//		count = env->GetArrayLength(keyList);
//	}
//		
//	for(SVIInt scaleIdx = 0; scaleIdx < count; ++scaleIdx) {
//		SVIParticleEffectInfo::ScaleKeyFrameInfo* scaleInfo = new SVIParticleEffectInfo::ScaleKeyFrameInfo();
//		jobject scaleKeyInfoObject = env->GetObjectArrayElement(keyList, scaleIdx);
//		jclass scaleKeyClass = env->GetObjectClass(scaleKeyInfoObject);
//		fid = env->GetFieldID(scaleKeyClass, "mKeyTime", "F");
//		scaleInfo->mKeyTime = env->GetFloatField(scaleKeyInfoObject, fid);
//		fid = env->GetFieldID(scaleKeyClass, "mScale", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(scaleKeyInfoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		scaleInfo->mScale.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		scaleInfo->mScale.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		scaleInfo->mScale.z = env->GetFloatField(obj, fid);
//
//		effectInfo->mScaleKeyFrameInfoList.push_back(scaleInfo);
//	}
//
//	// get mColorKeyFrameInfoListObject
//	fid = env->GetFieldID(effectClass, "mColorKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticleEffectInfo$ColorKeyFrameInfo;");
//	keyList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	count = 0;
//	if(keyList != NULL) {
//		count = env->GetArrayLength(keyList);
//	}
//		
//	for(SVIInt colorIdx = 0; colorIdx < count; ++colorIdx) {
//		SVIParticleEffectInfo::ColorKeyFrameInfo* colorInfo = new SVIParticleEffectInfo::ColorKeyFrameInfo();
//		jobject colorKeyInfoObject = env->GetObjectArrayElement(keyList, colorIdx);
//		jclass colorKeyClass = env->GetObjectClass(colorKeyInfoObject);
//		fid = env->GetFieldID(colorKeyClass, "mKeyTime", "F");
//		colorInfo->mKeyTime = env->GetFloatField(colorKeyInfoObject, fid);
//		fid = env->GetFieldID(colorKeyClass, "mColor", "Lcom/github/sviengine/basetype/SVIColor;");
//		obj = env->GetObjectField(colorKeyInfoObject, fid);
//		colorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(colorClass, "mR", "F");
//		colorInfo->mColor.mR = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mG", "F");
//		colorInfo->mColor.mG = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mB", "F");
//		colorInfo->mColor.mB = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mA", "F");
//		colorInfo->mColor.mA = env->GetFloatField(obj, fid);
//
//		effectInfo->mColorKeyFrameInfoList.push_back(colorInfo);
//	}
//
//	// get mPositionKeyFrameInfoListObject
//	fid = env->GetFieldID(effectClass, "mPositionKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticleEffectInfo$PositionKeyFrameInfo;");
//	keyList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	count = 0;
//	if(keyList != NULL) {
//		count = env->GetArrayLength(keyList);
//	}
//		
//	for(SVIInt positionIdx = 0; positionIdx < count; ++positionIdx) {
//		SVIParticleEffectInfo::PositionKeyFrameInfo* positionInfo = new SVIParticleEffectInfo::PositionKeyFrameInfo();
//		jobject positionKeyInfoObject = env->GetObjectArrayElement(keyList, positionIdx);
//		jclass positionKeyClass = env->GetObjectClass(positionKeyInfoObject);
//		fid = env->GetFieldID(positionKeyClass, "mKeyTime", "F");
//		positionInfo->mKeyTime = env->GetFloatField(positionKeyInfoObject, fid);
//		fid = env->GetFieldID(positionKeyClass, "mPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(positionKeyInfoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		positionInfo->mPosition.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		positionInfo->mPosition.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		positionInfo->mPosition.z = env->GetFloatField(obj, fid);
//
//		effectInfo->mPositionKeyFrameInfoList.push_back(positionInfo);
//	}
//
//
//	fid = env->GetFieldID(effectClass, "mParticlesListObject", "[Lcom/github/sviengine/particle/SVIParticlesInfo;");
//	
//	jobjectArray particlesInfoList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	jint particlesInfoCnt = env->GetArrayLength(particlesInfoList);
//
//	for(SVIInt i = 0; i < particlesInfoCnt; ++i) {
//		SVIParticlesInfo* info = new SVIParticlesInfo();
//		// get SVIParticlesInfo class
//		jobject infoObject = env->GetObjectArrayElement(particlesInfoList, i);
//		infoClass = env->GetObjectClass(infoObject);
//		// get mParticleCount
//		fid = env->GetFieldID(infoClass, "mParticleCount", "I");
//		info->mParticleCount = env->GetIntField(infoObject, fid);
//		// get mTextureFileName
//		fid = env->GetFieldID(infoClass, "mTextureFileName", "Ljava/lang/String;");
//		const char *utf8 = NULL;
//		jstring textureFileName = NULL;
//		textureFileName = (jstring)env->GetObjectField(infoObject, fid);
//		if(textureFileName == NULL) {
//			//LOGE("nativeSlide: nativeCreateParticleEffect - textureFileName = NULL!!!");
//			info->mTextureFileName = "";
//		}
//		else {
//			utf8 = env->GetStringUTFChars(textureFileName, NULL);
//			info->mTextureFileName = utf8;
//		}
//		// get mMaxDuration
//		fid = env->GetFieldID(infoClass, "mMaxDuration", "I");
//		info->mMaxDuration = env->GetIntField(infoObject, fid);
//		// get mRandomDuration
//		fid = env->GetFieldID(infoClass, "mRandomDuration", "I");
//		info->mRandomDuration = env->GetIntField(infoObject, fid);
//		// get mBlendType
//		fid = env->GetFieldID(infoClass, "mBlendType", "I");
//		info->mBlendType = env->GetIntField(infoObject, fid);
//		
//		// get mScaleKeyFrameDuration
//		fid = env->GetFieldID(infoClass, "mScaleKeyFrameDuration", "I");
//		info->mScaleKeyFrameDuration = env->GetIntField(infoObject, fid);
//		// get mScaleKeyFrameInterpolaterType
//		fid = env->GetFieldID(infoClass, "mScaleKeyFrameInterpolaterType", "I");
//		info->mScaleKeyFrameInterpolaterType = env->GetIntField(infoObject, fid);
//		// get mColorKeyFrameDuration
//		fid = env->GetFieldID(infoClass, "mColorKeyFrameDuration", "I");
//		info->mColorKeyFrameDuration = env->GetIntField(infoObject, fid);
//		// get mColorKeyFrameInterpolaterType
//		fid = env->GetFieldID(infoClass, "mColorKeyFrameInterpolaterType", "I");
//		info->mColorKeyFrameInterpolaterType = env->GetIntField(infoObject, fid);
//		// get mPositionKeyFrameDuration
//		fid = env->GetFieldID(infoClass, "mPositionKeyFrameDuration", "I");
//		info->mPositionKeyFrameDuration = env->GetIntField(infoObject, fid);
//		// get mPositionKeyFrameInterpolaterType
//		fid = env->GetFieldID(infoClass, "mPositionKeyFrameInterpolaterType", "I");
//		info->mPositionKeyFrameInterpolaterType = env->GetIntField(infoObject, fid);
//
//		// get mDefaultPosition
//		fid = env->GetFieldID(infoClass, "mDefaultPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mDefaultPosition.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mDefaultPosition.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mDefaultPosition.z = env->GetFloatField(obj, fid);
//		// get mRandomPosition
//		fid = env->GetFieldID(infoClass, "mRandomPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomPosition.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomPosition.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomPosition.z = env->GetFloatField(obj, fid);
//		// get mMaxColor
//		fid = env->GetFieldID(infoClass, "mMaxColor", "Lcom/github/sviengine/basetype/SVIColor;");
//		obj = env->GetObjectField(infoObject, fid);
//		colorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(colorClass, "mR", "F");
//		info->mMaxColor.mR = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mG", "F");
//		info->mMaxColor.mG = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mB", "F");
//		info->mMaxColor.mB = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mA", "F");
//		info->mMaxColor.mA = env->GetFloatField(obj, fid);
//		// get mRandomColor
//		fid = env->GetFieldID(infoClass, "mRandomColor", "Lcom/github/sviengine/basetype/SVIColor;");
//		obj = env->GetObjectField(infoObject, fid);
//		colorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(colorClass, "mR", "F");
//		info->mRandomColor.mR = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mG", "F");
//		info->mRandomColor.mG = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mB", "F");
//		info->mRandomColor.mB = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mA", "F");
//		info->mRandomColor.mA = env->GetFloatField(obj, fid);
//		// get mDefaultGravity
//		fid = env->GetFieldID(infoClass, "mDefaultGravity", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mDefaultGravity.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mDefaultGravity.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mDefaultGravity.z = env->GetFloatField(obj, fid);
//		// get mRandomGravity
//		fid = env->GetFieldID(infoClass, "mRandomGravity", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomGravity.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomGravity.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomGravity.z = env->GetFloatField(obj, fid);
//		// get mDefaultForce
//		fid = env->GetFieldID(infoClass, "mDefaultForce", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mDefaultForce.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mDefaultForce.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mDefaultForce.z = env->GetFloatField(obj, fid);
//		// get mRandomForce
//		fid = env->GetFieldID(infoClass, "mRandomForce", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomForce.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomForce.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomForce.z = env->GetFloatField(obj, fid);
//		// get mMaxMass
//		fid = env->GetFieldID(infoClass, "mMaxMass", "F");
//		info->mMaxMass = env->GetFloatField(infoObject, fid);
//		// get mRandomMass
//		fid = env->GetFieldID(infoClass, "mRandomMass", "F");
//		info->mRandomMass = env->GetFloatField(infoObject, fid);
//		// get mMaxParticleSize
//		fid = env->GetFieldID(infoClass, "mMaxParticleSize", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mMaxParticleSize.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mMaxParticleSize.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mMaxParticleSize.z = env->GetFloatField(obj, fid);
//		// get mRandomParticleSize
//		fid = env->GetFieldID(infoClass, "mRandomParticleSize", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomParticleSize.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomParticleSize.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomParticleSize.z = env->GetFloatField(obj, fid);
//
//		// get mScaleKeyFrameInfoListObject
//		fid = env->GetFieldID(infoClass, "mScaleKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticlesInfo$ScaleKeyFrameInfo;");
//		jobjectArray scaleKeyList = (jobjectArray)env->GetObjectField(infoObject, fid);
//		jint scaleKeyCount = 0;
//		if(scaleKeyList != NULL) {
//			scaleKeyCount = env->GetArrayLength(scaleKeyList);
//		}
//		
//		for(SVIInt scaleIdx = 0; scaleIdx < scaleKeyCount; ++scaleIdx) {
//			SVIParticlesInfo::ScaleKeyFrameInfo* scaleInfo = new SVIParticlesInfo::ScaleKeyFrameInfo();
//			jobject scaleKeyInfoObject = env->GetObjectArrayElement(scaleKeyList, scaleIdx);
//			jclass scaleKeyClass = env->GetObjectClass(scaleKeyInfoObject);
//			fid = env->GetFieldID(scaleKeyClass, "mKeyTime", "F");
//			scaleInfo->mKeyTime = env->GetFloatField(scaleKeyInfoObject, fid);
//			fid = env->GetFieldID(scaleKeyClass, "mScale", "Lcom/github/sviengine/basetype/SVIVector3;");
//			obj = env->GetObjectField(scaleKeyInfoObject, fid);
//			vectorClass = env->GetObjectClass(obj);
//			fid = env->GetFieldID(vectorClass, "mX", "F");
//			scaleInfo->mScale.x = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mY", "F");
//			scaleInfo->mScale.y = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mZ", "F");
//			scaleInfo->mScale.z = env->GetFloatField(obj, fid);
//
//			info->mScaleKeyFrameInfoList.push_back(scaleInfo);
//		}
//
//		// get mColorKeyFrameInfoListObject
//		fid = env->GetFieldID(infoClass, "mColorKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticlesInfo$ColorKeyFrameInfo;");
//		jobjectArray colorKeyList = (jobjectArray)env->GetObjectField(infoObject, fid);
//		jint colorKeyCount = 0;
//		if(colorKeyList != NULL) {
//			colorKeyCount = env->GetArrayLength(colorKeyList);
//		}
//		
//		for(SVIInt colorIdx = 0; colorIdx < colorKeyCount; ++colorIdx) {
//			SVIParticlesInfo::ColorKeyFrameInfo* colorInfo = new SVIParticlesInfo::ColorKeyFrameInfo();
//			jobject colorKeyInfoObject = env->GetObjectArrayElement(colorKeyList, colorIdx);
//			jclass colorKeyClass = env->GetObjectClass(colorKeyInfoObject);
//			fid = env->GetFieldID(colorKeyClass, "mKeyTime", "F");
//			colorInfo->mKeyTime = env->GetFloatField(colorKeyInfoObject, fid);
//			fid = env->GetFieldID(colorKeyClass, "mColor", "Lcom/github/sviengine/basetype/SVIColor;");
//			obj = env->GetObjectField(colorKeyInfoObject, fid);
//			colorClass = env->GetObjectClass(obj);
//			fid = env->GetFieldID(colorClass, "mR", "F");
//			colorInfo->mColor.mR = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(colorClass, "mG", "F");
//			colorInfo->mColor.mG = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(colorClass, "mB", "F");
//			colorInfo->mColor.mB = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(colorClass, "mA", "F");
//			colorInfo->mColor.mA = env->GetFloatField(obj, fid);
//
//			info->mColorKeyFrameInfoList.push_back(colorInfo);
//		}
//
//		// get mPositionKeyFrameInfoListObject
//		fid = env->GetFieldID(infoClass, "mPositionKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticlesInfo$PositionKeyFrameInfo;");
//		jobjectArray positionKeyList = (jobjectArray)env->GetObjectField(infoObject, fid);
//		jint positionKeyCount = 0;
//		if(positionKeyList != NULL) {
//			positionKeyCount = env->GetArrayLength(positionKeyList);
//		}
//		
//		for(SVIInt positionIdx = 0; positionIdx < positionKeyCount; ++positionIdx) {
//			SVIParticlesInfo::PositionKeyFrameInfo* positionInfo = new SVIParticlesInfo::PositionKeyFrameInfo();
//			jobject positionKeyInfoObject = env->GetObjectArrayElement(positionKeyList, positionIdx);
//			jclass positionKeyClass = env->GetObjectClass(positionKeyInfoObject);
//			fid = env->GetFieldID(positionKeyClass, "mKeyTime", "F");
//			positionInfo->mKeyTime = env->GetFloatField(positionKeyInfoObject, fid);
//			fid = env->GetFieldID(positionKeyClass, "mPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//			obj = env->GetObjectField(positionKeyInfoObject, fid);
//			vectorClass = env->GetObjectClass(obj);
//			fid = env->GetFieldID(vectorClass, "mX", "F");
//			positionInfo->mPosition.x = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mY", "F");
//			positionInfo->mPosition.y = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mZ", "F");
//			positionInfo->mPosition.z = env->GetFloatField(obj, fid);
//
//			info->mPositionKeyFrameInfoList.push_back(positionInfo);
//		}
//
//		effectInfo->mParticlesInfoList.push_back(info);
//	}
//
//	return effectInfo;
//}


//SVIKeyFrameParticleEffectInfo* SVIParticleEffectManager::parsingKeyFrameParticleEffectInfo(JNIEnv *env, jobject particleEffectInfo) {
//	jfieldID fid;
//	jclass effectClass, infoClass, vectorClass, colorClass;
//	jobject obj;
//	
//	SVIKeyFrameParticleEffectInfo* effectInfo = new SVIKeyFrameParticleEffectInfo();
//
//	effectClass = env->GetObjectClass(particleEffectInfo);
//
//	// get mDuration
//	fid = env->GetFieldID(effectClass, "mDuration", "I");
//	effectInfo->mDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mScaleKeyFrameDuration
//	fid = env->GetFieldID(effectClass, "mScaleKeyFrameDuration", "I");
//	effectInfo->mScaleKeyFrameDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mScaleKeyFrameInterpolaterType
//	fid = env->GetFieldID(effectClass, "mScaleKeyFrameInterpolaterType", "I");
//	effectInfo->mScaleKeyFrameInterpolaterType = env->GetIntField(particleEffectInfo, fid);
//	// get mColorKeyFrameDuration
//	fid = env->GetFieldID(effectClass, "mColorKeyFrameDuration", "I");
//	effectInfo->mColorKeyFrameDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mColorKeyFrameInterpolaterType
//	fid = env->GetFieldID(effectClass, "mColorKeyFrameInterpolaterType", "I");
//	effectInfo->mColorKeyFrameInterpolaterType = env->GetIntField(particleEffectInfo, fid);
//	// get mPositionKeyFrameDuration
//	fid = env->GetFieldID(effectClass, "mPositionKeyFrameDuration", "I");
//	effectInfo->mPositionKeyFrameDuration = env->GetIntField(particleEffectInfo, fid);
//	// get mPositionKeyFrameInterpolaterType
//	fid = env->GetFieldID(effectClass, "mPositionKeyFrameInterpolaterType", "I");
//	effectInfo->mPositionKeyFrameInterpolaterType = env->GetIntField(particleEffectInfo, fid);
//
//	// get mPosition
//	fid = env->GetFieldID(effectClass, "mPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//	obj = env->GetObjectField(particleEffectInfo, fid);
//	vectorClass = env->GetObjectClass(obj);
//	fid = env->GetFieldID(vectorClass, "mX", "F");
//	effectInfo->mPosition.x = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mY", "F");
//	effectInfo->mPosition.y = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mZ", "F");
//	effectInfo->mPosition.z = env->GetFloatField(obj, fid);
//
//	// get mScale
//	fid = env->GetFieldID(effectClass, "mScale", "Lcom/github/sviengine/basetype/SVIVector3;");
//	obj = env->GetObjectField(particleEffectInfo, fid);
//	vectorClass = env->GetObjectClass(obj);
//	fid = env->GetFieldID(vectorClass, "mX", "F");
//	effectInfo->mScale.x = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mY", "F");
//	effectInfo->mScale.y = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(vectorClass, "mZ", "F");
//	effectInfo->mScale.z = env->GetFloatField(obj, fid);
//
//	// get mColor
//	fid = env->GetFieldID(effectClass, "mColor", "Lcom/github/sviengine/basetype/SVIColor;");
//	obj = env->GetObjectField(particleEffectInfo, fid);
//	colorClass = env->GetObjectClass(obj);
//	fid = env->GetFieldID(colorClass, "mR", "F");
//	effectInfo->mColor.mR = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(colorClass, "mG", "F");
//	effectInfo->mColor.mG = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(colorClass, "mB", "F");
//	effectInfo->mColor.mB = env->GetFloatField(obj, fid);
//	fid = env->GetFieldID(colorClass, "mA", "F");
//	effectInfo->mColor.mA = env->GetFloatField(obj, fid);
//
//
//	jobjectArray keyList;
//	jint count = 0;
//
//	// get mScaleKeyFrameInfoListObject
//	fid = env->GetFieldID(effectClass, "mScaleKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticleEffectInfo$ScaleKeyFrameInfo;");
//	keyList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	if(keyList != NULL) {
//		count = env->GetArrayLength(keyList);
//	}
//		
//	for(SVIInt scaleIdx = 0; scaleIdx < count; ++scaleIdx) {
//		SVIParticleEffectInfo::ScaleKeyFrameInfo* scaleInfo = new SVIParticleEffectInfo::ScaleKeyFrameInfo();
//		jobject scaleKeyInfoObject = env->GetObjectArrayElement(keyList, scaleIdx);
//		jclass scaleKeyClass = env->GetObjectClass(scaleKeyInfoObject);
//		fid = env->GetFieldID(scaleKeyClass, "mKeyTime", "F");
//		scaleInfo->mKeyTime = env->GetFloatField(scaleKeyInfoObject, fid);
//		fid = env->GetFieldID(scaleKeyClass, "mScale", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(scaleKeyInfoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		scaleInfo->mScale.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		scaleInfo->mScale.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		scaleInfo->mScale.z = env->GetFloatField(obj, fid);
//
//		effectInfo->mScaleKeyFrameInfoList.push_back(scaleInfo);
//	}
//
//	// get mColorKeyFrameInfoListObject
//	fid = env->GetFieldID(effectClass, "mColorKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticleEffectInfo$ColorKeyFrameInfo;");
//	keyList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	count = 0;
//	if(keyList != NULL) {
//		count = env->GetArrayLength(keyList);
//	}
//		
//	for(SVIInt colorIdx = 0; colorIdx < count; ++colorIdx) {
//		SVIParticleEffectInfo::ColorKeyFrameInfo* colorInfo = new SVIParticleEffectInfo::ColorKeyFrameInfo();
//		jobject colorKeyInfoObject = env->GetObjectArrayElement(keyList, colorIdx);
//		jclass colorKeyClass = env->GetObjectClass(colorKeyInfoObject);
//		fid = env->GetFieldID(colorKeyClass, "mKeyTime", "F");
//		colorInfo->mKeyTime = env->GetFloatField(colorKeyInfoObject, fid);
//		fid = env->GetFieldID(colorKeyClass, "mColor", "Lcom/github/sviengine/basetype/SVIColor;");
//		obj = env->GetObjectField(colorKeyInfoObject, fid);
//		colorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(colorClass, "mR", "F");
//		colorInfo->mColor.mR = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mG", "F");
//		colorInfo->mColor.mG = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mB", "F");
//		colorInfo->mColor.mB = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mA", "F");
//		colorInfo->mColor.mA = env->GetFloatField(obj, fid);
//
//		effectInfo->mColorKeyFrameInfoList.push_back(colorInfo);
//	}
//
//	// get mPositionKeyFrameInfoListObject
//	fid = env->GetFieldID(effectClass, "mPositionKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticleEffectInfo$PositionKeyFrameInfo;");
//	keyList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	count = 0;
//	if(keyList != NULL) {
//		count = env->GetArrayLength(keyList);
//	}
//		
//	for(SVIInt positionIdx = 0; positionIdx < count; ++positionIdx) {
//		SVIParticleEffectInfo::PositionKeyFrameInfo* positionInfo = new SVIParticleEffectInfo::PositionKeyFrameInfo();
//		jobject positionKeyInfoObject = env->GetObjectArrayElement(keyList, positionIdx);
//		jclass positionKeyClass = env->GetObjectClass(positionKeyInfoObject);
//		fid = env->GetFieldID(positionKeyClass, "mKeyTime", "F");
//		positionInfo->mKeyTime = env->GetFloatField(positionKeyInfoObject, fid);
//		fid = env->GetFieldID(positionKeyClass, "mPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(positionKeyInfoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		positionInfo->mPosition.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		positionInfo->mPosition.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		positionInfo->mPosition.z = env->GetFloatField(obj, fid);
//
//		effectInfo->mPositionKeyFrameInfoList.push_back(positionInfo);
//	}
//
//
//	fid = env->GetFieldID(effectClass, "mKeyFrameParticlesListObject", "[Lcom/github/sviengine/particle/SVIKeyFrameParticlesInfo;");
//	
//	jobjectArray particlesInfoList = (jobjectArray)env->GetObjectField(particleEffectInfo, fid);
//	jint particlesInfoCnt = env->GetArrayLength(particlesInfoList);
//
//	for(SVIInt i = 0; i < particlesInfoCnt; ++i) {
//		SVIKeyFrameParticlesInfo* info = new SVIKeyFrameParticlesInfo();
//		// get SVIParticlesInfo class
//		jobject infoObject = env->GetObjectArrayElement(particlesInfoList, i);
//		infoClass = env->GetObjectClass(infoObject);
//		// get mKeyTime
//		fid = env->GetFieldID(infoClass, "mKeyTime", "F");
//		info->mKeyTime = env->GetFloatField(infoObject, fid);
//		// get mParticleCount
//		fid = env->GetFieldID(infoClass, "mParticleCount", "I");
//		info->mParticleCount = env->GetIntField(infoObject, fid);
//		// get mTextureFileName
//		fid = env->GetFieldID(infoClass, "mTextureFileName", "Ljava/lang/String;");
//		const char *utf8 = NULL;
//		jstring textureFileName = NULL;
//		textureFileName = (jstring)env->GetObjectField(infoObject, fid);
//		if(textureFileName == NULL) {
//			//LOGE("nativeSlide: nativeCreateParticleEffect - textureFileName = NULL!!!");
//			info->mTextureFileName = "";
//		}
//		else {
//			utf8 = env->GetStringUTFChars(textureFileName, NULL);
//			info->mTextureFileName = utf8;
//		}
//		// get mMaxDuration
//		fid = env->GetFieldID(infoClass, "mMaxDuration", "I");
//		info->mMaxDuration = env->GetIntField(infoObject, fid);
//		// get mRandomDuration
//		fid = env->GetFieldID(infoClass, "mRandomDuration", "I");
//		info->mRandomDuration = env->GetIntField(infoObject, fid);
//		// get mBlendType
//		fid = env->GetFieldID(infoClass, "mBlendType", "I");
//		info->mBlendType = env->GetIntField(infoObject, fid);
//		
//		// get mScaleKeyFrameDuration
//		fid = env->GetFieldID(infoClass, "mScaleKeyFrameDuration", "I");
//		info->mScaleKeyFrameDuration = env->GetIntField(infoObject, fid);
//		// get mScaleKeyFrameInterpolaterType
//		fid = env->GetFieldID(infoClass, "mScaleKeyFrameInterpolaterType", "I");
//		info->mScaleKeyFrameInterpolaterType = env->GetIntField(infoObject, fid);
//		// get mColorKeyFrameDuration
//		fid = env->GetFieldID(infoClass, "mColorKeyFrameDuration", "I");
//		info->mColorKeyFrameDuration = env->GetIntField(infoObject, fid);
//		// get mColorKeyFrameInterpolaterType
//		fid = env->GetFieldID(infoClass, "mColorKeyFrameInterpolaterType", "I");
//		info->mColorKeyFrameInterpolaterType = env->GetIntField(infoObject, fid);
//		// get mPositionKeyFrameDuration
//		fid = env->GetFieldID(infoClass, "mPositionKeyFrameDuration", "I");
//		info->mPositionKeyFrameDuration = env->GetIntField(infoObject, fid);
//		// get mPositionKeyFrameInterpolaterType
//		fid = env->GetFieldID(infoClass, "mPositionKeyFrameInterpolaterType", "I");
//		info->mPositionKeyFrameInterpolaterType = env->GetIntField(infoObject, fid);
//
//		// get mDefaultPosition
//		fid = env->GetFieldID(infoClass, "mDefaultPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mDefaultPosition.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mDefaultPosition.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mDefaultPosition.z = env->GetFloatField(obj, fid);
//		// get mRandomPosition
//		fid = env->GetFieldID(infoClass, "mRandomPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomPosition.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomPosition.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomPosition.z = env->GetFloatField(obj, fid);
//		// get mMaxColor
//		fid = env->GetFieldID(infoClass, "mMaxColor", "Lcom/github/sviengine/basetype/SVIColor;");
//		obj = env->GetObjectField(infoObject, fid);
//		colorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(colorClass, "mR", "F");
//		info->mMaxColor.mR = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mG", "F");
//		info->mMaxColor.mG = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mB", "F");
//		info->mMaxColor.mB = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mA", "F");
//		info->mMaxColor.mA = env->GetFloatField(obj, fid);
//		// get mRandomColor
//		fid = env->GetFieldID(infoClass, "mRandomColor", "Lcom/github/sviengine/basetype/SVIColor;");
//		obj = env->GetObjectField(infoObject, fid);
//		colorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(colorClass, "mR", "F");
//		info->mRandomColor.mR = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mG", "F");
//		info->mRandomColor.mG = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mB", "F");
//		info->mRandomColor.mB = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(colorClass, "mA", "F");
//		info->mRandomColor.mA = env->GetFloatField(obj, fid);
//		// get mDefaultGravity
//		fid = env->GetFieldID(infoClass, "mDefaultGravity", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mDefaultGravity.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mDefaultGravity.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mDefaultGravity.z = env->GetFloatField(obj, fid);
//		// get mRandomGravity
//		fid = env->GetFieldID(infoClass, "mRandomGravity", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomGravity.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomGravity.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomGravity.z = env->GetFloatField(obj, fid);
//		// get mDefaultForce
//		fid = env->GetFieldID(infoClass, "mDefaultForce", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mDefaultForce.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mDefaultForce.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mDefaultForce.z = env->GetFloatField(obj, fid);
//		// get mRandomForce
//		fid = env->GetFieldID(infoClass, "mRandomForce", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomForce.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomForce.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomForce.z = env->GetFloatField(obj, fid);
//		// get mMaxMass
//		fid = env->GetFieldID(infoClass, "mMaxMass", "F");
//		info->mMaxMass = env->GetFloatField(infoObject, fid);
//		// get mRandomMass
//		fid = env->GetFieldID(infoClass, "mRandomMass", "F");
//		info->mRandomMass = env->GetFloatField(infoObject, fid);
//		// get mMaxParticleSize
//		fid = env->GetFieldID(infoClass, "mMaxParticleSize", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mMaxParticleSize.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mMaxParticleSize.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mMaxParticleSize.z = env->GetFloatField(obj, fid);
//		// get mRandomParticleSize
//		fid = env->GetFieldID(infoClass, "mRandomParticleSize", "Lcom/github/sviengine/basetype/SVIVector3;");
//		obj = env->GetObjectField(infoObject, fid);
//		vectorClass = env->GetObjectClass(obj);
//		fid = env->GetFieldID(vectorClass, "mX", "F");
//		info->mRandomParticleSize.x = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mY", "F");
//		info->mRandomParticleSize.y = env->GetFloatField(obj, fid);
//		fid = env->GetFieldID(vectorClass, "mZ", "F");
//		info->mRandomParticleSize.z = env->GetFloatField(obj, fid);
//
//		// get mScaleKeyFrameInfoListObject
//		fid = env->GetFieldID(infoClass, "mScaleKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticlesInfo$ScaleKeyFrameInfo;");
//		jobjectArray scaleKeyList = (jobjectArray)env->GetObjectField(infoObject, fid);
//		jint scaleKeyCount = 0;
//		if(scaleKeyList != NULL) {
//			scaleKeyCount = env->GetArrayLength(scaleKeyList);
//		}
//		
//		for(SVIInt scaleIdx = 0; scaleIdx < scaleKeyCount; ++scaleIdx) {
//			SVIParticlesInfo::ScaleKeyFrameInfo* scaleInfo = new SVIParticlesInfo::ScaleKeyFrameInfo();
//			jobject scaleKeyInfoObject = env->GetObjectArrayElement(scaleKeyList, scaleIdx);
//			jclass scaleKeyClass = env->GetObjectClass(scaleKeyInfoObject);
//			fid = env->GetFieldID(scaleKeyClass, "mKeyTime", "F");
//			scaleInfo->mKeyTime = env->GetFloatField(scaleKeyInfoObject, fid);
//			fid = env->GetFieldID(scaleKeyClass, "mScale", "Lcom/github/sviengine/basetype/SVIVector3;");
//			obj = env->GetObjectField(scaleKeyInfoObject, fid);
//			vectorClass = env->GetObjectClass(obj);
//			fid = env->GetFieldID(vectorClass, "mX", "F");
//			scaleInfo->mScale.x = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mY", "F");
//			scaleInfo->mScale.y = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mZ", "F");
//			scaleInfo->mScale.z = env->GetFloatField(obj, fid);
//
//			info->mScaleKeyFrameInfoList.push_back(scaleInfo);
//		}
//
//		// get mColorKeyFrameInfoListObject
//		fid = env->GetFieldID(infoClass, "mColorKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticlesInfo$ColorKeyFrameInfo;");
//		jobjectArray colorKeyList = (jobjectArray)env->GetObjectField(infoObject, fid);
//		jint colorKeyCount = 0;
//		if(colorKeyList != NULL) {
//			colorKeyCount = env->GetArrayLength(colorKeyList);
//		}
//		
//		for(SVIInt colorIdx = 0; colorIdx < colorKeyCount; ++colorIdx) {
//			SVIParticlesInfo::ColorKeyFrameInfo* colorInfo = new SVIParticlesInfo::ColorKeyFrameInfo();
//			jobject colorKeyInfoObject = env->GetObjectArrayElement(colorKeyList, colorIdx);
//			jclass colorKeyClass = env->GetObjectClass(colorKeyInfoObject);
//			fid = env->GetFieldID(colorKeyClass, "mKeyTime", "F");
//			colorInfo->mKeyTime = env->GetFloatField(colorKeyInfoObject, fid);
//			fid = env->GetFieldID(colorKeyClass, "mColor", "Lcom/github/sviengine/basetype/SVIColor;");
//			obj = env->GetObjectField(colorKeyInfoObject, fid);
//			colorClass = env->GetObjectClass(obj);
//			fid = env->GetFieldID(colorClass, "mR", "F");
//			colorInfo->mColor.mR = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(colorClass, "mG", "F");
//			colorInfo->mColor.mG = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(colorClass, "mB", "F");
//			colorInfo->mColor.mB = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(colorClass, "mA", "F");
//			colorInfo->mColor.mA = env->GetFloatField(obj, fid);
//
//			info->mColorKeyFrameInfoList.push_back(colorInfo);
//		}
//
//		// get mPositionKeyFrameInfoListObject
//		fid = env->GetFieldID(infoClass, "mPositionKeyFrameInfoListObject", "[Lcom/github/sviengine/particle/SVIParticlesInfo$PositionKeyFrameInfo;");
//		jobjectArray positionKeyList = (jobjectArray)env->GetObjectField(infoObject, fid);
//		jint positionKeyCount = 0;
//		if(positionKeyList != NULL) {
//			positionKeyCount = env->GetArrayLength(positionKeyList);
//		}
//		
//		for(SVIInt positionIdx = 0; positionIdx < positionKeyCount; ++positionIdx) {
//			SVIParticlesInfo::PositionKeyFrameInfo* positionInfo = new SVIParticlesInfo::PositionKeyFrameInfo();
//			jobject positionKeyInfoObject = env->GetObjectArrayElement(positionKeyList, positionIdx);
//			jclass positionKeyClass = env->GetObjectClass(positionKeyInfoObject);
//			fid = env->GetFieldID(positionKeyClass, "mKeyTime", "F");
//			positionInfo->mKeyTime = env->GetFloatField(positionKeyInfoObject, fid);
//			fid = env->GetFieldID(positionKeyClass, "mPosition", "Lcom/github/sviengine/basetype/SVIVector3;");
//			obj = env->GetObjectField(positionKeyInfoObject, fid);
//			vectorClass = env->GetObjectClass(obj);
//			fid = env->GetFieldID(vectorClass, "mX", "F");
//			positionInfo->mPosition.x = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mY", "F");
//			positionInfo->mPosition.y = env->GetFloatField(obj, fid);
//			fid = env->GetFieldID(vectorClass, "mZ", "F");
//			positionInfo->mPosition.z = env->GetFloatField(obj, fid);
//
//			info->mPositionKeyFrameInfoList.push_back(positionInfo);
//		}
//
//		effectInfo->mParticlesInfoList.push_back(info);
//	}
//
//	return effectInfo;
//}


//SVIParticleEffect* SVIParticleEffectManager::createParticleEffectFromInfo(SVIParticleEffectInfo* info) {
//	SVIParticleEffect* particleEffect = new SVIParticleEffect();
//
//	particleEffect->setDuration(info->mDuration);
//	particleEffect->setPosition(info->mPosition);
//	particleEffect->setScale(info->mScale);
//	particleEffect->setColor(info->mColor);
//
//	// create scale key frame animation
//	if( info->mScaleKeyFrameInfoList.size() > 0 ) {
//		SVIParticleKeyFrameAnimation* keyAni = NULL;
//	
//		keyAni = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_SCALE);
//		keyAni->setInterpolator( (SVIAnimation::InterpolatorType)( info->mScaleKeyFrameInterpolaterType ) );
//			
//		keyAni->mDuration = info->mScaleKeyFrameDuration;
//			
//		if (keyAni == NULL) {
//			LOGE("Can't create particleEffect scaleKeyAnimation!");
//		}
//		else {
//			// get scale key frame
//			SVIParticleEffectInfo::ScaleKeyFrameInfoList::iterator scaleIter = info->mScaleKeyFrameInfoList.begin();
//			for(scaleIter; scaleIter != info->mScaleKeyFrameInfoList.end(); ++scaleIter) {
//				SVIFloat time = (*scaleIter)->mKeyTime;
//				keyAni->addKeyProperty( SVIParticleKeyFrameProperty(time, (*scaleIter)->mScale) );
//			}
//		}
//
//		particleEffect->setScaleKeyFrameAnimation(keyAni);
//	}
//
//	// create color key frame animation
//	if( info->mColorKeyFrameInfoList.size() > 0 ) {
//		SVIParticleKeyFrameAnimation* keyAni = NULL;
//	
//		keyAni = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_COLOR);
//		keyAni->setInterpolator( (SVIAnimation::InterpolatorType)( info->mColorKeyFrameInterpolaterType ) );
//			
//		keyAni->mDuration = info->mColorKeyFrameDuration;
//			
//		if (keyAni == NULL) {
//			LOGE("Can't create particleEffect colorKeyAnimation!");
//		}
//		else {
//			// get color key frame
//			SVIParticleEffectInfo::ColorKeyFrameInfoList::iterator colorIter = info->mColorKeyFrameInfoList.begin();
//			for(colorIter; colorIter != info->mColorKeyFrameInfoList.end(); ++colorIter) {
//				SVIFloat time = (*colorIter)->mKeyTime;
//				keyAni->addKeyProperty( SVIParticleKeyFrameProperty(time, (*colorIter)->mColor) );
//			}
//		}
//
//		particleEffect->setColorKeyFrameAnimation(keyAni);
//	}
//
//	// create position key frame animation
//	if( info->mPositionKeyFrameInfoList.size() > 0 ) {
//		SVIParticleKeyFrameAnimation* keyAni = NULL;
//	
//		keyAni = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_EFFECT_POSITION);
//		keyAni->setInterpolator( (SVIAnimation::InterpolatorType)( info->mPositionKeyFrameInterpolaterType ) );
//			
//		keyAni->mDuration = info->mPositionKeyFrameDuration;
//		
//		if (keyAni == NULL) {
//			LOGE("Can't create particleEffect positionKeyAnimation!");
//		}
//		else {
//			// get position key frame
//			SVIParticleEffectInfo::PositionKeyFrameInfoList::iterator positionIter = info->mPositionKeyFrameInfoList.begin();
//			for(positionIter; positionIter != info->mPositionKeyFrameInfoList.end(); ++positionIter) {
//				SVIFloat time = (*positionIter)->mKeyTime;
//				keyAni->addKeyProperty( SVIParticleKeyFrameProperty(time, (*positionIter)->mPosition) );
//			}
//		}
//
//		particleEffect->setPositionKeyFrameAnimation(keyAni);
//	}
//
//
//	SVIParticleEffectInfo::ParticlesInfoList::iterator iter = info->mParticlesInfoList.begin();
//
//	for(iter; iter != info->mParticlesInfoList.end(); ++iter) {
//		SVIParticles* particles = new SVIParticles();
//		particles->setTextureFileName( (*iter)->mTextureFileName );
//		particles->setParticleCount( (*iter)->mParticleCount );
//		particles->setDuration( (*iter)->mMaxDuration, (*iter)->mRandomDuration );
//		particles->setParticleSize( (*iter)->mMaxParticleSize, (*iter)->mRandomParticleSize );
//		particles->setPosition( (*iter)->mDefaultPosition, (*iter)->mRandomPosition );
//		particles->setColor( (*iter)->mMaxColor, (*iter)->mRandomColor );
//		particles->setGravity( (*iter)->mDefaultGravity, (*iter)->mRandomGravity );
//		particles->setForce( (*iter)->mDefaultForce, (*iter)->mRandomForce );
//		particles->setMass( (*iter)->mMaxMass, (*iter)->mRandomMass );
//
//		// create scale key frame animation
//		if( (*iter)->mScaleKeyFrameInfoList.size() > 0 ) {
//			SVIParticleKeyFrameAnimation* keyAni = NULL;
//	
//			keyAni = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_SCALE);
//			keyAni->setInterpolator( (SVIAnimation::InterpolatorType)( (*iter)->mScaleKeyFrameInterpolaterType ) );
//			
//			keyAni->mDuration = (*iter)->mScaleKeyFrameDuration;
//			
//			if (keyAni == NULL) {
//				LOGE("Can't create particle scaleKeyAnimation!");
//			}
//			else {
//				// get scale key frame
//				SVIParticlesInfo::ScaleKeyFrameInfoList::iterator scaleIter = (*iter)->mScaleKeyFrameInfoList.begin();
//				for(scaleIter; scaleIter != (*iter)->mScaleKeyFrameInfoList.end(); ++scaleIter) {
//					SVIFloat time = (*scaleIter)->mKeyTime;
//					keyAni->addKeyProperty( SVIParticleKeyFrameProperty(time, (*scaleIter)->mScale) );
//				}
//			}
//
//			particles->setScaleKeyFrameAnimation(keyAni);
//		}
//
//		// create color key frame animation
//		if( (*iter)->mColorKeyFrameInfoList.size() > 0 ) {
//			SVIParticleKeyFrameAnimation* keyAni = NULL;
//	
//			keyAni = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_COLOR);
//			keyAni->setInterpolator( (SVIAnimation::InterpolatorType)( (*iter)->mColorKeyFrameInterpolaterType ) );
//			
//			keyAni->mDuration = (*iter)->mColorKeyFrameDuration;
//			
//			if (keyAni == NULL) {
//				LOGE("Can't create particle colorKeyAnimation!");
//			}
//			else {
//				// get color key frame
//				SVIParticlesInfo::ColorKeyFrameInfoList::iterator colorIter = (*iter)->mColorKeyFrameInfoList.begin();
//				for(colorIter; colorIter != (*iter)->mColorKeyFrameInfoList.end(); ++colorIter) {
//					SVIFloat time = (*colorIter)->mKeyTime;
//					keyAni->addKeyProperty( SVIParticleKeyFrameProperty(time, (*colorIter)->mColor) );
//				}
//			}
//
//			particles->setColorKeyFrameAnimation(keyAni);
//		}
//
//		// create position key frame animation
//		if( (*iter)->mPositionKeyFrameInfoList.size() > 0 ) {
//			SVIParticleKeyFrameAnimation* keyAni = NULL;
//	
//			keyAni = SVIParticleKeyFrameAnimation::create(SVIParticlePropertyAnimation::PARTICLE_POSITION);
//			keyAni->setInterpolator( (SVIAnimation::InterpolatorType)( (*iter)->mPositionKeyFrameInterpolaterType ) );
//			
//			keyAni->mDuration = (*iter)->mPositionKeyFrameDuration;
//			
//			if (keyAni == NULL) {
//				LOGE("Can't create particle positionKeyAnimation!");
//			}
//			else {
//				// get position key frame
//				SVIParticlesInfo::PositionKeyFrameInfoList::iterator positionIter = (*iter)->mPositionKeyFrameInfoList.begin();
//				for(positionIter; positionIter != (*iter)->mPositionKeyFrameInfoList.end(); ++positionIter) {
//					SVIFloat time = (*positionIter)->mKeyTime;
//					keyAni->addKeyProperty( SVIParticleKeyFrameProperty(time, (*positionIter)->mPosition) );
//				}
//			}
//
//			particles->setPositionKeyFrameAnimation(keyAni);
//		}
//
//		particleEffect->addParticles(particles);
//	}
//
//	return particleEffect;
//
//	return NULL;
//}

} // end namespace SVI