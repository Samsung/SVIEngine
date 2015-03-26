#include "SVIMath.h"

namespace SVI {

	SVIQuaternion SVIQuaternion::operator * (const SVIQuaternion &kQuat)
	{
		SVIQuaternion kResult;
		kResult.x = ((w * kQuat.x) + (x * kQuat.w) + (y * kQuat.z) - (z * kQuat.y));
		kResult.y = ((w * kQuat.y) - (x * kQuat.z) + (y * kQuat.w) + (z * kQuat.x));
		kResult.z = ((w * kQuat.z) + (x * kQuat.y) - (y * kQuat.x) + (z * kQuat.w));
		kResult.w = ((w * kQuat.w) - (x * kQuat.x) - (y * kQuat.y) - (z * kQuat.z));
		return kResult;
	}

	void SVIQuaternion::operator *= (const SVIQuaternion &kQuat)
	{
		SVIQuaternion kResult;
		kResult.x = ((w * kQuat.x) + (x * kQuat.w) + (y * kQuat.z) - (z * kQuat.y));
		kResult.y = ((w * kQuat.y) - (x * kQuat.z) + (y * kQuat.w) + (z * kQuat.x));
		kResult.z = ((w * kQuat.z) + (x * kQuat.y) - (y * kQuat.x) + (z * kQuat.w));
		kResult.w = ((w * kQuat.w) - (x * kQuat.x) - (y * kQuat.y) - (z * kQuat.z));
		*this = kResult;
	} 

	void SVIQuaternion::setEuler(const SVIVector3 &kEuler)
	{
		float fSINx = sinf(kEuler.x * 0.5f);
		float fSINy = sinf(kEuler.y * 0.5f);
		float fSINz = sinf(kEuler.z * 0.5f);
		float fCOSx = cosf(kEuler.x * 0.5f);
		float fCOSy = cosf(kEuler.y * 0.5f);
		float fCOSz = cosf(kEuler.z * 0.5f);
		float fSINyCOSz = (fSINy * fCOSz);
		float fSINySINz = (fSINy * fSINz);
		float fCOSyCOSz = (fCOSy * fCOSz);
		float fCOSySINz = (fCOSy * fSINz);

		x = ((fSINx * fCOSyCOSz) - (fCOSx * fSINySINz));
		y = ((fSINx * fCOSySINz) + (fCOSx * fSINyCOSz));
		z = ((fCOSx * fCOSySINz) - (fSINx * fSINyCOSz));
		w = ((fCOSx * fCOSyCOSz) + (fSINx * fSINySINz));
	}

	void SVIQuaternion::setRotation(const SVIVector3 &kAxis, float fAngle)
	{
		SVIVector3 normalizeAxis = kAxis;
		normalizeAxis.normalize();

		float fSin = sin(fAngle*0.5f);
		float fCos = cos(fAngle*0.5f);

		x = normalizeAxis.x*fSin;
		y = normalizeAxis.y*fSin;
		z = normalizeAxis.z*fSin;
		w = fCos;
	}

	void SVIQuaternion::setMatrix(const SVIMatrix &kMatrix)
	{
		float w = 0.5f*sqrtf(kMatrix._11+kMatrix._22+kMatrix._33);
		float s = 0.25f/w;
		x = (kMatrix._23-kMatrix._32)*s;
		y = (kMatrix._31-kMatrix._13)*s;
		z = (kMatrix._12-kMatrix._21)*s;
		w = w;

	}

	SVIQuaternion SVIQuaternion::normal(void)
	{
		float flength = length();
		return SVIQuaternion((x / flength),
			(y / flength),
			(z / flength),
			(w / flength));
	}

	void SVIQuaternion::normalize(void)
	{
		*this = normal();
	}


	SVIQuaternion SVIQuaternion::Slerp(const SVIQuaternion &kQuat, float fFactor) const
	{
		float fdot = dot(kQuat);
		bool bFlip = SVIFALSE;
		if(fdot < 0.0f) {
			fdot = -fdot;
			bFlip = SVITRUE;
		}

		float fInv_factor = 0.0f;
		if(1.0 - fdot < 1e-5F) {
			fInv_factor = 1 - fFactor;
		} else {
			float fTheta = acosf(fdot);
			float s = 1/sinf(fTheta);

			fInv_factor = sinf((1 - fFactor)*fTheta)*s;
			fFactor = sinf(fFactor*fTheta)*s;
		}
		if(bFlip) fFactor = -fFactor;
		SVIQuaternion kResult;
		kResult.x = (x * fInv_factor + (kQuat.x * fFactor));
		kResult.y = (y * fInv_factor + (kQuat.y * fFactor));
		kResult.z = (z * fInv_factor + (kQuat.z * fFactor));
		kResult.w = (w * fInv_factor + (kQuat.w * fFactor));
		return kResult;
	}

