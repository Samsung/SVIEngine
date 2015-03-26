#pragma once

namespace SVI{
	

struct SVIVector2;
struct SVIVector3;
struct SVIVector4;
struct SVIPlane;
struct SVIQuaternion;

struct SVIMatrix
{

	
public:
	union 
	{
		float d[16];
		float m[4][4];
		
		struct {
			float _11, _12, _13, _14;	
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};		
	};

public:
	SVIMatrix(void);
	SVIMatrix(float *pfData);
	SVIMatrix(	float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44);
	SVIMatrix(const SVIMatrix &kMatrix);
	SVIMatrix(const SVIVector3 &kAxis, float fAngle);
	SVIMatrix(const SVIVector3 &kEuler);

	SVIVector3 getAxisX(void) const { return SVIVector3(_11,_12,_13); };
	SVIVector3 getAxisY(void) const { return SVIVector3(_21,_22,_23); };
	SVIVector3 getAxisZ(void) const { return SVIVector3(_31,_32,_33); };
	SVIVector3 getAxisW(void) const { return SVIVector3(_41,_42,_43); };


	const SVIMatrix& operator = (const SVIMatrix &kMatrix);
	void operator *= (const SVIMatrix &kMatrix);
	void operator *= (float fVal);
	
	bool operator==(const SVIMatrix& kMatrix) const;
	bool operator!=(const SVIMatrix& kMatrix) const;	

	float operator () (int iRow, int iCol) const;
	float operator () (int iRow, int iCol);
	operator const float * (void) const;
	operator float * (void);

	SVIMatrix operator * (const SVIMatrix &kMatrix);
	SVIMatrix operator * (const SVIMatrix &kMatrix) const;
	SVIMatrix operator * (float fVal);
	SVIMatrix operator * (float fVal) const;

	void setIdentity(void);								
	void setEuler(const SVIVector3 &kEuler);					
	bool getEuler(SVIVector3& kEuler);

	void setYawPitchRoll(float fYaw, float fPitch, float fRoll);
	void getYawPitchRoll(float& fYaw, float& fPitch, float& fRoll);

	void setPosition(const SVIVector3 &position);
	void setRotation(const SVIVector3 &kAxis, float fAngle);	
	void setSVIQuaternion(const SVIQuaternion &kQuat);				

	void setScaling(const SVIVector3 &kScale);			
	SVIVector3 getScaling(void) const;							

	void transpose(void);										
	float determinant(void) const;								
	SVIMatrix inverse(float* pdeterminant = 0) const;									
	SVIMatrix inverse4X4(void) const;									
	void invert(void);											

	void rotate(const SVIMatrix &kMatrix);

	SVIVector3 rotate(const SVIVector2 &kVector);
	SVIVector3 rotate(const SVIVector3 &kVector);
	SVIVector3 rotate(const SVIVector4 &kVector);
	SVIVector3 rotate(const SVIVector2 &kVector) const;
	SVIVector3 rotate(const SVIVector3 &kVector) const;
	SVIVector3 rotate(const SVIVector4 &kVector) const;

	SVIVector4 transform(const SVIVector2 &kVector);
	SVIVector3 transform(const SVIVector3 &kVector);
	SVIVector4 transform(const SVIVector4 &kVector);
	SVIVector4 transform(const SVIVector2 &kVector) const;
	SVIVector3 transform(const SVIVector3 &kVector) const;
	SVIVector4 transform(const SVIVector4 &kVector) const;


	void setViewMatrix( SVIVector3& vFrom, SVIVector3& vAt, SVIVector3& vWorldUp);
	void setProjection( float fFOV, float fAspect, float fNearSVIPlane=10.f, float fFarSVIPlane=10000.0f);

	static void debugMatrix(const SVIChar * tag,SVIMatrix * mat);
	
	static SVIMatrix ortho(float fXMin, float fYMin, float fXMax, float fYMax, float fNear, float fFar);
	static SVIMatrix perspective(float fFOV, float fAspect, float fNear, float fFar);
	static SVIMatrix mirror(const SVIPlane &kSVIPlane);
	static SVIMatrix target(const SVIVector3 &kOrigin, const SVIVector3 &kTarget);
	static SVIMatrix perspectiveLH(float w, float h, float zn, float zf);
	static SVIMatrix orthoLH(float w, float h, float zn, float zf);
	static SVIMatrix perspectiveOffCenterLH(float l, float r, float b, float t, float zn, float zf);
	static SVIMatrix fromPositionDirectionUp( const SVIVector3& Position, const SVIVector3& Direction, const SVIVector3& Up );
};


}
