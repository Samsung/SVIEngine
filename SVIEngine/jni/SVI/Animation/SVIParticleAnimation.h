#ifndef __SVI_PARTICLEANIMATION_H_
#define __SVI_PARTICLEANIMATION_H_

#include "../SVICores.h"
#include "../Slide/SVISlide.h"
#include "SVIAnimation.h"
#include <vector>

namespace SVI {

	class SVIParticleEffect;
	
	// Creation Date	: 2012/06/26 rareboy0112@facebook.com
	// Description		: particle animation class
	// Last Modified	: 2012/07/02 rareboy0112@facebook.com
	class SVIParticleAnimation : public SVIAnimation {
	public:

		SVIParticleAnimation(SVIGLSurface* saGLSurface);

		~SVIParticleAnimation();

		// @Override method
		void update(SVIUInt& time);
		void calculateAndApply(const SVIFloat& ratio) {};
		
		// TODO : 
		SVIAnimation* clone();
		
		void print();

		void cancel();

		void forceEnd();

		void setParticleEffect(SVIParticleEffect* effect);
		
	protected :
		void copy(const SVIParticleAnimation* rhs);

		//void processEnd();
	private:
		SVIParticleEffect*		mParticleEffect;
	private:  
		SVIParticleAnimation(const SVIParticleAnimation& rhs):SVIAnimation(rhs.mSVIGLSurface) {copy(&rhs);}
		void operator=(const SVIParticleAnimation& rhs){copy(&rhs);} 
	};

}

#endif //__SVI_PARTICLEANIMATION_H_
