#pragma once

#include "../Slide/SVISlide.h"
#include "SVISceneNode.h"

namespace SVI{

	//2011-11-07 masterkeaton27@gmail.com
	//let's pretend be scene can't has a child. (just for now)
	class SVI3DCamera;
	class SVI3DScene {
	public:		
		SVI3DScene(SVIGLSurface *surface);
		SVI3DScene(SVIGLSurface *surface, SVIFloat width, SVIFloat height);
		virtual ~SVI3DScene();
	private:
		SVI3DScene(const SVI3DScene& rhs){}
		void operator=(const SVI3DScene& rhs){}
	public:
		void run(float fDeltaTime);

	protected:
		void update(float fDeltaTime);
		void render();

	public:	
		void initialize();
		void reset();
		void finalize();

	public:
		void setName(const SVIChar * pstrName);
		void setSize(SVIFloat width, SVIFloat height);
		void setCamera(SVI3DCamera* pCamera);
		
		// 2011/11/30 rareboy0112@facebook.com
		void addSceneNode(SVISceneNode *sceneNode);

	public:
		SVISceneNode* getRootSceneNode() { return mRootSceneNode; }

	public:
		const SVIChar *		getName(){ return mName;}
		const SVI3DCamera *	getCamera(){ return mCamera; }
		const SVISize		getSize(){ return mSize;}

		void setParent(SVISlide *parentSlide) { mParentSlide = parentSlide; }

	protected:
		void updateCamera();

	private:
		SVISize			mSize;
		SVIChar			mName[64];
		SVI3DCamera *	mCamera;
		SVISceneNode *mRootSceneNode;
		SVISlide *mParentSlide;

		SVIGLSurface *mSVIGLSurface;
	};


}