#pragma once


namespace SVI{


	class SVISpring;
	class SVIPivot;
	struct SVIVector3; 
	//2011-11-11 masterkeaton27@gmail.com
	//Cloth simulator
	class SVIClothSimulator{
	public:
		SVIClothSimulator();
		virtual ~SVIClothSimulator();
	private:
		SVIClothSimulator(const SVIClothSimulator& rhs){}
		void operator=(const SVIClothSimulator& rhs){}
	public:
		void reset();
		void update();
		void build(SVIVector3 * pPositions,SVIUShort * pIndices,SVIVector3 * pNormals,SVIFloat fScaleX,SVIFloat fScaleY);

	public:
		SVIUInt getIndicesCount();
		SVIUInt getVerticesCount();

	private:
		SVIVector3 mGravity;
		SVIFloat mSpringConstant;
		SVIFloat mSpringNatualLength;
		SVIFloat mPivotMass;
		SVIFloat mDamping;

	private:
		SVIFloat mUpdateStep;
		SVIBool mUpdated;

	private:
		SVIUInt mGridSize;
		SVIInt mPivotCount;	
		SVIInt mSpringCount;

		SVIPivot * mPivotsA;
		SVIPivot * mPivotsB;

		SVIPivot * mCurrentPivots;
		SVIPivot * mNextPivots;

		SVISpring * mSprings;
	};
}