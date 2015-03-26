
#include "SVISceneNode.h"

namespace SVI {

SVISceneNode::SVISceneNode(SVIGLSurface* saGLSurface) :
mParent(NULL),
mId("") {
	mLocalTransform = new SVIMatrix();
	mLocalTransform->setIdentity();

	mGlobalTransform = new SVIMatrix();
	mGlobalTransform->setIdentity();
	
	mLocalTransformChanged = SVIFALSE;

	mPosition = SVIVector3(0.0f, 0.0f, 0.0f);
	// set quaternion identity
	mRotation = SVIVector4(0.0f, 0.0f, 0.0f, 1.0f);

	mScale = SVIVector3(1.0f, 1.0f, 1.0f);
	mCamera = NULL;

	mSVIGLSurface = saGLSurface;
}

SVISceneNode::~SVISceneNode() {
	SVI_SVIFE_DELETE(mLocalTransform);
	SVI_SVIFE_DELETE(mGlobalTransform);
	for(int i = 0; i < (int)mChildList.size(); ++i) {
		SVI_SVIFE_DELETE(mChildList[i]);
	}
	mChildList.clear();

	mSVIGLSurface = NULL;
}
SVISceneNode::SVISceneNode(const SVISceneNode& rhs){}
SVISceneNode& SVISceneNode::operator=(const SVISceneNode& rhs){
	return *this;
}

SVISceneNode::NodeType SVISceneNode::getNodeType() {
	return TRANSFORM_NODE;
}


void SVISceneNode::setCamera(SVI3DCamera *camera) {
	mCamera = camera;
}


void SVISceneNode::render(BeginModeType beginType) {
	update();

	for(int i = 0; i < (int)mChildList.size(); ++i) {
		mChildList[i]->setCamera(mCamera);
		mChildList[i]->render(beginType);
	}
}


void SVISceneNode::update() {
	if( mLocalTransformChanged == SVITRUE ) {
		/// update local transfrom matrix (SRT)
		SVIMatrix scaleMat;
		scaleMat.setIdentity();
		mLocalTransform->setIdentity();
		scaleMat.setScaling(mScale);
		mLocalTransform->setSVIQuaternion(mRotation);
		*mLocalTransform = scaleMat * (*mLocalTransform);
		
		mLocalTransform->setPosition(mPosition);
		mLocalTransformChanged = SVIFALSE;
	}
	
	if(mParent != NULL) {
		// globalTM = localTM * parentTM
		*mGlobalTransform = *mLocalTransform * *(mParent->getGlobalTransform());
	}
	else {
		*mGlobalTransform = *mLocalTransform;
	}
}


SVISceneNode* SVISceneNode::deepCopy(SVIBool withSubSlide) {
	SVISceneNode *copyNode = new SVISceneNode(mSVIGLSurface);

	copyNode->mParent = mParent;
	//SceneNodeList mChildList;
	copyNode->mId = mId;
	*(copyNode->mLocalTransform) = *mLocalTransform;
	*(copyNode->mGlobalTransform) = *mGlobalTransform;
	
	/*if(withSubSlide) {
		copyChildNodeList(copyNode);
	}*/
	
	return copyNode;
}


void SVISceneNode::copyChildNodeList(SVISceneNode *parent) {
	// copy child scene-node
	SceneNodeList newNodeList;
	SceneNodeList::iterator iter = mChildList.begin();
	for(iter; iter != mChildList.end(); ++iter) {
		SVISceneNode *newNode = NULL;
		
		newNode = (*iter)->deepCopy(SVITRUE);
		parent->addChild(newNode);
	}
}


SVISceneNode* SVISceneNode::getParent() {
	return mParent;
}


void SVISceneNode::setParent(SVISceneNode *parent) {
	mParent = parent;
}


SVIBool SVISceneNode::addChild(SVISceneNode *node) {
	SceneNodeList::iterator iter = mChildList.begin();
	SVISceneNode *temp = NULL;
	for(iter; iter != mChildList.end(); ++iter) {
		temp = *iter;
		if(node == temp) {
			return SVIFALSE;
		}
	}

	node->setParent(this);
	mChildList.push_back(node);

	return SVITRUE;
}


SVIBool SVISceneNode::removeChild(SVISceneNode *node) {
	SceneNodeList::iterator iter = mChildList.begin();
	SVISceneNode *temp = NULL;
	for(iter; iter != mChildList.end(); ++iter) {
		temp = *iter;
		if(node == temp) {
			node->setParent(NULL);
			mChildList.erase(iter);
			return SVITRUE;
		}
	}

	return SVIFALSE;
}


void SVISceneNode::setLocalTransform(SVIMatrix *localTM) {
	if(localTM == NULL)
		return;
	
	SVI_SVIFE_DELETE(mLocalTransform);
	mLocalTransform = localTM;
	mLocalTransformChanged = SVITRUE;
}


void SVISceneNode::setLocalTransform(SVIMatrix &localTM) {
	if(mLocalTransform == NULL) {
		mLocalTransform = new SVIMatrix();
	}

	*mLocalTransform = localTM;
	mLocalTransformChanged = SVITRUE;
}


void SVISceneNode::setId(std::string id) {
	mId = id;
}


const std::string SVISceneNode::getId() {
	return mId;
}


void SVISceneNode::setPosition(SVIVector3 &position) {
	mPosition = position;
	mLocalTransformChanged = SVITRUE;
}


SVIVector3 SVISceneNode::getPosition() {
	return mPosition;
}


void SVISceneNode::setRotation(SVIVector4 &rotation) {
	mRotation = rotation;
	mLocalTransformChanged = SVITRUE;
}


SVIVector4 SVISceneNode::getRotation() {
	return mRotation;
}


void SVISceneNode::setScale(SVIVector3 &scale) {
	mScale = scale;
	mLocalTransformChanged = SVITRUE;
}


SVIVector3 SVISceneNode::getScale() {
	return mScale;
}

} // end namespace SVI
