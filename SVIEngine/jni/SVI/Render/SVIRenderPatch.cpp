#include "../SVICores.h"

#include "SVIViewport.h"

#include "SVIRenderPatch.h"


#include "../Slide/SVIBaseSlide.h"
#include "../Slide/SVIProjectionSlide.h"

#include "./Geometry/SVIPoly.h"
#include "./Geometry/SVIQuadPoly.h"
#include "./Geometry/SVIRoundPoly.h"
#include "./Geometry/SVIExplodePoly.h"
#include "./Geometry/SVITwistPoly.h"
#include "./Geometry/SVIBrickPoly.h"
#include "./Geometry/SVIPannelPoly.h"
#include "./Geometry/SVIDomePoly.h"
#include "./Geometry/SVIDrumPoly.h"
#include "./Geometry/SVIConeCurvePoly.h"
#include "./Geometry/SVIPathPoly.h"
#include "./Geometry/SVITesselationPoly.h"

#include "SVIRenderer.h"
#include "SVIFrameRenderer.h"
#include "SVITexture.h"
#include "SVITextureManager.h"
#include "SVIProgramManager.h"

#include <cstdlib>
#include <cmath>
#include <ctime>

namespace SVI {

//2011-06-15 masterkeaton27@gmail.com
//perspective test
const float cfTestDepth = 1.0f;

static const SVIBool DEBUG = SVIFALSE;

/****************************************************************************/
// Render Patch object
/****************************************************************************/

SVIRenderPatch::SVIRenderPatch(SVIGLSurface *surface) :
		mMemoryUsed(0), mPoly(NULL), mType(SVI_PATCH_COLOR), mGeometryType(
				SVI_PATCH_ROUND), mOutfit(NULL), mOwnerSlide(NULL) {
	mTextureContainer = NULL;
	mTextureScale.x = mTextureScale.y = 1.0f;
	//2012-02-21 marx.kim
	mPageStatus = -1;
	mProgramId = -1;
	mUseRenderTarget = SVIFALSE;

	filterTexture0 = 0;

	if(surface == NULL) {
		LOGE("Invalid argument to the SVIRenderPatch::SVIRenderPatch(SVIGLSurface *surface) \n");
		exit(0); // To check for errors till stabilization.. To be removed later. 
	} else {
		mSVIGLSurface = surface;
	}
}

SVIRenderPatch::~SVIRenderPatch() {
	finalize();
	mSVIGLSurface = NULL;
}

void SVIRenderPatch::renderShadow() {
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
    //LOGI("LS: SVIRenderPatch::renderShadow() is called");

	SVIBool bUseTexture =
			(mTextureContainer != NULL && !mTextureContainer->isEmpty()) ?
					SVITRUE : SVIFALSE;
	SVIBool bUseAddBlendType =
			(mOutfit->mBlendType == BLEND_ADD) ? SVITRUE : SVIFALSE;

	SVIProgramParams renderParams;
	SVIProgramHandler * pHandler = NULL;
	SVIProgram * pProgram = NULL;

	
	if (mPoly->hasNormals()){
		renderParams.addElement(EM_SHADOW3D);
	}else renderParams.addElement(EM_SHADOW);

	renderParams.addElement(bUseAddBlendType ? EM_BLEND_ADD : EM_BLEND_MLT);

	if (bUseTexture) {
		renderParams.addElement(
			mPoly->isScaleToFit() ? EM_TEXTURE_SCALE : EM_TEXTURE);
	} else {
		renderParams.addElement(EM_COLOR);
	}

	pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
	if (pProgram == NULL || !pProgram->getActivated())
		return;
	pHandler = pProgram->getHandler();


	if (!pProgram->getUseShadow())
		return;

	pProgram->use();


	float shadowRadius = mOutfit->mShadowRadius
			/ (mOutfit->mSize.x * mOutfit->mScale.x);
	float deltax = -(mOutfit->mSize.x * mOutfit->mScale.x * shadowRadius)
			* (0.5f - mOutfit->mPivot.x);
	float deltay = -(mOutfit->mSize.y * mOutfit->mScale.y * shadowRadius)
			* (0.5f - mOutfit->mPivot.y);

	glEnable(GL_BLEND);
	//2011-10-28 masterkeaton27@gmail.com
	//change default blend function to one
	//this is very important issues for some reasons.
	//if some images displayed strange color, should check this function.
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	if (bUseTexture) {
		pHandler->setSampler(HD_U_SVIMPLER_0, mTextureContainer->getTextureID());
		SVIBool hasAlpha =
				mTextureContainer->getTexture() ?
						mTextureContainer->getTexture()->getHasAlpha() : SVIFALSE;
		SVIInt alphaType =
				mTextureContainer->getTexture() ?
						mTextureContainer->getTexture()->getAlphaType() :
						SVITexture::NORMAL;
		if (mOutfit->mShadowColor.w < 1.0f || hasAlpha
				|| (bUseAddBlendType && mOutfit->mGlobalOpacity < 1.0f)) {
			if (alphaType == SVITexture::PREMULTIPLIED) {
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
	}

	processTextureScale(pHandler);

	SVIVector2 shadowRadiusSize = SVIVector2(shadowRadius + 1.0f,
			shadowRadius + 1.0f);
	SVIVector2 shadowOffset = SVIVector2(mOutfit->mShadowOffset.x + deltax,
			mOutfit->mShadowOffset.y + deltay);
	SVIVector4 shadowColor = mOutfit->mShadowColor;

	if (bUseAddBlendType) {
		pHandler->setUniform(HD_U_OPACITY, mOutfit->mGlobalOpacity);
	}

	pHandler->setUniform(HD_U_SHADOW_RADIUS, shadowRadiusSize);
	pHandler->setUniform(HD_U_SHADOW_OFFSET, shadowOffset);
	pHandler->setUniform(HD_U_SHADOW_COLOR, shadowColor);


	//2012-03-13 masterkeaton27@gmail.com
	if (mPoly->hasTexcoords()) {
		//		__android_log_print(ANDROID_LOG_INFO, "TEXCOORDS", "CUSTOM %d",
		//				mPoly->getGeometryType());
		pHandler->setAttribute(HD_A_TEXCOORD, mPoly->getTextureCoordinates(), 0);
	} else {
		//		__android_log_print(ANDROID_LOG_INFO, "TEXCOORDS", "STANDART %d",
		//				mPoly->getGeometryType());
		SVIVector2 sideSize = SVIVector2(mOutfit->mSize.x, mOutfit->mSize.y);
		SVIVector2 sidePivot = SVIVector2(mOutfit->mPivot.x, mOutfit->mPivot.y);
		pHandler->setUniform(HD_U_SIZE, sideSize);
		pHandler->setUniform(HD_U_PIVOT, sidePivot);

		//LOGE("Texture Size Pivot %.02f,%.02f,%.02f,%.02f",sideSize.x,sideSize.y,sidePivot.x,sidePivot.y);
	}
	


	SVIMatrix modelMat = mPoly->getTransform();
	SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();
	if (mOutfit->mIsOrthogonal) {
		projectionMat._34 = 0.0f;
	}
	modelMat._43 -= cfTestDepth;

	pHandler->setUniform(HD_U_PROJ, projectionMat);
	pHandler->setUniform(HD_U_VIEW, modelMat);

	pHandler->releaseAttribute(HD_A_UNITCOORD);

	SVIUInt primitiveType = GL_TRIANGLE_STRIP;
	SVIUInt PolyPrimitiveType = mPoly->getPrimitiveType();
	if (PolyPrimitiveType == SVIPoly::SVI_POLY_TRI_STRIP) {
		primitiveType = GL_TRIANGLE_STRIP;
	} else if (PolyPrimitiveType == SVIPoly::SVI_POLY_TRI_LIST) {
		primitiveType = GL_TRIANGLES;
	} else if (PolyPrimitiveType == SVIPoly::SVI_POLY_TRI_FAN) {
		primitiveType = GL_TRIANGLE_FAN;
	}

	if (mPoly->getGeometryType() == SVIRenderPatch::SVI_PATCH_ROUND) {
		SVIRoundPoly * pRound = (SVIRoundPoly *) mPoly;
		pHandler->setAttribute(
				HD_A_POSITION,
				!mPoly->hasBorder() ?
						pRound->getVertices() : pRound->getShadowVertices());

		//2011.12.08 jongchae.moon
		glDrawElements(primitiveType, pRound->getIndicesCount(), GL_UNSIGNED_SHORT, pRound->getIndices());
        //glDrawElements(primitiveType, pRound->getOriginalIndiceCount(),
        //    GL_UNSIGNED_SHORT, pRound->getIndices());
		//

	} else if (mPoly->getGeometryType() == SVIRenderPatch::SVI_PATCH_QUAD) {
		SVIQuadPoly * pQuad = (SVIQuadPoly *) mPoly;
		pHandler->setAttribute(
				HD_A_POSITION,
				!mPoly->hasBorder() ?
						pQuad->getVertices() : pQuad->getShadowVertices());

		//2011.12.08 jongchae.moon
		//glDrawArrays(primitiveType, 0, pQuad->getVerticeCount());
		primitiveType = GL_TRIANGLE_FAN;
		glDrawArrays(primitiveType, 0, pQuad->getOriginalVerticeCount());
		//
	}else if (mPoly->getGeometryType() == SVIRenderPatch::SVI_PATCH_DOME) {
        SVIUInt uStride = 0;
        pHandler->setAttribute(HD_A_POSITION, mPoly->getVertices(), uStride);
        if (mPoly->hasIndices()) {
            glDrawElements(primitiveType, mPoly->getIndicesCount(),
                GL_UNSIGNED_SHORT, mPoly->getIndices());
        } else {
            glDrawArrays(primitiveType, 0, mPoly->getVerticeCount());
        }
    }else if (mPoly->getGeometryType() == SVIRenderPatch::SVI_PATCH_CONECURVE) {
        SVIUInt uStride = 0;
        pHandler->setAttribute(HD_A_POSITION, mPoly->getVertices(), uStride);
        if (mPoly->hasIndices()) {
            glDrawElements(primitiveType, mPoly->getIndicesCount(),
                GL_UNSIGNED_SHORT, mPoly->getIndices());
        } else {
            glDrawArrays(primitiveType, 0, mPoly->getVerticeCount());
        }

    }else if (mPoly->getGeometryType() == SVIRenderPatch::SVI_PATCH_PATH) {
        glBlendFunc(GL_ONE, GL_ONE);
        SVIUInt uStride = 0;
        SVIPathPoly * pPath = (SVIPathPoly *) mPoly;
        pHandler->setAttribute(HD_A_POSITION, pPath->getShadowVertices(), uStride);
        if (mPoly->hasIndices()) {
            glDrawElements(primitiveType, mPoly->getIndicesCount(),
                GL_UNSIGNED_SHORT, mPoly->getIndices());
        } else {
            glDrawArrays(primitiveType, 0, mPoly->getVerticeCount());
        }
    }

#if SVI_USING_CPU_TEXCOORD
	pHandler->releaseAttribute(HD_A_TEXCOORD);
#endif

	if (mPoly->hasTexcoords()) {
		pHandler->releaseAttribute(HD_A_TEXCOORD);
	}

	pHandler->releaseAttribute(HD_A_POSITION);

	glDisable(GL_BLEND);
}

SVIBool SVIRenderPatch::isRenderTarget() {
	return (mTextureContainer != NULL && mTextureContainer->hasCaptureData()) ?
SVITRUE : SVIFALSE;
}

SVIBool SVIRenderPatch::hasNoChild() {
	if (mOwnerSlide == NULL)
		return SVITRUE;
	return (mOwnerSlide->getSubCount() == 0) ? SVITRUE : SVIFALSE;
}

SVIBool SVIRenderPatch::isLastChild() {
	if (mOwnerSlide == NULL)
		return SVIFALSE;
	SVIProjectionSlide * pChild = mOwnerSlide;
	SVIProjectionSlide * pParent =
			(SVIProjectionSlide *) mOwnerSlide->getParent();
	if (pParent == NULL)
		return SVIFALSE;

	//check owners state
	SVIInt childCount = pParent->getSubCount();
	SVIProjectionSlide * pParentLastChild =
			(SVIProjectionSlide *) pParent->getSub(childCount - 1);
	if (pParentLastChild != pChild)
		return SVIFALSE;

	//go one depth up.
	pChild = pParent;
	pParent = (SVIProjectionSlide *) pParent->getParent();
	if (pParent == NULL)
		return SVIFALSE;

	while (pParent != NULL && !pChild->isClip()) {
		SVIInt childCount = pParent->getSubCount();
		pParentLastChild = (SVIProjectionSlide *) pParent->getSub(
				childCount - 1);
		if (pParentLastChild != pChild)
			return SVIFALSE;

		//go next depth
		pChild = pParent;
		pParent = (SVIProjectionSlide *) pParent->getParent();
	}
	return SVITRUE;
}

SVIBool SVIRenderPatch::hasClipParent() {
	if (mOwnerSlide == NULL)
		return SVIFALSE;
	SVIProjectionSlide * pParent =
			(SVIProjectionSlide *) mOwnerSlide->getParent();
	if (pParent == NULL)
		return SVIFALSE;
	while (pParent != NULL) {
		if (pParent->isClip())
			return SVITRUE;
		pParent = (SVIProjectionSlide *) pParent->getParent();
	}
	return SVIFALSE;
}

void SVIRenderPatch::preProcessRenderTargetEffect(SVIProgramParams * pParams) {
	if (mOutfit == NULL || pParams == NULL)
		return;

	if (!isRenderTarget())
		return;
	//pParams->addElement(EM_RENDER_TARGET);

#if SVI_USE_POST_EFFECT		
	if (mOutfit->mRTEffectType == RT_DIRECTIONBLUR) {
		pParams->addElement(EM_RT_DIRECTIONBLUR);
	} else if (mOutfit->mRTEffectType == RT_ZOOMBLUR) {
		pParams->addElement(EM_RT_ZOOMBLUR);
	} else if (mOutfit->mRTEffectType == RT_GLASSTILE) {
		pParams->addElement(EM_RT_GLASSTILE);
	} else if (mOutfit->mRTEffectType == RT_MAGNIFY) {
		pParams->addElement(EM_RT_MAGNIFY);
	} else if (mOutfit->mRTEffectType == RT_PAPERFOLD) {
		pParams->addElement(EM_RT_PAPERFOLD);
	} else if (mOutfit->mRTEffectType == RT_RIPPLE) {
		pParams->addElement(EM_RT_RIPPLE);
	} else if (mOutfit->mRTEffectType == RT_PULSE) {
		pParams->addElement(EM_RT_PULSE);
	} else if (mOutfit->mRTEffectType == RT_DREAMVISION) {
		pParams->addElement(EM_RT_DREAMVISION);
	} else if (mOutfit->mRTEffectType == RT_WIZZLE) {
		pParams->addElement(EM_RT_WIZZLE);
	} else
		pParams->addElement(
				mPoly->isScaleToFit() ? EM_TEXTURE_SCALE : EM_TEXTURE);
#else
	pParams->addElement(mPoly->isScaleToFit() ? EM_TEXTURE_SCALE : EM_TEXTURE);
#endif
}

void SVIRenderPatch::preProcessLighting(SVIProgramParams * pParams) {
	SVIBool bUseLinearLight = mOutfit->hasLinearLight();
	SVIBool bUseSpotLight = mOutfit->hasSpotLight();
	if (bUseLinearLight) {
		pParams->addElement(EM_LIGHT_LINEAR);
	} else if (bUseSpotLight) {
		pParams->addElement(EM_LIGHT_SPOT);
	}
}

void SVIRenderPatch::preProcessBlending(SVIProgramParams * pParams) {
	SVIBool bUseAddBlendType =
		(mOutfit->mBlendType == BLEND_ADD) ? SVITRUE : SVIFALSE;
	pParams->addElement(bUseAddBlendType ? EM_BLEND_ADD : EM_BLEND_MLT);
}

void SVIRenderPatch::preProcessShaderEffect(SVIProgramParams * pParams) {
	if (mOutfit->mSwipeType == SWIPE_LINEAR)
		pParams->addElement(EM_LINEAR_SWIPE);
	else if (mOutfit->mSwipeType == SWIPE_RADIAL)
		pParams->addElement(EM_RADIAL_SWIPE);
}


void SVIRenderPatch::preProcessFilter(SVIProgramParams * pParams){
    if (mOutfit->mFilterType == FILTER_NONE){
		//jdev.nam remove log 2013-07-29
        //DEBUG_CHECK_LOGE("preProcessFilter Filter FILTER_NONE");
    }else if (mOutfit->mFilterType == FILTER_SKETCH){
        pParams->addElement(EM_FILTER_SKETCH);
        DEBUG_CHECK_LOGE("preProcessFilter Filter EM_FILTER_SKETCH");
    }else if (mOutfit->mFilterType == FILTER_OIL_PAINT){
        pParams->addElement(EM_FILTER_OILPAINT);
        DEBUG_CHECK_LOGE("preProcessFilter Filter EM_FILTER_OIL");
    }else if (mOutfit->mFilterType == FILTER_WATERCOLOR){
        pParams->addElement(EM_FILTER_WATERCOLOR);
        DEBUG_CHECK_LOGE("preProcessFilter Filter EM_FILTER_WATERCOLOR");
    }else if (mOutfit->mFilterType == FILTER_CARTOON){
        pParams->addElement(EM_FILTER_CARTOON);
        DEBUG_CHECK_LOGE("preProcessFilter Filter EM_FILTER_CARTOON");
    }else if (mOutfit->mFilterType == FILTER_RADIAL){
        pParams->addElement(EM_FILTER_RADIAL);
        DEBUG_CHECK_LOGE("preProcessFilter Filter EM_FILTER_RADIAL");
    }else if (mOutfit->mFilterType == FILTER_MONOTONE){
        pParams->addElement(EM_FILTER_MONOTONE);
        DEBUG_CHECK_LOGE("preProcessFilter Filter EM_FILTER_MONOTONE");
    }else if (mOutfit->mFilterType == FILTER_SMOKE){
        pParams->addElement(EM_FILTER_SMOKE);
        DEBUG_CHECK_LOGE("preProcessFilter Filter EM_FILTER_SMOKE");
    }
}

void SVIRenderPatch::preProcessDeformation(SVIProgramParams * pParams) {
	if (mOutfit->mDeformType == DEFORM_TWIST
			|| mOutfit->mDeformType == DEFORM_EXPLODE
            || mOutfit->mDeformType == DEFORM_DRUM //2012-11-19 masterkeaton27@gmail.com
			|| mOutfit->mDeformType == DEFORM_PANNEL
			|| mOutfit->mDeformType == DEFORM_BRICK
            || mOutfit->mDeformType == DEFORM_CONECURVE){
		pParams->addElement(EM_3DMODEL);
    }

    //2013-06-24 masterkeaton27@gmail.com
    if (mOutfit->mDeformType == DEFORM_PATH){
        pParams->addElement(EM_CPU_TEXTURE);
    }

    //2012-11-21 masterkeaton27@gmail.com
    if (mOutfit->mDeformType == DEFORM_DOME){
        pParams->addElement(EM_3DMODEL_OFFSET);
    }



	//2012-04-09 masterkeaton27@gmail.com
	if (mOutfit->mEnableAA){
		pParams->addElement(EM_PAGE3D_AA);
	}
}

void SVIRenderPatch::preProcessTesselation(SVIProgramParams *pParams)
{
    if(mOutfit->mTesselation)
    {
        pParams->addElement(EM_TESSELATION);
    }
}

void SVIRenderPatch::preProcessTexture(SVIProgramParams * pParams) {
	//select elements
	SVIBool bUseTexture =
			(mTextureContainer != NULL && !mTextureContainer->isEmpty()) ?
					SVITRUE : SVIFALSE;
	SVIBool bUseNormalTexture = 
		(mTextureContainer && mOutfit->isUseNormalMap() && mTextureContainer->getNormalTexture()) ? SVITRUE : SVIFALSE;

    SVIBool bUseSpecularTexture = 
        (mTextureContainer && mOutfit->isUseNormalMap() && mTextureContainer->getSpecularTexture()) ? SVITRUE : SVIFALSE;
        
	//switch color patch when patch linked with invalid texture object
	if (bUseTexture && mTextureContainer->getTexture()
			&& !mTextureContainer->getTexture()->getValid()) {
		bUseTexture = SVIFALSE;
		setType(SVIRenderPatch::SVI_PATCH_COLOR);
		pParams->addElement(EM_COLOR);
	} else if (bUseTexture) {
		pParams->addElement(
				mPoly->isScaleToFit() ? EM_TEXTURE_SCALE : EM_TEXTURE);
	} else {
		setType(SVIRenderPatch::SVI_PATCH_COLOR);
		pParams->addElement(EM_COLOR);
	}
    
    //2012-12-10 masterkeaton27@gmail.com 
    if (bUseSpecularTexture){
        pParams->addElement(EM_NORMALSPECULARMAP);
    }else if (bUseNormalTexture){
		pParams->addElement(EM_NORMALMAP);
	}
}

void SVIRenderPatch::processFrontFaceRender(SVIProgramHandler * pHandler) {
	mPoly->prepareFrontFace();
	proccessGeometry(pHandler);
}

void SVIRenderPatch::processBackFaceRender(SVIProgramHandler * pHandler) {
	if (!mPoly->hasBackFace()) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		return;
	}
	if (!mPoly->hasNormals()) {
		return;
	}

    //2012-11-26 masterkeaton27@gmail.com
    if (mOutfit->isUseNormalMap()){
		LOGE("back face render skipped");
        return;
    }

	SVIVector3 lightPosition = SVIVector3(0.25f, 0.25f, -10.0f);
	SVIVector4 lightAmbient = SVIVector4(0.85f, 0.85f, 0.85f, 1.0f);
	SVIVector4 lightDiffuse = SVIVector4(0.59f, 0.59f, 0.59f, 1.0f);
	SVIVector4 lightSpecular = SVIVector4(0.3f, 0.3f, 0.3f, 1.0f);

	SVIMatrix normalMatrix = mPoly->getTransform();
	normalMatrix.invert();
	normalMatrix.transpose();

	SVIFloat lightShininess = !mOutfit->mIsBackFaceImageFlip ? 0.4f : 0.75f;
	
	pHandler->setUniform(HD_U_NORMAL, normalMatrix);
	pHandler->setUniform(HD_U_LIGHT_OFFSET, lightPosition);

	pHandler->setUniform(HD_U_AMBIENT, lightAmbient);
	pHandler->setUniform(HD_U_DIFFUSE, lightDiffuse);
	pHandler->setUniform(HD_U_SPECULAR, lightSpecular);
	pHandler->setUniform(HD_U_SHININESS, lightShininess);

	SVIVector2 texRegionSize = mPoly->getBackFaceTextureRegionSize();
	SVIVector2 texRegionOffset = mPoly->getBackFaceTextureRegionOffset();
	SVIVector4 texInfo = SVIVector4(texRegionOffset.x, texRegionOffset.y,
			texRegionSize.x, texRegionSize.y);
	pHandler->setUniform(HD_U_TEXTUREINFO, texInfo);

	//2012-01-09 masterkeaton27@gmail.com
	//capture texture should has higher priority then normal texture.
	//but, this may cause unexpected problem. if back faced texture appears strange way then should check this routine first.
	if (mTextureContainer != NULL
			&& mTextureContainer->getBackFaceCaptureTexture() != NULL) {
		pHandler->setSampler(HD_U_SVIMPLER_0,
				mTextureContainer->getBackFaceCaptureID());
	} else if (mTextureContainer != NULL
			&& mTextureContainer->getBackFaceTexture() != NULL) {
		pHandler->setSampler(HD_U_SVIMPLER_0,
				mTextureContainer->getBackFaceTextureID());
	}

	mPoly->prepareBackFace();

	proccessGeometry(pHandler);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void SVIRenderPatch::renderMorphing()
{
    if(!mOutfit->mTesselation)
    {
        return;
    }

    glDisable(GL_CULL_FACE);

    SVIProgramParams renderParams;
    SVIProgramHandler * pHandler = NULL;
    SVIProgram * pProgram = NULL;

    renderParams.addElement(EM_COLOR);

    pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
    if (pProgram == NULL || !pProgram->getActivated())
        return;
    pHandler = pProgram->getHandler();


    pProgram->use();

    SVITesselationPoly *pPoly = (SVITesselationPoly *)mPoly;
    SVIUInt uStride = 0;

    //change color of morphing geometry.

    pHandler->setUniform(HD_U_COLOR, mOutfit->mMorphingColor);

    pHandler->setAttribute(HD_A_POSITION, pPoly->getMorphingVertices(), uStride);
    pHandler->releaseAttribute(HD_A_UNITCOORD);

    SVIVector2 sideSize = SVIVector2(mOutfit->mSize.x, mOutfit->mSize.y);
    SVIVector2 sidePivot = SVIVector2(mOutfit->mPivot.x, mOutfit->mPivot.y);
    pHandler->setUniform(HD_U_SIZE, sideSize);
    pHandler->setUniform(HD_U_PIVOT, sidePivot);

    //2011-11-08 jbkim DMC Graphics Lab.
    if (mPoly->getAAcoords() != NULL) {
        pHandler->setAttribute(HD_A_AACOORD, 1, mPoly->getAAcoords());
    }

    SVIUInt primitiveType = GL_TRIANGLE_STRIP;
    SVIUInt polyPrimitiveType = mPoly->getPrimitiveType();
    if (polyPrimitiveType == SVIPoly::SVI_POLY_TRI_STRIP) {
        primitiveType = GL_TRIANGLE_STRIP;
    } else if (polyPrimitiveType == SVIPoly::SVI_POLY_TRI_LIST) {
        primitiveType = GL_TRIANGLES;
    } else if (polyPrimitiveType == SVIPoly::SVI_POLY_TRI_FAN) {
        primitiveType = GL_TRIANGLE_FAN;
    }

    //set model view projection matrix
    SVIMatrix modelMat = mPoly->getTransform();

    //2011-07-12 masterkeaton27@gmail.com
    SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();


    if (mOutfit->mIsOrthogonal){
        projectionMat._34 = 0.0f;
    }

    modelMat._43 -= cfTestDepth;

    pHandler->setUniform(HD_U_PROJ, projectionMat);
    pHandler->setUniform(HD_U_VIEW, modelMat);

    if (mPoly->hasIndices()) {
        //2011-12-08 jongchae.moon DMC Graphics Lab.DMC Graphics Lab.
        if (mPoly->getIsAntiAliasing()) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glDrawElements(primitiveType, pPoly->getMorphingIndicesCount(),
                GL_UNSIGNED_SHORT, pPoly->getMorphingIndices());
            glDisable(GL_BLEND);
        } else {
            glDrawElements(primitiveType, pPoly->getMorphingIndicesCount(),
                GL_UNSIGNED_SHORT, pPoly->getMorphingIndices());
        }
    } else {
        glDrawArrays(primitiveType, 0, pPoly->getMorphingVerticesCount());
    }
    //

    pHandler->releaseAttribute(HD_A_POSITION);

    // 2011-11-08 jbkim DMC Graphics Lab.
    if (mPoly->getAAcoords() != NULL) {
        pHandler->releaseAttribute(HD_A_AACOORD);
    }
}


void SVIRenderPatch::processShaderEffect(SVIProgramHandler * pHandler) {
	if (mOutfit->mSwipeType == SWIPE_LINEAR){
		pHandler->setUniform(HD_U_EDGE_RATIO, mOutfit->mSwipeRatio.x);		
		pHandler->setUniform(HD_U_LERP_WIDTH, mOutfit->mSwipeRatio.y);
		pHandler->setUniform(HD_U_LERP_BASE, mOutfit->mSwipeRatio.z);
	}else if (mOutfit->mSwipeType == SWIPE_RADIAL){
		pHandler->setUniform(HD_U_EDGE_RADIUS, mOutfit->mSwipeRatio.x);
		pHandler->setUniform(HD_U_LERP_WIDTH, mOutfit->mSwipeRatio.y);
		pHandler->setUniform(HD_U_LERP_BASE, mOutfit->mSwipeRatio.z);
	}	
}


void SVIRenderPatch::processFilter(SVIProgramHandler * pHandler) {

    if (mTextureContainer == NULL) return;

    SVITexture * pTexture = mTextureContainer->getTexture();
    if (pTexture == NULL) return;

    if (mOutfit->mFilterType == FILTER_SKETCH){        
        SVIVector3 factors((float)pTexture->getWidth() * mOutfit->mDeformRatio.x,(float)pTexture->getHeight() * mOutfit->mDeformRatio.y, mOutfit->mDeformRatio.z);
        pHandler->setUniform(HD_U_RESERVE01, factors);		
        DEBUG_CHECK_LOGE("Filer Factor %.02f, %.02f intensity %.02f", factors.x, factors.y, mOutfit->mDeformRatio.z);

    }else if (mOutfit->mFilterType == FILTER_OIL_PAINT){
        SVIVector3 factors((float)pTexture->getWidth(),(float)pTexture->getHeight(), mOutfit->mDeformRatio.z);
        pHandler->setUniform(HD_U_RESERVE01, factors);		
        DEBUG_CHECK_LOGE("Filer Factor %.02f, %.02f intensity %.02f", factors.x, factors.y, mOutfit->mDeformRatio.z);
    }else if (mOutfit->mFilterType == FILTER_WATERCOLOR){
        SVIVector3 factors(1.0f / (float)pTexture->getWidth(),1.0f / (float)pTexture->getHeight(), mOutfit->mDeformRatio.z);
        pHandler->setUniform(HD_U_RESERVE01, factors);		
        DEBUG_CHECK_LOGE("Filer Factor %.02f, %.02f intensity %.02f", factors.x, factors.y, mOutfit->mDeformRatio.z);
    }else if (mOutfit->mFilterType == FILTER_CARTOON){

    }else if (mOutfit->mFilterType == FILTER_RADIAL){
        SVIBool wider = (float)pTexture->getWidth() > (float)pTexture->getHeight();
        float ratiox = wider ? (float)pTexture->getWidth() / (float)pTexture->getHeight() : 1.0f;
        float ratioy = !wider ? (float)pTexture->getHeight() / (float)pTexture->getWidth() : 1.0f;

        DEBUG_CHECK_LOGE("tex %.02f, %.02f", (float)pTexture->getWidth(), (float)pTexture->getHeight());
        DEBUG_CHECK_LOGE("ratio %.02f, %.02f", ratiox, ratioy);

        float x = mOutfit->mDeformRatio.x;// * ratiox;  
        float y = mOutfit->mDeformRatio.y;// * ratioy;

        SVIVector3 factors(mOutfit->mSwipeRatio.x, mOutfit->mSwipeRatio.y, mOutfit->mSwipeRatio.z);
        pHandler->setUniform(HD_U_RESERVE01, factors);	

        SVIVector4 factors2(ratiox, ratioy, x * ratiox, y * ratioy);
        pHandler->setUniform(HD_U_RESERVE02, factors2);	
    }else if (mOutfit->mFilterType == FILTER_MONOTONE){
        SVIBool wider = (float)pTexture->getWidth() > (float)pTexture->getHeight();
        float ratiox = wider ? (float)pTexture->getWidth() / (float)pTexture->getHeight() : 1.0f;
        float ratioy = !wider ? (float)pTexture->getHeight() / (float)pTexture->getWidth() : 1.0f;

        DEBUG_CHECK_LOGE("tex %.02f, %.02f", (float)pTexture->getWidth(), (float)pTexture->getHeight());
        DEBUG_CHECK_LOGE("ratio %.02f, %.02f", ratiox, ratioy);

        float x = mOutfit->mDeformRatio.x;// * ratiox;  
        float y = mOutfit->mDeformRatio.y;// * ratioy;

        SVIVector3 factors(mOutfit->mSwipeRatio.x, mOutfit->mSwipeRatio.y, mOutfit->mSwipeRatio.z);
        pHandler->setUniform(HD_U_RESERVE01, factors);	

        SVIVector4 factors2(ratiox, ratioy, x * ratiox, y * ratioy);
        pHandler->setUniform(HD_U_RESERVE02, factors2);	
    }else if (mOutfit->mFilterType == FILTER_SMOKE){        
        SVIVector3 factors(mOutfit->mDeformRatio.x,  mOutfit->mDeformRatio.y, mOutfit->mDeformRatio.z);
        pHandler->setUniform(HD_U_RESERVE01, factors);		
        DEBUG_CHECK_LOGE("Filer Factor %.02f, %.02f intensity %.02f", factors.x, pow(factors.x, 10.0f), factors.z);
    }
}

void SVIRenderPatch::processDeformation(SVIProgramHandler * pHandler) {

	if (!mPoly->hasNormals()) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		return;
	}

	SVIVector3 lightPosition = SVIVector3(0.25f, 0.25f, 10.0f);
	SVIVector4 lightAmbient = SVIVector4(0.6f, 0.6f, 0.6f, 1.0f);
	SVIVector4 lightDiffuse = SVIVector4(0.69f, 0.69f, 0.69f, 1.0f);
	SVIVector4 lightSpecular = SVIVector4(0.4f, 0.4f, 0.4f, 1.0f);

	float lightShininess = 0.65f;
	pHandler->setUniform(HD_U_LIGHT_OFFSET, lightPosition);

	pHandler->setUniform(HD_U_AMBIENT, lightAmbient);
	pHandler->setUniform(HD_U_DIFFUSE, lightDiffuse);
	pHandler->setUniform(HD_U_SPECULAR, lightSpecular);
	pHandler->setUniform(HD_U_SHININESS, lightShininess);

	glEnable(GL_DEPTH_TEST);

	if (mOutfit->isOrthogonal()) {
		glClearDepthf(1.0);
		glDepthFunc(GL_LESS);
	} else {
		glClearDepthf(0.0);
		glDepthFunc(GL_GREATER);
	}

	if (mPoly->hasBackFace()) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

}

void SVIRenderPatch::processTextureScale(SVIProgramHandler * pHandler) {
	SVIVector2 texRegionSize = mPoly->getTextureRegionSize();
	SVIVector2 texRegionOffset = mPoly->getTextureRegionOffset();
	SVIVector4 texInfo = SVIVector4(texRegionOffset.x, texRegionOffset.y,
			texRegionSize.x, texRegionSize.y);
	pHandler->setUniform(HD_U_TEXTUREINFO, texInfo);

	texRegionSize = mPoly->getTextureScaleFitRegionSize();
	texRegionOffset = mPoly->getTextureScaleFitRegionOffset();
	texInfo = SVIVector4(texRegionOffset.x, texRegionOffset.y, texRegionSize.x,
			texRegionSize.y);
	pHandler->setUniform(HD_U_FITINFO, texInfo);
}

void pNoise(unsigned char* pixels,  int WIDTH, int HEIGHT, int PERIOD, int OCTAVE, float fraction) {
	int octaveFactor = (int)pow(2.0f, (float)(OCTAVE-1));
	
	int WIDTH_INTERVAL = (WIDTH>HEIGHT? WIDTH: HEIGHT) * octaveFactor;
	int HEIGHT_INTERVAL = (WIDTH<HEIGHT? WIDTH: HEIGHT) * octaveFactor;

	int INTERVAL = (WIDTH>HEIGHT)? WIDTH_INTERVAL: HEIGHT_INTERVAL;
	
	int redPermutations[INTERVAL];
	float directions[INTERVAL*2];

	for(int i=0; i<INTERVAL; i++){
		redPermutations[i] = (int)(rand()%INTERVAL); 

		directions[i*2] = (float)sin(i*2*3.1415f/INTERVAL);
		directions[i*2+1] = (float)cos(i*2*3.1415f/INTERVAL);
	}
	
	int i,j,m,n;
	float x,y,redSum;
	int gridX, gridY;
	float distX, distY, polyX, polyY;
	int redHashed;
	float redGrad;
	int redVal;
	float pow3, pow4, pow5;
 	
	int actualWidth = PERIOD*WIDTH;
	int actualHeight = PERIOD*HEIGHT;
	float multiplier = octaveFactor/(float)PERIOD;
	for(j=0; j<actualHeight; j++){
		for(i=0; i<actualWidth; i++){
			x = (float)i*multiplier; 
			y = (float)j*multiplier; 
	 		 
			redSum=0;
			
			for(m=0; m<2; m++){
				for(n=0; n<2; n++){
					gridX = (int)x + m;
					gridY = (int)y + n;
					
				        distX = (x > gridX)? (x-gridX): (gridX-x);
				        distY = (y > gridY)? (y-gridY): (gridY-y);
				        
				        pow3 = distX*distX*distX;
				        pow4 = pow3*distX;
				        pow5 = pow4*distX;
				        polyX = (float)(1 - 6*pow5 + 15*pow4 - 10*pow3);
				        
				        pow3 = distY*distY*distY;
				        pow4 = pow3*distY;
				        pow5 = pow4*distY;
				        polyY = (float)(1 - 6*pow5 + 15*pow4 - 10*pow3);
				        
				        redHashed = redPermutations[(redPermutations[gridX%WIDTH_INTERVAL] + gridY)%HEIGHT_INTERVAL];
				        redGrad = (x-gridX)*directions[redHashed*2] + (y-gridY)*directions[redHashed*2 + 1];				        
				        redSum += polyX * polyY * redGrad;
				}
			}	
			
			redVal = (pixels[j*actualWidth + i]&255) + (int)(redSum*256*fraction) + 16;
			if(redVal < 0){
				redVal = 0;
			}else if(redVal > 255){
				redVal = 255;
			}

			pixels[j*actualWidth + i] = redVal;
		}
	}
}

unsigned char* createSmokeTexture(int *widthArg, int *heightArg){
	//clock_t t0 = clock();
	
	int width = *widthArg;
	int height = *heightArg;
	
	width = 1 << (int)(log((float)width)/log(2.0f));
	height = 1 << (int)(log((float)height)/log(2.0f));
		
	int PERIOD = (width<height)? width/4: height/4;	
	
	int factoredWidth = width/PERIOD;
	int factoredHeight = height/PERIOD;

	float fraction = 2;
	unsigned char* pixels = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	memset(pixels, 0, sizeof(unsigned char)*width*height);
	for(int i=1; i<=5; i++){
		pNoise(pixels, factoredWidth, factoredHeight, PERIOD, i, fraction/=2); 
	}

	*widthArg = width;
	*heightArg = height;

	//clock_t t1 = clock();
	//LOGE("~~~~~~~~~~~~~sk time for (%d x %x) perlin generation: %d %d %d", width, height, (t1-t0));

	return pixels;
}

void SVIRenderPatch::processTextureBlendOperation(SVIProgramHandler * pHandler) {
	SVIVector4 colorValue = mPoly->getColor();

	SVIBool bUseTexture =
			(mTextureContainer != NULL && !mTextureContainer->isEmpty()) ?
					SVITRUE : SVIFALSE;
	SVIBool bUseAddBlendType =
			(mOutfit->mBlendType == BLEND_ADD) ? SVITRUE : SVIFALSE;

    //2013-02-01 masterkeaton27@gmail.com
    SVIBool bUseOneBlend = (mOutfit->mBlendType == BLEND_ONE) ? SVITRUE : SVIFALSE;

	SVIBool bUseBlending = SVIFALSE;

    //2012-11-26 masterkeaton27@gmail.com
    SVIBool bUseNormalMap = mOutfit->isUseNormalMap();

	SVIBool hasAlpha = (bUseTexture && mTextureContainer->getTexture()) ?
					mTextureContainer->getTexture()->getHasAlpha() : SVIFALSE;

	//2012-06-22 masterkeaton27@gmail.com
	//add check flag for shader effect.
	SVIBool hasShaderEffect = mOutfit->mSwipeType != SWIPE_NONE ? SVITRUE : SVIFALSE;

	if (bUseTexture) {
		if (colorValue.w < 1.0f || hasAlpha
				|| (bUseAddBlendType && mOutfit->mGlobalOpacity < 1.0f))
			bUseBlending = SVITRUE;
		
		pHandler->setSampler(HD_U_SVIMPLER_0, mTextureContainer->getTextureID());

		if (mTextureContainer->getTextureID() == 0){
			SVIVector4 filteredColor;
			//render target slide should have a white color.
			filteredColor.r = filteredColor.g = filteredColor.b = filteredColor.a = 0.0f;
			pHandler->setUniform(HD_U_COLOR, filteredColor);
		}

        //2012-11-26 masterkeaton27@gmail.com 
		//using multi-texture for normal map
		if (bUseNormalMap && mTextureContainer->getNormalTexture() != NULL){
			pHandler->setSampler(HD_U_SVIMPLER_1, mTextureContainer->getNormalID());
            //LOGE("Normal Map Setted <%d>", mTextureContainer->getNormalID());
        }

        //2012-12-10 masterkeaton27@gmail.com
        //using multi-texture for specular map
        if (bUseNormalMap && mTextureContainer->getSpecularTexture() != NULL){
            pHandler->setSampler(HD_U_SVIMPLER_2, mTextureContainer->getSpecularID());
            //LOGE("Specular Map Setted <%d>", mTextureContainer->getNormalID());
        }

		 if (mOutfit->getFilterType() ==FILTER_SMOKE){	
			pHandler->setSampler(HD_U_SVIMPLER_1, mTextureContainer->getBackFaceTextureID());

			if( !filterTexture0){
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			
				glGenTextures(1, &filterTexture0);
				if(!filterTexture0){
					LOGE("Couldn't create smoke texture !");
				}
				pHandler->setSampler(HD_U_SVIMPLER_2, filterTexture0);
			
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				int t;
				glGetIntegerv(GL_ACTIVE_TEXTURE, &t);

				int width = (mOutfit->mSize.x* mOutfit->mScale.x);
				int height = (mOutfit->mSize.y  * mOutfit->mScale.y);
				unsigned char* smokePixels = createSmokeTexture(&width, &height);
			
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, smokePixels);
				free(smokePixels);
			}else{
				pHandler->setSampler(HD_U_SVIMPLER_2, filterTexture0);
			}		
        }
	} else if (isUsingColor()) {
		if (colorValue.w < 1.0f)
			bUseBlending = SVITRUE;
	}

