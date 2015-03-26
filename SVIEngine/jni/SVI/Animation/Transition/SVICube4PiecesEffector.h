#ifndef __SVI_CUBE4PIECESEFFECTOR_H_
#define __SVI_CUBE4PIECESEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVICube4PiecesEffector : public SVITransitionEffector {
	public:
		SVICube4PiecesEffector(SVIGLSurface *surface);
		virtual ~SVICube4PiecesEffector() {}

	protected:
		void setAnimation();

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	private:
		void addZPositionAnimationTime(SVIAnimationSet* aniSet, SVIFloat preZPosition, SVIFloat midZPosition, SVIFloat nextZPosition);
		SVIFloat rotationAngle;
	};

} 
#endif