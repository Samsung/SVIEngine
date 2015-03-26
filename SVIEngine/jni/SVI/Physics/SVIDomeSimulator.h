#pragma once


namespace SVI{


	class SVISpring;
	class SVIPivot;
	struct SVIVector3; 

	
	//2011-11-11 masterkeaton27@gmail.com
	//Twist grid simulator
	class SVIDomeSimulator{
	public:
		SVIDomeSimulator();
		virtual ~SVIDomeSimulator();
		
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
		SVIUInt mDomeResolution;
		SVIInt mPivotCount;	

        SVIVector3 mControlTarget;
        SVIVector3 mControlBase;
	private:
        std::vector<SVIPivot *> mPivots;
        std::vector<SVIUShort> mIndices;
	};
}