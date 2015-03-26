#include "SVIFaceBuffer.h"
#include "../SVICores.h"

namespace SVI {

SVIFaceBuffer::SVIFaceBuffer() :
mMesh(NULL),
mMaterial(NULL),
mFaceCount(0),
mIndices(0) {

}


SVIFaceBuffer::~SVIFaceBuffer() {
	SVI_SVIFE_DELETE_ARRAY(mIndices);
}

} // end namespace SVI