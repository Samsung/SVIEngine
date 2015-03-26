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
#include "SVIRoundPoly.h"


namespace SVI {

const float cfTestDepth = 1.0f;
static const SVIBool DEBUG = SVIFALSE;

/****************************************************************************/
// Round Box polygon object	2011-06-06 masterkeaton27@gmail.com
/****************************************************************************/

SVIUInt SVIRoundPoly::getBorderVerticeCount() {
	return mBorderVerticeCount;
}
SVIUInt SVIRoundPoly::getBorderIndicesCount() {
	return mBorderIndicesCount;
}
SVIVector3 * SVIRoundPoly::getBorderVertices() {
	return mBorderVertices;
}

SVIUShort * SVIRoundPoly::getBorderIndices() {
	return mBorderIndices;
}

SVIVector3 * SVIRoundPoly::getShadowVertices() {
	return mShadowVertices;
}

void SVIRoundPoly::allocateBorderDatas() {
	int cornerCount = 4;
	//2011-06-28 masterkeaton27@gmail.com
	mBorderIndicesCount = mRoundLevel * (6 * cornerCount) + (6 * cornerCount);
	mBorderVerticeCount = (mRoundLevel + 1) * 2 * cornerCount
			+ (4 * cornerCount);

	//2011-06-27 masterkeaton27@gmail.com
	//memory optimization. but this block may cause exception errors
	//when use changing radius details in real-time situation.
#if SVI_USING_CPU_TEXCOORD
	if (mBorderVertices != NULL && mBorderTexcoords != NULL && mBorderIndices != NULL) return;
#else
	if (mBorderVertices != NULL && mBorderIndices != NULL)
		return;
#endif		

	mBorderVertices = new SVIVector3[mBorderVerticeCount];
#if SVI_USING_CPU_TEXCOORD
	mBorderTexcoords = new SVIVector2[mBorderVerticeCount];
	mMemoryUsed += sizeof(SVIVector2) * mBorderVerticeCount;
#endif
	mMemoryUsed += sizeof(SVIVector3) * mBorderVerticeCount;

	mBorderIndices = new SVIUShort[mBorderIndicesCount];
	mMemoryUsed += sizeof(SVIUShort) * mBorderIndicesCount;

	//2011.12.07 jongchae.moon
	if (mBorderAACoord == NULL) {
		int cornerCount = 4;
		int nBorderAACoordCount = (mRoundLevel + 1) * 2 * cornerCount
				+ (4 * cornerCount);

		mBorderAACoord = new SVIFloat[nBorderAACoordCount];

		for (int i = 0; i < nBorderAACoordCount; i++) {
			mBorderAACoord[i] = 1.0f;
		}
	}
	//
}

void SVIRoundPoly::deallocateBorderDatas() {
	//2011.12.07 jongchae.moon
	SVI_SVIFE_DELETE_ARRAY(mBorderAACoord);
	//
	SVI_SVIFE_DELETE_ARRAY(mBorderVertices);
#if SVI_USING_CPU_TEXCOORD
	SVI_SVIFE_DELETE_ARRAY(mBorderTexcoords);
#endif
	SVI_SVIFE_DELETE_ARRAY(mBorderIndices);
	mBorderIndicesCount = 0;
	mBorderVerticeCount = 0;
	mMemoryUsed = 0;
}

SVIRoundPoly::SVIRoundPoly(SVIGLSurface* saGLSurface) :
	SVIPoly(saGLSurface){


	mShadowVertices = NULL;
	mBorderVertices = NULL;
#if SVI_USING_CPU_TEXCOORD
	mBorderTexcoords = NULL;
#endif
	mBorderIndices = NULL;

	mBorderIndicesCount = 0;
	mBorderVerticeCount = 0;

	mRenderPritiveType = SVI_POLY_TRI_LIST;
	mHasAdditionalRender = SVITRUE;

	setRoundRadius(20); //10% radius corner
	mRoundLevel = 16;
	mBorderColor = SVIVector4(1.0f, 1.0f, 1.0f, 1.0f);
	mBorderWidth = 10.0f; //10 pixel width
	//2011.12.07 jongchae.moon
	mBorderAACoord = NULL;
	//
	mBorderTexcoords = NULL;
	mRoundRadius[SVI_ROUND_LEFT_TOP] = 0.0f;
	mRoundRadius[SVI_ROUND_LEFT_BOTTOM] = 0.0f;
	mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] = 0.0f;
	mRoundRadius[SVI_ROUND_RIGHT_TOP] = 0.0f;
	
	mRoundCenter[SVI_ROUND_LEFT_TOP] = SVIVector3(0.0f, 0.0f, 0.0f);
	mRoundCenter[SVI_ROUND_LEFT_BOTTOM] = SVIVector3(0.0f, 0.0f, 0.0f);
	mRoundCenter[SVI_ROUND_RIGHT_BOTTOM] = SVIVector3(0.0f, 0.0f, 0.0f);
	mRoundCenter[SVI_ROUND_RIGHT_TOP] = SVIVector3(0.0f, 0.0f, 0.0f);

}

void SVIRoundPoly::setup() {
	SVIFloat fXDist = mSize.x; // * mScale.x;
	SVIFloat fYDist = mSize.y; // * mScale.y;
	SVIFloat fNormal = (fXDist > fYDist) ? fYDist : fXDist;

	//2011-07-01 corner animation setup
	SVIFloat fDefaultRadius = mRoundRadius[SVI_ROUND_LEFT_TOP];

	//2011-07-04 masterkeaton27@gmail.com
	//corner radius size limitation fixed to proper way.
	if (fDefaultRadius * 2.0f > (mSize.x)) {
		fDefaultRadius = (mSize.x) * 0.5f;
		setRoundRadius(fDefaultRadius);
	} else if (fDefaultRadius * 2.0f > (mSize.y)) {
		fDefaultRadius = (mSize.y) * 0.5f;
		setRoundRadius(fDefaultRadius);
	}

	SVIFloat fMaxX = fXDist - fDefaultRadius;
	SVIFloat fMinX = fDefaultRadius;
	SVIFloat fMaxY = fYDist - fDefaultRadius;
	SVIFloat fMinY = fDefaultRadius;

	mRoundCenter[SVI_ROUND_LEFT_TOP] = SVIVector3(fMinX, fMinY, 0.0f);
	mRoundCenter[SVI_ROUND_LEFT_BOTTOM] = SVIVector3(fMinX, fMaxY, 0.0f);
	mRoundCenter[SVI_ROUND_RIGHT_BOTTOM] = SVIVector3(fMaxX, fMaxY, 0.0f);
	mRoundCenter[SVI_ROUND_RIGHT_TOP] = SVIVector3(fMaxX, fMinY, 0.0f);

	//2011-07-03 masterkeaton27@gmail.com
	//pre-setup indices buffer for indexed primitive rendering
	const SVIInt CORNER_COUNT = 4;
	SVIInt cornerVerticeCount = CORNER_COUNT * (mRoundLevel + 1) + CORNER_COUNT;
	SVIInt InnerVerticeCount = 5 * 4;

	SVIInt cornerIndiceCount = mRoundLevel * 3 * CORNER_COUNT; // corners are triangles
	SVIInt InnerIndiceCount = 5 * 6;
	//2011-07-04 masterkeaton27@gmail.com
	//index buffer memory leak bug fixed.
	allocate(cornerVerticeCount + InnerVerticeCount,
			cornerIndiceCount + InnerIndiceCount);

	if (hasBorder() && mShadowVertices == NULL) {
		mShadowVertices = new SVIVector3[cornerVerticeCount + InnerVerticeCount];
	}
#if SVI_USING_CPU_TEXCOORD		
	borderSetup();
#endif
}

SVIRoundPoly::~SVIRoundPoly() {
	deallocateBorderDatas();
	SVI_SVIFE_DELETE_ARRAY(mShadowVertices);
}

