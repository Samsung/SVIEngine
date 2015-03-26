#include "SVIMath.h"

namespace SVI{

	SVIPlane::SVIPlane(const SVIVector3 &kVector1, const SVIVector3 &kSVIVector2, const SVIVector3 &kSVIVector3)
	{
		SVIVector3 kVectorA(kVector1 - kSVIVector2);
		SVIVector3 kVectorB(kSVIVector3 - kSVIVector2);
		SVIVector3 kCross(kVectorA.Cross(kVectorB));
		n = kCross.normal();
		d = -n.dot(kVector1);
	}

	float SVIPlane::distance(const SVIVector3 &kPoint) const
	{
		return (n.dot(kPoint) + d);
	}
 
	/*bool SVIPlane::Intersect(const Ray &kRay) const
	{
	float fdistanceA = distance(kRay.m_kOrigin);
	float fdistanceB = distance(kRay.m_kOrigin + kRay.m_kVector);
	if((fdistanceA > 0.0f)&&(fdistanceB < 0.0f)) return SVITRUE;
	if((fdistanceA < 0.0f)&&(fdistanceB > 0.0f)) return SVITRUE;
	return SVIFALSE;
	}

	SVIVector3 SVIPlane::Intersection(const Ray &kRay) const
	{
	float fNumer = n.dot(kRay.m_kOrigin);
	float fDenom = n.dot(kRay.m_kVector);
	float fInter = ((-d - fNumer)/fDenom);
	return (kRay.m_kOrigin + (kRay.m_kVector * fInter));
	}*/

SVIPlane::SVIPlane(void)
{
	n.x = 0.0f;
	n.y = 0.0f;
	n.z = 0.0f;
	d = 0.0f;
}

SVIPlane::SVIPlane(const SVIPlane &kSVIPlane)
{
	n = kSVIPlane.n;
	d = kSVIPlane.d;
}

SVIPlane::SVIPlane(float fA, float fB, float fC, float fD)
{ 
	n.x = fA;
	n.y = fB;
	n.z = fC;
	d = fD;
}

SVIPlane::SVIPlane(const SVIVector3 &knormal, float fdistance)
{
	n = knormal;
	d = fdistance;
}

SVIPlane SVIPlane::operator = (const SVIPlane &kSVIPlane)
{
	n = kSVIPlane.n;
	d = kSVIPlane.d;
	return *this;
}

};