#include "SVIFTrace.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "../SVICores.h"


namespace SVI {

	#define DEBUG_FS "/sys/kernel/debug"
	//#define DEBUG_FS "/sdcard"

	const static SVIBool DEBUG = SVIFALSE;

	SVIFTrace* SVIFTrace::mInstance = NULL;

	SVIFTrace::SVIFTrace() {
		mMarkerFD = -1;
		mTracerFD = -1;
		mInitialized = SVIFALSE;

		initialize();
	}

	SVIFTrace::~SVIFTrace() {
	}

	SVIFTrace* SVIFTrace::getInstance() {
		static SVIFTrace instance;
		if( mInstance == NULL ) {
			mInstance = &instance;
		}

		return mInstance;
	}

	void SVIFTrace::initialize() {
		if( mInitialized )
			return;

		SVIChar* debugFS = NULL;
		SVIChar path[256];

		debugFS = findDebugFS();
		if (debugFS) {
			strcpy(mTracerPath, debugFS);
			strcat(mTracerPath,"/tracing/tracing_enabled");
			
			strcpy(mMarkerPath, debugFS);
			strcat(mMarkerPath,"/tracing/trace_marker");
		}
	}

	SVIChar* SVIFTrace::findDebugFS() {
		return (SVIChar *)DEBUG_FS;
	}

	SVIBool SVIFTrace::enable() {
		mTracerFD = open(mTracerPath, O_WRONLY);
		if( mTracerFD < 0 ) {
			LOGE("Failed opening tracing_enabled :%s", mTracerPath);
			return SVIFALSE;
		}

		if (mTracerFD >= 0) {
			write(mTracerFD, "1", 1);
			DEBUG_CHECK_LOGE("SVIFTrace::enable :%d", mTracerFD);

			close(mTracerFD);
			mTracerFD = -1;
		}

        return SVITRUE;
	}

	SVIBool SVIFTrace::disable() {
		mTracerFD = open(mTracerPath, O_WRONLY);
		if( mTracerFD < 0 ) {
			LOGE("Failed opening tracing_enabled :%s", mTracerPath);
			return SVIFALSE;
		}

		if (mTracerFD >= 0) {
			write(mTracerFD, "0", 1);
			DEBUG_CHECK_LOGE("SVIFTrace::disable :%d", mTracerFD);
			close(mTracerFD);
			mTracerFD = -1;
		}

        return SVITRUE;
	}

	void SVIFTrace::mark(SVIChar* mark) {
		if( mark == NULL )
			return;

		mMarkerFD = open(mMarkerPath, O_WRONLY);
		if( mMarkerFD < 0 ) {
			LOGE("Failed opening trace_marker :%s", mMarkerPath);
			return;
		}

		if( mMarkerFD >= 0 ) {
			SVIInt len = strlen(mark);
			write(mMarkerFD, mark, len);
			DEBUG_CHECK_LOGE("SVIFTrace::mark :%d %s", mMarkerFD, mark);

			close(mMarkerFD);
			mMarkerFD = -1;
		}
	}
}
