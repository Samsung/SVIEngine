#ifndef __SVI_SWITCHEFFECTOR_H_
#define __SVI_SWITCHEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVISwitchEffector : public SVITransitionEffector {
	public:
		SVISwitchEffector(SVIGLSurface *surface);
		virtual ~SVISwitchEffector() {}

	protected:
		void setAnimation();
        void addParentRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle);
        void addChildRotAni(SVIAnimationSet* aniSet, SVIFloat rotAngle);        
	private:
		SVIFloat rotationAngle;
        SVIFloat slideWidth;
		SVIFloat slideHeight;
	};

} 
#endif