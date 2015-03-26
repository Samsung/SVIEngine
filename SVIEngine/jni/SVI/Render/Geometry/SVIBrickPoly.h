#pragma once
#include "SVIPoly.h"

namespace SVI {

struct SVISlideOutfit;

//2012-06-20 masterkeaton27@gmail.com
//brick shaped poly
class SVIBrickPoly: public SVIPoly {
public:
	SVIBrickPoly(SVIGLSurface* surface);
	virtual ~SVIBrickPoly();

public:
	virtual void prepareFrontFace();
	void setOutfit(SVISlideOutfit * pOutpit);

protected:
	virtual void buildVertices();
	virtual void buildTextureCoordinates();
	virtual void buildExtraWork();
	virtual void buildIndices();
	virtual void additionalRender(SVISlideTextureContainer * pContainer);
	virtual void prepareBackFace();
	virtual void setup();
	
protected:
	void generateFront();
	void generateBack();
	void generateOutside();

private:
	SVISlideOutfit * mOutpit;
};


};
