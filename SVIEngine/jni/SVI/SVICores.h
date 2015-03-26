#pragma once

#include <jni.h>

#include <errno.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <sys/resource.h>
#include <sys/time.h>

#define GL_GLEXT_PROTOTYPES

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/native_activity.h>
#include <android/native_window.h>

#include <android/log.h>
#include <android/window.h>
#include <android/configuration.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <sys/stat.h>

#include <stack>
#include <vector>
#include <algorithm>

#include "SVIDefines.h"
#include "BaseType/SVIBaseTypes.h"
#include "Mathematics/SVIMath.h"
#include "BaseType/SVIExtendTypes.h"
#include "Utils/SVITime.h"

//2011-07-12 masterkeaton27@gmail.com
//block profiler header
//#include "prof.h"

