#ifndef __SVI_CHECKERBOARDEFFECTOR_H_
#define __SVI_CHECKERBOARDEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVICheckerBoardEffector : public SVITransitionEffector {
	public:
		SVICheckerBoardEffector(SVIGLSurface *surface);
		virtual ~SVICheckerBoardEffector() {}

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