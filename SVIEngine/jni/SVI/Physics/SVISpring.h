#pragma once


namespace SVI{

	//2011-11-09 masterkeaton27@gmail.com
	//spring object for physical simulation
	class SVISpring{
	public:
		SVISpring();
		SVISpring(SVIInt pivotA, SVIInt pivotB);
		SVISpring(SVIInt pivotA, SVIInt pivotB, SVIFloat tension, SVIFloat constant, SVIFloat length);
		virtual ~SVISpring();

	public:
		SVIInt getPivotA(){return mPivotA;}
		SVIInt getPivotB(){return mPivotB;}
		SVIFloat getTension(){return mTension;}
		SVIFloat getConstant(){return mConstant;}
		SVIFloat getLength(){return mNaturalLength;}

	public:
		void setPivotA(SVIInt index){mPivotA = index;}
		void setPivotB(SVIInt index){mPivotB = index;}

		void setTension(SVIFloat tension){mTension = tension;}
		void setConstant(SVIFloat constant){mConstant = constant;}
		void setLength(SVIFloat length){mNaturalLength = length;}

	private:
		SVIInt	mPivotA;
		SVIInt	mPivotB;

		SVIFloat mTension;
		SVIFloat mConstant;
		SVIFloat mNaturalLength;
	};
}