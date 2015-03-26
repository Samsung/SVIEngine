#include "SVIBaseSlide.h"
#include "../Utils/SVIMutex.h"
#include "../Particle/SVIParticleEffect.h"

namespace SVI {

#define ADD_CHANGE_PROPERTY_INFO(type)		addChangePropery(type)

static const SVIBool	DEBUG = SVIFALSE;


SVIBaseSlide::SVIBaseSlide(SVIGLSurface* surface) :
mPivotPoint(0.5f, 0.5f),
mBgColor(0.0f, 0.0f, 0.0f, 1.0f),
mBorderColor(0.5f, 0.5f, 0.5f, 1.0f),
mScale(1.0f, 1.0f, 1.0f),
mRotation(0.0f, 0.0f, 0.0f, 1.0f){
	DEBUG_CHECK_LOGE("SVIBaseSlide::SVIBaseSlide()");
	
	mSuperSlide = NULL;


	mZPosition = 0.0f;

	mClipSubSlide = SVIFALSE;
	mOpacity = 1.0f;
	mHidden = SVIFALSE;
	mHoldOpacity = SVIFALSE;
	mOrthogonal = SVITRUE;
	mCustomRTE = SVIFALSE;
    mUseNormalMap = SVIFALSE;

	SVIQuaternion q;
	q.setEuler(SVIVector3(0.0f,0.0f,0.0f));
	mRotation = SVIVector4(q);

	mSwipeRatio = SVIVector3(1.0f,1.0f,1.0f);
	
	mDeformRatio = SVIVector3(0.0f,0.0f,0.0f);
	mRTERatio = SVIVector3(0.0f, 0.0f, 0.0f);

	mCornerRadius = 0.0f;
	mBorderWidth = 0.0f;
	
	//2011-06-15 masterkeaton27@gmail.com
	//default shadow setting
	mShadowColor = SVIColor(0.0f,0.0f,0.0f,0.6f);
	mShadowRadius = 0.0f;
	mShadowOffset.mX = 0.0f;
	mShadowOffset.mY = 0.0f;
	//2012-03-12 masterkeaton27@gmail.com
	//initlaize bug fixed.
	mShadowOpacity = 1.0f;

	//2011-08-26 masterkeaton27@gmail.com
	//default light setting
	mLightRadius = 0.5f;
	mLightOpacity = 1.0f;
	mLightAngle = 0.0f;
	mLightOffset = SVIPoint(0.5f,0.5f);
	mLightType = 0; // zero means no light
	mLightColor = SVIColor(1.0f,1.0f,1.0f,1.0f);
    mLightDirection = SVIVector4(0.0f,0.0f,1.0f,0.0f);
	mViewDirection = SVIVector4(0.0f,0.0f,1.0f,0.0f);
	mLightPower = 1.0f;

	mTextureBackFaceSpriteRegion = SVIRect(0.0f,0.0f,1.0f,1.0f); 
	mTextureSpriteRegion = SVIRect(0.0f,0.0f,1.0f,1.0f); 
	mTextureScaleFitRegion = SVIRect(0.0f,0.0f,1.0f,1.0f); 
	mZOrderType = NORMAL;
	mDeformType = DEFORM_NONE;
	mFilterType = FILTER_NONE;
    mSwipeType = SWIPE_NONE;
	mDeformDirection = DEFORM_HORIZONTAL;

	mImageScaleType = FIT_XY;
	mBlendType = BLEND_MULTIPLY;
	mRTEffectType = RT_NONE;
	mHoldScale = SVIFALSE;
	mCapture = SVIFALSE;

	mModelPosition = SVIVector3(0.0f, 0.0f, 0.0f);
	mModelRotation = SVIVector4(0.0f, 0.0f, 0.0f, 1.0f);
	mModelScale = SVIVector3(1.0f, 1.0f, 1.0f);

	mLock = new SVIMutex("BaseSlideLock");

	//2011.12.09 jongchae.moon
	mEnableAA = SVIFALSE;

    //2012/12/08 rareboy0112@facebook.com
    mTesselation = SVIFALSE;
    mMorphingType = MORPHING_NONE;

	mSVIGLSurface = surface;
	//
	mMemHack = 0;
}

SVIBaseSlide::SVIBaseSlide(const SVIBaseSlide& rhs) {
	copy(rhs);
	
	SVI_SVIFE_DELETE(mLock);
	mLock = new SVIMutex("BaseSlideLock");
}

SVIBaseSlide::~SVIBaseSlide() {
	SVI_SVIFE_DELETE(mLock);

	while( !(mParticleEffectList.empty()) ) {
		SVIParticleEffect* effect = mParticleEffectList.front();
		mParticleEffectList.pop_front();
		SVI_SVIFE_DELETE(effect);
	}
	mParticleEffectList.clear();

	mSVIGLSurface = NULL;
}

SVIBaseSlide& SVIBaseSlide::operator = (const SVIBaseSlide& rhs) {
	copy(rhs);
	return *this;
}

SVIBool SVIBaseSlide::operator == (const SVIBaseSlide& rhs) {
	 return SVITRUE;
}

void SVIBaseSlide::clearSubSlides() {
	DEBUG_CHECK_LOGE("SVIBaseSlide::clearSubSlides");
	if( mSubSlideList.size() > 0 ) {
		DEBUG_CHECK_LOGE("SVIBaseSlide::clearSubSlides 1: size:%d", mSubSlideList.size());
		BaseSlideListIter iter = mSubSlideList.begin();
		SVIBaseSlide* slide = NULL;
		for(iter; iter != mSubSlideList.end(); iter++) {
			slide = (SVIBaseSlide*)(*iter);
			DEBUG_CHECK_LOGE("SVIBaseSlide::clearSubSlides 2");
			SVI_SVIFE_DELETE(slide);
		}

		mSubSlideList.clear();
		DEBUG_CHECK_LOGE("SVIBaseSlide::clearSubSlides 3");
	}
}

void SVIBaseSlide::copy(const SVIBaseSlide& rhs) {
	mSubSlideList = rhs.mSubSlideList;
	mSuperSlide = rhs.mSuperSlide;

	mRegion = rhs.mRegion;
	mBounds = rhs.mBounds;
	mPosition = rhs.mPosition;
	mZPosition = rhs.mZPosition;
	mRotation = rhs.mRotation;
	mScale = rhs.mScale;

	mBgColor = rhs.mBgColor;
	mClipSubSlide = rhs.mClipSubSlide;
	mOpacity = rhs.mOpacity;
	mHoldOpacity = rhs.mHoldOpacity;
	mHidden = rhs.mHidden;
	mCornerRadius = rhs.mCornerRadius;
	mBorderWidth = rhs.mBorderWidth;
	mBorderColor = rhs.mBorderColor;
	mShadowColor = rhs.mShadowColor;
	mShadowRadius = rhs.mShadowRadius;
	mShadowOffset = rhs.mShadowOffset;
	mShadowOpacity = rhs.mShadowOpacity;

	mLightRadius = rhs.mLightRadius;
	mLightOpacity = rhs.mLightOpacity;
	mLightAngle = rhs.mLightAngle;
	mLightOffset = rhs.mLightOffset;
	mLightType = rhs.mLightType; 
	mLightColor = rhs.mLightColor;
    mLightDirection = rhs.mLightDirection;
	mViewDirection = rhs.mViewDirection;
	mLightPower = rhs.mLightPower;

	mTextureScaleFitRegion = rhs.mTextureScaleFitRegion;
	mTextureSpriteRegion = rhs.mTextureSpriteRegion;
	mTextureBackFaceSpriteRegion = rhs.mTextureBackFaceSpriteRegion;
	mZOrderType = rhs.mZOrderType;
	mDeformType = rhs.mDeformType;
    mFilterType = rhs.mFilterType;
	mSwipeType = rhs.mSwipeType;
	mBlendType = rhs.mBlendType;

	mImageScaleType = rhs.mImageScaleType;
	mOrthogonal = rhs.mOrthogonal;
    mUseNormalMap = rhs.mUseNormalMap;
	mCustomRTE = rhs.mCustomRTE;
	mHoldScale = rhs.mHoldScale;
	mCapture = rhs.mCapture;
	mRTEffectType = rhs.mRTEffectType;

	mDeformDirection = rhs.mDeformDirection; 
	mDeformRatio = rhs.mDeformRatio;
	mSwipeRatio = rhs.mSwipeRatio;
	mRTERatio = rhs.mRTERatio;
	mLock =rhs.mLock;
	mEnableAA = rhs.mEnableAA;
	mMemHack =rhs.mMemHack;

    mTesselation = rhs.mTesselation;
    mMorphingType = rhs.mMorphingType;

    if(rhs.mTesselationPointList.size() > 0)
    {
        mTesselationPointList.reserve(rhs.mTesselationPointList.size());
        std::copy(rhs.mTesselationPointList.begin(), rhs.mTesselationPointList.end(),
            std::back_inserter(mTesselationPointList));
    }

	mSVIGLSurface = rhs.mSVIGLSurface;
}

void SVIBaseSlide::copyBaseProperties(const SVIBaseSlide& rhs) {
	mSubSlideList = rhs.mSubSlideList;
	mSuperSlide = rhs.mSuperSlide;

	mRegion = rhs.mRegion;
	mBounds = rhs.mBounds;
	mPosition = rhs.mPosition;
	mZPosition = rhs.mZPosition;
	mRotation = rhs.mRotation;
	mScale = rhs.mScale;

	mBgColor = rhs.mBgColor;
	mClipSubSlide = rhs.mClipSubSlide;
	mOpacity = rhs.mOpacity;
	mHoldOpacity = rhs.mHoldOpacity;
	mHidden = rhs.mHidden;
	mCornerRadius = rhs.mCornerRadius;
	mBorderWidth = rhs.mBorderWidth;
	mBorderColor = rhs.mBorderColor;
	mShadowColor = rhs.mShadowColor;
	mShadowRadius = rhs.mShadowRadius;
	mShadowOffset = rhs.mShadowOffset;

	mLightRadius = rhs.mLightRadius;
	mLightOpacity = rhs.mLightOpacity;
	mLightAngle = rhs.mLightAngle;
	mLightOffset = rhs.mLightOffset;
	mLightType = rhs.mLightType; 
	mLightColor = rhs.mLightColor;
    mLightDirection = rhs.mLightDirection;
	mViewDirection = rhs.mViewDirection;
	mLightPower = rhs.mLightPower;

	mTextureScaleFitRegion = rhs.mTextureScaleFitRegion;
	mTextureSpriteRegion = rhs.mTextureSpriteRegion;
	mTextureBackFaceSpriteRegion = rhs.mTextureBackFaceSpriteRegion;
	mZOrderType = rhs.mZOrderType;
	mDeformType = rhs.mDeformType;
    mFilterType = rhs.mFilterType;
	mSwipeType = rhs.mSwipeType;

	mImageScaleType = rhs.mImageScaleType;
	mOrthogonal = rhs.mOrthogonal;
    mUseNormalMap = rhs.mUseNormalMap;
	mCustomRTE = rhs.mCustomRTE;
	mHoldScale = rhs.mHoldScale;
	mCapture = rhs.mCapture;
	mRTEffectType = rhs.mRTEffectType;

	mDeformDirection = rhs.mDeformDirection;
	mDeformRatio = rhs.mDeformRatio;
	mSwipeRatio = rhs.mSwipeRatio;

	mRTERatio = rhs.mRTERatio;

    mTesselation = rhs.mTesselation;
    mMorphingType = rhs.mMorphingType;

    if(rhs.mTesselationPointList.size() > 0)
    {
        mTesselationPointList.reserve(rhs.mTesselationPointList.size());
        std::copy(rhs.mTesselationPointList.begin(), rhs.mTesselationPointList.end(),
            std::back_inserter(mTesselationPointList));
    }
}

void SVIBaseSlide::updateGeometryByRegion() {
	mBounds = SVIRect(SVIPoint(0.0f, 0.0f), mRegion.getSize());
	mPosition.mX = mRegion.mOrigin.mX + ( mRegion.mSize.mWidth * mPivotPoint.mX * mScale.x);
	mPosition.mY = mRegion.mOrigin.mY + ( mRegion.mSize.mHeight * mPivotPoint.mY * mScale.y);
}

void SVIBaseSlide::updateGeometryByBoundsOrPosition() {
	SVIFloat dx = mBounds.mSize.mWidth * mPivotPoint.mX;
	SVIFloat dy = mBounds.mSize.mHeight * mPivotPoint.mY;

	SVIPoint origin = SVIPoint(mPosition.mX - dx, mPosition.mY - dy);
	mRegion = SVIRect(origin, mBounds.mSize);
}

void SVIBaseSlide::updateGeometryByPivotPoint() {
	SVIFloat dx = mBounds.mSize.mWidth * mPivotPoint.mX;
	SVIFloat dy = mBounds.mSize.mHeight * mPivotPoint.mY;

	mPosition.mX = mRegion.mOrigin.mX + dx;
	mPosition.mY = mRegion.mOrigin.mY + dy;
}


SVIBool SVIBaseSlide::addSubSlide(SVIBaseSlide* slide) {

	SVIMutex::SVIAutolock lock(mLock);

	if( slide == NULL )
		return SVIFALSE;

	SVIBool ret = SVIFALSE;

	if( getSlideIndex(slide) == -1 ) {
		mSubSlideList.push_back(slide);
		ret = SVITRUE;

		slide->setParent(this);
	}

	DEBUG_CHECK_LOGI("%x: SubSlideList size : %d", this, mSubSlideList.size());

	return ret;
}

SVIBool SVIBaseSlide::removeSubSlide(SVIBaseSlide* slide) {
	SVIMutex::SVIAutolock lock(mLock);
	BaseSlideListIter iter = findSlideIter(slide);
	if( iter == mSubSlideList.end() ) {
		LOGE("SVIBaseSlide::removeSubSlide : Failed to remove slide!!!");
		return SVIFALSE;
	}
	(*iter)->setParent(NULL);
	mSubSlideList.erase(iter);

	return SVITRUE;
}

void SVIBaseSlide::removeFromSuperSlide() {
	SVIMutex::SVIAutolock lock(mLock);

	if( mSuperSlide != NULL ) {
		BaseSlideListIter iter = mSuperSlide->findSlideIter(this);
		if( iter != mSuperSlide->mSubSlideList.end() ) {
			(*iter)->setParent(NULL);
			mSuperSlide->mSubSlideList.erase(iter);
		}
	}
}

SVIBool SVIBaseSlide::insertSubSlide(SVIInt index, SVIBaseSlide* newSlide) {
	
	if( newSlide == NULL )
		return SVIFALSE;

	SVIMutex::SVIAutolock lock(mLock);

	SVIBool ret = SVIFALSE;
	SVIInt count = mSubSlideList.size();

	if( index < 0 || index > count ) {
		LOGE("insertSubSlide is failed : because index is out of range.!!!");
		return SVIFALSE;
	}

	if( index == 0 ) {
		if( count == 0 )
			mSubSlideList.push_back(newSlide);
		else
			mSubSlideList.push_front(newSlide);
		ret = SVITRUE;
	} else if( index == count ) {
		mSubSlideList.push_back(newSlide);
		ret = SVITRUE;
	} else {
		BaseSlideListIter iter = findSlideIter(index);
		if( iter != mSubSlideList.end() ) 
			ret = SVITRUE;
		mSubSlideList.insert(iter, newSlide);
	}

	if( ret ){
		newSlide->setParent(this);
	} else {
		LOGE("SVIBaseSlide::insertSubSlide Failed !!!index:%d, newSlide:%x", index, newSlide);
	}

	return ret;
}


SVIBool SVIBaseSlide::replaceSubSlide(SVIBaseSlide* oldSlide, SVIBaseSlide* newSlide) {
	SVIMutex::SVIAutolock lock(mLock);
	if( oldSlide == NULL || newSlide == NULL )
		return SVIFALSE;

	if( oldSlide->getParent() == NULL || oldSlide->getParent() != this  ||
		newSlide->getParent() != NULL) {
		DEBUG_CHECK_LOGE("SVIBaseSlide : oldSlide or newSlide has some problem to replace!!!");
		return SVIFALSE;
	}

	SVIInt index = getSlideIndex(oldSlide);
	if( index < 0 ) {
		DEBUG_CHECK_LOGE("SVIBaseSlide : can not find out oldSlide in subSlides!!!");
		return SVIFALSE;
	}

	if( !insertSubSlide(index, newSlide) ) {
		DEBUG_CHECK_LOGE("SVIBaseSlide : failed to insert newSlide!!!");
		return SVIFALSE;
	}

	if( !removeSubSlide(oldSlide) ) {
		DEBUG_CHECK_LOGE("SVIBaseSlide : failed to remove oldSlide!!!");
		return SVIFALSE;
	}

	return SVITRUE;
}

SVIInt SVIBaseSlide::getSlideIndex(SVIBaseSlide* subSlide) {
	int index = -1;
	BaseSlideList::iterator iter = mSubSlideList.begin();

	SVIBaseSlide* slide = NULL;

	int indexCounter = 0;
	for(iter; iter != mSubSlideList.end(); iter++ ) {
		slide = (SVIBaseSlide*)(*iter);
		if( subSlide == slide ) {
			index = indexCounter;
			break;
		}
		indexCounter++;
	}

	return index;
}

BaseSlideListIter SVIBaseSlide::findSlideIter(SVIBaseSlide* slide) {
	BaseSlideListIter iter = mSubSlideList.begin();

	SVIBaseSlide* temp = NULL;
	for(iter; iter != mSubSlideList.end(); iter++) {
		temp = (SVIBaseSlide*)(*iter);
		if( slide == temp ) {
			break;
		}
	}

	return iter;
}

BaseSlideListIter SVIBaseSlide::findSlideIter(SVIInt index) {
	BaseSlideListIter iter = mSubSlideList.end();
	if( index < 0 || index > mSubSlideList.size() )
		return iter;

	SVIInt counter = 0;

	for(iter = mSubSlideList.begin(); iter != mSubSlideList.end(); iter++) {
		if( counter == index )
			break;
		counter++;
	}

	return iter;
}

void SVIBaseSlide::setRegion(const SVIRect& region) {
	mRegion = region;
	updateGeometryByRegion();
	ADD_CHANGE_PROPERTY_INFO(REGION);
}

SVIRect SVIBaseSlide::getRegion() {
	return mRegion;
}

void SVIBaseSlide::setBounds(const SVIRect& bounds) {
	mBounds= bounds;
	updateGeometryByBoundsOrPosition();
}

SVIRect SVIBaseSlide::getBounds() {
	return mBounds;
}

void SVIBaseSlide::setPosition(const SVIPoint& position) {
	mPosition = position;
	updateGeometryByBoundsOrPosition();
	ADD_CHANGE_PROPERTY_INFO(POSITION);
}
SVIPoint SVIBaseSlide::getPosition() {
	return mPosition;
}

void SVIBaseSlide::setPivotPoint(const SVIPoint& anchorPoint) {
	mPivotPoint = anchorPoint;
	updateGeometryByPivotPoint();
	ADD_CHANGE_PROPERTY_INFO(PIVOT_POINT);
}

SVIPoint	SVIBaseSlide::getPivotPoint() {
	return mPivotPoint;
}

void SVIBaseSlide::setZPosition(const SVIFloat& zPosition) {
	mZPosition = zPosition;
	ADD_CHANGE_PROPERTY_INFO(ZPOSITION);
}

SVIFloat SVIBaseSlide::getZPosition() {
	return mZPosition;
}


void SVIBaseSlide::setDeformRatio(const SVIVector3& ratio){
	mDeformRatio = ratio;
	ADD_CHANGE_PROPERTY_INFO(DEFORM_RATIO);
}

SVIVector3 SVIBaseSlide::getDeformRatio(){
	return mDeformRatio;
}


void SVIBaseSlide::setSwipeRatio(const SVIVector3& ratio){
	mSwipeRatio = ratio;
	ADD_CHANGE_PROPERTY_INFO(SWIPE_RATIO);
}

SVIVector3 SVIBaseSlide::getSwipeRatio(){
	return mSwipeRatio;
}


void SVIBaseSlide::setRTERatio(const SVIVector3& ratio){
	mRTERatio = ratio;
	ADD_CHANGE_PROPERTY_INFO(RTE_RATIO);
}

SVIVector3 SVIBaseSlide::getRTERatio(){
	return mRTERatio;
}




void SVIBaseSlide::setRotation(const SVIVector4& rotation) {
	mRotation = rotation;
	ADD_CHANGE_PROPERTY_INFO(ROTATION);
}

SVIVector4 SVIBaseSlide::getRotation() {
	return mRotation;
}

void SVIBaseSlide::setScale(const SVIVector4& scale) {
	mScale = scale;
	ADD_CHANGE_PROPERTY_INFO(SCALE);
}

SVIVector3 SVIBaseSlide::getScale() {
	return mScale;
}

void SVIBaseSlide::setBgColor(const SVIColor& color) {
	mBgColor = color;
	//2011-08-08 masterkeaton27@gmail.com
	//mOpacity = mBgColor.mA;
	ADD_CHANGE_PROPERTY_INFO(BGCOLOR);
}

SVIColor SVIBaseSlide::getBgColor() {
	return mBgColor;
}

void SVIBaseSlide::setClipSubSlide(SVIBool clipSubSlide) {
	mClipSubSlide = clipSubSlide;
	ADD_CHANGE_PROPERTY_INFO(CLIP_SUBSLIDE);
}

SVIBool SVIBaseSlide::getClipSubSlide() {
	return mClipSubSlide;
}

void SVIBaseSlide::setOpacity(SVIFloat opacity) {
	mOpacity = opacity;
	//2011-08-08 masterkeaton27@gmail.com
	//mBgColor.mA = mOpacity;
	ADD_CHANGE_PROPERTY_INFO(OPACITY);
}

SVIFloat SVIBaseSlide::getOpacity() {
	return mOpacity;
}

void SVIBaseSlide::setHidden(SVIBool hidden) {
	mHidden = hidden;
	ADD_CHANGE_PROPERTY_INFO(HIDDEN);
}

SVIBool SVIBaseSlide::getHidden() {
	return mHidden;
}

void SVIBaseSlide::setHoldOpacity(SVIBool hold) {
	mHoldOpacity = hold;
	ADD_CHANGE_PROPERTY_INFO(HOLD_OPACITY);
}

SVIBool SVIBaseSlide::getHoldOpacity() {
	return mHoldOpacity;
}


void SVIBaseSlide::setHoldScale(SVIBool hold){
	mHoldScale = hold;
	ADD_CHANGE_PROPERTY_INFO(HOLD_SCALE);
}


SVIBool SVIBaseSlide::getHoldScale() {
	return mHoldScale;
}


void SVIBaseSlide::setCapture(SVIBool capture){
	mCapture = capture;
	ADD_CHANGE_PROPERTY_INFO(SET_CAPTURE);
}

SVIBool SVIBaseSlide::getCapture() {
	return mCapture;
}

void SVIBaseSlide::setCornerRadius(SVIFloat radius) {
	mCornerRadius = radius;
	ADD_CHANGE_PROPERTY_INFO(CORNER_RADIUS);
}

SVIFloat SVIBaseSlide::getCornerRadius() {
	return mCornerRadius;
}

void SVIBaseSlide::setBorderWidth(SVIFloat width) {
	mBorderWidth = width;
	ADD_CHANGE_PROPERTY_INFO(BORDER_WIDTH);
}

SVIFloat SVIBaseSlide::getBorderWidth() {
	return mBorderWidth;
}

void SVIBaseSlide::setBorderColor(const SVIColor& color) {
	mBorderColor = color;
	ADD_CHANGE_PROPERTY_INFO(BORDER_COLOR);
}

SVIColor SVIBaseSlide::getBorderColor() {
	return mBorderColor;
}

void SVIBaseSlide::setShadowColor(const SVIColor & color) {
	mShadowColor = color;
	ADD_CHANGE_PROPERTY_INFO(SHADOW_COLOR);
}

SVIColor SVIBaseSlide::getShadowColor() {
	return mShadowColor;
}

void SVIBaseSlide::setShadowRadius(SVIFloat radius) {
	mShadowRadius = radius;
	ADD_CHANGE_PROPERTY_INFO(SHADOW_RADIUS);
}

SVIFloat SVIBaseSlide::getShadowRadius() {
	return mShadowRadius;
}

void SVIBaseSlide::setShadowOffset(const SVIPoint& offset) {
	mShadowOffset = offset;
	//LOGE("SVIBaseSlide::setShadowOffset %.02f %.02f", offset.mX, offset.mY);
	ADD_CHANGE_PROPERTY_INFO(SHADOW_OFFSET);
}

SVIPoint SVIBaseSlide::getShadowOffset() {
	return mShadowOffset;
}

void SVIBaseSlide::setShadowOpacity(const SVIFloat& opacity) {
	mShadowOpacity = opacity;
	ADD_CHANGE_PROPERTY_INFO(SHADOW_OPACITY);
}

SVIFloat SVIBaseSlide::getShadowOpacity() {
	return mShadowOpacity;
}

void SVIBaseSlide::setLightOpacity(const SVIFloat& opacity){
	mLightOpacity = opacity;
	ADD_CHANGE_PROPERTY_INFO(LIGHT_OPACITY);
}

SVIFloat	SVIBaseSlide::getLightOpacity(){
	return mLightOpacity;
}

void SVIBaseSlide::setLightColor(const SVIColor& color){
	mLightColor = color;
	ADD_CHANGE_PROPERTY_INFO(LIGHT_COLOR);
}


SVIColor	SVIBaseSlide::getLightColor(){
    return mLightColor;
}

void SVIBaseSlide::setLightDirection(const SVIVector4& direction){
    mLightDirection = direction;
    ADD_CHANGE_PROPERTY_INFO(LIGHT_DIRECTION);
}

SVIVector4 SVIBaseSlide::getLightDirection(){
    return mLightDirection;
}


void SVIBaseSlide::setViewDirection(const SVIVector4& direction){
	mViewDirection = direction;
	ADD_CHANGE_PROPERTY_INFO(VIEW_DIRECTION);
}

SVIVector4 SVIBaseSlide::getViewDirection(){
	return mViewDirection;
}

void SVIBaseSlide::setLightOffset(const SVIPoint& offset){
	mLightOffset = offset;
	ADD_CHANGE_PROPERTY_INFO(LIGHT_OFFSET);
}

SVIPoint	SVIBaseSlide::getLightOffset(){
	return mLightOffset;
}

void SVIBaseSlide::setLightType(const SVIInt& type){
	mLightType = type;
	ADD_CHANGE_PROPERTY_INFO(LIGHT_TYPE);
} 

SVIInt SVIBaseSlide::getLightType(){
	return mLightType;
}

void SVIBaseSlide::setLightRadius(const SVIFloat& radius){
	mLightRadius = radius;
	ADD_CHANGE_PROPERTY_INFO(LIGHT_RADIUS);
}

SVIFloat	SVIBaseSlide::getLightRadius(){
	return mLightRadius;
}

void SVIBaseSlide::setLightAngle(const SVIFloat& angle){
	mLightAngle = angle;
	ADD_CHANGE_PROPERTY_INFO(LIGHT_ANGLE);
}

SVIFloat	SVIBaseSlide::getLightAngle(){
	return mLightAngle;
}

//2011-10-14 masterkeaton27@gmail.com
void SVIBaseSlide::setLightPower(const SVIFloat& power){
	mLightPower = power;
	ADD_CHANGE_PROPERTY_INFO(LIGHT_POWER);
}

SVIFloat	SVIBaseSlide::getLightPower(){
	return mLightPower;
}

//2011-08-29 masterkeaton27@gmail.com
void SVIBaseSlide::setTextureRegion(const SVIRect& region){
	mTextureSpriteRegion = region;
	ADD_CHANGE_PROPERTY_INFO(TEXTURE_REGION);
}

void SVIBaseSlide::setBackFaceTextureRegion(const SVIRect& region){
	mTextureBackFaceSpriteRegion = region;
	ADD_CHANGE_PROPERTY_INFO(BACKFACE_TEXTURE_REGION);
}

SVIRect	SVIBaseSlide::getBackFaceTextureRegion(){
	return mTextureBackFaceSpriteRegion;
}

SVIRect	SVIBaseSlide::getTextureRegion(){
	return mTextureSpriteRegion;
}

//2011-10-11 masterkeaton27@gmail.com
void	SVIBaseSlide::setTextureScaleFitRegion(const SVIRect& region){
	mTextureScaleFitRegion = region;
	ADD_CHANGE_PROPERTY_INFO(TEXTURE_SCALEFIT_REGION);
}
SVIRect	SVIBaseSlide::getTextureScaleFitRegion(){
	return mTextureScaleFitRegion;
}

//2012-02-07 masterkeaton27@gmail.com
void SVIBaseSlide::setDeformDirection(SlideDeformDirection direction) {
	mDeformDirection = direction;
	ADD_CHANGE_PROPERTY_INFO(DEFORM_DIRECTION);
}

SlideDeformDirection SVIBaseSlide::getDeformDirection() {
	return mDeformDirection;
}

void SVIBaseSlide::setSwipeType(SlideSwipeType type) {
	mSwipeType = type;
	ADD_CHANGE_PROPERTY_INFO(SWIPE_TYPE);
}

SlideSwipeType SVIBaseSlide::getSwipeType() {
	return mSwipeType;
}

void SVIBaseSlide::setDeformType(SlideDeformType type) {
	mDeformType = type;
	ADD_CHANGE_PROPERTY_INFO(DEFORM_TYPE);
}

SlideDeformType SVIBaseSlide::getDeformType() {
	return mDeformType;
}

void SVIBaseSlide::setMorphingTime(const SVIFloat &aTime)
{
    mMorphingTime = aTime;
    ADD_CHANGE_PROPERTY_INFO(MORPHING_TIME);
}

SVIFloat SVIBaseSlide::getMorphingTime()
{
    return mMorphingTime;
}

void SVIBaseSlide::setMorphingRadius(const SVIFloat radius)
{
    mMorphingRadius = radius;
    ADD_CHANGE_PROPERTY_INFO(MORPHING_RADIUS);
}

SVIFloat SVIBaseSlide::getMorphingRadius()
{
    return mMorphingRadius;
}

void SVIBaseSlide::setMorphingVertexCount(SVIInt vertexCount)
{
    mMorphingVertexCount = vertexCount;
    ADD_CHANGE_PROPERTY_INFO(MORPHING_VERTEX_COUNT);
}

SVIInt SVIBaseSlide::getMorphingVertexCount()
{
    return mMorphingVertexCount;
}

void SVIBaseSlide::setMorphingColor(SVIColor morphingColor)
{
    mMorphingColor = morphingColor;
    ADD_CHANGE_PROPERTY_INFO(MORPHING_COLOR);
}

SVIColor SVIBaseSlide::getMorphingColor()
{
    return mMorphingColor;
}

SVIBool SVIBaseSlide::getTesselation()
{
    return mTesselation;
}

void SVIBaseSlide::setTesselationPointList(const std::vector<SVIVector2>& pointList)
{
    mTesselation = SVITRUE;
    mTesselationPointList = pointList;
    ADD_CHANGE_PROPERTY_INFO(TESSELATION);
}

std::vector<SVIVector2>& SVIBaseSlide::getTesselationPointList()
{
    return mTesselationPointList;
}

void SVIBaseSlide::setMorphingType(SlideMorphingType type)
{
    mMorphingType = type;
    ADD_CHANGE_PROPERTY_INFO(MORPHING_TYPE);
}

SlideMorphingType SVIBaseSlide::getMorphingType()
{
    return mMorphingType;
}

void SVIBaseSlide::setZOrderType(SlideZOrderType type) {
	mZOrderType = type;
	ADD_CHANGE_PROPERTY_INFO(ZORDER_TYPE);
}

SlideZOrderType SVIBaseSlide::getZOrderType() {
	return mZOrderType;
}

//2011-08-01 masterkeaton27@gmail.com
void SVIBaseSlide::setBlendType(SlideBlendType type){
	mBlendType = type;
	ADD_CHANGE_PROPERTY_INFO(BLEND_TYPE);
}
SlideBlendType SVIBaseSlide::getBlendType(){
	return mBlendType;
}

//2011.12.09 jongchae.moon
void SVIBaseSlide::setAntiAliasing(SVIBool enableAA){
	mEnableAA = enableAA;
	ADD_CHANGE_PROPERTY_INFO(ANTI_ALIASING);
}

SVIBool SVIBaseSlide::getAntiAliasing(){
	return mEnableAA;
}
//


//2011-11-03 masterkeaton27@gmail.com
void SVIBaseSlide::setRTEffectType(SlideRTEffectType type){
	mRTEffectType = type;
	ADD_CHANGE_PROPERTY_INFO(RT_EEFECT_TYPE);
}

SlideRTEffectType	SVIBaseSlide::getRTEffectType(){
	return mRTEffectType;
}

//2011-07-01 masterkeaton27@gmail.com
void SVIBaseSlide::setImageScaleType(SlideImageScaleType type){
	mImageScaleType = type;
	ADD_CHANGE_PROPERTY_INFO(IMAGESCALE_TYPE);
} 

SlideImageScaleType SVIBaseSlide::getImageScaleType(){
	return mImageScaleType;
}

void SVIBaseSlide::setOrthogonal(SVIBool isOrtho){
	mOrthogonal = isOrtho;
	ADD_CHANGE_PROPERTY_INFO(ORTHOGONAL);
}

SVIBool SVIBaseSlide::getOrthogonal(){
	return mOrthogonal;
}

//2012-11-26 masterkeaton27@gmail.com
void SVIBaseSlide::setUseNormalMap(SVIBool useNormapMap){
    mUseNormalMap = useNormapMap;
    ADD_CHANGE_PROPERTY_INFO(USENORMALMAP);
}

SVIBool SVIBaseSlide::getUseNormalMap(){
    return mUseNormalMap;
}

//2012-01-03 phyuzion@gmail.com
void SVIBaseSlide::setCustomRTE(SVIBool isCustomRTE){
	mCustomRTE = isCustomRTE;
	ADD_CHANGE_PROPERTY_INFO(CUSTOM_RTE);
}

SVIBool SVIBaseSlide::getCustomRTE(){
	return mCustomRTE;
}


// 2011/11/30 rareboy0112@facebook.com
void SVIBaseSlide::setModelPosition(const SVIVector3 &position) {
	mModelPosition = position;
	ADD_CHANGE_PROPERTY_INFO(MODEL_POSITION);
}


SVIVector3 SVIBaseSlide::getModelPosition() {
	return mModelPosition;
}


void SVIBaseSlide::setModelRotation(const SVIVector4& rotation) {
	mModelRotation = rotation;

	ADD_CHANGE_PROPERTY_INFO(MODEL_ROTATION);
}


SVIVector4 SVIBaseSlide::getModelRotation() {
	return mModelRotation;
}


void SVIBaseSlide::setModelScale(const SVIVector3 &scale) {
	mModelScale = scale;
	ADD_CHANGE_PROPERTY_INFO(MODEL_SCALE);
}


SVIVector3 SVIBaseSlide::getModelScale() {
	return mModelScale;
}


SVIGLSurface* SVIBaseSlide::getSVIGLSurface() {
	return mSVIGLSurface;
}

void SVIBaseSlide::addParticleEffect(SVIParticleEffect* effect) {
	if(effect == NULL) {
		return;
	}

	mParticleEffectList.push_back(effect);
	ADD_CHANGE_PROPERTY_INFO(PARTICLE_EFFECT);
}

//2013-06-26 masterkeaton27@gmail.com
void SVIBaseSlide::setFilterType(SlideFilterType type) {
    mFilterType = type;
    ADD_CHANGE_PROPERTY_INFO(FILTER_TYPE);
}

SlideFilterType SVIBaseSlide::getFilterType(){
    return mFilterType;
}


} // end of SVI namespace
