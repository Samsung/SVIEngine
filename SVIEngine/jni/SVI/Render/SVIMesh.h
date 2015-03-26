#ifndef __SVI_MESH_H_
#define __SVI_MESH_H_

#include <vector>
#include <string>
#include "../BaseType/SVIBaseTypes.h"
#include "../SVIDefines.h"
#include "../Mathematics/SVIMath.h"
#include "SVISceneNode.h"
#include "SVIProgramManager.h"

namespace SVI {

	// Creation Date	: 2011/11/16 rareboy0112@facebook.com
	// Description		: render mesh interface(SVIRigidMesh, SVISkinnedMesh)
	// Last Modified	: 2011/11/16 rareboy0112@facebook.com
	class SVIMesh {
	public:

		enum MeshType {	
			NONE_MESH = 0,
			RIGID_MESH = 1,
			SKIND_MESH = 2,
			MESH_COUNT
		};

		SVIMesh() {
			mProgramHandler = NULL;
		}

		virtual ~SVIMesh() {}

		virtual MeshType getMeshType() { return NONE_MESH; }

		virtual void render(SVISceneNode::BeginModeType beginType = SVISceneNode::TRIANGLES) = 0 ;

	public:
		std::string mId;
		SVIProgramHandler *mProgramHandler;

	};

} // end namespace SVI

#endif // #ifndef __SVI_MESH_H_