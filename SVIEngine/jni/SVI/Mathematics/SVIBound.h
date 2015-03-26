#pragma once

namespace SVI{
	
struct SVIVector2;
struct SVIVector3;
struct SVIVector4;
struct SVIMatrix;
struct SVIQuaternion;

struct SVIBoundingBox
{
public:
	SVIVector3 mMin;
	SVIVector3 mMax;


public:
	SVIBoundingBox(void);
	SVIBoundingBox(const SVIVector3 &min, const SVIVector3 &max);

	SVIBoundingBox(const SVIVector3 * pPoints, SVIUInt count);
	SVIBoundingBox(const SVIBoundingBox * pBoxes, SVIUInt count);
public:
	void extend(const SVIVector3 &point);
	SVIBool isValid();

public:
	SVIVector3 getSize();
	SVIVector3 getCenter();
	SVIVector3 point(SVIInt i);


};

}