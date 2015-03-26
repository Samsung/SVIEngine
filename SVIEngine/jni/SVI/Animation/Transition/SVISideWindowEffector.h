#ifndef __SVI_SIDEWINDOWEFFECTOR_H_
#define __SVI_SIDEWINDOWEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVISideWindowEffector : public SVITransitionEffector {
	public:
		SVISideWindowEffector(SVIGLSurface *surface);
		virtual ~SVISideWindowEffector() {}

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