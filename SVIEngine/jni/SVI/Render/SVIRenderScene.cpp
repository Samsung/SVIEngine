#include "../SVICores.h"
#include "../Slide/SVIBaseSlide.h"
#include "../Slide/SVIBeamedSlide.h"
#include "../Slide/SVIProjectionSlide.h"
#include "SVIRenderPatch.h"
#include "SVIRenderScene.h"
#include "SVIViewport.h"
#include "SVIRenderer.h"
#include "SVITexture.h"
#include "SVITextureManager.h"
#include "SVIProgramManager.h"
#include "SVIFrameRenderer.h"
#include "SVIPolyClipInfo.h"


#include "SVIParticleRenderer.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Particle/SVIParticleEffectManager.h"
#include "../Particle/SVIParticles.h"

namespace SVI{
	#define SVI_MAX_RENDER_PATCH_COUNT 256
	#define SVI_MAX_RENDER_SLIDE_COUNT 1024

	#define SVI_PERPECTIVE_RATIO 1.0f / 500.0f

	#define SVI_BOTTOM 0 
	#define SVI_NORMAL 1
	#define SVI_TOPMOST 2


	SVIRenderScene::SVIRenderScene(SVIGLSurface* saGLSurface):
	mRootSlide(NULL),
	mTotalSceneMemory(0),
	mFullyReset(SVITRUE)
	{
		mRequestBuild = SVIFALSE;
		mSlideCount =0;
		mStencilDepth = 0;
		mSVIGLSurface = saGLSurface;
	}

	SVIRenderScene::~SVIRenderScene(){
		finalize();
		mSVIGLSurface = NULL;
	}	
		
	
	void SVIRenderScene::addSubSlides(SVIProjectionSlide * pParent){
		SVIProjectionSlide * pChild = NULL;
		SVIUInt subCount = pParent->getSubCount();
		if (subCount == 0) return;
		
		//2011-07-19 masterkeaton27@gmail.com
		//hidden flag check
		if (pParent->isHidden()) return;	

		for (SVIUInt n = 0; n < subCount; ++n){
			pChild = (SVIProjectionSlide *)pParent->getSub(n);
			//2011-06-23 masterkeaton27@gmail.comlink hidden property
			if (pChild->isHidden()) continue;									
			if (pChild->isTopMost()){
				mTopMostSlides.push_back(pChild);					
			}else if (pChild->isBottom()){
				mBottomSlides.push_back(pChild);			
			}else if (!pChild->isBottom() && !pChild->isTopMost()){
				SVIUInt nearestParentType = pChild->hasZOrderedParent();
				if (nearestParentType == BOTTOM){
					mBottomSlides.push_back(pChild);							
				}else if (nearestParentType == TOPMOST){
					mTopMostSlides.push_back(pChild);						
				}else{
					mNormalSlides.push_back(pChild);				
				}						
			}				
			addSubSlides(pChild);	
		}
	}

	void SVIRenderScene::clearSlides(){
		mSortedSlides.clear();	
		mBottomSlides.clear();
		mNormalSlides.clear();
		mTopMostSlides.clear();
	}

	void SVIRenderScene::requestBuild(){
		if (mRootSlide == NULL) return;
		mRequestBuild = SVITRUE;
	}

	SVIBool SVIRenderScene::buildRenderPatches(SVIProjectionSlide * pRootSlide){		
		mRootSlide = pRootSlide;
		//mRequestBuild = SVITRUE;
		mFullyReset = SVIFALSE;
		return SVITRUE;
	}