void SVIRoundPoly::setRoundRadius(SVIFloat radius) {
	if (mRoundRadius[SVI_ROUND_LEFT_TOP] != radius) {
		mNeedToUpdate = SVITRUE;
		//2011-07-19 masterkeaton27@gmail.com
		//re-calculate texture coordinate.
		mNeedToUpdateTexcoord = SVITRUE;
	}
	mRoundRadius[SVI_ROUND_LEFT_TOP] = radius;
	mRoundRadius[SVI_ROUND_LEFT_BOTTOM] = radius;
	mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] = radius;
	mRoundRadius[SVI_ROUND_RIGHT_TOP] = radius;
}

void SVIRoundPoly::setRoundLevel(SVIInt level) {
	if (mRoundLevel != (SVIUInt)level) {
		mRoundLevel = level;
		mNeedToUpdate = SVITRUE;
		mNeedToUpdateTexcoord = SVITRUE;
	}
}
void SVIRoundPoly::setBorderWidth(SVIFloat width) {
	if (width > 0.01f) {
		setBorder(SVITRUE);
	} else
		setBorder(SVIFALSE);

	if (mBorderWidth != width) {
		mBorderWidth = width;
		mNeedToUpdate = SVITRUE;
		mNeedToUpdateTexcoord = SVITRUE;
	}
}

void SVIRoundPoly::setBorderColor(SVIVector4 color) {
	mBorderColor = color;
}

void SVIRoundPoly::setBorderColor(SVIFloat r, SVIFloat g, SVIFloat b, SVIFloat a) {
	mBorderColor.x = r;
	mBorderColor.y = g;
	mBorderColor.z = b;
	mBorderColor.w = a;
}

SVIInt SVIRoundPoly::generateBorderCornerVertices(SVIInt index, SVIFloat forAA) {
	SVIFloat fXDist = mSize.x * mScale.x;
	SVIFloat fYDist = mSize.y * mScale.y;
	SVIFloat fNormal = (fXDist > fYDist) ? fYDist : fXDist;
	SVIInt startIndex = 0;
	SVIInt startBufferIndex = 0;
	SVIFloat fDeltaRadian = (SVI_PI * 0.5f) / (SVIFloat) mRoundLevel;
	SVIFloat fCurrentRadian = 0.0f;
	SVIFloat fNextRadian = fDeltaRadian;
	SVIFloat fRadius = mRoundRadius[index];
	SVIFloat fx = 1.0f, fy = 1.0f;
	SVIFloat cx = 1.0f, cy = 1.0f;
	SVIFloat cornerX = 1.0f, cornerY = 1.0f;

	switch (index) {
	case SVI_ROUND_LEFT_TOP:
		startIndex = 0;
		startBufferIndex = 0;
		fx = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - forAA);
		fy = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - forAA);
		cx = mRoundCenter[SVI_ROUND_LEFT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_TOP].y;
		cornerX = mBorderWidth;
		cornerY = mBorderWidth;
		break;
	case SVI_ROUND_LEFT_BOTTOM:
		startBufferIndex = (mRoundLevel * 1 * 3 * 2);
		startIndex = ((mRoundLevel + 1) * 1 * 2);
		fx = -(mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - forAA);
		fy = mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - forAA;
		cx = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y;
		cornerX = mBorderWidth;
		cornerY = fYDist - mBorderWidth;
		break;
	case SVI_ROUND_RIGHT_BOTTOM:
		startBufferIndex = (mRoundLevel * 2 * 3 * 2);
		startIndex = ((mRoundLevel + 1) * 2 * 2);
		fx = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - forAA;
		fy = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - forAA;
		cx = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].y;
		cornerX = fXDist - mBorderWidth;
		cornerY = fYDist - mBorderWidth;
		break;
	case SVI_ROUND_RIGHT_TOP:
		startBufferIndex = (mRoundLevel * 3 * 3 * 2);
		startIndex = ((mRoundLevel + 1) * 3 * 2);
		fx = mRoundRadius[SVI_ROUND_RIGHT_TOP] - forAA;
		fy = -(mRoundRadius[SVI_ROUND_RIGHT_TOP] - forAA);
		cx = mRoundCenter[SVI_ROUND_RIGHT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_TOP].y;
		cornerX = fXDist - mBorderWidth;
		cornerY = mBorderWidth;
		break;
	}

	SVIVector3 vA;
	SVIVector3 vtA, vB = SVIVector3(cx, cy, 0.0f), vC = SVIVector3(cornerX, cornerY, 0.0f);

	SVIInt deltaindex = startIndex;

	for (SVIUInt n = 0; n < mRoundLevel + 1; ++n) {
		float px = cosf(fCurrentRadian) * fx;
		float py = sinf(fCurrentRadian) * fy;

		if(mBorderWidth > fRadius){
			if(n == 0){
				switch(index){
					case SVI_ROUND_LEFT_TOP:
						py += mBorderWidth - fRadius;
						break;
					case SVI_ROUND_LEFT_BOTTOM:
						py -= mBorderWidth - fRadius;
						break;
					case SVI_ROUND_RIGHT_BOTTOM:
						py -= mBorderWidth - fRadius;
						break;
					case SVI_ROUND_RIGHT_TOP:
						py += mBorderWidth - fRadius;
				}
			}else if(n == mRoundLevel){
				switch(index){
					case SVI_ROUND_LEFT_TOP:
						px += mBorderWidth - fRadius;
						break;
					case SVI_ROUND_LEFT_BOTTOM:
						px += mBorderWidth - fRadius;
						break;
					case SVI_ROUND_RIGHT_BOTTOM:
						px -= mBorderWidth - fRadius;
						break;
					case SVI_ROUND_RIGHT_TOP:
						px -= mBorderWidth - fRadius;
				}
			}
		}		

		vA = SVIVector3(cx + px, cy + py, 0.0f);
		if(mBorderWidth > fRadius){
			vtA = vC;
		}else{
			vtA = vB - vA;
			vtA.normalize();
			vtA *= mBorderWidth;
			vtA = vA + vtA;
		}
		

		fCurrentRadian += fDeltaRadian;

		//generate vertex list
		mBorderVertices[deltaindex + 0] = vA;
		mBorderVertices[deltaindex + 1] = vtA;
		deltaindex += 2;
	}

	deltaindex = startIndex;
	SVIInt deltaBufferIndex = startBufferIndex;
	for (SVIUInt n = 0; n < mRoundLevel; ++n) {
		//generate index list
		mBorderIndices[deltaBufferIndex + 0] = deltaindex + 0;
		mBorderIndices[deltaBufferIndex + 1] = deltaindex + 1;
		mBorderIndices[deltaBufferIndex + 2] = deltaindex + 2;

		mBorderIndices[deltaBufferIndex + 3] = deltaindex + 1;
		mBorderIndices[deltaBufferIndex + 4] = deltaindex + 3;
		mBorderIndices[deltaBufferIndex + 5] = deltaindex + 2;

		deltaindex += 2;
		deltaBufferIndex += 6;
	}

	//set next vertices
	return deltaindex;
}

