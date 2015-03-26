
#include "SVIMath.h"
#include "SVIQuaternion.h"

namespace SVI {
	static const SVIBool DEBUG = SVIFALSE;

	SVIMatrix SVIMatrix::operator * (const SVIMatrix &kMatrix)
	{
		return SVIMatrix(((_11 * kMatrix._11) + (_12 * kMatrix._21) + (_13 * kMatrix._31) + (_14 * kMatrix._41)),
			((_11 * kMatrix._12) + (_12 * kMatrix._22) + (_13 * kMatrix._32) + (_14 * kMatrix._42)),
			((_11 * kMatrix._13) + (_12 * kMatrix._23) + (_13 * kMatrix._33) + (_14 * kMatrix._43)),
			((_11 * kMatrix._14) + (_12 * kMatrix._24) + (_13 * kMatrix._34) + (_14 * kMatrix._44)),

			((_21 * kMatrix._11) + (_22 * kMatrix._21) + (_23 * kMatrix._31) + (_24 * kMatrix._41)),
			((_21 * kMatrix._12) + (_22 * kMatrix._22) + (_23 * kMatrix._32) + (_24 * kMatrix._42)),
			((_21 * kMatrix._13) + (_22 * kMatrix._23) + (_23 * kMatrix._33) + (_24 * kMatrix._43)),
			((_21 * kMatrix._14) + (_22 * kMatrix._24) + (_23 * kMatrix._34) + (_24 * kMatrix._44)),

			((_31 * kMatrix._11) + (_32 * kMatrix._21) + (_33 * kMatrix._31) + (_34 * kMatrix._41)),
			((_31 * kMatrix._12) + (_32 * kMatrix._22) + (_33 * kMatrix._32) + (_34 * kMatrix._42)),
			((_31 * kMatrix._13) + (_32 * kMatrix._23) + (_33 * kMatrix._33) + (_34 * kMatrix._43)),
			((_31 * kMatrix._14) + (_32 * kMatrix._24) + (_33 * kMatrix._34) + (_34 * kMatrix._44)),

			((_41 * kMatrix._11) + (_42 * kMatrix._21) + (_43 * kMatrix._31) + (_44 * kMatrix._41)),
			((_41 * kMatrix._12) + (_42 * kMatrix._22) + (_43 * kMatrix._32) + (_44 * kMatrix._42)),
			((_41 * kMatrix._13) + (_42 * kMatrix._23) + (_43 * kMatrix._33) + (_44 * kMatrix._43)),
			((_41 * kMatrix._14) + (_42 * kMatrix._24) + (_43 * kMatrix._34) + (_44 * kMatrix._44)));
	}

	SVIMatrix SVIMatrix::operator * (const SVIMatrix &kMatrix) const
	{
		return SVIMatrix(((_11 * kMatrix._11) + (_12 * kMatrix._21) + (_13 * kMatrix._31) + (_14 * kMatrix._41)),
			((_11 * kMatrix._12) + (_12 * kMatrix._22) + (_13 * kMatrix._32) + (_14 * kMatrix._42)),
			((_11 * kMatrix._13) + (_12 * kMatrix._23) + (_13 * kMatrix._33) + (_14 * kMatrix._43)),
			((_11 * kMatrix._14) + (_12 * kMatrix._24) + (_13 * kMatrix._34) + (_14 * kMatrix._44)),

			((_21 * kMatrix._11) + (_22 * kMatrix._21) + (_23 * kMatrix._31) + (_24 * kMatrix._41)),
			((_21 * kMatrix._12) + (_22 * kMatrix._22) + (_23 * kMatrix._32) + (_24 * kMatrix._42)),
			((_21 * kMatrix._13) + (_22 * kMatrix._23) + (_23 * kMatrix._33) + (_24 * kMatrix._43)),
			((_21 * kMatrix._14) + (_22 * kMatrix._24) + (_23 * kMatrix._34) + (_24 * kMatrix._44)),

			((_31 * kMatrix._11) + (_32 * kMatrix._21) + (_33 * kMatrix._31) + (_34 * kMatrix._41)),
			((_31 * kMatrix._12) + (_32 * kMatrix._22) + (_33 * kMatrix._32) + (_34 * kMatrix._42)),
			((_31 * kMatrix._13) + (_32 * kMatrix._23) + (_33 * kMatrix._33) + (_34 * kMatrix._43)),
			((_31 * kMatrix._14) + (_32 * kMatrix._24) + (_33 * kMatrix._34) + (_34 * kMatrix._44)),

			((_41 * kMatrix._11) + (_42 * kMatrix._21) + (_43 * kMatrix._31) + (_44 * kMatrix._41)),
			((_41 * kMatrix._12) + (_42 * kMatrix._22) + (_43 * kMatrix._32) + (_44 * kMatrix._42)),
			((_41 * kMatrix._13) + (_42 * kMatrix._23) + (_43 * kMatrix._33) + (_44 * kMatrix._43)),
			((_41 * kMatrix._14) + (_42 * kMatrix._24) + (_43 * kMatrix._34) + (_44 * kMatrix._44)));
	}



