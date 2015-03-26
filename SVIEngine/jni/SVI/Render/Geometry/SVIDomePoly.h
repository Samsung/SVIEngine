#pragma once

#include "SVIPoly.h"

namespace SVI {

struct SVISlideOutfit;
class SVISlideTextureContainer;
class SVIDomeSimulator;

class SVIDomePoly: public SVIPoly {
public:
	SVIDomePoly(SVIGLSurface *surface);
	virtual ~SVIDomePoly();

public:
	SVIDomeSimulator * getPhysics();
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
	SVIDomeSimulator * mPhysics;
	SVISlideOutfit * mOutpit;
};

};