SVIInt SVIRoundPoly::generateCornerVertices(SVIInt index) {

	SVIInt startIndex = 0;
	SVIInt startBufferIndex = 0;
	SVIFloat fDeltaRadian = (SVI_PI * 0.5f) / (SVIFloat) mRoundLevel;
	SVIFloat fCurrentRadian = 0.0f;
	SVIFloat fNextRadian = fDeltaRadian;
	SVIFloat fRadius = mRoundRadius[index];
	SVIFloat fx = 1.0f, fy = 1.0f;
	SVIFloat nfx = 1.0f, nfy = 1.0f;
	SVIFloat cx = 1.0f, cy = 1.0f;

	switch (index) {
	case SVI_ROUND_LEFT_TOP:
		startIndex = 0;
		startBufferIndex = 0;
		fx = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - mBorderWidth);
		fy = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - mBorderWidth);
		nfx = -mRoundRadius[SVI_ROUND_LEFT_TOP];
		nfy = -mRoundRadius[SVI_ROUND_LEFT_TOP];
		cx = mRoundCenter[SVI_ROUND_LEFT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_TOP].y;

		break;
	case SVI_ROUND_LEFT_BOTTOM:
		startIndex = ((mRoundLevel + 1) * 1) + 1;
		startBufferIndex = (mRoundLevel * 1) * 3;
		fx = -(mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - mBorderWidth);
		fy = mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - mBorderWidth;
		nfx = -mRoundRadius[SVI_ROUND_LEFT_BOTTOM];
		nfy = mRoundRadius[SVI_ROUND_LEFT_BOTTOM];
		cx = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y;
		break;
	case SVI_ROUND_RIGHT_BOTTOM:
		startIndex = ((mRoundLevel + 1) * 2) + 2;
		startBufferIndex = (mRoundLevel * 2) * 3;
		fx = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - mBorderWidth;
		fy = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - mBorderWidth;
		nfx = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM];
		nfy = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM];
		cx = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].y;
		break;
	case SVI_ROUND_RIGHT_TOP:
		startIndex = ((mRoundLevel + 1) * 3) + 3;
		startBufferIndex = (mRoundLevel * 3) * 3;
		fx = mRoundRadius[SVI_ROUND_RIGHT_TOP] - mBorderWidth;
		fy = -(mRoundRadius[SVI_ROUND_RIGHT_TOP] - mBorderWidth);
		nfx = mRoundRadius[SVI_ROUND_RIGHT_TOP];
		nfy = -mRoundRadius[SVI_ROUND_RIGHT_TOP];
		cx = mRoundCenter[SVI_ROUND_RIGHT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_TOP].y;
		break;
	}

	SVIVector3 vA, vB;
	vB = SVIVector3(cx, cy, 0.0f);

	if(mRoundRadius[index] < mBorderWidth){
		fx = 0;
		fy = 0;
	}

	//generate FAN outside vertices
	SVIInt deltaindex = startIndex;
	for (SVIUInt n = 0; n < mRoundLevel + 1; ++n) {
		float px = cosf(fCurrentRadian);
		float py = sinf(fCurrentRadian);
		vA.x = cx + px * fx;
		vA.y = cy + py * fy;
		vA.z = 0.0f;
		fCurrentRadian += fDeltaRadian;
		mVertices[deltaindex].x = vA.x;
		mVertices[deltaindex].y = vA.y;
		mVertices[deltaindex].z = vA.z;

		mUnitCoords[deltaindex].x = mVertices[deltaindex].x/mSize.x;
		mUnitCoords[deltaindex].y = mVertices[deltaindex].y/mSize.y;
			
		if (hasBorder()) {
			mShadowVertices[deltaindex].x = cx + px * nfx;
			mShadowVertices[deltaindex].y = cy + py * nfy;
			mShadowVertices[deltaindex].z = 0.0;
		}
		deltaindex++;
	}

	if (hasBorder()) {
		//add last one center vertex
		mShadowVertices[deltaindex].x = mVertices[deltaindex].x = vB.x;
		mShadowVertices[deltaindex].y = mVertices[deltaindex].y = vB.y;
		mShadowVertices[deltaindex].z = mVertices[deltaindex].z = vB.z;

		mUnitCoords[deltaindex].x = mVertices[deltaindex].x/mSize.x;
		mUnitCoords[deltaindex].y = mVertices[deltaindex].y/mSize.y;
	} else {
		mVertices[deltaindex].x = vB.x;
		mVertices[deltaindex].y = vB.y;
		mVertices[deltaindex].z = vB.z;

		mUnitCoords[deltaindex].x = mVertices[deltaindex].x/mSize.x;
		mUnitCoords[deltaindex].y = mVertices[deltaindex].y/mSize.y;
	}
	deltaindex++;

	SVIInt savedDeltaIndex = deltaindex;
	SVIInt centerIndex = deltaindex - 1;
	deltaindex = startIndex;
	SVIInt deltaBufferIndex = startBufferIndex;
	for (SVIUInt n = 0; n < mRoundLevel; ++n) {
		mIndices[startBufferIndex + 0] = startIndex + n;
		mIndices[startBufferIndex + 1] = startIndex + n + 1;
		mIndices[startBufferIndex + 2] = centerIndex;
		startBufferIndex += 3;
	}
	return savedDeltaIndex;
}

void SVIRoundPoly::borderSetup() {

	allocateBorderDatas();

	//generate left top corner
	generateBorderCornerVertices(SVI_ROUND_LEFT_TOP);
	generateBorderCornerVertices(SVI_ROUND_LEFT_BOTTOM);
	generateBorderCornerVertices(SVI_ROUND_RIGHT_BOTTOM);
	SVIInt startIndex = generateBorderCornerVertices(SVI_ROUND_RIGHT_TOP) + 2;
	//DEBUG_CHECK_LOGI("Border Vertex Generation Complete %d/%d",startIndex, startIndex/3);

	SVIInt startBufferIndex = (mRoundLevel * 4 * 3 * 2);
	//left
	SVIFloat fXDist = mSize.x;
	SVIFloat fYDist = mSize.y;
	SVIFloat cx, cy, cw, ch;

	if (mRoundRadius[SVI_ROUND_RIGHT_TOP] * 2.0f < fYDist) {
		float cornerOffset = (mBorderWidth>mRoundRadius[SVI_ROUND_RIGHT_TOP])? mBorderWidth: mRoundRadius[SVI_ROUND_RIGHT_TOP];
		
		cx = 0.0f;
		cy = cornerOffset;
		cw = mBorderWidth;
		ch = fYDist - 2*cornerOffset;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);

		//right
		cx = fXDist - mBorderWidth;
		cy = cornerOffset;
		cw = mBorderWidth;
		ch = fYDist - 2*cornerOffset;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);
	} else {
		//mBorderVerticeCount -= 12;
		mBorderIndicesCount -= 12;
		mBorderVerticeCount -= 8;
	}
	if (mRoundRadius[SVI_ROUND_RIGHT_TOP] * 2.0f < fXDist) {
		float cornerOffset = (mBorderWidth>mRoundRadius[SVI_ROUND_RIGHT_TOP])? mBorderWidth: mRoundRadius[SVI_ROUND_RIGHT_TOP];
		
		//top
		cx = cornerOffset;
		cy = 0.0f;
		cw = fXDist - 2*cornerOffset;
		ch = mBorderWidth;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);

		//bottom
		cx = cornerOffset;
		cy = fYDist - mBorderWidth;
		cw = fXDist - 2*cornerOffset;
		ch = mBorderWidth;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);
	} else {
		//mBorderVerticeCount -= 12;
		mBorderIndicesCount -= 12;
		mBorderVerticeCount -= 8;
	}

	SVIFloat fXRatio = mPivot.x;
	SVIFloat fYRatio = mPivot.y;
	SVIFloat fZRatio = mPivot.z;

	for (SVIUInt n = 0; n < mBorderVerticeCount; ++n) {
#if SVI_USING_CPU_TEXCOORD
		mBorderTexcoords[n].x = mBorderVertices[n].x / fXDist;
		mBorderTexcoords[n].y = mBorderVertices[n].y / fYDist;
#endif
		mBorderVertices[n].x -= mSize.x * fXRatio;
		mBorderVertices[n].y -= mSize.y * fYRatio;
	}
}

