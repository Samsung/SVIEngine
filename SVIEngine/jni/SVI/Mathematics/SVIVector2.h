#pragma once

namespace SVI{
	
struct SVIVector3;
struct SVIVector4;
struct SVIPlane;
struct SVIMatrix;
struct SVIQuaternion;

struct SVIVector2
{
public:
	union
	{
		float xy[2];
		float uv[2];
		struct { float x, y; };
		struct { float u, v; };
	};

public:
	 SVIVector2(void);
	 SVIVector2(const SVIVector2 &kVector);
	 SVIVector2(const SVIVector3 &kVector);
	 SVIVector2(const SVIVector4 &kVector);
	 SVIVector2(float fX, float fY);

	 float &operator [] (int iAxis);
	 bool operator < (const SVIVector2 &kVector) const;
	 bool operator > (const SVIVector2 &kVector) const;
	 bool operator == (const SVIVector2 &kVector) const;
	 bool operator != (const SVIVector2 &kVector) const;
	
	 operator float * (void);
	 operator const float * (void) const;

	 const SVIVector2& operator = (const SVIVector2 &kVector);
	 const SVIVector2& operator = (const SVIVector3 &kVector);
	 const SVIVector2& operator = (const SVIVector4 &kVector);
	 int operator -= (const SVIVector2 &kVector);
	 int operator += (const SVIVector2 &kVector);
	 int operator /= (float fScalar);
	 int operator *= (float fScalar);
	 	
	 SVIVector2 operator - (const SVIVector2 &kVector) const;
	 SVIVector2 operator + (const SVIVector2 &kVector) const;
	 SVIVector2 operator - (const SVIVector2 &kVector);
	 SVIVector2 operator + (const SVIVector2 &kVector);
	 SVIVector2 operator / (float fScalar) const;
	 SVIVector2 operator * (float fScalar) const;
	 SVIVector2 operator / (float fScalar);
	 SVIVector2 operator * (float fScalar);


	 void setlength(float flength);					
	 float getlengthSqr(void) const;					
	 float getlengthSqr(void);						
	 float getlength(void) const;						
	 float getlength(void);							

	 float getAngle(const SVIVector2 &kVector);		
	 float distance(const SVIVector2 &kVector) const;	
	 float distance(const SVIVector2 &kVector);		
	 float dot(const SVIVector2 &kVector) const;	
	 float dot(const SVIVector2 &kVector);			

	 SVIVector2 lerp(const SVIVector2 &kVector, float fFactor) const;	
	 SVIVector2 lerp(const SVIVector2 &kVector, float fFactor);		
	 SVIVector2 normal(void) const;									
	 SVIVector2 normal(void);											
	 float normalize(void);											
};

 SVIVector2 operator - (const SVIVector2 &kVector);	
 SVIVector2 operator - (SVIVector2 &kVector);			
 SVIVector2 operator * (float fScalar, const SVIVector2& kVector);

}
