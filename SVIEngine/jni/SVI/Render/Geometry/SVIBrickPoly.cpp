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
#include "SVIBrickPoly.h"

namespace SVI {

	const float cfTestDepth = 1.0f;
	static const SVIBool DEBUG = SVIFALSE;

	SVIBrickPoly::SVIBrickPoly(SVIGLSurface* surface) :
		SVIPoly(surface) {

		mRenderPritiveType = SVI_POLY_TRI_LIST;
		mHasAdditionalRender = SVIFALSE;	

		mHasBackFace = SVITRUE;
		mNeedTexcoords = SVITRUE; 
		mNeedNormals = SVITRUE;
		mNeedIndices = SVITRUE;

		mOutpit = NULL;
	}

	void SVIBrickPoly::setOutfit(SVISlideOutfit * pOutpit) {
		mOutpit = pOutpit;
	}

	void SVIBrickPoly::setup() {
		SVIUInt uVerticesCount = 16;
		SVIUInt uNormalsCount = uVerticesCount;
		SVIUInt uIndicesCount = 24;

		//2011-07-04 masterkeaton27@gmail.com
		//index buffer memory leak bug fixed.
		allocate(uVerticesCount, uIndicesCount, uNormalsCount);
	}

	SVIBrickPoly::~SVIBrickPoly() {	
	}


	void SVIBrickPoly::buildIndices() {
		if (mIndices == NULL)
			return;

	}

	void SVIBrickPoly::buildVertices() {
		if (mVertices == NULL)
			return;

	}