SVIInt SVIRoundPoly::generateInnerVertices(SVIInt startIndex) {
	SVIInt index = startIndex;
	SVIFloat cx, cy, cw, ch;
	SVIBool widerZero = mRoundRadius[SVI_ROUND_RIGHT_TOP] * 2.0f
			> mSize.x * mScale.x;
	SVIBool higherZero = mRoundRadius[SVI_ROUND_RIGHT_TOP] * 2.0f
			> mSize.y * mScale.y;

	SVIInt startBufferIndex = (mRoundLevel * 4 * 3);

	if (!widerZero && !higherZero) {

		
		if(mRoundRadius[SVI_ROUND_LEFT_TOP] > mBorderWidth){
			cx = mBorderWidth +  (mRoundRadius[SVI_ROUND_LEFT_TOP] - mBorderWidth);
			cy = mBorderWidth +  (mRoundRadius[SVI_ROUND_LEFT_TOP] - mBorderWidth);
			cw = mSize.x - 2*(mBorderWidth +  (mRoundRadius[SVI_ROUND_LEFT_TOP] - mBorderWidth));
			ch = mSize.y - 2*(mBorderWidth +  (mRoundRadius[SVI_ROUND_LEFT_TOP] - mBorderWidth));
		}else{
			cx = mBorderWidth;
			cy = mBorderWidth;
			cw = mSize.x - 2*mBorderWidth;
			ch = mSize.y - 2*mBorderWidth;
		}		
		if (hasBorder())
			generateInnerShadowRectangle(index, cx, cy, cw, ch);

		index = generateInnerRectangle(index, &startBufferIndex, cx, cy, cw, ch);

	} else {
		mIndicesCount -= 6;
		mVerticeCount -= 4;
	}

	if (!higherZero && mRoundRadius[SVI_ROUND_LEFT_TOP] > mBorderWidth) {
		if (hasBorder()) {
			cx = 0.0f;
			cy = mRoundCenter[SVI_ROUND_LEFT_TOP].y;
			cw = mRoundRadius[SVI_ROUND_LEFT_TOP];
			ch = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y
					- mRoundCenter[SVI_ROUND_LEFT_TOP].y;
			generateInnerShadowRectangle(index, cx, cy, cw, ch);
		}
		cx = mBorderWidth;
		cy = mRoundCenter[SVI_ROUND_LEFT_TOP].y;
		cw = mRoundRadius[SVI_ROUND_LEFT_TOP] - mBorderWidth;
		ch = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y
				- mRoundCenter[SVI_ROUND_LEFT_TOP].y;
 		index = generateInnerRectangle(index, &startBufferIndex, cx, cy, cw, ch);
		
		if (hasBorder()) {
			cx = mRoundCenter[SVI_ROUND_RIGHT_TOP].x;
			cy = mRoundCenter[SVI_ROUND_RIGHT_TOP].y;
			cw = mRoundRadius[SVI_ROUND_RIGHT_TOP];
			ch = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].y
					- mRoundCenter[SVI_ROUND_RIGHT_TOP].y;
			generateInnerShadowRectangle(index, cx, cy, cw, ch);
		}
		cx = mRoundCenter[SVI_ROUND_RIGHT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_TOP].y;
		cw = mRoundRadius[SVI_ROUND_RIGHT_TOP] - mBorderWidth;
		ch = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].y
				- mRoundCenter[SVI_ROUND_RIGHT_TOP].y;
		index = generateInnerRectangle(index, &startBufferIndex, cx, cy, cw, ch);

	} else {
		mIndicesCount -= 12;
		mVerticeCount -= 8;
	}
	
	if (!widerZero  && mRoundRadius[SVI_ROUND_RIGHT_TOP] > mBorderWidth) {
		if (hasBorder()) {
			cx = mRoundCenter[SVI_ROUND_LEFT_TOP].x;
			cy = 0.0f;
			cw = mRoundCenter[SVI_ROUND_RIGHT_TOP].x
					- mRoundCenter[SVI_ROUND_LEFT_TOP].x;
			ch = mRoundRadius[SVI_ROUND_RIGHT_TOP];
			generateInnerShadowRectangle(index, cx, cy, cw, ch);
		}
		cx = mRoundCenter[SVI_ROUND_LEFT_TOP].x;
		cy = mBorderWidth;
		cw = mRoundCenter[SVI_ROUND_RIGHT_TOP].x
				- mRoundCenter[SVI_ROUND_LEFT_TOP].x;
		ch = mRoundRadius[SVI_ROUND_RIGHT_TOP] - mBorderWidth;
		index = generateInnerRectangle(index, &startBufferIndex, cx, cy, cw, ch);
		
		if (hasBorder()) {
			cx = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
			cy = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y;
			cw = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].x
					- mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
			ch = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM];
			generateInnerShadowRectangle(index, cx, cy, cw, ch);
		}
		cx = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y;
		cw = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].x
				- mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
		ch = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - mBorderWidth;
		index = generateInnerRectangle(index, &startBufferIndex, cx, cy, cw, ch);
	} else {
		mIndicesCount -= 12;
		mVerticeCount -= 8;
	}
	return index;
}

SVIInt SVIRoundPoly::generateBorderRectangle(SVIInt startIndex,
		SVIInt * indicesBufferIndex, SVIFloat x, SVIFloat y, SVIFloat w,
		SVIFloat h) {
	SVIInt index = startIndex;

	mBorderVertices[index + 0].x = x;
	mBorderVertices[index + 0].y = y;
	mBorderVertices[index + 0].z = 0.0f;

	mBorderVertices[index + 1].x = x;
	mBorderVertices[index + 1].y = y + h;
	mBorderVertices[index + 1].z = 0.0f;

	mBorderVertices[index + 2].x = x + w;
	mBorderVertices[index + 2].y = y;
	mBorderVertices[index + 2].z = 0.0f;

	mBorderVertices[index + 3].x = x + w;
	mBorderVertices[index + 3].y = y + h;
	mBorderVertices[index + 3].z = 0.0f;

	mBorderIndices[*indicesBufferIndex + 0] = index + 0;
	mBorderIndices[*indicesBufferIndex + 1] = index + 1;
	mBorderIndices[*indicesBufferIndex + 2] = index + 2;

	mBorderIndices[*indicesBufferIndex + 3] = index + 1;
	mBorderIndices[*indicesBufferIndex + 4] = index + 3;
	mBorderIndices[*indicesBufferIndex + 5] = index + 2;

	*indicesBufferIndex = *indicesBufferIndex + 6;

	return index + 4;
}

void SVIRoundPoly::generateInnerShadowRectangle(SVIInt startIndex, SVIFloat x,
		SVIFloat y, SVIFloat w, SVIFloat h) {
	SVIInt index = startIndex;

	mShadowVertices[index + 0].x = x;
	mShadowVertices[index + 0].y = y;
	mShadowVertices[index + 0].z = 0.0f;

	mShadowVertices[index + 1].x = x;
	mShadowVertices[index + 1].y = y + h;
	mShadowVertices[index + 1].z = 0.0f;

	mShadowVertices[index + 2].x = x + w;
	mShadowVertices[index + 2].y = y;
	mShadowVertices[index + 2].z = 0.0f;

	mShadowVertices[index + 3].x = x + w;
	mShadowVertices[index + 3].y = y + h;
	mShadowVertices[index + 3].z = 0.0f;
}

