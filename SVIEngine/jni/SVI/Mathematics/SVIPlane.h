#pragma once

namespace SVI{
	
struct SVIVector2;
struct SVIVector3;
struct SVIVector4;
struct SVIMatrix;
struct SVIQuaternion;

struct SVIPlane
{
public:
	SVIVector3 n;
	float d;

public:
	SVIPlane(void);
	SVIPlane(const SVIPlane &kSVIPlane);
	SVIPlane(float fA, float fB, float fC, float fD);
	SVIPlane(const SVIVector3 &knormal, float fdistance);
	SVIPlane(const SVIVector3 &kVector1, const SVIVector3 &kSVIVector2, const SVIVector3 &kSVIVector3);

	SVIPlane operator = (const SVIPlane &kSVIPlane);


	float distance(const SVIVector3 &kPoint) const;
};
}