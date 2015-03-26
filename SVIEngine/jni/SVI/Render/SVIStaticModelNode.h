#ifndef __SVI_STATICMODELNODE_H_
#define __SVI_STATICMODELNODE_H_

#include "SVIModelNode.h"

namespace SVI {

	class SVIGLSurface;

	// Creation Date	: 2011/11/17 rareboy0112@facebook.com
	// Description		: render rigid mesh node class
	// Last Modified	: 2011/11/22 rareboy0112@facebook.com

	
	class SVIStaticModelNode : public SVIModelNode {

	public:
		SVIStaticModelNode(SVIGLSurface *surface);

		virtual void render(BeginModeType beginType = TRIANGLES);

		virtual void update();

		virtual NodeType getNodeType() { return SVISceneNode::SVITATIC_MODEL_NODE; }
	
		virtual SVISceneNode* deepCopy(SVIBool withSubSlide = SVITRUE);


	};

} // end namespace SVI

#endif // #ifndef __SVI_STATICMODELNODE_H_