SVIInt SVIRoundPoly::generateInnerRectangle(SVIInt startIndex,
		SVIInt * indicesBufferIndex, SVIFloat x, SVIFloat y, SVIFloat w,
		SVIFloat h) {
	SVIInt index = startIndex;

	mVertices[index + 0].x = x;
	mVertices[index + 0].y = y;
	mVertices[index + 0].z = 0.0f;

	mVertices[index + 1].x = x;
	mVertices[index + 1].y = y + h;
	mVertices[index + 1].z = 0.0f;

	mVertices[index + 2].x = x + w;
	mVertices[index + 2].y = y;
	mVertices[index + 2].z = 0.0f;

	mVertices[index + 3].x = x + w;
	mVertices[index + 3].y = y + h;
	mVertices[index + 3].z = 0.0f;


	mUnitCoords[index + 0].x = x/mSize.x;
	mUnitCoords[index + 0].y = y/mSize.y;

	mUnitCoords[index + 1].x = x/mSize.x;
	mUnitCoords[index + 1].y = (y+h)/mSize.y;

	mUnitCoords[index + 2].x = (x+w)/mSize.x;
	mUnitCoords[index + 2].y = y/mSize.y;

	mUnitCoords[index + 3].x = (x+w)/mSize.x;
	mUnitCoords[index + 3].y = (y+h)/mSize.y;


	mIndices[*indicesBufferIndex + 0] = index + 0;
	mIndices[*indicesBufferIndex + 1] = index + 1;
	mIndices[*indicesBufferIndex + 2] = index + 2;

	mIndices[*indicesBufferIndex + 3] = index + 1;
	mIndices[*indicesBufferIndex + 4] = index + 3;
	mIndices[*indicesBufferIndex + 5] = index + 2;

	*indicesBufferIndex = *indicesBufferIndex + 6;

	return index + 4;
}

void SVIRoundPoly::buildIndices() {
	if (mIndices == NULL)
		return;

}

void SVIRoundPoly::buildVertices() {
	if (mVertices == NULL)
		return;
	//generate left top corner
	generateCornerVertices(SVI_ROUND_LEFT_TOP);
	//generate left right corner
	generateCornerVertices(SVI_ROUND_LEFT_BOTTOM);
	//generate left bottom corner
	generateCornerVertices(SVI_ROUND_RIGHT_BOTTOM);
	//generate right bottom corner
	SVIInt startIndex = generateCornerVertices(SVI_ROUND_RIGHT_TOP);
	DEBUG_CHECK_LOGI(
			"Corner Vertex Generation Complete %d/%d", startIndex, startIndex/3);

	//generate inner vertices
	startIndex = generateInnerVertices(startIndex);
	DEBUG_CHECK_LOGI(
			"Inner Vertex Generation Complete %d/%d", startIndex, startIndex/3);

#if SVI_USING_CPU_TEXCOORD
#else
	if (hasBorder()) {
		borderSetup();
	}
#endif
}

void SVIRoundPoly::buildTextureCoordinates() {
#if SVI_USING_CPU_TEXCOORD
	if (mTextureCoords == NULL) return;

	SVIFloat fXDistance = mSize.x; // * mScale.x;
	SVIFloat fYDistance = mSize.y;// * mScale.y;

	SVIFloat fTOffsetU = 0.0f;//mTextureRegionOffset.x;
	SVIFloat fTOffsetV = 0.0f;//mTextureRegionOffset.y;
	SVIFloat fTScaleU = 1.0f;//mTextureRegionSize.x;
	SVIFloat fTScaleV = 1.0f;//mTextureRegionSize.y;

	//2011-06-22 texture scale added for flip object
	float fAmountU = fabs(mTextureScale.x);
	float fAmountV = fabs(mTextureScale.y);

	for (int n = 0; n < mVerticeCount; ++n) {
		mTextureCoords[n].x = mVertices[n].x / fXDistance;
		mTextureCoords[n].y = mVertices[n].y / fYDistance;

		if (mTextureScale.x < 0.0f) {
			mTextureCoords[n].x = (1.0f - mTextureCoords[n].x) * fAmountU;
		}
		if (mTextureScale.y < 0.0f) {
			mTextureCoords[n].y = (1.0f - mTextureCoords[n].y) * fAmountV;
		}

		mTextureCoords[n].x = fTOffsetU + mTextureCoords[n].x * fTScaleU;
		mTextureCoords[n].y = fTOffsetV + mTextureCoords[n].y * fTScaleV;
	}
#endif	
}

void SVIRoundPoly::buildExtraWork() {
	generateScaledVertices();
}

void SVIRoundPoly::generateScaledVertices() {
	SVIFloat fXDistance = mSize.x; // * mScale.x;
	SVIFloat fYDistance = mSize.y; // * mScale.y;
	SVIFloat fZDistance = mSize.z; // * mScale.z;

	SVIFloat fXRatio = mPivot.x;
	SVIFloat fYRatio = mPivot.y;
	SVIFloat fZRatio = mPivot.z;

	SVIFloat fXFilteredRatio = fXDistance * fXRatio;
	SVIFloat fYFilteredRatio = fYDistance * fYRatio;

	for (SVIUInt n = 0; n < mVerticeCount; ++n) {
		mVertices[n].x -= fXFilteredRatio;
		mVertices[n].y -= fYFilteredRatio;
		if (hasBorder()) {
			mShadowVertices[n].x -= fXFilteredRatio;
			mShadowVertices[n].y -= fYFilteredRatio;
		}
	}
}

void SVIRoundPoly::additionalRender(SVISlideTextureContainer * pContainer) {
	//2011-07-12 masterkeaton27@gmail.com
	if (!hasBorder())
		return;DEBUG_CHECK_LOGE("SVIRoundPoly::additionalRender()");

	SVIProgramParams renderParams;
	SVIProgram * pProgram = NULL;
	SVIProgramHandler * pHandler = NULL;
	
    if( (mBorderWidth != 0.0f)  && !(mBorderColor.w < 1.0f))
		pContainer = NULL;	// Fix for  avoiding Blend incase if corner radius & border color set

	SVIBool bUseTexture =
			pContainer != NULL && !pContainer->isEmpty() ? SVITRUE : SVIFALSE;
	SVIBool bUseAddBlend = isAddBlend();
	if (false) {//bUseTexture No texture needed on border.
		renderParams.addElement(isScaleToFit() ? EM_TEXTURE_SCALE : EM_TEXTURE);
	} else {
		renderParams.addElement(EM_COLOR);
	}

	if (bUseTexture)
		renderParams.addElement(bUseAddBlend ? EM_BLEND_ADD : EM_BLEND_MLT);

	pProgram =  mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
	if (pProgram == NULL || !pProgram->getActivated())
		return;
	pHandler = pProgram->getHandler();

	pProgram->use();

	pHandler->setUniform(HD_U_COLOR, mBorderColor);
	pHandler->setUniform(HD_U_OPACITY, mOpacity);

	//2011-08-01 masterkeaton27@gmail.com: reducing for loop
	if (bUseTexture) {
		SVITexture * pTexture = pContainer->getTexture();
		SVIVector4 colorValue = mBorderColor;
        if (pTexture != NULL) {
            if ( colorValue.w < 1.0f || pTexture->getHasAlpha() || (isAddBlend() && mOpacity < 1.0f) ) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            }
            else {
                glDisable(GL_BLEND);
            }
		} 
        else {
			glDisable(GL_BLEND);
		}
		pHandler->setSampler(HD_U_SVIMPLER_0, pContainer->getTextureID());
	} else {
		SVIVector4 colorValue = mBorderColor;
		if (colorValue.w < 1.0f) {
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

	//2011.12.07 jognchae.moon
	if (mBorderAACoord != NULL) {
		pHandler->setAttribute(HD_A_AACOORD, 1, mBorderAACoord);
	}
	//

	//set model view projection matrix
	SVIMatrix modelMat = getTransform();

	//2011-07-12 masterkeaton27@gmail.com
	//orthogonal setting
	SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();
	if (mIsOrthogonal) {
		projectionMat._34 = 0.0f;
	}
	//modelMat._34 = 1.0f / -10.0f;
	modelMat._43 -= cfTestDepth;

	pHandler->setUniform(HD_U_PROJ, projectionMat);
	pHandler->setUniform(HD_U_VIEW, modelMat);

	//2011.12.07 jongchae.moon
	if (getIsAntiAliasing()) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDrawElements(GL_TRIANGLES, mBorderIndicesCount, GL_UNSIGNED_SHORT,
				mBorderIndices);
		glDisable(GL_BLEND);
	} else {
		glDrawElements(GL_TRIANGLES, mBorderIndicesCount, GL_UNSIGNED_SHORT,
				mBorderIndices);
	}
	//

#if SVI_USING_CPU_TEXCOORD
	pHandler->releaseAttribute(HD_A_TEXCOORD);
#endif
	pHandler->releaseAttribute(HD_A_POSITION);
	//2011.12.07 jognchae.moon
	if (mBorderAACoord != NULL) {
		pHandler->releaseAttribute(HD_A_AACOORD);
	}
	//
}

