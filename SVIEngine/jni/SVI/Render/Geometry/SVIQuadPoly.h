#pragma once
#include "SVIPoly.h"

namespace SVI {

class SVIQuadPoly: public SVIPoly {
public:
	enum {
		SVI_QUAD_LEFT_TOP = 0,
		SVI_QUAD_LEFT_BOTTOM,
		SVI_QUAD_RIGHT_BOTTOM,
		SVI_QUAD_RIGHT_TOP
	};
public:
	SVIQuadPoly(SVIGLSurface * saGLSurface);
	virtual ~SVIQuadPoly();

public:
	void setBorderWidth(SVIFloat width);
	void setBorderColor(SVIFloat r, SVIFloat g, SVIFloat b, SVIFloat a);
	void setBorderColor(SVIVector4 color);

public:
	SVIVector3 * getShadowVertices() {
		return mShadowVertices;
	}

public:

	SVIVector3 * getBorderVertices() {
		return mBorderVertices;
	}
	SVIUInt getBorderVerticeCount() {
		return mBorderVerticeCount;
	}

protected:
	virtual void buildVertices();
	virtual void buildTextureCoordinates();
	//2011-06-13 masterkeaton27@gmail.com
	//border render for rectangle
	//virtual void additionalRender(SVIProgram * pProgram,SVIBool bUseTexture, SVIUInt textureId);
	virtual void additionalRender(SVISlideTextureContainer * pContainer);
	virtual void setup();
private:
	SVIQuadPoly(const SVIQuadPoly& rhs):SVIPoly(rhs.mSVIGLSurface){}
	void operator=(const SVIQuadPoly& rhs){}
	SVIInt generateBorderRectangle(SVIInt startIndex, SVIFloat x, SVIFloat y,SVIFloat w, SVIFloat h);

protected:
	// Border Polygon

	//2011.12.06 jongchae.moon
	//change to virtuals
	virtual void allocateBorderDatas();
	virtual void deallocateBorderDatas();
	virtual void borderSetup();
	//

protected:
	SVIVector3 * mShadowVertices;
	SVIVector3 * mBorderVertices;
	SVIVector2 * mBorderTexcoords;
	SVIUInt mBorderVerticeCount;
	SVIVector4 mBorderColor;
	SVIFloat mBorderWidth;

	//2011.12.06 jongchae.moon
	SVIFloat* mBorderAACoord;
};


/////////////////////////////////////////////////////////
//2011.11.03 jongchae.moon
class SVIQuadPolyExt: public SVIQuadPoly {
public:
	SVIQuadPolyExt(SVIGLSurface *surface);
	virtual ~SVIQuadPolyExt();

protected:
	//2011.11.17 jongchae.moon
	SVIUByte * mBorderIndices;
	SVIUInt mBorderIndicesCount;
	SVIInt mBorderOriVerticeCount;
	SVIInt mBorderOriIndicesCount;
	SVIUByte mBorderIndicesAA_out[4];
	SVIUByte mBorderIndicesAA_in[4];

	virtual void buildVertices();

public:
	virtual void additionalRender(SVISlideTextureContainer * pContainer);

	virtual SVIInt allocate(SVIInt verticeCount);
	virtual SVIInt allocate(SVIInt verticeCount, SVIInt indicesCount);
	virtual void deallocateBorderDatas();

	//2011.11.14 jongchae.moon
	virtual void allocateBorderDatas();
	virtual void borderSetup();
	SVIInt borderSetup_AA_index(SVIInt indexAA, SVIInt index,SVIUByte *BorderIndicesAA);
	virtual SVIInt generateBorderRectangle(SVIInt startIndex, SVIFloat x,SVIFloat y, SVIFloat w, SVIFloat h);
	//
};

};
