#ifndef __SVI_CUBE2PIECESEFFECTOR_H_
#define __SVI_CUBE2PIECESEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVICube2PiecesEffector : public SVITransitionEffector {
	public:
		SVICube2PiecesEffector(SVIGLSurface *surface);
		virtual ~SVICube2PiecesEffector() {}

	protected:
		void setAnimation();
	private:
		void addRotAni(SVIAnimationSet* aniSet, SVIVector3 preAngle, SVIVector3 nextAngle, SVIInt duration, SVIInt offset);
		SVIFloat rotationAngle;
		SVIFloat slideWidth;
		SVIFloat slideHeight;
		SVIInt rowCount;
		SVIInt columnCount;
	
	};

} 
#endif