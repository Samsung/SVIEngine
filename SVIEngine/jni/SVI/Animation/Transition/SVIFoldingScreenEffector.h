#ifndef __SVI_FOLDINGSCREENEFFECTOR_H_
#define __SVI_FOLDINGSCREENEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIFoldingScreenEffector : public SVITransitionEffector {
	public:
		SVIFoldingScreenEffector(SVIGLSurface *surface);
		virtual ~SVIFoldingScreenEffector() {}

	protected:
		void setAnimation();

	private:
		void setOffsetDuration(SVIInt fullTimeDuration);
        void addPosAni(SVIAnimationSet* aniSet, bool isFrom, SVIFloat fromSlideWidth, SVIFloat angle, SVIFloat positionX, SVIFloat positionY);

	private:
		SVIFloat rotationAngle;

	protected:
		SVIInt rowCount;
		SVIInt columnCount;
	};

} 
#endif