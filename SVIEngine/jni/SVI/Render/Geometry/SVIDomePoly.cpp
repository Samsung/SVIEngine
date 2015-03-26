#include "../../SVICores.h"
#include "../../Physics/SVIPhysics.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"

#include "SVIPoly.h"
#include "SVIDomePoly.h"


namespace SVI {

static const SVIBool DEBUG = SVIFALSE;

/****************************************************************************/
// twist polygon object	2011-11-28 masterkeaton27@gmail.com
/****************************************************************************/

SVIDomePoly::SVIDomePoly(SVIGLSurface* saGLSurface) :
	SVIPoly(saGLSurface){

 	
	

    mOutpit = NULL;
    mHasAdditionalUpdate = SVITRUE;
    mHasBackFace = SVIFALSE;
    mNeedTexcoords = SVITRUE;
    mNeedNormals = SVITRUE;
    mNeedIndices = SVITRUE;
    mPhysics = new SVIDomeSimulator();
    setPrimitiveType(SVI_POLY_TRI_LIST);
}

SVIDomePoly::~SVIDomePoly() {
	SVI_SVIFE_DELETE(mPhysics);
}

void SVIDomePoly::setOutfit(SVISlideOutfit * pOutpit) {
	mOutpit = pOutpit;
}

void SVIDomePoly::buildVertices() {

}

void SVIDomePoly::buildTextureCoordinates() {

}

void SVIDomePoly::buildExtraWork() {

}

void SVIDomePoly::buildIndices() {

}

void SVIDomePoly::additionalUpdate() {
    if (mOutpit) {
        mPhysics->setControlTarget(mOutpit->mDeformRatio);
    } else {
        mPhysics->setControlTarget(mSVIGLSurface->getRenderer()->mTouchPosition);
    }
    mPhysics->update();
}

void SVIDomePoly::additionalRender(SVISlideTextureContainer * pContainer) {

}

void SVIDomePoly::prepareBackFace() {
	//no need for this poly
}

void SVIDomePoly::setup() {
	allocate(mPhysics->getVerticesCount(), mPhysics->getIndicesCount(),
			mPhysics->getVerticesCount());
}

void SVIDomePoly::prepareFrontFace() {
	SVIFloat fWidth = mSize.x;
	SVIFloat fHeight = mSize.y;
	SVIBool isFlipped = SVIFALSE;
	SVIBool isVertical = SVIFALSE;
	SVIBool isReverse = SVIFALSE;
	
    SVIVector3 vSize = SVIVector3(mSize.x, mSize.y, mSize.x * 0.7f);
    SVIVector3 vOffset = SVIVector3(-mPivot.x * vSize.x, -mPivot.y * vSize.y,
			0.0f);

	mPhysics->build(vOffset, vSize, mVertices, mIndices, mNormals,
			mTextureCoords, isVertical, isReverse, isFlipped);
}

}
