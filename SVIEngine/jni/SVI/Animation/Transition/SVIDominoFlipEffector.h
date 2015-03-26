#ifndef __SVI_DOMINOFLIPEFFECTOR_H_
#define __SVI_DOMINOFLIPEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIDominoFlipEffector : public SVITransitionEffector {
	public:
		SVIDominoFlipEffector(SVIGLSurface *surface);
		virtual ~SVIDominoFlipEffector() {}

	protected:
		void setAnimation();

	private:
		void setOffsetDuration(SVIInt fullTimeDuration);

	private:
		SVIFloat rotationAngle;

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	};

} 
#endif