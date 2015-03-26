#pragma once

#include "SVIPoly.h"

namespace SVI {

struct SVISlideOutfit;
class SVISlideTextureContainer;
class SVIDrumSimulator;

class SVIDrumPoly: public SVIPoly {
public:
	SVIDrumPoly(SVIGLSurface* saGLSurface);
	virtual ~SVIDrumPoly();

public:
	SVIDrumSimulator * getPhysics();
	void setOutfit(SVISlideOutfit * pOutpit);

public:
	virtual void additionalUpdate();
	virtual void prepareFrontFace();


protected:
	virtual void buildVertices();
	virtual void buildTextureCoordinates();
	virtual void buildExtraWork();
	virtual void buildIndices();
	virtual void additionalRender(SVISlideTextureContainer * pContainer);
	virtual void prepareBackFace();
	virtual void setup();

private:
	SVIDrumSimulator * mPhysics;
	SVISlideOutfit * mOutpit;
private:
	SVIDrumPoly(const SVIDrumPoly& rhs):SVIPoly(rhs.mSVIGLSurface){}
	void operator=(const SVIDrumPoly& rhs){}
};

};