    if (bUseBlending || hasShaderEffect || bUseOneBlend) {
        glEnable(GL_BLEND);

        if (mPoly->hasNormals()
            || (mTextureContainer != NULL
            && mTextureContainer->hasCaptureData())) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }

        if (bUseOneBlend){
            glBlendFunc(GL_ONE, GL_ONE);
        }
    } else {
        glDisable(GL_BLEND);
	}
}

void SVIRenderPatch::processRenderTargetEffect(SVIProgramHandler * pHandler) {
	//2011-11-03 masterkeaton27@gmail.com
	//should clean up following mass. just a test code.

	if (!isRenderTarget())
		return;

#if SVI_USE_POST_EFFECT
	if (mOutfit == NULL || pHandler == NULL)
		return;

	SVIInt typeRTEffect = mOutfit->mRTEffectType;

	if (mOutfit->isCustomRTE()) {

		if (typeRTEffect == RT_NONE) {

		} else if (typeRTEffect == RT_RIPPLE) {

			pHandler->setUniform(HD_U_RESERVE01, mOutfit->mRTERatio.x,
					mOutfit->mRTERatio.y, mOutfit->mRTERatio.z);

			SVIVector2 vCenter(0.5f, 0.5f);
			pHandler->setUniform(HD_U_RESERVE02, vCenter);

		} else if (typeRTEffect == RT_PULSE) {

			pHandler->setUniform(HD_U_RESERVE01, mOutfit->mRTERatio.x,
					mOutfit->mRTERatio.y, mOutfit->mRTERatio.z);

			SVIVector2 vCenter =
					mSVIGLSurface->getCommonShaderVariables()->getNormalizedPosition();
			pHandler->setUniform(HD_U_RESERVE02, vCenter);

		} else if (typeRTEffect == RT_PAPERFOLD) {

			pHandler->setUniform(HD_U_TIME, mOutfit->mRTERatio.x);

		} else if (typeRTEffect == RT_MAGNIFY) {

			pHandler->setUniform(HD_U_RESERVE01, mOutfit->mRTERatio.x,
					mOutfit->mRTERatio.y, mOutfit->mRTERatio.z);

			SVIVector2 vCenter(0.5f, 0.5f);
			pHandler->setUniform(HD_U_RESERVE02, vCenter);

		} else if (typeRTEffect == RT_DREAMVISION) {

		} else if (typeRTEffect == RT_WIZZLE) {
			pHandler->setUniform(HD_U_TIME, mOutfit->mRTERatio.z);

			pHandler->setUniform(HD_U_RESERVE01, mOutfit->mRTERatio.x,
					mOutfit->mRTERatio.y);

		} else if (typeRTEffect == RT_GLASSTILE) {

			pHandler->setUniform(HD_U_RESERVE01, mOutfit->mRTERatio.x,
					mOutfit->mRTERatio.y, mOutfit->mRTERatio.z);

		} else if (typeRTEffect == RT_ZOOMBLUR) {

			pHandler->setUniform(HD_U_RESERVE01, mOutfit->mRTERatio.x,
					mOutfit->mRTERatio.y, mOutfit->mRTERatio.z);

		} else if (typeRTEffect == RT_DIRECTIONBLUR) {

			pHandler->setUniform(HD_U_RESERVE01, mOutfit->mRTERatio.x,
					mOutfit->mRTERatio.y);
		}

	} else {
		if (typeRTEffect == RT_NONE) {

		} else if (typeRTEffect == RT_RIPPLE) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			float fRepeat = abs(sin(fTime));

			SVIVector3 vInfo(100.0f * fRepeat, 2.0f * fRepeat, 0.1f); //x: freq, y: phase, z: amp
			pHandler->setUniform(HD_U_RESERVE01, vInfo.x, vInfo.y, vInfo.z);

			SVIVector2 vCenter(0.5f, 0.5f); // = SVICommonShaderVariables::getInstance()->getNormalizedPosition();
			pHandler->setUniform(HD_U_RESERVE02, vCenter);

		} else if (typeRTEffect == RT_PULSE) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			pHandler->setUniform(HD_U_TIME, fTime);

			SVIVector3 vInfo(5.0f, 3.0f, 2.0f);
			pHandler->setUniform(HD_U_RESERVE01, vInfo.x, vInfo.y, vInfo.z);

			SVIVector2 vCenter =
					mSVIGLSurface->getCommonShaderVariables()->getNormalizedPosition();
			pHandler->setUniform(HD_U_RESERVE02, vCenter);
		} else if (typeRTEffect == RT_PAPERFOLD) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			//this is test code.
			float fRepeat = abs(sin(fTime));
			pHandler->setUniform(HD_U_TIME, fRepeat);

		} else if (typeRTEffect == RT_MAGNIFY) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			//this is test code.
			float fSin = sin(fTime);
			float fRepeat = abs(sin(fTime)) + 1.0f;

			SVIVector2 vCenter(0.5f + fSin * 0.1f, 0.5f + fSin * 0.1f);
			pHandler->setUniform(HD_U_RESERVE02, vCenter);

			if (fRepeat < 1.0f)
				fRepeat = 1.0f;

			SVIVector3 vInfo(0.15f * fRepeat, 0.2f * fRepeat, fRepeat * 3.0f); //x: inner radius , y: outer radius , z: magnification
			pHandler->setUniform(HD_U_RESERVE01, vInfo.x, vInfo.y, vInfo.z);

		} else if (typeRTEffect == RT_DREAMVISION) {

		} else if (typeRTEffect == RT_WIZZLE) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			pHandler->setUniform(HD_U_TIME, fTime);
			SVIVector2 vInfo(10.0f, 0.02f); //x: move radius, y: move scale
			pHandler->setUniform(HD_U_RESERVE01, vInfo.x, vInfo.y);
		} else if (typeRTEffect == RT_GLASSTILE) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			//this is test code.
			float fSin = sin(fTime);

			SVIVector3 vInfo(4.0f, 1.0f, 1.0 + fSin * 0.3f); //x: move radius, y: move scale
			pHandler->setUniform(HD_U_RESERVE01, vInfo.x, vInfo.y, vInfo.z);
		} else if (typeRTEffect == RT_ZOOMBLUR) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			//this is test code.
			float fSin = sin(fTime);
			float fCos = cos(fTime);
			SVIVector3 vInfo(0.5f + fCos * 0.5f, 0.5f + fSin * 0.5f, 0.15f); //xy: center z: blur amount
			pHandler->setUniform(HD_U_RESERVE01, vInfo.x, vInfo.y, vInfo.z);
		} else if (typeRTEffect == RT_DIRECTIONBLUR) {
			float fTime = mSVIGLSurface->getCommonShaderVariables()->getTime();
			SVIVector2 vInfo(fTime, 0.003f); //xy: center z: blur amount
			pHandler->setUniform(HD_U_RESERVE01, vInfo.x, vInfo.y);
		}
	}
