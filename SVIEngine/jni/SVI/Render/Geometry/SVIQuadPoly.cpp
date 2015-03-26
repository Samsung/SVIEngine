#include "../../SVICores.h"

#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVITexture.h"
#include "../SVITextureManager.h"
#include "../SVIProgramManager.h"
#include "../SVIViewport.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"


#include "SVIPoly.h"
#include "SVIQuadPoly.h"

namespace SVI {
	
const float cfTestDepth = 1.0f;
static const SVIBool DEBUG = SVIFALSE;


/****************************************************************************/
// Default complete quadrangle polygon object
/****************************************************************************/

SVIQuadPoly::SVIQuadPoly(SVIGLSurface * saGLSurface) :
	SVIPoly(saGLSurface){


	mBorderVertices = NULL;
	mBorderTexcoords = NULL;
	mShadowVertices = NULL;
	mHasAdditionalRender = SVITRUE;

	//2011.12.07 jongchae.moon
	mBorderAACoord = NULL;
	//

	mBorderColor = SVIVector4(1.0f, 1.0f, 1.0f, 1.0f);
	mBorderWidth = 0.0f; //10 pixel width
	mBorderVerticeCount = 0;
}

void SVIQuadPoly::setup() {
	//vertices for triangle strip
	allocate(4);
	if (hasBorder() && mShadowVertices == NULL) {
		mShadowVertices = new SVIVector3[4];
	}
	//2011-08-22 masterkeaton27@gmail.com
	//remove useless codes
	//mVertices[SVI_QUAD_LEFT_TOP]		= SVIVector3(1.0f, 1.0f, 0.0f);
	//mVertices[SVI_QUAD_LEFT_BOTTOM]	= SVIVector3(1.0f, 0.0f, 0.0f);
	//mVertices[SVI_QUAD_RIGHT_BOTTOM]	= SVIVector3(0.0f, 0.0f, 0.0f);
	//mVertices[SVI_QUAD_RIGHT_TOP]	= SVIVector3(0.0f, 1.0f, 0.0f);
	mNeedIndices = SVIFALSE;
}

SVIQuadPoly::~SVIQuadPoly() {
	deallocateBorderDatas();
	SVI_SVIFE_DELETE_ARRAY(mShadowVertices);
}

void SVIQuadPoly::buildVertices() {
	if (mVertices == NULL)
		return;

	SVIFloat width = mBorderWidth;

	SVIFloat fXDistance = mSize.x; // * mScale.x;
	SVIFloat fYDistance = mSize.y; // * mScale.y;
	SVIFloat fZDistance = mSize.z; // * mScale.z;

	SVIFloat fXRatio = mPivot.x;
	SVIFloat fYRatio = mPivot.y;
	SVIFloat fZRatio = mPivot.z;

	SVIFloat fInverseXValue = 1.0f - fXRatio;
	SVIFloat fInverseYValue = 1.0f - fYRatio;

	//2011-05-26 ignore z centering
	mVertices[SVI_QUAD_LEFT_TOP].x = fInverseXValue * fXDistance - width;
	mVertices[SVI_QUAD_LEFT_TOP].y = fInverseYValue * fYDistance - width;

	mVertices[SVI_QUAD_RIGHT_BOTTOM].x = -fXRatio * fXDistance + width;
	mVertices[SVI_QUAD_RIGHT_BOTTOM].y = -fYRatio * fYDistance + width;

	mVertices[SVI_QUAD_LEFT_BOTTOM].x = fInverseXValue * fXDistance - width;
	mVertices[SVI_QUAD_LEFT_BOTTOM].y = -fYRatio * fYDistance + width;

	mVertices[SVI_QUAD_RIGHT_TOP].x = -fXRatio * fXDistance + width;
	mVertices[SVI_QUAD_RIGHT_TOP].y = fInverseYValue * fYDistance - width;
	

	mUnitCoords[SVI_QUAD_LEFT_TOP].x = 1;
	mUnitCoords[SVI_QUAD_LEFT_TOP].y = 1;

	mUnitCoords[SVI_QUAD_RIGHT_TOP].x = 0;
	mUnitCoords[SVI_QUAD_RIGHT_TOP].y = 1;
	
	mUnitCoords[SVI_QUAD_RIGHT_BOTTOM].x = 0;
	mUnitCoords[SVI_QUAD_RIGHT_BOTTOM].y = 0;
	
	mUnitCoords[SVI_QUAD_LEFT_BOTTOM].x = 1;
	mUnitCoords[SVI_QUAD_LEFT_BOTTOM].y = 0;

	if (hasBorder()) {
		mShadowVertices[SVI_QUAD_LEFT_TOP].x = fInverseXValue * fXDistance;
		mShadowVertices[SVI_QUAD_LEFT_TOP].y = fInverseYValue * fYDistance;

		mShadowVertices[SVI_QUAD_RIGHT_BOTTOM].x = -fXRatio * fXDistance;
		mShadowVertices[SVI_QUAD_RIGHT_BOTTOM].y = -fYRatio * fYDistance;

		mShadowVertices[SVI_QUAD_LEFT_BOTTOM].x = fInverseXValue * fXDistance;
		mShadowVertices[SVI_QUAD_LEFT_BOTTOM].y = -fYRatio * fYDistance;

		mShadowVertices[SVI_QUAD_RIGHT_TOP].x = -fXRatio * fXDistance;
		mShadowVertices[SVI_QUAD_RIGHT_TOP].y = fInverseYValue * fYDistance;

		borderSetup();
	}
}

void SVIQuadPoly::setBorderWidth(SVIFloat width) {
	if (width > 0.01) {
		setBorder(SVITRUE);
	} else
		setBorder(SVIFALSE);

	if (mBorderWidth != width) {
		mBorderWidth = width;
		mNeedToUpdate = SVITRUE;
		mNeedToUpdateTexcoord = SVITRUE;
	}
}

void SVIQuadPoly::setBorderColor(SVIVector4 color) {
	mBorderColor = color;
}

void SVIQuadPoly::setBorderColor(SVIFloat r, SVIFloat g, SVIFloat b, SVIFloat a) {
	mBorderColor.x = r;
	mBorderColor.y = g;
	mBorderColor.z = b;
	mBorderColor.w = a;
}

void SVIQuadPoly::additionalRender(SVISlideTextureContainer * pContainer) {
	//2011-10-19 masterkeaton27@gmail.com
	if (!hasBorder())
		return;

	SVIProgramParams renderParams;
	pContainer = NULL;
	if (false) {//pContainer != NULL No texture needed on border
		SVITexture * pTexture = pContainer->getTexture();
		if (pTexture != NULL && pTexture->getUploaded()) {
			SVIVector4 colorValue = mBorderColor;
			if (pTexture->getHasAlpha() || colorValue.w < 1.0f
					|| (isAddBlend() && mOpacity < 1.0f)) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			} else {
				glDisable(GL_BLEND);
			}
			renderParams.addElement(
					isScaleToFit() ? EM_TEXTURE_SCALE : EM_TEXTURE);
		}
		renderParams.addElement(isAddBlend() ? EM_BLEND_ADD : EM_BLEND_MLT);
	} else {
		renderParams.addElement(EM_COLOR);
		if (mBorderColor.w < 1.0f) {
			glEnable(GL_BLEND);
			//2011-10-28 masterkeaton27@gmail.com
			//change default blend function to one
			//this is very important issues for some reasons.
			//if some images displayed strange color, should check this function.
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
	}

	SVIProgram * pProgram = mSVIGLSurface->getProgramManager()->getProgram(
			&renderParams);
	SVIProgramHandler * pHandler = NULL;

	if(pProgram){
	pHandler = pProgram->getHandler();
	pProgram->use();
	}

	SVIVector2 texRegionSize = getTextureRegionSize();
	SVIVector2 texRegionOffset = getTextureRegionOffset();
	SVIVector4 texInfo = SVIVector4(texRegionOffset.x, texRegionOffset.y,
			texRegionSize.x, texRegionSize.y);
	pHandler->setUniform(HD_U_TEXTUREINFO, texInfo);

	texRegionSize = getTextureScaleFitRegionSize();
	texRegionOffset = getTextureScaleFitRegionOffset();
	texInfo = SVIVector4(texRegionOffset.x, texRegionOffset.y, texRegionSize.x,
			texRegionSize.y);
	pHandler->setUniform(HD_U_FITINFO, texInfo);

#if SVI_USING_CPU_TEXCOORD	
	pHandler->setAttribute(HD_A_TEXCOORD, mBorderTexcoords);
#else
	SVIVector2 sideSize = SVIVector2(mSize.x, mSize.y);
	SVIVector2 sidePivot = SVIVector2(mPivot.x, mPivot.y);
	pHandler->setUniform(HD_U_SIZE, sideSize);
	pHandler->setUniform(HD_U_PIVOT, sidePivot);
#endif
	pHandler->setAttribute(HD_A_POSITION, mBorderVertices);
	pHandler->releaseAttribute(HD_A_UNITCOORD);
	if (pContainer != NULL && !pContainer->isEmpty())
		pHandler->setSampler(HD_U_SVIMPLER_0, pContainer->getTextureID());
	pHandler->setUniform(HD_U_OPACITY, mOpacity);
	pHandler->setUniform(HD_U_COLOR, mBorderColor);

	//set model view projection matrix
	SVIMatrix modelMat = getTransform();
	//2011-07-12 masterkeaton27@gmail.com
	//orthogonal setting
	SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();
	if (isOrthogonal()) {
		projectionMat._34 = 0.0f;
	}
	//modelMat._34 = 1.0f / -10.0f;
	modelMat._43 -= cfTestDepth;

	pHandler->setUniform(HD_U_PROJ, projectionMat);
	pHandler->setUniform(HD_U_VIEW, modelMat);

	//2011.12.07 jognchae.moon
	if (mBorderAACoord != NULL) {
		pHandler->setAttribute(HD_A_AACOORD, 1, mBorderAACoord);
	}
	//

	//render some
	glDrawArrays(GL_TRIANGLES, 0, mBorderVerticeCount);

	pHandler->releaseAttribute(HD_A_POSITION);
#if SVI_USING_CPU_TEXCOORD	
	pHandler->releaseAttribute(HD_A_TEXCOORD);
#endif
	//2011.12.07 jognchae.moon
	if (mBorderAACoord != NULL) {
		pHandler->releaseAttribute(HD_A_AACOORD);
	}
	//
}

SVIInt SVIQuadPoly::generateBorderRectangle(SVIInt startIndex, SVIFloat x,
		SVIFloat y, SVIFloat w, SVIFloat h) {
	SVIInt index = startIndex;

	//upper triangle
	mBorderVertices[index + 0] = SVIVector3(x, y, 0.0f);
	mBorderVertices[index + 1] = SVIVector3(x, y + h, 0.0f);
	mBorderVertices[index + 2] = SVIVector3(x + w, y, 0.0f);

	//down triangle
	mBorderVertices[index + 3] = SVIVector3(x, y + h, 0.0f);
	mBorderVertices[index + 4] = SVIVector3(x + w, y + h, 0.0f);
	mBorderVertices[index + 5] = SVIVector3(x + w, y, 0.0f);

	return index + 6;
}

void SVIQuadPoly::allocateBorderDatas() {
	mBorderVerticeCount = 6 * 4;
#if SVI_USING_CPU_TEXCOORD
	if (mBorderVertices != NULL && mBorderTexcoords != NULL) return;
#else
	if (mBorderVertices != NULL)
		return;
#endif
	mBorderVertices = new SVIVector3[mBorderVerticeCount];
#if SVI_USING_CPU_TEXCOORD
	mBorderTexcoords = new SVIVector2[mBorderVerticeCount];
	mMemoryUsed += sizeof(SVIVector2) * mBorderVerticeCount;
#endif
	mMemoryUsed += sizeof(SVIVector3) * mBorderVerticeCount;

	//2011.12.07 jongchae.moon
	if (mBorderAACoord == NULL) {
		int cornerCount = 4;
		int nBorderAACoordCount = mBorderVerticeCount;

		mBorderAACoord = new SVIFloat[nBorderAACoordCount];

		for (int i = 0; i < nBorderAACoordCount; i++) {
			mBorderAACoord[i] = 1.0f;
		}
	}
	//
}

void SVIQuadPoly::deallocateBorderDatas() {

	//2011.12.07 jongchae.moon
	SVI_SVIFE_DELETE_ARRAY(mBorderAACoord);
	//
	SVI_SVIFE_DELETE_ARRAY(mBorderVertices);
#if SVI_USING_CPU_TEXCOORD
	SVI_SVIFE_DELETE_ARRAY(mBorderTexcoords);
#endif
	mBorderVerticeCount = 0;
	mMemoryUsed = 0;
}

void SVIQuadPoly::borderSetup() {
	//2011-08-09 masterkeaton27@gmail.com
	if (!hasBorder())
		return;

	allocateBorderDatas();

	//Top border
	SVIFloat width = mBorderWidth;
	SVIFloat x = mVertices[SVI_QUAD_LEFT_TOP].x + width;
	SVIFloat y = mVertices[SVI_QUAD_LEFT_TOP].y;
	SVIFloat w = mVertices[SVI_QUAD_RIGHT_TOP].x - mVertices[SVI_QUAD_LEFT_TOP].x
			- width * 2.0f;
	SVIFloat h = width;
	SVIInt index = generateBorderRectangle(0, x, y, w, h);

	//Left border
	x = mVertices[SVI_QUAD_LEFT_TOP].x;
	y = mVertices[SVI_QUAD_LEFT_TOP].y;
	w = width;
	h = mVertices[SVI_QUAD_LEFT_BOTTOM].y - mVertices[SVI_QUAD_LEFT_TOP].y;
	index = generateBorderRectangle(index, x, y, w, h);

	//Bottom border
	x = mVertices[SVI_QUAD_LEFT_TOP].x + width;
	y = mVertices[SVI_QUAD_LEFT_BOTTOM].y;
	w = mVertices[SVI_QUAD_RIGHT_TOP].x - mVertices[SVI_QUAD_LEFT_TOP].x
			- width * 2.0f;
	h = -width;
	index = generateBorderRectangle(index, x, y, w, h);

	//right border
	x = mVertices[SVI_QUAD_RIGHT_TOP].x;
	y = mVertices[SVI_QUAD_LEFT_TOP].y;
	w = -width;
	h = mVertices[SVI_QUAD_LEFT_BOTTOM].y - mVertices[SVI_QUAD_LEFT_TOP].y;
	index = generateBorderRectangle(index, x, y, w, h);

#if SVI_USING_CPU_TEXCOORD	
	SVIFloat fXDistance = mSize.x; // * mScale.x;
	SVIFloat fYDistance = mSize.y;// * mScale.y;

	SVIFloat fXRatio = mPivot.x * fXDistance;
	SVIFloat fYRatio = mPivot.y * fYDistance;

	for (int n = 0; n < mBorderVerticeCount; ++n) {
		mBorderTexcoords[n].x = (mBorderVertices[n].x+fXRatio) / fXDistance;
		mBorderTexcoords[n].y = (mBorderVertices[n].y+fYRatio) / fYDistance;
	}
#endif
}

void SVIQuadPoly::buildTextureCoordinates() {
#if SVI_USING_CPU_TEXCOORD	
	if (mTextureCoords == NULL) return;
	SVIFloat fXDistance = mSize.x; // * mScale.x;
	SVIFloat fYDistance = mSize.y;// * mScale.y;

	//2011-06-22 texture scale added for flip object
	float fAmountU = fabs(mTextureScale.x);
	float fAmountV = fabs(mTextureScale.y);

	SVIFloat fXRatio = mPivot.x * fXDistance;
	SVIFloat fYRatio = mPivot.y * fYDistance;

	//2011-08-29 masterkeaton27@gmail.com
	//sprite texture added.
	SVIFloat fTOffsetU = 0.0f;//mTextureRegionOffset.x;
	SVIFloat fTOffsetV = 0.0f;//mTextureRegionOffset.y;
	SVIFloat fTScaleU = 1.0f;//mTextureRegionSize.x;
	SVIFloat fTScaleV = 1.0f;//mTextureRegionSize.y;

	for (int n = 0; n < mVerticeCount; ++n) {
		mTextureCoords[n].x = (mVertices[n].x+fXRatio) / fXDistance;
		mTextureCoords[n].y = (mVertices[n].y+fYRatio) / fYDistance;

		if (mTextureScale.x < 0.0f) {
			mTextureCoords[n].x = (1.0f - mTextureCoords[n].x) * fAmountU;
		}
		if (mTextureScale.y < 0.0f) {
			mTextureCoords[n].y = (1.0f - mTextureCoords[n].y) * fAmountV;
		}
		mTextureCoords[n].x = fTOffsetU + mTextureCoords[n].x * fTScaleU;
		mTextureCoords[n].y = fTOffsetV + mTextureCoords[n].y * fTScaleV;
	}

	//2011-06-13 masterkeaton27@gmail.com
	//border vertices should be setup here.
	borderSetup();
#endif
}

/////////////////////////////////////////////////////////
//2011.11.07 jongchae.moon
SVIQuadPolyExt::SVIQuadPolyExt(SVIGLSurface *surface) :
	SVIQuadPoly(surface){
	mIsAA = SVITRUE;
	mRenderPritiveType = SVI_POLY_TRI_LIST;
	mBorderIndices = NULL;
	 mBorderIndicesCount= 0;
	 mBorderOriVerticeCount=0 ;
	 mBorderOriIndicesCount=0 ;
}

SVIQuadPolyExt::~SVIQuadPolyExt() {
}

SVIInt SVIQuadPolyExt::allocate(SVIInt verticeCount) {
	return SVIQuadPolyExt::allocate(verticeCount, 6);
}

SVIInt SVIQuadPolyExt::allocate(SVIInt verticeCount, SVIInt indicesCount) {
	SVIInt ret = SVIQuadPoly::allocate(verticeCount + 4,
			indicesCount + 4 * 2 * 3);

	mOriVerticeCount = mVerticeCount = verticeCount;
	mOriIndicesCount = mIndicesCount = indicesCount;

	mNeedIndices = SVITRUE;

	return ret;
}

void SVIQuadPolyExt::buildVertices() {
	//Initialize index
	mfOriginBorderWidth = mBorderWidth;
	mNeedIndices = SVITRUE;

	const SVIFloat AAWidth = 1.0f;
	mBorderWidth += AAWidth / 2.0f;
	////////////////////////////
	SVIQuadPoly::buildVertices();

	mVertices[SVI_QUAD_LEFT_TOP + 4].x = mVertices[SVI_QUAD_LEFT_TOP].x + AAWidth;
	mVertices[SVI_QUAD_LEFT_TOP + 4].y = mVertices[SVI_QUAD_LEFT_TOP].y + AAWidth;

	mVertices[SVI_QUAD_RIGHT_BOTTOM + 4].x = mVertices[SVI_QUAD_RIGHT_BOTTOM].x
			- AAWidth;
	mVertices[SVI_QUAD_RIGHT_BOTTOM + 4].y = mVertices[SVI_QUAD_RIGHT_BOTTOM].y
			- AAWidth;

	mVertices[SVI_QUAD_LEFT_BOTTOM + 4].x = mVertices[SVI_QUAD_LEFT_BOTTOM].x
			+ AAWidth;
	mVertices[SVI_QUAD_LEFT_BOTTOM + 4].y = mVertices[SVI_QUAD_LEFT_BOTTOM].y
			- AAWidth;

	mVertices[SVI_QUAD_RIGHT_TOP + 4].x = mVertices[SVI_QUAD_RIGHT_TOP].x
			- AAWidth;
	mVertices[SVI_QUAD_RIGHT_TOP + 4].y = mVertices[SVI_QUAD_RIGHT_TOP].y
			+ AAWidth;

	mAACoord[SVI_QUAD_LEFT_TOP + 4] = 0.0f;
	mAACoord[SVI_QUAD_RIGHT_BOTTOM + 4] = 0.0f;
	mAACoord[SVI_QUAD_LEFT_BOTTOM + 4] = 0.0f;
	mAACoord[SVI_QUAD_RIGHT_TOP + 4] = 0.0f;

	// Add indices
	SVIInt index_mIndices = 0;

	// original rect
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_TOP;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP;

	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_BOTTOM;

	// upper patch
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_TOP + 4;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_TOP;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP + 4;

	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP + 4;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_TOP;

	// right patch
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP + 4;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_BOTTOM + 4;

	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_BOTTOM + 4;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_BOTTOM;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_TOP;

	// bottom patch
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_BOTTOM + 4;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_BOTTOM;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM + 4;

	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM + 4;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM;
	mIndices[index_mIndices++] = SVI_QUAD_RIGHT_BOTTOM;

	// left patch
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM + 4;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_TOP + 4;

	mIndices[index_mIndices++] = SVI_QUAD_LEFT_TOP + 4;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_TOP;
	mIndices[index_mIndices++] = SVI_QUAD_LEFT_BOTTOM;

	mIndicesCount = index_mIndices;

	////////////////////////////
	//revert to original data
	mBorderWidth = mfOriginBorderWidth;
}

void SVIQuadPolyExt::additionalRender(SVISlideTextureContainer * pContainer) {
	//2011-10-19 masterkeaton27@gmail.com
	if (!hasBorder())
		return;

	SVIProgramParams renderParams;

	pContainer = NULL;

	if (pContainer != NULL) {
		SVITexture * pTexture = pContainer->getTexture();
		if (pTexture != NULL && pTexture->getUploaded()) {
			SVIVector4 colorValue = mBorderColor;
			if (pTexture->getHasAlpha() || colorValue.w < 1.0f
					|| (isAddBlend() && mOpacity < 1.0f)) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			} else {
				glDisable(GL_BLEND);
			}
			renderParams.addElement(
					isScaleToFit() ? EM_TEXTURE_SCALE : EM_TEXTURE);
		}
		renderParams.addElement(isAddBlend() ? EM_BLEND_ADD : EM_BLEND_MLT);
	} else {
		renderParams.addElement(EM_COLOR);
		if (mBorderColor.w < 1.0f) {
			glEnable(GL_BLEND);
			//2011-10-28 masterkeaton27@gmail.com
			//change default blend function to one
			//this is very important issues for some reasons.
			//if some images displayed strange color, should check this function.
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
	}

	SVIProgram * pProgram = mSVIGLSurface->getProgramManager()->getProgram(
			&renderParams);
	SVIProgramHandler * pHandler = NULL;
	if(pProgram != NULL){
		pHandler = pProgram->getHandler();
	pProgram->use();
	}
	
	if(pHandler){
	SVIVector2 texRegionSize = getTextureRegionSize();
	SVIVector2 texRegionOffset = getTextureRegionOffset();
	SVIVector4 texInfo = SVIVector4(texRegionOffset.x, texRegionOffset.y,
			texRegionSize.x, texRegionSize.y);
	pHandler->setUniform(HD_U_TEXTUREINFO, texInfo);

	texRegionSize = getTextureScaleFitRegionSize();
	texRegionOffset = getTextureScaleFitRegionOffset();
	texInfo = SVIVector4(texRegionOffset.x, texRegionOffset.y, texRegionSize.x,
			texRegionSize.y);
	pHandler->setUniform(HD_U_FITINFO, texInfo);

#if SVI_USING_CPU_TEXCOORD
	pHandler->setAttribute(HD_A_TEXCOORD, mBorderTexcoords);
#else
	SVIVector2 sideSize = SVIVector2(mSize.x, mSize.y);
	SVIVector2 sidePivot = SVIVector2(mPivot.x, mPivot.y);
	pHandler->setUniform(HD_U_SIZE, sideSize);
	pHandler->setUniform(HD_U_PIVOT, sidePivot);
#endif
	pHandler->setAttribute(HD_A_POSITION, mBorderVertices);
	pHandler->releaseAttribute(HD_A_UNITCOORD);
	if (pContainer != NULL && !pContainer->isEmpty())
		pHandler->setSampler(HD_U_SVIMPLER_0, pContainer->getTextureID());
	pHandler->setUniform(HD_U_OPACITY, mOpacity);
	pHandler->setUniform(HD_U_COLOR, mBorderColor);

	//set model view projection matrix
	SVIMatrix modelMat = getTransform();
	//2011-07-12 masterkeaton27@gmail.com
	//orthogonal setting
	SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();
	if (isOrthogonal()) {
		projectionMat._34 = 0.0f;
	}
	//modelMat._34 = 1.0f / -10.0f;
	modelMat._43 -= cfTestDepth;

	pHandler->setUniform(HD_U_PROJ, projectionMat);
	pHandler->setUniform(HD_U_VIEW, modelMat);

	//2011.12.07 jognchae.moon
	if (mBorderAACoord != NULL) {
		pHandler->setAttribute(HD_A_AACOORD, 1, mBorderAACoord);
	}
	//

	//render some

	//2011.12.07 jongchae.moon
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, mBorderIndicesCount, GL_UNSIGNED_BYTE,
			mBorderIndices);
	glDisable(GL_BLEND);
	//

	pHandler->releaseAttribute(HD_A_POSITION);
#if SVI_USING_CPU_TEXCOORD
	pHandler->releaseAttribute(HD_A_TEXCOORD);
#endif
	//2011.12.07 jognchae.moon
	if (mBorderAACoord != NULL) {
		pHandler->releaseAttribute(HD_A_AACOORD);
	}
	}
	//
}

