#ifndef __SVI_CURTAINEFFECTOR_H_
#define __SVI_CURTAINEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVICurtainEffector : public SVITransitionEffector {
	public:
		SVICurtainEffector(SVIGLSurface *surface);
		virtual ~SVICurtainEffector() {}

	protected:
		void setAnimation();
	private:
		void setOffsetDuration(SVIInt fullTimeDuration);

	private:
		SVIFloat offsetDuration;
		SVIFloat rotationAngle;
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	};

} 
#endif