#endif
}

void SVIRenderPatch::processColoring(SVIProgramHandler * pHandler) {
	SVIBool bUseRenderTarget = isRenderTarget() ? SVITRUE : SVIFALSE;
	SVIBool bUseAddBlendType =
			(mOutfit->mBlendType == BLEND_ADD) ? SVITRUE : SVIFALSE;

	SVIVector4 colorValue = mPoly->getColor();
	SVIVector4 filteredColor = colorValue;
	//2011-10-20 masterkeaton27@gmail.com
	//to cover pre-multiplied alpha image resource issues
	SVIBool bUseTexture =
			(mTextureContainer != NULL && mTextureContainer->hasTexture()) ?
					SVITRUE : SVIFALSE;
	SVIBool bPreMultiplied =
			(bUseTexture
					&& mTextureContainer->getTexture()->getAlphaType()
							== SVITexture::PREMULTIPLIED) ? SVITRUE : SVIFALSE;

	if (bPreMultiplied && !bUseAddBlendType) {
		filteredColor = SVIVector4(colorValue.x * colorValue.w,
				colorValue.y * colorValue.w, colorValue.z * colorValue.w,
				colorValue.w);
	}
	if (bUseRenderTarget) {
		//render target slide should have a white color.
		filteredColor.r = filteredColor.g = filteredColor.b = 1.0f;
	}
	if (bUseAddBlendType) {
		pHandler->setUniform(HD_U_OPACITY, mOutfit->mGlobalOpacity);
	}
	pHandler->setUniform(HD_U_COLOR, filteredColor);
}

