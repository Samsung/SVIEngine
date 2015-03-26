#pragma once

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SVIEngine", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "SVIEngine", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SVIEngine", __VA_ARGS__))

#ifdef __DEBUG
#define ENABLE_DEBUG_LOG 1 
#else //__RELEASE
#define ENABLE_DEBUG_LOG 1
#endif

#if (ENABLE_DEBUG_LOG)
//	#pragma message("ENABLE_DEBUG_LOG is enabled !!!")
	#define DEBUG_CHECK_LOGI(...) if( DEBUG && ENABLE_DEBUG_LOG ) ((void)__android_log_print(ANDROID_LOG_INFO, "SVIEngine", __VA_ARGS__))
	#define DEBUG_CHECK_LOGE(...) if( DEBUG && ENABLE_DEBUG_LOG ) ((void)__android_log_print(ANDROID_LOG_ERROR, "SVIEngine", __VA_ARGS__))
#else
//	#pragma message("ENABLE_DEBUG_LOG is disabled !!!")
	#define DEBUG_CHECK_LOGI(...)  	
	#define DEBUG_CHECK_LOGE(...)  	
#endif


#define REPORT_PERFORMANCE_LOG

#ifdef REPORT_PERFORMANCE_LOG
#define CHECK_PERFORMANCE(...)			SVICheckTime checkTime;									\
										checkTime.setName((SVIChar*)__PRETTY_FUNCTION__, __VA_ARGS__);
#else
#define CHECK_PERFORMANCE(...)			
#endif


//2011-11-04 masterkeaton27@gmail.com
//new zero check macro
#define SVI_FLT_AS_DW(F) (*(unsigned long*)&(F))
#define SVI_ALMOST_ZERO(x) ((SVI_FLT_AS_DW(x) & 0x7f800000L) == 0)

#define ALMOST_ZERO(x) x < 0.000001 ? SVITRUE : SVIFALSE

//2011-11-04 masterkeaton27@gmail.com
#define SVI_INV_PI	0.31830988618379069122f
#define SVI_PI_2		1.57079632679489661923f
#define SVI_2_PI		6.28318530717958623200f
#define SVI_PI		3.14159265358979323846f

#define SVI_45 3.141592f * 0.25f
#define SVI_90 3.141592f * 0.5f
#define SVI_270 3.141592f * 1.5f

#define SVI_DEGTORAD(x) (x * 3.141592f) / 180.0f
#define SVI_RADTODEG(x) (x * 180.0f) / 3.141592f

#define SVIMIN(a,b) (((a) < (b)) ? (a) : (b))
#define SVIMAX(a,b) (((a) > (b)) ? (a) : (b))

#define SVI_MAX_VERTEXSHADER_SIZE 1024
#define SVI_MAX_FRAGMENTSHADER_SIZE 1024

#define SVI_SVIFE_FREE(x) if (x != NULL) {free(x); x = NULL;}
#define SVI_SVIFE_DELETE(x) if (x != NULL) {delete x; x = NULL;} 
#define SVI_SVIFE_DELETE_ARRAY(x) if (x != NULL) {delete [] x; x = NULL;} 
#define SVI_SVIFE_RELEASE(x) if (x != NULL) {(x)->release(); x = NULL;}
#define SVI_SVIFE_ADDREF(x) if(x != NULL) { (x)->addRef(); }

//2011-07-04 masterkeaton27@gmail.com
//linear filter texture based anti-aliasing
#define SVI_FRAME_FILTERED_RATIO 1.0f //1.6 is a best trade off value for P4(Mali), 0.9 for P4(Tegra)



#define SVI_CLEAR_COLOR					0.0f

//2011-07-04 masterkeaton27@gmail.com
//temporary disable off screen recording for P4 performance issues.
#define SVI_USING_POST_PROCESS				
#define SVI_TEXTURE_FILTER_MIN				GL_NEAREST
#define SVI_TEXTURE_FILTER_MAG				GL_LINEAR

//2011-10-17 masterkeaton27@gmail.com
#define SVI_USING_CPU_TEXCOORD			0
#define SVI_USE_POST_EFFECT				1

#define FIXEDSCALE 16
#define FIXEDTOFLOAT(x) x >> 16
#define FLOATTOFIXED(x) round(x*(1<<16))s


#define SEC_A_DAY 86400
