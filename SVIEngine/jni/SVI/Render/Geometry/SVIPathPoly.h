#pragma once
#include "SVIPoly.h"

namespace SVI {


//2013-06-20 masterkeaton27@gmail.com

class SVIPathPoly: public SVIPoly {
public:
	SVIPathPoly(SVIGLSurface* surface);
	virtual ~SVIPathPoly();

public:
	void setWidth(SVIFloat width);
    void addPath(float x, float y);
    void clearPath();
    void setOutfit(SVISlideOutfit * pOutpit);

    SVIVector3 * getShadowVertices() {
        return &mShadowVertices[0];
    }

protected:
	virtual void buildVertices();
	virtual void buildTextureCoordinates();
	virtual void additionalRender(SVISlideTextureContainer * pContainer);
	virtual void setup();

private:
	SVIInt generatePath();
	
protected:
    float mWidth;
    
    std::vector<float> mWidthFactor;
    std::vector<SVIVector3> mPath;
    SVISlideOutfit * mOutpit;
    std::vector<SVIVector3> mPathVertices;
    std::vector<SVIVector3> mShadowVertices;
    std::vector<SVIVector2> mPathTexCoord;
    std::vector<SVIUShort> mPathIndices;    
};

};
