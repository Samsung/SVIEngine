#include "../../SVICores.h"

#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"
#include "../../Slide/SVIRenderPartManager.h"

#include "../SVITexture.h"
#include "../SVITextureManager.h"
#include "../SVIProgramManager.h"
#include "../SVIDebugRenderer.h"
#include "../SVIViewport.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"


#include "SVIPoly.h"
#include "SVIPathPoly.h"

namespace SVI {
	
const float cfTestDepth = 1.0f;
static const SVIBool DEBUG = SVIFALSE;


#define MAX_PATH_POINT 300 

/****************************************************************************/
// real time path polygon
/****************************************************************************/

SVIPathPoly::SVIPathPoly(SVIGLSurface* surface):SVIPoly(surface){
	mHasAdditionalRender = SVIFALSE;
    mNeedTexcoords = SVITRUE;
    mNeedIndices = SVITRUE;
    mNeedNormals = SVIFALSE;
    mIsAA = SVIFALSE;
    mAACoord = NULL;
    mWidth = 200.0f;


    for (int n = 0; n < 1000; n++){
        mWidthFactor.push_back( (float)(rand()%100 - 50) * 0.01f);
    }    
}

void SVIPathPoly::setup() {
    //vertices for triangle strip
    setPrimitiveType(SVI_POLY_TRI_STRIP);	
}

SVIPathPoly::~SVIPathPoly() {
}

void SVIPathPoly::buildVertices() {
	
	SVIFloat widthHalf = mWidth * 0.5f;
        
    size_t pathCount = mPath.size();
    
    float distancePath = 0.0f;

    if (pathCount <= 1) {
        return;
    }
    

    std::vector<SVIVector3> swapVertices;
    swapVertices.reserve(0);
    mPathVertices.swap(swapVertices);
    mPathVertices.clear();

    std::vector<SVIVector2> swapTexCoord;
    swapTexCoord.reserve(0);
    mPathTexCoord.swap(swapTexCoord);
    mPathTexCoord.clear();

    std::vector<SVIUShort> swapIndices;
    swapIndices.reserve(0);
    mPathIndices.swap(swapIndices);
    mPathIndices.clear();

    std::vector<SVIVector3> swapShadowVertices;
    swapShadowVertices.reserve(0);
    mShadowVertices.swap(swapShadowVertices);
    mShadowVertices.clear();
    

	SVISize size = mSVIGLSurface->getRenderPartManager()->getSize();


    SVIUShort index = 0;
    int width_index = 0;
    for (size_t n = 0; n < pathCount-1; n++){

        SVIVector3 direction = mPath[n+1] - mPath[n];
        SVIVector3 up = SVIVector3(0,0,1.0f);

        SVIVector3 side = up.Cross(direction);
        side.normalize();
        
        widthHalf = mPath[n].z * 0.5f; 


        
        SVIVector3 vertexA = mPath[n] - side * widthHalf;
        SVIVector3 vertexB = mPath[n] + side * widthHalf;

        mPathVertices.push_back(vertexA);
        mPathVertices.push_back(vertexB);
        
        float shadowWidth = mWidthFactor[width_index++];
        width_index = width_index % 1000;

        vertexA = mPath[n] - side * widthHalf * 1.1f;
        vertexB = mPath[n] + side * widthHalf * 1.1f;

        vertexA -= side * shadowWidth * mPath[n].z * 0.08f;
        vertexB += side * shadowWidth * mPath[n].z * 0.08f;

        mShadowVertices.push_back(vertexA);
        mShadowVertices.push_back(vertexB);

        mPathIndices.push_back(index);
        mPathIndices.push_back(index+1);

        index += 2;
        //SVIDebugRenderer::drawPoint(vertexA, SVIVector4(1,1,1,1));
        //SVIDebugRenderer::drawPoint(vertexB, SVIVector4(1,1,1,1));
    }
    
    mVerticeCount = mPathVertices.size();
    mIndicesCount = mPathIndices.size();

    for (int n = 0; n < mPathVertices.size(); n++) {
        float u = (mPathVertices[n].x / (float)mOutpit->mSize.x);
        float v = (mPathVertices[n].y / (float)mOutpit->mSize.y);	
        mPathTexCoord.push_back(SVIVector2(u, v));        
    }

    mVertices = &mPathVertices[0];
    mIndices = &mPathIndices[0];
    mTextureCoords = &mPathTexCoord[0];
    
    
	//2011-05-26 ignore z centering
	//mVertices[SVI_QUAD_LEFT_TOP].x = fInverseXValue * fXDistance - width;
	//mVertices[SVI_QUAD_LEFT_TOP].y = fInverseYValue * fYDistance - width;
}


void SVIPathPoly::addPath(float x, float y){
    mPath.push_back(SVIVector3(x,y,0.0f));

    mNeedToUpdate = SVITRUE;
}

void SVIPathPoly::clearPath(){
    std::vector<SVIVector3> swapVertices;
    swapVertices.reserve(0);
    mPath.swap(swapVertices);
    mPath.clear();

    mNeedToUpdate = SVITRUE;
}

void SVIPathPoly::setWidth(SVIFloat width) {

    mWidth = width;	

    mNeedToUpdate = SVITRUE;
}

void SVIPathPoly::setOutfit(SVISlideOutfit * pOutpit){

    mOutpit = pOutpit;

    clearPath();
    if (!mOutpit->mPathPoints.empty()){
        mPath.assign(mOutpit->mPathPoints.begin(),mOutpit->mPathPoints.end());                            
    }   

    mNeedToUpdate = SVITRUE;
}

void SVIPathPoly::additionalRender(SVISlideTextureContainer * pContainer) {
	
}

SVIInt SVIPathPoly::generatePath() {
	return 0;
}

void SVIPathPoly::buildTextureCoordinates() {

}


}
