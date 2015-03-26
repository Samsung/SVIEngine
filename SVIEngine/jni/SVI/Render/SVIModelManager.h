#ifndef __SVI_MODELMANAGER_H_
#define __SVI_MODELMANAGER_H_

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include "../BaseType/SVIBaseTypes.h"
#include "SVIMaterial.h"
#include "SVIRigidMesh.h"
#include "SVISceneNode.h"
#include "SVIStaticModelNode.h"

namespace SVI {

	typedef std::pair<std::string, SharedModelNode> StringModelPair;
	typedef std::map<std::string, SharedModelNode> SharedModelMap;
	
	typedef std::pair<std::string, SharedModelTexture> StringModelTexturePair;
	typedef std::map<std::string, SharedModelTexture> SharedModelTextureMap;


	class SVIModelManager {
	public:
		SVIModelManager(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		~SVIModelManager();

		SVISceneNode* loadModelFile(const std::string &fileName, SVIBool deepCopy);
		SVISceneNode* loadModelFile(const std::string &fileName, SVIBool deepCopy, const char *buffer, SVIUInt maxSize);
		SVISceneNode* loadModelFileFromAssetManager(const std::string &fileName, SVIBool deepCopy);

		SharedModelTexture loadModelTexture(const std::string &fileName);

		SVIModelTexture* createTextureFromFile(const std::string &fileName);
		// through AssetManager texture is created.
		SVIModelTexture* createTextureFromAssetManager(const std::string &fileName);
		SVIModelTexture* createTextureFromBuffer(const char *buffer, SVIUInt size);

		SVIBool addModel(std::string id, SharedModelNode model);
		SVIBool addTexture(std::string id, SharedModelTexture texture);

		SharedModelNode getModel(std::string id);
		SharedModelTexture getTexture(std::string id);

		SVIBool genTexture(SVIModelTexture *texture);
		SVIBool genTexture(SharedModelTexture texture);

		void restore();

		SVIBool removeModel(std::string id);
		SVIBool removeTexture(std::string id);


	private:


		void createChildSceneNodeList(const char *buffer, SharedMeshList &meshList, SVISceneNode *parent, SVIModelNode *modelNode);
		void read(char *dst, const char *src, SVIUInt readSize);
		
	private:
		SVIGLSurface * mSVIGLSurface;
		SVIUInt mFileCursor;
		SharedModelMap mModelMap;
		SharedModelTextureMap mModelTextureMap;
	};

} // end namespace SVI


#endif // #ifndef __SVI_MODELMANAGER_H_
