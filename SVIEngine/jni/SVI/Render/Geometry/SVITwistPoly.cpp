#include "../../SVICores.h"
#include "../../Physics/SVIPhysics.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"

#include "SVIPoly.h"
#include "SVITwistPoly.h"


namespace SVI {

static const SVIBool DEBUG = SVIFALSE;

/****************************************************************************/
// twist polygon object	2011-11-28 masterkeaton27@gmail.com
/****************************************************************************/

SVITwistPoly::SVITwistPoly(SVIGLSurface* saGLSurface) :
	SVIPoly(saGLSurface) {

	mOutpit = NULL;
	mHasAdditionalUpdate = SVITRUE;
	mHasBackFace = SVITRUE;
	mNeedTexcoords = SVITRUE;
	mNeedNormals = SVITRUE;
	mNeedIndices = SVITRUE;
	mPhysics = new SVITwistSimulator();
    setPrimitiveType(SVI_POLY_TRI_LIST);
}

SVITwistPoly::~SVITwistPoly() {
	SVI_SVIFE_DELETE(mPhysics);
}

void SVITwistPoly::setOutfit(SVISlideOutfit * pOutpit) {
	mOutpit = pOutpit;
}

void SVITwistPoly::buildVertices() {

}

void SVITwistPoly::buildTextureCoordinates() {

}

void SVITwistPoly::buildExtraWork() {

}

void SVITwistPoly::buildIndices() {

}

void SVITwistPoly::additionalUpdate() {
	if (mOutpit) {
		mPhysics->setControlTarget(mOutpit->mDeformRatio);
	} else {
		mPhysics->setControlTarget(mSVIGLSurface->getRenderer()->mTouchPosition);
	}
	mPhysics->update();
}

void SVITwistPoly::additionalRender(SVISlideTextureContainer * pContainer) {

}

void SVITwistPoly::prepareBackFace() {
	SVIFloat fWidth = mSize.x;
	SVIFloat fHeight = mSize.y;
	SVIBool isFlipped = SVITRUE;
	SVIBool isVertical =
			(mOutpit != NULL
					&& (mOutpit->mDeformDirection == DEFORM_VERTICAL
							|| mOutpit->mDeformDirection
									== DEFORM_VERTICAL_REVERSE)) ?
					SVITRUE : SVIFALSE;
	SVIBool isReverse =
			(mOutpit != NULL
					&& (mOutpit->mDeformDirection == DEFORM_HORIZONTAL_REVERSE
							|| mOutpit->mDeformDirection
									== DEFORM_VERTICAL_REVERSE)) ?
					SVITRUE : SVIFALSE;

	SVIVector3 vSize = SVIVector3(mSize.x, mSize.y, mSize.x * 0.7f);
	SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, -mPivot.y * vSize.y,
			0.0f);
	mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
			mTextureCoords, isVertical, isReverse, isFlipped);
}

void SVITwistPoly::setup() {
	allocate(mPhysics->getVerticesCount(), mPhysics->getIndicesCount(),
			mPhysics->getVerticesCount());
}

void SVITwistPoly::prepareFrontFace() {
	SVIFloat fWidth = mSize.x;
	SVIFloat fHeight = mSize.y;
	SVIBool isFlipped = SVIFALSE;
	SVIBool isVertical =
			(mOutpit != NULL
					&& (mOutpit->mDeformDirection == DEFORM_VERTICAL
							|| mOutpit->mDeformDirection
									== DEFORM_VERTICAL_REVERSE)) ?
					SVITRUE : SVIFALSE;
	SVIBool isReverse =
			(mOutpit != NULL
					&& (mOutpit->mDeformDirection == DEFORM_HORIZONTAL_REVERSE
							|| mOutpit->mDeformDirection
									== DEFORM_VERTICAL_REVERSE)) ?
					SVITRUE : SVIFALSE;
	SVIVector3 vSize = SVIVector3(mSize.x, mSize.y, mSize.x * 0.7f);
	SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, -mPivot.y * vSize.y,
			0.0f);
	mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
			mTextureCoords, isVertical, isReverse, isFlipped);
}

}
