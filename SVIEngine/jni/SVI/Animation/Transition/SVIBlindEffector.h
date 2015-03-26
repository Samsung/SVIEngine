#ifndef __SVI_BLINDEFFECTOR_H_
#define __SVI_BLINDEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIBlindEffector : public SVITransitionEffector {
	public:
		SVIBlindEffector(SVIGLSurface *surface);
		virtual ~SVIBlindEffector() {}

	protected:
		void setAnimation();
		/*
	private:
		void setOffsetDuration(SVIInt fullTimeDuration);
		*/
	private:
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	};

} 
#endif