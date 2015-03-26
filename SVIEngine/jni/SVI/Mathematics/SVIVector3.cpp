#include "SVIMath.h"

namespace SVI{

#define length_ELIPSON			0.000001

	//2011-07-26
	//back to float

	SVIVector3 operator - (const SVIVector3 &kVector)
	{
		return SVIVector3(-kVector.x,
			-kVector.y,
			-kVector.z);
	}

	SVIVector3 operator - (SVIVector3 &kVector)
	{
		return SVIVector3(-kVector.x,
			-kVector.y,
			-kVector.z);
	}

	SVIVector3::SVIVector3(void)
	{
		x = y = z = 0.0f;
	}

	SVIVector3::SVIVector3(const SVIVector2 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		z = 0.0f;
	}

	SVIVector3::SVIVector3(const SVIVector3 &kVector) 
	{
		x = kVector.x;
		y = kVector.y;
		z = kVector.z;
	}

	SVIVector3::SVIVector3(const SVIVector4 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		z = kVector.z;
	}

	SVIVector3::SVIVector3(float fX, float fY, float fZ)
	{
		x = fX;
		y = fY;
		z = fZ;
	}

	SVIVector3::SVIVector3(float* pV)
	{
		x = pV[0];
		y = pV[1];
		z = pV[2];
	}

	SVIVector3::SVIVector3(const float* pV)
	{
		x = pV[0];
		y = pV[1];
		z = pV[2];
	}

	float &SVIVector3::operator [] (int iAxis)
	{
		return xyz[iAxis];
	}

	bool SVIVector3::operator < (const SVIVector3 &kVector) const
	{
		if((x < kVector.x)&&
			(y < kVector.y)&&
			(z < kVector.z)) return true;
		return false;
	}

	bool SVIVector3::operator > (const SVIVector3 &kVector) const
	{
		if((x > kVector.x)&&
			(y > kVector.y)&&
			(z > kVector.z)) return true;
		return false;
	}

	SVIVector3::operator float * (void)
	{
		return &x;
	}

	SVIVector3::operator const float * (void) const
	{
		return &x;
	}

	bool SVIVector3::operator==(const SVIVector3& kVector) const
	{
		float diff_x = x - kVector.x;
		if(!SVI_ALMOST_ZERO(diff_x)) return false;
		float diff_y = y - kVector.y;
		if(!SVI_ALMOST_ZERO(diff_y)) return false;
		float diff_z = z - kVector.z;
		if(!SVI_ALMOST_ZERO(diff_z)) return false;
		return true;
	}

	bool SVIVector3::operator!=(const SVIVector3& kVector) const
	{
		return (*this == kVector) ? false : true;
	}

	int SVIVector3::operator -= (const SVIVector3 &kVector)
	{
		x -= kVector.x;
		y -= kVector.y;
		z -= kVector.z;
		return 1;
	}

	int SVIVector3::operator += (const SVIVector3 &kVector)
	{
		x += kVector.x;
		y += kVector.y;
		z += kVector.z;
		return 1;

	}

	int SVIVector3::operator /= (float fScalar)
	{
		x /= fScalar;
		y /= fScalar;
		z /= fScalar;
		return 1;
	}

	int SVIVector3::operator *= (float fScalar)
	{
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		return 1;
	}

	SVIVector3 SVIVector3::operator * (const SVIVector3 &kVector)
	{
		return SVIVector3(x*kVector.x, y*kVector.y, z*kVector.z);
	}

	const SVIVector3& SVIVector3::operator = (const SVIVector2 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		z = 0.0f;
		return *this;
	}

	const SVIVector3& SVIVector3::operator = (const SVIVector3 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		z = kVector.z;
		return *this;
	}

	const SVIVector3& SVIVector3::operator = (const SVIVector4 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		z = kVector.z;
		return *this;
	}

	SVIVector3 SVIVector3::operator - (const SVIVector3 &kVector) const
	{
		return SVIVector3((x - kVector.x),
			(y - kVector.y),
			(z - kVector.z));
	}

	SVIVector3 SVIVector3::operator + (const SVIVector3 &kVector) const
	{
		return SVIVector3((x + kVector.x),
			(y + kVector.y),
			(z + kVector.z));
	}

	SVIVector3 SVIVector3::operator - (const SVIVector3 &kVector)
	{
		return SVIVector3((x - kVector.x),
			(y - kVector.y),
			(z - kVector.z));
	}

