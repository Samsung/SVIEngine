#ifndef __SVI_BASIC_ANIMATION_H_
#define __SVI_BASIC_ANIMATION_H_

#include "SVIPropertyAnimation.h"

namespace SVI {
	class SVIBasicAnimation : public SVIPropertyAnimation{
	private:
		SVIBasicAnimation(PropertyAnimationType type, SVIGLSurface* saGLSurface);
	public:
		virtual ~SVIBasicAnimation();

		static SVIBasicAnimation* create(PropertyAnimationType type, SVIGLSurface* saGLSurface);

		void update(SVIUInt& time);

		virtual SVIAnimation* clone();

		virtual void print();
	protected:
		SVIBasicAnimation(SVIGLSurface* saGLSurface);
		void copy(const SVIBasicAnimation* rhs);
	};
};
#endif //__SVI_BASIC_ANIMATION_H_
