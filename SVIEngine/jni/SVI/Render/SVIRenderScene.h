#pragma once

namespace SVI{

	class SVIPolyClipInfo;
	class SVIProjectionSlide;
	class SVIRenderPatch;


	class SVISceneTexture{
	public:
		SVISceneTexture();
		virtual ~SVISceneTexture();
	public:
		void initialize();
		void finalize();
		void restore();
	protected:

	private:
		SVIInt				mID;
		SVIProjectionSlide * mOwner;
	};

	class SVIRenderScene{
	public:
		SVIRenderScene(SVIGLSurface* saGLSurface);
		virtual ~SVIRenderScene();

	public:
		void			requestBuild();		
		SVIBool			buildRenderPatches(SVIProjectionSlide * rootSlide);
	public:
		virtual void	updateFrame();
		virtual void	renderFrame();
		virtual void	resetFrame();
		virtual void	finalize();

	public:
		std::vector<SVIRenderPatch *>& getPatches(){ return mRenderPatches; }

	protected:
		void prepareRender();
		void renderLowLayer();
		void renderMiddleLayer();
		void renderHighLayer();
		void releaseRender();

	public:
		void renderSpecifyTraversal(SVIProjectionSlide * pSlide);
		 void buildUnRenderedRenderPatch(SVIProjectionSlide * pSlide);
	protected:
		std::stack<SVIPolyClipInfo*> mClippingStack;

		SVIBool	preTraversal(SVIProjectionSlide * pSlide);
		SVIBool	renderTraversal(SVIProjectionSlide * pSlide);

		SVIBool	buildRenderPatch(SVIProjectionSlide * pSlide);

		SVIBool  beginClipping(SVIProjectionSlide * pSlide);
		SVIBool	endClipping(SVIProjectionSlide * pSlide);

		// 2011/10/25 rareboy0112@facebook.com
		// added by scissor test the limits of the clipping.
		SVIBool	beginStencilTest(SVIProjectionSlide* pSlide);
		SVIBool	endStencilTest(SVIProjectionSlide* pSlide);
		void	resetStencilTest();
		
		void    clearClippingStack();
		SVIBool	existPatch(SVIRenderPatch * pPatch);
	
	protected:
		
		void		removeUselessPatches();

	protected:
		SVIProjectionSlide *						mRootSlide;
		std::vector<SVIProjectionSlide *>		mSortedSlides;
		std::vector<SVIRenderPatch *>			mRenderPatches;

	protected:
		std::vector<SVIProjectionSlide *>		mTopMostSlides;
		std::vector<SVIProjectionSlide *>		mNormalSlides;
		std::vector<SVIProjectionSlide *>		mBottomSlides;

		void clearSlides();
		void addSubSlides(SVIProjectionSlide * pParent);		
	protected:
		SVIUInt								mTotalSceneMemory;
		SVIBool								mFullyReset;
		SVIBool								mRequestBuild;

	private :
		SVIInt								mSlideCount;
		SVIInt								mStencilDepth;
		SVIGLSurface* mSVIGLSurface;
	};
};