void SVIRenderPatch::processLighting(SVIProgramHandler * pHandler) {
	SVIBool bUseLinearLight = mOutfit->hasLinearLight();
	SVIBool bUseSpotLight = mOutfit->hasSpotLight();

	SVIFloat deltaX = cosf(SVI_DEGTORAD(mOutfit->mLightAngle));
	SVIFloat deltaY = sinf(SVI_DEGTORAD(mOutfit->mLightAngle));
	SVIVector2 targetOffset = SVIVector2(mOutfit->mLightCenter.x + deltaX,
				mOutfit->mLightCenter.y + deltaY);
	pHandler->setUniform(HD_U_LIGHT_TARGET, targetOffset);
    
    SVIVector4 lightDirection = mOutfit->mLightDirection;
    SVIVector3 tempDir = SVIVector3(lightDirection.x,lightDirection.y,lightDirection.z);
    tempDir.normalize();
    lightDirection.x =  tempDir.x;
    lightDirection.y =  tempDir.y;
    lightDirection.z =  tempDir.z;
    pHandler->setUniform(HD_U_LIGHT_DIRECTION, lightDirection);

	//2012-11-27 masterkeaton27@gmail.com : view direction for light-calculation
	SVIVector4 viewDirection = mOutfit->mViewDirection;
	tempDir = SVIVector3(viewDirection.x,viewDirection.y,viewDirection.z);
    tempDir.normalize();
    viewDirection.x =  tempDir.x;
    viewDirection.y =  tempDir.y;
    viewDirection.z =  tempDir.z;
    pHandler->setUniform(HD_U_VIEW_DIRECTION, viewDirection);

    SVIVector2 radius = SVIVector2(mOutfit->mLightRadius, 0.0f);
    pHandler->setUniform(HD_U_LIGHT_RADIUS, radius);

    pHandler->setUniform(HD_U_LIGHT_COLOR, mOutfit->mLightColor);
    pHandler->setUniform(HD_U_LIGHT_OFFSET, mOutfit->mLightCenter);
    pHandler->setUniform(HD_U_LIGHT_POWER, mOutfit->mLightPower);

    SVIVector2 screenSize = SVIVector2(mOutfit->mSize.x * mOutfit->mScale.x,
        mOutfit->mSize.y * mOutfit->mScale.y);
    pHandler->setUniform(HD_U_SCREEN_SIZE, screenSize);
}

