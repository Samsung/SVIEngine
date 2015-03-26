#ifndef __SVI_RIGIDMESH_H_
#define __SVI_RIGIDMESH_H_

#include "../BaseType/SVIBaseTypes.h"
#include "../SVIDefines.h"
#include "../Mathematics/SVIMath.h"
#include "SVIMesh.h"
#include "SVIMaterial.h"
#include "SVIFaceBuffer.h"
#include <vector>

namespace SVI {

	struct SVIFaceBuffer;
	class SVIGLSurface;
	class SVISceneNode;
	

	typedef std::vector<SVIMaterial *> MaterialList;
	typedef std::vector<SVIFaceBuffer *> FaceBufferList;

	// Creation Date	: 2011/11/07 rareboy0112@facebook.com
	// Description		: render rigid Mesh class(through model-node is called.)
	// Last Modified	: 2011/11/16 rareboy0112@facebook.com
	class SVIRigidMesh : public SVIMesh {
	public:
		SVIRigidMesh(SVIGLSurface* saGLSurface);

		virtual ~SVIRigidMesh();

		virtual void render(SVISceneNode::BeginModeType beginType = SVISceneNode::TRIANGLES);
	private:
		SVIRigidMesh(const SVIRigidMesh& rhs){}
	       void	operator=(const SVIRigidMesh& rhs){}
	
	public:
		SVIInt mVertexCount;
		
		SVIVector3 *mVertexPositionArray;
		SVIVector3 *mVertexNormalArray;
		SVIVector2 *mVertexUvArray;

		MaterialList mMaterialList;
		FaceBufferList mFaceBufferList;
		
	protected:
		SVIGLSurface* mSVIGLSurface;
	};

} // end namespace SVI

#endif // #ifndef __SVI_RIGIDMESH_H_