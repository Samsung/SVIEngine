#include "SVIMaterial.h"
#include "SVIModelManager.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

SVIMaterial::SVIMaterial(SVIGLSurface* saGLSurface) :
mRenderType(SOLID_COLOR),
mColor(1.0f, 1.0f, 1.0f, 1.0f),
mTexture(NULL) {
	mSVIGLSurface = saGLSurface;
}

SVIMaterial::~SVIMaterial() {
	if(mTexture){
		mSVIGLSurface->getModelManager()->removeTexture(mTexture->mFileName);
		mTexture = NULL;
	}
	mSVIGLSurface = NULL;
}

const SVIMaterial& SVIMaterial::operator = (const SVIMaterial &rMaterial) {
	mColor = rMaterial.mColor;
	mRenderType = rMaterial.mRenderType;
	mTexture = rMaterial.mTexture;

	return *this;
}

const SVIMaterial& SVIMaterial::operator = (const SVIMaterial *rMaterial) {
	mColor = rMaterial->mColor;
	mRenderType = rMaterial->mRenderType;
	mTexture = rMaterial->mTexture;

	return *this;
}


} // end namespace SVI