/////////////////////////////////////////
//SVIRoundPolyExt

//2011.11.03 jongchae.moon
SVIRoundPolyExt::SVIRoundPolyExt(SVIGLSurface *surface) :
	SVIRoundPoly(surface){
	mIsAA = SVITRUE;
}

SVIRoundPolyExt::~SVIRoundPolyExt() {

}

SVIInt SVIRoundPolyExt::allocate(SVIInt verticeCount) {
	// jbkim 20111108
	SVIInt ret = SVIRoundPoly::allocate(verticeCount);

	mOriVerticeCount = mVerticeCount = verticeCount;

	return ret;
}

SVIInt SVIRoundPolyExt::allocate(SVIInt verticeCount, SVIInt indicesCount) {
	const SVIInt numCorners = 4;
	SVIInt ret = SVIRoundPoly::allocate(
			verticeCount + (mRoundLevel + 1) * numCorners,
			indicesCount + mRoundLevel * numCorners * 3 * 2 + 3 * 2 * 4);

	mOriVerticeCount = mVerticeCount = verticeCount;
	mOriIndicesCount = mIndicesCount = indicesCount;

	return ret;
}

void SVIRoundPolyExt::generateScaledVertices() {
	SVIFloat fXDistance = mSize.x; // * mScale.x;
	SVIFloat fYDistance = mSize.y; // * mScale.y;
	SVIFloat fZDistance = mSize.z; // * mScale.z;

	SVIFloat fXRatio = mPivot.x;
	SVIFloat fYRatio = mPivot.y;
	SVIFloat fZRatio = mPivot.z;

	SVIFloat fXFilteredRatio = fXDistance * fXRatio;
	SVIFloat fYFilteredRatio = fYDistance * fYRatio;

	for (SVIUInt n = 0; n < mVerticeCount; ++n) {
		mVertices[n].x -= fXFilteredRatio;
		mVertices[n].y -= fYFilteredRatio;
		if (hasBorder() && n < (SVIUInt)mOriVerticeCount) {
			mShadowVertices[n].x -= fXFilteredRatio;
			mShadowVertices[n].y -= fYFilteredRatio;
		}
	}
}

void SVIRoundPolyExt::buildVertices() {
	//Initialize index
	mfOriginBorderWidth = mBorderWidth;

	mBorderWidth += 0.5f;
	mVerticeCount = mOriVerticeCount;
	mIndicesCount = mOriIndicesCount;
	////////////////////////////
	SVIRoundPoly::buildVertices();

	//2011.11.11 jongchae.moon
	cornerAAPatch(SVI_ROUND_LEFT_TOP);
	cornerAAPatch(SVI_ROUND_LEFT_BOTTOM);
	cornerAAPatch(SVI_ROUND_RIGHT_BOTTOM);
	cornerAAPatch(SVI_ROUND_RIGHT_TOP);

	rectAAPatch();

	////////////////////////////
	//revert to original data
	mBorderWidth = mfOriginBorderWidth;
}

void SVIRoundPolyExt::cornerAAPatch(SVIInt index) {

	SVIInt startIndex = 0;
	SVIFloat fDeltaRadian = (SVI_PI * 0.5f) / (SVIFloat) mRoundLevel;
	SVIFloat fCurrentRadian = 0.0f;
	SVIFloat fNextRadian = fDeltaRadian;
	SVIFloat fRadius = mRoundRadius[index];
	SVIFloat fx = 1.0f, fy = 1.0f;
	SVIFloat cx = 1.0f, cy = 1.0f;

	// calculate outer vertices

	SVIFloat outerBoaderWidth = mBorderWidth - 1.0f;

	SVIInt numCenters = 0;

	switch (index) {
	case SVI_ROUND_LEFT_TOP:
		startIndex = mOriVerticeCount;
		fx = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - outerBoaderWidth);
		fy = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - outerBoaderWidth);
		cx = mRoundCenter[SVI_ROUND_LEFT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_TOP].y;
		numCenters = 0;

		mCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0] = startIndex
				- (mOriVerticeCount - numCenters);
		mCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][1] = startIndex;
		mCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0] = startIndex
				- (mOriVerticeCount - numCenters) + mRoundLevel;
		mCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][1] = startIndex
				+ mRoundLevel;
		break;
	case SVI_ROUND_LEFT_BOTTOM:
		startIndex = mOriVerticeCount + ((mRoundLevel + 1) * 1);
		fx = -(mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - outerBoaderWidth);
		fy = mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - outerBoaderWidth;
		cx = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y;
		numCenters = 1;

		mCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][0] = startIndex
				- (mOriVerticeCount - numCenters);
		mCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1] = startIndex;
		mCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][0] = startIndex
				- (mOriVerticeCount - numCenters) + mRoundLevel;
		mCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1] = startIndex
				+ mRoundLevel;
		break;
	case SVI_ROUND_RIGHT_BOTTOM:
		startIndex = mOriVerticeCount + ((mRoundLevel + 1) * 2);
		fx = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - outerBoaderWidth;
		fy = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - outerBoaderWidth;
		cx = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].y;
		numCenters = 2;
		mCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][0] = startIndex
				- (mOriVerticeCount - numCenters);
		mCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1] = startIndex;
		mCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0] = startIndex
				- (mOriVerticeCount - numCenters) + mRoundLevel;
		mCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][1] = startIndex
				+ mRoundLevel;
		break;
	case SVI_ROUND_RIGHT_TOP:
		startIndex = mOriVerticeCount + ((mRoundLevel + 1) * 3);
		fx = mRoundRadius[SVI_ROUND_RIGHT_TOP] - outerBoaderWidth;
		fy = -(mRoundRadius[SVI_ROUND_RIGHT_TOP] - outerBoaderWidth);
		cx = mRoundCenter[SVI_ROUND_RIGHT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_TOP].y;
		numCenters = 3;
		mCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0] = startIndex
				- (mOriVerticeCount - numCenters);
		mCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][1] = startIndex;
		mCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][0] = startIndex
				- (mOriVerticeCount - numCenters) + mRoundLevel;
		mCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1] = startIndex
				+ mRoundLevel;
		break;
	}

	SVIVector3 vA, vB;
	vB = SVIVector3(cx, cy, 0.0f);

	//add vertices to mVertices
	SVIInt deltaindex = startIndex;
	for (SVIUInt n = 0; n < mRoundLevel + 1; ++n) {
		SVIFloat px = cosf(fCurrentRadian);
		SVIFloat py = sinf(fCurrentRadian);
		vA.x = cx + px * fx;
		vA.y = cy + py * fy;
		vA.z = 0.0f;
		fCurrentRadian += fDeltaRadian;
		mVertices[deltaindex].x = vA.x;
		mVertices[deltaindex].y = vA.y;
		mVertices[deltaindex].z = vA.z;
		mAACoord[deltaindex] = 0.0f;

		deltaindex++;
		mVerticeCount++;
	}

	SVIInt curIndex = startIndex;
	SVIInt cur_mIndices = mIndicesCount;

	SVIInt offset = mOriVerticeCount - numCenters;
	// generate triangles using indices
	for (SVIUInt n = 0; n < mRoundLevel; ++n) {
		mIndices[cur_mIndices++] = curIndex - offset;
		mIndices[cur_mIndices++] = curIndex;
		mIndices[cur_mIndices++] = curIndex + 1;

		mIndices[cur_mIndices++] = curIndex - offset;
		mIndices[cur_mIndices++] = curIndex + 1;
		mIndices[cur_mIndices++] = curIndex - offset + 1;

		curIndex++;
		mIndicesCount += 6;
	}
}

