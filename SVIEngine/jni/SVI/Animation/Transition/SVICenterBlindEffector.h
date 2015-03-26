#ifndef __SVI_CENTERBLINDEFFECTOR_H_
#define __SVI_CENTERBLINDEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVICenterBlindEffector : public SVITransitionEffector {
	public:
		SVICenterBlindEffector(SVIGLSurface *surface);
		virtual ~SVICenterBlindEffector() {}

	protected:
		void setAnimation();
	private:
		void setOffsetDuration(SVIInt fullTimeDuration);

	private:
		SVIFloat rotationAngle;
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	};

} 
#endif