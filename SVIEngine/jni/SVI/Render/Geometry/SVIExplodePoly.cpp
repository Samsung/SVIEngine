#include "../../SVICores.h"
#include "../../Physics/SVIPhysics.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"

#include "SVIPoly.h"
#include "SVIExplodePoly.h"

namespace SVI {

static const SVIBool DEBUG = SVIFALSE;

/****************************************************************************/
// explode polygon object	2011-11-28 masterkeaton27@gmail.com
/****************************************************************************/

SVIExplodePoly::SVIExplodePoly(SVIGLSurface * saGLSurface) :
	SVIPoly(saGLSurface) {

	mOutpit = NULL;
	mHasAdditionalUpdate = SVITRUE;
	mHasBackFace = SVIFALSE;
	mNeedTexcoords = SVITRUE;
	mNeedNormals = SVITRUE;
	mNeedIndices = SVITRUE;
	mPhysics = new SVIExplodeSimulator();
	//mPhysics->reset();
	setPrimitiveType(SVI_POLY_TRI_LIST);
}

SVIExplodePoly::~SVIExplodePoly() {
	SVI_SVIFE_DELETE(mPhysics);
}

void SVIExplodePoly::setOutfit(SVISlideOutfit * pOutpit) {
	mOutpit = pOutpit;
}

void SVIExplodePoly::buildVertices() {

}

void SVIExplodePoly::buildTextureCoordinates() {

}

void SVIExplodePoly::buildExtraWork() {

}

void SVIExplodePoly::buildIndices() {

}

void SVIExplodePoly::additionalUpdate() {
	if (mOutpit) {
		mPhysics->setControlTarget(mOutpit->mDeformRatio);
	} else {
		mPhysics->setControlTarget(mSVIGLSurface->getRenderer()->mTouchPosition);
	}

	mPhysics->update();
}

void SVIExplodePoly::additionalRender(SVISlideTextureContainer * pContainer) {

}

void SVIExplodePoly::prepareBackFace() {

}

void SVIExplodePoly::setup() {
	allocate(mPhysics->getVerticesCount(), mPhysics->getIndicesCount(),
			mPhysics->getVerticesCount());
}

void SVIExplodePoly::prepareFrontFace() {
	SVIFloat fWidth = mSize.x;
	SVIFloat fHeight = mSize.y;
	SVIBool isFlipped = SVIFALSE;
	SVIVector3 vSize = SVIVector3(mSize.x, mSize.y, mSize.x * 0.7f);
	SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, -mPivot.y * vSize.y,
			0.0f);
	mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
			mTextureCoords, isFlipped);
}

}
