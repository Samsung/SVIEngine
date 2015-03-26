#include "SVIMath.h"


namespace SVI{


SVIVector4 operator - (const SVIVector4 &kVector)
{
	return SVIVector4(-kVector.x,
					  -kVector.y,
					  -kVector.z,
					  -kVector.w);
}

 SVIVector4 operator - (SVIVector4 &kVector)
{
	return SVIVector4(-kVector.x,
					  -kVector.y,
					  -kVector.z,
					  -kVector.w);
}

 SVIVector4::SVIVector4(const SVIQuaternion & kQuat){
	 x = kQuat.x;
	 y = kQuat.y;
	 z = kQuat.z;
	 w = kQuat.w;
 }

 SVIVector4::SVIVector4(void)
{
	x = y = z = w = 0.0f;
}

 SVIVector4::SVIVector4(const SVIVector2 &kVector)
{
	x = kVector.x;
	y = kVector.y;
	z = 0.0f;
	w = 1.0f;
}

 SVIVector4::SVIVector4(const SVIVector3 &kVector)
{
	x = kVector.x; 
	y = kVector.y;
	z = kVector.z;
	w = 1.0f;
}

 SVIVector4::SVIVector4(const SVIVector4 &kVector)
{
	x = kVector.x;
	y = kVector.y;
	z = kVector.z;
	w = kVector.w;
}

