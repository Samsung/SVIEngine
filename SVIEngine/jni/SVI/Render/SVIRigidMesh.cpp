

#include "../SVICores.h"

#include "SVIRigidMesh.h"
#include "SVIModelManager.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

SVIRigidMesh::SVIRigidMesh(SVIGLSurface* saGLSurface) :
mVertexCount(0),
mVertexPositionArray(NULL),
mVertexNormalArray(NULL),
mVertexUvArray(NULL) {
		mSVIGLSurface = saGLSurface;
}

SVIRigidMesh::~SVIRigidMesh() {
	SVI_SVIFE_DELETE_ARRAY(mVertexPositionArray);
	SVI_SVIFE_DELETE_ARRAY(mVertexNormalArray);
	SVI_SVIFE_DELETE_ARRAY(mVertexUvArray);
		
	for(int i = 0; i < (int)mMaterialList.size(); ++i) {
		SVI_SVIFE_DELETE(mMaterialList[i]);
	}
	mMaterialList.clear();

	for(int i = 0; i < (int)mFaceBufferList.size(); ++i) {
		SVI_SVIFE_DELETE(mFaceBufferList[i]);
	}
	mFaceBufferList.clear();
	
	mSVIGLSurface = NULL;
}

void SVIRigidMesh::render(SVISceneNode::BeginModeType beginType) {
	
	mProgramHandler->setAttribute(HD_A_POSITION, mVertexPositionArray);
	mProgramHandler->setAttribute(HD_A_NORMAL, mVertexNormalArray);
	mProgramHandler->setAttribute(HD_A_TEXCOORD, mVertexUvArray);
	glDisable(GL_BLEND);
	for(int i = 0; i < (int)mFaceBufferList.size(); ++i) {
		SVIFaceBuffer *faceBuf = mFaceBufferList[i];

		if(faceBuf!= NULL){
		if(faceBuf->mMaterial->mRenderType == SVIMaterial::TEXTURE) {
			SVIBool genTexture = SVITRUE;
			if(faceBuf->mMaterial->mTexture->mTextureId == 0) {
				SVIModelTexture *texture = faceBuf->mMaterial->mTexture;
				if(texture)
					genTexture = mSVIGLSurface->getModelManager()->genTexture(texture);
			}

			if(genTexture) {
				mProgramHandler->setSampler(HD_U_SVIMPLER_0, faceBuf->mMaterial->mTexture->mTextureId);
			}
			
			glDrawElements(beginType, faceBuf->mFaceCount * 3, GL_UNSIGNED_SHORT, faceBuf->mIndices);
			}else {
			glDrawElements(beginType, faceBuf->mFaceCount * 3, GL_UNSIGNED_SHORT, faceBuf->mIndices);
			}
		}
	}

	mProgramHandler->releaseAttribute(HD_A_POSITION);
	mProgramHandler->releaseAttribute(HD_A_NORMAL);
	mProgramHandler->releaseAttribute(HD_A_TEXCOORD);
}

} // end namespace SVI