	void SVIBrickPoly::buildTextureCoordinates() {
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

	void SVIBrickPoly::buildExtraWork() {

	}

	void SVIBrickPoly::additionalRender(SVISlideTextureContainer * pContainer) {

	}

	void SVIBrickPoly::prepareBackFace(){
		mIndicesCount = 24;
		generateOutside();
	}

	void SVIBrickPoly::prepareFrontFace(){
		mIndicesCount = 12;
		generateFront();
	}

	void SVIBrickPoly::generateFront(){
		SVIFloat fXDistance = mSize.x; // * mScale.x;
		SVIFloat fYDistance = mSize.y; // * mScale.y;
		SVIFloat fZDistance = mOutpit != NULL ? mOutpit->mDeformRatio.x : mSize.x;//mSize.z; // * mScale.z;

		SVIFloat fXRatio = mPivot.x;
		SVIFloat fYRatio = mPivot.y;
		SVIFloat fZRatio = mOutpit != NULL ? mOutpit->mDeformRatio.y : 0.5f;//mPivot.z;
						
		SVIFloat fInvXRatio = 1.0f - fXRatio;
		SVIFloat fInvYRatio = 1.0f - fYRatio;
		SVIFloat fInvZRatio = 1.0f - fZRatio;

		//positions
		mVertices[0] = SVIVector3(fInvXRatio * fXDistance, -fYRatio * fYDistance, -fZRatio * fZDistance);
		mVertices[1] = SVIVector3(-fXRatio * fXDistance, fInvYRatio * fYDistance, -fZRatio * fZDistance);
		mVertices[2] = SVIVector3(fInvXRatio * fXDistance, fInvYRatio * fYDistance, -fZRatio * fZDistance);
		mVertices[3] = SVIVector3(-fXRatio * fXDistance, -fYRatio * fYDistance, -fZRatio * fZDistance);

		//texture coordinates
		mTextureCoords[0] = SVIVector2(1.0f,0.0f);
		mTextureCoords[1] = SVIVector2(0.0f,1.0f);
		mTextureCoords[2] = SVIVector2(1.0f,1.0f);
		mTextureCoords[3] = SVIVector2(0.0f,0.0f);

		//normals
		mNormals[0] = SVIVector3(0,0,1.0f);
		mNormals[1] = SVIVector3(0,0,1.0f);
		mNormals[2] = SVIVector3(0,0,1.0f);
		mNormals[3] = SVIVector3(0,0,1.0f);
		
		//indices
		mIndices[0] = 0;
		mIndices[1] = 1;
		mIndices[2] = 2;
		mIndices[3] = 0;
		mIndices[4] = 3;
		mIndices[5] = 1;


		SVIUInt uVertexIdx = 4;
		SVIUInt uIndexIdx = 6;

		//positions
		mVertices[uVertexIdx+0] = SVIVector3(-fXRatio * fXDistance, -fYRatio * fYDistance, fInvZRatio * fZDistance);
		mVertices[uVertexIdx+1] = SVIVector3(fInvXRatio * fXDistance, fInvYRatio * fYDistance,	fInvZRatio * fZDistance);
		mVertices[uVertexIdx+2] = SVIVector3(-fXRatio * fXDistance, fInvYRatio * fYDistance, fInvZRatio * fZDistance);
		mVertices[uVertexIdx+3] = SVIVector3(fInvXRatio * fXDistance, -fYRatio * fYDistance, fInvZRatio * fZDistance);

		//texture coordinates
		mTextureCoords[uVertexIdx+0] = SVIVector2(1.0f,0.0f);
		mTextureCoords[uVertexIdx+1] = SVIVector2(0.0f,1.0f);
		mTextureCoords[uVertexIdx+2] = SVIVector2(1.0f,1.0f);
		mTextureCoords[uVertexIdx+3] = SVIVector2(0.0f,0.0f);

		//normals
		mNormals[uVertexIdx+0] = SVIVector3(0,0,-1.0f);
		mNormals[uVertexIdx+1] = SVIVector3(0,0,-1.0f);
		mNormals[uVertexIdx+2] = SVIVector3(0,0,-1.0f);
		mNormals[uVertexIdx+3] = SVIVector3(0,0,-1.0f);

		//indices
		mIndices[uIndexIdx+0] = uVertexIdx+0;
		mIndices[uIndexIdx+1] = uVertexIdx+1;
		mIndices[uIndexIdx+2] = uVertexIdx+2;
		mIndices[uIndexIdx+3] = uVertexIdx+0;
		mIndices[uIndexIdx+4] = uVertexIdx+3;
		mIndices[uIndexIdx+5] = uVertexIdx+1;
	}

	void SVIBrickPoly::generateBack(){
		
	}

	void SVIBrickPoly::generateOutside(){
		SVIFloat fXDistance = mSize.x; // * mScale.x;
		SVIFloat fYDistance = mSize.y; // * mScale.y;
		SVIFloat fZDistance = mOutpit != NULL ? mOutpit->mDeformRatio.x : mSize.x;//mSize.z; // * mScale.z;

		SVIFloat fXRatio = mPivot.x;
		SVIFloat fYRatio = mPivot.y;
		SVIFloat fZRatio = mOutpit != NULL ? mOutpit->mDeformRatio.y : 0.5f;//mPivot.z;

		SVIFloat fInvXRatio = 1.0f - fXRatio;
		SVIFloat fInvYRatio = 1.0f - fYRatio;
		SVIFloat fInvZRatio = 1.0f - fZRatio;

		SVIUInt uVertexIdx = 0;
		SVIUInt uIndexIdx = 0;

		//Top
		//positions
		mVertices[uVertexIdx+0] = SVIVector3(-fXRatio * fXDistance, fInvYRatio * fYDistance, fInvZRatio * fZDistance);
		mVertices[uVertexIdx+1] = SVIVector3(fInvXRatio * fXDistance, fInvYRatio * fYDistance,	-fZRatio * fZDistance);
		mVertices[uVertexIdx+2] = SVIVector3(-fXRatio * fXDistance, fInvYRatio * fYDistance, -fZRatio * fZDistance);
		mVertices[uVertexIdx+3] = SVIVector3(fInvXRatio * fXDistance, fInvYRatio * fYDistance, fInvZRatio * fZDistance);

		//texture coordinates
		mTextureCoords[uVertexIdx+0] = SVIVector2(1.0f,1.0f);
		mTextureCoords[uVertexIdx+1] = SVIVector2(0.0f,0.0f);
		mTextureCoords[uVertexIdx+2] = SVIVector2(1.0f,0.0f);
		mTextureCoords[uVertexIdx+3] = SVIVector2(0.0f,1.0f);

		//normals
		mNormals[uVertexIdx+0] = SVIVector3(0,1,0.0f);
		mNormals[uVertexIdx+1] = SVIVector3(0,1,0.0f);
		mNormals[uVertexIdx+2] = SVIVector3(0,1,0.0f);
		mNormals[uVertexIdx+3] = SVIVector3(0,1,0.0f);

		//indices
		mIndices[uIndexIdx+0] = uVertexIdx+0;
		mIndices[uIndexIdx+1] = uVertexIdx+1;
		mIndices[uIndexIdx+2] = uVertexIdx+2;
		mIndices[uIndexIdx+3] = uVertexIdx+0;
		mIndices[uIndexIdx+4] = uVertexIdx+3;
		mIndices[uIndexIdx+5] = uVertexIdx+1;

		uVertexIdx += 4;
		uIndexIdx += 6;

		//Bottom
		//positions
		mVertices[uVertexIdx+0] = SVIVector3(-fXRatio * fXDistance, -fYRatio * fYDistance, fInvZRatio * fZDistance);
		mVertices[uVertexIdx+1] = SVIVector3(fInvXRatio * fXDistance, -fYRatio * fYDistance,	-fZRatio * fZDistance);
		mVertices[uVertexIdx+2] = SVIVector3(-fXRatio * fXDistance, -fYRatio * fYDistance, -fZRatio * fZDistance);
		mVertices[uVertexIdx+3] = SVIVector3(fInvXRatio * fXDistance, -fYRatio * fYDistance, fInvZRatio * fZDistance);

		//texture coordinates
		mTextureCoords[uVertexIdx+0] = SVIVector2(1.0f,1.0f);
		mTextureCoords[uVertexIdx+1] = SVIVector2(0.0f,0.0f);
		mTextureCoords[uVertexIdx+2] = SVIVector2(1.0f,0.0f);
		mTextureCoords[uVertexIdx+3] = SVIVector2(0.0f,1.0f);

		//normals
		mNormals[uVertexIdx+0] = SVIVector3(0,-1,0.0f);
		mNormals[uVertexIdx+1] = SVIVector3(0,-1,0.0f);
		mNormals[uVertexIdx+2] = SVIVector3(0,-1,0.0f);
		mNormals[uVertexIdx+3] = SVIVector3(0,-1,0.0f);

		//indices
		mIndices[uIndexIdx+0] = uVertexIdx+0;
		mIndices[uIndexIdx+1] = uVertexIdx+2;
		mIndices[uIndexIdx+2] = uVertexIdx+1;
		mIndices[uIndexIdx+3] = uVertexIdx+0;
		mIndices[uIndexIdx+4] = uVertexIdx+1;
		mIndices[uIndexIdx+5] = uVertexIdx+3;

		uVertexIdx += 4;
		uIndexIdx += 6;


		//Left
		//positions
		mVertices[uVertexIdx+0] = SVIVector3(fInvXRatio * fXDistance, -fYRatio * fYDistance, fInvZRatio * fZDistance);
		mVertices[uVertexIdx+1] = SVIVector3(fInvXRatio * fXDistance, fInvYRatio * fYDistance,	-fZRatio * fZDistance);
		mVertices[uVertexIdx+2] = SVIVector3(fInvXRatio * fXDistance, -fYRatio * fYDistance, -fZRatio * fZDistance);
		mVertices[uVertexIdx+3] = SVIVector3(fInvXRatio * fXDistance, fInvYRatio * fYDistance, fInvZRatio * fZDistance);

		//texture coordinates
		mTextureCoords[uVertexIdx+2] = SVIVector2(0.0f,0.0f);
		mTextureCoords[uVertexIdx+3] = SVIVector2(1.0f,1.0f);
		mTextureCoords[uVertexIdx+1] = SVIVector2(0.0f,1.0f);
		mTextureCoords[uVertexIdx+0] = SVIVector2(1.0f,0.0f);

		//normals
		mNormals[uVertexIdx+0] = SVIVector3(1,0,0);
		mNormals[uVertexIdx+1] = SVIVector3(1,0,0);
		mNormals[uVertexIdx+2] = SVIVector3(1,0,0);
		mNormals[uVertexIdx+3] = SVIVector3(1,0,0);

		//indices
		mIndices[uIndexIdx+0] = uVertexIdx+0;
		mIndices[uIndexIdx+1] = uVertexIdx+2;
		mIndices[uIndexIdx+2] = uVertexIdx+1;
		mIndices[uIndexIdx+3] = uVertexIdx+0;
		mIndices[uIndexIdx+4] = uVertexIdx+1;
		mIndices[uIndexIdx+5] = uVertexIdx+3;

		uVertexIdx += 4;
		uIndexIdx += 6;
		
		//Right
		//positions
		mVertices[uVertexIdx+0] = SVIVector3(-fXRatio * fXDistance, -fYRatio * fYDistance, fInvZRatio * fZDistance);
		mVertices[uVertexIdx+1] = SVIVector3(-fXRatio * fXDistance, fInvYRatio * fYDistance,	-fZRatio * fZDistance);
		mVertices[uVertexIdx+2] = SVIVector3(-fXRatio * fXDistance, -fYRatio * fYDistance, -fZRatio * fZDistance);
		mVertices[uVertexIdx+3] = SVIVector3(-fXRatio * fXDistance, fInvYRatio * fYDistance, fInvZRatio * fZDistance);

		//texture coordinates
		//texture coordinates
		mTextureCoords[uVertexIdx+2] = SVIVector2(0.0f,0.0f);
		mTextureCoords[uVertexIdx+3] = SVIVector2(1.0f,1.0f);
		mTextureCoords[uVertexIdx+1] = SVIVector2(0.0f,1.0f);
		mTextureCoords[uVertexIdx+0] = SVIVector2(1.0f,0.0f);

		//normals
		mNormals[uVertexIdx+0] = SVIVector3(-1,0,0.0f);
		mNormals[uVertexIdx+1] = SVIVector3(-1,0,0.0f);
		mNormals[uVertexIdx+2] = SVIVector3(-1,0,0.0f);
		mNormals[uVertexIdx+3] = SVIVector3(-1,0,0.0f);

		//indices
		mIndices[uIndexIdx+0] = uVertexIdx+0;
		mIndices[uIndexIdx+1] = uVertexIdx+1;
		mIndices[uIndexIdx+2] = uVertexIdx+2;
		mIndices[uIndexIdx+3] = uVertexIdx+0;
		mIndices[uIndexIdx+4] = uVertexIdx+3;
		mIndices[uIndexIdx+5] = uVertexIdx+1;

		uVertexIdx += 4;
		uIndexIdx += 6;

	}
}
