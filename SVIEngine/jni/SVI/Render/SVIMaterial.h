#ifndef __SVI_MATERIAL_H_
#define __SVI_MATERIAL_H_

#include "../BaseType/SVIExtendTypes.h"
#include "SVIModelTexture.h"

namespace SVI {

	class SVIGLSurface;

	// Creation Date	: 2011/11/15 rareboy0112@facebook.com
	// Description		: 3d mesh of the material
	// Last Modified	: 2011/11/23 rareboy0112@facebook.com
	class SVIMaterial {
	public:
		enum RenderType {	
			SOLID_COLOR = 0,
			TEXTURE = 1,
			MATERIAL_COUNT
		};

		SVIMaterial(SVIGLSurface *surface);

		~SVIMaterial();

		const SVIMaterial& operator = (const SVIMaterial &rMaterial);

		const SVIMaterial& operator = (const SVIMaterial *rMaterial);

	public:
		// diffuse color(only when the SOLID_COLOR mode is used.)
		SVIColor mColor;
		RenderType mRenderType;
		SharedModelTexture mTexture;
		
	protected:
		SVIGLSurface *mSVIGLSurface;

	};

} // end namespace SVI

#endif //__SVI_MATERIAL_H_
