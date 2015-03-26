#ifndef __SVI_MODELNODE_H_
#define __SVI_MODELNODE_H_

#include <vector>
#include "../BaseType/SVIExtendTypes.h"
#include "SVISceneNode.h"
#include "SVIMesh.h"

namespace SVI {

	class SVIModelNode;
	class SVIMesh;
	class SVISceneNode;

	typedef SVISharedPtr<SVIMesh> SharedMesh;
	typedef std::vector< SVISharedPtr<SVIMesh> > SharedMeshList;
	typedef SVISharedPtr<SVIModelNode> SharedModelNode;

	// Creation Date	: 2011/11/16 rareboy0112@facebook.com
	// Description		: render model node class
	// Last Modified	: 2011/11/28 rareboy0112@facebook.com
	class SVIModelNode : public SVISceneNode {
	public:

		SVIModelNode(SVIGLSurface *surface);

		virtual ~SVIModelNode();

		virtual NodeType getNodeType() { return SVISceneNode::ROOT_MODEL_NODE; }

		virtual SVISceneNode* deepCopy(SVIBool withSubSlide = SVITRUE);

		SVIBool addMesh(SharedMesh mesh);

		SVIBool removeMesh(SharedMesh mesh);

	public:
		SharedModelNode mSharedModel;


	protected:
		SharedMeshList mMeshList;
	};

} // end namespace SVI


#endif // #ifndef __SVI_MODELNODE_H_