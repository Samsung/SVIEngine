#ifndef __SVI_SHREDEFFECTOR_H_
#define __SVI_SHREDEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVIShredEffector : public SVITransitionEffector {
	public:
		SVIShredEffector(SVIGLSurface *surface);
		virtual ~SVIShredEffector() {}

	protected:
		void setAnimation();

	private:
		void addParentAni(SVIAnimationSet* aniSet);

	protected:
		SVIInt rowCount;
		SVIInt columnCount;

	};

} 
#endif