	bool SVIMatrix::operator==(const SVIMatrix& kMatrix) const
	{
		for (int i = 0; i < 16; i++){			
			float diff = m[i] - kMatrix.m[i];
			if(!ALMOST_ZERO(diff)) return SVIFALSE;
		}		
		return SVITRUE;
	}

	bool SVIMatrix::operator!=(const SVIMatrix& kMatrix) const
	{
		return (*this == kMatrix) ? SVIFALSE : SVITRUE;
	}

	SVIMatrix SVIMatrix::operator * (float fVal)
	{
		return SVIMatrix(_11*fVal, _12*fVal, _13*fVal, _14*fVal,
			_21*fVal, _22*fVal, _23*fVal, _24*fVal,
			_31*fVal, _32*fVal, _33*fVal, _34*fVal,
			_41*fVal, _42*fVal, _43*fVal, _44*fVal);
	}

	SVIMatrix SVIMatrix::operator * (float fVal) const
	{
		return SVIMatrix(_11*fVal, _12*fVal, _13*fVal, _14*fVal,
			_21*fVal, _22*fVal, _23*fVal, _24*fVal,
			_31*fVal, _32*fVal, _33*fVal, _34*fVal,
			_41*fVal, _42*fVal, _43*fVal, _44*fVal);
	}

	void SVIMatrix::operator *= (const SVIMatrix &kMatrix)
	{
		*this =  SVIMatrix(((_11 * kMatrix._11) + (_12 * kMatrix._21) + (_13 * kMatrix._31) + (_14 * kMatrix._41)),
			((_11 * kMatrix._12) + (_12 * kMatrix._22) + (_13 * kMatrix._32) + (_14 * kMatrix._42)),
			((_11 * kMatrix._13) + (_12 * kMatrix._23) + (_13 * kMatrix._33) + (_14 * kMatrix._43)),
			((_11 * kMatrix._14) + (_12 * kMatrix._24) + (_13 * kMatrix._34) + (_14 * kMatrix._44)),

			((_21 * kMatrix._11) + (_22 * kMatrix._21) + (_23 * kMatrix._31) + (_24 * kMatrix._41)),
			((_21 * kMatrix._12) + (_22 * kMatrix._22) + (_23 * kMatrix._32) + (_24 * kMatrix._42)),
			((_21 * kMatrix._13) + (_22 * kMatrix._23) + (_23 * kMatrix._33) + (_24 * kMatrix._43)),
			((_21 * kMatrix._14) + (_22 * kMatrix._24) + (_23 * kMatrix._34) + (_24 * kMatrix._44)),

			((_31 * kMatrix._11) + (_32 * kMatrix._21) + (_33 * kMatrix._31) + (_34 * kMatrix._41)),
			((_31 * kMatrix._12) + (_32 * kMatrix._22) + (_33 * kMatrix._32) + (_34 * kMatrix._42)),
			((_31 * kMatrix._13) + (_32 * kMatrix._23) + (_33 * kMatrix._33) + (_34 * kMatrix._43)),
			((_31 * kMatrix._14) + (_32 * kMatrix._24) + (_33 * kMatrix._34) + (_34 * kMatrix._44)),

			((_41 * kMatrix._11) + (_42 * kMatrix._21) + (_43 * kMatrix._31) + (_44 * kMatrix._41)),
			((_41 * kMatrix._12) + (_42 * kMatrix._22) + (_43 * kMatrix._32) + (_44 * kMatrix._42)),
			((_41 * kMatrix._13) + (_42 * kMatrix._23) + (_43 * kMatrix._33) + (_44 * kMatrix._43)),
			((_41 * kMatrix._14) + (_42 * kMatrix._24) + (_43 * kMatrix._34) + (_44 * kMatrix._44)));
	}

	void SVIMatrix::operator *= (float fVal)
	{
		_11 *= fVal; _12 *= fVal; _13 *= fVal; _14 *= fVal;
		_21 *= fVal; _22 *= fVal; _23 *= fVal; _24 *= fVal;
		_31 *= fVal; _32 *= fVal; _33 *= fVal; _34 *= fVal;
		_41 *= fVal; _42 *= fVal; _43 *= fVal; _44 *= fVal;
	}