 SVIVector4::SVIVector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}

 float &SVIVector4::operator [] (int iAxis)
{
	return xyzw[iAxis];
}



 bool SVIVector4::operator==(const SVIVector4& kVector) const
 {
	 float diff_x = x - kVector.x;
	 if(!SVI_ALMOST_ZERO(diff_x)) return SVIFALSE;
	 float diff_y = y - kVector.y;
	 if(!SVI_ALMOST_ZERO(diff_y)) return SVIFALSE;
	 float diff_z = z - kVector.z;
	 if(!SVI_ALMOST_ZERO(diff_z)) return SVIFALSE;
	 float diff_w = w - kVector.w;
	 if(!SVI_ALMOST_ZERO(diff_w)) return SVIFALSE;
	 return SVITRUE;
 }

 bool SVIVector4::operator!=(const SVIVector4& kVector) const
 {
	 return (*this == kVector) ? SVIFALSE : SVITRUE;
 }

 bool SVIVector4::operator < (const SVIVector4 &kVector) const
{
	if((x < kVector.x)&&
	   (y < kVector.y)&&
	   (z < kVector.z)&&
	   (w < kVector.w)) return SVITRUE;
	return SVIFALSE;
}

 bool SVIVector4::operator > (const SVIVector4 &kVector) const
{
	if((x > kVector.x)&&
	   (y > kVector.y)&&
	   (z > kVector.z)&&
	   (w > kVector.w)) return SVITRUE;
	return SVIFALSE;
}

 SVIVector4::operator float * (void)
{
	return &x;
}

 SVIVector4::operator const float * (void) const
{
	return &x;
}

 int SVIVector4::operator -= (const SVIVector4 &kVector)
{
	x -= kVector.x;
	y -= kVector.y;
	z -= kVector.z;
	w -= kVector.w;
	return 0;
}

 int SVIVector4::operator += (const SVIVector4 &kVector)
{
	x += kVector.x;
	y += kVector.y;
	z += kVector.z;
	w += kVector.w;
	return 0;
}

 int SVIVector4::operator /= (float fScalar)
{
	x /= fScalar;
	y /= fScalar;
	z /= fScalar;
	w /= fScalar;
	return 0;
}

 int SVIVector4::operator *= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;
	return 0;
}

 const SVIVector4& SVIVector4::operator = (const SVIVector2 &kVector)
{
	x = kVector.x;
	y = kVector.y;
	z = 0.0f;
	w = 1.0f;
	return *this;
}

 const SVIVector4& SVIVector4::operator = (const SVIVector3 &kVector)
{
	x = kVector.x;
	y = kVector.y;
	z = kVector.z;
	w = 1.0f;
	return *this;
}

 const SVIVector4& SVIVector4::operator = (const SVIVector4 &kVector)
{
	x = kVector.x;
	y = kVector.y;
	z = kVector.z;
	w = kVector.w;
	return *this;
}

 SVIVector4 SVIVector4::operator - (const SVIVector4 &kVector) const
{
	return SVIVector4((x - kVector.x),
					  (y - kVector.y),
					  (z - kVector.z),
					  (w - kVector.w));
}

 SVIVector4 SVIVector4::operator + (const SVIVector4 &kVector) const
{
	return SVIVector4((x + kVector.x),
					  (y + kVector.y),
					  (z + kVector.z),
					  (w + kVector.w));
}

 SVIVector4 SVIVector4::operator - (const SVIVector4 &kVector)
{
	return SVIVector4((x - kVector.x),
					  (y - kVector.y),
					  (z - kVector.z),
					  (w - kVector.w));
}

 SVIVector4 SVIVector4::operator + (const SVIVector4 &kVector)
{
	return SVIVector4((x + kVector.x),
					  (y + kVector.y),
					  (z + kVector.z),
					  (w + kVector.w));
}

 SVIVector4 SVIVector4::operator / (float fScalar) const
{
	return SVIVector4((x / fScalar),
				  	  (y / fScalar),
					  (z / fScalar),
					  (w / fScalar));
}

 SVIVector4 SVIVector4::operator * (float fScalar) const
{
	return SVIVector4((x * fScalar),
					  (y * fScalar),
					  (z * fScalar),
					  (w * fScalar));
}

 SVIVector4 SVIVector4::operator / (float fScalar)
{
	return SVIVector4((x / fScalar),
				  	  (y / fScalar),
					  (z / fScalar),
					  (w / fScalar));
}

 SVIVector4 SVIVector4::operator * (float fScalar)
{
	return SVIVector4((x * fScalar),
					  (y * fScalar),
					  (z * fScalar),
					  (w * fScalar));
}

 float SVIVector4::dot(const SVIVector4 &kVector) const
{
	return ((x * kVector.x) +
			(y * kVector.y) + 
			(z * kVector.z) + 
			(w * kVector.w));
}

 float SVIVector4::dot(const SVIVector4 &kVector)
{
	return ((x * kVector.x) +
			(y * kVector.y) + 
			(z * kVector.z) + 
			(w * kVector.w));
}

 float SVIVector4::getlengthSqr(void) const
{
	return dot(*this);
}

 float SVIVector4::getlengthSqr(void)
{
	return dot(*this);
}

 float SVIVector4::getlength(void) const
{
	return sqrtf(getlengthSqr());
}

 float SVIVector4::getlength(void)
{
	return sqrtf(getlengthSqr());
}

 void SVIVector4::setlength(float flength)
{
	*this *= (flength / getlength());
}

 float SVIVector4::distance(const SVIVector4 &kVector) const
{
	return (kVector - *this).getlength();
}

 float SVIVector4::distance(const SVIVector4 &kVector)
{
	return (kVector - *this).getlength();
}

 float SVIVector4::getAngle(const SVIVector4 &kVector)
{
	float fdot = dot(kVector);
	float fMagnitude = (getlength() * kVector.getlength());
	return acosf(fdot / fMagnitude);
}

 SVIVector4 SVIVector4::Cross(const SVIVector4 &kVector) const
{
//	return SVIVector2(((y * kVector.z) - (z * kVector.y)),
//					  ((z * kVector.x) - (x * kVector.z)),
//					  ((x * kVector.y) - (y * kVector.x)));
	return *this;
}

 SVIVector4 SVIVector4::Cross(const SVIVector4 &kVector)
{
//	return SVIVector2(((y * kVector.z) - (z * kVector.y)),
//					  ((z * kVector.x) - (x * kVector.z)),
//					  ((x * kVector.y) - (y * kVector.x)));
	return *this;
}

 SVIVector4 SVIVector4::lerp(const SVIVector4 &kVector, float fFactor) const
{
	return (*this + ((kVector - *this) * fFactor));
}

 SVIVector4 SVIVector4::lerp(const SVIVector4 &kVector, float fFactor)
{
	return (*this + ((kVector - *this) * fFactor));
}

 SVIVector4 SVIVector4::normal(void) const
{
	float flength = getlength();
	return (*this * (1.0f / flength));
}

 SVIVector4 SVIVector4::normal(void)
{
	float flength = getlength();
	return (*this * (1.0f / flength));
}

 float SVIVector4::normalize(void)
{
	float flength = getlength();
	float fInvlength = 1.0f/flength;
	x *= fInvlength;
	y *= fInvlength;
	z *= fInvlength;
	w *= fInvlength;
	return flength;
}
}