void SVIRoundPolyExt::rectAAPatch() {
	// left rect
	mIndices[mIndicesCount++] = mCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0];
	mIndices[mIndicesCount++] = mCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][1];
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1];

	mIndices[mIndicesCount++] = mCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1];

	// upper rect
	mIndices[mIndicesCount++] = mCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0];
	mIndices[mIndicesCount++] = mCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][1];
	mIndices[mIndicesCount++] = mCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1];

	mIndices[mIndicesCount++] = mCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0];
	mIndices[mIndicesCount++] = mCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][0];
	mIndices[mIndicesCount++] = mCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1];

	// right rect
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][1];
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1];

	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1];

	// bottom rect
	mIndices[mIndicesCount++] =
			mCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][1];
	mIndices[mIndicesCount++] =
			mCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1];

	mIndices[mIndicesCount++] =
			mCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][0];
	mIndices[mIndicesCount++] =
			mCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1];
}

//2011.11.14 jongchae.moon
void SVIRoundPolyExt::allocateBorderDatas() {
	SVIInt cornerCount = 4;
	mBorderOriIndicesCount = mRoundLevel * (6 * cornerCount)
			+ (6 * cornerCount);
	mBorderOriVerticeCount = (mRoundLevel + 1) * 2 * cornerCount
			+ (4 * cornerCount);

	mBorderIndicesCount = mBorderOriIndicesCount;
	mBorderVerticeCount = mBorderOriVerticeCount;

#if SVI_USING_CPU_TEXCOORD
	if (mBorderVertices != NULL && mBorderTexcoords != NULL && mBorderIndices != NULL) return;
#else
	if (mBorderVertices != NULL && mBorderIndices != NULL)
		return;
#endif

	mBorderIndicesCount = mRoundLevel * (6 * cornerCount) + (6 * cornerCount)
			+ (mRoundLevel * cornerCount * 3 * 2 + 3 * 2 * 4) * 2;
	mBorderVerticeCount = (mRoundLevel + 1) * 2 * cornerCount
			+ (4 * cornerCount) + (mRoundLevel + 1) * cornerCount * 2;

	mBorderVertices = new SVIVector3[mBorderVerticeCount];
#if SVI_USING_CPU_TEXCOORD
	mBorderTexcoords = new SVIVector2[mBorderVerticeCount];
	mMemoryUsed += sizeof(SVIVector2) * mBorderVerticeCount;
#endif
	mMemoryUsed += sizeof(SVIVector3) * mBorderVerticeCount;

	mBorderIndices = new SVIUShort[mBorderIndicesCount];
	mMemoryUsed += sizeof(SVIUShort) * mBorderIndicesCount;

	if (mBorderAACoord == NULL) {
		SVIInt nBorderAACoordCount = mBorderVerticeCount;
		mBorderAACoord = new SVIFloat[nBorderAACoordCount];
		for (SVIInt i = 0; i < nBorderAACoordCount; i++) {
			mBorderAACoord[i] = 1.0f;
		}
	}

	mBorderIndicesCount = mBorderOriIndicesCount;
	mBorderVerticeCount = mBorderOriVerticeCount;
}

void SVIRoundPolyExt::borderSetup() {
	
	allocateBorderDatas();

	
	SVIFloat forAA = 0.5f;

	//generate left top corner
	generateBorderCornerVertices(SVI_ROUND_LEFT_TOP, forAA);
	generateBorderCornerVertices(SVI_ROUND_LEFT_BOTTOM, forAA);
	generateBorderCornerVertices(SVI_ROUND_RIGHT_BOTTOM, forAA);
	SVIInt startIndex = generateBorderCornerVertices(SVI_ROUND_RIGHT_TOP, forAA)
			+ 2;
	borderSetup_InnerRect(startIndex, forAA);

	
	forAA = -0.5f;
	borderCornerAAPatch(SVI_ROUND_LEFT_TOP, forAA);
	borderCornerAAPatch(SVI_ROUND_LEFT_BOTTOM, forAA);
	borderCornerAAPatch(SVI_ROUND_RIGHT_BOTTOM, forAA);
	borderCornerAAPatch(SVI_ROUND_RIGHT_TOP, forAA);
	borderRectAAPatch(SVIFALSE);
	borderRectAAPatch(SVITRUE);

	
	SVIFloat fXRatio = mPivot.x;
	SVIFloat fYRatio = mPivot.y;
	SVIFloat fZRatio = mPivot.z;

	SVIFloat fXDist = mSize.x * mScale.x;
	SVIFloat fYDist = mSize.y * mScale.y;

	for (SVIUInt n = 0; n < mBorderVerticeCount; ++n) {
#if SVI_USING_CPU_TEXCOORD
		mBorderTexcoords[n].x = mBorderVertices[n].x / fXDist;
		mBorderTexcoords[n].y = mBorderVertices[n].y / fYDist;
#endif
		mBorderVertices[n].x -= mSize.x * fXRatio;
		mBorderVertices[n].y -= mSize.y * fYRatio;
	}
}

void SVIRoundPolyExt::borderSetup_InnerRect(SVIInt startIndex, SVIFloat forAA) {
	SVIInt startBufferIndex = (mRoundLevel * 4 * 3 * 2);
	//left
	SVIFloat fXDist = mSize.x;
	SVIFloat fYDist = mSize.y;
	SVIFloat cx, cy, cw, ch;

	SVIFloat fOriginalBorderWidth = mBorderWidth;
	mBorderWidth = mBorderWidth - forAA * 2;

	if (mRoundRadius[SVI_ROUND_RIGHT_TOP] * 2.0f < fYDist) {
		//left
		cx = 0.0f + forAA;
		cy = mRoundRadius[SVI_ROUND_LEFT_TOP];
		cw = mBorderWidth;
		ch = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y
				- mRoundCenter[SVI_ROUND_LEFT_TOP].y;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);

		//right
		cx = fXDist - (mBorderWidth + forAA);
		cy = mRoundRadius[SVI_ROUND_RIGHT_TOP];
		cw = mBorderWidth;
		ch = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y
				- mRoundCenter[SVI_ROUND_LEFT_TOP].y;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);
	} else {
		//mBorderVerticeCount -= 12;
		mBorderIndicesCount -= 12;
		mBorderVerticeCount -= 8;
	}
	if (mRoundRadius[SVI_ROUND_RIGHT_TOP] * 2.0f < fXDist) {
		//top
		cx = mRoundRadius[SVI_ROUND_RIGHT_TOP];
		cy = 0.0f + forAA;
		cw = mRoundCenter[SVI_ROUND_RIGHT_TOP].x
				- mRoundCenter[SVI_ROUND_LEFT_TOP].x;
		ch = mBorderWidth;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);

		//bottom
		cx = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM];
		cy = fYDist - (mBorderWidth + forAA);
		cw = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].x
				- mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
		ch = mBorderWidth;
		startIndex = generateBorderRectangle(startIndex, &startBufferIndex, cx,
				cy, cw, ch);
	} else {
		//mBorderVerticeCount -= 12;
		mBorderIndicesCount -= 12;
		mBorderVerticeCount -= 8;
	}
	mBorderWidth = fOriginalBorderWidth;
}