	void SVIMatrix::rotate(const SVIMatrix &kMatrix)
	{
		SVIMatrix kBase(*this);
		_11 = ((kBase._11 * kMatrix._11) + (kBase._12 * kMatrix._21) + (kBase._13 * kMatrix._31));
		_12 = ((kBase._11 * kMatrix._12) + (kBase._12 * kMatrix._22) + (kBase._13 * kMatrix._32));
		_13 = ((kBase._11 * kMatrix._13) + (kBase._12 * kMatrix._23) + (kBase._13 * kMatrix._33));

		_21 = ((kBase._21 * kMatrix._11) + (kBase._22 * kMatrix._21) + (kBase._23 * kMatrix._31));
		_22 = ((kBase._21 * kMatrix._12) + (kBase._22 * kMatrix._22) + (kBase._23 * kMatrix._32));
		_23 = ((kBase._21 * kMatrix._13) + (kBase._22 * kMatrix._23) + (kBase._23 * kMatrix._33));

		_31 = ((kBase._31 * kMatrix._11) + (kBase._32 * kMatrix._21) + (kBase._33 * kMatrix._31));
		_32 = ((kBase._31 * kMatrix._12) + (kBase._32 * kMatrix._22) + (kBase._33 * kMatrix._32));
		_33 = ((kBase._31 * kMatrix._13) + (kBase._32 * kMatrix._23) + (kBase._33 * kMatrix._33));
	}

	// 2003 6 16
	void SVIMatrix::setEuler(const SVIVector3 &kEuler)
	{
		setIdentity();
		float fCOSx = cosf(kEuler.x);
		float fSINx = sinf(kEuler.x);
		float fCOSy = cosf(kEuler.y);
		float fSINy = sinf(kEuler.y);
		float fCOSz = cosf(kEuler.z);
		float fSINz = sinf(kEuler.z);

		_11 = (fCOSy * fCOSz);
		_12 = -(fCOSy * fSINz);
		_13 = (fSINy);

		float fSINxSINy = (fSINx * fSINy);
		float fCOSxSINy = (fCOSx * fSINy);

		_21 = ((fSINxSINy * fCOSz) + (fCOSx * fSINz));
		_22 = (-(fSINxSINy * fSINz) + (fCOSx * fCOSz));
		_23 = -(fSINx * fCOSy);

		_31 = (-(fCOSxSINy * fCOSz) + (fSINx * fSINz));
		_32 = ((fCOSxSINy * fSINz) + (fSINx * fCOSz));
		_33 = (fCOSx * fCOSy);
	}

	// Pitch : x, Yaw : y, Roll : z (z * x * y)
	void SVIMatrix::setYawPitchRoll(float fYaw, float fPitch, float fRoll)
	{
		setIdentity();
		float fCosX = cosf(fPitch);
		float fSinX = sinf(fPitch);
		float fCosY = cosf(fYaw);
		float fSinY = sinf(fYaw);
		float fCosZ = cosf(fRoll);
		float fSinZ = sinf(fRoll);

		_11 = fCosZ*fCosY + fSinZ*fSinX*fSinY;
		_12 = fSinZ*fCosX;
		_13 = fCosZ*-fSinY + fSinZ*fSinX*fCosY;

		_21 = -fSinZ*fCosY + fCosZ*fSinX*fSinY;
		_22 = fCosZ*fCosX;
		_23 = fSinZ*fSinY + fCosZ*fSinX*fCosY;

		_31 = fCosX*fSinY;
		_32 = -fSinX;
		_33 = fCosX*fCosY;
	}

	void SVIMatrix::getYawPitchRoll(float& fYaw, float& fPitch, float& fRoll)
	{
		fPitch = asinf(-_32);
		float fThreshold = 0.001f;
		float fTest = cosf(fPitch);
		if(fTest > fThreshold) {
			fRoll = atan2f(_12, _22);
			fYaw = atan2f(_31, _33);
		} else {
			fRoll = atan2f(-_21, _11);
			fYaw = 0.0f;
		}
	}

	// 2003 6 16
	bool SVIMatrix::getEuler(SVIVector3& kEuler)
	{
#define RAD2DEG(val) (val/0.01745329f)
		float rfXAngle, rfYAngle, rfZAngle;
		// rot =  cy*cz          -cy*sz           sy
		//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
		//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy
		if ( _13 < 1.0f ) {
			if ( _13 > -1.0f ) {
				rfXAngle = (float)atan2(-_23, _33);
				rfYAngle = (float)asinf(_13);
				rfZAngle = (float)atan2(-_12, _11);
				kEuler = SVIVector3(RAD2DEG(rfXAngle), RAD2DEG(rfYAngle), RAD2DEG(rfZAngle));
				return SVITRUE;
			}
			else {
				// WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
				rfXAngle = (float)-atan2(_21, _22);
				rfYAngle = (float)-(3.141592/2);
				rfZAngle = 0.0F;
				kEuler = SVIVector3(RAD2DEG(rfXAngle), RAD2DEG(rfYAngle), RAD2DEG(rfZAngle));
				return SVIFALSE;
			}
		}
		else {
			// WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
			rfXAngle = (float)atan2(_21, _22);
			rfYAngle = (float)(3.141592/2);
			rfZAngle = 0.0F;
			kEuler = SVIVector3(RAD2DEG(rfXAngle), RAD2DEG(rfYAngle), RAD2DEG(rfZAngle));
			return SVIFALSE;
		}
#undef RAD2DEG
		return SVIFALSE;
	}

	void SVIMatrix::setPosition(const SVIVector3 &position) {
		_41 = position.x;
		_42 = position.y;
		_43 = position.z;
	}


