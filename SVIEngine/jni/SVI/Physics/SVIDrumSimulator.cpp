#include "SVIPhysics.h"

#include "../Render/SVIDebugRenderer.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

#define GRID_DETAIL 20
#define GRID_VERTICAL_COUNT 20
	
	SVIDrumSimulator::SVIDrumSimulator() :
	mGridSize(GRID_DETAIL),
	mPivotCount(mGridSize*GRID_VERTICAL_COUNT),
	mControlTarget(0.0f,0.5f,0.0f)
	{
		mPivots = new SVIPivot[mPivotCount];
        mTargetPivots = new SVIPivot[mPivotCount];
		reset();
	}

	SVIDrumSimulator::~SVIDrumSimulator(){
		SVI_SVIFE_DELETE_ARRAY(mPivots);
        SVI_SVIFE_DELETE_ARRAY(mTargetPivots);
	}
	

	void SVIDrumSimulator::reset(){
		SVIFloat inverseRatio = 1.0f / (SVIFloat)(mGridSize-1);
		SVIFloat inverseVerticalRatio = 1.0f / (SVIFloat)(GRID_VERTICAL_COUNT-1);

		for (SVIInt y = 0; y < GRID_VERTICAL_COUNT; ++y){
			for (SVIUInt x = 0; x < mGridSize; ++x){
				SVIPivot * pPivot = &mPivots[y * mGridSize + x];
				pPivot->setPosition(SVIVector3((SVIFloat)x*inverseRatio,(SVIFloat)y*inverseVerticalRatio,0));
			}
		}		

        //let's generate cylinder
        for (SVIInt y = 0; y < GRID_VERTICAL_COUNT; ++y){
            for (SVIUInt x = 0; x < mGridSize; ++x){

                float xGapRadian = SVI_PI * 0.05f;
                float xStepRadian = (SVI_PI * 2.0f - xGapRadian) / (float)mGridSize;
                float xRatioBasedYDelta = ((float)x / (float)mGridSize - 0.5f) * 0.06f;

                float xRadius = 0.5f;

                float xPos = 0.5f + cos(SVI_PI*1.5f - xStepRadian * (float)x) * xRadius;
                float zPos = sin(SVI_PI*1.5f - xStepRadian * (float)x) * xRadius; // 0.5f + 

                SVIPivot * pPivot = &mTargetPivots[y * mGridSize + x];
                pPivot->setPosition(SVIVector3(xPos,(SVIFloat)y*inverseVerticalRatio + xRatioBasedYDelta,zPos));
            }
        }
    }
	
	void SVIDrumSimulator::setControlTarget(const SVIVector3& target){
		mControlTarget = target;
	}
	
	
	void SVIDrumSimulator::updateTarget(){
						
	}


	void SVIDrumSimulator::update(){
        updateTarget();

	}
	
	SVIUInt SVIDrumSimulator::getIndicesCount(){
		return (mGridSize-1) * (GRID_VERTICAL_COUNT-1) * 6;
	}

	SVIUInt SVIDrumSimulator::getVerticesCount(){
		return (mGridSize-1) * (GRID_VERTICAL_COUNT-1) * 4;
	}
	
    void getIntPosition(float t, SVIVector3 * vA,SVIVector3 * vB, SVIVector3 * vC ){
        
        vC->x = vA->x + (vB->x - vA->x)*t;
        vC->y = vA->y + (vB->y - vA->y)*t;
        vC->z = vA->z + (vB->z - vA->z)*t;      
        
        /*
        vC->x = vB->x;
        vC->y = vB->y;
        vC->z = vB->z;
        */
    }

	void SVIDrumSimulator::build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,
		SVIUShort * pIndices,SVIVector3 * pNormals,SVIVector2 * pTexcoords,SVIBool isVertical, SVIBool isReverse, SVIBool bFliped){

        float ratio = mControlTarget.x;
        float ratioLeft = mControlTarget.y;
        float ratioRight = mControlTarget.z;

        SVIUInt indexVertex = 0;
        SVIUInt index = 0;

        for (SVIInt n = 0; n < mPivotCount; ++n){
            mPivots[n].setNormal(SVIVector3(0.0f,0.0f,0.0f));
        }

        for (SVIInt y = 0; y < GRID_VERTICAL_COUNT-1; ++y){
            for (SVIUInt x = 0; x < mGridSize-1; ++x){
                SVIVector3 p0,p1,p2,p3;

                float filteredRatio = ratio;
                /*
                if (x < mGridSize / 2){
                    filteredRatio = ratioLeft * ratio;
                }else{
                    filteredRatio = ratioRight * ratio;                    
                }*/

                getIntPosition(filteredRatio,&mPivots[y * mGridSize + x].getPosition(),&mTargetPivots[y * mGridSize + x].getPosition(),&p0); 
                getIntPosition(filteredRatio,&mPivots[y * mGridSize + x + 1].getPosition(),&mTargetPivots[y * mGridSize + x + 1].getPosition(),&p1);
                getIntPosition(filteredRatio,&mPivots[(y + 1) * mGridSize + x].getPosition(),&mTargetPivots[(y + 1) * mGridSize + x].getPosition(),&p2);
                getIntPosition(filteredRatio,&mPivots[(y + 1) * mGridSize + (x + 1)].getPosition(),&mTargetPivots[(y + 1) * mGridSize + (x + 1)].getPosition(),&p3);

                SVIVector3& n0 = mPivots[y * mGridSize + x].getNormal(); 
                SVIVector3& n1 = mPivots[y * mGridSize + x + 1].getNormal();
                SVIVector3& n2 = mPivots[(y + 1) * mGridSize + x].getNormal();
                SVIVector3& n3 = mPivots[(y + 1) * mGridSize + (x + 1)].getNormal();

                SVIVector3 normal = (p1 - p0).Cross(p2 - p0);
                if (!bFliped) normal = normal * -1.0f;
                n0 += normal;
                n1 += normal;
                n2 += normal;

                normal = (p1 - p2).Cross(p3 - p2);
                if (!bFliped) normal = normal * -1.0f;
                n1 += normal;
                n2 += normal;
                n3 += normal;
            }
        }

        for (SVIInt n = 0; n < mPivotCount; ++n){
            mPivots[n].getNormal().normalize();
        }
			
		//2012-02-09 masterkeaton27@gmail.com
		//let reverse action to page
		SVIVector3 filteredScale = scale;
		if (isReverse){
			if (isVertical) {
				filteredScale.y *= -1.0f;
			}else filteredScale.x *= -1.0f;
		}

		float tuR = 1.0f / (float)(mGridSize-1);
        float tvR = 1.0f / (float)(GRID_VERTICAL_COUNT-1);
        for (SVIInt y = 0; y < GRID_VERTICAL_COUNT-1; ++y){
            for (SVIUInt x = 0; x < mGridSize-1; ++x){

                SVIVector3 p0,p1,p2,p3;

                float filteredRatio = ratio;
                /*
                if (x < mGridSize / 2){
                    filteredRatio = ratioLeft * ratio;
                }else{
                    filteredRatio = ratioRight * ratio;                    
                }*/

                getIntPosition(filteredRatio,&mPivots[y * mGridSize + x].getPosition(),             &mTargetPivots[y * mGridSize + x].getPosition(),                &p0); 
                getIntPosition(filteredRatio,&mPivots[y * mGridSize + x + 1].getPosition(),         &mTargetPivots[y * mGridSize + x + 1].getPosition(),            &p1);
                getIntPosition(filteredRatio,&mPivots[(y + 1) * mGridSize + x].getPosition(),       &mTargetPivots[(y + 1) * mGridSize + x].getPosition(),          &p2);
                getIntPosition(filteredRatio,&mPivots[(y + 1) * mGridSize + (x + 1)].getPosition(), &mTargetPivots[(y + 1) * mGridSize + (x + 1)].getPosition(),    &p3);
                
                if (isVertical){
                    SVIVector3 pT0 = SVIVector3(p0.y,p0.x,p0.z);
                    SVIVector3 pT1 = SVIVector3(p1.y,p1.x,p1.z);
                    SVIVector3 pT2 = SVIVector3(p2.y,p2.x,p2.z);
                    SVIVector3 pT3 = SVIVector3(p3.y,p3.x,p3.z);

                    pPositions[indexVertex+0] = offset + pT0 * filteredScale;
                    pPositions[indexVertex+1] = offset + pT1 * filteredScale;
                    pPositions[indexVertex+2] = offset + pT2 * filteredScale;
                    pPositions[indexVertex+3] = offset + pT3 * filteredScale;			
                }else{
                    pPositions[indexVertex+0] = offset + p0 * filteredScale;
                    pPositions[indexVertex+1] = offset + p1 * filteredScale;
                    pPositions[indexVertex+2] = offset + p2 * filteredScale;
                    pPositions[indexVertex+3] = offset + p3 * filteredScale;			
				}		

                pNormals[indexVertex+0] = mPivots[y * mGridSize + x].getNormal();
                pNormals[indexVertex+1] = mPivots[y * mGridSize + (x + 1)].getNormal();
                pNormals[indexVertex+2] = mPivots[(y + 1) * mGridSize + x].getNormal();
                pNormals[indexVertex+3] = mPivots[(y + 1) * mGridSize + (x + 1)].getNormal();

                if (isVertical){
                    if (!bFliped){			
                        pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)(y) * tvR,(SVIFloat)(x) * tuR);				
                        pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)(y) * tvR,(SVIFloat)(x+1) * tuR);				
                        pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(y+1)* tvR,(SVIFloat)(x) * tuR);					
                        pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(y+1)* tvR,(SVIFloat)(x+1) * tuR);
                    }else{
                        pTexcoords[indexVertex+0] = SVIVector2(1.0f - (SVIFloat)(y) * tvR,(SVIFloat)(x) * tuR);						
                        pTexcoords[indexVertex+1] = SVIVector2(1.0f - (SVIFloat)(y) * tvR,(SVIFloat)(x+1) * tuR);								
                        pTexcoords[indexVertex+2] = SVIVector2(1.0f - (SVIFloat)(y+1)* tvR,(SVIFloat)(x) * tuR);							
                        pTexcoords[indexVertex+3] = SVIVector2(1.0f - (SVIFloat)(y+1)* tvR,(SVIFloat)(x+1) * tuR);			
                    }
                }else{
                    if (!bFliped){			
                        pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y) * tvR);				
                        pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)(x+1) * tuR,(SVIFloat)(y) * tvR);				
                        pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y+1)* tvR);					
                        pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(x+1) * tuR,(SVIFloat)(y+1)* tvR);
                    }else{
                        pTexcoords[indexVertex+0] = SVIVector2(1.0f - (SVIFloat)(x) * tuR,(SVIFloat)(y) * tvR);						
                        pTexcoords[indexVertex+1] = SVIVector2(1.0f - (SVIFloat)(x+1) * tuR,(SVIFloat)(y) * tvR);								
                        pTexcoords[indexVertex+2] = SVIVector2(1.0f - (SVIFloat)(x) * tuR,(SVIFloat)(y+1)* tvR);							
                        pTexcoords[indexVertex+3] = SVIVector2(1.0f - (SVIFloat)(x+1) * tuR,(SVIFloat)(y+1)* tvR);			
                    }
                }

                if (bFliped){
					pIndices[index + 0] = indexVertex+0;
					pIndices[index + 1] = indexVertex+1;
					pIndices[index + 2] = indexVertex+2;

					pIndices[index + 3] = indexVertex+2;
					pIndices[index + 4] = indexVertex+1;
					pIndices[index + 5] = indexVertex+3;
				}else{
					pIndices[index + 0] = indexVertex+2;
					pIndices[index + 1] = indexVertex+1;
					pIndices[index + 2] = indexVertex+0;

					pIndices[index + 3] = indexVertex+3;
					pIndices[index + 4] = indexVertex+1;
					pIndices[index + 5] = indexVertex+2;
				}
				indexVertex += 4;
				index += 6;
			}
		}		
	}



};