SVIInt SVIRoundPolyExt::borderCornerAAPatch(SVIInt index, SVIFloat forAA) {
	SVIFloat fXDist = mSize.x * mScale.x;
	SVIFloat fYDist = mSize.y * mScale.y;
	SVIFloat fNormal = (fXDist > fYDist) ? fYDist : fXDist;
	SVIInt startIndex = 0;
	SVIFloat fDeltaRadian = (SVI_PI * 0.5f) / (SVIFloat) mRoundLevel;
	SVIFloat fCurrentRadian = 0.0f;
	SVIFloat fNextRadian = fDeltaRadian;
	SVIFloat fRadius = mRoundRadius[index];
	SVIFloat fx = 1.0f, fy = 1.0f;
	SVIFloat cx = 1.0f, cy = 1.0f;

	switch (index) {
	case SVI_ROUND_LEFT_TOP:
		startIndex = mBorderOriVerticeCount;
		fx = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - forAA);
		fy = -(mRoundRadius[SVI_ROUND_LEFT_TOP] - forAA);
		cx = mRoundCenter[SVI_ROUND_LEFT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_TOP].y;

		mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0] = startIndex
				- mBorderOriVerticeCount;
		mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][1] = startIndex;
		mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0] = startIndex
				- mBorderOriVerticeCount + mRoundLevel * 2;
		mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][1] = startIndex
				+ mRoundLevel * 2;

		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0] = startIndex
				- mBorderOriVerticeCount + 1;
		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][1] = startIndex
				+ 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0] = startIndex
				- mBorderOriVerticeCount + mRoundLevel * 2 + 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][1] = startIndex
				+ mRoundLevel * 2 + 1;

		break;
	case SVI_ROUND_LEFT_BOTTOM:
		startIndex = mBorderOriVerticeCount + ((mRoundLevel + 1) * 1 * 2);
		fx = -(mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - forAA);
		fy = mRoundRadius[SVI_ROUND_LEFT_BOTTOM] - forAA;
		cx = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_LEFT_BOTTOM].y;

		mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][0] = startIndex
				- mBorderOriVerticeCount;
		mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1] = startIndex;
		mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][0] = startIndex
				- mBorderOriVerticeCount + mRoundLevel * 2;
		mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1] = startIndex
				+ mRoundLevel * 2;

		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][0] =
				startIndex - mBorderOriVerticeCount + 1;
		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1] =
				startIndex + 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][0] =
				startIndex - mBorderOriVerticeCount + mRoundLevel * 2 + 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1] =
				startIndex + mRoundLevel * 2 + 1;

		break;
	case SVI_ROUND_RIGHT_BOTTOM:
		startIndex = mBorderOriVerticeCount + ((mRoundLevel + 1) * 2 * 2);
		fx = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - forAA;
		fy = mRoundRadius[SVI_ROUND_RIGHT_BOTTOM] - forAA;
		cx = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_BOTTOM].y;

		mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][0] = startIndex
				- mBorderOriVerticeCount;
		mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1] = startIndex;
		mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0] = startIndex
				- mBorderOriVerticeCount + mRoundLevel * 2;
		mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][1] = startIndex
				+ mRoundLevel * 2;

		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][0] =
				startIndex - mBorderOriVerticeCount + 1;
		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1] =
				startIndex + 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0] =
				startIndex - mBorderOriVerticeCount + mRoundLevel * 2 + 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][1] =
				startIndex + mRoundLevel * 2 + 1;

		break;
	case SVI_ROUND_RIGHT_TOP:
		startIndex = mBorderOriVerticeCount + ((mRoundLevel + 1) * 3 * 2);
		fx = mRoundRadius[SVI_ROUND_RIGHT_TOP] - forAA;
		fy = -(mRoundRadius[SVI_ROUND_RIGHT_TOP] - forAA);
		cx = mRoundCenter[SVI_ROUND_RIGHT_TOP].x;
		cy = mRoundCenter[SVI_ROUND_RIGHT_TOP].y;

		mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0] = startIndex
				- mBorderOriVerticeCount;
		mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][1] = startIndex;
		mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][0] = startIndex
				- mBorderOriVerticeCount + mRoundLevel * 2;
		mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1] = startIndex
				+ mRoundLevel * 2;

		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0] =
				startIndex - mBorderOriVerticeCount + 1;
		mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][1] =
				startIndex + 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][0] = startIndex
				- mBorderOriVerticeCount + mRoundLevel * 2 + 1;
		mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1] = startIndex
				+ mRoundLevel * 2 + 1;

		break;
	}

	SVIVector3 vA;
	SVIVector3 vtA, vB = SVIVector3(cx, cy, 0.0f);

	SVIInt deltaindex = startIndex;

	for (SVIUInt n = 0; n < mRoundLevel + 1; ++n) {
		SVIFloat px = cosf(fCurrentRadian) * fx;
		SVIFloat py = sinf(fCurrentRadian) * fy;

		vA = SVIVector3(cx + px, cy + py, 0.0f);
		vtA = vB - vA;
		vtA.normalize();
		vtA.setlength(mBorderWidth + 1.0f);
		vtA = vA + vtA;

		fCurrentRadian += fDeltaRadian;

		//generate vertex list
		mBorderVertices[deltaindex + 0] = vA;
		mBorderVertices[deltaindex + 1] = vtA;

		//mBorderVertices[deltaindex+0] = SVIVector3(0.0f, 0.0f, 0.0f);
		//mBorderVertices[deltaindex+1] = SVIVector3(0.0f, 0.0f, 0.0f);

		mBorderAACoord[deltaindex + 0] = 0.0f;
		mBorderAACoord[deltaindex + 1] = 0.0f;

		deltaindex += 2;
		mBorderVerticeCount += 2;
	}

	//mBorderVerticeCount = mBorderOriVerticeCount + 4;

	SVIInt curIndex = startIndex;
	SVIInt cur_mIndices = mBorderIndicesCount;
	SVIInt offset = mBorderOriVerticeCount;

	//LOGE(	"mBorderOriVerticeCount: %d		/	 mBorderIndicesCount: %d", mBorderOriVerticeCount, mBorderIndicesCount);

	for (SVIUInt n = 0; n < mRoundLevel; ++n) {
		
		mBorderIndices[cur_mIndices++] = curIndex - offset;
		mBorderIndices[cur_mIndices++] = curIndex;
		mBorderIndices[cur_mIndices++] = curIndex + 2;

		mBorderIndices[cur_mIndices++] = curIndex - offset;
		mBorderIndices[cur_mIndices++] = curIndex + 2;
		mBorderIndices[cur_mIndices++] = curIndex - offset + 2;	

		
		mBorderIndices[cur_mIndices++] = curIndex - offset + 1;
		mBorderIndices[cur_mIndices++] = curIndex + 1;
		mBorderIndices[cur_mIndices++] = curIndex + 1 + 2;

		mBorderIndices[cur_mIndices++] = curIndex - offset + 1;
		mBorderIndices[cur_mIndices++] = curIndex + 1 + 2;
		mBorderIndices[cur_mIndices++] = curIndex - offset + 1 + 2;

		//deltaindex +=2;
		curIndex += 2;

		mBorderIndicesCount += 12;
	}

	//set next vertices
	return deltaindex;
}

void SVIRoundPolyExt::borderRectAAPatch(SVIBool bInside) {
	if (!bInside) {
		// left rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1];

		// upper rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1];

		// right rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1];

		// bottom rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1];
	} else {
		// left rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_LEFT_BOTTOM][1];

		// upper rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_TOP][1];

		// right rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_TOP][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusStartIndices[SVI_ROUND_RIGHT_BOTTOM][1];

		// bottom rect
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][1];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1];

		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_RIGHT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][0];
		mBorderIndices[mBorderIndicesCount++] =
				mBorderInsideCornerRadiusEndIndices[SVI_ROUND_LEFT_BOTTOM][1];
	}
}

}
