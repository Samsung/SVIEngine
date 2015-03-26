#include "../../SVICores.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"

#include "SVIPoly.h"

namespace SVI {

//2011-06-15 masterkeaton27@gmail.com
//perspective test
const float cfTestDepth = 1.0f;

static const SVIBool DEBUG = SVIFALSE;

SVIPoly::SVIPoly(SVIGLSurface* saGLSurface) :
		mMemoryUsed(0), mVertices(NULL), mUnitCoords(NULL), mNormals(NULL), mIndices(NULL), mTextureCoords(
				NULL), mHasBorder(SVIFALSE), mMemoryFreed(SVIFALSE), mNeedToUpdate(
				SVITRUE), mIsOrthogonal(SVIFALSE), mIsCustomRTE(SVIFALSE), mNeedToUpdateTexcoord(
				SVITRUE), mNeedTexcoords(SVIFALSE), mNeedIndices(SVIFALSE), mNeedNormals(
				SVIFALSE), mIndicesCount(0), mNormalCount(0), mHasAdditionalRender(
				SVIFALSE), mHasAdditionalUpdate(SVIFALSE), mHasBackFace(SVIFALSE), mNeedMorphing(SVIFALSE),
				mPivot(0.5f, 0.5f, 0.0f), //default pivot is center.
		mScale(1.0f, 1.0f, 1.0f), mCenter(0.0f, 0.0f, 0.0f), mTextureRegionOffset(
				0.0f, 0.0f), mTextureRegionSize(1.0f, 1.0f), mBackFaceTextureRegionOffset(
				0.0f, 0.0f), mBackFaceTextureRegionSize(1.0f, 1.0f), mTextureScaleFitOffset(
				0.0f, 0.0f), mTextureScaleFitSize(1.0f, 1.0f), mColor(0.0, 0.0,
				0.0, 1.0), mRenderPritiveType(SVI_POLY_TRI_FAN), mRenderGeometryType(
				0) {
	mTransform.setIdentity();


    mIsUsingMultiTexture = SVIFALSE;

	//2011.12.07 jongchae.moon
	mIsAA = SVIFALSE;
	mAACoord = NULL;
	mRenderPatch = NULL;
	mIsAddBlend = SVIFALSE;
	mOpacity =1.0f;
	mOriIndicesCount =0;
	mVerticeCount =0;
	mfOriginBorderWidth = 0.0f;

	if(saGLSurface == NULL) {
		LOGE("Invalid argument to the SVIPoly(SVIGLSurface *saGLSurface) \n");
		exit(0); // To check for errors till stabilization.. To be removed later. 
	} else {
		mSVIGLSurface = saGLSurface;
	}
	//
}


//getter
const SVIVector4& SVIPoly::getColor() {
	return mColor;
}

SVIUShort* SVIPoly::getIndices() {
	return mIndices;
}

SVIVector3 * SVIPoly::getVertices() {
	return mVertices;
}

SVIVector3 * SVIPoly::getNormals() {
	return mNormals;
}

SVIVector2 * SVIPoly::getUnitCoords() {
	return mUnitCoords;
}

SVIVector2 * SVIPoly::getTextureCoordinates() {
	return mTextureCoords;
}

const SVIVector2& SVIPoly::getTextureScale() {
	return mTextureScale;
}

const SVIVector2& SVIPoly::getTextureRegionOffset() {
	return mTextureRegionOffset;
}
		
const SVIVector2& SVIPoly::getTextureRegionSize() {
	return mTextureRegionSize;
}

const SVIVector2& SVIPoly::getBackFaceTextureRegionOffset() {
	return mBackFaceTextureRegionOffset;	
	
}

const SVIVector2& SVIPoly::getBackFaceTextureRegionSize() {
	return mBackFaceTextureRegionSize;
}

const SVIVector2& SVIPoly::getTextureScaleFitRegionOffset() {
	return mTextureScaleFitOffset;
}
		
const SVIVector2&SVIPoly:: getTextureScaleFitRegionSize() {
	return mTextureScaleFitSize;
}

//2011.12.09 jongchae.moon
SVIBool SVIPoly::getIsAntiAliasing() {
	return mIsAA;
}

SVIInt SVIPoly::getOriginalVerticeCount() {
	return mOriVerticeCount;
}

SVIInt SVIPoly::getOriginalIndiceCount() {
	return mOriIndicesCount;
}

SVIFloat* SVIPoly::getAAcoords() {
	return mAACoord;
}

const SVIVector3& SVIPoly::getPivot() {
	return mPivot;
}

const SVIVector3& SVIPoly::getScale() {
	return mScale;
}

const SVIFloat& SVIPoly::getOpacity() {
	return mOpacity;
}

const SVIVector3& SVIPoly::getSize() {
	return mSize;
}

const SVIMatrix& SVIPoly::getTransform() {
	return mTransform;
}

SVIUInt SVIPoly::getVerticeCount() {
	return mVerticeCount;
}

SVIUInt SVIPoly::getIndicesCount() {
	return mIndicesCount;
}

SVIUInt SVIPoly::getPrimitiveType() {
	return mRenderPritiveType;
}

SVIUInt SVIPoly::getGeometryType() {
	return mRenderGeometryType;
}

SVIUInt SVIPoly::getMemoryUsed() {
	return mMemoryUsed;
}

SVIBool SVIPoly::isFreed() {
	return mMemoryFreed;
}

SVIBool SVIPoly::isOrthogonal() {
	return mIsOrthogonal;
}

SVIBool SVIPoly::isCustomRTE() {
	return mIsCustomRTE;
}

SVIBool SVIPoly::isUsingMultiTexture(){
    return mIsUsingMultiTexture;
}

SVIBool SVIPoly::isAddBlend() {
	return mIsAddBlend;
}

SVIBool SVIPoly::hasBorder() {
	return mHasBorder;
}
	
SVIBool SVIPoly::hasAdditionalRender() {
	return mHasAdditionalRender;
}

SVIBool SVIPoly::hasAdditionalUpdate() {
	return mHasAdditionalUpdate;
}

SVIBool SVIPoly::hasIndices() {
	return mNeedIndices;
}

//2011-11-08 masterkeaton27@gmail.com
SVIBool SVIPoly::hasNormals() {
	return mNeedNormals;
}

//2011-11-16 masterkeaton27@gmail.com
SVIBool SVIPoly::hasTexcoords() {
	return mNeedTexcoords;
}

//2011-11-17 masterkeaton27@gmail.com
SVIBool SVIPoly::hasBackFace() {
	return mHasBackFace;
}

//2012/12/10 rareboy0112@facebook.com
SVIBool SVIPoly::hasMorphing()
{
    return mNeedMorphing;
}

void SVIPoly::resetMorphing()
{
    mNeedMorphing = SVIFALSE;
}

void SVIPoly::setTextureScaleFitRegion(const SVIVector2& offset,
		const SVIVector2& size) {
	if (offset != mTextureScaleFitOffset || size != mTextureScaleFitSize) {
		mTextureScaleFitOffset = offset;
		mTextureScaleFitSize = size;
		mNeedToUpdate = SVITRUE;
		mNeedToUpdateTexcoord = SVITRUE;
	}
}

void SVIPoly::setTextureRegion(const SVIVector2& offset, const SVIVector2& size) {
	if (offset != mTextureRegionOffset || size != mTextureRegionSize) {
		mTextureRegionOffset = offset;
		mTextureRegionSize = size;
		mNeedToUpdate = SVITRUE;
		mNeedToUpdateTexcoord = SVITRUE;
	}
}

void SVIPoly::setBackFaceTextureRegion(const SVIVector2& offset,
		const SVIVector2& size) {
	if (offset != mBackFaceTextureRegionOffset
			|| size != mBackFaceTextureRegionSize) {
		mBackFaceTextureRegionOffset = offset;
		mBackFaceTextureRegionSize = size;
		mNeedToUpdate = SVITRUE;
		mNeedToUpdateTexcoord = SVITRUE;
	}
}

void SVIPoly::setTextureScale(const SVIVector2 & scale) {
	if (scale != mTextureScale) {
		mTextureScale = scale;
		mNeedToUpdate = SVITRUE;
		mNeedToUpdateTexcoord = SVITRUE;
	}
}

void SVIPoly::setOrthogonal(SVIBool isOrtho) {
	mIsOrthogonal = isOrtho;
}

void SVIPoly::setCustomRTE(SVIBool isCusRTE) {
	mIsCustomRTE = isCusRTE;
}

void SVIPoly::setAddBlend(SVIBool bisAddBlend) {
	mIsAddBlend = bisAddBlend;
}

void SVIPoly::setBorder(SVIBool border) {
	mHasBorder = border;
}

//2012-02-21 marx.kim
void SVIPoly::setRenderPatch(SVIRenderPatch* patch) {
	mRenderPatch = patch;
}

void SVIPoly::setGeometryType(SVIUInt type) {
	mRenderGeometryType = type;
}

void SVIPoly::setPrimitiveType(SVIUInt type) {
	mRenderPritiveType = type;
}

void SVIPoly::setColor(const SVIVector4 & color) {
	mColor = color;
}

void SVIPoly::setOpacity(const SVIFloat & opacity) {
	mOpacity = opacity;
}

void SVIPoly::setPivot(const SVIVector3 & pivot) {
	if (pivot != mPivot) {
		mPivot = pivot;
		if (isValid()) {
			mNeedToUpdate = SVITRUE;
			mNeedToUpdateTexcoord = SVITRUE;
		}
	}
}

void SVIPoly::setSize(const SVIVector3 & size) {
	if (size != mSize) {
		mSize = size;

		//2011-07-20 masterkeaton27@gmail.com
		if (isValid()) {
			mNeedToUpdate = SVITRUE;
			mNeedToUpdateTexcoord = SVITRUE;
		}
	}
}

void SVIPoly::setScale(const SVIVector3 & scale) {
	if (scale != mScale) {
		mScale = scale;

		//2011-07-20 masterkeaton27@gmail.com
		if (isValid()) {
			mNeedToUpdate = SVITRUE;
			mNeedToUpdateTexcoord = SVITRUE;
		}
	}
}

void SVIPoly::setTransform(const SVIMatrix & transform) {
	if (transform != mTransform) {
		mTransform = transform;
		mNeedToUpdate = SVITRUE;
	}
}

SVIPoly::~SVIPoly() {
	deallocate();
	mSVIGLSurface = NULL;
}

//2011-07-20 masterkeaton27@gmail.com
//check zero area triangle error.
SVIBool SVIPoly::isValid() {
	if (mSize.x == 0.0f || mSize.y == 0.0f || mScale.x == 0.0f
			|| mScale.y == 0.0f) {
		return SVIFALSE;
	}
	return SVITRUE;
}

SVIBool SVIPoly::buildCenter() {
	SVIVector3 center;
	for (SVIUInt n = 0; n < mVerticeCount; ++n) {
		center += mVertices[n];
	}
	mCenter = center * (1.0f / (float) mVerticeCount);

    return SVITRUE;
}

SVIBool SVIPoly::build() {
	if (isFreed())
		return SVIFALSE;
	if (mNeedToUpdate) {
		setup();
		buildVertices();

		if (mNeedToUpdateTexcoord && mNeedTexcoords) {
			buildTextureCoordinates();
		}

		buildExtraWork();

	}
	return SVITRUE;
}

const SVIVector3& SVIPoly::getCenter() {
	return mCenter;
}

SVIBool SVIPoly::isScaleToFit() {
	return (mTextureScaleFitOffset.x != 0.0f || mTextureScaleFitOffset.y != 0.0f
			|| mTextureScaleFitSize.x != 1.0f || mTextureScaleFitSize.y != 1.0f) ?
			SVITRUE : SVIFALSE;
}

SVIInt SVIPoly::deallocate() {
	if (isFreed())
		return 0;
	
	SVI_SVIFE_DELETE_ARRAY(mVertices);
	SVI_SVIFE_DELETE_ARRAY(mTextureCoords);
	SVI_SVIFE_DELETE_ARRAY(mIndices);
	SVI_SVIFE_DELETE_ARRAY(mNormals);

	//2011.12.07 jongchae.moon
	SVI_SVIFE_DELETE_ARRAY(mAACoord);
	//

	int tempUsed = mMemoryUsed;
	mMemoryUsed = 0;
	mMemoryFreed = SVITRUE;
	mVerticeCount = 0;
	mIndicesCount = 0;
	mNormalCount = 0;
	return tempUsed;
}

SVIInt SVIPoly::allocate(SVIInt verticeCount, SVIInt indicesCount, SVIInt normalCount) {
	//2011-06-27 masterkeaton27@gmail.com
	//memory optimization.
	//this block is a quite good enough solution for reduce allocation.
	if (
        mVerticeCount == (SVIUInt)verticeCount && mIndicesCount == (SVIUInt)indicesCount && 
        mNormalCount == (SVIUInt)normalCount && mVertices != NULL && 
        mNormals != NULL && mTextureCoords != NULL && mIndices != NULL)
        return mMemoryUsed;

	//2011-05-24 masterkeaton27@gmail.com
	//following routine should be checked for valid memory allocation
	mVertices = new SVIVector3[verticeCount];
	mNormals = new SVIVector3[verticeCount];
	mUnitCoords= new SVIVector2[verticeCount];
	mTextureCoords = new SVIVector2[verticeCount];
	mIndices = new SVIUShort[indicesCount];
	mVerticeCount = verticeCount;
	mIndicesCount = indicesCount;
	mNormalCount = normalCount;

	mOriVerticeCount = verticeCount;
	mOriIndicesCount = indicesCount;

	mMemoryUsed += sizeof(SVIVector3) * verticeCount;
	mMemoryUsed += sizeof(SVIVector3) * normalCount;
	mMemoryUsed += sizeof(SVIVector2) * verticeCount;
	mMemoryUsed += sizeof(SVIVector2) * verticeCount;
	mMemoryUsed += sizeof(SVIUShort) * indicesCount;

	mNeedIndices = SVITRUE;
	mNeedNormals = SVITRUE;

    //2012/12/10 rareboy0112@facebook.com
    //need data
    mAACoord = new SVIFloat[verticeCount];
    for (int i = 0; i < verticeCount; i++) {
        mAACoord[i] = 1.0f;
    }

	return mMemoryUsed;
}

SVIInt SVIPoly::allocate(SVIInt verticeCount, SVIInt indicesCount) {
	//2011-06-27 masterkeaton27@gmail.com
	//memory optimization.
	//this block is a quite good enough solution for reduce allocation.
	if (
        mVerticeCount == (SVIUInt)verticeCount && mIndicesCount == (SVIUInt)indicesCount && 
        mVertices != NULL &&
#if SVI_USING_CPU_TEXCOORD
			mTextureCoords != NULL &&
#endif
			mIndices != NULL)
		return mMemoryUsed;

	//2011-05-24 masterkeaton27@gmail.com
	//following routine should be checked for valid memory allocation
	mVertices = new SVIVector3[verticeCount];
	mUnitCoords= new SVIVector2[verticeCount];
#if SVI_USING_CPU_TEXCOORD
	mTextureCoords = new SVIVector2[verticeCount];
#endif
	mIndices = new SVIUShort[indicesCount];

	//2011.12.07 jongchae.moon
	mAACoord = new SVIFloat[verticeCount];
	for (int i = 0; i < verticeCount; i++) {
		mAACoord[i] = 1.0f;
	}
	mOriVerticeCount = verticeCount;
	mOriIndicesCount = indicesCount;
	//

	mVerticeCount = verticeCount;
	mIndicesCount = indicesCount;

	mMemoryUsed += sizeof(SVIVector3) * verticeCount;
	mMemoryUsed += sizeof(SVIVector2) * verticeCount;
#if SVI_USING_CPU_TEXCOORD
	mMemoryUsed += sizeof(SVIVector2) * verticeCount;
#endif
	mMemoryUsed += sizeof(SVIUShort) * indicesCount;

	//2011.12.07 jongchae.moon
	mMemoryUsed += sizeof(SVIFloat) * verticeCount;
	//

	mNeedIndices = SVITRUE;

	return mMemoryUsed;
}

SVIInt SVIPoly::allocate(SVIInt verticeCount) {
	//2011-06-27 masterkeaton27@gmail.com
	//memory optimization.
	//this block is a quite good enough solution for reduce allocation.
#if SVI_USING_CPU_TEXCOORD
	if (
        mVerticeCount == verticeCount &&
        mVertices != NULL &&
        mTextureCoords != NULL) 
        return mMemoryUsed;
#else
	if (mVerticeCount == (SVIUInt)verticeCount && mVertices != NULL)
		return mMemoryUsed;
#endif

	//2011-05-24 masterkeaton27@gmail.com
	//following routine should be checked for valid memory allocation
	mVertices = new SVIVector3[verticeCount];
	mUnitCoords= new SVIVector2[verticeCount];
#if SVI_USING_CPU_TEXCOORD
	mTextureCoords = new SVIVector2[verticeCount];
#endif
	//2011.12.07 jongchae.moon
	mAACoord = new SVIFloat[verticeCount];
	for (int i = 0; i < verticeCount; i++) {
		mAACoord[i] = 1.0f;
	}
	mOriVerticeCount = verticeCount;
	//

	mVerticeCount = verticeCount;

	mMemoryUsed += sizeof(SVIVector3) * verticeCount;
	mMemoryUsed += sizeof(SVIVector2) * verticeCount;
#if SVI_USING_CPU_TEXCOORD
	mMemoryUsed += sizeof(SVIVector2) * verticeCount;
#endif

	//2011.12.07 jongchae.moon
	mMemoryUsed += sizeof(SVIFloat) * verticeCount;
	//

	mNeedIndices = SVIFALSE;
	mIndices = NULL;

	return mMemoryUsed;
}

//virtual override functions
void SVIPoly::buildVertices() {}
void SVIPoly::buildTextureCoordinates() {}
void SVIPoly::buildIndices() {}

//2011-11-09 masterkeaton27@gmail.com
void SVIPoly::buildNormals() {}

//2011-07-12 masterkeaton27@gmail.com
void SVIPoly::buildExtraWork() {}


}
