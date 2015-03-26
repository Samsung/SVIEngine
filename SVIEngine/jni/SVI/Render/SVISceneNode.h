#ifndef __SVI_SCENENODE_H_
#define __SVI_SCENENODE_H_

#include <string>
#include <vector>
#include "../BaseType/SVIBaseTypes.h"
#include "../SVIDefines.h"
#include "../Mathematics/SVIMath.h"
#include "SVI3DCamera.h"


namespace SVI {

	class SVISceneNode;
	class SVIGLSurface;

	typedef std::vector<SVISceneNode *> SceneNodeList;

	// Creation Date	: 2011/11/16 rareboy0112@facebook.com
	// Description		: render transform node class
	// Last Modified	: 2011/11/28 rareboy0112@facebook.com
	class SVISceneNode {
	public:

		enum NodeType {
			TRANSFORM_NODE = 0,
			ROOT_MODEL_NODE = 1,
			SVITATIC_MODEL_NODE = 2,
			SKELETAL_MODEL_NODE = 3,
			NODE_COUNT
		};

		// same as GL.
		enum BeginModeType {
			POINTS = 0,
			LINES = 1,
			LINE_LOOP = 2,
			LINE_STRIP = 3,
			TRIANGLES = 4,
			TRIANGLE_STRIP = 5,
			TRIANGLE_FAN = 6,
			QUADS = 7,
			QUAD_STRIP = 8,
			POLYGON = 9,
			RENDER_MODE_COUNT
		};

		SVISceneNode(SVIGLSurface* mSVIGLSurface);

		virtual ~SVISceneNode();

		virtual NodeType getNodeType();

		virtual void render(BeginModeType beginType = TRIANGLES);

		virtual void update();

		virtual SVISceneNode* deepCopy(SVIBool withSubSlide = SVITRUE);

		SVISceneNode* getParent();

		void setParent(SVISceneNode *parent);

		SVIBool addChild(SVISceneNode *node);

		SVIBool removeChild(SVISceneNode *node);

		void setLocalTransform(SVIMatrix *localTM);

		void setLocalTransform(SVIMatrix &localTM);

		const SVIMatrix* getLocalTransform() { return mLocalTransform; }

		const SVIMatrix* getGlobalTransform() { return mGlobalTransform; }

		void setId(std::string id);

		const std::string getId();
		
		void setCamera(SVI3DCamera *camera);

		void setPosition(SVIVector3 &position);
		SVIVector3 getPosition();

		void setRotation(SVIVector4 &rotation);
		SVIVector4 getRotation();

		void setScale(SVIVector3 &scale);
		SVIVector3 getScale();

	protected:
		void copyChildNodeList(SVISceneNode *parent);

	private:
		SVISceneNode(const SVISceneNode& rhs);
		SVISceneNode& operator=(const SVISceneNode& rhs);
	protected:
		SVISceneNode *mParent;
		SceneNodeList mChildList;
		std::string mId;
		SVIBool mLocalTransformChanged;
		SVIMatrix *mLocalTransform;
		SVIMatrix *mGlobalTransform;

		SVIVector3 mPosition;
		SVIVector4 mRotation;
		SVIVector3 mScale;

		SVI3DCamera *mCamera;

		SVIGLSurface* mSVIGLSurface;
	};

} // end namespace SVI

#endif // #ifndef __SVI_SCENENODE_H_