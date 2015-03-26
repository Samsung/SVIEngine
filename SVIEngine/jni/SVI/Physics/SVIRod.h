#pragma once


namespace SVI{

	//2011-11-09 masterkeaton27@gmail.com
	//spring object for physical simulation
	class SVIRod{
	public:

	    static const int ROD_3 = 1;
        static const int ROD_3D = 2;

        static const int BENT_UP = 1;
        static const int BENT_DOWN = -1;
        static const int BENT_LINE = 0;
		SVIRod(SVIInt pivotA, SVIInt pivotB, SVIInt pivotC, SVIFloat fArc, SVIInt mRodeType);
		virtual ~SVIRod();

	public:
		SVIInt getPivotA(){return mPivotA;}
		SVIInt getPivotB(){return mPivotB;}
		SVIInt getPivotC(){return mPivotC;}
		SVIFloat getArc(){return fArc;}

	public:
		void setPivotA(SVIInt index){mPivotA = index;}
		void setPivotB(SVIInt index){mPivotB = index;}
		void setPivotC(SVIInt index){mPivotC = index;}

		void setArcTension(SVIFloat tension){fArc = tension;}
		void setRodType(SVIInt type)        {mRodType = type;}

	private:
		SVIInt	mPivotA;
		SVIInt	mPivotB;
		SVIInt	mPivotC;

		SVIFloat fArc;
		SVIInt mRodType;
	};
}
