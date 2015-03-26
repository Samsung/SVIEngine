#pragma once

#include "SVIPoly.h"

namespace SVI {

struct SVISlideOutfit;
class SVISlideTextureContainer;
class SVIRenderPatch;
class SVIExplodeSimulator;

class SVIExplodePoly: public SVIPoly {
public:
	SVIExplodePoly(SVIGLSurface * saGLSurface);
	virtual ~SVIExplodePoly();

public:
	SVIExplodeSimulator * getPhysics();
	void setOutfit(SVISlideOutfit * pOutpit);

//2012-03-16 masterkeaton27@gmail.com
//following 2 functions are very important when initialize sequence.
//for pre-calculation and fill buffers.
//should check twice when merging
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
	SVIExplodeSimulator * mPhysics;
	SVISlideOutfit * mOutpit;
private:
	SVIExplodePoly(const SVIExplodePoly& rhs):SVIPoly(rhs.mSVIGLSurface){}
	void operator=(const SVIExplodePoly& rhs){}
};

};