	SVIBool  SVIRenderScene::beginClipping(SVIProjectionSlide * pSlide){
		if (!pSlide->isClip()) return SVIFALSE;
		SVISlideOutfit outfit = pSlide->getOutfit();		
		
		SVIVector3 position = SVIVector3(
			outfit.mGlobalTransform._41,
			outfit.mGlobalTransform._42,
			outfit.mGlobalTransform._43);
		
		position.y = (mSVIGLSurface->getRenderer()->getViewSize().y - position.y);

		SVIVector3 pivot = outfit.mPivot;
		SVIVector3 scale = outfit.mScale;
		SVIVector3 size = outfit.mSize;

		SVIFloat width = scale.x * size.x *SVI_FRAME_FILTERED_RATIO;
		SVIFloat height = scale.y * size.y *SVI_FRAME_FILTERED_RATIO; 
		SVIFloat left = position.x*SVI_FRAME_FILTERED_RATIO - width * pivot.x;
		SVIFloat top = position.y*SVI_FRAME_FILTERED_RATIO - height * (1.0-pivot.y);

		SVIVector2 minV,maxV; 

		minV = SVIVector2(left,top);
		maxV = SVIVector2(left+width,top+height);
		
		//2011-08-04 masterkeaton27@gmail.com: to solve clipping bug 
		//2011-08-05 masterkeaton27@gmail.com: solution changed to area cropping.
		if (!mClippingStack.empty()){
			SVIPolyClipInfo* pParentClipInfo = (SVIPolyClipInfo*)mClippingStack.top();
			
			SVIFloat parentWidth = pParentClipInfo->m_ClipRegion.mSize.mWidth;
			SVIFloat parentHeight = pParentClipInfo->m_ClipRegion.mSize.mHeight;
			SVIFloat parentLeft = pParentClipInfo->m_ClipRegion.mOrigin.mX;
			SVIFloat parentTop = pParentClipInfo->m_ClipRegion.mOrigin.mY;

			SVIVector2 parentMinV,parentMaxV; 
			parentMinV = SVIVector2(parentLeft,parentTop);
			parentMaxV = SVIVector2(parentLeft+parentWidth,parentTop+parentHeight);

			//check rect to rect intersection
			SVIBool	isIntersect = 
				(maxV.x >= parentMinV.x) && (minV.x <= parentMaxV.x) && 
				(maxV.y >= parentMinV.y) && (minV.y <= parentMaxV.y) ? SVITRUE : SVIFALSE;
			
			//2011-08-05 masterkeaton27@gmail.com
			//let's crop clipping area based on parent's one.
			if (isIntersect){
				if (parentTop > top) top = parentTop;
				if (parentTop + parentHeight < top + height){
					SVIFloat heightGap = (top + height) - (parentTop + parentHeight);
					height = height - heightGap;
				}
				if (parentLeft > left) left = parentLeft;
				if (parentLeft + parentWidth < left + width){
					SVIFloat widthGap = (left + width) - (parentLeft + parentWidth);
					width = width - widthGap;
				}
			}else{
				//if not, don't need to worry about this case.
			}
		}


		SVIPolyClipInfo* pClipInfo = new SVIPolyClipInfo(left,top,width,height);
		pClipInfo->doBegin();
		mClippingStack.push(pClipInfo);
		return SVITRUE;
	}
	
	SVIBool	SVIRenderScene::endClipping(SVIProjectionSlide * pSlide){
		if (!pSlide->isClip()) return SVIFALSE;

		SVIPolyClipInfo* pTop = (SVIPolyClipInfo*)mClippingStack.top();
		if (pTop != NULL){
			pTop->doEnd();			
			SVI_SVIFE_DELETE(pTop);
		}
		mClippingStack.pop();
		if (!mClippingStack.empty()){
			SVIPolyClipInfo* pNewTop = (SVIPolyClipInfo*)mClippingStack.top();
			if (pNewTop != NULL) pNewTop->doBegin();				
		}
		return SVITRUE;
	}

	void SVIRenderScene::clearClippingStack(){
		while (!mClippingStack.empty()){
			SVIPolyClipInfo* pTop = (SVIPolyClipInfo*)mClippingStack.top();
			if (pTop != NULL){
				pTop->doEnd();
				SVI_SVIFE_DELETE(pTop);
			}
			mClippingStack.pop();
		}
	}


	SVIBool SVIRenderScene::beginStencilTest(SVIProjectionSlide* pSlide) {

		if(pSlide->isClip() != SVITRUE) 
			return SVIFALSE;

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_LEQUAL, mStencilDepth, 0xffffffff);
		glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
		buildRenderPatch(pSlide);
		++mStencilDepth;


