#pragma once


namespace SVI{


	class SVISpring;
	class SVIPivot;
	struct SVIVector3; 


	class SVIExplodeMolcule{
	public:			   
		SVIExplodeMolcule();
		~SVIExplodeMolcule();
	private:
		SVIExplodeMolcule(const SVIExplodeMolcule& rhs){}
		void operator=(const SVIExplodeMolcule& rhs){}

	public:
		void init(SVIFloat fPower, const SVIVector3 &size, const SVIVector3 &position, const SVIVector3 &velocity, const SVIVector3 &angularvelocity);
		void setPower(SVIFloat fPower);		
		void setPosition(const SVIVector3 &position);
		void setVelocity(const SVIVector3 &velocity);
		void setAngularVelocity(const SVIVector3 &velocity);
		void setRotation(const SVIVector3 &rotation);
		void setSize(SVIVector3 size);

		void update(SVIFloat fElapsed);
	public:
		SVIVector3 getPosition(SVIInt index);
		SVIVector3 getNormal();
		SVIVector3 getSize();

	private:
		SVIVector3 mInitPosition;
		SVIVector3 mSize;
		SVIVector3 mPosition;
		SVIVector3 mRotation;
		SVIVector3 mInitVelocity;
		SVIVector3 mVelocity;
		SVIVector3 mNormal;
		SVIVector3 mAngularVelocity;
		SVIFloat mPower;

	};

	//2011-11-11 masterkeaton27@gmail.com
	//explode grid simulator
	class SVIExplodeSimulator{
	public:
		SVIExplodeSimulator();
		virtual ~SVIExplodeSimulator();
		
	public:
		void setControlTarget(const SVIVector3& target);

	public:
		const SVIVector3& getControlTarget(){return mControlTarget;} 

	public:
		void reset();
		void update();
		void build(const SVIVector3& offset, const SVIVector3& scale,SVIVector3 * pPositions,SVIUShort * pIndices,SVIVector3 * pNormals,SVIVector2 * pTexcoords, SVIBool bFliped = SVIFALSE);

	public:
		SVIUInt getIndicesCount();
		SVIUInt getVerticesCount();

	protected:
		void updateTarget();

	private:
		SVIUInt mGridSize;
		SVIInt mMolculeCount;	
		SVIVector3 mControlTarget;
	private:
		SVIExplodeMolcule * mMolcules;
	};
}