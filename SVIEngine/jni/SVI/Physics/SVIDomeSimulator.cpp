#include "SVIPhysics.h"

#include "../Render/SVIDebugRenderer.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

#define DELTA_THETA 10.0f
#define DELTA_PHI 10.0f
#define SQR(x) x*x
#define DTOR SVI_PI/180.0f
#define CALC_UVS(x, y, z, u, v) \
    vx = x; \
    vy = y; \
    vz = z; \
    mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz)); \
    vx /= mag;  \
    vy /= mag;  \
    vz /= mag;  \
    u = (float)(atan2(vx, vz)/(SVI_PI * 2)) + 0.5f;    \
    v = (float)(asinf(vy) / SVI_PI) + 0.5f;

    SVIDomeSimulator::SVIDomeSimulator() :
	mControlTarget(0.0f,0.5f,0.0f),
	mControlBase(0.0f,0.5f,0.0f),
	mDomeResolution(0)
	{
        mPivotCount = (int)((360.0f / DELTA_THETA) * ((int)(90.0f / DELTA_PHI)+1));
		reset();
	}

	SVIDomeSimulator::~SVIDomeSimulator(){
	    for (SVIUInt n = 0; n < mPivots.size(); n++)
            SVI_SVIFE_DELETE(mPivots[n]);

        std::vector<SVIPivot*> temp;
        mPivots.swap(temp);
        temp.clear();
    }

    float x(float phi, float theta) {
        return cos(phi*DTOR)*cos(theta*DTOR) + 1.0f;
    };

    float z(float phi, float theta) {
        return sin(phi*DTOR);
    };

    float y(float phi, float theta) {
        return cos(phi*DTOR)*sin(theta*DTOR) + 1.0f;
    };

    void SVIDomeSimulator::reset(){
        /*SVIFloat inverseRatio = 1.0f / (SVIFloat)(mGridSize-1);
        SVIFloat inverseVerticalRatio = 1.0f / (SVIFloat)(mGridSize-1);

        for (SVIInt y = 0; y < mGridSize; ++y){
        for (SVIInt x = 0; x < mGridSize; ++x){
        SVIPivot * pPivot = &mPivots[y * mGridSize + x];
        pPivot->setPosition(SVIVector3((SVIFloat)x*inverseRatio,(SVIFloat)y*inverseVerticalRatio,0));
        }
        }*/       

        for (SVIUInt n = 0; n < mPivots.size(); n++)
            SVI_SVIFE_DELETE(mPivots[n]);
        std::vector<SVIPivot*> temp;
        mPivots.swap(temp);
        temp.clear();
        mPivots.clear();

        float radius = 0.5f;
        float vertices[12];
        float uvs[8];
        float phi0;
        float theta0;
        float vx, vy, vz, mag;

        SVIUInt vertexIdx = 0;
        for (float phi = 0.0f; phi <= 90; phi += DELTA_PHI) {
             phi0 = phi + DELTA_PHI;

            for (float theta = 0.0f; theta < 360; theta += DELTA_THETA) {
                theta0 = theta + DELTA_THETA;                              

                //point 0
                vertices[0] = x(phi, theta);
                vertices[1] = y(phi, theta);
                vertices[2] = z(phi, theta);

                uvs[0] = vertices[0];
                uvs[1] = vertices[1];
                //CALC_UVS(vertices[0], vertices[1], vertices[2], uvs[0], uvs[1]);

                //point 1
                vertices[3] = x(phi0, theta);
                vertices[4] = y(phi0, theta);
                vertices[5] = z(phi0, theta);

                uvs[2] = vertices[3];
                uvs[3] = vertices[4];
                //CALC_UVS(vertices[3], vertices[4], vertices[5], uvs[2], uvs[3]);

                //point 2
                vertices[6] = x(phi0, theta0);
                vertices[7] = y(phi0, theta0);
                vertices[8] = z(phi0, theta0);

                uvs[4] = vertices[6];
                uvs[5] = vertices[7];
                //CALC_UVS(vertices[6], vertices[7], vertices[8], uvs[4], uvs[5]);

                //point 3
                vertices[9] = x(phi, theta0);
                vertices[10] = y(phi, theta0);
                vertices[11] = z(phi, theta0);

                uvs[6] = vertices[9];
                uvs[7] = vertices[10];
                //CALC_UVS(vertices[9], vertices[10], vertices[11], uvs[6], uvs[7]);

                SVIPivot* pPivotA = new SVIPivot();
                pPivotA->setPosition(SVIVector3(vertices[0],vertices[1],vertices[2]));
                pPivotA->setVelocity(SVIVector3(uvs[0],uvs[1],0.0f));

                SVIPivot* pPivotB = new SVIPivot();
                pPivotB->setPosition(SVIVector3(vertices[3],vertices[4],vertices[5]));
                pPivotB->setVelocity(SVIVector3(uvs[2],uvs[3],0.0f));

                SVIPivot* pPivotC = new SVIPivot();
                pPivotC->setPosition(SVIVector3(vertices[6],vertices[7],vertices[8]));
                pPivotC->setVelocity(SVIVector3(uvs[4],uvs[5],0.0f));

                SVIPivot* pPivotD = new SVIPivot();
                pPivotD->setPosition(SVIVector3(vertices[9],vertices[10],vertices[11]));
                pPivotD->setVelocity(SVIVector3(uvs[6],uvs[7],0.0f));

                mPivots.push_back(pPivotA);
                mPivots.push_back(pPivotB);
                mPivots.push_back(pPivotC);
                mPivots.push_back(pPivotD);

                mIndices.push_back(vertexIdx+0);
                mIndices.push_back(vertexIdx+1);
                mIndices.push_back(vertexIdx+2);

                mIndices.push_back(vertexIdx+0);
                mIndices.push_back(vertexIdx+2);
                mIndices.push_back(vertexIdx+3);

                vertexIdx += 4;
                
            }
        }
    }

    void SVIDomeSimulator::setControlTarget(const SVIVector3& target){
		mControlTarget = target;
	}
	
	
	void SVIDomeSimulator::updateTarget(){}


	void SVIDomeSimulator::update(){
		updateTarget();


       /* float centerX = 0.5f;
        float centerY = 0.5f;

        float heightScale = mControlTarget.x;
        float radius = mControlTarget.y;

        for (SVIInt y = 0; y < mGridSize-1; ++y){
            float yUnitCoord = (float)y / ((float)mGridSize-1);
            float thetaValueY = SVI_PI * yUnitCoord;
            for (SVIInt x = 0; x < mGridSize-1; ++x){
                float xUnitCoord = (float)x / ((float)mGridSize-1);

                float fDistSqr = (xUnitCoord - centerX) * (xUnitCoord - centerX) + (yUnitCoord - centerY) * (yUnitCoord - centerY);
                float fDistRatio = sqrt(fDistSqr) / radius;
                float fEffectiveness = fDistRatio > 1.0f ? 1.0f : fDistRatio; 

                float thetaValue = SVI_PI*0.5f + SVI_PI * 0.5f * fEffectiveness;
                float zFactor = sin(thetaValue) * heightScale;                


                SVIPivot * pPivot = &mPivots[y * mGridSize + x];
                SVIVector3& position = pPivot->getPosition();
                position.z = zFactor;
            }
        }*/
    }


    SVIUInt SVIDomeSimulator::getIndicesCount(){
        return (mPivotCount) * 2 * 3;
    }

    SVIUInt SVIDomeSimulator::getVerticesCount(){
        return mPivotCount * 4;
    }
	
	void SVIDomeSimulator::build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions, SVIUShort * pIndices,
		SVIVector3 * pNormals,SVIVector2 * pTexcoords,SVIBool isVertical, SVIBool isReverse, SVIBool bFliped){
            


        float heightScale = mControlTarget.x;
        float radius = mControlTarget.y;
        float radiusInner = mControlTarget.z;

        size_t size = mPivots.size();
        for (size_t n = 0; n < size; n++){
            SVIVector3 position = mPivots[n]->getPosition();

            bool isFlat = false;
            if (position.z > radiusInner) {
                position.z = radiusInner;
                isFlat = true;
            }

            SVIVector3 normal = SVIVector3(position.x * radius, position.y * radius, position.z * radius * heightScale);
            SVIVector3 texcoord = mPivots[n]->getVelocity();

            pPositions[n] = offset + (normal * scale);

            if (heightScale > 0) pNormals[n] = normal.normal(); else pNormals[n] = -normal.normal();
            
            if (isFlat) pNormals[n] = SVIVector3(0.0f, 0.0f, 1.0);

            pTexcoords[n] = SVIVector2(texcoord.x * radius, texcoord.y * radius);
        }


        for (size_t n = 0; n < mIndices.size(); n++){
            pIndices[n] = mIndices[n];
        }
                

		//SVIUInt indexVertex = 0;
		//SVIUInt index = 0;
		//		
		//for (SVIInt n = 0; n < mPivotCount; ++n){
		//	mPivots[n].setNormal(SVIVector3(0.0f,0.0f,0.0f));
		//}

		//for (SVIInt y = 0; y < mGridSize-1; ++y){
		//	for (SVIInt x = 0; x < mGridSize-1; ++x){
		//		SVIVector3& p0 = mPivots[y * mGridSize + x].getPosition(); 
		//		SVIVector3& p1 = mPivots[y * mGridSize + x + 1].getPosition();
		//		SVIVector3& p2 = mPivots[(y + 1) * mGridSize + x].getPosition();
		//		SVIVector3& p3 = mPivots[(y + 1) * mGridSize + (x + 1)].getPosition();
		//		
		//		SVIVector3& n0 = mPivots[y * mGridSize + x].getNormal(); 
		//		SVIVector3& n1 = mPivots[y * mGridSize + x + 1].getNormal();
		//		SVIVector3& n2 = mPivots[(y + 1) * mGridSize + x].getNormal();
		//		SVIVector3& n3 = mPivots[(y + 1) * mGridSize + (x + 1)].getNormal();

		//		SVIVector3 normal = (p1 - p0).Cross(p2 - p0);
		//		if (!bFliped) normal = normal * -1.0f;
		//		n0 += normal;
		//		n1 += normal;
		//		n2 += normal;

		//		normal = (p1 - p2).Cross(p3 - p2);
		//		if (!bFliped) normal = normal * -1.0f;
		//		n1 += normal;
		//		n2 += normal;
		//		n3 += normal;
		//	}
		//}
	
		//for (SVIInt n = 0; n < mPivotCount; ++n){
		//	mPivots[n].getNormal().normalize();
		//}
		//	
		////2012-02-09 masterkeaton27@gmail.com
		////let reverse action to page
		//SVIVector3 filteredScale = scale;
		//if (isReverse){
		//	if (isVertical) {
		//		filteredScale.y *= -1.0f;
		//	}else filteredScale.x *= -1.0f;
		//}

		//float tuR = 1.0f / (float)(mGridSize-1);
		//float tvR = 1.0f / (float)(mGridSize-1);
		//for (SVIInt y = 0; y < mGridSize-1; ++y){
		//	for (SVIInt x = 0; x < mGridSize-1; ++x){
		//		SVIVector3& p0 = mPivots[y * mGridSize + x].getPosition(); 
		//		SVIVector3& p1 = mPivots[y * mGridSize + x + 1].getPosition();
		//		SVIVector3& p2 = mPivots[(y + 1) * mGridSize + x].getPosition();
		//		SVIVector3& p3 = mPivots[(y + 1) * mGridSize + (x + 1)].getPosition();
		//		

		//		if (isVertical){
		//			SVIVector3 pT0 = SVIVector3(p0.y,p0.x,p0.z);
		//			SVIVector3 pT1 = SVIVector3(p1.y,p1.x,p1.z);
		//			SVIVector3 pT2 = SVIVector3(p2.y,p2.x,p2.z);
		//			SVIVector3 pT3 = SVIVector3(p3.y,p3.x,p3.z);

		//			pPositions[indexVertex+0] = offset + pT0 * filteredScale;
		//			pPositions[indexVertex+1] = offset + pT1 * filteredScale;
		//			pPositions[indexVertex+2] = offset + pT2 * filteredScale;
		//			pPositions[indexVertex+3] = offset + pT3 * filteredScale;			
		//		}else{
		//			pPositions[indexVertex+0] = offset + p0 * filteredScale;
		//			pPositions[indexVertex+1] = offset + p1 * filteredScale;
		//			pPositions[indexVertex+2] = offset + p2 * filteredScale;
		//			pPositions[indexVertex+3] = offset + p3 * filteredScale;			
		//		}		

		//		pNormals[indexVertex+0] = mPivots[y * mGridSize + x].getNormal();
		//		pNormals[indexVertex+1] = mPivots[y * mGridSize + (x + 1)].getNormal();
		//		pNormals[indexVertex+2] = mPivots[(y + 1) * mGridSize + x].getNormal();
		//		pNormals[indexVertex+3] = mPivots[(y + 1) * mGridSize + (x + 1)].getNormal();

		//		if (isVertical){
		//			if (!bFliped){			
		//				pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)(y) * tvR,(SVIFloat)(x) * tuR);				
		//				pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)(y) * tvR,(SVIFloat)(x+1) * tuR);				
		//				pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(y+1)* tvR,(SVIFloat)(x) * tuR);					
		//				pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(y+1)* tvR,(SVIFloat)(x+1) * tuR);
		//			}else{
		//				pTexcoords[indexVertex+0] = SVIVector2(1.0f - (SVIFloat)(y) * tvR,(SVIFloat)(x) * tuR);						
		//				pTexcoords[indexVertex+1] = SVIVector2(1.0f - (SVIFloat)(y) * tvR,(SVIFloat)(x+1) * tuR);								
		//				pTexcoords[indexVertex+2] = SVIVector2(1.0f - (SVIFloat)(y+1)* tvR,(SVIFloat)(x) * tuR);							
		//				pTexcoords[indexVertex+3] = SVIVector2(1.0f - (SVIFloat)(y+1)* tvR,(SVIFloat)(x+1) * tuR);			
		//			}
		//		}else{
		//			if (!bFliped){			
		//				pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y) * tvR);				
		//				pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)(x+1) * tuR,(SVIFloat)(y) * tvR);				
		//				pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y+1)* tvR);					
		//				pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(x+1) * tuR,(SVIFloat)(y+1)* tvR);
		//			}else{
		//				pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)(x) * tuR,1.0f - (SVIFloat)(y) * tvR);						
		//				pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)(x+1) * tuR,1.0f - (SVIFloat)(y) * tvR);								
		//				pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(x) * tuR,1.0f - (SVIFloat)(y+1)* tvR);							
		//				pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(x+1) * tuR,1.0f - (SVIFloat)(y+1)* tvR);			
		//			}
		//		}

		//		if (bFliped){
		//			pIndices[index + 0] = indexVertex+0;
		//			pIndices[index + 1] = indexVertex+1;
		//			pIndices[index + 2] = indexVertex+2;

		//			pIndices[index + 3] = indexVertex+2;
		//			pIndices[index + 4] = indexVertex+1;
		//			pIndices[index + 5] = indexVertex+3;
		//		}else{
		//			pIndices[index + 0] = indexVertex+2;
		//			pIndices[index + 1] = indexVertex+1;
		//			pIndices[index + 2] = indexVertex+0;

		//			pIndices[index + 3] = indexVertex+3;
		//			pIndices[index + 4] = indexVertex+1;
		//			pIndices[index + 5] = indexVertex+2;
		//		}
		//		indexVertex += 4;
		//		index += 6;
		//	}
		//}		
	}



};
