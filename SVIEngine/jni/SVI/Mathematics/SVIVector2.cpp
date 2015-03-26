#include "SVIMath.h"

//2011-07-26
//back to float
namespace SVI{
	SVIVector2 operator - (const SVIVector2 &kVector)
	{
		return SVIVector2(-kVector.x,
			-kVector.y);
	}

	SVIVector2 operator - (SVIVector2 &kVector)
	{
		return SVIVector2(-kVector.x,
			-kVector.y);
	}

	SVIVector2 operator * (float fScalar, const SVIVector2& kVector)
	{
		return SVIVector2(kVector.x*fScalar, kVector.y*fScalar);
	}

	SVIVector2::SVIVector2(void)
	{
		x = y = 0.0f;
	}

	SVIVector2::SVIVector2(const SVIVector2 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
	}

	SVIVector2::SVIVector2(const SVIVector3 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
	}

	SVIVector2::SVIVector2(const SVIVector4 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
	}

	SVIVector2::SVIVector2(float fX, float fY)
	{
		x = fX;
		y = fY;
	}

	float &SVIVector2::operator [] (int iAxis)
	{
		return xy[iAxis];
	}

	bool SVIVector2::operator==(const SVIVector2& kVector) const
	{
		float diff_x = x - kVector.x;
		if(!SVI_ALMOST_ZERO(diff_x)) return false;
		float diff_y = y - kVector.y;
		if(!SVI_ALMOST_ZERO(diff_y)) return false;

		return true;
	}

	bool SVIVector2::operator!=(const SVIVector2& kVector) const
	{
		return (*this == kVector) ? false : true;
	}
	bool SVIVector2::operator < (const SVIVector2 &kVector) const
	{
		if((x < kVector.x)&&
			(y < kVector.y)) return true;
		return false;
	}

	bool SVIVector2::operator > (const SVIVector2 &kVector) const
	{
		if((x > kVector.x)&&
			(y > kVector.y)) return true;
		return false;
	}

	SVIVector2::operator float * (void)
	{
		return &x;
	}

	SVIVector2::operator const float * (void) const
	{
		return &x;
	}

	int SVIVector2::operator -= (const SVIVector2 &kVector)
	{
		x -= kVector.x;
		y -= kVector.y;
		return 1;
	}

	int SVIVector2::operator += (const SVIVector2 &kVector)
	{
		x += kVector.x;
		y += kVector.y;
		return 1;
	}

	int SVIVector2::operator /= (float fScalar)
	{
		x /= fScalar;
		y /= fScalar;
		return 1;
	}

	int SVIVector2::operator *= (float fScalar)
	{
		x *= fScalar;
		y *= fScalar;
		return 1;
	}

	const SVIVector2& SVIVector2::operator = (const SVIVector2 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		return *this;
	}

	const SVIVector2& SVIVector2::operator = (const SVIVector3 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		return *this;
	}

	const SVIVector2& SVIVector2::operator = (const SVIVector4 &kVector)
	{
		x = kVector.x;
		y = kVector.y;
		return *this;
	}

	SVIVector2 SVIVector2::operator - (const SVIVector2 &kVector) const
	{
		return SVIVector2((x - kVector.x),
			(y - kVector.y));
	}

	SVIVector2 SVIVector2::operator + (const SVIVector2 &kVector) const
	{
		return SVIVector2((x + kVector.x),
			(y + kVector.y));
	}

	SVIVector2 SVIVector2::operator - (const SVIVector2 &kVector)
	{
		return SVIVector2((x - kVector.x),
			(y - kVector.y));
	}

	SVIVector2 SVIVector2::operator + (const SVIVector2 &kVector)
	{
		return SVIVector2((x + kVector.x),
			(y + kVector.y));
	}

	SVIVector2 SVIVector2::operator / (float fScalar) const
	{
		return SVIVector2((x / fScalar),
			(y / fScalar));
	}

	SVIVector2 SVIVector2::operator * (float fScalar) const
	{
		return SVIVector2((x * fScalar),
			(y * fScalar));
	}

	SVIVector2 SVIVector2::operator / (float fScalar)
	{
		return SVIVector2((x / fScalar),
			(y / fScalar));
	}

	SVIVector2 SVIVector2::operator * (float fScalar)
	{
		return SVIVector2((x * fScalar),
			(y * fScalar));
	}

	float SVIVector2::dot(const SVIVector2 &kVector) const
	{
		return ((x * kVector.x) +
			(y * kVector.y));
	}

	float SVIVector2::dot(const SVIVector2 &kVector)
	{
		return ((x * kVector.x) +
			(y * kVector.y));
	}

	float SVIVector2::getlengthSqr(void) const
	{
		return dot(*this);
	}

	float SVIVector2::getlengthSqr(void)
	{
		return dot(*this);
	}

	float SVIVector2::getlength(void) const
	{
		return sqrtf(getlengthSqr());
	}

	float SVIVector2::getlength(void)
	{
		return sqrtf(getlengthSqr());
	}

	void SVIVector2::setlength(float flength)
	{
		*this *= (flength / getlength());
	}

	float SVIVector2::getAngle(const SVIVector2 &kVector)
	{
		float fdot = dot(kVector);
		fdot = acosf(fdot);
		if(x*kVector.y - y*kVector.x < 0) fdot = -fdot;
		return -fdot;
	}

	float SVIVector2::distance(const SVIVector2 &kVector) const
	{
		return (kVector - *this).getlength();
	}

	float SVIVector2::distance(const SVIVector2 &kVector)
	{
		return (kVector - *this).getlength();
	}

	SVIVector2 SVIVector2::lerp(const SVIVector2 &kVector, float fFactor) const
	{
		return (*this + ((kVector - *this) * fFactor));
	}

	SVIVector2 SVIVector2::lerp(const SVIVector2 &kVector, float fFactor)
	{
		return (*this + ((kVector - *this) * fFactor));
	}

	SVIVector2 SVIVector2::normal(void) const
	{
		float flength = getlength();
		return (*this * (1.0f / flength));
	}

	SVIVector2 SVIVector2::normal(void)
	{
		float flength = getlength();
		return (*this * (1.0f / flength));
	}

	float SVIVector2::normalize(void)
	{
		float flength = getlength();
		float fInvlength = 1.0f/flength;
		x *= fInvlength;
		y *= fInvlength;
		return flength;
	}

}