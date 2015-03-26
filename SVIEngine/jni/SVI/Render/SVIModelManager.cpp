#include <sys/stat.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "../Slide/SVIRenderPartManager.h"
#include "SVIModelManager.h"

namespace SVI {


SVIModelManager::SVIModelManager(SVIGLSurface* saGLSurface)
	:mFileCursor(0) {//Multi-Instance-Support
		mSVIGLSurface=saGLSurface;
	}


SVIModelManager::~SVIModelManager() {
	mModelMap.clear();
	mSVIGLSurface=NULL;
}


void SVIModelManager::read(char *dst, const char *src, SVIUInt readSize) {
	memcpy(dst, src + mFileCursor, readSize);
	mFileCursor += readSize;
}


SVISceneNode* SVIModelManager::loadModelFileFromAssetManager(const std::string &fileName, SVIBool deepCopy) {
	mFileCursor = 0;

	AAssetManager *assetMgr = mSVIGLSurface->getRenderPartManager()->getAssetManager();

	if(assetMgr == NULL) {
		LOGE("SVIModelManager: AssetManager has not been prepared !!!");
		return NULL;
	}
	
	// load file inside assets
	AAsset *asset = AAssetManager_open(assetMgr, fileName.c_str(), AASSET_MODE_UNKNOWN);

	// the asset might not be found
	if(asset == NULL) {
		LOGE("SVIModelManager: load asset fail !!!");
		return NULL;
	}

	off_t bufferSize = AAsset_getLength(asset);

	const char *buffer = (const char *)AAsset_getBuffer(asset);

	int numBytesRemaining = AAsset_getRemainingLength(asset);

	LOGE("SVIModelManager: file-size(%d)", numBytesRemaining);

	SVISceneNode *sceneNode = NULL;

	sceneNode = loadModelFile(fileName, deepCopy, buffer, numBytesRemaining);

	AAsset_close(asset);

	return sceneNode;
}


SVISceneNode* SVIModelManager::loadModelFile(const std::string &fileName, SVIBool deepCopy) {
	mFileCursor = 0;

	std::ifstream fs(fileName.c_str(), std::ios::in | std::ios::binary);
	if (!fs.is_open()) {
		return NULL;
	}

	struct stat statBuf;

	if( stat(fileName.c_str(), &statBuf) != 0 ) {
		return NULL;
	}
	
	SVIUInt fileSize = statBuf.st_size;
	char *fileBuffer = new char[fileSize];
	fs.read(fileBuffer, fileSize);

	SVISceneNode *sceneNode = NULL;

	sceneNode = loadModelFile(fileName, deepCopy, fileBuffer, fileSize);

	SVI_SVIFE_DELETE_ARRAY(fileBuffer);

	return sceneNode;
}


SVISceneNode* SVIModelManager::loadModelFile(const std::string &fileName, SVIBool deepCopy, const char *buffer, SVIUInt maxSize) {
	mFileCursor = 0;

	SharedModelNode sharedModel;

	SVIModelNode *rootNode = NULL;

	SharedModelMap::iterator iter = mModelMap.find(fileName);

	if( iter == mModelMap.end() ) {
		LOGI("SVIModelManager: '%s' file first upload", fileName.c_str());
		//! read .BSVI file data
		// header ====================================================================
		// version :		4 (byte)
		// material count : 4 (byte)
		// mesh count :		4 (byte)
		// ===========================================================================
	
		SVIInt version, materialCount, meshCount;
		read((char *)&version, buffer, 4);
		read((char *)&materialCount, buffer, 4);
		read((char *)&meshCount, buffer, 4);

		// create material list
		MaterialList materialList;
		for(int materialIdx = 0; materialIdx < materialCount; ++materialIdx) {
		
			// meterial ======================================================================
			// useTexture :	4 (byte)
			// ===============================================================================
		
			SVIInt useTexture;
			read((char *)&useTexture, buffer, 4);

			SVIMaterial *mat = new SVIMaterial(mSVIGLSurface);

			if(useTexture == 1) {
				// useTexture == 1 ===============================================================
				// fileNameSize :	4 (byte)
				// fileName :		fileNameSize (byte)
				// ===============================================================================
				SVIInt fileNameSize;
				read((char *)&fileNameSize, buffer, 4);

				char *fileName = new char[fileNameSize + 1];
				read((char *)fileName, buffer, fileNameSize);
				// add a null-terminated character
				fileName[fileNameSize] = '\0';
				SharedModelTexture texture = loadModelTexture(fileName);
				mat->mTexture = texture;
				mat->mRenderType = SVIMaterial::TEXTURE;
				SVI_SVIFE_DELETE_ARRAY(fileName);
			}
			else if(useTexture == 0) {
				// useTexture == 0 ===============================================================
				// diffuse color : 4 * 4 (byte)
				// ===============================================================================
				SVIColor color;
				read((char *)&color.mR, buffer, 4);
				read((char *)&color.mG, buffer, 4);
				read((char *)&color.mB, buffer, 4);
				read((char *)&color.mA, buffer, 4);
				mat->mColor = color;
				mat->mRenderType = SVIMaterial::SOLID_COLOR;
			}
			materialList.push_back(mat);
		}

		// create mesh list
		SharedMeshList meshList;
	
		for(int meshIdx = 0; meshIdx < meshCount; ++meshIdx) {
		
			// mesh ======================================================================
			// id size :			4 (byte)
			// id :					id size (byte)
			// vertex count :		4 (byte)
			// face buffer count :	4 (byte)
			// vertex size :		4 (byte)
			// index size :			4 (byte)
			// vertex buffer :		vertex size * vertex count (byte)
			// ============================================================================

			SVIRigidMesh *rigidMesh = new SVIRigidMesh(mSVIGLSurface);

			SVIInt idSize, vertexCount, faceBufferCount, vertexSize, indexSize;
			read((char *)&idSize, buffer, 4);
			char *id = new char[idSize + 1];
			read((char *)id, buffer, idSize);
			// add a null-terminated character
			id[idSize] = '\0';
			rigidMesh->mId = id;
			SVI_SVIFE_DELETE_ARRAY(id);
			read((char *)&vertexCount, buffer, 4);
			read((char *)&faceBufferCount, buffer, 4);
			read((char *)&vertexSize, buffer, 4);
			read((char *)&indexSize, buffer, 4);

			rigidMesh->mVertexCount = vertexCount;

			// TODO: check type
			// Currently only supports the Rigid Vertex.
			if(vertexSize != 32 || indexSize != 2) {
				SVI_SVIFE_DELETE(rigidMesh);
				return NULL;
			}

			// read vertex data
			SVIVector3 *positionArray = new SVIVector3[vertexCount];
			SVIVector3 *normalArray = new SVIVector3[vertexCount];
			SVIVector2 *uvArray = new SVIVector2[vertexCount];
			// position
			read((char *)positionArray, buffer, 12 * vertexCount);
			// normal
			read((char *)normalArray, buffer, 12 * vertexCount);
			// uv
			read((char *)uvArray, buffer, 8 * vertexCount);
		
			rigidMesh->mVertexPositionArray = positionArray;
			rigidMesh->mVertexNormalArray = normalArray;
			rigidMesh->mVertexUvArray = uvArray;

			// read face buffer list
			MaterialList meshMaterialList;
			FaceBufferList faceBufferList;
			std::vector<SVIInt> meshMaterialNumList;

			for(int faceBufferIdx = 0; faceBufferIdx < faceBufferCount; ++faceBufferIdx) {

				// face buffer ================================================================
				// material number :	4 (byte)
				// face count :			4 (byte)
				// index buffer :		face count * index size * 3 (byte)
				// ============================================================================

				// read material number
				SVIInt mtrNum;
				read((char *)&mtrNum, buffer, 4);
				SVIUInt faceCount;
				// face count
				read((char *)&faceCount, buffer, 4);

				// read index data
				SVIUShort *indexArray = new SVIUShort[faceCount * 3];
				read((char *)indexArray, buffer, 2 * faceCount * 3);

				SVIFaceBuffer *faceBuf = new SVIFaceBuffer();
				faceBuf->mFaceCount = faceCount;
				faceBuf->mIndices = indexArray;

				SVIBool bExist = SVIFALSE;
				for(int mtrNumIdx = 0; mtrNumIdx < (int)meshMaterialNumList.size(); ++mtrNumIdx) {
					if(meshMaterialNumList[mtrNumIdx] == mtrNum) {
						bExist = SVITRUE;
						faceBuf->mMaterial = meshMaterialList[mtrNumIdx];
						break;
					}
				}
			
				if(bExist == SVIFALSE) {
					meshMaterialNumList.push_back(mtrNum);
					
					//SVIMaterial *meshMaterial = new SVIMaterial(mSVIGLSurface);/*to avoid the resource Leak*/
					SVIMaterial *meshMaterial  = materialList[mtrNum];
					meshMaterialList.push_back(meshMaterial);
					faceBuf->mMaterial = meshMaterial;
				}

				faceBuf->mMesh = rigidMesh;
				faceBufferList.push_back(faceBuf);
			}
		
			rigidMesh->mMaterialList = meshMaterialList;
			rigidMesh->mFaceBufferList = faceBufferList;

			SharedMesh sMesh(rigidMesh);

			meshList.push_back(sMesh);

			//SVIOpenGLHelper::addDrawMesh(rigidMesh[geometryIdx]);
		}

		// create scene node
		SVIModelNode *sharedNode = new SVIModelNode(mSVIGLSurface);
		sharedNode->setId(fileName);

		// model node header =========================================================
		// root node count :	4 (byte)
		// ===========================================================================
		SVIInt rootNodeCount;
		read((char *)&rootNodeCount, buffer, 4);

		for(int nodeIdx = 0; nodeIdx < rootNodeCount; ++nodeIdx) {
			SVIModelNode *childNode = NULL;
			createChildSceneNodeList(buffer, meshList, sharedNode, childNode);
		}

		sharedModel = sharedNode;
		addModel(fileName, sharedModel);
	}
	else {
		LOGI("SVIModelManager: '%s' file uploaded", fileName.c_str());
		sharedModel = iter->second;
	}
	
	if(deepCopy) {
		rootNode = (SVIModelNode *)( sharedModel->deepCopy(SVITRUE) );
		rootNode->mSharedModel = sharedModel;
	}
	else {
		rootNode = sharedModel.getPtr();
	}

	return rootNode;
}


void SVIModelManager::createChildSceneNodeList(const char *buffer, SharedMeshList &meshList, SVISceneNode *parent, SVIModelNode *modelNode) {
	
	// model node ================================================================
	// id size :		4 (byte)
	// id :				id size (byte)
	// matrix :			4 * 4 (byte)
	// mesh number :	4 (byte)
	// child count :	4 (byte) 
	// ===========================================================================

	SVIInt idSize;
	read((char *)&idSize, buffer, 4);
	char *nodeId = new char[idSize + 1];
	read((char *)nodeId, buffer, idSize);
	// add a null-terminated character
	nodeId[idSize] = '\0';
	// read matrix
	SVIMatrix mat;
	read((char *)&mat, buffer, 4 * 16);
	
	// read mesh number
	SVIInt meshNum;
	read((char *)&meshNum, buffer, 4);
	// read chld count
	SVIInt childCount;
	read((char *)&childCount, buffer, 4);

	// check model-node type.
	if(meshNum == -1) {

	}
	else {
		if(modelNode == NULL){
		modelNode = new SVIStaticModelNode(mSVIGLSurface);
		}	
		std::string stringId = nodeId;
		modelNode->setId(stringId);
		modelNode->addMesh(meshList[meshNum]);
		modelNode->setLocalTransform(mat);
	}

	for(int i = 0; i < childCount; ++i) {
		SVIModelNode *childNode = NULL;
		createChildSceneNodeList(buffer, meshList, modelNode, childNode); 
	}

	parent->addChild(modelNode);
	SVI_SVIFE_DELETE_ARRAY(nodeId);
}


SharedModelTexture SVIModelManager::loadModelTexture(const std::string &fileName) {
	SharedModelTexture sharedModelTexture;

	SVIModelTexture *texture = NULL;

	SharedModelTextureMap::iterator iter = mModelTextureMap.find(fileName);

	if( iter == mModelTextureMap.end() ) {
		texture = createTextureFromAssetManager(fileName);
		if(texture != NULL) {
			sharedModelTexture = texture;

			addTexture(fileName, sharedModelTexture);
		}
		else {
			return NULL;
		}
	}
	else {
		sharedModelTexture = iter->second;
	}

	return sharedModelTexture;
}


SVIModelTexture* SVIModelManager::createTextureFromFile(const std::string &fileName) {
	// 2012/01/02 rareboy0112@facebook.com
	// Do not use cximage a temporary expedient.
	/*BYTE *pixels = NULL;
    long rawSize = 0;

    CxImage img;
	img.Load((const TCHAR*)(fileName.c_str()) );
    bool returnCode = img.Encode2RGBA(pixels, rawSize);

	if(!pixels) {
		return NULL;
	}

	SVIModelTexture *texture = new SVIModelTexture();
	texture->mFileName = fileName;
	texture->mWidth = img.GetWidth();
	texture->mHeight = img.GetHeight();
	texture->mBits = pixels;

    genTexture(texture);
	
	return texture;*/

	return NULL;
}


SVIModelTexture* SVIModelManager::createTextureFromAssetManager(const std::string &fileName) {
	AAssetManager *assetMgr = mSVIGLSurface->getRenderPartManager()->getAssetManager();
	
	if(assetMgr == NULL) {
		return NULL;
	}
	
	// load file inside assets
	AAsset *asset = AAssetManager_open(assetMgr, fileName.c_str(), AASSET_MODE_UNKNOWN);

	// the asset might not be found
	if(asset == NULL) {
		return NULL;
	}

	off_t bufferSize = AAsset_getLength(asset);

	const char *buffer = (const char *)AAsset_getBuffer(asset);

	int numBytesRemaining = AAsset_getRemainingLength(asset);

	SVIBool result = SVIFALSE;

	SVIModelTexture *texture = createTextureFromBuffer(buffer, numBytesRemaining);
	texture->mFileName = fileName;

	AAsset_close(asset);

	return texture;
}



SVIModelTexture* SVIModelManager::createTextureFromBuffer(const char *buffer, SVIUInt size) {

	
	return NULL;
}


SVIBool SVIModelManager::genTexture(SVIModelTexture *texture) {
	glGenTextures(1, &(texture->mTextureId));

	if( texture->mTextureId == 0 ) {
		LOGE("SVIModelManager::upload glGenTexture is failed !!!");
		return SVIFALSE;
	}

	glBindTexture(GL_TEXTURE_2D, texture->mTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->mWidth, texture->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->mBits);

	return SVITRUE;
}


SVIBool SVIModelManager::genTexture(SharedModelTexture texture) {
	return genTexture(texture.getPtr());
}


SVIBool SVIModelManager::addModel(std::string id, SharedModelNode model) {
	if( getModel(id) ) {
		mModelMap.find(id)->second = NULL;
		mModelMap.find(id)->second = model;
	}
	else {
		mModelMap.insert(StringModelPair( id, model) );
	}

	return SVITRUE;
}


SVIBool SVIModelManager::addTexture(std::string id, SharedModelTexture texture) {
	if( getTexture(id) ) {
		mModelTextureMap.find(id)->second = NULL;
		mModelTextureMap.find(id)->second = texture;
	}
	else {
		mModelTextureMap.insert(StringModelTexturePair( id, texture) );
	}

	return SVITRUE;
}


SharedModelNode SVIModelManager::getModel(std::string id) {
	SharedModelMap::iterator iter;
	if( (int)mModelMap.size() > 0 )
		 iter = mModelMap.find(id);
	else
		return NULL;
	
	if( iter != mModelMap.end() ) {
		return iter->second;
	}
	else { 
		return NULL;
	}
}


SharedModelTexture SVIModelManager::getTexture(std::string id) {
	SharedModelTextureMap::iterator iter;
	if( (int)mModelTextureMap.size() > 0 )
		 iter = mModelTextureMap.find(id);
	else
		return NULL;
	
	if( iter != mModelTextureMap.end() ) {
		return iter->second;
	}
	else { 
		return NULL;
	}
}


void SVIModelManager::restore() {
	SharedModelTextureMap::iterator iter = mModelTextureMap.begin();
	for(; iter != mModelTextureMap.end(); ++iter) {
		SVIModelTexture *texture = iter->second;
		if(texture !=NULL){
		if(texture->mTextureId != 0) {
			glDeleteTextures(1, &(texture->mTextureId));
		}
		genTexture(texture);
		}
	}
}


SVIBool SVIModelManager::removeModel(std::string id) {
	SharedModelMap::iterator iter;
	if( (int)mModelMap.size() > 0 )
		 iter = mModelMap.find(id);
	else
		return SVIFALSE;
	
	if( iter == mModelMap.end() ) {
		return SVIFALSE;
	}
	else {
		int count = iter->second.getCount();
		if( count <= 1 ) {
			LOGI( "Remove model data node(%s)", id.c_str() );
			mModelMap.erase(iter);
		}
	}

    return SVITRUE;
}


SVIBool SVIModelManager::removeTexture(std::string id) {
	SharedModelTextureMap::iterator iter;
	if( (int)mModelTextureMap.size() > 0 )
		 iter = mModelTextureMap.find(id);
	else
		return SVIFALSE;
	
	if( iter == mModelTextureMap.end() ) {
		return SVIFALSE;
	}
	else {
		int count = iter->second.getCount();
		if( count <= 1 ) {
			LOGI( "Remove model texture data(%s)", id.c_str() );
			mModelTextureMap.erase(iter);
		}
	}

    return SVITRUE;
}


} // end namespace SVI
