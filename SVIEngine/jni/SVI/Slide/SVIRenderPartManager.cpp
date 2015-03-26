#include "SVIRenderPartManager.h"
#include "../Render/SVITextureManager.h"
#include "SVIBeamedSlide.h"
#include "../Render/SVIRenderer.h"
#include "../Render/SVIFrameRenderer.h"
#include "../Render/SVIDebugRenderer.h"
#include "../Render/SVIPostEffectRenderer.h"
#include "../NativeInterface/nativeGLSurface.h"
#include "../NativeInterface/nativeGLWallpaper.h"
#include "../NativeInterface/nativeGLExternalSurface.h"
#include "../Animation/SVIKeyFrameAnimation.h"
#include "../Animation/SVIBasicAnimation.h"
#include "../Animation/SVIAnimationSet.h"
#include "../TestBedDelegator/TestBedForAnimation.h"
#include "../TestBedDelegator/TestBedForPerformance.h"
#include "../Particle/SVIParticleEffectManager.h"
#include <time.h>


namespace SVI {
    
const static SVIBool DEBUG = SVIFALSE;

#define FRAME_COUNT_BASE_ANIMATION	0

SVIBool compareDepthForBeamedSlide(const SVIBeamedSlide* lhs,const SVIBeamedSlide* rhs){
	SVIBeamedSlide* left = (SVIBeamedSlide*)lhs;
	SVIBeamedSlide* right = (SVIBeamedSlide*)rhs;
	if (left->getProjectionSlide()->getDepthLevel() < right->getProjectionSlide()->getDepthLevel() ) 
		return SVITRUE;
	return SVIFALSE;
}

SVIBool compareAnimationList(const SVIAnimation* lhs, const SVIAnimation* rhs) {
	SVIAnimation* left = (SVIAnimation*)lhs;
	SVIAnimation* right = (SVIAnimation*)rhs;
	SVIUInt leftDepth = left->getBeamedSlide()->getProjectionSlide()->getDepthLevel();
	SVIUInt rightDepth = right->getBeamedSlide()->getProjectionSlide()->getDepthLevel();

	if( leftDepth < rightDepth )
		return SVITRUE;

	return SVIFALSE;
}

enum { TEST_ANIMATION, TEST_PERFORMANCE, TEST_COUNT };
TestBedDelegator* createTestBed(SVIInt testType, SVIInt w, SVIInt h) {
	TestBedDelegator* testBedDelegator = NULL;
	switch(testType) {
	case TEST_ANIMATION:
		testBedDelegator = new TestBedForAnimation(w, h);
		break;
	case TEST_PERFORMANCE:
		testBedDelegator = new TestBedForPerformance(w, h);
		break;
	default:
		break;
	}
	return testBedDelegator;
};

SVIRenderPartManager::SVIRenderPartManager(SVIGLSurface* saGLSurface) {//Multi-Instance-Support

	LOGI("SVI ENGINE-NATIVE-RELEASE DATE(2014/05/19)");
	LOGI("SVI ENGINE-NATIVE-BUILD TIME  (%s:%s)",__DATE__, __TIME__);
	mSVIGLSurface=saGLSurface;//Multi-Instance-Support
	mRootSlide = NULL;
	mRootBeamedSlide = NULL;
	mTestBedDelegator = NULL;
	mDeltaTime = mDrawTime = 0;
	mEnableEffect = SVIFALSE;
	mFPS = 0.0f;
	mTestBedEnable = SVIFALSE;

	mEnableLogFPS = SVIFALSE;
	mEnableShowFPS = SVIFALSE;
	mEnableAnimation = SVITRUE;
    mEnablePostEffect = SVIFALSE;
	mEnableUpdateListener = SVIFALSE;
	mAssetManager =NULL;
	mEnableTranslucent = SVIFALSE;
	mIsExternal = SVIFALSE;
	mIsWallpaper = SVIFALSE;
	mHeight =0;
	mWidth =0;
	mIsHandledSetImage = SVIFALSE;

	mElaspedDrawTimeList.reserve(60);
	mIsPaused = SVIFALSE;

	mAnimationSlideLock = new SVIMutex("AnimationSlideLock");
	mRenderPartMainLock = new SVIMutex("RenderPartMainLock");
	mAnimationSlideList = new AnimationSlideList();
	mAnimationSlideListBuf = new AnimationSlideList();

	g_FPSMax = 0.0f;
	g_FPSMin = 60.0f;
	g_FPSCheckCount = 0;

	previousPos.x = 0.0f;
	previousPos.y = 0.0f;
}

SVIRenderPartManager::~SVIRenderPartManager() {
	SVI_SVIFE_DELETE(mTestBedDelegator);
	SVI_SVIFE_DELETE(mAnimationSlideLock);
	SVI_SVIFE_DELETE(mRenderPartMainLock);
	SVI_SVIFE_DELETE(mAnimationSlideList);
	SVI_SVIFE_DELETE(mAnimationSlideListBuf);

	for(SVIUInt i=0; i<mImageList.size(); i++) {
		SVIImage* image = mImageList[i];
		SVI_SVIFE_DELETE(image);
	}
	mImageList.clear();
}

SVIRenderPartManager::SVIRenderPartManager(const SVIRenderPartManager& rhs) {}
void SVIRenderPartManager::operator=(const SVIRenderPartManager& rhs){}
#define SEC_A_DAY 86400
#define MAX_ELAPSED_TIME 1000
#define FPS_SVIMPLING_COUNT 5
//2011-10-24 masterkeaton27@gmail.com

void SVIRenderPartManager::setTime() {
	struct timeval t;
	gettimeofday(&t, NULL);

	//time is measured as millisecond.
	SVIUInt currentTime = (SVIUInt)((t.tv_sec % SEC_A_DAY)*1000 + (t.tv_usec / 1000));
	SVIUInt elapsedTime = currentTime - mDrawTime;

	mDeltaTime = elapsedTime;
	//2011-11-03 masterkeaton27@gmail.com
	//set time for shader.

#if FRAME_COUNT_BASE_ANIMATION
	mDrawTime += 16.6f;
#else
	mDrawTime = currentTime;
#endif

	//2012-04-30 masterkeaton27@gmail.com
	//change FPS calculating policy
	if( elapsedTime <= MAX_ELAPSED_TIME )
		mElaspedDrawTimeList.push_back(elapsedTime);
	else 
		mElaspedDrawTimeList.clear();

	DEBUG_CHECK_LOGE("SVIRenderPartManager::setTime() TIME:%d", mDrawTime);
	if( mElaspedDrawTimeList.size() >= FPS_SVIMPLING_COUNT ) {
		SVIUInt totalTime = 0;
		SVIUInt listSize = mElaspedDrawTimeList.size();
		for (SVIUInt i=0; i<listSize; i++) {
			totalTime += mElaspedDrawTimeList[i];
		}
		SVIFloat fps = (1000.0f / (totalTime / (SVIFloat)listSize));
		if( mEnableLogFPS ) LOGI("FPS(Sampling count:%d): %f FPS", FPS_SVIMPLING_COUNT, fps);
		mFPS = fps;
		if (mEnableShowFPS){
			g_FPSList.push_back(fps);
			if (g_FPSList.size() > 10){
				g_FPSList.erase(g_FPSList.begin());
			}
		}
		mElaspedDrawTimeList.clear();
	}
}

void SVIRenderPartManager::addUpdatedSlide(SVIBeamedSlide* slide) {
	mUpdatedSlideList.push_back(slide);
}

void SVIRenderPartManager::removeUpdatedSlideList(SVIBeamedSlide* slide) {
	if( slide == NULL )
		return;

	UpdatedBeamedSlideList::iterator iter;
	for(iter=mUpdatedSlideList.begin(); iter!=mUpdatedSlideList.end(); iter++) {
		if( slide == (*iter) ) {
			mUpdatedSlideList.erase(iter);
			break;
		}
	}
}

void SVIRenderPartManager::checkUpdateSlideList(){
	//CHECK_PERFORMANCE("");
	if( mUpdatedSlideList.size() > 0 ) {

		std::sort(mUpdatedSlideList.begin(), mUpdatedSlideList.end(), compareDepthForBeamedSlide);
	
		SVIUInt depthLevel = 0xffffffff;
		//CHECK_PERFORMANCE("run check loop performance.");
		for(SVIUInt i=0; i<mUpdatedSlideList.size(); i++) {
			SVIProjectionSlide* projectionSlide = mUpdatedSlideList[i]->getProjectionSlide();
			if( projectionSlide->isNeedInvalidate() ) {
				projectionSlide->invalidate();
			}
			projectionSlide->resetInvalidate();
		}
	}
}

void SVIRenderPartManager::clearUpdateSlideList() {
	if( mUpdatedSlideList.size() > 0 ) {
		for(SVIUInt i=0; i<mUpdatedSlideList.size(); i++) {
			mUpdatedSlideList[i]->clearChangePropertyList();
		}
		mUpdatedSlideList.clear();
	}
}

void SVIRenderPartManager::addAnimation(SVIAnimation* animation) {
	if( animation == NULL )
		return;

	mPlayingAnimationListBuf.push_back(animation);
	DEBUG_CHECK_LOGE("mPlayingAnimationListBuf size : %d, animation:%x", mPlayingAnimationListBuf.size(), (SVIInt)animation);
}

void SVIRenderPartManager::removeAnimation(SVIAnimation* animation) {
	DEBUG_CHECK_LOGE("SVIRenderPartManager::removeAnimation %x", (SVIInt)animation);
	mStopAnimationListBuf.push_back(animation);
}

void SVIRenderPartManager::checkAnimationList() {
	//CHECK_PERFORMANCE("");
	SVIAnimation* animation = NULL;

	if( mStopAnimationListBuf.size() > 0 ) {
		AnimationList::iterator iter;
		for(iter=mStopAnimationListBuf.begin(); iter != mStopAnimationListBuf.end(); iter++) {
			animation = (*iter);
			mStopAnimationList.push_back(animation);
		}
		mStopAnimationListBuf.clear();
	}

	if( !mStopAnimationList.empty() ) {
		//CHECK_PERFORMANCE("handle StopAnimationList count : %d", mStopAnimationList.size());
		AnimationList::iterator iter;
		SVIInt animationCount = mStopAnimationList.size();
		for(int i=0;i<animationCount; i++) {
			animation = mStopAnimationList[i];
			DEBUG_CHECK_LOGI("SVIRenderPartManager::checkAnimationList stopAnimation %x", (SVIInt)animation);
			removeAnimationFromAnimationList(animation);
			animation->endAnimation(mDrawTime);
			SVI_SVIFE_DELETE(animation);
		}
		DEBUG_CHECK_LOGI("SVIRenderPartManager::checkAnimationList() : %d animations will be removed.", animationCount);
		mStopAnimationList.clear();
	}

	if( mPlayingAnimationListBuf.size() > 0 ) {
		AnimationList::iterator iter;
		for(iter=mPlayingAnimationListBuf.begin(); iter != mPlayingAnimationListBuf.end(); iter++) {
			animation = (*iter);
			mPlayingAnimationList.push_back(animation);
		}
		mPlayingAnimationListBuf.clear();
	}
		
	if( !mPlayingAnimationList.empty() ) {
		//CHECK_PERFORMANCE("Handle AnimationList count:%d", mAnimationList.size());
		AnimationList::iterator iter;
		for(iter=mPlayingAnimationList.begin(); iter != mPlayingAnimationList.end(); iter++) {
			animation = (*iter);
			if( animation != NULL ) {
				animation->update(mDrawTime);
				//animation->print();
			}
			else 
				LOGE("Animation object is null !!!");
		}

		requestRender();
	}
}

void SVIRenderPartManager::clearAllAnimationList() {

	if( !mStopAnimationListBuf.empty() ) {
		AnimationList::iterator iter;
		SVIAnimation* animation = NULL;
		for(iter=mStopAnimationListBuf.begin(); iter != mStopAnimationListBuf.end(); iter++) {
			animation = (*iter);
			SVI_SVIFE_DELETE(animation);
		}
		mStopAnimationListBuf.clear();
	}

	if( !mStopAnimationList.empty() ) {
		AnimationList::iterator iter;
		SVIAnimation* animation = NULL;
		for(iter=mStopAnimationList.begin(); iter != mStopAnimationList.end(); iter++) {
			animation = (*iter);
			SVI_SVIFE_DELETE(animation);
		}
		mStopAnimationList.clear();
	}

	if( !mPlayingAnimationListBuf.empty() ) {
		AnimationList::iterator iter;
		SVIAnimation* animation = NULL;
		for(iter=mPlayingAnimationListBuf.begin(); iter != mPlayingAnimationListBuf.end(); iter++) {
			animation = (*iter);
			SVI_SVIFE_DELETE(animation);
		}
		mPlayingAnimationListBuf.clear();
	}

	if( !mPlayingAnimationList.empty() ) {
		AnimationList::iterator iter;
		SVIAnimation* animation = NULL;
		for(iter=mPlayingAnimationList.begin(); iter != mPlayingAnimationList.end(); iter++) {
			animation = (*iter);
			SVI_SVIFE_DELETE(animation);
		}
		mPlayingAnimationList.clear();
	}
}

SVIBool SVIRenderPartManager::removeAnimationFromAnimationList(SVIAnimation* animation) {
	AnimationList::iterator iter;

	for(iter = mPlayingAnimationListBuf.begin(); iter != mPlayingAnimationListBuf.end(); ++iter) {
		if( (*iter) == animation ) {
			mPlayingAnimationListBuf.erase(iter);
			return SVITRUE;
		}
	}

	for(iter = mPlayingAnimationList.begin(); iter != mPlayingAnimationList.end(); ++iter) {
		if( (*iter) == animation ) {
			mPlayingAnimationList.erase(iter);
			return SVITRUE;
		}
	}

	return SVIFALSE;
}

void SVIRenderPartManager::forceEndAllAnimation() {

	if( !mPlayingAnimationListBuf.empty() ) {
		AnimationList::iterator iter = mPlayingAnimationListBuf.begin();
		SVIAnimation* ani = NULL;
		for(iter; iter != mPlayingAnimationListBuf.end(); iter++) {
			ani = (*iter);
			mPlayingAnimationList.push_back(ani);
		}
		mPlayingAnimationListBuf.clear();
	}

	if( !mPlayingAnimationList.empty() ) {
		AnimationList::iterator iter = mPlayingAnimationList.begin();
		SVIAnimation* ani = NULL;
		SVIInt animationCount = mPlayingAnimationList.size();
		for(iter; iter != mPlayingAnimationList.end(); iter++) {
			ani = (*iter);
			ani->forceEnd();
		}
		LOGE("SVIRendarPartManager::forceEndAllAnimation() : %d animations were ended by force.", animationCount);
	}
}


SVIImage* SVIRenderPartManager::getImage(SVIInt idx) {
	if( idx < 0 || (SVIUInt)idx >= mImageList.size() )
		return NULL;

	return mImageList[idx];
}

/**
* Restores currently used textures to the present gl context.
*/
void SVIRenderPartManager::restoreImages(){	
	SVIMutex::SVIAutolock lock(mRenderPartMainLock);
	if(mRootSlide != NULL){
		mRootSlide->restoreImages();
	}
}

SVIInt SVIRenderPartManager::getImageCount() {
	return mImageList.size();
}
void SVIRenderPartManager::drawFrame(SVIBool updateMsg) {
	//CHECK_PERFORMANCE("");
	mIsHandledSetImage = SVIFALSE;

	
    if( updateMsg )
		handleMessage();

    
	SVIMutex::SVIAutolock lock(mRenderPartMainLock);

	//2012-05-23 masterkeaton27@gmail.com
	//split animation handler to prevent SEGV Fault after paused.
	if( updateMsg )
		handleRunSlideAnimation();

	//CHECK_PERFORMANCE("");
	if( mRootSlide == NULL )
		return;
		
	setTime();

	// 2012/06/13 rareboy0112@facebook.com
	// must update time of SVIParticleEffectManager
	mSVIGLSurface->getParticleEffectManager()->update(mDrawTime);

	/* check animation list to animate slides */
	checkAnimationList();

	/* check slide list to update slides */
	checkUpdateSlideList();

	SVIFloat fElapsedTime = (float)mDeltaTime * 0.001f;
	mSVIGLSurface->getRenderer()->updateScene(fElapsedTime);
	mSVIGLSurface->getRenderer()->renderScene();

	if (mEnableShowFPS){
		int size = g_FPSList.size();
		float avgFPS = 0.0;
		
		for (int n = g_FPSList.size()-1; n > 0; n--){
			float fps = g_FPSList[n];
			avgFPS += fps;
			float ratio = (float)n / (float)size;
			SVIVector4 vColor = SVIVector4(0.3f,0.9f,0.3f,ratio);
			mSVIGLSurface->getDebugRenderer()->drawNumeric(10.0f, 10.0f + 20.0f * n, fps, vColor);
			if (fps >= g_FPSMax && fps < 90.0f) g_FPSMax = fps;
			if (fps <= g_FPSMin) g_FPSMin = fps;
		}

		if (g_FPSList.size() > 0){
			avgFPS = avgFPS / (size-1);
			mSVIGLSurface->getDebugRenderer()->drawNumeric(10.0f, 10.0f + 20.0f * (size), avgFPS, SVIVector4(0.3f,1.0f,0.3f,1.0f), SVITRUE);
			mSVIGLSurface->getDebugRenderer()->drawNumeric(10.0f, 10.0f + 20.0f * (size+1), g_FPSMax, SVIVector4(1.0f,0.3f,0.3f,1.0f));
			mSVIGLSurface->getDebugRenderer()->drawNumeric(10.0f, 10.0f + 20.0f * (size+2), g_FPSMin, SVIVector4(1.0f,0.3f,0.3f,1.0f));
			g_FPSCheckCount++;
			if (g_FPSCheckCount > 500){
				g_FPSCheckCount = 0;
				g_FPSMax = 0.0f;
				g_FPSMin = 60.0f;
			}
		}
	}

	mSVIGLSurface->getDebugRenderer()->render();

	if( mTestBedDelegator )
		mTestBedDelegator->traversalBeamedTree();

	//2012/04/03 rareboy0112@facebook.com
	if( mEnableUpdateListener && mIsHandledSetImage )
		mSVIGLSurface->nativeUpdate(mSVIGLSurface->mJavaObject);

	clearUpdateSlideList();
}


void SVIRenderPartManager::pause() {
	SVIMutex::SVIAutolock lock(mRenderPartMainLock);
	mIsPaused = SVITRUE;
	LOGE("pause");
	forceEndAllAnimation();
	mSVIGLSurface->getRenderer()->finalize();
	LOGE("pause end");
}

void SVIRenderPartManager::resume() {
	SVIMutex::SVIAutolock lock(mRenderPartMainLock);
	
	//2012-05-23 masterkeaton27@gmail.com
	mIsPaused = SVIFALSE;
	LOGE("resume");	
	mSVIGLSurface->getRenderer()->restore();
	LOGE("resume end");
}

void SVIRenderPartManager::configurationChanged(SVIInt rotation) {
	LOGE("configurationChanged : %d", rotation);
	forceEndAllAnimation();
	if( mTestBedDelegator != NULL )
		mTestBedDelegator->configurationChanged(rotation);
}

void SVIRenderPartManager::resize(SVIInt w, SVIInt h, SVIInt rotation) {
	LOGI("resize");
	mSVIGLSurface->getRenderer()->resize(w,h);

	mWidth = w;
	mHeight = h;
	if( mTestBedEnable ) {
		LOGI("native base resize w:%d, h:%d", w, h);
		if( mTestBedDelegator == NULL ) {
			mTestBedDelegator = createTestBed(TEST_ANIMATION, w, h);
		} else { 
			mTestBedDelegator->resize(w, h, rotation);
		}

		mRootBeamedSlide = mTestBedDelegator->getRootSlide()->getBeamedSlide();
		mSVIGLSurface->getRenderer()->setRootSlide(mRootBeamedSlide->getProjectionSlide());
	} else {
		LOGI("java base resize w:%d, h:%d", w, h);
		if( mRootBeamedSlide != NULL )
			mSVIGLSurface->getRenderer()->setRootSlide(mRootBeamedSlide->getProjectionSlide());
	}

	requestRender();
	LOGI("resize end");
}

void SVIRenderPartManager::create() {
	mSVIGLSurface->getRenderer()->initialize();
}

void SVIRenderPartManager::touchEvent(SVIInt type, SVIFloat x, SVIFloat y) {

	if( mRootBeamedSlide == NULL )
		return;
	
	//down and move event
	if (type == 1){
		if( mTestBedDelegator != NULL ) {
			mTestBedDelegator->runTestBed();
		}
	}
	
	if (type == 2){
		mSVIGLSurface->getRenderer()->mTouchPosition = SVIVector2(1.0f - x/mWidth,y/mHeight);
		return;
	}

	//touch move test.
	if( type == 0 || type == 2 ) {
		static SVIBeamedSlide* mainSlide = NULL;
		if( mainSlide == NULL ) {
			BaseSlideList list = mRootBeamedSlide->getSubSlides();
			mainSlide = (SVIBeamedSlide*)(*list.begin());
		}
		mainSlide->setOpacity(x/480.0f);
		mainSlide->setPosition(SVIPoint(x, y));
		mainSlide->setClipSubSlide(SVITRUE);

		requestRender();
	}
}

void SVIRenderPartManager::requestRender() {
	if (mIsWallpaper){
		requestRenderGLWallpaper();
	}else if(mIsExternal){
		//requestRenderGLExternalSurface();
	}else{
		mSVIGLSurface->requestRenderGLSurface(mSVIGLSurface->mJavaObject);
	}
}

void SVIRenderPartManager::addTestImage(SVIImage* image) {
	mImageList.push_back(image);
}

void SVIRenderPartManager::onAnimationEnd(SVIAnimation* animation) {
	DEBUG_CHECK_LOGI("onAnimationEnd");
}
void SVIRenderPartManager::onAnimationRepeat(SVIAnimation* animation) {
	DEBUG_CHECK_LOGI("onAnimationRepeat");
}
void SVIRenderPartManager::onAnimationStart(SVIAnimation* animation) {
	DEBUG_CHECK_LOGI("onAnimationStart");
}

void SVIRenderPartManager::addAnimationSlide(SVISlide* slide) {
	//SVIMutex::SVIAutolock lock(mAnimationSlideLock);

	SVISlide* temp = NULL;
	for(AnimationSlideList::iterator iter = mAnimationSlideListBuf->begin(); iter != mAnimationSlideListBuf->end(); iter++) {
		temp = (SVISlide*)(*iter);
		if(temp == slide) {
			DEBUG_CHECK_LOGE("SVISlide::isExistImplicitSlide : slide=%d", (SVIUInt)temp);		
			return;
		}
	}

	mAnimationSlideListBuf->push_back(slide);
}

void SVIRenderPartManager::removeAnimationSlide(SVISlide* slide) {
	SVISlide* temp = NULL;		

	if( !mAnimationSlideListBuf->empty() ) {
		SVIBool found = SVIFALSE;
		AnimationSlideList::iterator iter;
		//LOGI("SVIRenderPartManager::removeAnimationSlide mAnimationSlideListBuf %x", slide);
		while(1) {
			found = SVIFALSE;
			for(iter = mAnimationSlideListBuf->begin(); iter != mAnimationSlideListBuf->end(); iter++) {
				temp = (SVISlide*)(*iter);
				if(temp == slide) {
					DEBUG_CHECK_LOGE("SVISlide::removeAnimationSlide from mAnimationSlideListBuf: slide=%x", (SVIUInt)temp);		
					found = SVITRUE;
					break;
				}
			}
			if( found ) 
				mAnimationSlideListBuf->erase(iter);
			else
				break;
		}
	}

	if( !mAnimationSlideList->empty() ) {
		SVIBool found = SVIFALSE;
		AnimationSlideList::iterator iter;
		//LOGI("SVIRenderPartManager::removeAnimationSlide mAnimationSlideList %x", slide);
		while(1) {
			found = SVIFALSE;
			for(iter = mAnimationSlideList->begin(); iter != mAnimationSlideList->end(); iter++) {
				temp = (SVISlide*)(*iter);
				if(temp == slide) {
					DEBUG_CHECK_LOGE("SVISlide::removeAnimationSlide from mAnimationSlideList: slide=%x", (SVIUInt)temp);		
					found = SVITRUE;
					break;
				}
			}
			if( found ) 
				mAnimationSlideList->erase(iter);
			else
				break;
		}
	}
}

void SVIRenderPartManager::swapAnimationSlide() {

	SVIMutex::SVIAutolock lock(mAnimationSlideLock);

	AnimationSlideList *temp;

	if (mAnimationSlideListBuf->size() == 0) {
		return;
	}
	
	if (mAnimationSlideList->size() == 0) {
		temp = mAnimationSlideList;
		mAnimationSlideList = mAnimationSlideListBuf;
		mAnimationSlideListBuf = temp;
	}
	else {
		mAnimationSlideList->merge(*mAnimationSlideListBuf);
	}
}



void SVIRenderPartManager::handleRunSlideAnimation(){
	//2012-05-23 masterkeaton27@gmail.com
	if (mIsPaused){
		mMessageHandledSlideMap.clear();
		LOGI("clear message-map : handleRunSlideAnimation called after onPause");
		return;
	}

	//LOGI("MessageHandled Slide count : %d", mMessageHandledSlideMap.size());
	SVISlideMap::iterator iter = mMessageHandledSlideMap.begin();
	for(iter; iter != mMessageHandledSlideMap.end(); iter++) {
		SVISlide* slide = iter->second;
		//LOGE("runSlideAnimation slide:%x", slide);
		slide->runSlideAnimation();
	}

	mMessageHandledSlideMap.clear();
}

void SVIRenderPartManager::handleMessage() {
	//2012-05-23 masterkeaton27@gmail.com
	mSVIGLSurface->getMessageHandler()->handleMessage();	
}

void SVIRenderPartManager::addMessageHandledSlide(SVISlide* slide) {
	SVIInt slideKey = (SVIInt)slide;
	SVISlideMap::iterator iter = mMessageHandledSlideMap.find(slideKey);

	if( iter == mMessageHandledSlideMap.end() )
		mMessageHandledSlideMap.insert(std::pair<SVIInt, SVISlide*>((SVIInt)slide, slide));
}

void SVIRenderPartManager::removeMessageHandledSlide(SVISlide* slide) {
	SVIInt slideKey = (SVIInt)slide;
	SVISlideMap::iterator iter = mMessageHandledSlideMap.find(slideKey);

	if( iter != mMessageHandledSlideMap.end() ) {
		//LOGE("[SVIRenderPartManager::removeMessageHandledSlide] Removed slide:%x", slide);
		mMessageHandledSlideMap.erase(iter);
	}
	/*
	else 
		LOGE("[SVIRenderPartManager::removeMessageHandledSlide] Failed to search slide:%x", slide);
	*/
}

void SVIRenderPartManager::clearAnimationSlide() {
	SVIMutex::SVIAutolock lock(mAnimationSlideLock);

	if( mAnimationSlideList != NULL )
		mAnimationSlideList->clear();
	if( mAnimationSlideListBuf != NULL )
		mAnimationSlideListBuf->clear();
}

SVISize SVIRenderPartManager::getSize() {
	return SVISize(mWidth, mHeight);
}

#define ENABLE_DEFAULT_ROOT_SLIDE 0
void SVIRenderPartManager::setRootSlide(SVISlide* slide) {

	SVIMutex::SVIAutolock lock(mRenderPartMainLock);

	LOGE("SVIRenderPartManager::setRootSlide %x", (SVIUInt)slide);

	// TODO : 
	// If ENABLE_DEFAULT_ROOT_SLIDE is enable, performance become slow. 
	// We should find out cause why this make performance slow.
#if ENABLE_DEFAULT_ROOT_SLIDE
	if( mRootSlide == NULL ) {
		mRootSlide = new SVISlide();
		mRootSlide->initialize(NULL, slide->getRegion(), slide->getBgColor(), NULL);
	}

	if( slide != NULL ) {
		mRootSlide->addModelSlide(slide);
		mRootBeamedSlide = mRootSlide->getBeamedSlide();
	} else {
		mRootBeamedSlide = NULL;
		mSVIGLSurface->getRenderer()->setRootSlide(NULL);
	}
#else
	mRootSlide = slide;
	if( slide != NULL ) {
		mRootBeamedSlide = mRootSlide->getBeamedSlide();
	} else {
		mRootBeamedSlide = NULL;
		mSVIGLSurface->getRenderer()->setRootSlide(NULL);
	}
#endif
}

void SVIRenderPartManager::removeAllRelatedToSlide(SVISlide* slide) {
	if( slide == NULL )
		return;

	SVIMutex::SVIAutolock lock(mRenderPartMainLock);
	slide->removeAnimation();
	removeUpdatedSlideList(slide->getBeamedSlide());
	removeAnimationSlide(slide);
	//SVIMessageHandler::getInstance()->removeMessage(slide);
}

SVIBool SVIRenderPartManager::isAnimating() {
	return ( mPlayingAnimationList.size() > 0 || mPlayingAnimationListBuf.size() > 0 ) ? SVITRUE : SVIFALSE;
}



/*****************************************************************************************
/ SVIMessageHandler class implementation Part.
*****************************************************************************************/

const SVIChar* SVIMessageHandler::mMessageString[] = {
	"SM_SET_ROOT_SLIDE",
	"SM_ADD_SLIDE",
	"SM_INSERT_SLIDE",
	"SM_REMOVE_SLIDE",
	"SM_FREE_SLIDE",
	"SM_SET_REGION",
	"SM_SET_BOUND",
	"SM_SET_POSITION",
	"SM_SET_PIVOT_POINT",
	"SM_SET_ZPOSITION",
	"SM_SET_ROTATION",
	"SM_SET_SCALE",
	"SM_SET_BG_COLOR",
	"SM_SET_OPACITY",
	"SM_SET_CORNER_RADIUS",
	"SM_SET_BORDER_WIDTH",
	"SM_SET_BORDER_COLOR",
	"SM_SET_SHADOW_RADIUS",
	"SM_SET_SHADOW_COLOR",
	"SM_SET_SHADOW_OPACITY",
	"SM_SET_SHADOW_OFFSET",
	"SM_SET_TEXTURE_REGION",
	"SM_SET_BACKFACE_TEXTURE_REGION",
	"SM_SET_CLIP_SUBSLIDE",
	"SM_SET_HIDDEN",
	"SM_SET_ZORDER_TYPE",
	"SM_SET_SWIPE_TYPE",
	"SM_SET_DEFORM_TYPE",
	"SM_SET_DEFORM_DIRECTION",
	"SM_SET_ORTHOGONAL",
	"SM_SET_CUSTOM_RTE",
	"SM_SET_HOLD_OPACITY",
	"SM_SET_HOLD_SCALE",
	"SM_SET_CAPTURE",
	"SM_SET_BLEND_TYPE",
	"SM_SET_ANTI_ALIASING", //2011.12.09 jongchae.moon
	"SM_SET_RT_EFFECT_TYPE",
	"SM_SET_IMAGESCALE_TYPE",
	"SM_SET_IMAGESCALE_REGION",
	"SM_SET_LIGHT_TYPE",
	"SM_SET_LIGHT_RADIUS",
	"SM_SET_LIGHT_COLOR",
	"SM_SET_LIGHT_OPACITY",
	"SM_SET_LIGHT_ANGLE",
	"SM_SET_LIGHT_OFFSET",
	"SM_SET_LIGHT_POWER",
	"SM_SET_IMAGE",
	"SM_SET_BACKFACE_IMAGE",
	"SM_SET_BACKFACE_IMAGE_FLIP",
	"SM_SET_BACKFACE_IMAGE_CLEAR",
	"SM_FREE_IMAGE",
	"SM_ADD_EXPLICIT_ANIMATION",
	"SM_STOP_ANIMATION",
	"SM_SET_ROTATION_FROM",
	"SM_SET_SWIPE_RATIO",
	"SM_SET_DEFORM_RATIO",
	"SM_SET_DEFORM_TOUCH_DOWN",
	"SM_SET_RTE_RATIO",
	"SM_SET_MODEL_POSITION",
	"SM_SET_MODEL_ROTATION",
	"SM_SET_MODEL_SCALE",
	"SM_ADD_PARTICLE_EFFECT",
    "SM_SET_USENORMALMAP",
    "SM_SET_USEPOSTEFFECT",
    "SM_SET_POSTEFFECT_TYPE",
    "SM_SET_RIPPLE_WAVE",
    "SM_SET_RIPPLE_ENVMAP",
    "SM_SET_RIPPLE_INFO",
    "SM_SET_RIPPLE_RESET",
    "SM_ADD_PATH_POINT",    //2013-06-21 masterkeaton27@gmail.com
    "SM_CLEAR_PATH_POINTS", //2013-06-21 masterkeaton27@gmail.com
    "SM_SET_FILTER_TYPE",   //2013-06-21 masterkeaton27@gmail.com
    "SM_SET_TESSELATION",
    "SM_SET_MORPHING_TYPE",
    "SM_SET_MORPHING_RADIUS",
    "SM_SET_MORPHING_GRID_COUNT",
    "SM_SET_MORPHING_COLOR",
	"SM_MSG_COUNT"
};

SVIMessageHandler::SVIMessageHandler(SVIGLSurface* saGLSurface) {//Multi-Instance-Support
	mSVIGLSurface=saGLSurface;
	mLock = new SVIMutex("Message Handler");
	mMessageQueue.reserve(100);
	mMessageQueueBuf.reserve(100);
	mMessageCounter = 0;
}

SVIMessageHandler::~SVIMessageHandler() {	
	SVI_SVIFE_DELETE(mLock);

	MessageQueue::iterator iter = mMessageQueue.begin();
	for(iter; iter != mMessageQueue.end(); iter++) {
		SVIMessage& msg = (*iter);
		if( msg.mSlide == NULL )
			messageProc(msg);
		else {
			msg.mSlide->slideProc(msg);
			mSVIGLSurface->getRenderPartManager()->addMessageHandledSlide(msg.mSlide);
		}
	}
	mMessageQueue.clear();

	iter = mMessageQueueBuf.begin();
	for(iter; iter != mMessageQueueBuf.end(); iter++) {
		SVIMessage& msg = (*iter);
		if( msg.mSlide == NULL )
			messageProc(msg);
		else {
			msg.mSlide->slideProc(msg);
			mSVIGLSurface->getRenderPartManager()->addMessageHandledSlide(msg.mSlide);
		}
	}
	mMessageQueueBuf.clear();
	
	
	mSVIGLSurface = NULL;
}

SVIMessageHandler::SVIMessageHandler(const SVIMessageHandler& rhs){}
void SVIMessageHandler::operator=(const SVIMessageHandler& rhs){}

const SVIChar* SVIMessageHandler::getMessageString(const SVIInt& msgID) {
	return mMessageString[msgID];
}

void SVIMessageHandler::sendMessage(SVIMessage& msg) {
	SVIMutex::SVIAutolock lock(mLock);

	// SM_ADD_EXPLICIT_ANIMATION should have top priority to handle explicit animation prior to implicit animation.
	// Because, if explicit animation is assigned, implicit animation can not be handled.
	if( msg.mMessage == SM_ADD_EXPLICIT_ANIMATION || msg.mMessage == SM_STOP_ANIMATION )
		msg.mPriority = SVIMessage::TOP;
	// SM_SET_ROTATION_FROM should have bottom priority due to handling rotation animation.
	// ( Because rotation animation is using position animation to compensate center position of slide.
	//   If position animation is handled prior to region animation, position can not be applied due to region.
	//   So, Message handler should change priority of SM_SET_ROTATION_FROM. )
	else if( msg.mMessage == SM_SET_ROTATION_FROM || msg.mMessage == SM_FREE_SLIDE )
		msg.mPriority = SVIMessage::BOTTOM;

	msg.mMessageID = ++mMessageCounter;

	DEBUG_CHECK_LOGI("sendMessage : %s msgID:%d, priority:%d", getMessageString(msg.mMessage), msg.getID(), msg.getPriority());
	mMessageQueueBuf.push_back(msg);
}

void SVIMessageHandler::removeMessage(SVISlide* slide) {
	if( !mMessageQueue.empty() )
		removeMessage(mMessageQueue, slide);
}

void SVIMessageHandler::removeMessage(MessageQueue& msgQueue, SVISlide* slide){
	MessageQueue::iterator iter;
	SVIBool found = SVIFALSE;
	while(1) {
		found = SVIFALSE;
		for(iter=msgQueue.begin(); iter != msgQueue.end(); iter++) {
			SVIMessage& msg = (*iter);
			if( msg.mSlide == slide || (SVISlide*)msg.mParam1 == slide ) {
				found = SVITRUE;
				break;
			}
		}

		if( found ) 
			msgQueue.erase(iter);
		else 
			break;
	}
}

void SVIMessageHandler::swapMessage() {
	SVIMutex::SVIAutolock lock(mLock);
	
	if (mMessageQueueBuf.size() == 0) {
		return;
	}
	
	MessageQueue::iterator iter = mMessageQueueBuf.begin();

	for(iter; iter != mMessageQueueBuf.end(); iter++) {
		SVIMessage& msg = (*iter);
		mMessageQueue.push_back(msg);
	}

	//for(int i=0; i<mMessageQueue.size(); i++) 
	//	LOGE("Before Sort :%s ID : %d priority:%d", SVIMessageHandler::getMessageString(mMessageQueue[i].mMessage), mMessageQueue[i].getID(), mMessageQueue[i].getPriority());

	std::sort(mMessageQueue.begin(), mMessageQueue.end());

	//for(int i=0; i<mMessageQueue.size(); i++) 
	//	LOGI("After Sort :%s ID : %d priority:%d", SVIMessageHandler::getMessageString(mMessageQueue[i].mMessage), mMessageQueue[i].getID(), mMessageQueue[i].getPriority());

	mMessageQueueBuf.clear();
}

void SVIMessageHandler::clearAllMessage() {
	SVIMutex::SVIAutolock lock(mLock);

	mMessageQueue.clear();
	mMessageQueueBuf.clear();
}

void SVIMessageHandler::handleMessage() {
	SVIMutex::SVIAutolock lock(mLock);

	MessageQueue::iterator iter = mMessageQueue.begin();

	for(iter; iter != mMessageQueue.end(); iter++) {
		SVIMessage& msg = (*iter);
		if( msg.mSlide == NULL )
			messageProc(msg);
		else {
			msg.mSlide->slideProc(msg);
			mSVIGLSurface->getRenderPartManager()->addMessageHandledSlide(msg.mSlide);
		}
	}

	mMessageQueue.clear();
}

void SVIMessageHandler::messageProc(const SVIMessage& msg) {
	if( msg.mSlide != NULL )
		LOGE("Slide pointer of message should be NULL in SVIMessageHandler::handleMessage");

	DEBUG_CHECK_LOGI("handleMessage slide:%x, msg type:%s, param1:%x", (SVIUInt)msg.mSlide, mSVIGLSurface->getMessageHandler()->getMessageString(msg.mMessage), msg.mParam1);
	//LOGE("handleMessage slide:%x, msg type:%s, param1:%x", msg.mSlide, SVIMessageHandler::getMessageString(msg.mMessage), msg.mParam1);
	
	// 2011/09/30 rareboy0112@facebook.com
	// Add resetInvalidate code to the way the message queue.
	
	switch(msg.mMessage){
	case SM_SET_ROOT_SLIDE:
		{
			SVISlide* rootSlide = msg.mSlide;
			mSVIGLSurface->getRenderPartManager()->setRootSlide(rootSlide);
			DEBUG_CHECK_LOGI("SM_SET_ROOT_SLIDE : Root slide is %x", (SVIUInt)rootSlide);
			break;
		}
		break;

	case SM_FREE_SLIDE :
		{
			//2012-05-11 masterkeaton27@gmail.com
			//wrong type casting fixed.
			SVISlide* slide = (SVISlide*)msg.mParam1;

			if( slide != NULL ) 
				slide->stopAnimationDirectly();

			mSVIGLSurface->getRenderPartManager()->removeMessageHandledSlide(slide);

			SVI_SVIFE_DELETE(slide);
		}
		break;	

	case SM_FREE_IMAGE:
		{
		//This message will not be sent.
		}
		break;           

    case SM_SET_USEPOSTEFFECT:
        {
            mSVIGLSurface->getRenderer()->mUsePostEffect = msg.mParam1;
        }
        break;

    case SM_SET_POSTEFFECT_TYPE:
        {
            int type = msg.mParam1;
            if (type == POST_EFFECT_BLOOM){
                mSVIGLSurface->getRenderer()->mUsePostRippleEffect = SVIFALSE;
            }else if (type == POST_EFFECT_RIPPLE){
                mSVIGLSurface->getRenderer()->mUsePostRippleEffect = SVITRUE;
            }
        }
        break;

    case SM_SET_RIPPLE_WAVE: 
        {
            SVIVector3* info = (SVIVector3*)msg.mParam1;
            mSVIGLSurface->getRippleRenderer()->ripple(info->x,info->y,info->z);
            SVI_SVIFE_DELETE(info);
        }
        break;

    case SM_SET_RIPPLE_ENVMAP: 
        {
            SVIImage* image = (SVIImage*)msg.mParam1;
            mSVIGLSurface->getRippleRenderer()->setEnvImage(image);
        }
        break;

    case SM_SET_RIPPLE_RESET:
        {
            mSVIGLSurface->getRippleRenderer()->reset();
        }
        break;

    case SM_SET_RIPPLE_INFO:
        {
            SVIVector3* info = (SVIVector3*)msg.mParam1;
            mSVIGLSurface->getRippleRenderer()->setReductionRatio(info->x);
            mSVIGLSurface->getRippleRenderer()->setThreshhold(info->y);
            SVI_SVIFE_DELETE(info);
        }
        break;
    }


}
} // end of SVI namespace
