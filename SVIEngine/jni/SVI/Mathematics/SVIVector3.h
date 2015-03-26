#pragma once

namespace SVI{

	
struct SVIVector2;
struct SVIVector4;
struct SVIPlane;
struct SVIMatrix;
struct SVIQuaternion;


struct SVIVector3
{
public:
	union
	{
		float xyz[3];
		struct { float x, y, z; };
	};

public:
	SVIVector3(void);
	SVIVector3(const SVIVector2 &kVector);
	SVIVector3(const SVIVector3 &kVector);
	SVIVector3(const SVIVector4 &kVector);
	SVIVector3(float fX, float fY, float fZ);
	SVIVector3(float* pV);
	SVIVector3(const float* pV);

	float &operator [] (int iAxis);
	bool operator < (const SVIVector3 &kVector) const;
	bool operator > (const SVIVector3 &kVector) const;

	operator float * (void); 
	operator const float * (void) const; 

	bool operator==(const SVIVector3& kVector) const;
	bool operator!=(const SVIVector3& kVector) const;

public:
	const SVIVector3& operator = (const SVIVector2 &kVector);
	const SVIVector3& operator = (const SVIVector3 &kVector);
	const SVIVector3& operator = (const SVIVector4 &kVector);
	int operator -= (const SVIVector3 &kVector);
	int operator += (const SVIVector3 &kVector);
	int operator /= (float fScalar);
	int operator *= (float fScalar);

	SVIVector3 operator - (const SVIVector3 &kVector) const;
	SVIVector3 operator + (const SVIVector3 &kVector) const;
	SVIVector3 operator - (const SVIVector3 &kVector);
	SVIVector3 operator + (const SVIVector3 &kVector);

	SVIVector3 operator * (const SVIVector3 &kVector);

	SVIVector3 operator / (float fScalar) const;
	SVIVector3 operator * (float fScalar) const;
	SVIVector3 operator / (float fScalar);
	SVIVector3 operator * (float fScalar);


	void setlength(float flength);						
	float getlengthSqr(void) const;						
	float getlengthSqr(void);							
	float getlength(void) const;							
	float getlength(void);								

	float getAngle(const SVIVector3 &kVector);		
	SVIVector3 Cross(const SVIVector3 &kVector) const;	
	SVIVector3 Cross(const SVIVector3 &kVector);			
	float distance(const SVIVector3 &kVector) const;		
	float distance(const SVIVector3 &kVector);			
	float dot(const SVIVector3 &kVector) const;			
	float dot(const SVIVector3 &kVector);				

	SVIVector3 lerp(const SVIVector3 &kVector, float fFactor) const;		
	SVIVector3 lerp(const SVIVector3 &kVector, float fFactor);			
	SVIVector3 normal(void) const;										
	SVIVector3 normal(void);												
	float normalize(void);												
};

SVIVector3 operator - (const SVIVector3 &kVector);	
SVIVector3 operator - (SVIVector3 &kVector);			
float	SquareMagnitude (const SVIVector3& v);
float	Magnitude (const SVIVector3& v);
float	dotProduct (const SVIVector3& v1, const SVIVector3& v2);

 
}