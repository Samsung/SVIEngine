#ifndef __SVI_TESSELATION_POLY_H_
#define __SVI_TESSELATION_POLY_H_

#include "../../SVICores.h"
#include "SVIPoly.h"

namespace SVI {

    struct SVISlideOutfit;
    
    class SVITesselationPoly : public SVIPoly
    {
    public:
        SVITesselationPoly(SVIGLSurface *surface);
        
        virtual ~SVITesselationPoly();
    
    //2012/12/10 rareboy0112@facebook.com
    //TODO : move Morphing class
    public:
        enum Quadrant
        {
            QUADRANT1 = 0,
            QUADRANT2,
            QUADRANT3,
            QUADRANT4,
            QUADRANT_COUNT
        };

        enum Direction
        {
            UP = 1,
            DOWN,
            LEFT,
            RIGHT
        };

        struct SVIMorphingInfo
        {
            SVIVector2   mOriginPoint;
            Quadrant    mQuadrant;
            SVIInt       mTurnNumber;
        };

        struct SVIMorphingQuadrant
        {
            SVIVector2   mOriginPoint;
            SVIInt       mIndex;
        };

        typedef std::vector<SVIVector2>          PointList;
        typedef std::vector<SVIVector2>          VertexList;
        typedef std::vector<SVIUShort>           IndexList;
        typedef std::vector<SVIMorphingQuadrant> QuadrantList;
        typedef std::vector<SVIMorphingInfo>     MorphingInfoList;

        SVIUInt      mMorphingVerticesCount;
        SVIUInt      mMorphingIndicesCount;
        SVIVector3*  mMorphingVertices;
        SVIUShort*   mMorphingIndices;
    //END :

    private:
        SVISlideOutfit       *mOutpit;

        //Tesselation values
        SVIVector2           mSize;
        SVIVector2           mRealSize;
        SVIVector2           mLeftUpPoint;
        SVIVector2           mCenterPoint;
        SVIBool              mIsBuild;
        VertexList          mVerticesBuffer;
        IndexList           mIndicesBuffer;

        VertexList          mVerticesBufferDummy;

        //Morphing values
        SVIBool              mIsMorphingBuild;
        SVIBool              mIsConvex;
        SVIBool              mInitializedMorphingInfo;
        SVIBool              mClockwise;

        SVIVector2           mTouchPoint;
        SVIVector2           mMorphingLeftUpPoint;
        SVIVector2           mMorphingCenterPoint;
        SVIVector2           mPreMorphingTouchPoint;

        //mMorphingInfoList more than one(because have center vector)
        VertexList          mMorphingVerticesBuffer;
        IndexList           mMorphingIndicesBuffer;
        Quadrant            mStartQuadrant;
        QuadrantList        mQuadrant1List;
        QuadrantList        mQuadrant2List;
        QuadrantList        mQuadrant3List;
        QuadrantList        mQuadrant4List;

        float               mDegreeMorphingRotateZ;
        PointList           mMorphingPointList;
        PointList           mOriginMorphingPointList;
        
        //MorphingInfoList    mMorphingInfoList;
        SVIFloat             mMorphingRadius;

        SVIUShort            mMorphingCenterIndex;
        SVIUShort            mMorphingStartIndex;
        SVIUShort            mMorphingFarIndex;

        SVIFloat             mConvexCompletedTime;
        SVIFloat             mMorphingRatio;

        SVIInt               mMorphingVertexCount;
        SVIInt               mMorphingQuadrantCount;

        //poly common initialization
        void init();

        void fillBuffers();

    public:
        void setOutfit(SVISlideOutfit *pOutpit);
        
        SVIBool build();

        void setChangeShape();

        virtual void prepareFrontFace();

        //2012/12/10 rareboy0112@facebook.com
        //TODO : move MorphingPoly class
        SVIUShort* getMorphingIndices();
        SVIVector3* getMorphingVertices();
        SVIUInt getMorphingVerticesCount();
        SVIUInt getMorphingIndicesCount();
        //END :

    protected:
        virtual void buildVertices();

        virtual void setup();

        void createBuffers(std::vector<SVIVector2> &aPointList, std::vector<SVIVector2> &aVertices, std::vector<SVIUShort> &aIndices);
        
        //tesselation method
        void addTriangleIndices(std::vector<SVIUShort> &aIndices, int &aIndexCount);
        SVIBool triangulate(const std::vector<SVIVector2> &aSrcVertices, std::vector<SVIVector2> &aResultVertices);
        SVIFloat area(const std::vector<SVIVector2> &aSrcVertices);
        SVIBool snip(const std::vector<SVIVector2> &aSrcVertices, int aPrevIdx, int aNewIdx, int aNextIdx, int aVertexCount, int *aIndices);
        SVIBool insideTriangle(float aAx, float aAy, float aBx, float aBy, float aCx, float aCy, float aPx, float aPy);

        //2012/12/10 rareboy0112@facebook.com
        //TODO : move MorphingPoly class

        //START : auto Convex method.
        Quadrant getQuadrant(SVIVector2 aPoint, SVIVector2 aCenterPoint);
        SVIBool checkConvex();
        //create Fan style mesh
        void createMorphingBuffers(std::vector<SVIVector2> &aPointList, std::vector<SVIVector2> &aVertices, std::vector<SVIUShort> &aIndices);
        void createClockwiseList(std::vector<SVIVector2> &aPointList);
        SVIInt getQuadrantIndex(int aSearchIndex, QuadrantList aSearchList);
        void changeConvex();
        //END : auto Convex method.

        SVIBool getMorphingLeftUpPointAndCenterPoint(SVIVector2 &aLeftUpPoint, SVIVector2 &aCenterPoint);
        SVIBool createMorphingGridPointList(std::vector<SVIVector2> &aPointList);
        void buildMorphing();

        //2013/02/05 rareboy0112@facebook.com
        //changed UX(tesselation geometry morphing)
        void buildTesselation();
        
        SVIUShort getMorphingFarIndex(SVIVector2 aPoint, std::vector<SVIVector2> &aPointList);
        SVIBool initMorphingInfo();
        void changeShape();
        void fillMorphingVerticesBuffers();
        void fillMorphingIndicesBuffers();
        
        void animateMorphing(SVIFloat aNormalizeTime);

        //only test morphing method.
        void createTestMorphingPointList(std::vector<SVIVector2> &aPointList);
        //END :
    };

}; //end namespace SVI

#endif //__SVI_TESSELATION_POLY_H_