		return SVITRUE;
	}


	SVIBool SVIRenderScene::endStencilTest(SVIProjectionSlide* pSlide) {
		if(pSlide->isClip() != SVITRUE) 
			return SVIFALSE;

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_LEQUAL, mStencilDepth, 0xffffffff);
		glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
		buildRenderPatch(pSlide);
		glDisable(GL_STENCIL_TEST);
		--mStencilDepth;


		return SVITRUE;
	}


	void SVIRenderScene::resetStencilTest() {
		mStencilDepth = 0;
	}


	void SVIRenderScene::renderSpecifyTraversal(SVIProjectionSlide * pSlide){
		clearClippingStack();
		requestBuild();
		renderTraversal(pSlide);
	}

	SVIBool SVIRenderScene::preTraversal(SVIProjectionSlide * pSlide){
		if (pSlide == NULL) return SVIFALSE;
		// 07/29 [marx] fix bug which hidden property did not work.
		if (pSlide->isHidden()) return SVITRUE;


		if (pSlide->isTopMost()){
			mTopMostSlides.push_back(pSlide);
			//return SVIFALSE;
		}else if (pSlide->isBottom()){
			mBottomSlides.push_back(pSlide);
			//return SVIFALSE;
		}

		SVIInt subSlideCount = pSlide->getSubCount();
		for (SVIInt n = 0; n < subSlideCount; ++n){
			SVIProjectionSlide * pSubSlide = (SVIProjectionSlide *)pSlide->getSub(n);
			preTraversal(pSubSlide);
		}

        return SVITRUE;
	}
		
	class SVIZPositionInfo{
	public:
		SVIZPositionInfo(){
			mZPosition = 0.0f;
			mIndex = 0;
		}
		SVIZPositionInfo(SVIFloat zposition, SVIUInt index){
			mIndex = index;
			mZPosition = zposition;
		}
	public:
		SVIFloat mZPosition;
		SVIUInt	mIndex;
	};

	SVIBool compareZPosition(const SVIZPositionInfo* left,const SVIZPositionInfo* right){
		//2011-08-22 masterkeaton27@gmail.com
		//sort algorithm changed for bug issues.
		if (left->mZPosition > right->mZPosition) return SVITRUE;
		else if (left->mZPosition == right->mZPosition) {
			if (left->mIndex < right->mIndex) return SVITRUE;
		}
		return SVIFALSE;
	}
	

	SVIBool	SVIRenderScene::renderTraversal(SVIProjectionSlide * pSlide){
		if (pSlide == NULL) return SVIFALSE;
		// 07/29 [marx] fix bug which hidden property did not work.
		if (pSlide->isHidden()) return SVITRUE;

		SVIBool isUltimateRoot = pSlide->isAbsoluteRoot();
		SVIBool isTransparent = pSlide->isTransparent();
		SVIBool isRenderObject = (!isUltimateRoot && !isTransparent) ? SVITRUE : SVIFALSE;

		pSlide->updateParticleEffect();

		if (isRenderObject) {

#if USE_STENCIL_BUFFER
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			if(mStencilDepth > 0 && !pSlide->isRenderTarget()) {
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_LEQUAL, mStencilDepth, 0xffffffff);
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			}
			else if(pSlide->isRenderTarget()) {
				glDisable(GL_STENCIL_TEST);
			}
#endif // #if USE_STENCIL_BUFFER
			
			buildRenderPatch(pSlide);
		}


		SVIBeamedSlide * pLinkedSlide = (SVIBeamedSlide *)pSlide->getLink();
		// 2012/06/20 rareboy0112@facebook.com
		// change code : pSlide->has3DScene() != NULL -> pSlide->has3DScene() != SVIFALSE
		if (pLinkedSlide != NULL && pSlide->has3DScene() != SVIFALSE){
			SVIBool isCapture = pLinkedSlide->getCapture();			
			if (isCapture){
				(pSlide->getSVIGLSurface())->getSlideCapture()->start(pSlide);//Multi-Instance-Support
			}else if (pSlide->getTextureContainer()->hasCaptureData()){
				(pSlide->getSVIGLSurface())->getSlideCapture()->stop(pSlide);//Multi-Instance-Support
			}
		}
		
		//2011-10-17 check for render patch
		if (pSlide->isRenderTarget()){
			//2011-11-30 masterkeaton27@gmail.com
							
			return SVIFALSE;
		}

		SVIInt subSlideCount = pSlide->getSubCount();
		SVIBool checkZPosition = SVITRUE;	

		// test code to check slide count.
		if( isUltimateRoot )
			mSlideCount = 0;
		else
			mSlideCount += subSlideCount;

		std::vector<SVIZPositionInfo*> templist;
		for (SVIInt n = 0; n < subSlideCount; ++n){
			SVIProjectionSlide * pSubSlide = (SVIProjectionSlide *)pSlide->getSub(n);					

			SVIBeamedSlide* beamedSlide = const_cast<SVIBeamedSlide*>(pSubSlide->getLink());
			SVIFloat fZPosition = beamedSlide->getZPosition();
			
			templist.push_back(new SVIZPositionInfo(fZPosition,n));
		}

		std::sort(templist.begin(),templist.end(),compareZPosition);

