#include "SVIMath.h"

//2011-11-04 masterkeaton27@gmail.comAABB 

namespace SVI{

	SVIBoundingBox::SVIBoundingBox(void) : mMin(10000.0f,10000.0f,10000.0f), mMax(-10000.0f,-10000.0f,-10000.0f){}
	SVIBoundingBox::SVIBoundingBox(const SVIVector3 &min, const SVIVector3 &max)  : mMin(min), mMax(max) {}

	SVIBoundingBox::SVIBoundingBox(const SVIVector3 * pPoints, SVIUInt count){
		for(SVIUInt i=0; i<count; ++i) extend(pPoints[i]);
	}
	SVIBoundingBox::SVIBoundingBox(const SVIBoundingBox * pBoxes, SVIUInt count){
		for(SVIUInt i=0; i<count; ++i) {
			extend(pBoxes[i].mMax);
			extend(pBoxes[i].mMin);
		}
	}
	
	void SVIBoundingBox::extend(const SVIVector3 &point){
		mMin.x = SVIMIN(mMin.x, point.x);
		mMin.y = SVIMIN(mMin.y, point.y);
		mMin.z = SVIMIN(mMin.z, point.z);

		mMax.x = SVIMAX(mMax.x, point.x);
		mMax.y = SVIMAX(mMax.y, point.y);
		mMax.z = SVIMAX(mMax.z, point.z);
	}


	SVIBool SVIBoundingBox::isValid(){
		return ((mMin.x < mMax.x) && (mMin.y < mMax.y) && (mMin.z < mMax.z));
	}

	
	SVIVector3 SVIBoundingBox::getSize(){return (mMax - mMin);}
	SVIVector3 SVIBoundingBox::getCenter(){return ((mMax + mMin) / 2.0f); }
	SVIVector3 SVIBoundingBox::point(SVIInt i){
		return SVIVector3((i&1) ? mMin.x : mMax.x, (i&2) ? mMin.y : mMax.y, (i&4) ? mMin.z : mMax.z); 
	}
};