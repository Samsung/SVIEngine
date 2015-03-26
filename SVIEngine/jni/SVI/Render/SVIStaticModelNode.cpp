#include "SVIStaticModelNode.h"
#include "SVIProgramManager.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

SVIStaticModelNode::SVIStaticModelNode(SVIGLSurface *surface) :
	SVIModelNode(surface) {
	}

void SVIStaticModelNode::render(BeginModeType beginType) {

	update();

	SVIProgramParams renderParams;
	SVIProgramHandler *pHandler = NULL;
	SVIProgram *pProgram = NULL;
	renderParams.addElement(EM_3DMODEL);

	pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
	if (pProgram == NULL || !pProgram->getActivated()) return;
	pHandler = pProgram->getHandler();
	pProgram->use();	

	SVIVector4 color = SVIVector4(1.0f, 1.0f, 1.0f, 1.0);

	SVIMatrix viewMat = mCamera->getViewMatrix();
	SVIMatrix projectionMat = mCamera->getProjMatrix();
	
	viewMat = (*mGlobalTransform) * viewMat;
	
	pHandler->setUniform(HD_U_PROJ, projectionMat);
	pHandler->setUniform(HD_U_VIEW, viewMat);
	pHandler->setUniform(HD_U_COLOR, color);

	SVIVector3 lightPosition = SVIVector3(0.25f,0.25f,10.0f);		
	SVIVector4 lightAmbient = SVIVector4(0.6f,0.6f,0.6f,1.0f);
	SVIVector4 lightDiffuse = SVIVector4(0.68f,0.66f,0.63f,1.0f);
	SVIVector4 lightSpecular = SVIVector4(0.3f,0.28f,0.275f,1.0f);


	SVIMatrix normalMatrix = *mGlobalTransform;
	normalMatrix.invert();
	normalMatrix.transpose();
	SVIFloat	lightShininess = 50.0f;
	pHandler->setUniform(HD_U_NORMAL,normalMatrix);
	pHandler->setUniform(HD_U_LIGHT_OFFSET,lightPosition);
	pHandler->setUniform(HD_U_AMBIENT,lightAmbient);
	pHandler->setUniform(HD_U_DIFFUSE,lightDiffuse);
	pHandler->setUniform(HD_U_SPECULAR,lightSpecular);
	pHandler->setUniform(HD_U_SHININESS,lightShininess);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	if(!mMeshList.empty())
	for(int i = 0; i < (int)mMeshList.size(); ++i) {
		mMeshList[i]->mProgramHandler = pHandler;
		mMeshList[i]->render(beginType);
	}

	if(!mChildList.empty())
	for(int i = 0; i < (int)mChildList.size(); ++i) {
		mChildList[i]->setCamera(mCamera);
		mChildList[i]->render(beginType);
	}
}


void SVIStaticModelNode::update() {
	if(mParent != NULL) {
		// globalTM = localTM * parentTM
		*mGlobalTransform = *mLocalTransform * *(mParent->getGlobalTransform());
	}
	else {
		*mGlobalTransform = *mLocalTransform;
	}
}


SVISceneNode* SVIStaticModelNode::deepCopy(SVIBool withSubSlide) {
	SVIStaticModelNode *copyNode = new SVIStaticModelNode(mSVIGLSurface);

	copyNode->mParent = mParent;
	//SceneNodeList mChildList;
	copyNode->mId = mId;
	*(copyNode->mLocalTransform) = *mLocalTransform;
	*(copyNode->mGlobalTransform) = *mGlobalTransform;
	// increase refer count
	copyNode->mMeshList = mMeshList;

	if(withSubSlide) {
		copyChildNodeList(copyNode);
	}
	
	return copyNode;
}


} // end namespace SVI