void SVIRenderPatch::proccessGeometry(SVIProgramHandler * pHandler) {
	SVIUInt uStride = 0;

	pHandler->setAttribute(HD_A_POSITION, mPoly->getVertices(), uStride);
	pHandler->setAttribute(HD_A_UNITCOORD, mPoly->getUnitCoords(), uStride);
	
	if (mPoly->hasNormals()) {
		pHandler->setAttribute(HD_A_NORMAL, mPoly->getNormals(), uStride);
	}

	if (mPoly->hasTexcoords()) {
		pHandler->setAttribute(HD_A_TEXCOORD, mPoly->getTextureCoordinates(),
				uStride);
	} else {
		SVIVector2 sideSize = SVIVector2(mOutfit->mSize.x, mOutfit->mSize.y);
		SVIVector2 sidePivot = SVIVector2(mOutfit->mPivot.x, mOutfit->mPivot.y);
		pHandler->setUniform(HD_U_SIZE, sideSize);
		pHandler->setUniform(HD_U_PIVOT, sidePivot);
	}
	
	//2011-11-08 jbkim DMC Graphics Lab.
	if (mPoly->getAAcoords() != NULL) {
		pHandler->setAttribute(HD_A_AACOORD, 1, mPoly->getAAcoords());
	}

	SVIUInt primitiveType = GL_TRIANGLE_STRIP;
	SVIUInt polyPrimitiveType = mPoly->getPrimitiveType();
	if (polyPrimitiveType == SVIPoly::SVI_POLY_TRI_STRIP) {
		primitiveType = GL_TRIANGLE_STRIP;
	} else if (polyPrimitiveType == SVIPoly::SVI_POLY_TRI_LIST) {
		primitiveType = GL_TRIANGLES;
	} else if (polyPrimitiveType == SVIPoly::SVI_POLY_TRI_FAN) {
		primitiveType = GL_TRIANGLE_FAN;
	}

	//set model view projection matrix
	SVIMatrix modelMat = mPoly->getTransform();

	//2011-07-12 masterkeaton27@gmail.com
	SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();


	if (mOutfit->mIsOrthogonal){
		projectionMat._34 = 0.0f;
	}

	modelMat._43 -= cfTestDepth;

	pHandler->setUniform(HD_U_PROJ, projectionMat);
	pHandler->setUniform(HD_U_VIEW, modelMat);

	SVIMatrix normalMatrix = mPoly->getTransform();
	normalMatrix.invert();
	normalMatrix.transpose();
	pHandler->setUniform(HD_U_NORMAL, normalMatrix);

	//2012-11-29 masterkeaton27@gmail.com
	modelMat = modelMat.inverse4X4();
	modelMat.transpose();
	pHandler->setUniform(HD_U_INV_VIEW, modelMat);


	//2012-05-16 masterkeaton27@gmail.com
	//force linear set.
	//temporary set for D2 external test.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (mPoly->hasIndices()) {
		//2011-12-08 jongchae.moon DMC Graphics Lab.DMC Graphics Lab.
		if (mPoly->getIsAntiAliasing()) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glDrawElements(primitiveType, mPoly->getIndicesCount(),
					GL_UNSIGNED_SHORT, mPoly->getIndices());
			glDisable(GL_BLEND);
		} else {
			glDrawElements(primitiveType, mPoly->getIndicesCount(),
					GL_UNSIGNED_SHORT, mPoly->getIndices());
		}
	} else {
		glDrawArrays(primitiveType, 0, mPoly->getVerticeCount());
	}
	//

	pHandler->releaseAttribute(HD_A_POSITION);
	if (mPoly->hasNormals()) {
		pHandler->releaseAttribute(HD_A_NORMAL);
	}
	if (mPoly->hasTexcoords()) {
		pHandler->releaseAttribute(HD_A_TEXCOORD);
	}

	// 2011-11-08 jbkim DMC Graphics Lab.
	if (mPoly->getAAcoords() != NULL) {
		pHandler->releaseAttribute(HD_A_AACOORD);
	}
}

