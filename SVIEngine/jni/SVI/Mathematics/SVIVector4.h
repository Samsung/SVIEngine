#pragma once

namespace SVI{
	
struct SVIVector2;
struct SVIVector3;
struct SVIPlane;
struct SVIMatrix;
struct SVIQuaternion;


struct SVIVector4
{
public:
	union
	{
		float xyzw[4];
		float rgba[4];
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
	};

public:
	 SVIVector4(void);
	 SVIVector4(const SVIVector2 &kVector);
	 SVIVector4(const SVIVector3 &kVector);
	 SVIVector4(const SVIVector4 &kVector);
	 SVIVector4(float fX, float fY, float fZ, float fW);
	 SVIVector4(const SVIQuaternion & kQuat);

	 float &operator [] (int iAxis);
	 bool operator < (const SVIVector4 &kVector) const;
	 bool operator > (const SVIVector4 &kVector) const;

	 bool operator == (const SVIVector4 &kVector) const;
	 bool operator != (const SVIVector4 &kVector) const;
	
	 operator float * (void);
	 operator const float * (void) const;

	 const SVIVector4& operator = (const SVIVector2 &kVector);
	 const SVIVector4& operator = (const SVIVector3 &kVector);
	 const SVIVector4& operator = (const SVIVector4 &kVector);
	 int operator -= (const SVIVector4 &kVector);
	 int operator += (const SVIVector4 &kVector);
	 int operator /= (float fScalar);
	 int operator *= (float fScalar);

	 SVIVector4 operator - (const SVIVector4 &kVector) const;
	 SVIVector4 operator + (const SVIVector4 &kVector) const;
	 SVIVector4 operator - (const SVIVector4 &kVector);
	 SVIVector4 operator + (const SVIVector4 &kVector);
	 SVIVector4 operator / (float fScalar) const;
	 SVIVector4 operator * (float fScalar) const;
	 SVIVector4 operator / (float fScalar);
	 SVIVector4 operator * (float fScalar);
	

	 void setlength(float flength);					
	 float getlengthSqr(void) const;					
	 float getlengthSqr(void);							
	 float getlength(void) const;							
	 float getlength(void);								

	 float getAngle(const SVIVector4 &kVector);		
	 SVIVector4 Cross(const SVIVector4 &kVector) const;	
	 SVIVector4 Cross(const SVIVector4 &kVector);			
	 float distance(const SVIVector4 &kVector) const;		
	 float distance(const SVIVector4 &kVector);			
	 float dot(const SVIVector4 &kVector) const;			
	 float dot(const SVIVector4 &kVector);				

	 SVIVector4 lerp(const SVIVector4 &kVector, float fFactor) const;	
	 SVIVector4 lerp(const SVIVector4 &kVector, float fFactor);		
	 SVIVector4 normal(void) const;									
	 SVIVector4 normal(void);											
	 float normalize(void);													
};

 SVIVector4 operator - (const SVIVector4 &kVector);	
 SVIVector4 operator - (SVIVector4 &kVector);		

}