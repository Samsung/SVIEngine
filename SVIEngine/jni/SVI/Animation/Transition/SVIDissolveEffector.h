#ifndef __SVI_DISSOLVEEFFECTOR_H_
#define __SVI_DISSOLVEEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIDissolveEffector : public SVITransitionEffector {
	public:
		SVIDissolveEffector(SVIGLSurface *surface);
		virtual ~SVIDissolveEffector() {}

	protected:
		void setAnimation();
		
	private:
		void setOffsetDuration(SVIInt fullTimeDuration);
	private:
			SVIInt rowCount;
			SVIInt columnCount;
	};

} 
#endif