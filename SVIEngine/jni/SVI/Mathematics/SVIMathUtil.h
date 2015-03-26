#pragma once

namespace SVI{
		
	struct SVIVector2;
	struct SVIVector3;
	struct SVIVector4;
	struct SVIMatrix;
	struct SVIQuaternion;
	struct SVIBoundingBox;

	namespace Util{
		SVIVector3&   getXBasis( SVIMatrix& Matrix );
		SVIVector3&   getYBasis( SVIMatrix& Matrix );
		SVIVector3&   getZBasis( SVIMatrix& Matrix ); 
		SVIVector3&   getWBasis( SVIMatrix& Matrix ); 


		void			getRadianFromVector( SVIVector3& Direction, SVIFloat& fAngleXY, SVIFloat& fAngleXZ );
		SVIVector2		getScreenSpaceCoord( SVIMatrix mView, SVIMatrix mProj, SVIVector3 Pos, long w, long h);
		SVIQuaternion    rotationBetweenVectors( SVIVector3& VectorA, SVIVector3& VectorB );
		SVIVector3       orthogonalVector( SVIVector3& Vector );
		void			simpleDecomposeMatrix( SVIMatrix * pMatrix, SVIFloat * pScale, SVIQuaternion* pRotation, SVIVector3* pTranslation );

		void computeTangentSpace(const SVIFloat* afVertex1, const SVIFloat* afVertex2, const SVIFloat* afVertex3,
			const SVIFloat* afTexCoord1, const SVIFloat* afTexCoord2, const SVIFloat* afTexCoord3,
			SVIFloat* afTangent, SVIFloat* afBinormal); 
	}

	namespace Intersection{

		SVIBool intersectSphereLine(const SVIVector3* pCenter, SVIFloat fRadius, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, const SVIVector3* pPoint);

		SVIBool intersectBoxLine(const SVIBoundingBox* pBox, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, SVIVector3* pPoint);

		SVIInt intersectBoxLineSide(const SVIBoundingBox* pBox, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, const SVIVector3* pPoint);

		SVIBool intersectPlaneLine(const SVIPlane* pPlane, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, SVIVector3* pPoint, SVIBool TwoSided = SVITRUE);

		SVIBool intersectBoxBox(const SVIBoundingBox* pBox0, const SVIBoundingBox* pBox1);


		SVIBool insideBoxPoint(const SVIBoundingBox* pBox, const SVIVector3* pPoint);
	}

}