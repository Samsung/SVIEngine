#include "SVIPhysics.h"

#include "../Render/SVIDebugRenderer.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

#define GRID_DETAIL 40
		
	SVIExplodeMolcule::SVIExplodeMolcule():
	mInitPosition(0.0f,0.0f,0.0f),
	mSize(0.0f,0.0f,0.0f),
	mPosition(0.0f,0.0f,0.0f),
	mVelocity(0.0f,0.0f,0.0f),
	mInitVelocity(0.0f,0.0f,0.0f),
	mAngularVelocity(0.0f,0.0f,0.0f),
	mPower(0.0f),
	mNormal(0.0f,0.0f,1.0f),
	mRotation(0.0f,0.0f,1.0f)
	{

	}

	SVIExplodeMolcule::~SVIExplodeMolcule(){

	}

	
	void SVIExplodeMolcule::init(SVIFloat fPower, const SVIVector3 &size, const SVIVector3 &position, const SVIVector3 &velocity, const SVIVector3 &angularvelocity){
		mPower = fPower;
		mSize = size;
		mInitPosition = mPosition = position;
		mVelocity = mInitVelocity = velocity;
		mAngularVelocity = angularvelocity;
		mNormal = SVIVector3(0.0f,0.0f,1.0f);
		mRotation = SVIVector3(0.0f,0.0f,0.0f);
	}
	
	void SVIExplodeMolcule::setPower(SVIFloat fPower){
		mPower = fPower;
	}		
	
	void SVIExplodeMolcule::setPosition(const SVIVector3 &position){
		mInitPosition = mPosition = position;
	}
	
	void SVIExplodeMolcule::setVelocity(const SVIVector3 &velocity){
		mVelocity = velocity;
	}

	void SVIExplodeMolcule::setRotation(const SVIVector3 &rotation){
		mRotation = rotation;
	}

	void SVIExplodeMolcule::setAngularVelocity(const SVIVector3 &velocity){
		mAngularVelocity = velocity;
	}

	void SVIExplodeMolcule::setSize(SVIVector3 size){
		mSize = size;
	}

	void SVIExplodeMolcule::update(SVIFloat fElapsed){

		mPosition = mInitPosition + mVelocity * mPower * fElapsed;
		mRotation += mAngularVelocity * 0.01f;


		mNormal = SVIVector3(0.0f,0.0f,1.0f);
		SVIMatrix rotationMatrix;
		rotationMatrix.setYawPitchRoll(mRotation.x,mRotation.y,mRotation.z);
		mNormal = rotationMatrix.rotate(mNormal);
		mNormal.normalize();				
	}

	SVIVector3 SVIExplodeMolcule::getNormal(){
		return mNormal;
	}

	SVIVector3 SVIExplodeMolcule::getPosition(SVIInt index){
		SVIVector3 side = mNormal.Cross(mVelocity);
		side.normalize();

		//position order
		// 0-----1
		// |     |
		// |     |
		// 2-----3

		if (index == 0){
			return mPosition - (mNormal * 0.5f * mSize.x) - (side * 0.5f * mSize.y);
		}else if (index == 1){
			return mPosition + (mNormal * 0.5f * mSize.x) - (side * 0.5f * mSize.y);
		}else if (index == 2){
			return mPosition - (mNormal * 0.5f * mSize.x) + (side * 0.5f * mSize.y);
		}else if (index == 3){
			return mPosition + (mNormal * 0.5f * mSize.x) + (side * 0.5f * mSize.y);
		}

		return mPosition;
	}

	SVIVector3 SVIExplodeMolcule::getSize(){
		return mSize;
	}


	SVIExplodeSimulator::SVIExplodeSimulator() :
	mGridSize(GRID_DETAIL),
		mMolculeCount(GRID_DETAIL*GRID_DETAIL),
		mControlTarget(0.0f,0.5f,0.0f)
	{
		mMolcules = new SVIExplodeMolcule[mMolculeCount];
		reset();
	}

	SVIExplodeSimulator::~SVIExplodeSimulator(){
		SVI_SVIFE_DELETE_ARRAY(mMolcules);
	}


	void SVIExplodeSimulator::reset(){
		SVIFloat inverseRatio = 1.0f / (SVIFloat)(mGridSize);

		for (SVIUInt y = 0; y < mGridSize; ++y){
			for (SVIUInt x = 0; x < mGridSize; ++x){
				SVIExplodeMolcule * pMolecule = &mMolcules[y * mGridSize + x];
				pMolecule->setPosition(SVIVector3((SVIFloat)x*inverseRatio,(SVIFloat)y*inverseRatio,0));

				SVIFloat fYaw = SVI_DEGTORAD((SVIInt)rand()%360) - SVI_PI;
				SVIFloat fPitch = SVI_DEGTORAD((SVIInt)rand()%360) - SVI_PI;
				SVIFloat fRoll = SVI_DEGTORAD((SVIInt)rand()%360) - SVI_PI;
				pMolecule->setAngularVelocity(SVIVector3(fYaw,fPitch,fRoll));

				fYaw = SVI_DEGTORAD((SVIInt)rand()%360);
				fPitch = SVI_DEGTORAD((SVIInt)rand()%360);
				fRoll = SVI_DEGTORAD((SVIInt)rand()%360);
				pMolecule->setRotation(SVIVector3(fYaw,fPitch,fRoll));

				SVIFloat fVx = (SVIInt)rand()%100 - 50;
				SVIFloat fVy = (SVIInt)rand()%100 - 50;
				SVIFloat fVz = (SVIInt)rand()%100 - 50;

				SVIFloat fPower = (1.0f + ((SVIInt)rand()%100 - 50)) * 0.02f;

				pMolecule->setVelocity(SVIVector3(fVx,fVy,fVz).normal());
				pMolecule->setPower(fPower);
				pMolecule->setSize(SVIVector3(inverseRatio,inverseRatio,inverseRatio));
			}
		}
	}
	
	void SVIExplodeSimulator::setControlTarget(const SVIVector3& target){
		mControlTarget = target;
	}
		
	void SVIExplodeSimulator::updateTarget(){
						
	}

	void SVIExplodeSimulator::update(){
		updateTarget();
		for (SVIInt n = 0; n < mMolculeCount; ++n){
			SVIExplodeMolcule * pMolecule = &mMolcules[n];
			pMolecule->update(mControlTarget.x);			
		}
	}

	SVIUInt SVIExplodeSimulator::getIndicesCount(){
		return (GRID_DETAIL) * (GRID_DETAIL) * 6;
	}

	SVIUInt SVIExplodeSimulator::getVerticesCount(){
		return (GRID_DETAIL) * (GRID_DETAIL) * 4;
	}

	void SVIExplodeSimulator::build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,SVIUShort * pIndices,SVIVector3 * pNormals,SVIVector2 * pTexcoords,SVIBool bFliped){
		SVIUInt indexVertex = 0;
		SVIUInt index = 0;

		float tuR = 1.0f / (float)(mGridSize);
		float tvR = 1.0f / (float)(mGridSize);
		float tLineCurrentU = 0.0f;
		float tLineNextU = 0.0f;

		for (SVIUInt y = 0; y < mGridSize; ++y){
			for (SVIUInt x = 0; x < mGridSize; ++x){
				SVIExplodeMolcule * pMolecule = &mMolcules[y * mGridSize + x];

				SVIVector3 p0 = pMolecule->getPosition(0); 
				SVIVector3 p1 = pMolecule->getPosition(1);
				SVIVector3 p2 = pMolecule->getPosition(2);
				SVIVector3 p3 = pMolecule->getPosition(3);

				pPositions[indexVertex+0] = offset + p0 * scale;
				pPositions[indexVertex+1] = offset + p1 * scale;
				pPositions[indexVertex+2] = offset + p2 * scale;
				pPositions[indexVertex+3] = offset + p3 * scale;			

				SVIVector3 n = pMolecule->getNormal();

				pNormals[indexVertex+0] = bFliped ? n : -n;
				pNormals[indexVertex+1] = bFliped ? n : -n;
				pNormals[indexVertex+2] = bFliped ? n : -n;
				pNormals[indexVertex+3] = bFliped ? n : -n;

				if (!bFliped){			
					pTexcoords[indexVertex+0] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y) * tvR);				
					pTexcoords[indexVertex+1] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y) * tvR);				
					pTexcoords[indexVertex+2] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y)* tvR);					
					pTexcoords[indexVertex+3] = SVIVector2((SVIFloat)(x) * tuR,(SVIFloat)(y)* tvR);
				}else{
					pTexcoords[indexVertex+0] = SVIVector2(1.0f - (SVIFloat)(x) * tuR,(SVIFloat)(y) * tvR);						
					pTexcoords[indexVertex+1] = SVIVector2(1.0f - (SVIFloat)(x) * tuR,(SVIFloat)(y) * tvR);								
					pTexcoords[indexVertex+2] = SVIVector2(1.0f - (SVIFloat)(x) * tuR,(SVIFloat)(y)* tvR);							
					pTexcoords[indexVertex+3] = SVIVector2(1.0f - (SVIFloat)(x) * tuR,(SVIFloat)(y)* tvR);			
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
