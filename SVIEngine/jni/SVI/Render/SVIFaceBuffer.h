#ifndef __SVI_FACEBUFFER_H_
#define __SVI_FACEBUFFER_H_

#include "../SVIDefines.h"
#include "SVIMesh.h"
#include "SVIMaterial.h"
#include "SVISceneNode.h"

namespace SVI {

	struct SVIFaceBuffer {
	public:
		SVIFaceBuffer();

		~SVIFaceBuffer();

		SVIMesh *mMesh;
		SVIMaterial *mMaterial;
		SVIUInt mFaceCount;
		SVIUShort *mIndices;
	private:
		SVIFaceBuffer(const SVIFaceBuffer& rhs){}
		void operator=(const SVIFaceBuffer& rhs){}
	};

} // end namespace SVI

#endif // #ifndef __SVI_FACEBUFFER_H_