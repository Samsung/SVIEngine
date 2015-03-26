#pragma once


namespace SVI{


	class SVISpring;
	class SVIPivot;
	struct SVIVector3; 


	class SVISectorLine{
	public:
		std::vector<SVIPivot *> mPivots;
		SVIFloat		mNaturalLength;
		SVIFloat		mActiveLength;

		SVIPivot * mStart;
		SVIPivot * mEnd;

		void calculateActiveLength();
	};

	//2011-11-11 masterkeaton27@gmail.com
	//Page turn curve simulator
	class SVICurveSimulator{
	public:
		SVICurveSimulator();
		virtual ~SVICurveSimulator();
	private:
		SVICurveSimulator(const SVICurveSimulator& rhs){}
		void operator=(const SVICurveSimulator& rhs){}
		
	public:
		void setControlTarget(const SVIVector3& target);

	public:
		const SVIVector3& getControlTarget(){return mControlTarget;} 


	public:
		void reset();
		void update(SVIBool fakeDepth, SVIFloat fakeDepthMax);
		void build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,SVIUShort * pIndices,SVIVector3 * pNormals,
			SVIVector2 * pTexcoords, SVIBool isVertical = SVIFALSE, 
			SVIBool isReverse = SVIFALSE, SVIBool bFliped = SVIFALSE, SVIBool isBackFaceImageFlip = SVITRUE, SVIBool isBackFaceClear = SVIFALSE);

	public:
		SVIUInt getIndicesCount();
		SVIUInt getVerticesCount();

	protected:
		void updateTarget(SVIBool fakeDepth, SVIFloat fakeDepthMax);

	private:
		SVIUInt mGridSize;
		SVIInt mPivotCount;	

		SVIVector3 mControlTarget;
		SVIVector3 mControlBase;
	private:
		SVISectorLine * mLines;
		SVISectorLine mControlLine;
		SVIPivot * mPivots;
	};
}