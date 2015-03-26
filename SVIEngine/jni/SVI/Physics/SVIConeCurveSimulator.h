#pragma once

#define RAD ((float)PI/180)
#define PI 3.1415f

namespace SVI{


	class SVISpring;
	class SVIPivot;
	struct SVIVector3; 

	
	//2012-12-11 masterkeaton27@gmail.com
	//conecurve simulator
	class SVIConeCurveSimulator{
	public:
		SVIConeCurveSimulator();
		virtual ~SVIConeCurveSimulator();
	private:
		SVIConeCurveSimulator(const SVIConeCurveSimulator& rhs){}
		void operator=(const SVIConeCurveSimulator& rhs){}
		
	public:
		void setControlTarget(const SVIVector3& target);

	public:
		const SVIVector3& getControlTarget(){return mControlTarget;} 
		
	public:
		void reset();
		void update();
		void build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,SVIUShort * pIndices,
			SVIVector3 * pNormals,SVIVector2 * pTexcoords,SVIBool isVertical = SVIFALSE, SVIBool isReverse = SVIFALSE,SVIBool bFliped = SVIFALSE);

	public:
		SVIUInt getIndicesCount();
		SVIUInt getVerticesCount();
		//venkat
		void deformMesh(); //use update for getting the new cone points
		void Animate(float Theta, float ConeApex);
		void setAxis(float Axis);
		void setAngle(float Angle);
		void setxOffset(float Offset);

	protected:
		void updateTarget();

	private:
		SVIUInt mGridSize;
		SVIInt mPivotCount;	

		SVIVector3 mControlTarget;
		SVIVector3 mControlBase;
	private:
		SVISectorLine * mLines;
		SVIPivot * mPivots;

		//venkat
		SVIFloat mWidth;
		SVIFloat mHeight;
		int mRows;
		int mColumns;
		SVIFloat mRho; //rotation about the Y Axis;
		SVIFloat mTheta; //Angle of the cone
		SVIVector2 mA; //Distance of Apex of the cone from origin on Y axis

		SVIVector2 *mpInputVertex2D;

		int mNumOfVertices;
		SVIFloat xOffset;
	};
}
