#include "SVIPhysics.h"

#include "../Render/SVIDebugRenderer.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

#define GRID_DETAIL 15
	
	SVICurveSimulator::SVICurveSimulator() :
	mGridSize(GRID_DETAIL),
	mPivotCount(mGridSize*mGridSize),
	mControlTarget(1.0f,0.0f,0.0f),
	mControlBase(1.0f,0.0f,0.0f)
	{
		mPivots = new SVIPivot[mPivotCount];
		mLines = new SVISectorLine[mGridSize];
		reset();
	}

	SVICurveSimulator::~SVICurveSimulator(){
		SVI_SVIFE_DELETE_ARRAY(mPivots);
		SVI_SVIFE_DELETE_ARRAY(mLines);
	}

	void SVISectorLine::calculateActiveLength(){
		SVIFloat length = 0.0f;
		SVIInt count = mPivots.size();
		SVIPivot * pCurrentPivot = NULL;
		SVIPivot * pNextPivot = NULL;
		for (SVIInt n = 0;  n < count-1; n++){
			pCurrentPivot = mPivots[n];
			pNextPivot = mPivots[n+1];
			SVIVector3 & currentPos = pCurrentPivot->getPosition();
			SVIVector3 & nextPos = pNextPivot->getPosition();

			SVIFloat fDistance = currentPos.distance(nextPos);
			pCurrentPivot->setMass(length);
			length += fDistance;
		}
		mEnd->setMass(length);
		mActiveLength = length;
	}

	void SVICurveSimulator::reset(){
		SVIFloat inverseRatio = 1.0f / (SVIFloat)(mGridSize-1);

		for (SVIUInt y = 0; y < mGridSize; ++y){
			for (SVIUInt x = 0; x < mGridSize; ++x){
				SVIPivot * pPivot = &mPivots[y * mGridSize + x];
				pPivot->setPosition(SVIVector3((SVIFloat)x*inverseRatio,(SVIFloat)y*inverseRatio,0));
			}
		}

		for (SVIUInt y = 0; y < mGridSize; ++y){
			SVISectorLine * pLine = &mLines[y];			
			for (SVIUInt x = 0; x < mGridSize; ++x){
				SVIPivot * pPivot = &mPivots[y * mGridSize + x];
				if (x == 0) pLine->mStart = pPivot; else
					if (x == (SVIUInt)(mGridSize-1)) pLine->mEnd = pPivot; 
				pLine->mNaturalLength = 1.0f;
				pLine->mActiveLength = 1.0f;
				pLine->mPivots.push_back(pPivot);
			}
		}		

		for (SVIUInt n = 0; n < mGridSize; ++n){
			SVISectorLine * pLine = &mLines[n];		
			if (n == 0) mControlLine.mStart = pLine->mEnd; else
				if (n == (SVIUInt)(mGridSize-1)) mControlLine.mEnd = pLine->mEnd;
			mControlLine.mNaturalLength = 1.0f;
			mControlLine.mActiveLength = 1.0f;
			mControlLine.mPivots.push_back(pLine->mEnd); 
		}		
	}
	
	void SVICurveSimulator::setControlTarget(const SVIVector3& target){
		mControlTarget = target;
	}


	SVIVector3 testMove(const SVIVector3& target){
		SVIVector3 side = SVIVector3(0.0,1.0,0.0);
		SVIVector3 up = SVIVector3(0.0,0.0,1.0);
		SVIVector3 dir = SVIVector3(1.0,0.0,0.0);
		

		SVIVector3 controlTarget = target;
		SVIFloat splitRatio = target.y;//0.5f;

		float curvecos = cosf(-controlTarget.x*SVI_PI);
		float curvesin = sinf(-controlTarget.x*SVI_PI);


		//2011-11-17 masterkeaton27@gmail.com
		//should replace with mControlTarget's z factor
		float curvevalue = SVIMAX(0.65f,abs(sinf(controlTarget.x*SVI_PI + SVI_PI * 0.5f)));

		//test moving control target
		up = dir.Cross(side);
		controlTarget = up * curvesin + dir * curvecos;
		controlTarget.normalize();
		controlTarget = side * splitRatio + controlTarget * curvevalue;

		return controlTarget;
	}

	void SVICurveSimulator::updateTarget(SVIBool fakeDepth, SVIFloat fakeDepthMax){
		float curvecos = cosf(- mControlTarget.x * SVI_PI);
		float curvesin = sinf(- mControlTarget.x * SVI_PI);
		SVIFloat splitRatio = mControlTarget.y;//0.5f;
		SVIFloat invSplitRatio = 1.0f - splitRatio;
		SVIFloat targetHeight = fakeDepthMax;
		SVIVector3 controlTarget = testMove(mControlTarget);


		//2011-11-17 masterkeaton27@gmail.com
		//should have target coordinate validation check here before set to control base.
		mControlBase = controlTarget;

		SVIVector3 &controlStart = mControlLine.mStart->getPosition();
		SVIVector3 &controlEnd = mControlLine.mEnd->getPosition();
				

		SVIFloat controlTheta = SVI_PI*0.5f + (cosf(splitRatio* SVI_PI)* 0.5f * curvesin);
		SVIVector3 controlDirection;
		controlDirection.x = cosf(controlTheta);
		controlDirection.y = sinf(controlTheta);
		controlDirection.z = 0.0f;
		controlDirection.normalize();

		controlStart = mControlBase - controlDirection * splitRatio;
		controlEnd = mControlBase + controlDirection * invSplitRatio;
		

		controlDirection = controlEnd - controlStart;
		controlDirection.normalize();


		SVIFloat stepRatio = 1.0f / (SVIFloat)(mGridSize-1);
		for (SVIUInt n = 0; n < mGridSize; ++n){
			SVIPivot * pPivot = mControlLine.mPivots[n];
			if (pPivot == mControlLine.mStart || pPivot == mControlLine.mEnd) continue;
			SVIVector3& pos = pPivot->getPosition();
			pos = controlStart + controlDirection * stepRatio * (SVIFloat)n;
		}
	}


	void SVICurveSimulator::update(SVIBool fakeDepth, SVIFloat fakeDepthMax){
		updateTarget(fakeDepth,fakeDepthMax);

		SVIVector3 side = SVIVector3(0.0,1.0,0.0);
		SVIFloat targetHeight = fakeDepthMax;
		SVIFloat seperateRatio = 0.5f;
		SVIFloat thetaRatio = 0.0f;
		thetaRatio = (seperateRatio < 0.5f) ? thetaRatio : 1.0f;

		int index = mGridSize / 2;
		SVIVector3 basePosition = mLines[index].mStart->getPosition();

		for (SVIUInt n = 0; n < mGridSize; ++n){
			SVISectorLine * pLine = &mLines[n];		
			SVIVector3 start = pLine->mStart->getPosition();
			SVIVector3 direction = pLine->mEnd->getPosition() - pLine->mStart->getPosition();
			SVIFloat fDist = direction.getlength();
			direction.normalize();

			//2011-11-17 masterkeaton27@gmail.com
			//end position exception check : the length of direction should equal or less then one.
			if (fDist > pLine->mNaturalLength){
				SVIVector3 resultPosition = start + direction;
				pLine->mEnd->setPosition(resultPosition);				
			}
			
			//up vector exception check
			SVIVector3 up = side.Cross(direction);
			if (up.z > 0.0f){
				up.z = 0.0f;
				up.normalize();
			}			

			SVIInt pivotCount = pLine->mPivots.size() -1;
			SVIFloat stepTheta = (SVI_PI*thetaRatio) / (SVIFloat)(pivotCount);			
			
			//float stepRatio = (1.0f / (float)(pivotCount)) * fDist * seperateRatio;

			float fDistUp = sqrt(0.25f - (fDist*0.5f*fDist*0.5f)) * 1.5f;
			float fUpAdaptValue = 1.0f;
			float fDeltaUp = (1.0f / ((float)(pivotCount)));
			
			SVIVector3 stepBase = start + (direction * seperateRatio * fDist);
			for (SVIInt k = 0; k <= pivotCount; k++){
				SVIPivot * pPivot = pLine->mPivots[k];

				//pPivot == pLine->mStart 
				if ( pPivot == pLine->mEnd ){
					SVIVector3 resultPosition = pPivot->getPosition();
					SVIVector3 adaptationPosition = SVIVector3(resultPosition.x - basePosition.x,resultPosition.y - basePosition.y,0.0f) * (resultPosition.z / targetHeight);
					if (!fakeDepth) adaptationPosition = SVIVector3(0.0f,0.0f,0.0f);
					pPivot->setPosition(resultPosition - adaptationPosition);			
				}

				if (pPivot == pLine->mStart || pPivot == pLine->mEnd) continue;

				fUpAdaptValue -= fDeltaUp;
				SVIFloat sinTheta = sinf(SVI_PI - stepTheta * (float)k) * fUpAdaptValue; 
				SVIFloat cosTheta = cosf(SVI_PI - stepTheta * (float)k);

				SVIVector3 upVector = fDistUp > 0.01f? (up * sinTheta * fDistUp) : SVIVector3(0.0f,0.0f,0.0f);
				SVIVector3 frontVector = direction * (cosTheta * fDist * seperateRatio);
				
				
				SVIVector3 resultPosition = stepBase + frontVector + upVector;
				SVIVector3 adaptationPosition = SVIVector3(resultPosition.x - basePosition.x,resultPosition.y - basePosition.y,0.0f) * (resultPosition.z / targetHeight);
				if (!fakeDepth) adaptationPosition = SVIVector3(0.0f,0.0f,0.0f);
				pPivot->setPosition(resultPosition - adaptationPosition);							
			}		
			pLine->calculateActiveLength();
		}
	}


	SVIUInt SVICurveSimulator::getIndicesCount(){
		return (mGridSize-1) * (mGridSize-1) * 6;
	}

	SVIUInt SVICurveSimulator::getVerticesCount(){
		return (mGridSize-1) * (mGridSize-1) * 4;
	}


	void SVICurveSimulator::build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,SVIUShort * pIndices,
		SVIVector3 * pNormals,SVIVector2 * pTexcoords, SVIBool isVertical, 
		SVIBool isReverse, SVIBool bFliped, SVIBool isBackFaceImageFlip, SVIBool isBackFaceClear){


		SVIUInt indexVertex = 0;
		SVIUInt index = 0;
				
		for (SVIInt n = 0; n < mPivotCount; ++n){
			mPivots[n].setNormal(SVIVector3(0.0f,0.0f,0.0f));
		}

		for (SVIUInt y = 0; y < mGridSize-1; ++y){
			for (SVIUInt x = 0; x < mGridSize-1; ++x){
				SVIVector3& p0 = mPivots[y * mGridSize + x].getPosition(); 
				SVIVector3& p1 = mPivots[y * mGridSize + x + 1].getPosition();
				SVIVector3& p2 = mPivots[(y + 1) * mGridSize + x].getPosition();
				SVIVector3& p3 = mPivots[(y + 1) * mGridSize + (x + 1)].getPosition();

				SVIVector3& n0 = mPivots[y * mGridSize + x].getNormal(); 
				SVIVector3& n1 = mPivots[y * mGridSize + x + 1].getNormal();
				SVIVector3& n2 = mPivots[(y + 1) * mGridSize + x].getNormal();
				SVIVector3& n3 = mPivots[(y + 1) * mGridSize + (x + 1)].getNormal();


				SVIVector3 directA = SVIVector3(p1.x - p0.x,p1.y - p0.y,p1.z - p0.z);
				SVIVector3 directB = SVIVector3(p2.x - p0.x,p2.y - p0.y,p2.z - p0.z);

				if (isReverse){
					directA.x =  p0.x - p1.x;
					directB.x =  p0.x - p2.x;
				}
				
				SVIVector3 normal = directA.Cross(directB);
				if (!bFliped) normal = normal * -1.0f;
				n0 += normal;
				n1 += normal;
				n2 += normal;
				
				directA = SVIVector3(p1.x - p2.x,p1.y - p2.y,p1.z - p2.z);
				directB = SVIVector3(p3.x - p2.x,p3.y - p2.y,p3.z - p2.z);

				if (isReverse){
					directA.x =  p2.x - p1.x;
					directB.x =  p2.x - p3.x;
				}

				normal = directA.Cross(directB);
				if (!bFliped) normal = normal * -1.0f;
				n1 += normal;
				n2 += normal;
				n3 += normal;

			}
		}

		for (SVIInt n = 0; n < mPivotCount; ++n){
			mPivots[n].getNormal().normalize();			
		}		

		float tvR = 1.0f / (float)(mGridSize-1);
		float tLineCurrentU = 0.0f;
		float tLineNextU = 0.0f;

		//2012-02-09 masterkeaton27@gmail.com
		//let reverse action to page
		SVIVector3 filteredScale = scale;
		if (isReverse){
			if (isVertical) {
				filteredScale.y *= -1.0f;
			}else filteredScale.x *= -1.0f;
		}

		for (SVIUInt y = 0; y < mGridSize-1; ++y){
			if (!isBackFaceClear){
				tLineCurrentU = 1.0f / mLines[y].mActiveLength;
				tLineNextU = 1.0f / mLines[y+1].mActiveLength;
			}
			for (SVIUInt x = 0; x < mGridSize-1; ++x){


				SVIVector3& p0 = mPivots[y * mGridSize + x].getPosition(); 
				SVIVector3& p1 = mPivots[y * mGridSize + x + 1].getPosition();
				SVIVector3& p2 = mPivots[(y + 1) * mGridSize + x].getPosition();
				SVIVector3& p3 = mPivots[(y + 1) * mGridSize + (x + 1)].getPosition();

				SVIFloat v0u = mPivots[y * mGridSize + x].getMass(); 
				SVIFloat v1u = mPivots[y * mGridSize + x + 1].getMass();
				SVIFloat v2u = mPivots[(y + 1) * mGridSize + x].getMass();
				SVIFloat v3u = mPivots[(y + 1) * mGridSize + (x + 1)].getMass();


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

				if (isBackFaceClear){
					pTexcoords[indexVertex+0] = SVIVector2(0.1f,0.1f);				
					pTexcoords[indexVertex+1] = SVIVector2(0.1f,0.1f);				
					pTexcoords[indexVertex+2] = SVIVector2(0.1f,0.1f);					
					pTexcoords[indexVertex+3] = SVIVector2(0.1f,0.1f);
				}else if (isVertical){
					if (!isBackFaceImageFlip){			
						pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)y * tvR,(v0u*tLineCurrentU));				
						pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)y * tvR,(v1u*tLineCurrentU));				
						pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(y+1) * tvR,(v2u*tLineNextU));					
						pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(y+1) * tvR,(v3u*tLineNextU));
					}else{
						pTexcoords[indexVertex+0] = SVIVector2(1.0f - (SVIFloat)y * tvR,(v0u*tLineCurrentU));					
						pTexcoords[indexVertex+1] = SVIVector2(1.0f - (SVIFloat)y * tvR,(v1u*tLineCurrentU));					
						pTexcoords[indexVertex+2] = SVIVector2(1.0f - (SVIFloat)(y+1) * tvR,(v2u*tLineNextU));					
						pTexcoords[indexVertex+3] = SVIVector2(1.0f - (SVIFloat)(y+1) * tvR,(v3u*tLineNextU));
					}
				}else{
					if (!isBackFaceImageFlip){			
						pTexcoords[indexVertex+0] = SVIVector2((v0u*tLineCurrentU)   ,(SVIFloat)y * tvR);				
						pTexcoords[indexVertex+1] = SVIVector2((v1u*tLineCurrentU)	,(SVIFloat)y * tvR);				
						pTexcoords[indexVertex+2] = SVIVector2((v2u*tLineNextU)		,(SVIFloat)(y+1) * tvR);					
						pTexcoords[indexVertex+3] = SVIVector2((v3u*tLineNextU)		,(SVIFloat)(y+1) * tvR);
					}else{
						pTexcoords[indexVertex+0] = SVIVector2(1.0f - (v0u*tLineCurrentU)    ,(SVIFloat)y * tvR);					
						pTexcoords[indexVertex+1] = SVIVector2(1.0f - (v1u*tLineCurrentU)	,(SVIFloat)y * tvR);					
						pTexcoords[indexVertex+2] = SVIVector2(1.0f - (v2u*tLineNextU)		,(SVIFloat)(y+1) * tvR);					
						pTexcoords[indexVertex+3] = SVIVector2(1.0f - (v3u*tLineNextU)		,(SVIFloat)(y+1) * tvR);
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
