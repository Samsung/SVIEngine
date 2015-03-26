#include "SVIPhysics.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{


	SVIRod::SVIRod(SVIInt pivotA, SVIInt pivotB, SVIInt pivotC, SVIFloat Arc, SVIInt type):
	mPivotA(pivotA), 
	mPivotB(pivotB),
	mPivotC(pivotC),
	fArc(Arc),
	mRodType(type)
	{
	}

	SVIRod::~SVIRod(){}

};
