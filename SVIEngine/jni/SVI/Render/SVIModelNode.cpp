#include "SVIModelNode.h"
#include "SVIModelManager.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

SVIModelNode::SVIModelNode(SVIGLSurface *surface) :
	SVISceneNode(surface){
}
	

SVIModelNode::~SVIModelNode() {
	mMeshList.clear();
	if(mSharedModel.getPtr() != NULL) {
		std::string oldId = mSharedModel->getId();
		mSharedModel = NULL;
		mSVIGLSurface->getModelManager()->removeModel(oldId);
	}

}


SVISceneNode* SVIModelNode::deepCopy(SVIBool withSubSlide) {
	SVIModelNode *copyNode = new SVIModelNode(mSVIGLSurface);

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


SVIBool SVIModelNode::addMesh(SharedMesh mesh) {
	SharedMeshList::iterator iter = mMeshList.begin();
	SharedMesh temp = NULL;
	for(iter; iter != mMeshList.end(); ++iter) {
		temp = *iter;
		if(mesh == temp) {
			return SVIFALSE;
		}
	}

	mMeshList.push_back(mesh);

	return SVITRUE;
}


SVIBool SVIModelNode::removeMesh(SharedMesh mesh) {
	SharedMeshList::iterator iter = mMeshList.begin();
	SharedMesh temp = NULL;
	for(iter; iter != mMeshList.end(); ++iter) {
		temp = *iter;
		if(mesh == temp) {
			mMeshList.erase(iter);
			return SVITRUE;
		}
	}

	return SVIFALSE;
}


} // end namespace SVI