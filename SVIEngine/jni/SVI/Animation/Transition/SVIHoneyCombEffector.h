#ifndef __SVI_HONEYCOMBEFFECTOR_H_
#define __SVI_HONEYCOMBEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIHoneyCombEffector : public SVITransitionEffector {
	public:
		SVIHoneyCombEffector(SVIGLSurface *surface);
		virtual ~SVIHoneyCombEffector() {}

	protected:
		void setAnimation();

	private:
		void setOffsetDuration(SVIInt fullTimeDuration);

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	};

} 
#endif