// 2011/10/25 rareboy0112@facebook.com
		// select clipping mode
#if USE_STENCIL_BUFFER
		beginStencilTest(pSlide);
#else
		beginClipping(pSlide);
#endif

		for (SVIUInt n = 0; n < templist.size(); ++n){
			SVIProjectionSlide * pSubSlide = (SVIProjectionSlide *)pSlide->getSub(templist[n]->mIndex);			
			SVI_SVIFE_DELETE(templist[n]);
			if (pSubSlide->isTopMost() || pSubSlide->isBottom())  continue;				
			renderTraversal(pSubSlide);
		}

#if USE_STENCIL_BUFFER
		endStencilTest(pSlide);
#else
		endClipping(pSlide);
#endif

		templist.clear();
				
		

		return SVITRUE;
	}

	SVIBool	SVIRenderScene::existPatch(SVIRenderPatch * pPatch){
		for (SVIUInt n=0; n < mRenderPatches.size(); ++n){
			if (pPatch == mRenderPatches[n]) return SVITRUE;
		}
		return SVIFALSE;
	}

	void SVIRenderScene::buildUnRenderedRenderPatch(SVIProjectionSlide * pSlide){

		SVIProgram * pProgram = NULL;
		SVIRenderPatch * pPatch = pSlide->getRenderPatch();
		if (pPatch == NULL){

			pPatch = new SVIRenderPatch(mSVIGLSurface);
			pSlide->setRenderPatch(pPatch);
			pPatch->setOwner(pSlide);
			mRenderPatches.push_back(pPatch); 
		}else{
			pPatch->setOwner(pSlide);
		}			
		SVISlideOutfit * pOutfit = (SVISlideOutfit*)&pSlide->getOutfit();

		//2011-10-14 masterkeaton27@gmail.com: null-pointer access violation protection code added.
		if (pPatch->getOutfit() == NULL){						
			//2011-07-01 masterkeaton27@gmail.com
			//changed checking routines for dynamic texture setting.
            SVISlideTextureContainer * pContainer = pSlide->getTextureContainer();
            SVITexture * pTexture = pContainer->getTexture();
            if (pTexture != NULL && !pTexture->getValid()){
                pSlide->setTexture(NULL);
            }

            if (!pContainer->isEmpty()){
                pPatch->setTextureContainer(pContainer);
                if (pContainer->hasTexture()){					
                    pPatch->setType(SVIRenderPatch::SVI_PATCH_TEXTURE);									
                }else{
					pPatch->setType(SVIRenderPatch::SVI_PATCH_COLOR);					
				}		
			}else{
				pPatch->setTextureContainer(NULL);
			}
			pPatch->initialize(pOutfit);
		}
	}

	SVIBool SVIRenderScene::buildRenderPatch(SVIProjectionSlide * pSlide){
		SVIProgram * pProgram = NULL;
		SVIRenderPatch * pPatch = pSlide->getRenderPatch();
		if (pPatch == NULL){
			pPatch = new SVIRenderPatch(mSVIGLSurface);
			pSlide->setRenderPatch(pPatch);
			pPatch->setOwner(pSlide);
			mRenderPatches.push_back(pPatch); 
		}else{
			pPatch->setOwner(pSlide);
		}			

		SVISlideOutfit * pOutfit = (SVISlideOutfit*)&pSlide->getOutfit();
		
		//2011-10-14 masterkeaton27@gmail.com: null-pointer access violation protection code added.
		if (mRequestBuild || pPatch->getOutfit() == NULL){						
			//2011-07-01 masterkeaton27@gmail.com
			//changed checking routines for dynamic texture setting.
			SVISlideTextureContainer * pContainer = pSlide->getTextureContainer();
			SVITexture * pTexture = pContainer->getTexture();
			if (pTexture != NULL && !pTexture->getValid()){
				pSlide->setTexture(NULL);
			}

			if (!pContainer->isEmpty()){
				pPatch->setTextureContainer(pContainer);
				if (pContainer->hasTexture()){					
					pPatch->setType(SVIRenderPatch::SVI_PATCH_TEXTURE);
				}else{
					pPatch->setType(SVIRenderPatch::SVI_PATCH_COLOR);
				}		
			}else{
				pPatch->setTextureContainer(NULL);
			}

			pPatch->initialize(pOutfit);
		}
		pPatch->render();

		ParticleEffectList* particleList = pSlide->getParticleEffectList();
		if( particleList != NULL && particleList->size() > 0 ){
			ParticleEffectList::iterator iter = particleList->begin();
			while (iter != particleList->end()){
				SVIParticleEffect * pEffect = *iter;

				//2012-06-22 masterkeaton27@gmail.com
				//add parent transform for global transform calculation
				pEffect->setParentTransform((SVIMatrix*)&pSlide->getOutfit().mGlobalTransform);
				mSVIGLSurface->getParticleRenderer()->immediateRender(pEffect);
				++iter;
			}
		}

		return SVITRUE;
	}

	//2011-07-22 masterkeaton27@gmail.com
	//check useless patch's existence for deletion
	void SVIRenderScene::removeUselessPatches(){
		/*
		std::vector<SVIRenderPatch*>::iterator iter = mRenderPatches.begin();
		while (iter != mRenderPatches.end()){
			SVIRenderPatch * pPatch = *iter;
			if (pPatch->getOwner() == NULL){
				pPatch->finalize();
				SVI_SVIFE_DELETE(pPatch);
				iter = mRenderPatches.erase(iter);
				continue;
				}
				iter++;
				}*/
	}

	//2011-05-26 masterkeaton27@gmail.comfor futher MT integration
	void SVIRenderScene::updateFrame(){
		//2011-07-22 masterkeaton27@gmail.com
		//check useless patch's existence for deletion
		//removeUselessPatches();
	}


	void SVIRenderScene::prepareRender(){		
		clearClippingStack();
		if (mRequestBuild) {
			mBottomSlides.clear();
			mTopMostSlides.clear();
			preTraversal(mRootSlide);
			mFullyReset = SVIFALSE;
		}
	}
	void SVIRenderScene::renderLowLayer(){
		for (SVIUInt n = 0; n < mBottomSlides.size(); ++n){
			renderTraversal(mBottomSlides[n]);
		}
	}
	void SVIRenderScene::renderMiddleLayer(){
		renderTraversal(mRootSlide);
	}
	void SVIRenderScene::renderHighLayer(){
		for (SVIUInt n = 0; n < mTopMostSlides.size(); ++n){
			renderTraversal(mTopMostSlides[n]);
		}
	}
	void SVIRenderScene::releaseRender(){
		mRequestBuild = SVIFALSE;		
		mFullyReset = SVITRUE;
	}

	void SVIRenderScene::renderFrame(){
		mSlideCount = 0;

#if USE_STENCIL_BUFFER
		resetStencilTest();
#endif
		prepareRender();
		renderLowLayer();
		renderMiddleLayer();
		renderHighLayer();
		releaseRender();		
		//test code to check slide count
		//if( mSlideCount > 0 )
		//	LOGE("Rendered Slide Count : %d", mSlideCount);
	}
	

	void SVIRenderScene::resetFrame(){
		SVIRenderPatch * pPatch = NULL;
		/*
		for (int n = 0; n < mRenderPatches.size(); ++n){
			pPatch = mRenderPatches[n];
			if (pPatch->getOwner() == NULL){	
				SVI_SVIFE_DELETE(pPatch);
			}
		}
		*/
		mRenderPatches.clear();
		clearSlides();
		// 2011/10/18 rareboy0112@facebook.com
		// Need prepareRender methods
		mRequestBuild = SVITRUE;
	}

	void SVIRenderScene::finalize(){
		resetFrame();
	}
};
