#include "SVIMath.h"

//2011-11-04 masterkeaton27@gmail.commath utils.

namespace SVI{
	namespace Util{
		SVIVector3&   getXBasis( SVIMatrix& Matrix ) { return *(SVIVector3*)&((SVIFloat*)(&Matrix))[0*4]; }
		SVIVector3&   getYBasis( SVIMatrix& Matrix ) { return *(SVIVector3*)&((SVIFloat*)(&Matrix))[1*4]; }
		SVIVector3&   getZBasis( SVIMatrix& Matrix ) { return *(SVIVector3*)&((SVIFloat*)(&Matrix))[2*4]; }
		SVIVector3&   getWBasis( SVIMatrix& Matrix ) { return *(SVIVector3*)&((SVIFloat*)(&Matrix))[3*4]; }

		void getRadianFromVector( SVIVector3& Direction, SVIFloat& fAngleXY, SVIFloat& fAngleXZ ){
			SVIFloat fPolar = acosf(-Direction.y);
			SVIFloat fValue = Direction.x*Direction.x+Direction.z*Direction.z;
			if(fValue > 0.0001f) {
				fValue = Direction.z / sqrtf(fValue);
			} else {

			}
			SVIFloat fAzimuthal = ((Direction.x >= 0.0f) ? 1.0f : -1.0f) * acosf(fValue);
			fAngleXY = fPolar + SVI_PI*0.5f;
			fAngleXZ = fAzimuthal;
		}

		SVIVector2	getScreenSpaceCoord( SVIMatrix mView, SVIMatrix mProj, SVIVector3 Pos, long w, long h){
			long ClipWidth = w/2;
			long ClipHeight = h/2;

			SVIMatrix mResult = mView * mProj;

			SVIVector4 v;

			v = mResult.transform(SVIVector4(Pos.x,Pos.y,Pos.z,1.0f));

			v.x = (1.0f + (v.x/v.w))*ClipWidth;
			v.y = (1.0f - (v.y/v.w))*ClipHeight;

			return SVIVector2(v.x,v.y);
		}

		SVIQuaternion   rotationBetweenVectors( SVIVector3& VectorA, SVIVector3& VectorB ){
			SVIVector3 Axis;
			Axis = VectorA.Cross(VectorB);

			SVIFloat cosAngle = VectorA.dot(VectorB);

			SVIQuaternion quat;
			quat.setRotation(Axis, acosf( cosAngle ));

			return quat;
		}

		SVIVector3       orthogonalVector( SVIVector3& vector ){
			SVIVector3 Temp( vector .z, vector .x, vector .y );
			Temp = vector.Cross(Temp);
			return Temp;
		}

		void simpleDecomposeMatrix( SVIMatrix * pMatrix, SVIFloat * pScale, SVIQuaternion* pRotation, SVIVector3* pTranslation ){
			SVIMatrix Temp = *pMatrix;

			SVIVector3 Translation = getWBasis( Temp );
			getWBasis( Temp ) = SVIVector3( 0.0f, 0.0f, 0.0f );

			SVIFloat Scale = getZBasis( Temp ).getlength();
			SVIFloat InvScale = 1.0f / Scale;

			Temp._11 *= InvScale;
			Temp._12 *= InvScale;
			Temp._13 *= InvScale;
			Temp._14 *= InvScale;

			Temp._21 *= InvScale;
			Temp._22 *= InvScale;
			Temp._23 *= InvScale;
			Temp._24 *= InvScale;

			Temp._31 *= InvScale;
			Temp._32 *= InvScale;
			Temp._33 *= InvScale;
			Temp._34 *= InvScale;

			Temp._41 *= InvScale;
			Temp._42 *= InvScale;
			Temp._43 *= InvScale;
			Temp._44 *= InvScale;

			SVIQuaternion Rotation;

			Rotation.setMatrix(Temp);
			Rotation.normalize();

			if( pTranslation )
				*pTranslation = Translation;

			if( pScale )
				*pScale = Scale;

			if( pRotation )
				*pRotation = Rotation;
		}