void SVIRenderPatch::render() {

	if (mOwnerSlide == NULL) {
		LOGE("CRITICAL ERROR: mOwerSlide is NULL! %x", (SVIInt)this);
		return;
	}

	if (mPoly != NULL && !mPoly->isValid())
		return;
		
if(mPoly != NULL)
	if (mPoly->hasAdditionalUpdate())
		mPoly->additionalUpdate();

	//2011-06-15 masterkeaton27@gmail.com
	//render shadow mesh
	if (mOutfit->hasShadow()){
		renderShadow();
	}

    //2012/12/10 rareboy0112@facebook.com
    //render morphing mesh
    //2013/02/05 rareboy0112@facebook.com
    //not use morphing geometry(chaned UX)
    SVIInt stencilDepth = 0;
    if(mPoly->hasMorphing())
    {
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, stencilDepth, 0xffffffff);
        glStencilOp(GL_INCR, GL_INCR, GL_INCR);

        renderMorphing();
    }

	SVIProgram * pProgram = NULL;
	SVIProgramHandler * pHandler = NULL;
	SVIProgramParams renderParams;

	//set proper elements for program
	preProcessTexture(&renderParams);
	preProcessRenderTargetEffect(&renderParams);
	preProcessLighting(&renderParams);
	preProcessBlending(&renderParams);
	preProcessDeformation(&renderParams);
	preProcessShaderEffect(&renderParams);
    preProcessFilter(&renderParams);
    preProcessTesselation(&renderParams);

	//get proper program
	pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
	if (pProgram == NULL || !pProgram->getActivated())
		return;

	pHandler = pProgram->getHandler();
	pProgram->use();

	//process shader pipeline variables.
    processFilter(pHandler);
	processShaderEffect(pHandler);
	processDeformation(pHandler);
	processRenderTargetEffect(pHandler);
	processColoring(pHandler);
	processLighting(pHandler);
	processTextureScale(pHandler);
	processTextureBlendOperation(pHandler);

    ///finally, render.
    //2013/02/05 rareboy0112@facebook.com
    //not use morphing geometry(chaned UX)
    if(mPoly->hasMorphing())
    {
        glStencilFunc(GL_LESS, stencilDepth, 0xffffffff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    }

    //render front-face
	processFrontFaceRender(pHandler);

	//render back-face
	processBackFaceRender(pHandler);

	//if owner slide has border, then render
	if (mPoly->hasAdditionalRender())
		mPoly->additionalRender(mTextureContainer);

    if(mPoly->hasMorphing())
    {
        mPoly->resetMorphing();
        glDisable(GL_STENCIL_TEST);
    }
}

//2011-05-24 masterkeaton27@gmail.com
//all types, shaders, texture id must be setted
//before this function called by renderer
SVIBool SVIRenderPatch::initialize(SVISlideOutfit * pOutfit) {
	mOutfit = pOutfit;
	SVIBool bResult = SVIFALSE;
	bResult = buildPolyObject(pOutfit);
	mMemoryUsed = mPoly->getMemoryUsed();
	return bResult;
}

SVIBool SVIRenderPatch::finalize() {
	SVI_SVIFE_DELETE(mPoly);

	if(filterTexture0){
		glDeleteTextures(1, &filterTexture0);
		filterTexture0 = 0;
	}
	
	return SVITRUE;
}


