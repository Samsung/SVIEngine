#include "nativeFTrace.h"
#include "../SVICores.h"
#include "../Utils/SVIFTrace.h"
#include <wchar.h>
#include <string.h>

using namespace SVI;

static const SVIBool DEBUG = SVIFALSE;
static const SVIInt STRING_BUFFER_SIZE = 1024;
static SVIChar gStringBuffer[STRING_BUFFER_SIZE + 1];

SVIChar* jstringToChar(JNIEnv* env, jstring jstr)  
{
	
	SVIChar* ret = NULL;
#if 0
	SVIInt length = env->GetStringLength(jstr);  
	const jchar* jcstr = env->GetStringChars(jstr, 0);  
	SVIInt requiredSize = length * 2 + 1;
	int size = 0;  
	if( requiredSize <= STRING_BUFFER_SIZE) {
		size = WideCharToMultiByte( CP_ACP,0, (LPCWSTR) jcstr, length, gStringBuffer, requiredSize, NULL, NULL );  
		if( size > 0 ) {
			ret = gStringBuffer;
			ret[size] = 0;
		}
	}
	env->ReleaseStringChars(jstr, jcstr );  
#else
	const char *nativeString = env->GetStringUTFChars(jstr, 0);
	SVIInt len = strlen(nativeString);
	if( len < STRING_BUFFER_SIZE ) {
		memcpy(gStringBuffer, nativeString, len);
		gStringBuffer[len+1] = 0;
		ret = gStringBuffer;
	}
	env->ReleaseStringUTFChars(jstr, nativeString);
#endif
	return ret;  
}  

JNIEXPORT void JNICALL Java_com_github_sviengine_debug_SVIFTrace_nativeEnable(JNIEnv *env, jclass jobj) {
	SVIFTrace::getInstance()->enable();
}

JNIEXPORT void JNICALL Java_com_github_sviengine_debug_SVIFTrace_nativeDisable(JNIEnv *env, jclass jobj) {
	SVIFTrace::getInstance()->disable();
}

JNIEXPORT void JNICALL Java_com_github_sviengine_debug_SVIFTrace_nativeMark(JNIEnv *env, jclass jobj, jstring mark) {
	SVIChar* nativeString = jstringToChar(env, mark);
	if( nativeString != NULL )
		SVIFTrace::getInstance()->mark(nativeString);
}
