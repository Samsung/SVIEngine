#include "SVIPhysics.h"

#include "../Render/SVIDebugRenderer.h"


// The below implementation is based on cone curl algorithm
// Mesh will be created with width of 1.0 and height of 0.8 units.
// The deformation of mesh will depend on 3 factors Axis, Angle and Offset.
// The above 3 parameters are set passed using deformation ratio
// and needs to updated to this class in update method.
//
// For every loop the 2d mesh coords will be wrapped as the axis, angle and offset
// and copied to the 3D coords output array
//
//
// For some reasons the image was inverted and hence Y and Z are getting multiplied
// by -1 in build()
//

# define DEFAULT_Z  0.0f

//2012-12-11 masterkeaton27@gmail.com
namespace SVI{

#define GRID_DETAIL 20
#define GRID_VERTICAL_COUNT 20
	
#define MESH_WIDTH 1.0f
#define MESH_HEIGHT 1.0f

	SVIConeCurveSimulator::SVIConeCurveSimulator() :
	mGridSize(GRID_DETAIL),
	mPivotCount(mGridSize*GRID_VERTICAL_COUNT),
	mControlTarget(0.0f,0.5f,0.0f),
	mControlBase(0.0f,0.5f,0.0f)
	{
		mPivots = new SVIPivot[mPivotCount];
		mLines = new SVISectorLine[mGridSize];

		mWidth = MESH_WIDTH;
		mHeight = MESH_HEIGHT;
		mRows = GRID_DETAIL - 1;			//Y_PARTICLES;
		mColumns = GRID_VERTICAL_COUNT - 1; //X_PARTICLES;
		mRho = 0; 							//rotation
		mTheta = 0; 						//Angle of
		mA.x = 0; 							//Distance
		mA.y = 0; 							//Distance
		xOffset = 0.0f;
		mNumOfVertices = (GRID_DETAIL) * (GRID_VERTICAL_COUNT);

		//Array to create a 2D mesh and store the position of 2D mesh
		mpInputVertex2D = new SVIVector2[sizeof(SVIVector2) * mNumOfVertices]; //2d description of page mesh
		if(mpInputVertex2D == NULL)
		return;

		SVIInt vertexCountY = GRID_VERTICAL_COUNT;
		SVIInt vertexCountX = GRID_DETAIL;
		SVIInt currentVertex= 0;

		for(SVIInt iY =0 ; iY < vertexCountY  ; iY++)
		{
			for(SVIInt iX= 0 ; iX < vertexCountX ; iX++)
			{
				SVIFloat px = (SVIFloat)iX * mWidth/mRows;
				SVIFloat py = (SVIFloat)((vertexCountY - 1)- iY)  * mHeight/mColumns;

				mpInputVertex2D[currentVertex].x = px;
				mpInputVertex2D[currentVertex].y = py;

				currentVertex++;
			}
		}
		reset();
	}

	SVIConeCurveSimulator::~SVIConeCurveSimulator(){
		SVI_SVIFE_DELETE_ARRAY(mPivots);
		SVI_SVIFE_DELETE_ARRAY(mLines);

		SVI_SVIFE_DELETE_ARRAY(mpInputVertex2D);
	}


	void SVIConeCurveSimulator::reset(){
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
	
	void SVIConeCurveSimulator::setControlTarget(const SVIVector3& target){
		mControlTarget = target;
	}
	
	
	void SVIConeCurveSimulator::updateTarget(){
		mControlBase.x = mControlTarget.x * SVI_2_PI;
		mControlBase.y = mControlTarget.y;								
	}

	void SVIConeCurveSimulator::deformMesh()
	{
		SVIVector2 p;
		SVIVector3 temp;
		SVIFloat R;
		SVIFloat r;
		SVIFloat beta;
		SVIFloat tempX;

		for(int count = 0 ; count < mNumOfVertices ; count++)
		{

			if(mpInputVertex2D[count].x >= xOffset)
			{
				p = mpInputVertex2D[count];
				tempX = p.x - xOffset;

				R = sqrt(pow(tempX -mA.x, 2)  + pow(p.y - mA.y, 2));
				r = R * sin(mTheta);
				beta = asin(tempX / R) / sin(mTheta);

				//wrap p around the imaginary cone
				temp.x = r * sin(beta);
				temp.y = R + mA.y - r *(1.0f - cos(beta)) * sin(mTheta);
				temp.z = r* (1.0f - cos(beta)) * cos(mTheta);

     			// to x we add offset
				// Z is calculated as follows
				// default position is added to the above calculation of z with cone curl
				// formula

				temp.x = temp.x + xOffset;
				temp.z += DEFAULT_Z;
			}
			else
			{
				temp.x = mpInputVertex2D[count].x;
				temp.y = mpInputVertex2D[count].y;
				temp.z = DEFAULT_Z;
			}
			mPivots[count].setPosition(temp);
		}
	}


	void SVIConeCurveSimulator::setAxis(SVIFloat Axis){
		mA.y = Axis;
	}

	void SVIConeCurveSimulator::setAngle(SVIFloat Angle){
		mTheta = Angle * RAD;
	}

	void SVIConeCurveSimulator::setxOffset(SVIFloat Offset){
		if(Offset < 0.0f)
			xOffset = 0.0f;
		else if(Offset > 1.0f)
			xOffset = 1.0f;
		else
			xOffset = Offset;
	}

	void SVIConeCurveSimulator::update(){
		deformMesh();

		updateTarget();

	}
	
	SVIUInt SVIConeCurveSimulator::getIndicesCount(){
		return (mGridSize-1) * (GRID_VERTICAL_COUNT-1) * 6;
	}

	SVIUInt SVIConeCurveSimulator::getVerticesCount(){
		return (mGridSize-1) * (GRID_VERTICAL_COUNT-1) * 4;
	}
	
	void SVIConeCurveSimulator::build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,
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

		//2012-12-19 venkat.vr
		//Y and Z coordinates are inverted so as we were getting an inverted image
		//we are seeing the back face of the image and hence inverting the Y and Z coords

		if(1){
			filteredScale.y *=-1.0f;
			filteredScale.z *=-1.0f;
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