	void SVIMatrix::setRotation(const SVIVector3 &kAxis, float fAngle)
	{
		float fCOS = cosf(fAngle);
		float fSIN = sinf(fAngle);
		SVIVector3 knormal(((SVIVector3 &)kAxis).normal());

		_11 = ((knormal.x * knormal.x) * (1.0f - fCOS) + fCOS);
		_12 = ((knormal.x * knormal.y) * (1.0f - fCOS) - (knormal.z * fSIN));
		_13 = ((knormal.x * knormal.z) * (1.0f - fCOS) + (knormal.y * fSIN));

		_21 = ((knormal.y * knormal.x) * (1.0f - fCOS) + (knormal.z * fSIN));
		_22 = ((knormal.y * knormal.y) * (1.0f - fCOS) + fCOS);
		_23 = ((knormal.y * knormal.z) * (1.0f - fCOS) - (knormal.x * fSIN));

		_31 = ((knormal.z * knormal.x) * (1.0f - fCOS) - (knormal.y * fSIN));
		_32 = ((knormal.z * knormal.y) * (1.0f - fCOS) + (knormal.x * fSIN));
		_33 = ((knormal.z * knormal.z) * (1.0f - fCOS) + fCOS);
	}

	void SVIMatrix::setSVIQuaternion(const SVIQuaternion &kQuat)
	{
		float xx = kQuat.x*kQuat.x;
		float xy = kQuat.x*kQuat.y, yy = kQuat.y*kQuat.y;
		float xz = kQuat.x*kQuat.z, yz = kQuat.y*kQuat.z, zz = kQuat.z*kQuat.z;
		float xw = kQuat.x*kQuat.w, yw = kQuat.y*kQuat.w, zw = kQuat.z*kQuat.w, ww = kQuat.w*kQuat.w;
		_11 = +xx-yy-zz+ww; _12 = +xy+zw+xy+zw; _13 = +xz-yw+xz-yw;	_14 = 0.0f;
		_21 = +xy-zw+xy-zw; _22 = -xx+yy-zz+ww; _23 = +yz+xw+yz+xw;	_24 = 0.0f;
		_31 = +xz+yw+xz+yw; _32 = +yz-xw+yz-xw; _33 = -xx-yy+zz+ww; _34 = 0.0f;
		_41 = 0.0f;			_42 = 0.0f;			_43 = 0.0f;			_44 = 1.0f;
		
	}

	void SVIMatrix::transpose(void)
	{
		SVIMatrix kMatrix(*this);
		_11 = kMatrix._11;    _12 = kMatrix._21;    _13 = kMatrix._31;    _14 = kMatrix._41;
		_21 = kMatrix._12;    _22 = kMatrix._22;    _23 = kMatrix._32;    _24 = kMatrix._42;
		_31 = kMatrix._13;    _32 = kMatrix._23;    _33 = kMatrix._33;    _34 = kMatrix._43;
		_41 = kMatrix._14;    _42 = kMatrix._24;    _43 = kMatrix._34;    _44 = kMatrix._44;
	}

	SVIMatrix SVIMatrix::inverse4X4(void) const
	{
		SVIMatrix invMatrix;
		float tmp[12]; /* temp array for pairs */ 
		float src[16]; /* array of transpose source matrix */ 
		float det; /* determinant */ 
		for ( int i = 0; i < 4; i++) { 
			src[i] = d[i*4]; 
			src[i + 4] = d[i*4 + 1]; 
			src[i + 8] = d[i*4 + 2]; 
			src[i + 12] = d[i*4 + 3]; 
		} 

		/* transpose matrix */ 
		/* calculate pairs for first 8 elements (cofactors) */ 
		tmp[0] = src[10] * src[15]; 
		tmp[1] = src[11] * src[14]; 
		tmp[2] = src[9] * src[15]; 
		tmp[3] = src[11] * src[13]; 
		tmp[4] = src[9] * src[14]; 
		tmp[5] = src[10] * src[13]; 
		tmp[6] = src[8] * src[15]; 
		tmp[7] = src[11] * src[12]; 
		tmp[8] = src[8] * src[14]; 
		tmp[9] = src[10] * src[12]; 
		tmp[10] = src[8] * src[13]; 
		tmp[11] = src[9] * src[12]; 
		/* calculate first 8 elements (cofactors) */ 
		invMatrix.d[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7]; 
		invMatrix.d[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7]; 
		invMatrix.d[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7]; 
		invMatrix.d[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7]; 
		invMatrix.d[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7]; 
		invMatrix.d[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7]; 
		invMatrix.d[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6]; 
		invMatrix.d[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6]; 
		invMatrix.d[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3]; 
		invMatrix.d[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3]; 
		invMatrix.d[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3]; 
		invMatrix.d[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3]; 
		invMatrix.d[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3]; 
		invMatrix.d[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3]; 
		invMatrix.d[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2]; 
		invMatrix.d[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2]; 
		/* calculate pairs for second 8 elements (cofactors) */ 
		tmp[0] = src[2]*src[7]; 
		tmp[1] = src[3]*src[6]; 
		tmp[2] = src[1]*src[7]; 
		tmp[3] = src[3]*src[5]; 
		tmp[4] = src[1]*src[6]; 
		tmp[5] = src[2]*src[5]; 

		tmp[6] = src[0]*src[7]; 
		tmp[7] = src[3]*src[4]; 
		tmp[8] = src[0]*src[6]; 
		tmp[9] = src[2]*src[4]; 
		tmp[10] = src[0]*src[5]; 
		tmp[11] = src[1]*src[4]; 
		/* calculate second 8 elements (cofactors) */ 
		invMatrix.d[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15]; 
		invMatrix.d[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15]; 
		invMatrix.d[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15]; 
		invMatrix.d[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15]; 
		invMatrix.d[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15]; 
		invMatrix.d[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15]; 
		invMatrix.d[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14]; 
		invMatrix.d[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14]; 
		invMatrix.d[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9]; 
		invMatrix.d[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10]; 
		invMatrix.d[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10]; 
		invMatrix.d[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8]; 
		invMatrix.d[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8]; 
		invMatrix.d[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9]; 
		invMatrix.d[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9]; 
		invMatrix.d[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8]; 
		/* calculate determinant */ 
		det=src[0]*invMatrix.d[0]+src[1]*invMatrix.d[1]+src[2]*invMatrix.d[2]+src[3]*invMatrix.d[3]; 
		/* calculate matrix inverse */ 
		if(det == 0) det = 1;
		det = 1/det; 
		for ( int j = 0; j < 16; j++) invMatrix.d[j] *= det;

		return invMatrix;
	}