void SVIQuadPolyExt::allocateBorderDatas() {
	mBorderOriVerticeCount = 6 * 4 + 4 * 2;
	mBorderOriIndicesCount = 6 * 4 + 3 * 2 * 4 * 2;

	mBorderVerticeCount = mBorderOriVerticeCount;
	mBorderIndicesCount = mBorderOriIndicesCount;

#if SVI_USING_CPU_TEXCOORD
	if (mBorderVertices != NULL && mBorderTexcoords != NULL && mBorderIndices != NULL) return;
#else
	if (mBorderVertices != NULL && mBorderIndices != NULL)
		return;
#endif

	//mBorderIndicesCount = mRoundLevel * (6 * cornerCount) + (6 * cornerCount) + (mRoundLevel*cornerCount*3*2+3*2*4) * 2;
	//mBorderVerticeCount = (mRoundLevel+1) * 2 * cornerCount + (4 * cornerCount) + (mRoundLevel+1)*cornerCount * 2;

	mBorderVertices = new SVIVector3[mBorderVerticeCount];
#if SVI_USING_CPU_TEXCOORD
	mBorderTexcoords = new SVIVector2[mBorderVerticeCount];
	mMemoryUsed += sizeof(SVIVector2) * mBorderVerticeCount;
#endif
	mMemoryUsed += sizeof(SVIVector3) * mBorderVerticeCount;

	mBorderIndices = new SVIUByte[mBorderIndicesCount];
	mMemoryUsed += sizeof(SVIUByte) * mBorderIndicesCount;

	if (mBorderAACoord == NULL) {
		SVIInt nBorderAACoordCount = mBorderVerticeCount;
		mBorderAACoord = new SVIFloat[nBorderAACoordCount];
		for (int i = 0; i < nBorderAACoordCount; i++) {
			mBorderAACoord[i] = 1.0f;
		}
	}

	mBorderIndicesCount = mBorderOriIndicesCount;
	mBorderVerticeCount = mBorderOriVerticeCount;
}