	SVIQuaternion SVIQuaternion::lerp(const SVIQuaternion &kQuat, float fFactor) const
	{
		SVIQuaternion kResult;
		kResult.x = (x + ((kQuat.x - x) * fFactor));
		kResult.y = (y + ((kQuat.y - y) * fFactor));
		kResult.z = (z + ((kQuat.z - z) * fFactor));
		kResult.w = (w + ((kQuat.w - w) * fFactor));
		return kResult;
	}


	SVIQuaternion::SVIQuaternion(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

SVIQuaternion::SVIQuaternion(const SVIVector4 &kQuat){
	x = kQuat.x;
	y = kQuat.y; 
	z = kQuat.z;
	w = kQuat.w;
}

SVIQuaternion::SVIQuaternion(const SVIMatrix &kMatrix)
{
	setMatrix(kMatrix);
}

SVIQuaternion::SVIQuaternion(const SVIQuaternion &kQuat)
{
	x = kQuat.x;
	y = kQuat.y; 
	z = kQuat.z;
	w = kQuat.w;
}

SVIQuaternion::SVIQuaternion(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}

SVIQuaternion::SVIQuaternion(const SVIVector3 &kAxis, float fAngle)
{
	setRotation(kAxis, fAngle);
}

SVIQuaternion::SVIQuaternion(const SVIVector3 &kEuler)
{
	setEuler(kEuler);
}

SVIQuaternion::SVIQuaternion(const float* pQuat)
{
	x = pQuat[0];
	y = pQuat[1];
	z = pQuat[2];
	w = pQuat[3];
}

bool SVIQuaternion::operator==(const SVIQuaternion& kQuaternion) const
{
	float diff_x = x - kQuaternion.x;
	if(!SVI_ALMOST_ZERO(diff_x)) return SVIFALSE;
	float diff_y = y - kQuaternion.y;
	if(!SVI_ALMOST_ZERO(diff_y)) return SVIFALSE;
	float diff_z = z - kQuaternion.z;
	if(!SVI_ALMOST_ZERO(diff_z)) return SVIFALSE;
	float diff_w = w - kQuaternion.w;
	if(!SVI_ALMOST_ZERO(diff_w)) return SVIFALSE;
	return SVITRUE;
}

bool SVIQuaternion::operator!=(const SVIQuaternion& kQuaternion) const
{
	return (*this == kQuaternion) ? SVIFALSE : SVITRUE;
}
SVIQuaternion::operator float * (void)
{
	return &x;
}

int SVIQuaternion::operator -= (const SVIQuaternion &kQuat)
{
	x -= kQuat.x;
	y -= kQuat.y;
	z -= kQuat.z;
	w -= kQuat.w;
	return 0;
}

int SVIQuaternion::operator += (const SVIQuaternion &kQuat)
{
	x += kQuat.x;
	y += kQuat.y;
	z += kQuat.z;
	w += kQuat.w;
	return 0;
}

int SVIQuaternion::operator /= (float fScalar)
{
	x /= fScalar;
	y /= fScalar;
	z /= fScalar;
	w /= fScalar;
	return 0;
}

int SVIQuaternion::operator *= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;
	return 0;
}

const SVIQuaternion& SVIQuaternion::operator = (const SVIQuaternion &kQuat)
{
	x = kQuat.x;
	y = kQuat.y;
	z = kQuat.z;
	w = kQuat.w;
	return *this;
}

SVIQuaternion SVIQuaternion::operator - (const SVIQuaternion &kQuat)
{
	return SVIQuaternion((x - kQuat.x),
					    (y - kQuat.y),
					    (z - kQuat.z),
					    (w - kQuat.w));
}

SVIQuaternion SVIQuaternion::operator + (const SVIQuaternion &kQuat)
{
	return SVIQuaternion((x + kQuat.x),
					    (y + kQuat.y),
					    (z + kQuat.z),
					    (w + kQuat.w));
}

SVIQuaternion SVIQuaternion::operator / (float fScalar)
{
	return SVIQuaternion((x / fScalar),
				  		 (y / fScalar),
						 (z / fScalar),
						 (w / fScalar));
}

SVIQuaternion SVIQuaternion::operator * (float fScalar)
{
	return SVIQuaternion((x * fScalar),
						 (y * fScalar),
						 (z * fScalar),
						 (w * fScalar));
}

void SVIQuaternion::setIdentity(void)
{
	x = y = z = 0.0f;
	w = 1.0f;
}

float SVIQuaternion::dot(const SVIQuaternion &kQuat) const
{
	return ((x * kQuat.x) +
			(y * kQuat.y) +
			(z * kQuat.z) +
			(w * kQuat.w));
}

float SVIQuaternion::lengthSqr(void)
{
	return dot(*this);
}

float SVIQuaternion::length(void)
{
	return sqrtf(lengthSqr());
}
};