		void computeTangentSpace(const SVIFloat* afVertex1, const SVIFloat* afVertex2, const SVIFloat* afVertex3,
			const SVIFloat* afTexCoord1, const SVIFloat* afTexCoord2, const SVIFloat* afTexCoord3,
			SVIFloat* afTangent, SVIFloat* afBinormal){
			const SVIFloat fTolerence = 1e-8f;
			SVIFloat afEdge1[3], afEdge2[3], afCrossP[3];

			afEdge1[1] = afTexCoord2[0] - afTexCoord1[0];
			afEdge1[2] = afTexCoord2[1] - afTexCoord1[1];

			afEdge2[1] = afTexCoord3[0] - afTexCoord1[0];
			afEdge2[2] = afTexCoord3[1] - afTexCoord1[1];

			afCrossP[0] = afEdge1[1]*afEdge2[2] - afEdge1[2]*afEdge2[1];
			if(fabsf(afCrossP[0]) <= fTolerence) return;

			SVIFloat fTan, fBi;

			afEdge1[0] = afVertex2[0] - afVertex1[0];
			afEdge2[0] = afVertex3[0] - afVertex1[0];

			afCrossP[1] = afEdge1[2]*afEdge2[0] - afEdge1[0]*afEdge2[2];
			afCrossP[2] = afEdge1[0]*afEdge2[1] - afEdge1[1]*afEdge2[0];
			fTan = -afCrossP[1]/afCrossP[0];
			fBi = -afCrossP[2]/afCrossP[0];
			afTangent[0] = fTan;
			afBinormal[0] = fBi;

			afEdge1[0] = afVertex2[1] - afVertex1[1];
			afEdge2[0] = afVertex3[1] - afVertex1[1];

			afCrossP[1] = afEdge1[2]*afEdge2[0] - afEdge1[0]*afEdge2[2];
			afCrossP[2] = afEdge1[0]*afEdge2[1] - afEdge1[1]*afEdge2[0];
			fTan = -afCrossP[1]/afCrossP[0];
			fBi = -afCrossP[2]/afCrossP[0];
			afTangent[1] = fTan;
			afBinormal[1] = fBi;

			afEdge1[0] = afVertex2[2] - afVertex1[2];
			afEdge2[0] = afVertex3[2] - afVertex1[2];

			afCrossP[1] = afEdge1[2]*afEdge2[0] - afEdge1[0]*afEdge2[2];
			afCrossP[2] = afEdge1[0]*afEdge2[1] - afEdge1[1]*afEdge2[0];
			fTan = -afCrossP[1]/afCrossP[0];
			fBi = -afCrossP[2]/afCrossP[0];
			afTangent[2] = fTan;
			afBinormal[2] = fBi;

			SVIFloat fLength = afTangent[0]*afTangent[0]+afTangent[1]*afTangent[1]+afTangent[2]*afTangent[2];
			if(fLength > 0.0f) {
				fLength = sqrtf(fLength);
				afTangent[0] /= fLength;
				afTangent[1] /= fLength;
				afTangent[2] /= fLength;
			}

			fLength = afBinormal[0]*afBinormal[0]+afBinormal[1]*afBinormal[1]+afBinormal[2]*afBinormal[2];
			if(fLength > 0.0f) {
				fLength = sqrtf(fLength);
				afBinormal[0] /= fLength;
				afBinormal[1] /= fLength;
				afBinormal[2] /= fLength;
			}
		}
	};


	namespace Intersection{
		SVIBool intersectSphereLine(const SVIVector3* pCenter, SVIFloat fRadius, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, SVIVector3* pPoint){
			return SVIFALSE;		
		}