	SVIMatrix SVIMatrix::inverse(float* pdeterminant) const
	{
		/*	if((ABS(_44 - 1.0f) > 0.001f)||
		(ABS(_14) > 0.001f)||
		(ABS(_24) > 0.001f)||
		(ABS(_34) > 0.001f)) return *this;*/
		SVIMatrix kResult;
		float fdeterminant = determinant();
		if(pdeterminant) *pdeterminant = fdeterminant;
		float fDetinverse = (1.0f / fdeterminant);
		kResult._11 = ( fDetinverse * ((_22 * _33) - (_23 * _32)));
		kResult._12 = (-fDetinverse * ((_12 * _33) - (_13 * _32)));
		kResult._13 = ( fDetinverse * ((_12 * _23) - (_13 * _22)));
		kResult._14 = 0.0f;

		kResult._21 = (-fDetinverse * ((_21 * _33) - (_23 * _31)));
		kResult._22 = ( fDetinverse * ((_11 * _33) - (_13 * _31)));
		kResult._23 = (-fDetinverse * ((_11 * _23) - (_13 * _21)));
		kResult._24 = 0.0f;

		kResult._31 = ( fDetinverse * ((_21 * _32) - (_22 * _31)));
		kResult._32 = (-fDetinverse * ((_11 * _32) - (_12 * _31)));
		kResult._33 = ( fDetinverse * ((_11 * _22) - (_12 * _21)));
		kResult._34 = 0.0f;

		kResult._41 = -((_41 * kResult._11) + (_42 * kResult._21) + (_43 * kResult._31));
		kResult._42 = -((_41 * kResult._12) + (_42 * kResult._22) + (_43 * kResult._32));
		kResult._43 = -((_41 * kResult._13) + (_42 * kResult._23) + (_43 * kResult._33));
		kResult._44 = 1.0f;
		return kResult;
	}

	void SVIMatrix::invert(void)
	{
		*this = inverse();
	}

	SVIVector3 SVIMatrix::rotate(const SVIVector2 &kVector)
	{
		return SVIVector3(((kVector.x * _11) + (kVector.y * _21)),
			((kVector.x * _12) + (kVector.y * _22)),
			((kVector.x * _13) + (kVector.y * _23)));
	}

