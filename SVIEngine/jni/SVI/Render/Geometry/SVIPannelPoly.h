#pragma once
#include "SVIPoly.h"

namespace SVI {

struct SVISlideOutfit;

//2012-06-20 masterkeaton27@gmail.com
//pannel shaped poly
class SVIPannelPoly: public SVIPoly {
public:
	SVIPannelPoly(SVIGLSurface* surface);
	virtual ~SVIPannelPoly();
	
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


private: 
	void generateFront();
	void generateBack();
	void generateOutside();

private:
	SVISlideOutfit * mOutpit;
};

};