		SVIBool intersectBoxLine(const SVIBoundingBox* pBox, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, SVIVector3* pPoint){
			SVIVector3 Center = const_cast<SVIBoundingBox*>(pBox)->getCenter();
			SVIVector3 SDir = (*pDirection)*0.5f;
			SVIVector3 SCen = *pOrigin + SDir;
			SVIVector3 Diff = SCen - Center;
			SVIVector3 Extent = pBox->mMax - Center;
			if(fabs(Diff.x) > Extent.x + fabs(SDir.x)) return SVIFALSE;
			if(fabs(Diff.y) > Extent.y + fabs(SDir.y)) return SVIFALSE;
			if(fabs(Diff.z) > Extent.z + fabs(SDir.z)) return SVIFALSE;
			SVIVector3 WxD = SDir.Cross(Diff);
			if(fabs(WxD.x) > Extent.y*fabs(SDir.z) + Extent.z*fabs(SDir.y)) return SVIFALSE;
			if(fabs(WxD.y) > Extent.x*fabs(SDir.z) + Extent.z*fabs(SDir.x)) return SVIFALSE;
			if(fabs(WxD.z) > Extent.x*fabs(SDir.y) + Extent.y*fabs(SDir.x)) return SVIFALSE;
			return SVITRUE;
		}

		SVIInt intersectBoxLineSide(const SVIBoundingBox* pBox, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, SVIVector3* pPoint){
			SVIInt iRet = -1;
			SVIVector3 Dir = *pDirection;
			SVIVector3 Ret;
			for(SVIInt i=0; i<3; ++i) {
				//2011-11-07 masterkeaton27@gmail.com
				//skip y axis. which one is use in quad-tree checking.
				if(i == 1) continue; 
				SVIPlane Side;
				if(Dir[i] > 0) {
					if((*pOrigin)[i] < pBox->mMin[i]) {
						Side.n[i] = -1;
						Side.d = pBox->mMin[i];
					} else {
						Side.n[i] = 1;
						Side.d = -pBox->mMax[i];
					}
				} else if(Dir[i] < 0) {
					if((*pOrigin)[i] > pBox->mMax[i]) {
						Side.n[i] = 1; 
						Side.d = -pBox->mMax[i];
					} else {
						Side.n[i] = -1;
						Side.d = pBox->mMin[i];
					}
				} else {
					continue;
				}
				if(intersectPlaneLine(&Side, pOrigin, &Dir, NULL, &Ret, SVITRUE)) {
					*pPoint = Ret;
					Dir = Ret - (*pOrigin);
					iRet = i*2+(Side.n[i] > 0);
				}
			}
			return iRet;
		}



		SVIBool intersectPlaneLine(const SVIPlane* pPlane, 
			const SVIVector3* pOrigin, const SVIVector3* pDirection, SVIFloat* pDepth, SVIVector3* pPoint, SVIBool TwoSided){
			SVIFloat fDistBefore = pPlane->n.dot(*pOrigin) + pPlane->d;
			SVIFloat fDistAfter = pPlane->n.dot((*pOrigin)+(*pDirection)) + pPlane->d;
			const SVIFloat cfEpsilon = 0.00001f;
			if((fDistBefore > cfEpsilon && fDistAfter < -cfEpsilon) || (fDistBefore < -cfEpsilon && fDistAfter > cfEpsilon)) {
				if(pPoint) {
					SVIFloat fNumer = pPlane->n.dot(*pOrigin);
					SVIFloat fDenom = pPlane->n.dot(*pDirection);
					SVIFloat fInter = (-pPlane->d - fNumer)/fDenom;
					*pPoint = *pOrigin + (*pDirection)*fInter;
				}
				return SVITRUE;
			}
			return SVIFALSE;
		}


		SVIBool intersectBoxBox(const SVIBoundingBox* pBox0, const SVIBoundingBox* pBox1) {
			return ((pBox0->mMax.x >= pBox1->mMin.x) && (pBox0->mMin.x <= pBox1->mMax.x) && 
				(pBox0->mMax.y >= pBox1->mMin.y) && (pBox0->mMin.y <= pBox1->mMax.y) && 
				(pBox0->mMax.z >= pBox1->mMin.z) && (pBox0->mMin.z <= pBox1->mMax.z));
		}

		SVIBool insideBoxPoint(const SVIBoundingBox* pBox, const SVIVector3* pPoint) {
			return ((pBox->mMin.x <= pPoint->x) && (pBox->mMax.x >= pPoint->x) && 
				(pBox->mMin.y <= pPoint->y) && (pBox->mMax.y >= pPoint->y) && 
				(pBox->mMin.z <= pPoint->z) && (pBox->mMax.z >= pPoint->z));
		}
	};

};