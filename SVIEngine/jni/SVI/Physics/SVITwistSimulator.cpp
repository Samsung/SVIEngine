#include "SVIPhysics.h"

#include "../Render/SVIDebugRenderer.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

#define GRID_DETAIL 20
#define GRID_VERTICAL_COUNT 20
	
	SVITwistSimulator::SVITwistSimulator() :
	mGridSize(GRID_DETAIL),
	mPivotCount(mGridSize*GRID_VERTICAL_COUNT),
	mControlTarget(0.0f,0.5f,0.0f),
	mControlBase(0.0f,0.5f,0.0f)
	{
		mPivots = new SVIPivot[mPivotCount];
		mLines = new SVISectorLine[mGridSize];
		reset();
	}

	SVITwistSimulator::~SVITwistSimulator(){
		SVI_SVIFE_DELETE_ARRAY(mPivots);
		SVI_SVIFE_DELETE_ARRAY(mLines);
	}
	

	void SVITwistSimulator::reset(){
		SVIFloat inverseRatio = 1.0f / (SVIFloat)(mGridSize-1);
		SVIFloat inverseVerticalRatio = 1.0f / (SVIFloat)(GRID_VERTICAL_COUNT-1);

		for (SVIInt y = 0; y < GRID_VERTICAL_COUNT; ++y){
			for (SVIUInt x = 0; x < mGridSize; ++x){
				SVIPivot * pPivot = &mPivots[y * mGridSize + x];
				pPivot->setPosition(SVIVector3((SVIFloat)x*inverseRatio,(SVIFloat)y*inverseVerticalRatio,0));
			}
		}

		for (SVIUInt x = 0; x < mGridSize; ++x){
			SVISectorLine * pLine = &mLines[x];
			for (SVIInt y = 0; y < GRID_VERTICAL_COUNT; ++y){
				SVIPivot * pPivot = &mPivots[y * mGridSize + x];
				if (y == 0) pLine->mStart = pPivot; else
					if (y == GRID_VERTICAL_COUNT-1) pLine->mEnd = pPivot; 
				pLine->mNaturalLength = 1.0f;
				pLine->mActiveLength = 1.0f;
				pLine->mPivots.push_back(pPivot);
			}
		}		
		
	}
	
	void SVITwistSimulator::setControlTarget(const SVIVector3& target){
		mControlTarget = target;
	}
	
	
	void SVITwistSimulator::updateTarget(){
		mControlBase.x = mControlTarget.x * SVI_2_PI;
		mControlBase.y = mControlTarget.y;								
	}


	void SVITwistSimulator::update(){
		updateTarget();

		SVIFloat ratioBasis = mControlTarget.x;
		SVIFloat radiusBasis = mControlBase.y;
		SVIFloat horzontalThetaBasis = (mControlBase.x) + (SVI_PI * 0.5f);
		
		SVIFloat theta = horzontalThetaBasis;
		SVIFloat effectiveRange = mControlTarget.z;

		for (SVIUInt n = 0; n < mGridSize; ++n){

			SVIFloat ratio = (SVIFloat)n / (SVIFloat)(mGridSize-1);
			
			if (ratioBasis-effectiveRange > ratio){
				theta = SVI_PI + (SVI_PI * 0.5f);
			}else if (ratioBasis+effectiveRange < ratio){
				theta = (SVI_PI * 0.5f);
			}else{
				float tempratio = (ratio - (ratioBasis-effectiveRange)) / (effectiveRange * 2.0f);

				//if (effectiveness < effectiveRange) effectiveness = effectiveness / effectiveRange; else effectiveness = 1.0f;
				theta = SVI_PI + SVI_PI * tempratio + (SVI_PI * 0.5f);
			}

			SVISectorLine * pLine = &mLines[n];	
			SVIVector3 &start = pLine->mStart->getPosition();
			SVIVector3 &end = pLine->mEnd->getPosition();
			SVIVector3 center = (end + start) * 0.5f;

			SVIVector3 rotationDir(0.0f,sinf(theta),cosf(theta));
			rotationDir.normalize();

			start = center - rotationDir * radiusBasis;
			end = center + rotationDir * radiusBasis;		



			SVIInt pivotCount = pLine->mPivots.size()-1;
			SVIFloat stepDelta = radiusBasis * 2.0f / pivotCount;
			SVIVector3 stepBase = start;

			for (SVIInt k = 0; k < pivotCount; ++k){
				SVIPivot * pPivot = pLine->mPivots[k];
				if (pPivot == pLine->mStart || pPivot == pLine->mEnd) continue;
				pPivot->setPosition(stepBase + rotationDir * (stepDelta * (SVIFloat)k));
			}
		}
	}
	
	SVIUInt SVITwistSimulator::getIndicesCount(){
		return (mGridSize-1) * (GRID_VERTICAL_COUNT-1) * 6;
	}

	SVIUInt SVITwistSimulator::getVerticesCount(){
		return (mGridSize-1) * (GRID_VERTICAL_COUNT-1) * 4;
	}
	
	void SVITwistSimulator::build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,
		SVIUShort * pIndices,SVIVector3 * pNormals,SVIVector2 * pTexcoords,SVIBool isVertical, SVIBool isReverse, SVIBool bFliped){


		SVIUInt indexVertex = 0;
		SVIUInt index = 0;
				
		for (SVIInt n = 0; n < mPivotCount; ++n){
			mPivots[n].setNormal(SVIVector3(0.0f,0.0f,0.0f));
		}

		for (SVIInt y = 0; y < GRID_VERTICAL_COUNT-1; ++y){
			for (SVIUInt x = 0; x < mGridSize-1; ++x){
				SVIVector3& p0 = mPivots[y * mGridSize + x].getPosition(); 
				SVIVector3& p1 = mPivots[y * mGridSize + x + 1].getPosition();
				SVIVector3& p2 = mPivots[(y + 1) * mGridSize + x].getPosition();
				SVIVector3& p3 = mPivots[(y + 1) * mGridSize + (x + 1)].getPosition();
				
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
				SVIVector3& p0 = mPivots[y * mGridSize + x].getPosition(); 
				SVIVector3& p1 = mPivots[y * mGridSize + x + 1].getPosition();
				SVIVector3& p2 = mPivots[(y + 1) * mGridSize + x].getPosition();
				SVIVector3& p3 = mPivots[(y + 1) * mGridSize + (x + 1)].getPosition();
				

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
						pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)(x) * tuR,1.0f - (SVIFloat)(y) * tvR);						
						pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)(x+1) * tuR,1.0f - (SVIFloat)(y) * tvR);								
						pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(x) * tuR,1.0f - (SVIFloat)(y+1)* tvR);							
						pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(x+1) * tuR,1.0f - (SVIFloat)(y+1)* tvR);			
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
