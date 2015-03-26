#include <stdlib.h>
#include "../SVICores.h"
#include "../Render/SVIModelManager.h"
#include "../Slide/SVISlide.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../Slide/SVIProjectionSlide.h"
#include "../Slide/SVIRenderPartManager.h"
#include "../Render/SVI3DScene.h"
#include "../Render/SVISceneNode.h"
#include "nativeSceneNode.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_scene_SVISceneNode_nativeLoad3dModelFile
  (JNIEnv *env, jclass clazz, jint natSurfaceHandle, jstring fileName) {
	
	const char *utf8;

	utf8 = env->GetStringUTFChars(fileName, NULL);

	SVIGLSurface* surface= (SVIGLSurface*)natSurfaceHandle;

	LOGI("nativeSceneNode: loadModelFileFromAssetManager(%s)", utf8);
	SVISceneNode *sceneNode = surface->getModelManager()->loadModelFileFromAssetManager(utf8, SVIFALSE);

	if(sceneNode == NULL)
		return -1;

	return (jint)sceneNode;
}