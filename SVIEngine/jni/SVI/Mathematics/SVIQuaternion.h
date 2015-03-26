#pragma once

namespace SVI{


struct SVIVector2;
struct SVIVector3;
struct SVIVector4;
struct SVIPlane;
struct SVIMatrix;


struct SVIQuaternion
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	SVIQuaternion(void);
	SVIQuaternion(const SVIMatrix &kMatrix);
	SVIQuaternion(const SVIQuaternion &kQuat);
	SVIQuaternion(float fX, float fY, float fZ, float fW);
	SVIQuaternion(const SVIVector3 &kAxis, float fAngle);
	SVIQuaternion(const SVIVector3 &kEuler);
	SVIQuaternion(const SVIVector4 &kQuat);
	SVIQuaternion(const float* pQuat);
 	operator float * (void);

	
	bool operator == (const SVIQuaternion &kQuaternion) const;
	bool operator != (const SVIQuaternion &kQuaternion) const;
	void operator *= (const SVIQuaternion &kQuat);
	const SVIQuaternion& operator = (const SVIQuaternion &kQuat);
	int operator -= (const SVIQuaternion &kQuat);
	int operator += (const SVIQuaternion &kQuat);
	int operator /= (float fScalar);
	int operator *= (float fScalar);
	
	SVIQuaternion operator * (const SVIQuaternion &kQuat);
	SVIQuaternion operator - (const SVIQuaternion &kQuat);
	SVIQuaternion operator + (const SVIQuaternion &kQuat);
	SVIQuaternion operator / (float fScalar);
	SVIQuaternion operator * (float fScalar);
	

	void setIdentity(void);								
	void setEuler(const SVIVector3 &kEuler);					
	void setRotation(const SVIVector3 &kAxis, float fAngle);	
	void setMatrix(const SVIMatrix &kMatrix);					
	
	float dot(const SVIQuaternion &kQuat) const;				
	float lengthSqr(void);								
	float length(void);									

	SVIQuaternion normal(void);									
	void normalize(void);										

	SVIQuaternion Slerp(const SVIQuaternion &kQuat, float fFactor) const;	
	SVIQuaternion lerp(const SVIQuaternion &kQuat, float fFactor) const;	
};
}