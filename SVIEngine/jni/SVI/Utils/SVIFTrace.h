#ifndef __SVI_SVIFTRACE_H_
#define __SVI_SVIFTRACE_H_

#include "../BaseType/SVIBaseTypes.h"

namespace SVI {
	class SVIFTrace {

	private:
		SVIFTrace();

	public:
		~SVIFTrace();

		static SVIFTrace* getInstance();

		SVIBool enable();
		SVIBool disable();
		void mark(SVIChar* mark);

	private:
		void initialize();
		SVIChar* findDebugFS();

	private :
		static SVIFTrace*	mInstance;
		SVIInt				mTracerFD;		// File Descriptor handle of /tracing/tracing_on
		SVIInt				mMarkerFD;		// File Descriptor handle of /tracing/trace_maker
		SVIBool				mInitialized;	
		SVIChar				mTracerPath[256];
		SVIChar				mMarkerPath[256];
	};
};

#endif //__SVI_SVIFTRACE_H_