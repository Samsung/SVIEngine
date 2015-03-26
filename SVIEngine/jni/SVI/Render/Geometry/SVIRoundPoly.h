#pragma once
#include "SVIPoly.h"

namespace SVI {

//2011-06-06 masterkeaton27@gmail.com
//round box poly
class SVIRoundPoly: public SVIPoly {
public:
	enum {
		SVI_ROUND_LEFT_TOP = 0,
		SVI_ROUND_LEFT_BOTTOM,
		SVI_ROUND_RIGHT_BOTTOM,
		SVI_ROUND_RIGHT_TOP
	};

public:
	SVIRoundPoly(SVIGLSurface* saGLSurface);
	virtual ~SVIRoundPoly();

public:
	void setRoundRadius(SVIFloat radius);
	void setRoundLevel(SVIInt level);
	void setBorderWidth(SVIFloat width);
	void setBorderColor(SVIFloat r, SVIFloat g, SVIFloat b, SVIFloat a);
	void setBorderColor(SVIVector4 color);

public:
	SVIUInt getBorderVerticeCount(); 
	SVIUInt getBorderIndicesCount();
	SVIVector3 * getBorderVertices();
	SVIUShort * getBorderIndices();

public:
	SVIVector3 * getShadowVertices();
	void generateInnerShadowRectangle(SVIInt startIndex, SVIFloat x, SVIFloat y,
			SVIFloat w, SVIFloat h);

private:
	SVIRoundPoly(const SVIRoundPoly& rhs):SVIPoly(rhs.mSVIGLSurface){}
	void operator=(const SVIRoundPoly& rhs){}
	SVIInt generateCornerVertices(SVIInt index);
	SVIInt generateInnerVertices(SVIInt startIndex);
	SVIInt generateInnerRectangle(SVIInt startIndex, SVIInt * indicesBufferIndex,
			SVIFloat x, SVIFloat y, SVIFloat w, SVIFloat h);
	virtual void generateScaledVertices();

	//2011.12.07 jongchae.moon
protected:
	SVIInt generateBorderCornerVertices(SVIInt index, SVIFloat forAA = 0.0f);
	SVIInt generateBorderRectangle(SVIInt startIndex, SVIInt * indicesBufferIndex,
			SVIFloat x, SVIFloat y, SVIFloat w, SVIFloat h);

protected:
	virtual void buildVertices();
	virtual void buildTextureCoordinates();
	virtual void buildExtraWork();
	//2011-06-28 masterkeaton27@gmail.com
	virtual void buildIndices();
	//virtual void additionalRender(SVIProgram * pProgram, SVIBool bUseTexture, SVIUInt textureId);
	virtual void additionalRender(SVISlideTextureContainer * pContainer);
	virtual void setup();

protected:
	// Border Polygon
	void deallocateBorderDatas();

	//2011.12.07 jongchae.moon
	virtual void allocateBorderDatas();
	virtual void borderSetup();
	//

protected:
	// Border Polygon

	SVIVector3 * mShadowVertices;
	SVIUShort * mBorderIndices;
	SVIVector3 * mBorderVertices;
	SVIVector2 * mBorderTexcoords;
	SVIUInt mBorderVerticeCount;
	SVIUInt mBorderIndicesCount;

	//2011.12.07 jongchae.moon
	SVIFloat* mBorderAACoord;

protected:
	SVIUInt mRoundLevel;
	SVIVector4 mBorderColor;
	SVIFloat mBorderWidth;
	SVIFloat mRoundRadius[4];
	SVIVector3 mRoundCenter[4];

};

class SVIRoundPolyExt: public SVIRoundPoly {
public:
	SVIRoundPolyExt(SVIGLSurface *surface);
	virtual ~SVIRoundPolyExt();

protected:
	SVIUByte mCornerRadiusStartIndices[4][2];
	SVIUByte mCornerRadiusEndIndices[4][2];

	//2011.11.16 jongchae.moon
	SVIUByte mBorderCornerRadiusStartIndices[4][2];
	SVIUByte mBorderCornerRadiusEndIndices[4][2];

	SVIUByte mBorderInsideCornerRadiusStartIndices[4][2];
	SVIUByte mBorderInsideCornerRadiusEndIndices[4][2];

	SVIInt mBorderOriVerticeCount;
	SVIInt mBorderOriIndicesCount;
	//

	virtual void buildVertices();

public:

	// jbkim 20111108
	void    cornerAAPatch(SVIInt index);
	void    rectAAPatch();

	virtual SVIInt allocate(SVIInt verticeCount);
	virtual SVIInt allocate(SVIInt verticeCount, SVIInt indicesCount);

	//2011.11.14 jongchae.moon
	virtual void allocateBorderDatas();
	void borderSetup_InnerRect(SVIInt startIndex, SVIFloat forAA);
	virtual void borderSetup();
	SVIInt borderCornerAAPatch(SVIInt index, SVIFloat forAA);
	void borderRectAAPatch(SVIBool bInside);
	//

	virtual void generateScaledVertices();
};

};
