#ifndef __SVI_MODELTEXTURE_H_
#define __SVI_MODELTEXTURE_H_

#include <string>
#include "../BaseType/SVIExtendTypes.h"

namespace SVI {

	// Creation Date	: 2011/11/23 rareboy0112@facebook.com
	// Description		: used by material.
	// Last Modified	: 2011/12/01 rareboy0112@facebook.com
	class SVIModelTexture {
	public:
		SVIModelTexture(void) :
		mFileName(""),
		mTextureId(0),
		mBits(NULL) {

		}

		~SVIModelTexture() {
			SVI_SVIFE_DELETE_ARRAY(mBits);
		}

	public:
		// 0 Do not use the texture.
		SVIUInt mTextureId;
		std::string mFileName;
		SVIInt mWidth;
		SVIInt mHeight;
		SVIUByte *mBits;
	};

	typedef SVISharedPtr<SVIModelTexture> SharedModelTexture;

} // end namespace SVI

#endif //__SVI_MATERIAL_H_
