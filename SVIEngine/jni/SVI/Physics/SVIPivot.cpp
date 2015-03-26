#include "SVIPhysics.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{


	SVIPivot::SVIPivot():
    mPosition(0.0f,0.0f,0.0f),
	mVelocity(0.0f,0.0f,0.0f),
	mNormal(0.0f,0.0f,0.0f),
	mMass(0.0f),
	mFixed(SVIFALSE)
{}

SVIPivot::SVIPivot(const SVIVector3& position, const SVIVector3& velocity, const SVIVector3& normal, SVIFloat mass, SVIBool bFixed):
mPosition(position),
	mVelocity(velocity),
	mNormal(normal),
	mMass(mass),
	mFixed(bFixed)
{}		

SVIPivot::~SVIPivot(){}




};