void SVIQuadPolyExt::deallocateBorderDatas() {
	SVIQuadPoly::deallocateBorderDatas();
	SVI_SVIFE_DELETE_ARRAY(mBorderIndices);
	mBorderIndicesCount = 0;
}

void SVIQuadPolyExt::borderSetup() {
	if (!hasBorder())
		return;

	allocateBorderDatas();

	SVIFloat forAA = 0.5f;
	SVIFloat fOriginalBorderWidth = mBorderWidth;
	//mBorderWidth = mBorderWidth - forAA * 2;

	//Top border  //bottom
	SVIFloat width = mBorderWidth;
	SVIFloat x = mVertices[SVI_QUAD_LEFT_TOP].x + width - forAA;
	SVIFloat y = mVertices[SVI_QUAD_LEFT_TOP].y - forAA;
	SVIFloat w = mVertices[SVI_QUAD_RIGHT_TOP].x - mVertices[SVI_QUAD_LEFT_TOP].x
			- width * 2.0f + forAA * 2.0f;
	SVIFloat h = width;
	SVIInt index = generateBorderRectangle(0, x, y, w, h);
	mBorderIndicesAA_out[2] = index - 6 + 4;
	mBorderIndicesAA_out[3] = index - 6 + 3;

	//Left border  //right
	x = mVertices[SVI_QUAD_LEFT_TOP].x - forAA;
	y = mVertices[SVI_QUAD_LEFT_TOP].y - forAA;
	w = width;
	h = mVertices[SVI_QUAD_LEFT_BOTTOM].y - mVertices[SVI_QUAD_LEFT_TOP].y
			+ forAA * 2.0f;
	index = generateBorderRectangle(index, x, y, w, h);
	mBorderIndicesAA_in[1] = index - 6 + 1;
	mBorderIndicesAA_in[3] = index - 6 + 0;

	//Bottom border  //top
	x = mVertices[SVI_QUAD_LEFT_TOP].x + width - forAA;
	y = mVertices[SVI_QUAD_LEFT_BOTTOM].y + forAA;
	w = mVertices[SVI_QUAD_RIGHT_TOP].x - mVertices[SVI_QUAD_LEFT_TOP].x
			- width * 2.0f + forAA * 2.0f;
	h = -width;
	index = generateBorderRectangle(index, x, y, w, h);
	mBorderIndicesAA_out[0] = index - 6 + 4;
	mBorderIndicesAA_out[1] = index - 6 + 3;

	//right border   //left
	x = mVertices[SVI_QUAD_RIGHT_TOP].x + forAA;
	y = mVertices[SVI_QUAD_LEFT_TOP].y - forAA;
	w = -width;
	h = mVertices[SVI_QUAD_LEFT_BOTTOM].y - mVertices[SVI_QUAD_LEFT_TOP].y
			+ forAA * 2.0f;
	index = generateBorderRectangle(index, x, y, w, h);
	mBorderIndicesAA_in[0] = index - 6 + 1;
	mBorderIndicesAA_in[2] = index - 6 + 0;

	//AA
	mBorderVertices[index + 0] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_out[0]].x - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[0]].y - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[0]].z);
	mBorderVertices[index + 1] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_out[1]].x + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[1]].y - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[1]].z);
	mBorderVertices[index + 2] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_out[2]].x - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[2]].y + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[2]].z);
	mBorderVertices[index + 3] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_out[3]].x + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[3]].y + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_out[3]].z);
	mBorderAACoord[index + 0] = 0.0f;
	mBorderAACoord[index + 1] = 0.0f;
	mBorderAACoord[index + 2] = 0.0f;
	mBorderAACoord[index + 3] = 0.0f;

	SVIInt indexAA = index;
	indexAA = borderSetup_AA_index(indexAA, index, mBorderIndicesAA_out);
	index += 4;

	mBorderVertices[index + 0] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_in[0]].x + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[0]].y + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[0]].z);
	mBorderVertices[index + 1] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_in[1]].x - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[1]].y + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[1]].z);
	mBorderVertices[index + 2] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_in[2]].x + forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[2]].y - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[2]].z);
	mBorderVertices[index + 3] = SVIVector3(
			mBorderVertices[mBorderIndicesAA_in[3]].x - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[3]].y - forAA * 2.0f,
			mBorderVertices[mBorderIndicesAA_in[3]].z);
	mBorderAACoord[index + 0] = 0.0f;
	mBorderAACoord[index + 1] = 0.0f;
	mBorderAACoord[index + 2] = 0.0f;
	mBorderAACoord[index + 3] = 0.0f;
	borderSetup_AA_index(indexAA, index, mBorderIndicesAA_in);

	mBorderWidth = fOriginalBorderWidth;