SVIBool SVIRenderPatch::createPolyObject(){
	//2012-03-16 masterkeaton27@gmail.com
	SVIBool isRecreated = SVIFALSE;
	if (mGeometryType == SVI_PATCH_ROUND) {
		if (mPoly == NULL) {
			//2011.12.09 jongchae.moon
			if (mOutfit->mEnableAA) {
				mPoly = new SVIRoundPolyExt(mSVIGLSurface);
			} else {
				mPoly = new SVIRoundPoly(mSVIGLSurface);
			}
			isRecreated = true;
			mPoly->setGeometryType(SVI_PATCH_ROUND);
		} else if (mPoly != NULL
			&& mPoly->getGeometryType() != SVI_PATCH_ROUND) {
				SVI_SVIFE_DELETE(mPoly);
				//2011.12.09 jongchae.moon
				if (mOutfit->mEnableAA) {
					mPoly = new SVIRoundPolyExt(mSVIGLSurface);
				} else {
					mPoly = new SVIRoundPoly(mSVIGLSurface);
				}
				isRecreated = true;
				mPoly->setGeometryType(SVI_PATCH_ROUND);
		}

	} else if (mGeometryType == SVI_PATCH_QUAD) {
		if (mPoly == NULL) {
			//2011.12.09 jongchae.moon
			if (mOutfit->mEnableAA) {
				mPoly = new SVIQuadPolyExt(mSVIGLSurface);
			} else {
				mPoly = new SVIQuadPoly(mSVIGLSurface);
			}
			isRecreated = true;
			mPoly->setGeometryType(SVI_PATCH_QUAD);

		} else if (mPoly != NULL && mPoly->getGeometryType() != SVI_PATCH_QUAD) {
			SVI_SVIFE_DELETE(mPoly);
			//2011.12.09 jongchae.moon
			if (mOutfit->mEnableAA) {
				mPoly = new SVIQuadPolyExt(mSVIGLSurface);
			} else {
				mPoly = new SVIQuadPoly(mSVIGLSurface);
			}
			isRecreated = true;
			mPoly->setGeometryType(SVI_PATCH_QUAD);
		}
	} else if (mGeometryType == SVI_PATCH_TWIST) {
		if (mPoly == NULL) {
			isRecreated = true;
			mPoly = new SVITwistPoly(mSVIGLSurface);
			mPoly->setGeometryType(SVI_PATCH_TWIST);
		} else if (mPoly != NULL
			&& mPoly->getGeometryType() != SVI_PATCH_TWIST) {
				SVI_SVIFE_DELETE(mPoly);
				isRecreated = true;
				mPoly = new SVITwistPoly(mSVIGLSurface);
				mPoly->setGeometryType(SVI_PATCH_TWIST);
		}
    } else if (mGeometryType == SVI_PATCH_CONECURVE) {
        if (mPoly == NULL) {
            isRecreated = true;
            mPoly = new SVIConeCurvePoly(mSVIGLSurface);
            mPoly->setGeometryType(SVI_PATCH_CONECURVE);
        } else if (mPoly != NULL
            && mPoly->getGeometryType() != SVI_PATCH_CONECURVE) {
                SVI_SVIFE_DELETE(mPoly);
                isRecreated = true;
                mPoly = new SVIConeCurvePoly(mSVIGLSurface);
                mPoly->setGeometryType(SVI_PATCH_CONECURVE);
        }
    } else if (mGeometryType == SVI_PATCH_EXPLODE) {
		if (mPoly == NULL) {
			isRecreated = true;
			mPoly = new SVIExplodePoly(mSVIGLSurface);
			mPoly->setGeometryType(SVI_PATCH_EXPLODE);
		} else if (mPoly != NULL
			&& mPoly->getGeometryType() != SVI_PATCH_EXPLODE) {
				SVI_SVIFE_DELETE(mPoly);
				isRecreated = true;
				mPoly = new SVIExplodePoly(mSVIGLSurface);
				mPoly->setGeometryType(SVI_PATCH_EXPLODE);
		}
	} else if (mGeometryType == SVI_PATCH_PANNEL) {
		if (mPoly == NULL) {
			isRecreated = true;
			mPoly = new SVIPannelPoly(mSVIGLSurface);
			mPoly->setGeometryType(SVI_PATCH_PANNEL);
		} else if (mPoly != NULL
			&& mPoly->getGeometryType() != SVI_PATCH_PANNEL) {
				SVI_SVIFE_DELETE(mPoly);
				isRecreated = true;
				mPoly = new SVIPannelPoly(mSVIGLSurface);
				mPoly->setGeometryType(SVI_PATCH_PANNEL);
		}
	} else if (mGeometryType == SVI_PATCH_BRICK) {
		if (mPoly == NULL) {
			isRecreated = true;
			mPoly = new SVIBrickPoly(mSVIGLSurface);
			mPoly->setGeometryType(SVI_PATCH_BRICK);
		} else if (mPoly != NULL
			&& mPoly->getGeometryType() != SVI_PATCH_BRICK) {
				SVI_SVIFE_DELETE(mPoly);
				isRecreated = true;
				mPoly = new SVIBrickPoly(mSVIGLSurface);
				mPoly->setGeometryType(SVI_PATCH_BRICK);
		}
	} else if (mGeometryType == SVI_PATCH_DOME){
        if (mPoly == NULL) {
            isRecreated = true;
            mPoly = new SVIDomePoly(mSVIGLSurface);
            mPoly->setGeometryType(SVI_PATCH_DOME);
        } else if (mPoly != NULL
            && mPoly->getGeometryType() != SVI_PATCH_DOME) {
                SVI_SVIFE_DELETE(mPoly);
                isRecreated = true;
                mPoly = new SVIDomePoly(mSVIGLSurface);
                mPoly->setGeometryType(SVI_PATCH_DOME);
        }
    } else if (mGeometryType == SVI_PATCH_DRUM){
        if (mPoly == NULL) {
            isRecreated = true;
            mPoly = new SVIDrumPoly(mSVIGLSurface);
            mPoly->setGeometryType(SVI_PATCH_DRUM);
        } else if (mPoly != NULL
            && mPoly->getGeometryType() != SVI_PATCH_DRUM) {
                SVI_SVIFE_DELETE(mPoly);
                isRecreated = true;
                mPoly = new SVIDrumPoly(mSVIGLSurface);
                mPoly->setGeometryType(SVI_PATCH_DRUM);
        }
    }  else if (mGeometryType == SVI_PATCH_PATH){
        if (mPoly == NULL) {
            isRecreated = true;
            mPoly = new SVIPathPoly(mSVIGLSurface);
            mPoly->setGeometryType(SVI_PATCH_PATH);
        } else if (mPoly != NULL
            && mPoly->getGeometryType() != SVI_PATCH_PATH) {
                SVI_SVIFE_DELETE(mPoly);
                isRecreated = true;
                mPoly = new SVIPathPoly(mSVIGLSurface);
                mPoly->setGeometryType(SVI_PATCH_PATH);
        }
    }
    //2012/12/06 rareboy0112@facebook.com
    //tesselation geometry
    else if(mGeometryType == SVI_PATCH_TESSELATION)
    {
        if(mPoly == NULL)
        {
            isRecreated = true;
            mPoly = new SVITesselationPoly(mSVIGLSurface);
            mPoly->setGeometryType(SVI_PATCH_TESSELATION);
        }
        else if (mPoly != NULL && mPoly->getGeometryType() != SVI_PATCH_TESSELATION)
        {
            SVI_SVIFE_DELETE(mPoly);
            isRecreated = true;
            mPoly = new SVITesselationPoly(mSVIGLSurface);
            mPoly->setGeometryType(SVI_PATCH_TESSELATION);
        }
    }

	return isRecreated;
}


void SVIRenderPatch::setPolyType(SVISlideOutfit * pOutfit){
	if (pOutfit->mDeformType == DEFORM_NONE) {
		if (pOutfit->isRound()) {
			setGeometryType(SVIRenderPatch::SVI_PATCH_ROUND);
			//2011.12.09 jongchae.moon
			if (pOutfit->mEnableAA && pOutfit->mCornerRadius < 0.5f) {
				setGeometryType(SVIRenderPatch::SVI_PATCH_QUAD);
			}
		}
        //2012/12/08 rareboy0112@facebook.com
        //added Tesselation type
        else if(pOutfit->mTesselation == SVITRUE)
        {
            setGeometryType(SVIRenderPatch::SVI_PATCH_TESSELATION);
        }
		else {
			setGeometryType(SVIRenderPatch::SVI_PATCH_QUAD);
		}
	} else if (pOutfit->mDeformType == DEFORM_TWIST) {
		setGeometryType(SVIRenderPatch::SVI_PATCH_TWIST);
    } else if (pOutfit->mDeformType == DEFORM_CONECURVE) {
        setGeometryType(SVIRenderPatch::SVI_PATCH_CONECURVE);
    } else if (pOutfit->mDeformType == DEFORM_EXPLODE) {
		setGeometryType(SVIRenderPatch::SVI_PATCH_EXPLODE);
	} else if (pOutfit->mDeformType == DEFORM_PANNEL) {
		setGeometryType(SVIRenderPatch::SVI_PATCH_PANNEL);
	} else if (pOutfit->mDeformType == DEFORM_BRICK) {
		setGeometryType(SVIRenderPatch::SVI_PATCH_BRICK);
    } else if (pOutfit->mDeformType == DEFORM_DOME) {
        setGeometryType(SVIRenderPatch::SVI_PATCH_DOME);
    } else if (pOutfit->mDeformType == DEFORM_DRUM) {
        setGeometryType(SVIRenderPatch::SVI_PATCH_DRUM);
    } else if (pOutfit->mDeformType == DEFORM_PATH) {
        setGeometryType(SVIRenderPatch::SVI_PATCH_PATH);
    }

}

