#ifndef __SVI_SPLITEFFECTOR_H_
#define __SVI_SPLITEFFECTOR_H_

#include "SVITransitionEffector.h"

namespace SVI {

	class SVISplitEffector : public SVITransitionEffector {
	public:
		SVISplitEffector(SVIGLSurface *surface);
		virtual ~SVISplitEffector() {}

	protected:
		void setAnimation();

	private:
		SVIFloat slideWidth;
		SVIFloat slideHeight;

	protected:
		SVIInt rowCount;
		SVIInt columnCount;
	};

} 
#endif