#if SVI_USING_CPU_TEXCOORD	
	SVIFloat fXDistance = mSize.x; // * mScale.x;
	SVIFloat fYDistance = mSize.y;// * mScale.y;

	SVIFloat fXRatio = mPivot.x * fXDistance;
	SVIFloat fYRatio = mPivot.y * fYDistance;

	for (int n = 0; n < mBorderVerticeCount; ++n) {
		mBorderTexcoords[n].x = (mBorderVertices[n].x+fXRatio) / fXDistance;
		mBorderTexcoords[n].y = (mBorderVertices[n].y+fYRatio) / fYDistance;
	}
#endif
}

SVIInt SVIQuadPolyExt::borderSetup_AA_index(SVIInt indexAA, SVIInt index,
		SVIUByte *borderIndicesAA) {
	//AA Index up
	mBorderIndices[indexAA + 0] = index + 1;
	mBorderIndices[indexAA + 1] = index + 0;
	mBorderIndices[indexAA + 2] = borderIndicesAA[0];

	mBorderIndices[indexAA + 3] = index + 1;
	mBorderIndices[indexAA + 4] = borderIndicesAA[0];
	mBorderIndices[indexAA + 5] = borderIndicesAA[1];
	//

	//AA index left
	mBorderIndices[indexAA + 6] = index + 0;
	mBorderIndices[indexAA + 7] = index + 2;
	mBorderIndices[indexAA + 8] = borderIndicesAA[2];

	mBorderIndices[indexAA + 9] = index + 0;
	mBorderIndices[indexAA + 10] = borderIndicesAA[2];
	mBorderIndices[indexAA + 11] = borderIndicesAA[0];
	//

	//AA index right
	mBorderIndices[indexAA + 12] = index + 3;
	mBorderIndices[indexAA + 13] = index + 1;
	mBorderIndices[indexAA + 14] = borderIndicesAA[1];

	mBorderIndices[indexAA + 15] = index + 3;
	mBorderIndices[indexAA + 16] = borderIndicesAA[1];
	mBorderIndices[indexAA + 17] = borderIndicesAA[3];
	//

	//AA index bottom
	mBorderIndices[indexAA + 18] = index + 3;
	mBorderIndices[indexAA + 19] = index + 2;
	mBorderIndices[indexAA + 20] = borderIndicesAA[2];

	mBorderIndices[indexAA + 21] = index + 3;
	mBorderIndices[indexAA + 22] = borderIndicesAA[2];
	mBorderIndices[indexAA + 23] = borderIndicesAA[3];

	return indexAA + 24;
}

SVIInt SVIQuadPolyExt::generateBorderRectangle(SVIInt startIndex, SVIFloat x,
		SVIFloat y, SVIFloat w, SVIFloat h) {
	SVIInt index = startIndex;

	//upper triangle
	mBorderVertices[index + 0] = SVIVector3(x, y, 0.0f);
	mBorderVertices[index + 1] = SVIVector3(x, y + h, 0.0f);
	mBorderVertices[index + 2] = SVIVector3(x + w, y, 0.0f);

	//down triangle
	mBorderVertices[index + 3] = SVIVector3(x, y + h, 0.0f);
	mBorderVertices[index + 4] = SVIVector3(x + w, y + h, 0.0f);
	mBorderVertices[index + 5] = SVIVector3(x + w, y, 0.0f);

	//index
	mBorderIndices[index + 0] = index + 0;
	mBorderIndices[index + 1] = index + 1;
	mBorderIndices[index + 2] = index + 2;

	mBorderIndices[index + 3] = index + 3;
	mBorderIndices[index + 4] = index + 4;
	mBorderIndices[index + 5] = index + 5;

	return index + 6;
}

}
