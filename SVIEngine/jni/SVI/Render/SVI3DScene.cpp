#include "../SVICores.h"
#include "SVI3DScene.h"
#include "SVI3DCamera.h"
#include "SVIViewport.h"
#include "SVIRenderer.h"
#include "SVIDebugRenderer.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

	SVI3DScene::SVI3DScene(SVIGLSurface * surface){
		sprintf(mName,"UnKnownScene");

		if(surface == NULL) {
			LOGE("Invalid argument to the SVI3DScene::SVI3DScene(SVIGLSurface *surface) \n");
			exit(0); // To check for errors till stabilization.. To be removed later. 
		} else {
			mSVIGLSurface = surface;
		}

		mCamera = new SVI3DFreeCamera();
		SVI3DFreeCamera * pCamera = (SVI3DFreeCamera *)mCamera;
		pCamera->setViewMode(SVI3DFreeCamera::ViewMode_Perspective);
		mParentSlide = NULL;
		SVIVector2 size =  mSVIGLSurface->getRenderer()->getViewSize();
		
		mSize.mWidth = size.x; 
		mSize.mHeight = size.y;

		mRootSceneNode = new SVISceneNode(surface);


	}

	SVI3DScene::SVI3DScene(SVIGLSurface *surface, SVIFloat width, SVIFloat height) : mSize(width, height){
		sprintf(mName,"UnKnownScene");

		if(surface == NULL) {
			LOGE("Invalid argument to the SVI3DScene::SVI3DScene(SVIGLSurface *surface) \n");
			exit(0); // To check for errors till stabilization.. To be removed later. 
		} else {
			mSVIGLSurface = surface;
		}

		
		mCamera = new SVI3DFreeCamera();
		SVI3DFreeCamera * pCamera = (SVI3DFreeCamera *)mCamera;
		pCamera->setViewMode(SVI3DFreeCamera::ViewMode_Perspective);
		mParentSlide = NULL;
		mRootSceneNode = new SVISceneNode(surface);
	}

	SVI3DScene::~SVI3DScene(){
		SVI_SVIFE_DELETE(mCamera);
		SVI_SVIFE_DELETE(mRootSceneNode);

		mSVIGLSurface = NULL;
	}


	void SVI3DScene::update(float fDeltaTime){
		updateCamera();

	}

	void SVI3DScene::render(){
		mSVIGLSurface->getDebugRenderer()->render3DGrid(mCamera,15.0f,20);

		SVIVector3 origin(0,0,0);
		SVIVector3 xMax(50,3,3);
		SVIVector3 yMax(3,50,3);
		SVIVector3 zMax(3,3,50);

		SVIVector4 xColor(1,0,0,1); 
		SVIVector4 yColor(0,1,0,1);
		SVIVector4 zColor(0,0,1,1);
		
		//SVIDebugRenderer::render3DBox(mCamera,origin,xMax,xColor);
		//SVIDebugRenderer::render3DBox(mCamera,origin,yMax,yColor);
		//SVIDebugRenderer::render3DBox(mCamera,origin,zMax,zColor);
		
		static float radian = 0.0f;
		const float radius = 100.0f;
		radian += 0.01f;
		if (radian > SVI_2_PI) radian = 0.0f;
		
		mRootSceneNode->setCamera(mCamera);
		mRootSceneNode->render(SVISceneNode::TRIANGLES);
		
		SVI3DFreeCamera * pCamera = (SVI3DFreeCamera *)mCamera;
		//pCamera->m_Input.m_RotateX = 200.0f;
	}

	void SVI3DScene::initialize(){
	
	}
	void SVI3DScene::reset(){}
	void SVI3DScene::finalize(){}

	void SVI3DScene::addSceneNode(SVISceneNode *sceneNode) {
		mRootSceneNode->addChild(sceneNode);
	}

	void SVI3DScene::setName(const SVIChar * pstrName){
		sprintf(mName,"%s",pstrName);
	}	

	void SVI3DScene::setSize(SVIFloat width, SVIFloat height){
		mSize.mWidth = width; 
		mSize.mHeight = height;
	}

	void SVI3DScene::setCamera(SVI3DCamera* pCamera){
		if (pCamera != NULL && pCamera != mCamera){
			SVI_SVIFE_DELETE(mCamera);
		}
		mCamera = pCamera;
	}


	void SVI3DScene::run(float fDeltaTime){
		update(fDeltaTime);
		render();
	}

	void SVI3DScene::updateCamera(){
		SVI3DFreeCamera * pCamera = (SVI3DFreeCamera *)mCamera;
		SVIFloat aspectRatio = mSize.mWidth / mSize.mHeight;
		SVIFloat fov = SVI_DEGTORAD(60.0f);
		SVIFloat near = 1.0f;
		SVIFloat far = 500.0f;
		pCamera->updateCamera(aspectRatio,fov,near,far);
		LOGE("3D: update Camera");
	}
}
