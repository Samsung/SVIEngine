#pragma once

namespace SVI {

class SVISlideTextureContainer;
class SVIRenderPatch;
	

class SVIPoly {
public:
	SVIPoly(SVIGLSurface* saGLSurface);
	virtual ~SVIPoly();

public:
	enum {
		SVI_POLY_TRI_STRIP = 0, SVI_POLY_TRI_LIST, SVI_POLY_TRI_FAN
	};

public:
	SVIBool build();
	SVIBool buildCenter();
	void releaseUpdate() {
		mNeedToUpdate = SVIFALSE;
		mNeedToUpdateTexcoord = SVIFALSE;
	}
	virtual SVIInt deallocate();
	virtual SVIInt allocate(SVIInt verticeCount);
	virtual SVIInt allocate(SVIInt verticeCount, SVIInt indicesCount);
	virtual SVIInt allocate(SVIInt verticeCount, SVIInt indicesCount,
			SVIInt normalCount);
public:
	//getter
	const SVIVector4& getColor() ;
	SVIUShort* getIndices();
	SVIVector3 * getVertices();
	SVIVector3 * getNormals();
	SVIVector2 * getUnitCoords() ;
	SVIVector2 * getTextureCoordinates() ;
	const SVIVector2& getTextureScale();
	const SVIVector2& getTextureRegionOffset();
	const SVIVector2& getTextureRegionSize();

	const SVIVector2& getBackFaceTextureRegionOffset();
	const SVIVector2& getBackFaceTextureRegionSize();

	const SVIVector2& getTextureScaleFitRegionOffset();
	const SVIVector2& getTextureScaleFitRegionSize();

	//2011.12.09 jongchae.moon
	SVIBool      getIsAntiAliasing();
	SVIInt       getOriginalVerticeCount();
	SVIInt       getOriginalIndiceCount();
	SVIFloat*    getAAcoords();


	const SVIVector3& getPivot();
	const SVIVector3& getScale();
	const SVIFloat& getOpacity();
	const SVIVector3& getSize();
	const SVIVector3& getCenter();
	const SVIMatrix& getTransform();
	SVIUInt getVerticeCount();
	SVIUInt getIndicesCount();
	SVIUInt getPrimitiveType();
	SVIUInt getGeometryType();
	SVIUInt getMemoryUsed();
	SVIBool isFreed();
	SVIBool isOrthogonal();
	SVIBool isCustomRTE();
    SVIBool isUsingMultiTexture();
	SVIBool isAddBlend();
	SVIBool hasBorder();
	SVIBool isScaleToFit();
	SVIBool hasAdditionalRender();
	SVIBool hasAdditionalUpdate();
	SVIBool hasIndices();
	//2011-11-08 masterkeaton27@gmail.com
	SVIBool hasNormals();
	//2011-11-16 masterkeaton27@gmail.com
	SVIBool hasTexcoords();
	//2011-11-17 masterkeaton27@gmail.com
	SVIBool hasBackFace();	//2011-07-20 masterkeaton27@gmail.com
    //2012/12/10 rareboy0112@facebook.com
    SVIBool hasMorphing();
    void resetMorphing();
	SVIBool isValid();
public:
	//setter
	void setTextureScaleFitRegion(const SVIVector2& offset,
			const SVIVector2& size);
	void setBackFaceTextureRegion(const SVIVector2& offset,
			const SVIVector2& size);
	void setTextureRegion(const SVIVector2& offset, const SVIVector2& size);
	void setTextureScale(const SVIVector2 & scale);
	void setColor(const SVIVector4 & color);
	void setOpacity(const SVIFloat & opacity);
	void setPivot(const SVIVector3 & pivot);
	void setScale(const SVIVector3 & scale);
	void setSize(const SVIVector3 & size);
	void setPrimitiveType(SVIUInt type);
	void setGeometryType(SVIUInt type);
	void setTransform(const SVIMatrix & transform);
	void setOrthogonal(SVIBool isOrtho);
	void setCustomRTE(SVIBool isCusRTE);
	void setAddBlend(SVIBool bisAddBlend);
	void setBorder(SVIBool border);
	//2012-02-21 marx.kim
	void setRenderPatch(SVIRenderPatch* patch); 
public:
	//virtual void additionalRender(SVIProgram * pProgram,SVIBool bUseTexture, SVIUInt textureId){}
    virtual void additionalRender(SVISlideTextureContainer * pContainer) {}
	virtual void additionalUpdate() {}
	virtual void prepareBackFace() {}
	virtual void prepareFrontFace() {}

protected:
	virtual void setup() {}
	virtual void buildVertices();
	virtual void buildTextureCoordinates();
	virtual void buildExtraWork();

	//2011-06-28 masterkeaton27@gmail.com
	//rendering optimization
	virtual void buildIndices();

	//2011-11-09 masterkeaton27@gmail.com
	virtual void buildNormals();

    

protected:
	SVIBool mNeedTexcoords;
	SVIBool mNeedNormals;
	SVIBool mNeedIndices;
	SVIBool mNeedToUpdate;
	SVIBool mNeedToUpdateTexcoord;
	SVIBool mHasBackFace;
	SVIBool mHasAdditionalRender;
	SVIBool mHasAdditionalUpdate;
    SVIBool mNeedMorphing;

protected:
	SVIBool mIsOrthogonal;
	SVIBool mIsCustomRTE;
	SVIBool mIsAddBlend;
	SVIBool mHasBorder;
    SVIBool mIsUsingMultiTexture;

	SVIBool mMemoryFreed;
	SVIUInt mMemoryUsed;
	SVIUInt mVerticeCount;
	SVIUInt mRenderPritiveType;
	SVIUInt mRenderGeometryType;

	SVIVector3* mVertices;
	SVIVector2* mUnitCoords;
	SVIVector2* mTextureCoords;

	//2011-06-28 masterkeaton27@gmail.com
	SVIUShort* mIndices;
	SVIUInt mIndicesCount;

	//2011-11-08 masterkeaton27@gmail.com
	SVIVector3 * mNormals;
	SVIUInt mNormalCount;

	SVIFloat mOpacity;
	SVIVector4 mColor;
	SVIVector3 mPivot;
	SVIVector3 mScale;
	SVIVector2 mTextureScale;
	SVIVector3 mSize;
	SVIVector3 mCenter;
	SVIVector2 mBackFaceTextureRegionOffset;
	SVIVector2 mBackFaceTextureRegionSize;
	SVIVector2 mTextureRegionOffset;
	SVIVector2 mTextureRegionSize;

	SVIVector2 mTextureScaleFitOffset;
	SVIVector2 mTextureScaleFitSize;

	SVIMatrix mTransform;

	//2011.12.06 jongchae.moon
protected:
	SVIBool mIsAA;
	SVIFloat mfOriginBorderWidth;
	SVIFloat* mAACoord;

	SVIInt mOriVerticeCount;
	SVIInt mOriIndicesCount;

	//2012-02-21 marx.kim
	SVIRenderPatch* mRenderPatch;

	SVIGLSurface* mSVIGLSurface;
};

};
