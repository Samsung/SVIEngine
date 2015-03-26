#ifndef __SVI_SLIDE_H_
#define __SVI_SLIDE_H_

#include "../SVICores.h"
#include "SVIAnimation.h"
#include "SVIAnimationSet.h"

namespace SVI {

	class SVIAnimationListenerProxy : public SVIAnimationListener {
		public:
			enum AnimationListenerType {	
				EXPLICIT = 0,
				IMPLICIT = 1,
			};
			
			SVIAnimationListenerProxy();
			SVIAnimationListenerProxy(SVIInt type);
			virtual ~SVIAnimationListenerProxy();

			void onAnimationEnd(SVIAnimation* animation);
			void onAnimationRepeat(SVIAnimation* animation);
			void onAnimationStart(SVIAnimation* animation);

		protected:
			SVIInt					mType;
	};
};
#endif