	SVIVector3 SVIMatrix::rotate(const SVIVector3 &kVector)
	{
		return SVIVector3(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31)),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32)),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33)));
	}

	SVIVector3 SVIMatrix::rotate(const SVIVector4 &kVector)
	{
		return SVIVector3(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31)),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32)),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33)));
	}

	SVIVector3 SVIMatrix::rotate(const SVIVector2 &kVector) const
	{
		return SVIVector3(((kVector.x * _11) + (kVector.y * _21)),
			((kVector.x * _12) + (kVector.y * _22)),
			((kVector.x * _13) + (kVector.y * _23)));
	}

	SVIVector3 SVIMatrix::rotate(const SVIVector3 &kVector) const
	{
		return SVIVector3(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31)),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32)),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33)));
	}

	SVIVector3 SVIMatrix::rotate(const SVIVector4 &kVector) const
	{
		return SVIVector3(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31)),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32)),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33)));
	}

	SVIVector4 SVIMatrix::transform(const SVIVector2 &kVector)
	{
		return SVIVector4(((kVector.x * _11) + (kVector.y * _21) + _41),
			((kVector.x * _12) + (kVector.y * _22) + _42),
			((kVector.x * _13) + (kVector.y * _23) + _43),
			((kVector.x * _14) + (kVector.y * _24) + _44));
	}

	SVIVector3 SVIMatrix::transform(const SVIVector3 &kVector)
	{
		SVIVector4 r(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31) + _41),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32) + _42),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33) + _43),
			((kVector.x * _14) + (kVector.y * _24) + (kVector.z * _34) + _44));
		float inv_w = 1.f / r.w;
		return SVIVector3(r.x*inv_w, r.y*inv_w, r.z*inv_w);
	}

	SVIVector4 SVIMatrix::transform(const SVIVector4 &kVector)
	{
		return SVIVector4(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31) + (kVector.w * _41)),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32) + (kVector.w * _42)),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33) + (kVector.w * _43)),
			((kVector.x * _14) + (kVector.y * _24) + (kVector.z * _34) + (kVector.w * _44)));
	}

	SVIVector4 SVIMatrix::transform(const SVIVector2 &kVector) const
	{
		return SVIVector4(((kVector.x * _11) + (kVector.y * _21) + _41),
			((kVector.x * _12) + (kVector.y * _22) + _42),
			((kVector.x * _13) + (kVector.y * _23) + _43),
			((kVector.x * _14) + (kVector.y * _24) + _44));
	}

	SVIVector3 SVIMatrix::transform(const SVIVector3 &kVector) const 
	{
		SVIVector4 r(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31) + _41),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32) + _42),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33) + _43),
			((kVector.x * _14) + (kVector.y * _24) + (kVector.z * _34) + _44));
		float inv_w = 1.f / r.w;
		return SVIVector3(r.x*inv_w, r.y*inv_w, r.z*inv_w);
	}

	SVIVector4 SVIMatrix::transform(const SVIVector4 &kVector) const
	{
		return SVIVector4(((kVector.x * _11) + (kVector.y * _21) + (kVector.z * _31) + (kVector.w * _41)),
			((kVector.x * _12) + (kVector.y * _22) + (kVector.z * _32) + (kVector.w * _42)),
			((kVector.x * _13) + (kVector.y * _23) + (kVector.z * _33) + (kVector.w * _43)),
			((kVector.x * _14) + (kVector.y * _24) + (kVector.z * _34) + (kVector.w * _44)));
	}
	void SVIMatrix::setProjection( float fFOV, float fAspect, float fNearSVIPlane, float fFarSVIPlane)
	{
		

		if( fabs(fFarSVIPlane-fNearSVIPlane) < 0.01f )	return ;
		if( fabs(sinf(fFOV/2)) < 0.001f )			return ;

		float w = fAspect * ( cosf(fFOV/2)/sinf(fFOV/2) );
		float h =   1.0f  * ( cosf(fFOV/2)/sinf(fFOV/2) );
		float Q = fFarSVIPlane / ( fFarSVIPlane - fNearSVIPlane );

		setIdentity();
		_11 = w;
		_22 = h;
		_33 = Q;
		_34 = 1.0f;
		_43 = -Q*fNearSVIPlane;
	}

	void SVIMatrix::setViewMatrix( SVIVector3& vFrom, SVIVector3& vAt, SVIVector3& vWorldUp)
	{
		SVIVector3 vView = vAt - vFrom;

		float flength = Magnitude(vView);

		if (flength < 1e-6f) return ;

		vView /= flength;

		float fdotProduct = dotProduct(vWorldUp, vView);

		SVIVector3 vUp = vWorldUp - vView*fdotProduct ;


		if (1e-6f > (flength = Magnitude(vUp)))
		{
			vUp = SVIVector3(0.0f, 1.0f, 0.0f) -  vView*vView.y ;

			if (1e-6f > (flength = Magnitude(vUp)))
			{
				vUp = SVIVector3(0.0f, 0.0f, 1.0f) -  vView*vView.z ;

				if (1e-6f > (flength = Magnitude(vUp)))
					//return E_INVALIDARG;
					return ;
			}
		}

		vUp /= flength;

		SVIVector3 vRight=vUp.Cross(vView);

		setIdentity();

		_11 = vRight.x; _12 = vUp.x; _13 = vView.x;
		_21 = vRight.y; _22 = vUp.y; _23 = vView.y;
		_31 = vRight.z; _32 = vUp.z; _33 = vView.z;

		_41 = - vFrom.dot(vRight);
		_42 = - vFrom.dot(vUp);
		_43 = - vFrom.dot(vView);
	}


	void SVIMatrix::debugMatrix(const SVIChar * tag,SVIMatrix * mat){
		DEBUG_CHECK_LOGI("Matrix %s----------\n%.03f %.03f %.03f %.03f\n%.03f %.03f %.03f %.03f\n%.03f %.03f %.03f %.03f\n %.03f %.03f %.03f %.03f\n",
			tag,
			mat->_11,mat->_12,mat->_13,mat->_14,
			mat->_21,mat->_22,mat->_23,mat->_24,
			mat->_31,mat->_32,mat->_33,mat->_34,
			mat->_41,mat->_42,mat->_43,mat->_44);

	}

	SVIMatrix SVIMatrix::ortho(float fXMin, float fYMin, float fXMax, float fYMax, float fNear, float fFar)
	{

		float fX = (2.0f / (fXMax - fXMin));
		float fY = (2.0f / (fYMax - fYMin));
		float fZ = (1.0f / (fFar - fNear));
		float fTX = ((fXMax + fXMin) / (fXMin - fXMax));
		float fTY = ((fYMax + fYMin) / (fYMin - fYMax));
		float fTZ = ((fNear) / (fNear - fFar));

		return SVIMatrix(	fX, 0.0f, 0.0f, 0.0f,
			0.0f,  fY, 0.0f, 0.0f,
							0.0f, 0.0f,  fZ, 0.0f,
							fTX, fTY, fTZ, 1.0f);
	}

	SVIMatrix SVIMatrix::perspective(float fFOV, float fAspect, float fNear, float fFar)
	{

		float fYMax = (fNear * (sinf(fFOV / 2.0f) / cosf(fFOV / 2.0f)));
		float fXMax = (fYMax * fAspect);
		float fYMin = -fYMax;
		float fXMin = -fXMax;

		float fX = ((2.0f * fNear) / (fXMax - fXMin));
		float fY = ((2.0f * fNear) / (fYMax - fYMin));
		float fA = ((fXMax + fXMin) / (fXMax - fXMin));
		float fB = ((fYMax + fYMin) / (fYMax - fYMin));
		float fC = -((fFar + fNear) / (fFar - fNear));
		float fD = -((2.0f * fFar * fNear) / (fFar - fNear));

		return SVIMatrix(  fX, 0.0f,   fA, 0.0f,
						0.0f,   fY,   fB, 0.0f,
						0.0f, 0.0f,  -fC,  -fD,
						0.0f, 0.0f,-1.0f, 0.0f);

	}

	SVIMatrix SVIMatrix::mirror(const SVIPlane &kSVIPlane)
	{
		float fX = kSVIPlane.n.x;
		float fY = kSVIPlane.n.y;
		float fZ = kSVIPlane.n.z;
		float fD = kSVIPlane.d;

		return SVIMatrix((0.0f - (2.0f * fX * fX)), (0.0f - (2.0f * fX * fY)), (0.0f - (2.0f * fX * fZ)), 0.0f,
			(0.0f - (2.0f * fY * fX)), (0.0f - (2.0f * fY * fY)), (0.0f - (2.0f * fY * fZ)), 0.0f,
			(0.0f - (2.0f * fZ * fX)), (0.0f - (2.0f * fZ * fY)), (0.0f - (2.0f * fZ * fZ)), 0.0f,
			(0.0f - (2.0f * fD * fX)), (0.0f - (2.0f * fD * fY)), (0.0f - (2.0f * fD * fZ)), 1.0f);
	}

	SVIMatrix SVIMatrix::target(const SVIVector3 &kOrigin, const SVIVector3 &kTarget)
	{
		SVIVector3 kZAxis(kTarget - kOrigin);
		kZAxis.normalize();

		SVIVector3 m_kYAxis(0.0f, 1.0f, 0.0f);
		SVIVector3 kXAxis(m_kYAxis.Cross(kZAxis));
		kXAxis.normalize();

		SVIVector3 kYAxis(kZAxis.Cross(kXAxis));

		return SVIMatrix(kXAxis.x, kXAxis.y, kXAxis.z, -kXAxis.dot(kOrigin),
			kYAxis.x, kYAxis.y, kYAxis.z, -kYAxis.dot(kOrigin),
			kZAxis.x, kZAxis.y, kZAxis.z, -kZAxis.dot(kOrigin),
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	SVIMatrix SVIMatrix::perspectiveLH(float w, float h, float zn, float zf)
	{
		return SVIMatrix(
			2*zn/w, 0,		0,				0,
			0,		2*zn/h, 0,				0, 
			0,		0,		zf/(zf-zn),		1,
			0,		0,		zn*zf/(zn-zf),	0
			);

	}

	SVIMatrix SVIMatrix::orthoLH(float w, float h, float zn, float zf) 
	{

		return SVIMatrix(
			2/w,	0,		0,				0,
			0,		2/h,	0,				0,
			0,		0,		1/(zf-zn),		0,
			0,		0,		zn/(zn-zf),		1
			);

	}

	SVIMatrix SVIMatrix::perspectiveOffCenterLH(float l, float r, float b, float t, float zn, float zf)
	{

		return SVIMatrix(
			2.0f*zn/(r-l),	0,				0,				0,
			0,				2*zn/(t-b),		0,				0,
			(l+r)/(l-r),	(t+b)/(b-t),	zf/(zf-zn),		1,
			0,				0,				zn*zf/(zn-zf),	0
			);

	}

	SVIMatrix SVIMatrix::fromPositionDirectionUp( const SVIVector3& Position, const SVIVector3& Direction, const SVIVector3& Up ){
		SVIVector3 NormDir = Direction.normal();

		SVIVector3 NormUp = Up.normal();

		// get X basis vector
		SVIVector3 X = NormUp.Cross(NormDir);

		// get Y basis vector
		SVIVector3 Y = NormDir.Cross(X);

		return SVIMatrix( X.x, X.y, X.z, 0.0f,
			Y.x, Y.y, Y.z, 0.0f,
			NormDir.x, NormDir.y, NormDir.z, 0.0f,
			Position.x, Position.y, Position.z, 1.0f );
	}

	SVIMatrix::SVIMatrix(void)
	{
		setIdentity();
	}

	SVIMatrix::SVIMatrix(float *pfData)
	{
		_11 = pfData[ 0];	_12 = pfData[ 1];	_13 = pfData[ 2];	_14 = pfData[ 3];
		_21 = pfData[ 4];	_22 = pfData[ 5];	_23 = pfData[ 6];	_24 = pfData[ 7];
		_31 = pfData[ 8];	_32 = pfData[ 9];	_33 = pfData[10];	_34 = pfData[11];
		_41 = pfData[12];	_42 = pfData[13];	_43 = pfData[14];	_44 = pfData[15];
	}

	SVIMatrix::SVIMatrix(float _11, float _12, float _13, float _14, 
								float _21, float _22, float _23, float _24,
								float _31, float _32, float _33, float _34,
								float _41, float _42, float _43, float _44)
	{
		this->_11 = _11;	this->_12 = _12;	this->_13 = _13;	this->_14 = _14;
		this->_21 = _21;	this->_22 = _22;	this->_23 = _23;	this->_24 = _24;
		this->_31 = _31;	this->_32 = _32;	this->_33 = _33;	this->_34 = _34;
		this->_41 = _41;	this->_42 = _42;	this->_43 = _43;	this->_44 = _44;
	}

	SVIMatrix::SVIMatrix(const SVIMatrix &kMatrix)
	{
		_11 = kMatrix._11;	_12 = kMatrix._12;	_13 = kMatrix._13;	_14 = kMatrix._14;
		_21 = kMatrix._21;	_22 = kMatrix._22;	_23 = kMatrix._23;	_24 = kMatrix._24;
		_31 = kMatrix._31;	_32 = kMatrix._32;	_33 = kMatrix._33;	_34 = kMatrix._34;
		_41 = kMatrix._41;	_42 = kMatrix._42;	_43 = kMatrix._43;	_44 = kMatrix._44;
	}

	SVIMatrix::SVIMatrix(const SVIVector3 &kAxis, float fAngle)
	{
		setIdentity();
		setRotation(kAxis, fAngle);
	}

	SVIMatrix::SVIMatrix(const SVIVector3 &kEuler)
	{
		setIdentity();
		setEuler(kEuler);
	}

	const SVIMatrix& SVIMatrix::operator = (const SVIMatrix &kMatrix)
	{
		_11 = kMatrix._11;	_12 = kMatrix._12;	_13 = kMatrix._13;	_14 = kMatrix._14;
		_21 = kMatrix._21;	_22 = kMatrix._22;	_23 = kMatrix._23;	_24 = kMatrix._24;
		_31 = kMatrix._31;	_32 = kMatrix._32;	_33 = kMatrix._33;	_34 = kMatrix._34;
		_41 = kMatrix._41;	_42 = kMatrix._42;	_43 = kMatrix._43;	_44 = kMatrix._44;
		return *this;
	}

	float SVIMatrix::operator () (int iRow, int iCol) const
	{
		return m[iRow][iCol];
	}

	float SVIMatrix::operator () (int iRow, int iCol)
	{
		return m[iRow][iCol];
	}

	SVIMatrix::operator const float * (void) const
	{
		return &_11;
	}

	SVIMatrix::operator float * (void)
	{
		return &_11;
	}

	void SVIMatrix::setIdentity(void)
	{
		_11 = _22 = _33 = _44 = 1.0f;
		_12 = _13 = _14 = 0.0f;
		_21 = _23 = _24 = 0.0f;
		_31 = _32 = _34 = 0.0f;
		_41 = _42 = _43 = 0.0f;
	}

	void SVIMatrix::setScaling(const SVIVector3 &kScale)
	{
		_11 = kScale.x;
		_22 = kScale.y;
		_33 = kScale.z;
	}

	SVIVector3 SVIMatrix::getScaling(void) const
	{
		return SVIVector3(getAxisX().getlength(), getAxisY().getlength(), getAxisZ().getlength());
	}

	float SVIMatrix::determinant(void) const
	{
		return ((_11 * ((_22 * _33) - (_23 * _32))) -
				(_12 * ((_21 * _33) - (_23 * _31))) +
				(_13 * ((_21 * _32) - (_22 * _31))));
	}
};