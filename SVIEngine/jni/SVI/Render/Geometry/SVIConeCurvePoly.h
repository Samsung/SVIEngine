#pragma once

#include "SVIPoly.h"

namespace SVI {

struct SVISlideOutfit;
class SVISlideTextureContainer;
class SVIConeCurveSimulator;

class SVIConeCurvePoly: public SVIPoly {
public:
	SVIConeCurvePoly(SVIGLSurface *surface);
	virtual ~SVIConeCurvePoly();

private:
	SVIConeCurvePoly(const SVIConeCurvePoly& rhs):SVIPoly(rhs.mSVIGLSurface){}
	void operator=(const SVIConeCurvePoly& rhs){}
public:
	SVIConeCurveSimulator * getPhysics();
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
	SVIConeCurveSimulator * mPhysics;
	SVISlideOutfit * mOutpit;
};

};