void SVIRenderPatch::setPolyProperties(SVIPoly * pPoly, SVISlideOutfit * pOutfit){
	pPoly->setTransform(pOutfit->mGlobalTransform);
	pPoly->setPivot(pOutfit->mPivot);

	//2011-09-14 masterkeaton27@gmail.com
	//blend type add.
	if (pOutfit->mBlendType == BLEND_ADD) {
		pPoly->setColor(pOutfit->mColor);
		pPoly->setOpacity(pOutfit->mGlobalOpacity);
	} else {
		pPoly->setColor(
			SVIVector4(pOutfit->mColor.r, pOutfit->mColor.g,
			pOutfit->mColor.b,
			pOutfit->mColor.a * pOutfit->mGlobalOpacity));
		pPoly->setOpacity(1.0f);
	}
	pPoly->setSize(pOutfit->mSize);
	pPoly->setScale(SVIVector2(1.0f, 1.0f));
	pPoly->setOrthogonal(pOutfit->mIsOrthogonal);
	pPoly->setAddBlend((pOutfit->mBlendType == BLEND_ADD) ? SVITRUE : SVIFALSE);
	pPoly->setTextureScale(pOutfit->mTextureScale);

	//2011-08-29 masterkeaton27@gmail.com
	//sprite texture properties
	SVIVector2 offset, size;
	SVIRect& textureRegion = pOutfit->mTextureSpriteRegion;
	SVIRect& textureBackFaceRegion = pOutfit->mTextureBackFaceSpriteRegion;
	SVIRect& textureScaleFitRegion = pOutfit->mTextureScaleFitRegion;
	if (mTextureContainer != NULL) {

		SVISize sizeSrc = mTextureContainer->getSize();
		offset.x = textureRegion.mOrigin.mX / sizeSrc.mWidth;
		offset.y = textureRegion.mOrigin.mY / sizeSrc.mHeight;
		size.x = textureRegion.mSize.mWidth / sizeSrc.mWidth;
		size.y = textureRegion.mSize.mHeight / sizeSrc.mHeight;

		pPoly->setTextureRegion(offset, size);

		//2011-10-10 masterkeaton27@gmail.com
		//scale to fit value is unit value (normalized)
		offset.x = textureScaleFitRegion.mOrigin.mX;
		offset.y = textureScaleFitRegion.mOrigin.mY;
		size.x = textureScaleFitRegion.mSize.mWidth;
		size.y = textureScaleFitRegion.mSize.mHeight;

		pPoly->setTextureScaleFitRegion(offset, size);

		//2012-01-06 masterkeaton27@gmail.com
		SVITexture * backFaceTexture = mTextureContainer->getBackFaceTexture();
		SVICaptureData * backFaceCaptureData =
			mTextureContainer->getBackFaceCaptureTexture();
		if (backFaceCaptureData != NULL) {
			SVIVector2 size = backFaceCaptureData->getSize();
			float width = size.x;
			float height = size.y;
			offset.x = textureBackFaceRegion.mOrigin.mX / width;
			offset.y = textureBackFaceRegion.mOrigin.mY / height;
			size.x = textureBackFaceRegion.mSize.mWidth / width;
			size.y = textureBackFaceRegion.mSize.mHeight / height;
			pPoly->setBackFaceTextureRegion(offset, size);
		} else if (backFaceTexture != NULL) {
			float width = backFaceTexture->getWidth();
			float height = backFaceTexture->getHeight();
			offset.x = textureBackFaceRegion.mOrigin.mX / width;
			offset.y = textureBackFaceRegion.mOrigin.mY / height;
			size.x = textureBackFaceRegion.mSize.mWidth / width;
			size.y = textureBackFaceRegion.mSize.mHeight / height;
			pPoly->setBackFaceTextureRegion(offset, size);
		}
	} else {
		offset.y = offset.x = 0.0f;
		size.y = size.x = 1.0f;
		pPoly->setTextureRegion(offset, size);
		pPoly->setBackFaceTextureRegion(offset, size);

		//2011-10-10 masterkeaton27@gmail.com
		//scale to fit value is unit value (normalized)
		offset.y = offset.x = 0.0f;
		size.y = size.x = 1.0f;
		pPoly->setTextureScaleFitRegion(offset, size);
	}
}

SVIBool SVIRenderPatch::buildPolyObject(SVISlideOutfit * pOutfit) {
	//2012-06-20 masterkeaton27@gmail.comcode refactoring
	setPolyType(pOutfit);

	//2012-03-16 masterkeaton27@gmail.com
	//2012-06-20 masterkeaton27@gmail.comcode refactoring
	SVIBool isRecreated = createPolyObject();
	
	//2012-02-21 marx.kim
	mPoly->setRenderPatch(this);
	mPoly->releaseUpdate();
	
	//2012-06-20 masterkeaton27@gmail.comcode refactoring
	setPolyProperties(mPoly,pOutfit);

	if (mPoly->getGeometryType() == SVI_PATCH_ROUND) {
		SVIRoundPoly * pRoundPoly = (SVIRoundPoly *) mPoly;

		pRoundPoly->setBorderColor(
				SVIVector4(pOutfit->mBorderColor.r, pOutfit->mBorderColor.g,
						pOutfit->mBorderColor.b,
						pOutfit->mBorderColor.a * pOutfit->mGlobalOpacity));

		pRoundPoly->setBorderWidth(pOutfit->mBorderWidth);
		pRoundPoly->setRoundRadius(pOutfit->mCornerRadius);
		//2011-06-24 masterkeaton27@gmail.com
		//round polygon : auto corner 1/4 fan details setting
		//2011-07-04 masterkeaton27@gmail.com
		//trade off value fixed (5). this value can be more bigger when we use better devices.
		pRoundPoly->setRoundLevel(10);
	} else if (mPoly->getGeometryType() == SVI_PATCH_QUAD) {
		//2011-06-13 masterkeaton27@gmail.com
		//quad (rectangle) polygon with border geometry
		SVIQuadPoly * pQuadPoly = (SVIQuadPoly *) mPoly;
		if (pOutfit->hasBorder()) {
			pQuadPoly->setBorderColor(
					SVIVector4(pOutfit->mBorderColor.r, pOutfit->mBorderColor.g,
							pOutfit->mBorderColor.b,
							pOutfit->mBorderColor.a * pOutfit->mGlobalOpacity));
			pQuadPoly->setBorderWidth(pOutfit->mBorderWidth);
		}
	} else if (mPoly->getGeometryType() == SVI_PATCH_TWIST) {
		SVITwistPoly * pTwistPoly = (SVITwistPoly *) mPoly;
		pTwistPoly->setOutfit(pOutfit);

		//2012-02-29 masterkeaton27@gmail.com
		//update data and fill buffers before rendering 
		//to solve blink issue when switching heavy polygon render type. 
		if (isRecreated){
			pTwistPoly->build();
			pTwistPoly->additionalUpdate();		
			pTwistPoly->prepareFrontFace();
			return SVITRUE;
		}

    } else if (mPoly->getGeometryType() == SVI_PATCH_CONECURVE) { 
        SVIConeCurvePoly * pConeCurvePoly = (SVIConeCurvePoly *) mPoly;
        pConeCurvePoly->setOutfit(pOutfit);

        //2012-12-11 masterkeaton27@gmail.com
        //update data and fill buffers before rendering 
        //to solve blink issue when switching heavy polygon render type. 
        if (isRecreated){
            pConeCurvePoly->build();
            pConeCurvePoly->additionalUpdate();		
            pConeCurvePoly->prepareFrontFace();
            return SVITRUE;
        }

    } else if (mPoly->getGeometryType() == SVI_PATCH_EXPLODE) {
		SVIExplodePoly * pExplodePoly = (SVIExplodePoly *) mPoly;
		pExplodePoly->setOutfit(pOutfit);

		//2012-02-29 masterkeaton27@gmail.com
		//update data and fill buffers before rendering 
		//to solve blink issue when switching heavy polygon render type. 
		if (isRecreated){
			pExplodePoly->build();
			pExplodePoly->additionalUpdate();		
			pExplodePoly->prepareFrontFace();
			return SVITRUE;
		}

	} else if (mPoly->getGeometryType() == SVI_PATCH_PANNEL) {
			SVIPannelPoly * pPannelPoly = (SVIPannelPoly *) mPoly;
			if (isRecreated){
				pPannelPoly->build();
				pPannelPoly->setOutfit(pOutfit);
				pPannelPoly->prepareFrontFace();
				return SVITRUE;
			}
			return pPannelPoly->build();
	}else if (mPoly->getGeometryType() == SVI_PATCH_BRICK) {
		SVIBrickPoly * pBrickPoly = (SVIBrickPoly *) mPoly;
		if (isRecreated){
			pBrickPoly->build();
			pBrickPoly->setOutfit(pOutfit);
			pBrickPoly->prepareFrontFace();
			return SVITRUE;
		}
		return pBrickPoly->build();
    }else if (mPoly->getGeometryType() == SVI_PATCH_DOME) {
        SVIDomePoly * pDomePoly = (SVIDomePoly *) mPoly;
        if (isRecreated){
            pDomePoly->build();
            pDomePoly->setOutfit(pOutfit);
            pDomePoly->prepareFrontFace();
            return SVITRUE;
        }
        return pDomePoly->build();
    }else if (mPoly->getGeometryType() == SVI_PATCH_DRUM) {
        SVIDrumPoly * pDrumPoly = (SVIDrumPoly *) mPoly;
        if (isRecreated){
            pDrumPoly->build();
            pDrumPoly->setOutfit(pOutfit);
            pDrumPoly->prepareFrontFace();
            return SVITRUE;
        }
        return pDrumPoly->build();
    }else if (mPoly->getGeometryType() == SVI_PATCH_PATH){
        SVIPathPoly * pPathPoly = (SVIPathPoly *) mPoly;

        pPathPoly->clearPath();
        pPathPoly->setOutfit(pOutfit);
        
        if (isRecreated){
             pPathPoly->build();
            pPathPoly->prepareFrontFace();
           
            return SVITRUE;
        }
        return pPathPoly->build();
    }
    else if(mPoly->getGeometryType() == SVI_PATCH_TESSELATION)
    {
        SVITesselationPoly *pTesselationPoly = (SVITesselationPoly *)mPoly;
        if (isRecreated)
        {
            //2012/12/09 setOutfit method always must be called first.
            pTesselationPoly->setOutfit(pOutfit);
            pTesselationPoly->build();

            return SVITRUE;
        }

        if(pOutfit->mChangeShape == SVITRUE)
        {
            pTesselationPoly->setChangeShape();
            pOutfit->mChangeShape = SVIFALSE;
        }
        return pTesselationPoly->build();
    }

	return mPoly->build();
}
}
