#include "../../SVICores.h"
#include "../../Physics/SVIPhysics.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"

#include "SVIPoly.h"
#include "SVIConeCurvePoly.h"



namespace SVI {

static const SVIBool DEBUG = SVIFALSE;

/****************************************************************************/
// twist polygon object	2011-11-28 masterkeaton27@gmail.com
/****************************************************************************/

SVIConeCurvePoly::SVIConeCurvePoly(SVIGLSurface *surface) :
SVIPoly(surface){
	mOutpit = NULL;
	mHasAdditionalUpdate = SVITRUE;
	mHasBackFace = SVITRUE;
	mNeedTexcoords = SVITRUE;
	mNeedNormals = SVITRUE;
	mNeedIndices = SVITRUE;

	mPhysics = new SVIConeCurveSimulator();
	setPrimitiveType(SVI_POLY_TRI_LIST);
}

SVIConeCurvePoly::~SVIConeCurvePoly() {
	SVI_SVIFE_DELETE(mPhysics);
}

void SVIConeCurvePoly::setOutfit(SVISlideOutfit * pOutpit) {
	mOutpit = pOutpit;
}

void SVIConeCurvePoly::buildVertices() {

}

void SVIConeCurvePoly::buildTextureCoordinates() {

}

void SVIConeCurvePoly::buildExtraWork() {

}

void SVIConeCurvePoly::buildIndices() {

}

void SVIConeCurvePoly::additionalUpdate() {

	if (mOutpit) {
		mPhysics->setControlTarget(mOutpit->mDeformRatio);
	} else {
		mPhysics->setControlTarget(mSVIGLSurface->getRenderer()->mTouchPosition);
	}
	if(mOutpit){
	mPhysics->setAxis(mOutpit->mDeformRatio.x);
	mPhysics->setAngle( mOutpit->mDeformRatio.y);
	mPhysics->setxOffset(mOutpit->mDeformRatio.z);
	}

	mPhysics->update();
}

void SVIConeCurvePoly::additionalRender(SVISlideTextureContainer * pContainer) {

}

void SVIConeCurvePoly::prepareBackFace() {
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

	SVIVector3 vSize = SVIVector3(mSize.x, mSize.y, mSize.x );
	SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, mPivot.y * vSize.y,
			0.0f);
	mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
			mTextureCoords, isVertical, isReverse, isFlipped);
}

void SVIConeCurvePoly::setup() {
	allocate(mPhysics->getVerticesCount(), mPhysics->getIndicesCount(),
			mPhysics->getVerticesCount());
}

void SVIConeCurvePoly::prepareFrontFace() {
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
	SVIVector3  vSize = SVIVector3(mSize.x, mSize.y, mSize.x);
	SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, mPivot.y * vSize.y,
			0.0f);
	mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
			mTextureCoords, isVertical, isReverse, isFlipped);
}

}
