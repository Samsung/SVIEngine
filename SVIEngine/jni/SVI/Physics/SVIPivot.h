#pragma once


namespace SVI{

	struct SVIVector3;

#pragma pack(push)
#pragma pack(4)

	//2011-11-09 masterkeaton27@gmail.com
	class SVIPivot{
	public:
		SVIPivot();
		SVIPivot(const SVIVector3& position, const SVIVector3& velocity, const SVIVector3& normal, SVIFloat mass, SVIBool bFixed);		
		virtual ~SVIPivot();

	public:
		SVIVector3&	getPosition(){return mPosition;}
		SVIVector3&	getVelocity(){return mVelocity;}
		SVIVector3&	getNormal(){return mNormal;}
		SVIFloat		getMass(){return mMass;}
		SVIBool		isFixed(){return mFixed;}

	public:
		void    setPosition(const SVIVector3& position){mPosition = position;}
		void    setVelocity(const SVIVector3& velocity){mVelocity = velocity;}
		void    setNormal(const SVIVector3& normal){mNormal = normal;}
		void    setMass(SVIFloat mass){mMass = mass;}
		void    setFixed(SVIBool bFixed){mFixed = bFixed;}

	private:
		SVIVector3 mPosition;
		SVIVector3 mVelocity;
		SVIVector3 mNormal;
		SVIFloat mMass;
		SVIBool mFixed;
	};

#pragma pack(pop)

}