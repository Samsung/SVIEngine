#ifndef __SVI_TIME_H_
#define __SVI_TIME_H_

#include "../SVICores.h"

class SVITime {
public:
	static SVIUInt currentTimeMillis();
};

class SVICheckTime {
public:
	SVICheckTime(SVIChar* name = NULL, SVIBool reportTime = SVITRUE);
	~SVICheckTime();

	void setName(SVIChar* fmt, ...);
	void setName(SVIChar* main, SVIChar* fmt, ...);

private:
	SVIChar	mName[128];
	SVIUInt	mStartTime;
	SVIBool	mReportTime;
};

#endif