#pragma once

namespace SVI {

struct SVISlideOutfit;
class SVIProjectionSlide;
class SVISlideTextureContainer;
class SVIPoly;
class SVIProgramHandler;
class SVIProgramParams;
class SVIGLSurface;

class SVIRenderPatch {
public:
	enum GeometryType {
		SVI_PATCH_QUAD           = 0,
		SVI_PATCH_ROUND          = 1,
		SVI_PATCH_TWIST          = 3,
		SVI_PATCH_EXPLODE        = 4,
		SVI_PATCH_PANNEL         = 11,
		SVI_PATCH_BRICK          = 12,
        SVI_PATCH_DOME           = 13,
        SVI_PATCH_DRUM           = 14,
        SVI_PATCH_CONECURVE      = 15,
        SVI_PATCH_PATH           = 16,
        SVI_PATCH_TESSELATION	= 18,
	};

	enum PatchType {
		SVI_PATCH_COLOR = 0, SVI_PATCH_TEXTURE, SVI_PATCH_COLORTEXTURE,
	};

public:
	SVIRenderPatch(SVIGLSurface *surface);
	virtual ~SVIRenderPatch();

protected:

public:
	void setOwner(SVIProjectionSlide* pSlide) {
		mOwnerSlide = pSlide;
	}
	void setType(SVIUInt type) {
		mType = type;
	}
	void setGeometryType(SVIUInt type) {
		mGeometryType = type;
	}
	void setProgramId(SVIUInt id) {
		mProgramId = id;
	}
	void setTextureContainer(SVISlideTextureContainer * pContainer) {
		mTextureContainer = pContainer;
	}
	void setTextureScale(const SVIVector2 & scale) {
		mTextureScale = scale;
	}
	//2012-02-21 marx.kim
	void setPageStatus(int status) {
		mPageStatus = status;
	}
public:
	SVIProjectionSlide * getOwner() {
		return mOwnerSlide;
	}
	SVIUInt getType() {
		return mType;
	}
	SVIUInt getGeometryType() {
		return mGeometryType;
	}
	SVIUInt getProgramId() {
		return mProgramId;
	}
	SVIUInt getMemoryUsed() {
		return mMemoryUsed;
	}
	const SVIVector2 & getTextureScale() {
		return mTextureScale;
	}
	SVISlideTextureContainer * getTextureContainer() {
		return mTextureContainer;
	}
	SVISlideOutfit* getOutfit() {
		return mOutfit;
	}
	//2012-02-21 marx.kim
	SVIInt getPageStatus() {
		return mPageStatus;
	}

	SVIBool isRenderTarget();
	SVIBool isUsingTexture() {
		return mType == SVI_PATCH_TEXTURE || mType == SVI_PATCH_COLORTEXTURE;
	}
	SVIBool isUsingColor() {
		return mType == SVI_PATCH_COLOR || mType == SVI_PATCH_COLORTEXTURE;
	}

	SVIBool isLastChild();
	SVIBool hasNoChild();
	SVIBool hasClipParent();
public:
	virtual void render();
	//2011-06-15 masterkeaton27@gmail.com
	virtual void renderShadow();
    //2012/12/10 rareboy0112@facebook.com
    //only need VI Effect
    virtual void renderMorphing();
	virtual SVIBool initialize(SVISlideOutfit * pOutfit);
	virtual SVIBool finalize();

protected:
	virtual SVIBool buildPolyObject(SVISlideOutfit * pOutfit);
	SVIBool	createPolyObject();
	void	setPolyType(SVISlideOutfit * pOutfit);	
	void	setPolyProperties(SVIPoly * pPoly, SVISlideOutfit * pOutfit);
protected:
	//2011-11-03 masterkeaton27@gmail.com
	//clean up render code
	void preProcessRenderTargetEffect(SVIProgramParams * pParams);
	void preProcessLighting(SVIProgramParams * pParams);
	void preProcessBlending(SVIProgramParams * pParams);
	void preProcessTexture(SVIProgramParams * pParams);
	void preProcessShaderEffect(SVIProgramParams * pParams);
	void preProcessDeformation(SVIProgramParams * pParams);
    void preProcessTesselation(SVIProgramParams *pParams);

    //2013-06-26 masterkeaton27@gmail.com
    void preProcessFilter(SVIProgramParams * pParams);

	void processShaderEffect(SVIProgramHandler * pHandler);
	void processDeformation(SVIProgramHandler * pHandler);
    void processFilter(SVIProgramHandler * pHandler);
	void processRenderTargetEffect(SVIProgramHandler * pHandler);
	void processTextureScale(SVIProgramHandler * pHandler);
	void processTextureBlendOperation(SVIProgramHandler * pHandler);
	void processColoring(SVIProgramHandler * pHandler);
	void processLighting(SVIProgramHandler * pHandler);
	void proccessGeometry(SVIProgramHandler * pHandler);

	void processFrontFaceRender(SVIProgramHandler * pHandler);
	void processBackFaceRender(SVIProgramHandler * pHandler);
protected:
	std::vector<SVIProjectionSlide*> mSlides;
	SVIUInt mProgramId;

	SVISlideTextureContainer * mTextureContainer;

	//SVIUInt						mTextureId;
	//2011-08-01 masterkeaton27@gmail.com: for loop optimization
	//SVITexture *					mTexture;

	SVIProjectionSlide* mOwnerSlide;
	SVISlideOutfit* mOutfit;

	SVIPoly* mPoly;
	SVIUInt mType;
	SVIUInt mGeometryType;
	SVIUInt mMemoryUsed;

	SVIGLSurface* mSVIGLSurface; // Multi-instance-Support

protected:
	SVIBool mUseRenderTarget;
	SVIVector2 mTextureScale;
	//2012-02-21	 marx.kim
	SVIInt mPageStatus;

	SVIUInt filterTexture0;
};
}
;
