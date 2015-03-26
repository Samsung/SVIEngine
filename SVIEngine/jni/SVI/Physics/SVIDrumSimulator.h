#pragma once


namespace SVI{


	class SVISpring;
	class SVIPivot;
	struct SVIVector3; 

	
	//2011-11-11 masterkeaton27@gmail.com
	//drum grid simulator
	class SVIDrumSimulator{
	public:
		SVIDrumSimulator();
		virtual ~SVIDrumSimulator();
	private:
		SVIDrumSimulator(const SVIDrumSimulator& rhs){}
		void operator=(const SVIDrumSimulator& rhs){}
		
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

	protected:
		void updateTarget();

    private:
        SVIUInt mGridSize;
        SVIInt mPivotCount;	

        SVIVector3 mControlTarget;
    private:
        SVIPivot * mPivots;
        SVIPivot * mTargetPivots;
    };
}