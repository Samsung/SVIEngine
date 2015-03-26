#include "SVIPhysics.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

	SVISpring::SVISpring() : 
	mPivotA(-1), 
	mPivotB(-1),
	mTension(0.0f),
	mConstant(0.0f),
	mNaturalLength(1.0f)
	{		
	}
	
	SVISpring::SVISpring(SVIInt pivotA, SVIInt pivotB) :
	mPivotA(pivotA), 
	mPivotB(pivotB),
	mTension(0.0f),
	mConstant(0.0f),
	mNaturalLength(1.0f)
	{
	}

	SVISpring::SVISpring(SVIInt pivotA, SVIInt pivotB, SVIFloat tension, SVIFloat constant, SVIFloat length):
	mPivotA(pivotA), 
	mPivotB(pivotB),
	mTension(tension),
	mConstant(constant),
	mNaturalLength(length)
	{
	}

	SVISpring::~SVISpring(){}

};
