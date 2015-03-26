#include "../../SVICores.h"
#include "../../Physics/SVIPhysics.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"

#include "SVIPoly.h"
#include "SVIDrumPoly.h"


namespace SVI {

static const SVIBool DEBUG = SVIFALSE;

/****************************************************************************/
// twist polygon object	2011-11-28 masterkeaton27@gmail.com
/****************************************************************************/

SVIDrumPoly::SVIDrumPoly(SVIGLSurface* saGLSurface):
	SVIPoly(saGLSurface){

	mOutpit = NULL;
	mHasAdditionalUpdate = SVITRUE;
	mHasBackFace = SVITRUE;
	mNeedTexcoords = SVITRUE;
	mNeedNormals = SVITRUE;
	mNeedIndices = SVITRUE;
	mPhysics = new SVIDrumSimulator();
	setPrimitiveType(SVI_POLY_TRI_LIST);
}

SVIDrumPoly::~SVIDrumPoly() {
	SVI_SVIFE_DELETE(mPhysics);
}

void SVIDrumPoly::setOutfit(SVISlideOutfit * pOutpit) {
	mOutpit = pOutpit;
}

void SVIDrumPoly::buildVertices() {

}

void SVIDrumPoly::buildTextureCoordinates() {

}

void SVIDrumPoly::buildExtraWork() {

}

void SVIDrumPoly::buildIndices() {

}

void SVIDrumPoly::additionalUpdate() {
    if (mOutpit) {
        mPhysics->setControlTarget(mOutpit->mDeformRatio);
    } else {
        mPhysics->setControlTarget(mSVIGLSurface->getRenderer()->mTouchPosition);
    }
    mPhysics->update();
}

void SVIDrumPoly::additionalRender(SVISlideTextureContainer * pContainer) {

}

void SVIDrumPoly::prepareBackFace() {
	SVIFloat fWidth = mSize.x;
	SVIFloat fHeight = mSize.y;
	SVIBool isFlipped = SVITRUE;
	SVIFloat deformRatio = 0;
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
	if(mOutpit != NULL)
		deformRatio = mOutpit->mDeformRatio.x;
    SVIVector3 vSize = SVIVector3(mSize.x, mSize.y, mSize.x);
    SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, -mPivot.y * vSize.y, -0.5f * vSize.x * deformRatio);
    mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
        mTextureCoords, isVertical, isReverse, isFlipped);
}

void SVIDrumPoly::setup() {
	allocate(mPhysics->getVerticesCount(), mPhysics->getIndicesCount(),
			mPhysics->getVerticesCount());
}

void SVIDrumPoly::prepareFrontFace() {
	SVIFloat fWidth = mSize.x;
	SVIFloat fHeight = mSize.y;
	SVIBool isFlipped = SVIFALSE;
	SVIFloat deformRatio = 0;
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
					
	if(mOutpit != NULL)
		deformRatio = mOutpit->mDeformRatio.x;
	SVIVector3 vSize = SVIVector3(mSize.x, mSize.y, mSize.x);
	SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, -mPivot.y * vSize.y, -0.5f * vSize.x * deformRatio);
	mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
			mTextureCoords, isVertical, isReverse, isFlipped);
}

}
