#include "../../SVICores.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIProjectionSlide.h"

#include "../SVIRenderer.h"
#include "../SVIRenderPatch.h"

#include "SVITesselationPoly.h"

namespace SVI 
{
	
static const SVIBool DEBUG = SVIFALSE;

//if only want see morphing mesh? change value to SVITRUE
const SVIBool ONLY_MORPHING = SVIFALSE;
const SVIInt DEFAULT_VERTEX_COUNT = 32;

/****************************************************************************/
// Default tesselation polygon object
/****************************************************************************/
//--------------------------------------------------------------------------------------------------
SVITesselationPoly::SVITesselationPoly(SVIGLSurface *surface)
:SVIPoly(surface)
{
	init();
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::setOutfit(SVISlideOutfit * pOutpit) 
{
	mOutpit = pOutpit;
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::init() 
{
    //tesselation initialize
	mOutpit = NULL;
    mNeedIndices = SVITRUE;
    mNeedTexcoords = SVITRUE;
	setPrimitiveType(SVI_POLY_TRI_LIST);
	mIsBuild = SVIFALSE;

    //morphing initialize
    mIsMorphingBuild = SVIFALSE;
    mInitializedMorphingInfo = SVIFALSE;
    mIsConvex = SVIFALSE;
    mClockwise = SVITRUE;
    mMorphingRadius = 0.0f;
    
    mMorphingCenterIndex = 0;
    mMorphingStartIndex = 0;
    mMorphingFarIndex = 0;

    mMorphingVerticesCount = 0;
    mMorphingIndicesCount = 0;

    mMorphingVertices = NULL;
    mMorphingIndices = NULL;

    mConvexCompletedTime = 0.0f;
    //TODO : set temp value.
    mMorphingRatio = 0.7f;

    mMorphingVertexCount = DEFAULT_VERTEX_COUNT;
    mMorphingQuadrantCount = mMorphingVertexCount / 4;

    mDegreeMorphingRotateZ = 0.0f;
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::build() 
{
	if (mIsBuild) {
		return SVIFALSE;
	} else {
		mIsBuild = SVITRUE;
		return SVIPoly::build();
	}
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::setChangeShape()
{
    if (SVI_ALMOST_ZERO(mOutpit->mMorphingColor.a)) {
        mNeedMorphing = SVIFALSE;
    }
    else {
        mNeedMorphing = SVITRUE;
    }
    changeShape();
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::prepareFrontFace() 
{

}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::buildVertices()
{
    fillBuffers();
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::setup()
{

    if(mOutpit == NULL){
    	LOGE("Outpit is NULL", "SVITesselationPoly::setup", "Warning");
    	return;
    }else if (mOutpit->mTesselationPointList.size() < 3){
    	LOGE("TESSELATION(%s) - Type(%s), Tesselation point number less than 3", "SVITesselationPoly::setup", "Warning");
    	return;
    }

    mSize.x = mOutpit->mSize.x;
    mSize.y= mOutpit->mSize.y;


    mVerticesBuffer.clear();
    mIndicesBuffer.clear();

    std::vector<SVIVector2> pointList = mOutpit->mTesselationPointList;

    //copy tesselation point list
    mMorphingPointList.clear();
    mMorphingPointList.reserve(pointList.size());
    mMorphingPointList.insert(mMorphingPointList.begin(), pointList.begin(), pointList.end());
    
    float left, right, up, bottom;
    left = mSize.x; up = mSize.y; right = 0.0f; bottom = 0.0f;

    for(size_t i = 0; i < pointList.size(); ++i)
    {
        if(left > pointList[i].x) left = pointList[i].x;
        if(up > pointList[i].y) up = pointList[i].y;
        if(right < pointList[i].x) right = pointList[i].x;
        if(bottom < pointList[i].y) bottom = pointList[i].y;
    }

    mLeftUpPoint.x = left; 
    mLeftUpPoint.y = up;
    
    mRealSize.x = right - left;
    mRealSize.y = bottom - up;

    mCenterPoint.x = mRealSize.x * 0.5f + left;
    mCenterPoint.y = mRealSize.y * 0.5f + up;

    mTouchPoint.x = pointList[pointList.size() - 1].x;
    mTouchPoint.y = pointList[pointList.size() - 1].y;

    createBuffers(pointList, mVerticesBuffer, mIndicesBuffer);

    //copy tesselation vertex list
    mVerticesBufferDummy.clear();
    mVerticesBufferDummy.reserve(mVerticesBuffer.size());
    std::copy(mVerticesBufferDummy.begin(), mVerticesBufferDummy.end(), std::back_inserter(mVerticesBufferDummy));

    allocate(mVerticesBuffer.size(), mIndicesBuffer.size(), 0);
    
    if(ONLY_MORPHING == SVITRUE)
        mIndicesCount = 0;
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::fillBuffers()
{
    //set texture coordinate
    for(size_t i = 0; i < mVerticesBuffer.size(); ++i)
    {
        mTextureCoords[i].x = mVerticesBuffer[i].x / mSize.x;
        mTextureCoords[i].y = mVerticesBuffer[i].y / mSize.y;
    }

    for(size_t i = 0; i < mVerticesBuffer.size(); ++i)
    {
        mVertices[i].x = mVerticesBuffer[i].x;
        mVertices[i].y = mVerticesBuffer[i].y;
        //TODO : need resize?
        //mVertices[i].x = mVerticesBuffer[i].x - mLeftUpPoint.x;
        //mVertices[i].y = mVerticesBuffer[i].y - mLeftUpPoint.y;
        mVertices[i].z = 0.0f;
    }

    for(size_t i = 0; i < mIndicesBuffer.size(); ++i) 
        mIndices[i] = mIndicesBuffer[i];
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::fillMorphingVerticesBuffers()
{
    for(size_t i = 0; i < mMorphingVerticesBuffer.size(); ++i)
    {
        mMorphingVertices[i].x = mMorphingVerticesBuffer[i].x;
        mMorphingVertices[i].y = mMorphingVerticesBuffer[i].y;
        mMorphingVertices[i].z = 0.0f;
    }
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::fillMorphingIndicesBuffers()
{
    for(size_t i = 0; i < mMorphingIndicesBuffer.size(); ++i)
    {
        mMorphingIndices[i] = mMorphingIndicesBuffer[i];
    }
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::createBuffers(std::vector<SVIVector2>& aPointList, std::vector<SVIVector2>& aVertices, std::vector<SVIUShort>& aIndices)
{
    int nIndexCount = 0;

    // polygon triangulation
    std::vector<SVIVector2> triangleVec;
    triangulate(aPointList, triangleVec);

    for (std::size_t i = 0; i < triangleVec.size(); i+=3)
    {
        aVertices.push_back(SVIVector2(triangleVec[i+2].x, triangleVec[i+2].y));
        aVertices.push_back(SVIVector2(triangleVec[i+1].x, triangleVec[i+1].y));
        aVertices.push_back(SVIVector2(triangleVec[i].x, triangleVec[i].y));

        addTriangleIndices(aIndices, nIndexCount);
    }
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::triangulate(const std::vector<SVIVector2> &aSrcVertices, std::vector<SVIVector2> &aResultVertices)
{
    // allocate and initialize list of Vertices in polygon
    int nSrcVertexCount = (int)aSrcVertices.size();
    if (nSrcVertexCount < 3) return SVIFALSE;
    int *aIndexArray = new int[nSrcVertexCount];

    // make a counter-clockwise polygon in iIndexArray
    if (0.0f < area(aSrcVertices))
    {
        for (int i = 0; i < nSrcVertexCount; i++)
        {
            aIndexArray[i] = i;
        }
    }
    else
    {
        for (int i = 0; i < nSrcVertexCount; i++)
        {
            aIndexArray[i] = (nSrcVertexCount-1) - i;
        }
    }
    int nVertexCount = nSrcVertexCount;

    // remove nVertexCount-2 Vertices, creating 1 triangle every time
    int nCountForError = 2 * nVertexCount; // error detection
    for (int nTriCount = 0, iNewIdx = nVertexCount - 1; nVertexCount > 2;)
    {
        if (0 >= (nCountForError--))
        {
            // it is probably a non-simple polygon
            SVI_SVIFE_DELETE_ARRAY(aIndexArray);
            return SVIFALSE;
        }

        // three consecutive vertices in current polygon
        int iPrevIdx = iNewIdx;
        if (nVertexCount <= iPrevIdx)
        {
            iPrevIdx = 0;
        }
        iNewIdx = iPrevIdx + 1;
        if (nVertexCount <= iNewIdx)
        {
            iNewIdx = 0;
        }
        int iNextIdx = iNewIdx + 1;
        if (nVertexCount <= iNextIdx)
        {
            iNextIdx = 0;
        }

        if (snip(aSrcVertices, iPrevIdx, iNewIdx, iNextIdx, nVertexCount, aIndexArray))
        {
            int iVertexIdx1, iVertexIdx2, iVertexIdx3;
            // true names of the vertices 
            iVertexIdx1 = aIndexArray[iPrevIdx];
            iVertexIdx2 = aIndexArray[iNewIdx];
            iVertexIdx3 = aIndexArray[iNextIdx];

            // output Triangle
            aResultVertices.push_back(aSrcVertices[iVertexIdx1]);
            aResultVertices.push_back(aSrcVertices[iVertexIdx2]);
            aResultVertices.push_back(aSrcVertices[iVertexIdx3]);
            nTriCount++;

            // remove v from remaining polygon
            for (int i = iNewIdx, j = iNewIdx + 1; j < nVertexCount; i++, j++)
            {
                aIndexArray[i] = aIndexArray[j];
            }
            nVertexCount--;

            // resest error detection counter
            nCountForError = 2 * nVertexCount;
        }
    }
    SVI_SVIFE_DELETE_ARRAY(aIndexArray);
    return SVITRUE;
}
//--------------------------------------------------------------------------------------------------
SVIFloat SVITesselationPoly::area(const std::vector<SVIVector2> &aSrcVertices)
{
    int nSrcVertexCount = (int)aSrcVertices.size();
    float fA = 0.0f;
    for (int i = nSrcVertexCount - 1, j = 0; j < nSrcVertexCount; i = j++)
    {
        fA += aSrcVertices[i].x * aSrcVertices[j].y - aSrcVertices[j].x * aSrcVertices[i].y;
    }

    return fA * 0.5f;
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::snip(const std::vector<SVIVector2> &aSrcVertices, int aPrevIdx, int aNewIdx, int aNextIdx, int aVertexCount, int *aIndices)
{
    int iPointIdx;
    float fAx, fAy, fBx, fBy, fCx, fCy, fPx, fPy;
    fAx = aSrcVertices[aIndices[aPrevIdx]].x;
    fAy = aSrcVertices[aIndices[aPrevIdx]].y;
    fBx = aSrcVertices[aIndices[aNewIdx]].x;
    fBy = aSrcVertices[aIndices[aNewIdx]].y;
    fCx = aSrcVertices[aIndices[aNextIdx]].x;
    fCy = aSrcVertices[aIndices[aNextIdx]].y;
    if (1.192092896e-07F > (((fBx - fAx) * (fCy - fAy)) - ((fBy - fAy) * (fCx - fAx))))
    {
        return SVIFALSE;
    }
    for (iPointIdx = 0; iPointIdx < aVertexCount; iPointIdx++)
    {
        if ((iPointIdx == aPrevIdx) || (iPointIdx == aNewIdx) || (iPointIdx == aNextIdx))
        {
            continue;
        }
        fPx = aSrcVertices[aIndices[iPointIdx]].x;
        fPy = aSrcVertices[aIndices[iPointIdx]].y;
        if (insideTriangle(fAx, fAy, fBx, fBy, fCx, fCy, fPx, fPy))
        {
            return SVIFALSE;
        }
    }
    return SVITRUE;
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::insideTriangle(float aAx, float aAy, float aBx, float aBy, float aCx, float aCy, float aPx, float aPy)
{
    float fAX, fAY, fBX, fBY, fCX, fCY, fAPX, fAPY, fBPX, fBPY, fCPX, fCPY;
    float fCCrossAP, fBCrossCP, fACrossBP;

    fAX = aCx - aBx;
    fAY = aCy - aBy;
    fBX = aAx - aCx;
    fBY = aAy - aCy;
    fCX = aBx - aAx;
    fCY = aBy - aAy;
    fAPX = aPx - aAx;
    fAPY = aPy - aAy;
    fBPX = aPx - aBx;
    fBPY = aPy - aBy;
    fCPX = aPx - aCx;
    fCPY = aPy - aCy;
    fACrossBP = fAX * fBPY - fAY * fBPX;
    fCCrossAP = fCX * fAPY - fCY * fAPX;
    fBCrossCP = fBX * fCPY - fBY * fCPX;

    return ((fACrossBP >= 0.0f) && (fBCrossCP >= 0.0f) && (fCCrossAP >= 0.0f));
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::addTriangleIndices(std::vector<SVIUShort> &aIndices, int &aIndexCount)
{
    aIndices.push_back(aIndexCount);
    aIndices.push_back(aIndexCount + 1);
    aIndices.push_back(aIndexCount + 2);

    aIndexCount += 3;
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::getMorphingLeftUpPointAndCenterPoint(SVIVector2 &aLeftUpPoint, SVIVector2 &aCenterPoint)
{
    return SVITRUE;
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::buildTesselation()
{

}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::buildMorphing()
{
    mMorphingVerticesBuffer.clear();
    mMorphingIndicesBuffer.clear();

    createBuffers(mMorphingPointList, mMorphingVerticesBuffer, mMorphingIndicesBuffer);
    
    mMorphingVerticesCount = mMorphingVerticesBuffer.size();
    mMorphingIndicesCount = mMorphingIndicesBuffer.size();

    SVI_SVIFE_DELETE_ARRAY(mMorphingVertices);
    SVI_SVIFE_DELETE_ARRAY(mMorphingIndices);

    mMorphingVertices = new SVIVector3[mMorphingVerticesBuffer.size()];
    mMorphingIndices = new SVIUShort[mMorphingIndicesBuffer.size()];

    fillMorphingVerticesBuffers();
    fillMorphingIndicesBuffers();
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::createMorphingBuffers(std::vector<SVIVector2> &aPointList, std::vector<SVIVector2> &aVertices, std::vector<SVIUShort> &aIndices)
{
    
}
//--------------------------------------------------------------------------------------------------
SVITesselationPoly::Quadrant SVITesselationPoly::getQuadrant(SVIVector2 aPoint, SVIVector2 aCenterPoint)
{
    Quadrant result;
    
    if(aPoint.x > aCenterPoint.x)
    {
        if(aPoint.y < aCenterPoint.y)
            result = QUADRANT1;
        else
            result = QUADRANT4;
    }
    else
    {
        if(aPoint.y < aCenterPoint.y)
            result = QUADRANT2;
        else
            result = QUADRANT3;
    }

    return result;
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::checkConvex()
{
    return SVITRUE;
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::createClockwiseList(std::vector<SVIVector2> &aPointList)
{

}
//--------------------------------------------------------------------------------------------------
SVIUShort SVITesselationPoly::getMorphingFarIndex(SVIVector2 aPoint, std::vector<SVIVector2> &aPointList)
{
    SVIFloat farDistance = 0.0f;
    SVIFloat distance, distance_x, distance_y;
    SVIUShort index = 0; 
    for(int i = 0; i < aPointList.size(); ++i)
    {
        distance_x = aPoint.x - aPointList[i].x;
        distance_y = aPoint.y - aPointList[i].y;
        distance = (distance_x * distance_x) + (distance_y * distance_y);
        if(distance > farDistance)
        {
            index = i;
            farDistance = distance;
        }
    }
    
    return index;
}
//--------------------------------------------------------------------------------------------------
SVIBool compareXPositionAscending(const SVITesselationPoly::SVIMorphingQuadrant &left, const SVITesselationPoly::SVIMorphingQuadrant &right)
{
    if(left.mOriginPoint.x < right.mOriginPoint.x) 
        return true;

    return false;
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::initMorphingInfo()
{
    return SVITRUE;
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::createTestMorphingPointList(std::vector<SVIVector2> &aPointList)
{
    aPointList.clear();

    aPointList.push_back(SVIVector2(200.0f, 200.0f));
    aPointList.push_back(SVIVector2(200.0f, 150.0f));
    aPointList.push_back(SVIVector2(200.0f, 100.0f));
    aPointList.push_back(SVIVector2(200.0f, 50.0f));
    aPointList.push_back(SVIVector2(200.0f, 0.0f));

    aPointList.push_back(SVIVector2(0.0f, 0.0f));
    aPointList.push_back(SVIVector2(0.0f, 50.0f));
    aPointList.push_back(SVIVector2(0.0f, 100.0f));
    aPointList.push_back(SVIVector2(0.0f, 150.0f));
    aPointList.push_back(SVIVector2(0.0f, 200.0f));
}
//--------------------------------------------------------------------------------------------------
SVIBool SVITesselationPoly::createMorphingGridPointList(std::vector<SVIVector2> &aPointList)
{
    if(aPointList.size() < 3)
        return SVIFALSE;

    SVIFloat gridSizeY = mRealSize.y / (mMorphingVertexCount * 0.5f);

    SVIVector2 distance;
    SVIFloat quotient;

    SVIBool isEnd = SVIFALSE;
    SVIInt nextIndex = 0;
    //fill the space
    std::vector<SVIVector2> frontList;
    std::vector<SVIVector2> backList;
    SVIVector2 newPoint;
    while(isEnd == SVIFALSE)
    {
        ++nextIndex;
        if(nextIndex < aPointList.size())
        {
            distance = aPointList[nextIndex] - aPointList[nextIndex - 1];
            quotient = abs( distance.y / (gridSizeY * 0.5f) );
            if(quotient > 1.0f)
            {
                std::vector<SVIVector2> newPointList;
                for(int i = 0; i < (int)quotient; ++i)
                {
                    SVIFloat fI = i + 1;
                    newPoint.x = aPointList[nextIndex - 1].x + (distance.x * (fI / quotient));
                    newPoint.y = aPointList[nextIndex - 1].y + (distance.y * (fI / quotient));
                    newPointList.push_back(newPoint);
                }

                //insert
                if(newPointList.size() > 0)
                {
                    frontList.clear();
                    backList.clear();

                    frontList.insert(frontList.begin(), aPointList.begin(), aPointList.begin() + nextIndex);
                    backList.insert(backList.begin(), aPointList.begin() + nextIndex, aPointList.end());

                    for(int i = 0; i < newPointList.size(); ++i)
                    {
                        frontList.push_back(newPointList[i]);
                    }

                    aPointList.clear();
                    aPointList.reserve(frontList.size() + backList.size());
                    aPointList.insert(aPointList.begin(), frontList.begin(), frontList.end());
                    aPointList.insert(aPointList.end(), backList.begin(), backList.end());

                    nextIndex += newPointList.size();
                }
            }
        }
        //last point
        else
        {
            distance = aPointList[0] - aPointList[nextIndex - 1];
            quotient = abs( distance.y / (gridSizeY * 0.5f) );
            if(quotient > 1.0f)
            {
                std::vector<SVIVector2> newPointList;
                for(int i = 0; i < (int)quotient; ++i)
                {
                    SVIFloat fI = i + 1;
                    newPoint.x = aPointList[nextIndex - 1].x + (distance.x * (fI / quotient));
                    newPoint.y = aPointList[nextIndex - 1].y + (distance.y * (fI / quotient));
                    newPointList.push_back(newPoint);
                }

                //insert
                if(newPointList.size() > 0)
                {
                    frontList.clear();

                    frontList.reserve(aPointList.size());
                    frontList.insert(frontList.begin(), aPointList.begin(), aPointList.end());

                    for(int i = 0; i < newPointList.size(); ++i)
                    {
                        frontList.push_back(newPointList[i]);
                    }

                    aPointList.clear();
                    aPointList.reserve(frontList.size());
                    aPointList.insert(aPointList.begin(), frontList.begin(), frontList.end());
                }
            }

            isEnd = SVITRUE;
        }
    }

    //divide up and bottom point-list
    std::vector<SVIVector2> upPointList, bottomPointList;
    SVIFloat upCenter, bottomCenter;
    upCenter = mLeftUpPoint.y + gridSizeY * mMorphingQuadrantCount;
    bottomCenter = (mLeftUpPoint.y + mRealSize.y) - gridSizeY * mMorphingQuadrantCount;
    
    //why have two loop?(because need duplicate point)
    for(int i = 0; i < aPointList.size(); ++i)
    {
        if(aPointList[i].y < upCenter)
            upPointList.push_back(aPointList[i]);
    }
    for(int i = 0; i < aPointList.size(); ++i)
    {
        if(aPointList[i].y > bottomCenter)
            bottomPointList.push_back(aPointList[i]);
    }


    //create grid point-list
    std::vector<SVIVector2> gridPointList;
    SVIVector2 gridPoint(0.0f, 0.0f);
    SVIFloat x;

    //1quadrant point list
    for(int i = mMorphingQuadrantCount - 1; i >= 0; --i)
    {
        x = 0.0f;
        gridPoint.y = mLeftUpPoint.y + gridSizeY * i;
        
        for(int j = 0; j < upPointList.size(); ++j)
        {
            if( upPointList[j].y <= (gridPoint.y + gridSizeY) && upPointList[j].y >= (gridPoint.y - gridSizeY) )
            {
                if(upPointList[j].x > x)
                    x = upPointList[j].x;
            }
        }

        gridPoint.x = x;
        gridPointList.push_back(gridPoint);
    }

    //2quadrant point list
    for(int i = 0; i < mMorphingQuadrantCount; ++i)
    {
        x = mLeftUpPoint.x + mRealSize.x;
        gridPoint.y = mLeftUpPoint.y + gridSizeY * i;

        for(int j = 0; j < upPointList.size(); ++j)
        {
            if( upPointList[j].y <= (gridPoint.y + gridSizeY) && upPointList[j].y >= (gridPoint.y - gridSizeY) )
            {
                if(upPointList[j].x < x)
                    x = upPointList[j].x;
            }
        }

        gridPoint.x = x;
        gridPointList.push_back(gridPoint);
    }

    //3quadrant point list
    for(int i = mMorphingQuadrantCount - 1; i >= 0; --i)
    {
        x = mLeftUpPoint.x + mRealSize.x;
        gridPoint.y = (mLeftUpPoint.y + mRealSize.y) - gridSizeY * i;

        for(int j = 0; j < bottomPointList.size(); ++j)
        {
            if( bottomPointList[j].y <= (gridPoint.y + gridSizeY) && bottomPointList[j].y >= (gridPoint.y - gridSizeY) )
            {
                if(bottomPointList[j].x < x)
                    x = bottomPointList[j].x;
            }
        }

        gridPoint.x = x;
        gridPointList.push_back(gridPoint);
    }

    //4quadrant point list
    for(int i = 0; i < mMorphingQuadrantCount; ++i)
    {
        x = 0.0f;
        gridPoint.y = (mLeftUpPoint.y + mRealSize.y) - gridSizeY * i;

        for(int j = 0; j < bottomPointList.size(); ++j)
        {
            if( bottomPointList[j].y <= (gridPoint.y + gridSizeY) && bottomPointList[j].y >= (gridPoint.y - gridSizeY) )
            {
                if(bottomPointList[j].x > x)
                    x = bottomPointList[j].x;
            }
        }

        gridPoint.x = x;
        gridPointList.push_back(gridPoint);
    }

    aPointList.clear();
    aPointList.reserve(gridPointList.size());
    aPointList.insert(aPointList.begin(), gridPointList.begin(), gridPointList.end());

    mOriginMorphingPointList.clear();
    mOriginMorphingPointList.reserve(aPointList.size());
    mOriginMorphingPointList.insert(mOriginMorphingPointList.begin(), aPointList.begin(), aPointList.end());

    return SVITRUE;
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::changeShape()
{
    //create Morphing buffers.
    //2013-05-21 rareboy0112@facebook.com
    //check use Morphing
    if(mIsMorphingBuild != SVITRUE && mNeedMorphing)
    {
        mMorphingRadius = mOutpit->mMorphingRadius;

        mMorphingVertexCount = mOutpit->mMorphingVertexCount;
        mMorphingQuadrantCount = mMorphingVertexCount / 4;
        if(mMorphingVertexCount == 0)
        {
            mMorphingVertexCount = DEFAULT_VERTEX_COUNT;
            mMorphingQuadrantCount = mMorphingVertexCount / 4;
        }
        else if (mMorphingVertexCount < 4) {
            mMorphingVertexCount = 4;
            mMorphingQuadrantCount = mMorphingVertexCount / 4;
        }

        createMorphingGridPointList(mMorphingPointList);
        
        mMorphingFarIndex = getMorphingFarIndex(mTouchPoint, mMorphingPointList);

        buildMorphing();

        mIsMorphingBuild = SVITRUE;
    }
    else
    {
        animateMorphing(mOutpit->mMorphingTime);
    }
}
//--------------------------------------------------------------------------------------------------
void SVITesselationPoly::animateMorphing(SVIFloat aNormalizeTime)
{
    SVIFloat distance_x, distance_y;
    SVIFloat target_x, target_y;

    if(aNormalizeTime > 1.0f)
        aNormalizeTime = 1.0f;

    //2013-05-21 rareboy0112@facebook.com
    //check use Morphing
    if (mNeedMorphing) {
        //animate morphing mesh
        {
            SVIInt quadrantNum = mMorphingQuadrantCount;
            SVIFloat multiply_y = mMorphingRadius / (SVIFloat)mMorphingQuadrantCount;
            SVIInt turn;

            //morphing animation
            if( aNormalizeTime < mMorphingRatio )
            {
                SVIFloat timeRatio = aNormalizeTime * (1.0f / mMorphingRatio);
                if(timeRatio > 1.0f) timeRatio = 1.0f;

                for(int i = 0; i < mMorphingPointList.size(); ++i)
                {
                    //QUADRANT1
                    if(i < quadrantNum)
                    {
                        turn = i;
                        target_y = mTouchPoint.y - multiply_y * turn;
                        distance_y = target_y - mOriginMorphingPointList[i].y;

                        float gap_y = mTouchPoint.y - target_y;
                        target_x = mTouchPoint.x + sqrt(mMorphingRadius * mMorphingRadius - gap_y * gap_y);
                        distance_x = target_x - mOriginMorphingPointList[i].x;

                        mMorphingPointList[i].x = mOriginMorphingPointList[i].x + distance_x * timeRatio;
                        mMorphingPointList[i].y = mOriginMorphingPointList[i].y + distance_y * timeRatio;
                    }
                    //QUADRANT2
                    else if(i < quadrantNum * 2)
                    {
                        turn = i - quadrantNum;
                        target_y = mTouchPoint.y - (mMorphingRadius - multiply_y * turn);
                        distance_y = target_y - mOriginMorphingPointList[i].y;

                        float gap_y = mTouchPoint.y - target_y;
                        target_x = mTouchPoint.x - sqrt(mMorphingRadius * mMorphingRadius - gap_y * gap_y);
                        distance_x = target_x - mOriginMorphingPointList[i].x;

                        mMorphingPointList[i].x = mOriginMorphingPointList[i].x + distance_x * timeRatio;
                        mMorphingPointList[i].y = mOriginMorphingPointList[i].y + distance_y * timeRatio;
                    }
                    //QUADRANT3
                    else if(i < quadrantNum * 3)
                    {
                        turn = i - quadrantNum * 2;
                        target_y = mTouchPoint.y + multiply_y * turn;
                        distance_y = target_y - mOriginMorphingPointList[i].y;

                        float gap_y = mTouchPoint.y - target_y;
                        target_x = mTouchPoint.x - sqrt(mMorphingRadius * mMorphingRadius - gap_y * gap_y);
                        distance_x = target_x - mOriginMorphingPointList[i].x;

                        mMorphingPointList[i].x = mOriginMorphingPointList[i].x + distance_x * timeRatio;
                        mMorphingPointList[i].y = mOriginMorphingPointList[i].y + distance_y * timeRatio;
                    }
                    //QUADRANT4
                    else
                    {
                        turn = i - quadrantNum * 3;
                        target_y = mTouchPoint.y + (mMorphingRadius - multiply_y * turn);
                        distance_y = target_y - mOriginMorphingPointList[i].y;

                        float gap_y = mTouchPoint.y - target_y;
                        target_x = mTouchPoint.x + sqrt(mMorphingRadius * mMorphingRadius - gap_y * gap_y);
                        distance_x = target_x - mOriginMorphingPointList[i].x;

                        mMorphingPointList[i].x = mOriginMorphingPointList[i].x + distance_x * timeRatio;
                        mMorphingPointList[i].y = mOriginMorphingPointList[i].y + distance_y * timeRatio;
                    }
                }

                buildMorphing();
            }
            //scale animation
            else
            {
                SVIFloat scaleDuration = 1.0 - mMorphingRatio;
                SVIFloat timeRatio = (aNormalizeTime - mMorphingRatio) * (1.0f / scaleDuration);
                if(timeRatio > 1.0f) timeRatio = 1.0f;

                for(int i = 0; i < mMorphingVerticesCount; ++i)
                {
                    target_y = mTouchPoint.y;
                    distance_y = target_y - mMorphingVerticesBuffer[i].y;

                    target_x = mTouchPoint.x;
                    distance_x = target_x - mMorphingVerticesBuffer[i].x;

                    mMorphingVertices[i].x = mMorphingVerticesBuffer[i].x + distance_x * timeRatio;
                    mMorphingVertices[i].y = mMorphingVerticesBuffer[i].y + distance_y * timeRatio;
                }
            }

        } //end animate morphing mesh
    }

    //animate tesselation mesh
    {
        //move and scale(0.2)
        if(mOutpit->mMorphingTime > 0.2f && mOutpit->mMorphingTime < 0.7f)
        {
            SVIFloat ratio = (mOutpit->mMorphingTime - 0.2f) * (1.0f / 0.5f);
            SVIFloat scale_ratio = ratio * 0.8f;

            SVIFloat move_distance_x, move_distance_y;
            for(int i = 0; i < mVerticeCount; ++i)
            {
                SVIFloat x, y;
                target_y = mCenterPoint.y;
                distance_y = target_y - mVerticesBuffer[i].y;

                target_x = mCenterPoint.x;
                distance_x = target_x - mVerticesBuffer[i].x;

                move_distance_y = mTouchPoint.y - mCenterPoint.y;
                move_distance_x = mTouchPoint.x - mCenterPoint.x;

                x = mVerticesBuffer[i].x + distance_x * scale_ratio + move_distance_x * ratio;
                y = mVerticesBuffer[i].y + distance_y * scale_ratio + move_distance_y * ratio;

                mVertices[i].x = x;
                mVertices[i].y = y;

                mVerticesBufferDummy[i].x = x;
                mVerticesBufferDummy[i].y = y;
            }
        }
        //scale
        if(mOutpit->mMorphingTime > 0.7f)
        {
            SVIFloat ratio = (mOutpit->mMorphingTime - 0.7f) * (1.0f / 0.3f);

            for(int i = 0; i < mVerticeCount; ++i)
            {
                target_y = mTouchPoint.y;
                distance_y = target_y - mVerticesBufferDummy[i].y;

                target_x = mTouchPoint.x;
                distance_x = target_x - mVerticesBufferDummy[i].x;

                mVertices[i].x = mVerticesBufferDummy[i].x + distance_x * ratio;
                mVertices[i].y = mVerticesBufferDummy[i].y + distance_y * ratio;
            }
        }

    } //end animate tesselation mesh
}
//--------------------------------------------------------------------------------------------------
SVIInt SVITesselationPoly::getQuadrantIndex(int aSearchIndex, QuadrantList aSearchList)
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
SVIUShort* SVITesselationPoly::getMorphingIndices()
{
    return mMorphingIndices;
}
//--------------------------------------------------------------------------------------------------
SVIVector3* SVITesselationPoly::getMorphingVertices()
{
    return mMorphingVertices;
}
//--------------------------------------------------------------------------------------------------
SVIUInt SVITesselationPoly::getMorphingVerticesCount()
{
    return mMorphingVerticesCount;
}
//--------------------------------------------------------------------------------------------------
SVIUInt SVITesselationPoly::getMorphingIndicesCount()
{
    return mMorphingIndicesCount;
}
//--------------------------------------------------------------------------------------------------
SVITesselationPoly::~SVITesselationPoly() 
{
    SVI_SVIFE_DELETE_ARRAY(mMorphingVertices);
    SVI_SVIFE_DELETE_ARRAY(mMorphingIndices);
}
//--------------------------------------------------------------------------------------------------

} //end namespace SVI