	SVIVector3 SVIVector3::operator + (const SVIVector3 &kVector)
	{
		return SVIVector3((x + kVector.x),
			(y + kVector.y),
			(z + kVector.z));
	}

	SVIVector3 SVIVector3::operator / (float fScalar) const
	{
		if(fabs(fScalar) < length_ELIPSON) {
			return SVIVector3();
		}
		return SVIVector3((x / fScalar),
			(y / fScalar),
			(z / fScalar));
	}

	SVIVector3 SVIVector3::operator * (float fScalar) const
	{
		return SVIVector3((x * fScalar),
			(y * fScalar),
			(z * fScalar));
	}

	SVIVector3 SVIVector3::operator / (float fScalar)
	{
		if(fabs(fScalar) < length_ELIPSON) {
			return SVIVector3();
		}
		return SVIVector3((x / fScalar),
			(y / fScalar),
			(z / fScalar));
	}

	SVIVector3 SVIVector3::operator * (float fScalar)
	{
		return SVIVector3((x * fScalar),
			(y * fScalar),
			(z * fScalar));
	}

	float SVIVector3::dot(const SVIVector3 &kVector) const
	{
		return ((x * kVector.x) +
			(y * kVector.y) + 
			(z * kVector.z));
	}

	float SVIVector3::dot(const SVIVector3 &kVector)
	{
		return ((x * kVector.x) +
			(y * kVector.y) + 
			(z * kVector.z));
	}

	float SVIVector3::getlengthSqr(void) const
	{
		return dot(*this);
	}

	float SVIVector3::getlengthSqr(void)
	{
		return dot(*this);
	}

	float SVIVector3::getlength(void) const
	{
		return sqrtf(getlengthSqr());
	}

	float SVIVector3::getlength(void)
	{
		return sqrtf(getlengthSqr());
	}

	void SVIVector3::setlength(float flength)
	{
		*this *= (flength / getlength());
	}

	float SVIVector3::distance(const SVIVector3 &kVector) const
	{
		return (kVector - *this).getlength();
	}

	float SVIVector3::distance(const SVIVector3 &kVector)
	{
		return (kVector - *this).getlength();
	}

	float SVIVector3::getAngle(const SVIVector3 &kVector)
	{
		float fdot = dot(kVector);
		float fMagnitude = (getlength() * kVector.getlength());
		return acosf(fdot / fMagnitude);
	}

	SVIVector3 SVIVector3::Cross(const SVIVector3 &kVector) const
	{
		return SVIVector3(((y * kVector.z) - (z * kVector.y)),
			((z * kVector.x) - (x * kVector.z)),
			((x * kVector.y) - (y * kVector.x)));
	}

	SVIVector3 SVIVector3::Cross(const SVIVector3 &kVector)
	{
		return SVIVector3(((y * kVector.z) - (z * kVector.y)),
			((z * kVector.x) - (x * kVector.z)),
			((x * kVector.y) - (y * kVector.x)));
	}

	SVIVector3 SVIVector3::lerp(const SVIVector3 &kVector, float fFactor) const
	{
		return (*this + ((kVector - *this) * fFactor));
	}

	SVIVector3 SVIVector3::lerp(const SVIVector3 &kVector, float fFactor)
	{
		return (*this + ((kVector - *this) * fFactor));
	}

	SVIVector3 SVIVector3::normal(void) const
	{
		float flength = getlength();
		if(flength < length_ELIPSON) {
			return *this;
		}
		return (*this * (1.0f / flength));
	}

	SVIVector3 SVIVector3::normal(void)
	{
		float flength = getlength();
		if(flength < length_ELIPSON) {
			return *this;
		}
		return (*this * (1.0f / flength));
	}

	float SVIVector3::normalize(void)
	{
		float flength = getlength();
		float fInvlength = 1.0f/flength;
		x *= fInvlength;
		y *= fInvlength;
		z *= fInvlength;
		return flength;
	}

	float	SquareMagnitude (const SVIVector3& v)
	{
		return v.x*v.x + v.y*v.y + v.z*v.z;
	}

	float	Magnitude (const SVIVector3& v)
	{
		return (float) sqrt(SquareMagnitude(v));
	}

	float
		dotProduct (const SVIVector3& v1, const SVIVector3& v2)
	{
		return v1.x*v2.x + v1.y * v2.y + v1.z*v2.z;
	}


}