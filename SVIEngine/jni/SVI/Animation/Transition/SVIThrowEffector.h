#ifndef __SVI_THROWEFFECTOR_H_
#define __SVI_THROWEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIThrowEffector : public SVITransitionEffector {
	public:
		SVIThrowEffector(SVIGLSurface *surface);
		virtual ~SVIThrowEffector() {}

	protected:
		void setAnimation();
	private:
		void setOffsetDuration(SVIInt fullTimeDuration);
	private:
		SVIVector3 scaleValue;
		SVIFloat slideWidth;
		SVIFloat slideHeight;
		SVIFloat rotationAngle;
	protected:
		SVIInt rowCount;
		SVIInt columnCount;
	};

} 
#endif