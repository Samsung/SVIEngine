#include "SVITime.h"
#include "../SVICores.h"
#include <time.h>
#include <stdarg.h>

#define SEC_A_DAY 86400

SVIUInt SVITime::currentTimeMillis() {
	struct timeval t;
	gettimeofday(&t, NULL);

	//time is measured as millisecond.
	SVIUInt currentTime = (SVIUInt)((t.tv_sec % SEC_A_DAY)*1000 + (t.tv_usec / 1000));

	return currentTime;
}

SVICheckTime::SVICheckTime(SVIChar* name, SVIBool reportTime) {
	mReportTime = reportTime;
	if( mReportTime ) {
		mStartTime = SVITime::currentTimeMillis();
		if( name != NULL )
			snprintf(mName, 128, "%s", name);
	}
	
}

SVICheckTime::~SVICheckTime() {
	if( mReportTime ) {
		SVIUInt endTime = SVITime::currentTimeMillis();
		SVIUInt elapsedTime = endTime - mStartTime;

		LOGI("%s : %d", mName, elapsedTime);
	}
}

void SVICheckTime::setName(SVIChar* fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vsprintf(mName, fmt, ap);
	va_end(ap);
}

void SVICheckTime::setName(SVIChar* name, SVIChar* fmt, ...) {

	SVIChar temp[128];

	va_list ap;

	va_start(ap, fmt);
	vsprintf(temp, fmt, ap);
	va_end(ap);

	snprintf(mName, 128, "%s %s", name, temp);
}