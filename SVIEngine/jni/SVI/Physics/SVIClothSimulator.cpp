#include "SVIPhysics.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

	
	SVIClothSimulator::SVIClothSimulator() :
	mPivotsA(NULL),
	mPivotsB(NULL),
	mCurrentPivots(NULL),
	mNextPivots(NULL),
	mSprings(NULL),
	mGravity(0.0f,0.0f,-0.98f),
	mGridSize(13),
	mPivotCount(13*13),
	mSpringConstant(50.0f),
	mSpringNatualLength(1.0f),
	mPivotMass(0.05f),
	mUpdateStep(0.0f),
	mDamping(0.92f)
	{
		mSpringCount = (mGridSize-1) * mGridSize * 2;
		mSpringCount += (mGridSize-1) * (mGridSize-1) * 2;
		mSpringCount += (mGridSize-2) * mGridSize * 2;

		mPivotsA = new SVIPivot[mPivotCount];
		mPivotsB = new SVIPivot[mPivotCount];
		mSprings = new SVISpring[mSpringCount];

		reset();


	}

	SVIClothSimulator::~SVIClothSimulator(){
		SVI_SVIFE_DELETE_ARRAY(mSprings);
		SVI_SVIFE_DELETE_ARRAY(mPivotsA);
		SVI_SVIFE_DELETE_ARRAY(mPivotsB);
	}

	void SVIClothSimulator::reset(){
		SVIFloat reverseRatio = 1.0f;
		for (SVIUInt y= 0; y < mGridSize; ++y){
			for (SVIUInt x= 0; x < mGridSize; ++x){
				mPivotsA[y*mGridSize+x].setPosition(SVIVector3(SVIFloat(x),SVIFloat(y), -10.00f ));
				mPivotsA[y*mGridSize+x].setMass(mPivotMass);
				mPivotsA[y*mGridSize+x].setFixed(SVIFALSE);
			}	
		}	

		

		//2011-11-11 masterkeaton27@gmail.com
		//should set fixed pivots here
		for (SVIUInt y= 0; y < mGridSize; ++y){
			for (SVIUInt x= 0; x < mGridSize; ++x){
				if (x == (SVIUInt)(mGridSize-1)){
					mPivotsA[y*mGridSize+x].setFixed(SVITRUE);
				}
			}	
		}	

		for (int n = 0; n < mPivotCount; ++n) mPivotsB[n] = mPivotsA[n];

		mCurrentPivots = mPivotsA;
		mNextPivots = mPivotsB;

		//1. link pivots to the next
		SVISpring * pCurrentSpring = &mSprings[0];
		for (SVIUInt y= 0; y < mGridSize; ++y){
			for (SVIUInt x= 0; x < mGridSize-1; ++x){
				pCurrentSpring->setPivotA(y*mGridSize+x);
				pCurrentSpring->setPivotB(y*mGridSize+x+1);
				pCurrentSpring->setConstant(mSpringConstant);
				pCurrentSpring->setLength(mSpringNatualLength);
				++pCurrentSpring;
			}
		}

		//2. link pivots to the one below
		for (SVIUInt y= 0; y < mGridSize-1; ++y){
			for (SVIUInt x= 0; x < mGridSize; ++x){
				pCurrentSpring->setPivotA(y*mGridSize+x);
				pCurrentSpring->setPivotB((y+1)*mGridSize+x);
				pCurrentSpring->setConstant(mSpringConstant);
				pCurrentSpring->setLength(mSpringNatualLength);
				++pCurrentSpring;
			}
		}


		SVIFloat fSqrtValue = sqrtf(2.0f);
		
		//3. link pivots to the one below and right, excluding those on the bottom or right
		for (SVIUInt y= 0; y < mGridSize-1; ++y){
			for (SVIUInt x= 0; x < mGridSize-1; ++x){
				pCurrentSpring->setPivotA(y*mGridSize+x);
				pCurrentSpring->setPivotB((y+1)*mGridSize+(x+1));
				pCurrentSpring->setConstant(mSpringConstant);
				pCurrentSpring->setLength(mSpringNatualLength*fSqrtValue);
				++pCurrentSpring;
			}
		}

		//4. link pivots to the one below and left, excluding those on the  bottom or right
		for (SVIUInt y= 0; y < mGridSize-1; ++y){
			for (SVIUInt x = 1; x < mGridSize; ++x){
				pCurrentSpring->setPivotA(y*mGridSize+x);
				pCurrentSpring->setPivotB((y+1)*mGridSize+(x-1));
				pCurrentSpring->setConstant(mSpringConstant);
				pCurrentSpring->setLength(mSpringNatualLength*fSqrtValue);
				++pCurrentSpring;
			}
		}
		
		//5. link pivots to the one below and next to the right hand edge
		for (SVIUInt y= 0; y < mGridSize; ++y){
			for (SVIUInt x= 0; x < mGridSize-2; ++x){
				pCurrentSpring->setPivotA(y*mGridSize+x);
				pCurrentSpring->setPivotB(y*mGridSize+x+2);
				pCurrentSpring->setConstant(mSpringConstant);
				pCurrentSpring->setLength(mSpringNatualLength*2.0f);
				++pCurrentSpring;
			}
		}

		//6. link pivots to the one below and next to the bottom hand edge
		for (SVIUInt y= 0; y < mGridSize-2; ++y){
			for (SVIUInt x= 0; x < mGridSize; ++x){
				pCurrentSpring->setPivotA(y*mGridSize+x);
				pCurrentSpring->setPivotB((y+2)*mGridSize+x);
				pCurrentSpring->setConstant(mSpringConstant);
				pCurrentSpring->setLength(mSpringNatualLength*2.0f);
				++pCurrentSpring;
			}
		}

		//need to build with first update.
		mUpdated = SVITRUE;
	}

	void SVIClothSimulator::update(){
		SVIBool isUpdated = SVIFALSE;

		const SVIFloat floorHeight = -10.0f;
		const SVIFloat limitDelta = 0.1f;
		SVIFloat stepDelta = 0.01f;
		mUpdateStep += stepDelta;
		
		if (mUpdateStep > limitDelta){
			isUpdated = SVITRUE;
			mUpdateStep = 0.0f;

			//calculate the tensions in the springs
			for (SVIInt i = 0; i < mSpringCount; ++i){
				SVIInt pivotA = mSprings[i].getPivotA(); 
				SVIInt pivotB = mSprings[i].getPivotB();
				SVIVector3 positionDelta = mCurrentPivots[pivotA].getPosition() - mCurrentPivots[pivotB].getPosition();
				SVIFloat springLength = positionDelta.getlength();
				SVIFloat extension = springLength - mSprings[i].getLength();
				SVIFloat tension = mSprings[i].getConstant() * extension / mSprings[i].getLength();
				mSprings[i].setTension(tension);
			}

			//calculate the next pivots from the current pivots
			for (SVIInt n = 0; n < mPivotCount; ++n){
				//transfer properties which do not change
				mNextPivots[n].setFixed(mCurrentPivots[n].isFixed());
				mNextPivots[n].setMass(mCurrentPivots[n].getMass());

				//if the pivot is fixed, transfer the position and zero the velocity, otherwise calculate the new values.
				if (mCurrentPivots[n].isFixed()){
					mNextPivots[n].setPosition( mCurrentPivots[n].getPosition() );
					mNextPivots[n].setVelocity( SVIVector3(0.0f,0.0f,0.0f) );
				}else{
					SVIVector3 & force = mGravity;

					for (SVIInt k = 0; k < mSpringCount; ++k){
						SVIInt pivotA = mSprings[k].getPivotA();
						SVIInt pivotB = mSprings[k].getPivotB();

						if (mSprings[k].getPivotA() == n){
							SVIVector3 tensionDirection = mCurrentPivots[pivotB].getPosition() - mCurrentPivots[n].getPosition();
							tensionDirection.normalize();
							force += tensionDirection * mSprings[k].getTension();
						}

						if (mSprings[k].getPivotB() == n){
							SVIVector3 tensionDirection = mCurrentPivots[pivotA].getPosition() - mCurrentPivots[n].getPosition();
							tensionDirection.normalize();
							force += tensionDirection * mSprings[k].getTension();
						}
					}

					SVIVector3 acceleration = force / mCurrentPivots[n].getMass();

					SVIVector3 velocity = mCurrentPivots[n].getVelocity() + acceleration * stepDelta;
					velocity *= mDamping;

					mNextPivots[n].setVelocity(velocity);

					SVIVector3 position = mCurrentPivots[n].getPosition() + (velocity + mCurrentPivots[n].getVelocity()) * stepDelta * 0.5f;


					if (position.z < floorHeight) position.z = floorHeight;

					mNextPivots[n].setPosition(position);
				}
			}

			SVIPivot * pTempPivots = mCurrentPivots;
			mCurrentPivots = mNextPivots;
			mNextPivots = pTempPivots;
		}

		if (isUpdated){
			mUpdated = SVITRUE;
			for (SVIInt n = 0; n < mPivotCount; ++n){
				mCurrentPivots[n].setNormal(SVIVector3(0.0f,0.0f,0.0f));
			}

			for (SVIUInt y = 0; y < mGridSize-1; ++y){
				for (SVIUInt x = 0; x < mGridSize-1; ++x){
					SVIVector3& p0 = mCurrentPivots[y * mGridSize + x].getPosition(); 
					SVIVector3& p1 = mCurrentPivots[y * mGridSize + x + 1].getPosition();
					SVIVector3& p2 = mCurrentPivots[(y + 1) * mGridSize + x].getPosition();
					SVIVector3& p3 = mCurrentPivots[(y + 1) * mGridSize + (x + 1)].getPosition();

					SVIVector3& n0 = mCurrentPivots[y * mGridSize + x].getNormal(); 
					SVIVector3& n1 = mCurrentPivots[y * mGridSize + x + 1].getNormal();
					SVIVector3& n2 = mCurrentPivots[(y + 1) * mGridSize + x].getNormal();
					SVIVector3& n3 = mCurrentPivots[(y + 1) * mGridSize + (x + 1)].getNormal();

					SVIVector3 normal = (p1 - p0).Cross(p2 - p0);
					n0 += normal;
					n1 += normal;
					n2 += normal;

					normal = (p1 - p2).Cross(p3 - p2);
					n1 += normal;
					n2 += normal;
					n3 += normal;
				}
			}

			for (SVIInt n = 0; n < mPivotCount; ++n){
				mCurrentPivots[n].getNormal().normalize();
			}
		}		
	}


	SVIUInt SVIClothSimulator::getIndicesCount(){
		return (mGridSize-1) * (mGridSize-1) * 6;
	}

	SVIUInt SVIClothSimulator::getVerticesCount(){
		return (mGridSize-1) * (mGridSize-1) * 4;
	}


	void SVIClothSimulator::build(SVIVector3 * pPositions,SVIUShort * pIndices,SVIVector3 * pNormals, SVIFloat fScaleX,SVIFloat fScaleY){
		if (!mUpdated) return;

		SVIUInt indexVertex = 0;
		SVIUInt index = 0;
		SVIFloat reverseRatio = 1.0f/(SVIFloat)mGridSize;
		SVIVector3 scaleVector(reverseRatio * fScaleX * 0.25,reverseRatio * fScaleY * 0.25,reverseRatio * fScaleY * 0.25);
		SVIVector3 offsetVector(0.0f,0.0f,0.0f);


		for (SVIUInt y = 0; y < mGridSize-1; ++y){
			for (SVIUInt x = 0; x < mGridSize-1; ++x){
				pPositions[indexVertex+0] = offsetVector + mCurrentPivots[y * mGridSize + x].getPosition() * scaleVector;
				pPositions[indexVertex+1] = offsetVector + mCurrentPivots[y * mGridSize + (x + 1)].getPosition() * scaleVector;
				pPositions[indexVertex+2] = offsetVector + mCurrentPivots[(y + 1) * mGridSize + x].getPosition() * scaleVector;
				pPositions[indexVertex+3] = offsetVector + mCurrentPivots[(y + 1) * mGridSize + (x + 1)].getPosition() * scaleVector;			

				pNormals[indexVertex+0] = mCurrentPivots[y * mGridSize + x].getNormal();
				pNormals[indexVertex+1] = mCurrentPivots[y * mGridSize + (x + 1)].getNormal();
				pNormals[indexVertex+2] = mCurrentPivots[(y + 1) * mGridSize + x].getNormal();
				pNormals[indexVertex+3] = mCurrentPivots[(y + 1) * mGridSize + (x + 1)].getNormal();

				pIndices[index + 0] = indexVertex+0;
				pIndices[index + 1] = indexVertex+1;
				pIndices[index + 2] = indexVertex+2;

				pIndices[index + 3] = indexVertex+2;
				pIndices[index + 4] = indexVertex+1;
				pIndices[index + 5] = indexVertex+3;

				indexVertex += 4;
				index += 6;
			}
		}

		mUpdated = SVIFALSE;
	}



};
