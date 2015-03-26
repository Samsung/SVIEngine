#define USE_COMPILE 1

#define USE_BINARY_PROGRAM
//#define WRITE_BINARY_PROGRAM

#include "../SVICores.h"
//#include "SVIProgram.h"
#include "SVIProgramManager.h"


#include "SVIBinaryProgram.h"


#include "SVIRenderer.h"

namespace SVI{

	const SVIChar* SVIProgramHandleSymbols[] =  {
		"uView",			//HD_U_VIEW
		"uInvView",			//HD_U_INV_VIEW
		"uProjection", 		//HD_U_PROJ
		"uNormalMatrix",	//HD_U_NORMAL
		"uEyePos",			//HD_U_EYEPOS
		"aPosition", 		//HD_A_POSITION
		"aNormal",			//HD_A_NORMAL
		"aTexcoord", 		//HD_A_TEXCOORD
        "aHeights", 		//HD_A_HEIGHTS
		"uColor", 			//HD_U_COLOR
		"uSampler0", 		//HD_U_SVIMPLER_0
		"uSampler1", 		//HD_U_SVIMPLER_1
		"uSampler2", 		//HD_U_SVIMPLER_2
		"uOpacity",			//HD_U_OPACITY
		"uLightOffset", 	//HD_U_LIGHT_OFFSET
		"uLightTarget",		//DH_U_LIGHT_TARGET
		"uLightRadius",		//HD_U_LIGHT_RADIUS
		"uLightColor",		//HD_U_LIGHT_COLOR
		"uLightPower",	    //HD_U_LIGHT_POWER
        "uLightDirection",  //HD_U_LIGHT_DIRECTION
		"uViewDirection",   //HD_U_VIEW_DIRECTION
		"uShadowOffset", 	//HD_U_SHADOW_OFFSET
		"uShadowRadius",	//HD_U_SHADOW_RADIUS
		"uShadowColor",		//HD_U_SHADOW_COLOR
		"uFitInfo",			//HD_U_FITINFO
		"uTexInfo",			//HD_U_TEXTUREINFO
		"uTime",			//HD_U_TIME		
		"uScreenSize",		//HD_U_SCREEN_SIZE
		"uSize",			//HD_U_SIZE
		"uOffset",			//HD_U_PIVOT



		"uAmbient",			//HD_U_AMBIENT,			//C04 : r,g,b,a
		"uDiffuse",			//HD_U_DIFFUSE,			//C04 : r,g,b,a
		"uSpecular",		//HD_U_SPECULAR,		//C04 : r,g,b,a
		"uShininess",		//HD_U_SHINIESS,		//V01 : power

		"uReserve01",		//HD_U_RESERVE01,			
		"uReserve02",		//HD_U_RESERVE02,			
		"uReserve03",		//HD_U_RESERVE03,			
		"uReserve04",		//HD_U_RESERVE04,			
		"uReserve05",		//HD_U_RESERVE05,			

		//2011.11.03 jongchae.moon  //from DMC Graphics lab.
		"aAACoord",			//HD_A_AACOORD
		// 20111111 jbkim  //from DMC Graphics lab.
		"uNumGradationColors",
		"uGradationColors",
		"uGradationOffsets",
		"uGradationStartPos",
		"uGradationEndPos",
		//

		//2012-06-21 masterkeaton27@gmail.com
		//for new shader effect for mobile office
		"uEdgeRatio",
		"uEdgeRadius",
		"uLerpWidth",
		"uLerpBase",

	        //2012-12-17 masterkeaton27@gmail.com
	        "uSampleWeight",
	        "uSampleOffset",

		"aUnitCoord" //HD_A_UNITCOORD
	};

	#define SVI_OK 1
	#define SVI_ERROR -1
	#define SVI_NO_HANDLE -1

	#define SVI_CHECK_HANDLE(x) if (x == SVI_NO_HANDLE) return;
	#define SVI_CHECK_HANDLE_ERROR(x) if (x == SVI_NO_HANDLE) return SVI_ERROR;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SVIPROGRAMPARAMS
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void SVIProgramParams::addElement(SVIProgramElementType type){
		mElements.push_back(type);
	}

	void SVIProgramParams::reset(){
		clear();
	}

	void SVIProgramParams::clear(){
		mElements.clear();
	}

	SVIProgramParams::SVIProgramParams(){}

	SVIProgramParams::~SVIProgramParams(){
		clear();
	}

	SVIProgramType SVIProgramParams::getProgram(){
		SVIInt size = mElements.size();
		SVIBool bUseColor = SVIFALSE;
		SVIBool bUseTexture = SVIFALSE;
		SVIBool bUseLightSpot = SVIFALSE;
		SVIBool bUseLightLinear = SVIFALSE;
		SVIBool bUseShadow = SVIFALSE;
		SVIBool bUseBlendAdd = SVIFALSE;
		SVIBool bUseBlendMultiply = SVIFALSE;
		SVIBool bUseTextureFit = SVIFALSE;

		SVIBool bUseRenderTarget = SVIFALSE;
		SVIBool bUseRipple = SVIFALSE;
		SVIBool bUsePulse = SVIFALSE;
		SVIBool bUseDreamVision = SVIFALSE;
		SVIBool bUseWizzle = SVIFALSE;
		SVIBool bUsePaperFold = SVIFALSE;
		SVIBool bUseMagnify = SVIFALSE;
		SVIBool bUseGlassTile = SVIFALSE;
		SVIBool bUseZoomBlur = SVIFALSE;
		SVIBool bUseDirectionBlur = SVIFALSE;

        SVIBool bTesselation = SVIFALSE;
		SVIBool bUse3DModel = SVIFALSE;
		SVIBool bUse3DShadow = SVIFALSE;
		//2012-04-03 masterkeaton27@gmail.com
		SVIBool bUse3DModelBackFace = SVIFALSE;

		//2012-02-16 masterkeaton27@gmail.com
		//from SURC
		SVIBool bUsePhysic3DModel = SVIFALSE;

		//2012-04-09 masterkeaton27@gmail.com
		SVIBool bUse3DAA = SVIFALSE;

        SVIBool bUse3DModelOffset = SVIFALSE;

		// 20111111 jbkim
		//SVIBool bUseLinearGradation = SVIFALSE;
		//SVIBool bUseRadialGradation = SVIFALSE;

		//2102-06-21 masterkeaton27@gmail.com
		//new shader effect for mobile office
		SVIBool bUseLinearSwipe = SVIFALSE;
		SVIBool bUseRadialSwipe = SVIFALSE;

		//2012-11-29 masterkeaton27@gmail.com
		SVIBool bUseNormalMap = SVIFALSE;

        //2012-12-10 masterkeaton27@gmail.com
        SVIBool bUseSpecularNormalMap  = SVIFALSE;

        //2013-06-24 masterkeaton27@gmail.com
        SVIBool bUseCPUTexture = SVIFALSE;

        //2013-06-26 masterkeaton27@gmail.com
        SVIBool bUseSketchFilter = SVIFALSE;
        SVIBool bUseOilPaintFilter = SVIFALSE;
        SVIBool bUseWaterColorFilter = SVIFALSE;
        SVIBool bUseCartoonFilter = SVIFALSE;
        SVIBool bUseRadialFilter = SVIFALSE;
        SVIBool bUseMonotoneFilter = SVIFALSE;
	 	SVIBool bUseSmokeFilter = SVIFALSE;

		//checking activated elements
		for (SVIInt n = 0; n < size; n++){
			switch (mElements[n]){
			case EM_COLOR: bUseColor = SVITRUE; break;
			case EM_TEXTURE: bUseTexture = SVITRUE; break;
			case EM_TEXTURE_SCALE: bUseTextureFit = SVITRUE; break;
			case EM_BLEND_ADD: bUseBlendAdd = SVITRUE; break;
			case EM_BLEND_MLT: bUseBlendMultiply = SVITRUE; break;
			case EM_LIGHT_SPOT: bUseLightSpot = SVITRUE; break;
			case EM_LIGHT_LINEAR: bUseLightLinear = SVITRUE; break;
			case EM_SHADOW: bUseShadow = SVITRUE; break;

			//2012-03-13 masterkeaton27@gmail.com
			case EM_SHADOW3D: bUse3DShadow = SVITRUE; break;

			case EM_3DMODEL: bUse3DModel = SVITRUE; break;

            //2012/12/10 rareboy0112@facebook.com
            case EM_TESSELATION: bTesselation = SVITRUE; break;

			case EM_NORMALMAP : bUseNormalMap = SVITRUE; break;

            case EM_NORMALSPECULARMAP : bUseSpecularNormalMap = SVITRUE; break;
            //2012-11-21
            case EM_3DMODEL_OFFSET: bUse3DModelOffset = SVITRUE; break;

			//2011-12-26 d.kovalenko
			case EM_PHYSIC3DMODEL: bUsePhysic3DModel = SVITRUE; break;
			//2011-11-11 masterkeaton27@gmail.com
			case EM_RENDER_TARGET: bUseRenderTarget = SVITRUE; break;
			//2012-04-09 masterkeaton27@gmail.com
			case EM_PAGE3D_AA: bUse3DAA = SVITRUE; break;

#if SVI_USE_POST_EFFECT
			case EM_RT_RIPPLE:bUseRipple = SVITRUE; break;
			case EM_RT_PULSE:bUsePulse = SVITRUE; break;
			case EM_RT_DREAMVISION:bUseDreamVision = SVITRUE; break;
			case EM_RT_WIZZLE:bUseWizzle = SVITRUE; break;
			case EM_RT_PAPERFOLD:bUsePaperFold = SVITRUE; break;
			case EM_RT_MAGNIFY: bUseMagnify = SVITRUE; break;
			case EM_RT_GLASSTILE: bUseGlassTile = SVITRUE; break;
			case EM_RT_ZOOMBLUR: bUseZoomBlur = SVITRUE; break;
			case EM_RT_DIRECTIONBLUR: bUseDirectionBlur = SVITRUE; break;
#endif
			// 2011-11-11 jbkim  DMC Graphics lab.
			//case EM_LINEAR_GRADATION: bUseLinearGradation = SVITRUE; break;
			//case EM_RADIAL_GRADATION: bUseRadialGradation = SVITRUE; break;

			//2012-06-21 masterkeaton27@gmail.com
			case EM_LINEAR_SWIPE: bUseLinearSwipe = SVITRUE; break;
			case EM_RADIAL_SWIPE: bUseRadialSwipe = SVITRUE; break;

            //2013-06-24 masterkeaton27@gmail.com
            case EM_CPU_TEXTURE: bUseCPUTexture = SVITRUE; break;
            
            //2013-06-26 masterkeaton27@gmail.com
            case EM_FILTER_OILPAINT : bUseOilPaintFilter = SVITRUE; break;
            case EM_FILTER_SKETCH : bUseSketchFilter = SVITRUE; break;
            case EM_FILTER_WATERCOLOR : bUseWaterColorFilter = SVITRUE; break;
            case EM_FILTER_CARTOON: bUseCartoonFilter = SVITRUE; break;
            case EM_FILTER_RADIAL: bUseRadialFilter = SVITRUE; break;
            case EM_FILTER_MONOTONE: bUseMonotoneFilter = SVITRUE; break;
	     	case EM_FILTER_SMOKE: bUseSmokeFilter = SVITRUE; break;
            default:
                break;
			}
		}
        
        //2013-06-26 masterkeaton27@gmail.com
        if (bUseSmokeFilter){
            return PR_FILTER_SMOKE;
        } 
        if (bUseMonotoneFilter){
            return PR_FILTER_MONOTONE;
        }        
        if (bUseRadialFilter){
            return PR_FILTER_RADIAL;
        }
        if (bUseCartoonFilter){
            return PR_FILTER_CARTOON;
        }        
        if (bUseSketchFilter){
            return PR_FILTER_SKETCH;
        }
        if (bUseOilPaintFilter){
            return PR_FILTER_OILPAINT;
        }
        if (bUseWaterColorFilter){
            return PR_FILTER_WATERCOLOR;
        }

        //2013-06-24 masterkeaton27@gmail.com
        if (bUseCPUTexture){
            return PR_TEX_CPU;
        }

        //2012-12-10 masterkeaton27@gmail.com
        //to using normal-specular light.
        if (bUseSpecularNormalMap){
            return PR_SPECULAR_NORMALMAP;
        }else if (bUseNormalMap){
			return PR_NORMALMAP;
		}

        //2012/12/10 rareboy0112@facebook.com
        if(bTesselation)
            return PR_TESSELATION;

        //let's find out which shader is proper one.
        if (bUse3DModelOffset){
            return PR_3DMODEL_OFFSET;
        }else
		if (bUse3DShadow){ //2012-03-13 masterkeaton27@gmail.com
			return PR_3DSHADOW;
		}else
		if (bUse3DModel){
			//if (bUse3DAA) return PR_3DMODELAA;
			return PR_3DMODEL;
		}else if (bUsePhysic3DModel){ //2012-02-16 masterkeaton27@gmail.comfrom SURC (dm.sedukhin, dm.kovalenko)
			if (bUse3DAA) return PR_PHYSIC3DMODELAA;
			return PR_PHYSIC3DMODEL;
		}

		//2012-06-21 masterkeaton27@gmail.com
		//new shader effect for mobile office
		if (bUseLinearSwipe){
			return PR_LINEAR_SWIPE;
		}else if (bUseRadialSwipe){
			return PR_RADIAL_SWIPE;
		}

#if SVI_USE_POST_EFFECT		
		else if (bUseRipple){
			return PR_RIPPLE_RT;
		}else if (bUsePulse){
			return PR_PULSE_RT;
		}else if (bUseDreamVision){
			return PR_DREAMVISION_RT;
		}else if (bUseWizzle){
			return PR_WIZZLE_RT;
		}else if (bUsePaperFold){
			return PR_PAPERFOLD_RT;
		}else if (bUseMagnify){
			return PR_MAGNIFY_RT;
		}else if (bUseGlassTile){
			return PR_GLASSTILE_RT;
		}else if (bUseZoomBlur){
			return PR_ZOOMBLUR_RT;
		}else if (bUseDirectionBlur){
			return PR_DIRECTIONBLUR_RT;
		}
#endif
		else if (bUseRenderTarget){
			return PR_TEX_MLT;
		}else if (bUseColor){
			if (bUseLightLinear){
				return PR_COLOR_LT_LINEAR;
			}else if (bUseLightSpot){
				return PR_COLOR_LT_SPOT;
			}else if (bUseShadow){
				return PR_COLOR_SHADOW;
			}
			return PR_COLOR;
		}else if (bUseTextureFit){
			if (bUseBlendAdd){
				if (bUseLightSpot){
					return PR_TEX_ADD_LT_SPOT_FIT;
				}else if (bUseLightLinear){
					return PR_TEX_ADD_LT_LINEAR_FIT;
				}else if (bUseShadow){
					return PR_TEX_ADD_SHADOW_FIT;
				}
				return PR_TEX_ADD_FIT;
			}else if (bUseBlendMultiply){
				if (bUseLightSpot){
					return PR_TEX_MLT_LT_SPOT_FIT;
				}else if (bUseLightLinear){
					return PR_TEX_MLT_LT_LINEAR_FIT;
				}else if (bUseShadow){
					return PR_TEX_MLT_SHADOW_FIT;
				}
				return PR_TEX_MLT_FIT;
			}
		}else if (bUseTexture){
			if (bUseBlendAdd){
				if (bUseLightSpot){
					return PR_TEX_ADD_LT_SPOT;
				}else if (bUseLightLinear){
					return PR_TEX_ADD_LT_LINEAR;
				}else if (bUseShadow){
					return PR_TEX_ADD_SHADOW;
				}
				return PR_TEX_ADD;
			}else if (bUseBlendMultiply){
				if (bUseLightSpot){
					return PR_TEX_MLT_LT_SPOT;
				}else if (bUseLightLinear){
					return PR_TEX_MLT_LT_LINEAR;
				}else if (bUseShadow){
					return PR_TEX_MLT_SHADOW;
				}
				return PR_TEX_MLT;
			}
		}

		//return default color shader
		return PR_COLOR;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SVIPROGRAMHANDLER
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SVIProgramHandler::SVIProgramHandler(SVIInt programID){
		link(programID);
	}

	SVIProgramHandler::SVIProgramHandler(){
		for (SVIInt n = 0; n < HD_COUNT; n++) mHandles[n] = SVI_NO_HANDLE;
		mProgramID = SVI_NO_HANDLE;
	}

	SVIProgramHandler::~SVIProgramHandler(){

	}

	void SVIProgramHandler::link(SVIInt programID){
		if (programID == 0) {
			LOGE("Error: SVIProgramHandler::link - Invalid Program ID");
			return;
		}
		mProgramID = programID;

		for (SVIInt n = 0; n < HD_COUNT; n++){
			if (n == HD_A_POSITION || n == HD_A_HEIGHTS || n == HD_A_TEXCOORD ||
				n == HD_A_NORMAL || n == HD_A_AACOORD || n == HD_A_UNITCOORD){
				
				mHandles[n] = glGetAttribLocation(mProgramID,SVIProgramHandleSymbols[n]);
			}else{
				mHandles[n] = glGetUniformLocation(mProgramID,SVIProgramHandleSymbols[n]);
			}
		}
	}

	SVIInt SVIProgramHandler::getHandle(SVIProgramHandleType type){
		return mHandles[type];
	}

	SVIBool 	SVIProgramHandler::getHandleLinked(SVIProgramHandleType type){
		return (mHandles[type] == SVI_NO_HANDLE) ? SVIFALSE : SVITRUE;
	}

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIMatrix& pValue){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniformMatrix4fv(mHandles[type], 1, GL_FALSE, pValue);
	}

    void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIInt count, SVIVector4* pValue){
        SVI_CHECK_HANDLE(mHandles[type]);
        glUniform4fv(mHandles[type], count, (const GLfloat*)pValue);
    }

    void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIInt count, SVIFloat* pValue){
        SVI_CHECK_HANDLE(mHandles[type]);
        glUniform1fv(mHandles[type], count, (const GLfloat*)pValue);
    }

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIVector4& pValue){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform4f(mHandles[type], pValue.x, pValue.y, pValue.z, pValue.w);
	}

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIVector3& pValue){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform3f(mHandles[type], pValue.x, pValue.y, pValue.z);
	}

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, float x, float y){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform2f(mHandles[type], x, y);
	}
	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, float x, float y, float z){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform3f(mHandles[type], x, y, z);
	}
	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, float x, float y, float z, float w){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform4f(mHandles[type], x, y, z, w);
	}

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIVector2& pValue){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform2f(mHandles[type], pValue.x, pValue.y);
	}

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIColor& refValue){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform4f(mHandles[type], refValue.mR, refValue.mG, refValue.mB, refValue.mA);
	}

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIFloat pValue){
		SVI_CHECK_HANDLE(mHandles[type]);
		glUniform1f(mHandles[type], pValue);
	}

	void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIBool pValue){
		SVI_CHECK_HANDLE(mHandles[type]);
		SVIInt boolValue = (SVIInt)pValue;
		glUniform1i(mHandles[type], boolValue);
	}

    void 	SVIProgramHandler::setUniform(SVIProgramHandleType type, SVIInt value){
        SVI_CHECK_HANDLE(mHandles[type]);
        glUniform1i(mHandles[type], value);
    }

	void 	SVIProgramHandler::setSampler(SVIProgramHandleType type, SVIUInt textureID){
		//2011-09-28 masterkeaton27@gmail.com
		//following set samplers codes can reduce confusion of using multitexture.
		//but, may cause some unexpected problem. so i can't convinced with this.
		//so seriously, need to check soon or later.
		SVI_CHECK_HANDLE(mHandles[type]);
		SVIUInt texLevel = GL_TEXTURE0;
		SVIInt texIndex = 0;
		if ( type == HD_U_SVIMPLER_1 ){
			texLevel = GL_TEXTURE1;
			texIndex = 1;
		}else if ( type == HD_U_SVIMPLER_2 ){
			texLevel = GL_TEXTURE2;
			texIndex = 2;
		}
		glActiveTexture ( texLevel );
		glBindTexture( GL_TEXTURE_2D, textureID);
		glUniform1i( mHandles[type], texIndex );
	}

	void	SVIProgramHandler::setAttribute(SVIProgramHandleType type, SVIInt primCount, SVIFloat* pValues ,SVIUInt stride){
		SVI_CHECK_HANDLE(mHandles[type]);
		glEnableVertexAttribArray( mHandles[type] );
		glVertexAttribPointer( mHandles[type], primCount, GL_FLOAT, GL_FALSE, stride, pValues);
	}

	void 	SVIProgramHandler::setAttribute(SVIProgramHandleType type, SVIVector4* pValue ,SVIUInt stride){
		SVI_CHECK_HANDLE(mHandles[type]);
		glEnableVertexAttribArray( mHandles[type] );
        glVertexAttribPointer( mHandles[type], 4, GL_FLOAT, GL_FALSE, stride, pValue);
    }
    void 	SVIProgramHandler::setAttribute(SVIProgramHandleType type, SVIVector3* pValue ,SVIUInt stride){
        SVI_CHECK_HANDLE(mHandles[type]);
        glEnableVertexAttribArray( mHandles[type] );
        glVertexAttribPointer( mHandles[type], 3, GL_FLOAT, GL_FALSE, stride, pValue);
    }
	void 	SVIProgramHandler::setAttribute(SVIProgramHandleType type, SVIVector2* pValue ,SVIUInt stride){
		SVI_CHECK_HANDLE(mHandles[type]);
		glEnableVertexAttribArray( mHandles[type] );
		glVertexAttribPointer( mHandles[type], 2, GL_FLOAT, GL_FALSE, stride, pValue);
	}

	void	SVIProgramHandler::releaseAttribute(SVIProgramHandleType type){
		SVI_CHECK_HANDLE(mHandles[type]);
		glDisableVertexAttribArray( mHandles[type] );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SVIPROGRAM
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SVIProgramHandler * 	SVIProgram::getHandler(){
		return mHandler;
	}

	SVIInt SVIProgram::getID(){
		return mID;
	}

	SVIInt	SVIProgram::getType(){
		return mType;
	}

	SVIBool	SVIProgram::getActivated(){
		return (mID == SVI_NO_HANDLE) ? SVIFALSE : SVITRUE;
	}

    SVIBool SVIProgram::getUseCPUTexture(){
        return ( mType == PR_TEX_CPU ) ? SVITRUE : SVIFALSE;
    }

	SVIBool SVIProgram::getUseTexture(){
		return ( mType != PR_COLOR &&
			mType != PR_COLOR_LT_SPOT &&
			mType != PR_COLOR_LT_LINEAR &&
			mType != PR_COLOR_SHADOW) ? SVITRUE : SVIFALSE;
	}

	SVIBool SVIProgram::getUseShadow(){
		return (mType == PR_COLOR_SHADOW ||
			mType == PR_TEX_ADD_SHADOW ||
			mType == PR_TEX_MLT_SHADOW ||
			mType == PR_TEX_ADD_SHADOW_FIT ||
			mType == PR_TEX_MLT_SHADOW_FIT ||
			mType == PR_3DSHADOW) ? SVITRUE : SVIFALSE;
	}

	SVIBool SVIProgram::getUseNormal(){
		return (mType == PR_3DMODEL || mType == PR_PHYSIC3DMODEL || mType == PR_PHYSIC3DMODELAA || mType == PR_3DSHADOW || mType == PR_3DMODEL_OFFSET) ? SVITRUE : SVIFALSE;
	}

    SVIBool SVIProgram::getTesselation()
    {
        return mType == PR_TESSELATION;
    }

    //2013-01-30 masterkeaton27@gmail.com
    SVIBool SVIProgram::getUseHeights(){
        return (mType == PR_POST_RIPPLE) ? SVITRUE : SVIFALSE;
    }

	SVIBool SVIProgram::getUse2DNormalMap(){
		return (mType == PR_NORMALMAP || mType == PR_SPECULAR_NORMALMAP) ? SVITRUE : SVIFALSE;
	}

	void SVIProgram::use(){
		glUseProgram(mID);
	}

	void SVIProgram::create(){

	}

	void SVIProgram::create(SVIInt vertexShaderId){
		mVertexShaderID = vertexShaderId;
	}


	void SVIProgram::restore(){

	}

	void SVIProgram::destroy(){
		if (mVertexShaderID == mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL ||
			mVertexShaderID == mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE||
            mVertexShaderID == mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION ||
            mVertexShaderID == mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_COLOR||
			mVertexShaderID == mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW||
			mVertexShaderID == mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_COLOR_SHADOW ||
			mVertexShaderID == mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_3D_SHADOW){
			mVertexShaderID = SVI_NO_HANDLE;
		}else if (mVertexShaderID != SVI_NO_HANDLE){
			glDeleteShader(SVI_NO_HANDLE);
		}

		if (mID != SVI_NO_HANDLE){
			glDeleteProgram(mID);
			mID = SVI_NO_HANDLE;
        }
	}

	SVIProgram::SVIProgram(SVIGLSurface *surface){
		if (mHandler != NULL)
			SVI_SVIFE_DELETE(mHandler);
		
		mHandler = new SVIProgramHandler();
		mType = mFragmentShaderID = mVertexShaderID = mID = SVI_NO_HANDLE;
		mActivated = SVIFALSE;
        
        mRAMBinary = SVIFALSE;

        mProgramBinary = NULL;
        mProgramBinaryLength = 0;
	mProgramBinaryFormat = 0;

		mSVIGLSurface = surface;
	}

	SVIProgram::SVIProgram(SVIGLSurface *surface, SVIProgramType type, SVIBool isAutoLoad){
		mType = mFragmentShaderID = mVertexShaderID = mID = SVI_NO_HANDLE;
		mActivated = SVIFALSE;
		
		mSVIGLSurface = surface;
		
        mRAMBinary = SVIFALSE;

        mType = type;
	mHandler = NULL;

        mProgramBinary = NULL;
        mProgramBinaryLength = 0;
	mProgramBinaryFormat = 0;
		if (isAutoLoad){
			//LOGI("SVIProgram Auto Loaded %d",type);
			mID = createPresetShader();
			mHandler = new SVIProgramHandler(mID);
		}
	}

	SVIProgram::~SVIProgram(){
        if (!mActivated) return;

		destroy();
		mActivated = false;
		SVI_SVIFE_DELETE(mHandler);

#ifdef USE_BINARY_PROGRAM
        if (mProgramBinary != NULL){
            if (!mRAMBinary) free(mProgramBinary);
            mProgramBinary = NULL;
        }
        mProgramBinaryLength = 0;
#endif
		mSVIGLSurface = NULL;
	}


#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH GL_INFO_LOG_length
#endif

	SVIInt SVIProgramManager::compileShader(SVIInt shaderType, SVIPChar srcShader){
		SVIInt compiled = -1;
		SVIUInt shaderId = 0;
		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, (const char**)&srcShader, NULL);
		glCompileShader(shaderId);
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shaderId, infoLen, NULL, buf);
					LOGE("SVIProgram: Could not compile shader %d:\n%s\n",
						shaderType, buf);
					free(buf);
				}
				glDeleteShader(shaderId);
				shaderId = 0;
				return 0;
			}
		}

		return shaderId;
	}



	SVIInt SVIProgramManager::binaryShader(SVIInt shaderType, unsigned char *srcBinary, SVIUInt binarySize){

		//2011-12-14 masterkeaton27@gmail.com
		//reset glError
		int error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR){
			LOGE("glError was occurred from before process ERROR: %d",error);
		}

		SVIUInt shaderId = 0;
		shaderId = glCreateShader(shaderType);

		SVIInt numFormats = 0;
		glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &numFormats);


		if( 0< numFormats){
			SVIBool compiled = SVIFALSE;

			SVIInt *formats = new SVIInt[numFormats];
			glGetIntegerv(GL_SHADER_BINARY_FORMATS, formats);

			for(int i  = 0 ; i<numFormats ; i++){
				glShaderBinary(1, &shaderId, formats[i], (const void *)srcBinary, binarySize);

				if(glGetError()){
					continue;
				}

				compiled = true;
				break;
			}

			SVI_SVIFE_DELETE_ARRAY(formats);

			if(compiled == SVIFALSE)
				return 0;
		}

		return shaderId;
	}



	SVIInt SVIProgram::linkShader(SVIInt program){
		SVIInt linked = SVI_NO_HANDLE;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (linked == 0) {
			LOGE("SVIProgram: Could not link shader %d",mID);
			return SVI_ERROR;
		}

		return SVI_OK;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// SVI Default Vertex Shaders (texture / color)
	////////////////////////////////////////////////////////////////////////////////////

#if USE_COMPILE
	//2011-11-11 masterkeaton27@gmail.com
	//this is a test shader to find out page turn effect's rendering quality.
	//V/F lighting code based on phong shading.
	static char SVITextureNormalVertexShader[] =
		//viewing matrices
		"uniform mat4		uProjection;\n"
		"uniform mat4		uView;\n"
		"uniform mat4		uNormalMatrix;\n"

		//attributes
		"attribute vec2		aTexcoord;\n"
		"attribute vec4		aPosition;\n"
		"attribute vec3		aNormal;\n"

		"varying vec3		lightDir;\n"
		"varying vec3		eyeVec;\n"
		"varying vec3		eyeSpaceNormal;\n"
		"varying vec2		vTexcoord;\n"

		"attribute vec2		aUnitCoord;\n"
		"varying vec2			vUnitCoord;\n"

		"void main() {\n"
		"	vec4 position = uProjection * uView * aPosition;\n"
		"	eyeSpaceNormal = normalize(vec4(uNormalMatrix * vec4(aNormal, 1.0)).xyz);\n"
        "	lightDir = position.xyz;\n"
		"	eyeVec = normalize(vec3(-1.0,0.0,-1.0));\n"
		"	vTexcoord = aTexcoord;\n"
		"	gl_Position = position;\n"
		"  	vUnitCoord = aUnitCoord;\n"
		"}\n";

	static char Frag_SVI3DModel[] =
		"precision mediump float;\n"
		"varying vec3		lightDir;\n"
		"varying vec3		eyeVec;\n"
		"varying vec3		eyeSpaceNormal;\n"
		"varying vec2		vTexcoord;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uAmbient;\n"
		"uniform vec4		uDiffuse;\n"
		"uniform vec4		uSpecular;\n"
		"uniform float		uShininess;\n"
		"uniform float		uLightPower;\n"

		"vec4 directionalLight(vec3 normal){\n"
		"	return vec4(0.65,0.65,0.65,1.0) * clamp(dot(normal,vec3(0.0,-1.0,-1.0)),0.0,1.0) * uSpecular;\n"
		"}\n"

		"void main()\n"
		"{\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec3 N = eyeSpaceNormal;\n"
		"	vec3 L = lightDir;\n"
		"	vec3 E = eyeVec;\n"
        "	vec3 H = normalize(L + E);\n"
        "	float df = max(0.0, dot(N, L));\n"
        "	float sf = max(0.0, dot(N, H));\n"
        "	sf = pow(sf, uShininess);\n"
        "	vec4 orignalColor = texture2D( uSampler0, scaledCoord );\n"
        "	vec4 color = uAmbient * orignalColor + df * uDiffuse + sf * uSpecular;\n"
        "	vec4 lightedColor = color + directionalLight(eyeSpaceNormal);\n"
        "	gl_FragColor = (orignalColor*(1.0 - uLightPower) + lightedColor * uLightPower) * uColor;\n"
        "}\n";

    static char Frag_SVI3DModelOffset[] = //2012-11-21 masterkeaton27@gmail.comfor J project model
        "precision mediump float;\n"
        "varying vec3		lightDir;\n"
        "varying vec3		eyeVec;\n"
        "varying vec3		eyeSpaceNormal;\n"
        "varying vec2		vTexcoord;\n"
        "uniform sampler2D	uSampler0;\n"
        "uniform vec4		uTexInfo;\n"
        "uniform vec4		uColor;\n"
        "uniform vec4		uAmbient;\n"
        "uniform vec4		uDiffuse;\n"
        "uniform vec4		uSpecular;\n"
        "uniform float		uShininess;\n"
        "uniform float		uLightPower;\n"
        "uniform vec4		uLightDirection;\n"

        "vec4 directionalLight(vec3 normal){\n"
        "	return vec4(0.65,0.65,0.65,1.0) * clamp(dot(normal,vec3(-1.0,0.0,-1.0)),0.0,1.0) * 0.4;\n"
        "}\n"

        "void main()\n"
        "{\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec3 N = eyeSpaceNormal;\n"
        "	vec3 L = uLightDirection.xyz;\n"
        "	vec3 E = eyeVec;\n"
        "	vec3 H = normalize(L + E);\n"
        "	float df = max(0.0, dot(N, L));\n"
        "	float sf = max(0.0, dot(N, H));\n"
        "	sf = pow(sf, uShininess);\n"
        "	vec4 orignalColor = texture2D( uSampler0, scaledCoord );\n"
        "	vec4 color = uAmbient * orignalColor + df * uDiffuse + sf * uSpecular;\n"
        "	vec4 lightedColor = color + directionalLight(eyeSpaceNormal);\n"
        "	gl_FragColor = (orignalColor*(1.0 - uLightPower) + lightedColor * uLightPower) * uColor;\n"
        "}\n";





    static char SVITextureNormalMapVertexShader[] =
		/*
        //viewing matrices
        "uniform mat4		uProjection;\n"
        "uniform mat4		uView;\n"

        "attribute vec4		aPosition;\n"

		"uniform vec2		uSize;\n"
		"uniform vec2		uOffset;\n"

        "varying vec2		vTexcoord;\n"
		"varying mat3		vTBN;\n"


		"void main() {\n"
		"	vec4 position = uProjection * uView * aPosition;\n"

		"   vec4 t = uView * vec4(1.0,0.0,0.0,0.0);\n"
		"   vec4 n = uView * vec4(0.0,0.0,1.0,0.0);\n"

		"	vec3 tangent = normalize(t.xyz);\n"
		"	vec3 normal = normalize(n.xyz);\n"
		"	vec3 binormal = cross(normal, tangent);\n"

		"   vTBN[0] = tangent;\n"
		"   vTBN[1] = binormal;\n"
		"   vTBN[2] = normal;\n"

		"  vec2 Ratio = uOffset * uSize;\n"
		"   vTexcoord = (aPosition.xy + Ratio) / uSize;\n"
        "	gl_Position = position;\n"
        "}\n";
		*/
	// Incoming per-vertex attribute values
		"uniform mat4		uProjection;\n"
		"uniform mat4		uView;\n"

		"attribute vec4		aPosition;\n"

		"uniform vec2		uSize;\n"
		"uniform vec2		uOffset;\n"
		"uniform vec4		uLightDirection;\n"
		"uniform vec4		uViewDirection;\n"

		"varying vec2		vTexcoord;\n"
		"varying vec3		vVaryingNormal;\n"
		"varying vec3		vVaryingLightDir;\n"

		"attribute vec2		aUnitCoord;\n"
		"varying vec2			vUnitCoord;\n"

		"void main()\n"
		"{\n"
		"	vec4 n = uViewDirection;\n"
		// Get surface normal in eye coordinates and pass them through to the fragment shader
		"	vVaryingNormal = n.xyz;\n"

		// Get vector to light source
		"	vVaryingLightDir = uLightDirection.xyz;\n"

		"	vec2 Ratio = uOffset * uSize;\n"
        	"   vTexcoord = (aPosition.xy + Ratio) / uSize;\n"
		"	gl_Position = uProjection * uView * aPosition;\n"
		"  	vUnitCoord = aUnitCoord;\n"
		"}\n";


    static char Frag_SVITex_NormalMap[] =  //2012-11-21 masterkeaton27@gmail.comfor normal map shader
		"precision highp float;\n"
		"varying vec2		vTexcoord;\n"
		"varying vec3		vVaryingNormal;\n"
		"varying vec3		vVaryingLightDir;\n"

		"uniform mat4		uNormalMatrix;\n"
		"uniform sampler2D	uSampler0;\n" //diffuse map texture
		"uniform sampler2D	uSampler1;\n" //normal map texture

		"uniform vec4		uTexInfo;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4       uLightColor;"
        "uniform float		uLightPower;\n"

		"vec4		Ambient = vec4(0.3,0.3,0.3,1.0);\n"
		"vec4		Diffuse = vec4(0.7,0.7,0.7,1.0);\n"
		//"vec4		Specular = vec4(0.8,0.8,0.8,1.0);\n"

		"void main()\n"
		"{\n"
		"	float maxVariance = 10.0;\n" // Mess around with this value to increase/decrease normal perturbation
	    "   float minVariance = maxVariance / 2.0;\n"

		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n;"

        "   vec3 normalbasis = texture2D(uSampler1, scaledCoord).rgb;\n"

        "   vec3 normalmap = (normalbasis * maxVariance - minVariance);\n"

		"   normalmap.xy *= uLightPower;\n"
		"	vec3 eyeSpaceNormal = normalize(vec4(uNormalMatrix * vec4(normalize(normalmap), 1.0)).xyz);\n"
		"	vec3 lightDir = vVaryingLightDir;\n"
		"	vec3 eyeVec = vVaryingNormal;\n"

        "   vec4 texturecolor = texture2D(uSampler0, scaledCoord);\n"
		"	vec3 diffusemap = texturecolor.rgb;\n"

		"	vec3 aNormal = vVaryingNormal + eyeSpaceNormal;\n"

        "	float diffuseIntensity = max(0.55, dot(aNormal, vVaryingLightDir));\n"
        "	vec3 colour = (diffuseIntensity * Diffuse.rgb) * diffusemap + Ambient.rgb * texturecolor.a;\n"

        "	vec4 vFragColour = vec4(colour, texturecolor.a);\n"

        "	vec3 vReflection = normalize(reflect(-aNormal, vVaryingLightDir));\n"
        "	float specularIntensity = max(0.0, dot(aNormal, vReflection));\n"

        "	if (diffuseIntensity > 0.6)\n"
        "	{\n"
        "		float fSpec = pow(specularIntensity, uLightColor.a);\n"
        "		vFragColour.rgb += vec3(fSpec * uLightColor.rgb);\n"
        "	}\n"

        "   float lightRatio = smoothstep(0.95, 1.0, normalbasis.b);\n"
        "   vec3 filterColor = vFragColour.rgb * (1.0 - lightRatio) + diffusemap * lightRatio;\n"
        "	gl_FragColor = vec4(filterColor,vFragColour.a) * uColor;\n"//vec4(vFragColour.rgb * uColor.rgb, 0.0);\n"
		"}\n";

    static char Frag_SVITex_SpecularNormalMap[] =  //2012-11-21 masterkeaton27@gmail.comfor normal map shader
        "precision highp float;\n"
        "varying vec2		vTexcoord;\n"
        "varying vec3		vVaryingNormal;\n"
        "varying vec3		vVaryingLightDir;\n"

        "uniform mat4		uNormalMatrix;\n"
        "uniform sampler2D	uSampler0;\n" //diffuse map texture
        "uniform sampler2D	uSampler1;\n" //normal map texture
        "uniform sampler2D	uSampler2;\n" //specular map texture

        "uniform vec4		uTexInfo;\n"
        "uniform vec4		uColor;\n"
        "uniform float		uLightPower;\n"
        "uniform vec4       uLightColor;"

        "vec4		Ambient = vec4(0.1,0.1,0.1,1.0);\n"
        "vec4		Diffuse = vec4(0.7,0.7,0.7,1.0);\n"
        //"vec4		Specular = vec4(0.8,0.8,0.8,1.0);\n"

        "void main()\n"
        "{\n"
        "	float maxVariance = 5.0;\n" // Mess around with this value to increase/decrease normal perturbation
        "   float minVariance = maxVariance / 2.0;\n"

        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n;"

        "   vec4 texturecolor = texture2D(uSampler0, scaledCoord);\n"
        "   vec3 normalbasis = texture2D(uSampler1, scaledCoord).rgb;\n"
        "   vec3 specularbasis = texture2D(uSampler2, scaledCoord).rgb;\n"
        "   vec3 filterColor = texturecolor.rgb;\n"
        //"   if (specularbasis.r > 0.01)\n"
        //"	{\n"
        "   vec3 normalmap = (normalbasis * maxVariance - minVariance);\n"
        "   normalmap.xy *= uLightPower;\n"
        "	vec3 eyeSpaceNormal = normalize(vec4(uNormalMatrix * vec4(normalize(normalmap), 1.0)).xyz);\n"
        "	vec3 lightDir = vVaryingLightDir;\n"
        "	vec3 eyeVec = vVaryingNormal;\n"

        "	vec3 diffusemap = texturecolor.rgb;\n"

        "	vec3 aNormal = vVaryingNormal + eyeSpaceNormal;\n"

        "	float diffuseIntensity = max(uLightColor.g, dot(aNormal, vVaryingLightDir));\n"
        "	vec3 colour = diffuseIntensity * diffusemap + Ambient.rgb * texturecolor.a;\n"

        "	vec4 vFragColour = vec4(colour, texturecolor.a);\n"

        "	vec3 vReflection = normalize(reflect(-aNormal, vVaryingLightDir));\n"
        "	float specularIntensity = max(0.0, dot(aNormal, vReflection));\n"

        "	if (diffuseIntensity > uLightColor.b)\n"
        "	{\n"
        "		float fSpec = pow(specularIntensity, uLightColor.a);\n"
        "		vFragColour.rgb += vec3(fSpec * uLightColor.r);\n"
        "	}\n"
        "   filterColor = vFragColour.rgb * specularbasis.r + diffusemap * (1.0 - specularbasis.r);\n"
        "	gl_FragColor = vec4(filterColor,texturecolor.a) * uColor;\n"//vec4(vFragColour.rgb * uColor.rgb, 0.0);\n"
        "}\n";


	static char Frag_SVIAAPhysics3DModel[] = //2012-02-16 masterkeaton27@gmail.com: shader from SURC
		"precision mediump float;\n"
		"varying vec3		lightDir;\n"
		"varying vec3		eyeVec;\n"
		"varying vec3		eyeSpaceNormal;\n"
		"varying vec2		vTexcoord;\n"
		"uniform sampler2D	uSampler0;\n"
        "uniform vec4		uTexInfo;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uAmbient;\n"
		"uniform vec4		uDiffuse;\n"
		"uniform vec4		uSpecular;\n"
		"uniform float		uShininess;\n"

		"void main()\n"
		"{\n"
		"	vec3 norm = eyeSpaceNormal;\n"
		"	norm.z = abs(eyeSpaceNormal.z);\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"   vec4 texcolor = texture2D(uSampler0, scaledCoord);\n"
		"   if (uShininess <= 0.4) texcolor = max(texcolor, 0.5);\n"
		"	texcolor = (max(dot(norm, vec3(0.0, 0.0, 1.0)), uShininess) * texcolor);\n"
		"	float lineAlpha = 1.0;\n"

		"	if (vTexcoord.x < 0.002) lineAlpha = vTexcoord.x / 0.002 * lineAlpha; \n"
		"	if (vTexcoord.y < 0.002) lineAlpha = vTexcoord.y / 0.002 * lineAlpha; \n"
		"	if (vTexcoord.x > 0.998) lineAlpha = (1.0 - ((vTexcoord.x - 0.998) / 0.002)) * lineAlpha; \n"
		"	if (vTexcoord.y > 0.998) lineAlpha = (1.0 - ((vTexcoord.y - 0.998) / 0.002)) * lineAlpha; \n"

		"	gl_FragColor = vec4(texcolor.rgb, lineAlpha);\n"
		"}\n";

	static char Frag_SVIPhysics3DModel[] = //2012-02-16 masterkeaton27@gmail.com: shader from SURC
		"precision mediump float;\n"
		"varying vec3		lightDir;\n"
		"varying vec3		eyeVec;\n"
		"varying vec3		eyeSpaceNormal;\n"
		"varying vec2		vTexcoord;\n"
		"uniform sampler2D	uSampler0;\n"
        "uniform vec4		uTexInfo;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uAmbient;\n"
		"uniform vec4		uDiffuse;\n"
		"uniform vec4		uSpecular;\n"
		"uniform float		uShininess;\n"

		"void main()\n"
		"{\n"
		"	vec3 norm = eyeSpaceNormal;\n"
		"	norm.z = abs(eyeSpaceNormal.z);\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"   vec4 texcolor = texture2D(uSampler0, scaledCoord);\n"
		"   if (uShininess <= 0.4) texcolor = max(texcolor, 0.5);\n"
		"	texcolor = (max(dot(norm, vec3(0.0, 0.0, 1.0)), uShininess) * texcolor);\n"
		"	float lineAlpha = 1.0;\n"
		"	gl_FragColor = vec4(texcolor.rgb, lineAlpha);\n"
		//"	gl_FragColor = vec4(0.5,0.5,0.5, lineAlpha);\n"
		"}\n";



	static char SVITextureVertexShader[] =
		"uniform mat4		uProjection;\n"
		"uniform mat4		uView;\n"
		"attribute vec4		aPosition;\n"
#if SVI_USING_CPU_TEXCOORD
		"attribute vec2		aTexcoord;\n"
#else
		"uniform vec2		uSize;\n"
		"uniform vec2		uOffset;\n"
#endif
		"varying vec2		vTexcoord;\n"

		"attribute float	aAACoord; \n"
		"varying float		vAACoord; \n"

		"attribute vec2		aUnitCoord;\n"
		"varying vec2			vUnitCoord;\n"

		"void main() {\n"
		"  gl_Position = uProjection * uView * aPosition;\n"
#if SVI_USING_CPU_TEXCOORD
		"  vTexcoord = aTexcoord;\n"
#else
		"  vec2 vRatio = uOffset * uSize;\n"
		"  vTexcoord = (aPosition.xy + vRatio) / uSize;\n"
#endif
		"  vAACoord = aAACoord;\n"
		"  vUnitCoord = aUnitCoord;\n"
		"}\n";

    //2013-06-24 masterkeaton27@gmail.com
    static char SVICPUTextureVertexShader[] =
        "uniform mat4		uProjection;\n"
        "uniform mat4		uView;\n"
        "attribute vec4		aPosition;\n"
        "attribute vec2		aTexcoord;\n"
        "varying vec2		vTexcoord;\n"

	"attribute vec2		aUnitCoord;\n"
	"varying vec2			vUnitCoord;\n"
		
        "void main() {\n"
        "  vTexcoord = aTexcoord;\n"
        "  gl_Position = uProjection * uView * aPosition;\n"        
        "  vUnitCoord = aUnitCoord;\n"
        "}\n";


    //2013-06-24 masterkeaton27@gmail.com
    static char Frag_SVIFilterSketch[] = 
                
    /*
    "precision mediump float;\n"
    "varying vec2       vTexcoord;\n"
    "uniform sampler2D  uSampler0;\n"
    "uniform vec4       uColor;\n"
    "uniform vec4		uTexInfo;\n"
    "uniform vec4		uReserve01;\n"
    "uniform sampler2D tex0;\n"

    "const vec4 luminance_vector = vec4(0.3, 0.59, 0.11, 0.0);\n"

    "void main() {\n"
    "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
    "   vec2 uv = vec2(1.0) - (scaledCoord.xy / uReserve01.xy);\n"
    "   vec2 n = 1.0/uReserve01.xy;\n"
    "   vec4 CC = texture2D(uSampler0, uv);\n"
    "    vec4 RD = texture2D(uSampler0, uv + vec2( n.x, -n.y));\n"
    "    vec4 RC = texture2D(uSampler0, uv + vec2( n.x,  0.0));\n"
    "    vec4 RU = texture2D(uSampler0, uv + n);\n"
    "    vec4 LD = texture2D(uSampler0, uv - n);\n"
    "    vec4 LC = texture2D(uSampler0, uv - vec2( n.x,  0.0));\n"
    "    vec4 LU = texture2D(uSampler0, uv - vec2( n.x, -n.y));\n"
    "    vec4 CD = texture2D(uSampler0, uv - vec2( 0.0,  n.y));\n"
    "    vec4 CU = texture2D(uSampler0, uv + vec2( 0.0,  n.y));\n"

    "    gl_FragColor = vec4(2.0*abs(length(\n"
    "        vec2(\n"
    "        -abs(dot(luminance_vector, RD - LD))\n"
    "        +4.0*abs(dot(luminance_vector, RC - LC))\n"
    "        -abs(dot(luminance_vector, RU - LU)),\n"
    "        -abs(dot(luminance_vector, LD - LU))\n"
    "        +4.0*abs(dot(luminance_vector, CD - CU))\n"
    "        -abs(dot(luminance_vector, RD - RU))\n"
    "        )\n"
    "        )-0.5));\n"
    "}\n";
    */


    
    "precision mediump float;\n"
    
    "uniform sampler2D  uSampler0;\n"
    "uniform vec4       uColor;\n"
    "uniform vec4		uTexInfo;\n"
    "uniform vec3		uReserve01;\n"
    
    "varying vec2       vTexcoord;\n"

    "const vec3 W = vec3(0.2125, 0.7154, 0.0721);\n"
    "void main()\n"
    "{\n"        
    "   float imageWidthFactor = uReserve01.x; \n"
    "   float imageHeightFactor = uReserve01.y; \n"
    "   float intensity = uReserve01.z;\n"

    "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
    "   vec3 textureColor = texture2D(uSampler0, scaledCoord).rgb;\n"

    "    vec2 stp0 = vec2(1.0 / imageWidthFactor, 0.0);\n"
    "    vec2 st0p = vec2(0.0, 1.0 / imageHeightFactor);\n"
    "    vec2 stpp = vec2(1.0 / imageWidthFactor, 1.0 / imageHeightFactor);\n"
    "    vec2 stpm = vec2(1.0 / imageWidthFactor, -1.0 / imageHeightFactor);\n"

    "    float i00   = dot( textureColor, W);\n"
    "    float im1m1 = dot( texture2D(uSampler0, scaledCoord - stpp).rgb, W);\n"
    "    float ip1p1 = dot( texture2D(uSampler0, scaledCoord + stpp).rgb, W);\n"
    "    float im1p1 = dot( texture2D(uSampler0, scaledCoord - stpm).rgb, W);\n"
    "    float ip1m1 = dot( texture2D(uSampler0, scaledCoord + stpm).rgb, W);\n"
    "    float im10 = dot( texture2D(uSampler0, scaledCoord - stp0).rgb, W);\n"
    "    float ip10 = dot( texture2D(uSampler0, scaledCoord + stp0).rgb, W);\n"
    "    float i0m1 = dot( texture2D(uSampler0, scaledCoord - st0p).rgb, W);\n"
    "    float i0p1 = dot( texture2D(uSampler0, scaledCoord + st0p).rgb, W);\n"
    "    float h = -im1p1 - 2.0 * i0p1 - ip1p1 + im1m1 + 2.0 * i0m1 + ip1m1;\n"
    "    float v = -im1m1 - 2.0 * im10 - im1p1 + ip1m1 + 2.0 * ip10 + ip1p1;\n"

    "    float mag = 1.0 - length(vec2(h, v));\n"
    "    vec3 target = vec3(mag - 0.05);\n"
    "    gl_FragColor = vec4(mix(textureColor, target, intensity), 1.0) * uColor;\n"
    "}\n";


    
    static char Frag_SVIFilterOilpaint[] = 
    "precision mediump float;\n"
    "varying vec2       vTexcoord;\n"
    "uniform sampler2D uSampler0;\n"
    "uniform vec4       uColor;\n"
    "uniform vec4		uTexInfo;\n"
    "uniform vec3		uReserve01;\n"

    "void main()\n" 
    "{\n"
    "	 vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
    "    vec2 uv = scaledCoord;\n"
    "    int radius = int(uReserve01.z);\n"
    "    float n = float((radius + 1) * (radius + 1));\n"    
    "    vec2 src_size = uReserve01.xy;\n"

    "    vec3 m[4];\n"
    "    vec3 s[4];\n"
    "    for (int k = 0; k < 4; ++k) {\n"
    "        m[k] = vec3(0.0);\n"
    "        s[k] = vec3(0.0);\n"
    "    }\n"

    "    for (int j = -radius; j <= 0; ++j)  {\n"
    "        for (int i = -radius; i <= 0; ++i)  {\n"
    "            vec3 c = texture2D(uSampler0, uv + vec2(i,j) / src_size).rgb;\n"
    "            m[0] += c;\n"
    "            s[0] += c * c;\n"
    "        }\n"
    "    }\n"

    "    for (int j = -radius; j <= 0; ++j)  {\n"
    "        for (int i = 0; i <= radius; ++i)  {\n"
    "            vec3 c = texture2D(uSampler0, uv + vec2(i,j) / src_size).rgb;\n"
    "            m[1] += c;\n"
    "            s[1] += c * c;\n"
    "        }\n"
    "    }\n"

    "    for (int j = 0; j <= radius; ++j)  {\n"
    "        for (int i = 0; i <= radius; ++i)  {\n"
    "            vec3 c = texture2D(uSampler0, uv + vec2(i,j) / src_size).rgb;\n"
    "            m[2] += c;\n"
    "            s[2] += c * c;\n"
    "        }\n"
    "    }\n"

    "    for (int j = 0; j <= radius; ++j)  {\n"
    "        for (int i = -radius; i <= 0; ++i)  {\n"
    "            vec3 c = texture2D(uSampler0, uv + vec2(i,j) / src_size).rgb;\n"
    "            m[3] += c;\n"
    "            s[3] += c * c;\n"
    "        }\n"
    "    }\n"


    "    float min_sigma2 = 1e+2;\n"
    "    for (int k = 0; k < 4; ++k) {\n"
    "        m[k] /= n;\n"
    "        s[k] = abs(s[k] / n - m[k] * m[k]);\n"

    "        float sigma2 = s[k].r + s[k].g + s[k].b;\n"
    "        if (sigma2 < min_sigma2) {\n"
    "            min_sigma2 = sigma2;\n"
    "            gl_FragColor = vec4(m[k], 1.0);\n"
    "        }\n"
    "    }\n"
    "}\n";


    static char Frag_SVIFilterWaterColor[] = 
        
    "precision mediump float;\n"
    "varying vec2       vTexcoord;\n"
    "uniform sampler2D uSampler0;\n"
    "uniform vec4       uColor;\n"
    "uniform vec4		uTexInfo;\n"
    "uniform vec3		uReserve01;\n"
    
    // Change these 2 defines to change precision,
    "#define vec vec3\n"
    "#define toVec(x) x.rgb\n"

    "#define s2(a, b)				temp = a; a = min(a, b); b = max(temp, b);\n"
    "#define mn3(a, b, c)			s2(a, b); s2(a, c);\n"
    "#define mx3(a, b, c)			s2(b, c); s2(a, c);\n"

    "#define mnmx3(a, b, c)			mx3(a, b, c); s2(a, b);\n"                                   // 3 exchanges
    "#define mnmx4(a, b, c, d)		s2(a, b); s2(c, d); s2(a, c); s2(b, d);\n"                   // 4 exchanges
    "#define mnmx5(a, b, c, d, e)	s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);\n"           // 6 exchanges
    "#define mnmx6(a, b, c, d, e, f) s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f);\n" // 7 exchanges

    "void main() {\n"
    "   vec v[9];\n"
    "   vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        // Add the pixels which make up our window to the pixel array.
    "   for(int dX = -1; dX <= 1; ++dX) {\n"
    "       for(int dY = -1; dY <= 1; ++dY) {\n"		
    "           vec2 offset = vec2(float(dX), float(dY));\n"

                // If a pixel in the window is located at (x+dX, y+dY), put it at index (dX + R)(2R + 1) + (dY + R) of the
                // pixel array. This will fill the pixel array, with the top left pixel of the window at pixel[0] and the
                // bottom right pixel of the window at pixel[N-1].
    "           v[(dX + 1) * 3 + (dY + 1)] = toVec(texture2D(uSampler0, scaledCoord  + offset * uReserve01.xy));\n"
    "       }\n"
    "   }\n"

    "   vec temp;\n"

    // Starting with a subset of size 6, remove the min and max each time
    "   mnmx6(v[0], v[1], v[2], v[3], v[4], v[5]);\n"
    "   mnmx5(v[1], v[2], v[3], v[4], v[6]);\n"
    "   mnmx4(v[2], v[3], v[4], v[7]);\n"
    "   mnmx3(v[3], v[4], v[8]);\n"
    "   toVec(gl_FragColor) = v[4];\n"
    "}\n";
      
    static char Frag_SVIFilterCartoon[] = "";

    static char Frag_SVIFilterRadial[] = 
    "precision mediump float;\n"
    "varying vec2		vTexcoord;\n"
    "uniform vec4		uColor;\n"
    "uniform sampler2D	uSampler0;\n"
    "uniform vec4		uTexInfo;\n"

    "uniform vec3		uReserve01;\n"
    "uniform vec4		uReserve02;\n"

    "varying float vAACoord;\n"
    "void main() {\n"
    "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
    "	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"

    "   float		edgeRadius = uReserve01.x;\n"
    "   float		lerpWidth = uReserve01.y;\n"
    "   float		lerpBase = uReserve01.z;\n"

    "   float       centerx = uReserve02.z;\n"
    "   float       centery = uReserve02.w;\n"

    "	float distance = max(0.001,length(scaledCoord*uReserve02.xy - vec2(centerx, centery)));\n"

    "	float salpha = abs(lerpBase - smoothstep(edgeRadius,edgeRadius+lerpWidth,distance));\n"
    "	gl_FragColor = vec4((texColor.rgb * uColor.rgb) * uColor.a*salpha,texColor.a*uColor.a*salpha);\n"
    "}\n";

    static char Frag_SVIFilterMonotone[] = 
        "precision mediump float;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "uniform vec4		uTexInfo;\n"

        "uniform vec3		uReserve01;\n"
        "uniform vec4		uReserve02;\n"

        "varying float vAACoord;\n"
        "void main() {\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"

        "   float		edgeRadius = uReserve01.x;\n"
        "   float		lerpWidth = uReserve01.y;\n"
        "   float		lerpBase = uReserve01.z;\n"

        "   float       centerx = uReserve02.z;\n"
        "   float       centery = uReserve02.w;\n"

        "	float distance = max(0.001,length(scaledCoord*uReserve02.xy - vec2(centerx, centery)));\n"
        "   float grayColor = (texColor.r + texColor.g + texColor.b) * 0.333333;\n"
        "	float salpha = abs(lerpBase - smoothstep(edgeRadius,edgeRadius+lerpWidth,distance));\n"
        "	gl_FragColor = vec4( ((texColor.rgb * uColor.rgb) * salpha + vec3(grayColor) * (1.0 - salpha)) * uColor.a, texColor.a*uColor.a);\n"
        "}\n";


 static char Frag_SVIFilterSmoke[] = 
       "precision mediump float;\n"
	
	"uniform sampler2D	uSampler0;\n"
	"uniform sampler2D	uSampler1;\n"
	"uniform sampler2D	uSampler2;\n"
	"uniform vec4       uColor;\n"
	"uniform vec3		uReserve01;\n"
	
	"varying vec2		vTexcoord;\n"
	
	"void main()\n"
	"{\n"		 
	"	float shift = uReserve01.x;\n"
	"	float intensity = uReserve01.y;\n"

 	"	float gradient1 =  texture2D(uSampler2, vec2(vTexcoord.x+shift, vTexcoord.y)).a;\n"
	"	float gradient2 =  texture2D(uSampler2, vec2(1.0-vTexcoord.x+shift, vTexcoord.y)).a;\n"
	"	float gradient3 =  texture2D(uSampler2, vec2(vTexcoord.x, vTexcoord.y+shift)).a;\n"
	"	float gradient4 =  texture2D(uSampler2, vec2(vTexcoord.x, 1.0-vTexcoord.y+shift)).a;\n"

 	"	float fraction = clamp(sqrt(gradient1 * gradient2 * gradient3 * gradient4) * 30.0 * intensity, 0.0, 1.0);"
 	"	vec2 distortion = fraction/2.0 * vec2(gradient1-gradient2, gradient3-gradient4);"

	"	gl_FragColor =  fraction * texture2D(uSampler0, vTexcoord) + (1.0-fraction) * texture2D(uSampler1, vTexcoord + distortion/5.0 ) * (1.0-intensity);"
	"}\n";

    //2013-06-24 masterkeaton27@gmail.com
    static char Frag_SVICPUTex[] =
        "precision mediump float;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "void main() {\n"
        "	vec4 texColor = texture2D( uSampler0, vTexcoord );\n"
        "  gl_FragColor = vec4((texColor.rgb * uColor.rgb) * uColor.a,texColor.a*uColor.a);\n"
        "}\n";

    static char SVITextureTesselationVertexShader[] =
        "uniform mat4		uProjection;\n"
        "uniform mat4		uView;\n"
        "attribute vec4		aPosition;\n"
        "attribute vec2		aTexcoord;\n"
        "varying vec2		vTexcoord;\n"

        "attribute float	aAACoord; \n"
        "varying float		vAACoord; \n"

        "void main() {\n"
        "  gl_Position = uProjection * uView * aPosition;\n"
        "  vTexcoord = aTexcoord;\n"
        "  vAACoord = aAACoord;\n"
        "}\n";

    static char SVIColorVertexShader[] =
        "uniform mat4		uProjection;\n"
        "uniform mat4		uView;\n"
        "attribute vec4		aPosition;\n"

        "attribute float	aAACoord; \n"
        "varying float		vAACoord; \n"

		"attribute vec2		aUnitCoord;\n"
		"varying vec2			vUnitCoord;\n"

        "void main() {\n"
		"  gl_Position = uProjection * uView * aPosition;\n"
		"  vAACoord = aAACoord;\n"
		"  vUnitCoord = aUnitCoord;\n"
		"}\n";

	static char SVITextureShadowVertexShader[] =
		"uniform mat4		uProjection;\n"
		"uniform mat4		uView;\n"
		"uniform vec2		uShadowOffset;\n"
		"uniform vec2		uShadowRadius;\n"
#if SVI_USING_CPU_TEXCOORD
		"attribute vec2		aTexcoord;\n"
#else
		"uniform vec2		uSize;\n"
		"uniform vec2		uOffset;\n"
#endif
		"attribute vec4		aPosition;\n"

		"varying vec2		vTexcoord;\n"

		"attribute float	aAACoord; \n"
		"varying float		vAACoord; \n"

		"attribute vec2		aUnitCoord;\n"
		"varying vec2			vUnitCoord;\n"

		"void main() {\n"
		"	vec4 offset = vec4(uShadowOffset.x,uShadowOffset.y,0.0,0.0);\n"
		"	vec4 scaledPos = vec4(aPosition.x*uShadowRadius.x,aPosition.y*uShadowRadius.y,aPosition.z,1.0);\n"
		"	gl_Position = uProjection * uView * (scaledPos+offset);\n"
#if SVI_USING_CPU_TEXCOORD
		"   vTexcoord = aTexcoord;\n"
#else
		"   vec2 vRatio = uOffset * uSize;\n"
		"   vTexcoord = (aPosition.xy + vRatio) / uSize;\n"
#endif
		"  vAACoord = aAACoord;\n"
		"  vUnitCoord = aUnitCoord;\n"
		"}\n";


	static char SVI3DShadowVertexShader[] =
		"uniform mat4		uProjection;\n"
		"uniform mat4		uView;\n"
		"uniform vec2		uShadowOffset;\n"
		"uniform vec2		uShadowRadius;\n"

		"attribute vec2		aTexcoord;\n"
		"attribute vec4		aPosition;\n"

		"varying vec2		vTexcoord;\n"

		"attribute float	aAACoord; \n"
		"varying float		vAACoord; \n"

		"attribute vec2		aUnitCoord;\n"
		"varying vec2			vUnitCoord;\n"

		"void main() {\n"
		"	vec4 offset = vec4(uShadowOffset.x,uShadowOffset.y,0.0,0.0);\n"
		"	vec4 scaledPos = vec4(aPosition.x*uShadowRadius.x,aPosition.y*uShadowRadius.y,0.0,1.0);\n"
		"	gl_Position = uProjection * uView * (scaledPos+offset);\n"
		"   	vTexcoord = aTexcoord;\n"
		"  	vAACoord = aAACoord;\n"
		"  	vUnitCoord = aUnitCoord;\n"
		"}\n";

	static char SVIColorShadowVertexShader[] =
		"uniform mat4		uProjection;\n"
		"uniform mat4		uView;\n"
		"uniform vec2		uShadowOffset;\n"
		"uniform vec2		uShadowRadius;\n"
		"attribute vec4		aPosition;\n"

		"attribute vec2		aUnitCoord;\n"
		"varying vec2			vUnitCoord;\n"

		"void main() {\n"
		"	vec4 offset = vec4(uShadowOffset.x,uShadowOffset.y,0.0,0.0);\n"
		"	vec4 scaledPos = vec4(aPosition.x*uShadowRadius.x,aPosition.y*uShadowRadius.y,aPosition.z,1.0);\n" //aPosition.z + 50
		"	gl_Position = uProjection * uView * (scaledPos+offset);\n"
		"  	vUnitCoord = aUnitCoord;\n"
		"}\n";



	////////////////////////////////////////////////////////////////////////////////////
	// SVI Default Fragment Shaders (texture / color)
	////////////////////////////////////////////////////////////////////////////////////



	//1. pure-color based shaders
    //2012/12/21 rareboy0112@facebook.com
    //changed bottom code
    //"  gl_FragColor = vec4(uColor.rgb * uColor.a * vAACoord,uColor.a * vAACoord);\n"
static char Frag_SVIColor[] =
		"precision mediump float;\n"
		"uniform vec4		uColor;\n"
		"varying float		vAACoord;\n"
		"void main() {\n"
		"  gl_FragColor = vec4(uColor.rgb * uColor.a * 1.0,uColor.a * 1.0);\n"
		"}\n";

	static char Frag_SVIColor_LT_SPOT[] =
		"precision mediump float;\n"
		"uniform vec4		uColor;\n"
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightRadius;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec2		uScreenSize;\n"
		"varying float		vAACoord;\n"

		"varying vec2		vUnitCoord;\n"
	
		"void main()\n"
		"{\n"
		"	vec4 color = uColor;\n"
		"	float dist = distance( vUnitCoord, uLightOffset.xy);\n"
		"	color.rgb = color.rgb + (uLightColor.rgb * smoothstep(uLightRadius.x, uLightRadius.y, dist)*uLightColor.a) * uLightPower;\n"
		"  gl_FragColor = vec4((color.rgb + uColor.rgb) * uColor.a * vAACoord,color.a * uColor.a * vAACoord);\n"
		"}\n";

	static char Frag_SVIColor_LT_LINEAR[] =// 2
		"precision mediump float;\n"
		"uniform vec4		uColor;\n"
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightTarget;\n"
#ifdef USE_LINEAR_RADIUS
		"uniform vec2		uLightRadius;\n"
#endif
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec2		uScreenSize;\n"
		"varying float		vAACoord;\n"

		"varying vec2		vUnitCoord;\n"

		"void main()\n"
		"{\n"
		"	vec4 color = uColor;\n"
		"	float magnitude = distance(uLightTarget,uLightOffset);\n"
		"	float distance =\n"
		"			(vUnitCoord.x * (uLightTarget.x - uLightOffset.x) - (uLightOffset.x * uLightTarget.x) + (uLightOffset.x * uLightOffset.x) +\n"
		"			vUnitCoord.y * (uLightTarget.y - uLightOffset.y) - (uLightOffset.y * uLightTarget.y) + (uLightOffset.x * uLightOffset.y)) / magnitude;\n"
		"	float ratio = abs((2.0 * distance - distance*distance) / (2.0 * magnitude - magnitude*magnitude));\n"
#ifndef USE_LINEAR_RADIUS
		"	color.rgb = color.rgb + (uLightColor.rgb * (1.0 - min(1.0,ratio)) * uLightColor.a) * uLightPower;\n"
#else
		"	color.rgb = color.rgb + (uLightColor.rgb * smoothstep(uRadius, 0.1, ratio) * uLightColor.a);\n"
#endif	
		"  gl_FragColor = vec4(color.rgb * uColor.a * vAACoord, color.a*uColor.a* vAACoord);\n"
		"}\n";

	static char Frag_SVIColor_SHADOW[] =
		"precision mediump float;\n"
		"uniform vec4		uShadowColor;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(uShadowColor.rgb,uShadowColor.a);\n"
		"}\n";


	//2012-06-21 masterkeaton27@gmail.com
	//add new shader for swipe
	static char Frag_SVITex_Linear_Swipe[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"	
		"uniform float		uEdgeRatio;\n"
		"uniform float		uLerpWidth;\n"
		"uniform float		uLerpBase;\n"
		"varying float vAACoord;\n"
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
		"	float salpha = abs(uLerpBase - smoothstep(uEdgeRatio,uEdgeRatio+uLerpWidth,vTexcoord.x));\n"
		"	gl_FragColor = vec4((texColor.rgb * uColor.rgb) * uColor.a*vAACoord*salpha,texColor.a*uColor.a*vAACoord*salpha);\n"
		"}\n";

	static char Frag_SVITex_Radial_Swipe[] =
        "precision mediump float;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "uniform vec4		uTexInfo;\n"
        "uniform float		uEdgeRadius;\n"
        "uniform float		uLerpWidth;\n"
        "uniform float		uLerpBase;\n"
        "varying float vAACoord;\n"
        "void main() {\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
        "	float distance = max(0.001,length(vTexcoord - vec2(0.5,0.5)));\n"
        "	float salpha = abs(uLerpBase - smoothstep(uEdgeRadius,uEdgeRadius+uLerpWidth,distance));\n"
        "	gl_FragColor = vec4((texColor.rgb * uColor.rgb) * uColor.a*vAACoord*salpha,texColor.a*uColor.a*vAACoord*salpha);\n"
        "}\n";


	//2. texture based shaders
	static char Frag_SVITexAdd[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform float		uOpacity;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"

		"varying float vAACoord;\n"

		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
		"   float finalAlpha = uOpacity * min(1.0 , uColor.a + texColor.a);\n"
		"	gl_FragColor = vec4( ( (texColor.rgb * uOpacity) + (uColor.rgb * ((1.0 - texColor.a) * uColor.a) * uOpacity) ) * vAACoord, finalAlpha * vAACoord );\n"
		"}\n";

	static char Frag_SVITexAdd_LT_SPOT[] =
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uLightColor;\n"
		"uniform vec4		uColor;\n"
		"uniform float		uOpacity;\n"
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightRadius;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float		vAACoord;\n"

		"varying vec2		vUnitCoord;\n"
	
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
		"   float finalAlpha = uOpacity * min(1.0 , uColor.a + texColor.a);\n"
		"	vec4 addColor = vec4((texColor.rgb * uOpacity) + (uColor.rgb * ((1.0 - texColor.a) * uColor.a) * uOpacity),finalAlpha);\n"
		"	float dist = distance(vUnitCoord, uLightOffset);\n"
		"	addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.y, uLightRadius.x, dist)*uLightColor.a ) * uLightPower;\n"
		"	gl_FragColor = vec4( addColor.rgb * vAACoord, addColor.a * vAACoord);\n"
		"}\n";

	static char Frag_SVITexAdd_LT_LINEAR[] =
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform float		uOpacity;\n"
#ifdef USE_LINEAR_RADIUS
		"uniform vec2		uLightRadius;\n"
#endif
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightTarget;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float		vAACoord;\n"

		"varying vec2		vUnitCoord;\n"
		
		"void main()\n"
		"{\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
		"   float finalAlpha = uOpacity * min(1.0 , uColor.a + texColor.a);\n"
		"	vec4 addColor = vec4((texColor.rgb * uOpacity) + (uColor.rgb * ((1.0 - texColor.a) * uColor.a)) * uOpacity,finalAlpha);\n"
		"	float magnitude = distance(uLightTarget, uLightOffset);\n"
		"	float distance =\n"
		"			(vUnitCoord.x * (uLightTarget.x - uLightOffset.x) - (uLightOffset.x * uLightTarget.x) + (uLightOffset.x * uLightOffset.x) +\n"
		"			vUnitCoord.y * (uLightTarget.y - uLightOffset.y) - (uLightOffset.y * uLightTarget.y) + (uLightOffset.x * uLightOffset.y)) / magnitude;\n"
		"	float ratio = abs((2.0 * distance - distance*distance) / (2.0 * magnitude - magnitude*magnitude));\n"
#ifndef USE_LINEAR_RADIUS
		"	addColor.rgb = addColor.rgb + (uLightColor.rgb * (1.0 - min(1.0,ratio)) * uLightColor.a) * uLightPower;\n"
#else
		"	addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.y, uLightRadius.x, ratio) * uLightColor.a);\n"
#endif	
		"	gl_FragColor = vec4( addColor.rgb * vAACoord, addColor.a * vAACoord);\n"
		"}\n";

	static char Frag_SVITexAdd_SHADOW[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uShadowColor;\n"
		"uniform float		uOpacity;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
		"   if (texColor.a < uShadowColor.a) texColor.a = uShadowColor.a;\n"
		"   float finalAlpha = uOpacity * min(texColor.a, uShadowColor.a);\n"
		"	gl_FragColor = vec4(uShadowColor.rgb,finalAlpha);\n"
		"}\n";
        
   

	static char Frag_SVITexMlt[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float vAACoord;\n"
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
		"  gl_FragColor = vec4((texColor.rgb * uColor.rgb) * uColor.a*vAACoord,texColor.a*uColor.a*vAACoord);\n"
		"}\n";

	static char Frag_SVILinearGradation[] =
		"precision lowp float;\n"
		"#define MAX_GRADATION_COLORS 10\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uTexInfo;\n"
		"uniform vec4		uGradationColors[MAX_GRADATION_COLORS];\n"
		"uniform float		uGradationOffsets[MAX_GRADATION_COLORS];\n"
		"uniform vec2		uGradationStartPos;\n"
		"uniform vec2		uGradationEndPos;\n"
		"uniform int		uNumGradationColors;\n"
		"varying float vAACoord;\n"
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec2 gradientVector = uGradationEndPos - uGradationStartPos; \n"
		"	float lengthGradientVector = length(gradientVector); \n"
		"	vec2 UVVector = scaledCoord - uGradationStartPos; \n"
		"	float dist = dot(gradientVector, UVVector)/lengthGradientVector; \n"
		"	float weight = dist/lengthGradientVector; \n"
		"	weight = clamp(weight, 0.0, 1.0); \n"
		"	lowp int index; \n"
		"	float tempWeight; \n"
		"	for (int i=0; i < uNumGradationColors; i++) \n"
		"	{ \n"
		"		tempWeight = weight - uGradationOffsets[i]; \n"
		"		index = i; \n"
		"		if (tempWeight < 0.0) \n"
		"		{ \n"
		"			break; \n"
		"		} \n"
		"	} \n"
		"	weight = (-tempWeight)/(uGradationOffsets[index]-uGradationOffsets[index-1]); \n"
		"	vec4 startColor; \n"
		"	vec4 endColor; \n"
		"	startColor = uGradationColors[index-1]; \n"
		"	endColor = uGradationColors[index]; \n"
		"	vec4 resultColor = startColor*weight+endColor*(1.0-weight);\n"
		"   gl_FragColor = vec4(resultColor.rgb*resultColor.a*vAACoord,resultColor.a*vAACoord);\n"
		"}\n";

	static char Frag_SVIRadialGradation[] =
		"precision lowp float;\n"
		"#define MAX_GRADATION_COLORS 10\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uTexInfo;\n"
		"uniform vec4		uGradationColors[MAX_GRADATION_COLORS];\n"
		"uniform float		uGradationOffsets[MAX_GRADATION_COLORS];\n"
		"uniform vec2		uGradationStartPos;\n"
		"uniform vec2		uGradationEndPos;\n"
		"uniform int		uNumGradationColors;\n"
		"varying float vAACoord;\n"
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec2 gradientVector = uGradationEndPos - uGradationStartPos; \n"
		"	float lengthGradientVector = length(gradientVector); \n"
		"	vec2 UVVector = scaledCoord - uGradationStartPos; \n"
		"	float dist = distance(vTexcoord, uGradationStartPos); \n"
		"	float weight = 1.0 - dist/lengthGradientVector; \n"
		"	weight = clamp(weight, 0.0, 1.0); \n"
		"	lowp int index; \n"
		"	float tempWeight; \n"
		"	for (int i=0; i < uNumGradationColors; i++) \n"
		"	{ \n"
		"		tempWeight = weight - uGradationOffsets[i]; \n"
		"		index = i; \n"
		"		if (tempWeight < 0.0) \n"
		"		{ \n"
		"			break; \n"
		"		} \n"
		"	} \n"
		"	weight = (-tempWeight)/(uGradationOffsets[index]-uGradationOffsets[index-1]); \n"
		"	vec4 startColor; \n"
		"	vec4 endColor; \n"
		"	startColor = uGradationColors[index-1]; \n"
		"	endColor = uGradationColors[index]; \n"
		"	vec4 resultColor = startColor*weight+endColor*(1.0-weight);\n"
		"   gl_FragColor = vec4(resultColor.rgb*resultColor.a*vAACoord,resultColor.a*vAACoord);\n"
		"}\n";

	static char Frag_SVITexMlt_LT_SPOT[] =
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightRadius;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float 		vAACoord;\n"
		
		"varying vec2		vUnitCoord;\n"
		
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 addColor = texture2D( uSampler0, scaledCoord ) * vec4(uColor.rgb, 1.0);\n"
		"	float dist = distance(vUnitCoord, uLightOffset);\n"
		"	addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.x, uLightRadius.y, dist)*uLightColor.a) * uLightPower * addColor.a;\n"
		"	gl_FragColor = vec4(addColor.rgb* uColor.a * vAACoord,addColor.a*vAACoord);\n"
		"}\n";

	static char Frag_SVITexMlt_LT_LINEAR[] =//10
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
#ifdef USE_LINEAR_RADIUS
		"uniform vec2		uLightRadius;\n"
#endif
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightTarget;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float 		vAACoord;\n"

		"varying vec2		vUnitCoord;\n" 

		"void main()\n"
		"{\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
		"	vec4 addColor = texture2D( uSampler0, scaledCoord ) * vec4(uColor.rgb, 1.0);\n"
		"	float magnitude = distance(uLightTarget, uLightOffset);\n"
		"	float distance =\n"
		"			(vUnitCoord.x * (uLightTarget.x - uLightOffset.x) - (uLightOffset.x * uLightTarget.x) + (uLightOffset.x * uLightOffset.x) +\n"
		"			vUnitCoord.y * (uLightTarget.y - uLightOffset.y) - (uLightOffset.y * uLightTarget.y) + (uLightOffset.x * uLightOffset.y)) / magnitude;\n"
		"	float ratio = abs((2.0 * distance - distance*distance) / (2.0 * magnitude - magnitude*magnitude));\n"
#ifndef USE_LINEAR_RADIUS
		"	addColor.rgb = addColor.rgb + (uLightColor.rgb * (1.0 - min(1.0,ratio)) * uLightColor.a) * uLightPower * addColor.a;\n"
#else
		"	addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.y, uLightRadius.x, ratio) * uLightColor.a);\n"
#endif	
		"	gl_FragColor = vec4(addColor.rgb*uColor.a*vAACoord,addColor.a*uColor.a*vAACoord);\n"
		"}\n";


	static char Frag_SVI3DShadow[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uShadowColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"void main() {\n"
		"	vec4 addColor = texture2D( uSampler0, vTexcoord );\n"
		"	float shadowAlpha = addColor.a * uShadowColor.a;\n"

		"	if (vTexcoord.x < 0.03) shadowAlpha = vTexcoord.x / 0.03 * shadowAlpha; \n"
		"	if (vTexcoord.y < 0.03) shadowAlpha = vTexcoord.y / 0.03 * shadowAlpha; \n"
		"	if (vTexcoord.x > 0.97) shadowAlpha = (1.0 - ((vTexcoord.x - 0.97) / 0.03)) * shadowAlpha; \n"
		"	if (vTexcoord.y > 0.97) shadowAlpha = (1.0 - ((vTexcoord.y - 0.97) / 0.03)) * shadowAlpha; \n"

		"	gl_FragColor = vec4(uShadowColor.rgb, shadowAlpha);\n"
		"}\n";

	static char Frag_SVITexMlt_SHADOW[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uShadowColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"void main() {\n"
		"	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"


		"	vec4 addColor = texture2D( uSampler0, scaledCoord );\n"
		"	float shadowAlpha = addColor.a * uShadowColor.a;\n"
		"	gl_FragColor = vec4(uShadowColor.rgb, shadowAlpha);\n"
		"}\n";

	//3. texture scale based shaders
	static char Frag_SVITexAdd_FIT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uFitInfo;\n"
		"uniform float		uOpacity;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"

		"varying float vAACoord;\n"

		"void main() {\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledTex );\n"
		"   float finalAlpha = uOpacity * min(1.0 , uColor.a + texColor.a);\n"
		"	vec3 fitColor =  uColor.rgb * ((1.0 - texColor.a) * uColor.a) * uOpacity;\n"
		"	vec4 addColor = vec4((texColor.rgb * uOpacity) + fitColor,finalAlpha);\n"
		"   if (uFitInfo.x > vTexcoord.x ||\n"
		"		uFitInfo.y > vTexcoord.y ||\n"
		"		uFitInfo.x+uFitInfo.z < vTexcoord.x ||\n"
		"		uFitInfo.y+uFitInfo.w < vTexcoord.y) addColor = vec4(uColor.rgb, uOpacity * uColor.a);\n"
		"	gl_FragColor = vec4(addColor.rgb*vAACoord, addColor.a*vAACoord);\n"
		"}\n";

	static char Frag_SVITexAdd_LT_SPOT_FIT[] =
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform float		uOpacity;\n"
		"uniform vec4		uFitInfo;\n"
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightRadius;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float vAACoord;\n"
		"void main() {\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledTex );\n"
		"   float finalAlpha = uOpacity * min(1.0 , uColor.a + texColor.a);\n"
		"	vec3 fitColor =  uColor.rgb * ((1.0 - texColor.a) * uColor.a) * uOpacity;\n"
		"	vec4 addColor = vec4((texColor.rgb * uOpacity) + fitColor,finalAlpha);\n"
		"	float dist = distance(vTexcoord, uLightOffset);\n"
		"   if (uFitInfo.x > vTexcoord.x ||\n"
		"		uFitInfo.y > vTexcoord.y ||\n"
		"		uFitInfo.x+uFitInfo.z < vTexcoord.x ||\n"
		"		uFitInfo.y+uFitInfo.w < vTexcoord.y) {\n"
		"		addColor.rgb = uColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.x, uLightRadius.y, dist)*uLightColor.a) * uLightPower * addColor.a;\n"
		"	}else{\n"
		"		addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.x, uLightRadius.y, dist)*uLightColor.a) * uLightPower * addColor.a;\n"
		"	}\n"
		"	gl_FragColor = vec4(addColor.rgb*vAACoord, addColor.a*vAACoord);\n"
		"}\n";

	static char Frag_SVITexAdd_LT_LINEAR_FIT[] =
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform float		uOpacity;\n"
		"uniform vec4		uFitInfo;\n"
#ifdef USE_LINEAR_RADIUS
		"uniform vec2		uLightRadius;\n"
#endif
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightTarget;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float vAACoord;\n"
		"void main()\n"
		"{\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledTex );\n"
		"   float finalAlpha = uOpacity * min(1.0 , uColor.a + texColor.a);\n"
		"	vec3 fitColor =  uColor.rgb * ((1.0 - texColor.a) * uColor.a) * uOpacity;\n"
		"	vec4 addColor = vec4((texColor.rgb * uOpacity) + fitColor,finalAlpha);\n"
		"	float magnitude = distance(uLightTarget, uLightOffset);\n"
		"	float distance =\n"
		"			(vTexcoord.x * (uLightTarget.x - uLightOffset.x) - (uLightOffset.x * uLightTarget.x) + (uLightOffset.x * uLightOffset.x) +\n"
		"			vTexcoord.y * (uLightTarget.y - uLightOffset.y) - (uLightOffset.y * uLightTarget.y) + (uLightOffset.x * uLightOffset.y)) / magnitude;\n"
		"	float ratio = abs((2.0 * distance - distance*distance) / (2.0 * magnitude - magnitude*magnitude));\n"
		"   if (uFitInfo.x > vTexcoord.x ||\n"
		"		uFitInfo.y > vTexcoord.y ||\n"
		"		uFitInfo.x+uFitInfo.z < vTexcoord.x ||\n"
		"		uFitInfo.y+uFitInfo.w < vTexcoord.y) {\n"
		"		addColor.rgb = uColor.rgb + (uLightColor.rgb * (1.0 - min(1.0,ratio)) * uLightColor.a) * uLightPower* addColor.a;\n"
		"	}else{\n"
#ifndef USE_LINEAR_RADIUS
		"		addColor.rgb = addColor.rgb + (uLightColor.rgb * (1.0 - min(1.0,ratio)) * uLightColor.a) * uLightPower* addColor.a;\n"
#else
		"		addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.y, uLightRadius.x, ratio) * uLightColor.a);\n"
#endif
		"	}\n"
		"	gl_FragColor = vec4(addColor.rgb*vAACoord, addColor.a*vAACoord);\n"
		"}\n";

	static char Frag_SVITexAdd_SHADOW_FIT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uShadowColor;\n"
		"uniform float		uOpacity;\n"
		"uniform vec4		uFitInfo;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"void main() {\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledTex );\n"
		"   if (texColor.a < uShadowColor.a) texColor.a = uShadowColor.a;\n"
		"   float finalAlpha = uOpacity * min(texColor.a, uShadowColor.a);\n"
		"	gl_FragColor = vec4(uShadowColor.rgb,finalAlpha);\n"
		"}\n";

	static char Frag_SVITexMlt_FIT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uFitInfo;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float vAACoord;\n"
		"void main() {\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	vec4 texColor = texture2D( uSampler0, scaledTex ) * uColor;\n"
		"   if (uFitInfo.x > vTexcoord.x ||\n"
		"		uFitInfo.y > vTexcoord.y ||\n"
		"		uFitInfo.x+uFitInfo.z < vTexcoord.x ||\n"
		"		uFitInfo.y+uFitInfo.w < vTexcoord.y) texColor = vec4(0.0,0.0,0.0,0.0);\n"
		"	gl_FragColor = vec4((texColor.rgb * uColor.rgb) * uColor.a * vAACoord,texColor.a*uColor.a * vAACoord);\n"
		"}\n";

	static char Frag_SVITexMlt_LT_SPOT_FIT[] =
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uFitInfo;\n"
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightRadius;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float vAACoord;\n"
		"void main() {\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	vec4 addColor = texture2D( uSampler0, scaledTex ) * uColor;\n"
		"	float dist = distance(vTexcoord, uLightOffset);\n"
		"   if (uFitInfo.x > vTexcoord.x ||\n"
		"		uFitInfo.y > vTexcoord.y ||\n"
		"		uFitInfo.x+uFitInfo.z < vTexcoord.x ||\n"
		"		uFitInfo.y+uFitInfo.w < vTexcoord.y) {\n"
		"		addColor = vec4(0.0,0.0,0.0,0.0);\n"
		"	}else{\n"
		"		addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.x, uLightRadius.y, dist)*uLightColor.a) * uLightPower* addColor.a;\n"
		"	}\n"
		"	gl_FragColor = vec4((addColor.rgb * uColor.rgb) * uColor.a * vAACoord,addColor.a*uColor.a * vAACoord);\n"
		"}\n";


	static char Frag_SVITexMlt_LT_LINEAR_FIT[] =
		"precision mediump float;\n"
		"uniform sampler2D	uSampler0;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uFitInfo;\n"
#ifdef USE_LINEAR_RADIUS
		"uniform vec2		uLightRadius;\n"
#endif
		"uniform vec2		uLightOffset;\n"
		"uniform vec2		uLightTarget;\n"
		"uniform vec4		uLightColor;\n"
		"uniform float		uLightPower;\n"
		"uniform vec4		uTexInfo;\n"
		"varying float vAACoord;\n"
		"void main()\n"
		"{\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	vec4 addColor = texture2D( uSampler0, scaledTex ) * uColor;\n"
		"	float magnitude = distance(uLightTarget, uLightOffset);\n"
		"	float distance =\n"
		"			(vTexcoord.x * (uLightTarget.x - uLightOffset.x) - (uLightOffset.x * uLightTarget.x) + (uLightOffset.x * uLightOffset.x) +\n"
		"			vTexcoord.y * (uLightTarget.y - uLightOffset.y) - (uLightOffset.y * uLightTarget.y) + (uLightOffset.x * uLightOffset.y)) / magnitude;\n"
		"	float ratio = abs((2.0 * distance - distance*distance) / (2.0 * magnitude - magnitude*magnitude));\n"
		"   if (uFitInfo.x > vTexcoord.x ||\n"
		"		uFitInfo.y > vTexcoord.y ||\n"
		"		uFitInfo.x+uFitInfo.z < vTexcoord.x ||\n"
		"		uFitInfo.y+uFitInfo.w < vTexcoord.y) {\n"
#ifndef USE_LINEAR_RADIUS
		"		addColor = vec4(0.0,0.0,0.0,0.0);\n"
#else
		"		addColor.rgb = uColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.y, uLightRadius.x, ratio) * uLightColor.a);\n"
#endif
		"	}else{\n"
#ifndef USE_LINEAR_RADIUS
		"		addColor.rgb = addColor.rgb + (uLightColor.rgb * (1.0 - min(1.0,ratio)) * uLightColor.a) * uLightPower* addColor.a;\n"
#else
		"		addColor.rgb = addColor.rgb + (uLightColor.rgb * smoothstep(uLightRadius.y, uLightRadius.x, ratio) * uLightColor.a);\n"
#endif	
		"	}\n"
		"	gl_FragColor = vec4((addColor.rgb * uColor.rgb) * uColor.a * vAACoord,addColor.a*uColor.a * vAACoord);\n"
		"}\n";

	static char Frag_SVITexMlt_SHADOW_FIT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uShadowColor;\n"
		"uniform vec4		uFitInfo;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec4		uTexInfo;\n"
		"void main() {\n"
		"   vec2 scaledTex = uTexInfo.xy + uTexInfo.zw * ((vTexcoord-uFitInfo.xy)/uFitInfo.zw);\n"
		"	float shadowAlpha = texture2D( uSampler0 , scaledTex ).a * uShadowColor.a;\n"
		"   if (uFitInfo.x > vTexcoord.x ||\n"
		"		uFitInfo.y > vTexcoord.y ||\n"
		"		uFitInfo.x+uFitInfo.z < vTexcoord.x ||\n"
		"		uFitInfo.y+uFitInfo.w < vTexcoord.y) {\n"
		"		shadowAlpha = 0.0;\n"
		"	}\n"
		"	gl_FragColor = vec4(uShadowColor.rgb, shadowAlpha);\n"
		"}\n";


#if SVI_USE_POST_EFFECT
	static char Frag_SVITex_Ripple_RT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec3		uReserve01;\n" // x: frequency , y: phase , z: amplitude
		"uniform vec2		uReserve02;\n" // center
		"void main() {\n"
        "	vec2 dir = vTexcoord-uReserve02;\n"
        "	float dist = length(dir);\n"
        "	vec2 wave = vec2(0.0,0.0);\n"
        "	wave.x = sin(uReserve01.x * dist + uReserve01.y);\n"
        "	wave.y = cos(uReserve01.x * dist + uReserve01.y);\n"
        "	float falloff = clamp(1.0 - dist,0.0,1.0);\n"
        "	falloff *= falloff;\n"
        "	dist += uReserve01.z * wave.x * falloff;\n"
        "	vec2 samplePoint = uReserve02 + dist * dir;\n"
        "	vec4 color = texture2D( uSampler0, samplePoint);\n"
        "	float lighting = 1.0 - uReserve01.z * 0.2 * (1.0 - clamp(wave.y * falloff, 0.0, 1.0));\n"
        "	color.rgb *= lighting;\n"
        "	gl_FragColor = vec4((color.rgb * uColor.rgb) * uColor.a,uColor.a);\n"
		"}\n";

	static char Frag_SVITex_Wizzle_RT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform float		uTime;\n"
		"uniform vec2		uReserve01;\n" // x: scale y : scale
		"void main() {\n"
		"	vec2 texcoord = vTexcoord;\n"
		"	texcoord.x += sin(uTime+texcoord.x*uReserve01.x)*uReserve01.y;\n"
		"	texcoord.y += cos(uTime+texcoord.y*uReserve01.x)*uReserve01.y;\n"
		"	vec4 texColor = texture2D( uSampler0, texcoord);\n"
		"	gl_FragColor = vec4((texColor.rgb * uColor.rgb) * uColor.a,uColor.a);\n"
		"}\n";

	static char Frag_SVITex_Pulse_RT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec3		uReserve01;\n"
		"uniform vec2		uReserve02;\n"
		"uniform float		uTime;\n"
		"void main(void){\n"
		"	vec2 cPos = vTexcoord;\n"
		"	cPos.x -= 0.5*uReserve02.x*sin(uTime/uReserve01.x)+ 0.5*uReserve02.x*cos(uTime) + uReserve02.x;\n"
		"	cPos.y -= 0.5*uReserve02.y*sin(uTime/uReserve01.x)+ 0.5*uReserve02.y*cos(uTime) + uReserve02.y;\n"
		"	float cLength = length(cPos)*uReserve01.x;\n"
		"	vec2 uv = vTexcoord + (cPos/cLength)*sin(cLength/uReserve01.y-uTime*10.0)/uReserve01.z;\n"
		"	uv = clamp(uv,0.0,1.0);\n"
		"	vec3 col = texture2D(uSampler0,uv).rgb/cLength;\n"//
		"	gl_FragColor = vec4((col * uColor.rgb) * uColor.a,uColor.a);\n"
		"}\n";

	static char Frag_SVITex_DreamVision_RT[] =
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"void main()\n"
		"{\n"
		"	vec2 uv = vTexcoord;\n"
		"	vec4 c = texture2D(uSampler0, uv);\n"
		"	c += texture2D(uSampler0, uv+0.001);\n"
		"	c += texture2D(uSampler0, uv+0.003);\n"
		"	c += texture2D(uSampler0, uv+0.005);\n"
		"	c += texture2D(uSampler0, uv+0.007);\n"
		"	c += texture2D(uSampler0, uv+0.009);\n"
		"	c += texture2D(uSampler0, uv+0.011);\n"
		"	c += texture2D(uSampler0, uv-0.001);\n"
		"	c += texture2D(uSampler0, uv-0.003);\n"
		"	c += texture2D(uSampler0, uv-0.005);\n"
		"	c += texture2D(uSampler0, uv-0.007);\n"
		"	c += texture2D(uSampler0, uv-0.009);\n"
		"	c += texture2D(uSampler0, uv-0.011);\n"
		"	c.rgb = vec3((c.r+c.g+c.b)/3.0) / 9.5;\n"
		"	gl_FragColor = vec4((c.rgb * uColor.rgb) * uColor.a,uColor.a);\n"
		"}\n";

	static char Frag_SVITex_PaperFold_RT[]=
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform float		uTime;\n"
		"void main()\n"
		"{\n"
		"	float right = 1.0 - uTime;\n"
		"	vec2 tuv = vec2((vTexcoord.x - uTime) / (right - uTime), vTexcoord.y);\n"
		"	float tx = tuv.x;\n"
		"	if (tx > 0.5){\n"
		"		tx = 1.0 - tx;\n"
		"	}\n"
		"	float top = uTime * tx;\n"
		"	float bottom = 1.0 - top;\n"
		"	vec4 result = vec4(0.0,0.0,0.0,0.0);\n"
		"	if (vTexcoord.x > uTime && vTexcoord.x < right && vTexcoord.y >= top && vTexcoord.y <= bottom){\n"
		"		float ty = mix(0.0, 1.0, (tuv.y - top) / (bottom - top));\n"
		"		result =  texture2D(uSampler0, vec2(tuv.x,ty));\n"
		"	}\n"
		"	gl_FragColor = result;\n"
		"}\n";

	static char Frag_SVITex_Magnify_RT[]=
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec3		uReserve01;\n" //x: inner radius, y: outer radius, z: magnification, 
		"uniform vec2		uReserve02;\n"
		"void main()\n"
		"{\n"
		"	vec2 centerToPixel = vTexcoord - uReserve02;\n"
		"	float dist = length(centerToPixel);\n"
		"	float ratio = smoothstep(uReserve01.x,max(uReserve01.x,uReserve01.y), dist);\n"
		"	vec2 samplePoint = mix(uReserve02 + centerToPixel / uReserve01.z,vTexcoord,ratio);\n"
		"	vec4 color = texture2D(uSampler0, samplePoint);\n"
		"	gl_FragColor = vec4((color.rgb * uColor.rgb) * uColor.a, uColor.a);\n"
		"}\n";

	static char Frag_SVITex_GlassTile_RT[]=
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec3		uReserve01;\n" //x: tiles, y: bevel width, z: offset, 
		"void main()\n"
		"{\n"
		"	vec2 newUV1;\n"
		"	newUV1.xy = vTexcoord.xy + tan((uReserve01.x*2.5)*vTexcoord.xy + uReserve01.z)*(uReserve01.y/100.0);\n"
		"	vec4 color = texture2D(uSampler0, newUV1);\n"
		"	if (newUV1.x < 0.0 || newUV1.x > 1.0 || newUV1.y < 0.0 || newUV1.y > 1.0) color = uColor;\n"
		"	gl_FragColor = vec4(color.rgb * uColor.a, uColor.a);\n"
		"}\n";

	static char Frag_SVITex_ZoomBlur_RT[]=
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec3		uReserve01;\n" //x: center x, y: center y, z: blur amount		
		"void main()\n"
		"{\n"
		"	vec2 newUV1;\n"
		"	newUV1.xy = vTexcoord.xy - uReserve01.xy;\n"
		"	vec4 color = vec4(0.0,0.0,0.0,0.0);\n"
		"	for (float i = 0.0; i < 15.0; i += 1.0){\n"
		"		float scale = 1.0 + uReserve01.z * (i / 14.0);\n"
		"		color += texture2D(uSampler0, newUV1.xy * scale + uReserve01.xy);\n"
		"	}\n"
		"	color /= 15.0;\n"
		"	gl_FragColor = vec4(color.rgb * uColor.a, uColor.a);\n"
		"}\n";

	static char Frag_SVITex_DirectionBlur_RT[]=
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"uniform vec2		uReserve01;\n" //x: angle, y: blur amount		
		"void main()\n"
		"{\n"
		"	vec2 newUV1 = vTexcoord;\n"
		"	vec4 color = vec4(0.0,0.0,0.0,0.0);\n"
		"	float xOffset = cos(uReserve01.x);\n"
		"	float yOffset = sin(uReserve01.x);\n"
		"	for (float i = 0.0; i < 16.0; i += 1.0){\n"
		"		newUV1.x = newUV1.x - uReserve01.y * xOffset;\n"
		"		newUV1.y = newUV1.y - uReserve01.y * yOffset;\n"
		"		color += texture2D(uSampler0, newUV1);\n"
		"	}\n"
		"	color /= 16.0;\n"
		"	gl_FragColor = vec4(color.rgb * uColor.a, uColor.a);\n"
		"}\n";
#endif


    static char Frag_SVIPostHighLight[] =
        "precision mediump float;\n"
        "varying float		vAACoord;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"

        "uniform sampler2D	uSampler0;\n"
        "uniform vec4		uSampleOffset[16];\n"

        "uniform vec4		uTexInfo;\n"
        "void main() {\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"

        "	vec3 color = vec3(0.0,0.0,0.0);\n"
        "	for (int i = 0; i < 16; i++){\n"
        "		color = max(texture2D(uSampler0, scaledCoord + uSampleOffset[i].xy).rgb, color);\n"
        "	}\n"

        "   float ratio = 0.0;\n"//(color.r + color.g + color.b) / 3.0;\n"
        "   if (color.r > 0.7) ratio = 1.0;\n"
        "   if (color.g > 0.7) ratio = 1.0;\n"
        "   if (color.b > 0.7) ratio = 1.0;\n"
        "  gl_FragColor = vec4(color * ratio,1.0) * uColor;\n"
        "}\n";


    static char Frag_SVIDown8X8[] =
        "precision mediump float;\n"
        "varying float		vAACoord;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"

        "uniform sampler2D	uSampler0;\n"
        "uniform vec4		uSampleOffset[16];\n"

        "uniform vec4		uTexInfo;\n"
        "void main() {\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec3 color = vec3(0.0,0.0,0.0);\n"
        "	for (int i = 0; i < 4; i++){\n"
        "		color += texture2D(uSampler0, scaledCoord + uSampleOffset[i].xy).rgb;\n"
        "	}\n"
        "   color /= 4.0;\n"
        "   gl_FragColor = vec4(color, 1.0) * uColor;\n"
        "}\n";


    static char SVIPostRippleVertexShader[] =
    	"precision mediump float;\n"
        "uniform mat4 uProjection;\n"
        "uniform mat4 uView;\n"
        "attribute vec4 aPosition;\n"
        "attribute vec3 aHeights;\n"
    	"varying vec3 vRefract;\n"
    	"varying vec3 vReflect;\n"
    	"varying float vSpecular;\n"
    	"varying float vAlpha;\n"
        "uniform vec4 uReserve02;\n" //uMESH_SIZE_WIDTH , uMESH_SIZE_HEIGHT , uNUM_DETAILS_WIDTH, uNUM_DETAILS_HEIGHT
        "uniform vec4 uReserve03;\n "//uRefractiveIndex

	"attribute vec2		aUnitCoord;\n"
	"varying vec2			vUnitCoord;\n"
		
        "void main() { \n"
    	" 	float dx = aHeights.x - aHeights.z;\n"
    	"	float dy =  aHeights.y - aHeights.x;\n"
        "  	vec3 N = normalize(cross(vec3(1.0, 0.0, dx), vec3(0.0, 1.0, dy)));\n"
   		"  	vec3 fresnelN = normalize(cross(vec3(0.1, 0.0, dx), vec3(0.0, 0.1, dy)));\n"
    	"  	vRefract = refract(vec3(0.0, 0.0, -1.0), N, 0.751879) * 0.15;\n" // 1.0 / 1.33
    	"  	vReflect = reflect(vec3(0.0, 0.0, -1.0), N) * 0.25;\n"

    	// Specular
    	"	vec3 L = normalize(vec3(1.0, 1.0, 0.8));\n"
    	"	vec3 HV = normalize(L + vec3(0.0, 0.0, 1.0));\n"
    	"	float NdotHV = max(dot(N, HV),0.0);\n"
    	"	vSpecular = pow(NdotHV, 40.0);\n"
    	"	L = normalize(vec3(-1.0, -1.0, 0.8));\n"
    	"	HV = normalize(L + vec3(0.0, 0.0, 1.0));\n"
    	"	NdotHV = max(dot(N, HV),0.0);\n"
    	"	vSpecular += pow(NdotHV, 40.0);\n"
        "   vSpecular *= 35.0 * aHeights.x;\n"
        "   vSpecular = min(vSpecular, 1.0);\n"
    	"	vAlpha = pow(1.0 - clamp(dot(N, vec3(-5.0, -5.0, 1.0)),0.0, 1.0), 1.0) * 0.23;"
        "  	gl_Position = uProjection * uView * aPosition;\n"
        "  	vUnitCoord = aUnitCoord;\n"
        "}\n";

    static char Frag_SVIRipple[] =
            "precision highp float;\n"
    		"varying vec3 vRefract;\n"
    		"varying vec3 vReflect;\n"
    		"varying float vSpecular;\n"
    		"varying float vAlpha;\n"
            "uniform sampler2D uSampler0;\n"
            "uniform sampler2D uSampler1;\n"
            "uniform vec4 uReserve01;\n"
            "uniform vec4 uReserve02;\n"
            "uniform vec4 uReserve04;\n" //specularRatio , exponent, fresnelRatio
            "void main() {\n"
            "  vec2 coord = vec2(gl_FragCoord.x /uReserve02.x, 1.0 - gl_FragCoord.y / uReserve02.y);\n"

            "  vec4 tex0 = texture2D(uSampler0, vec2(coord.x + vReflect.x, coord.y + vReflect.y));\n"
            "  vec4 tex1 = texture2D(uSampler1, vec2(coord.x + vRefract.x, coord.y + vRefract.y));\n"

            "  vec4 rippleRGB = vSpecular + tex0 * vAlpha * 0.2 + tex1 * uReserve01.y;\n"
            "  float value = step(0.1, tex1.a);\n"
            "  float r = min(rippleRGB.r, tex1.a);\n"
            "  float g = min(rippleRGB.g, tex1.a);\n"
            "  float b = min(rippleRGB.b, tex1.a);\n"
            "  gl_FragColor = vec4(r * value, g * value, b * value, tex1.a);\n"
            "}\n";

        /*
        "precision mediump float;\n"
        "varying float		vAACoord;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"

        "uniform sampler2D	uSampler0;\n"
        "uniform int		uReserve01;\n"
        "uniform vec4		uReserve02[4];\n"
        "uniform vec4		uTexInfo;\n"

        "void main() {\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec4 color = vec4(0.0,0.0,0.0,1.0);\n"
        "   float accum = 0.0;\n"
        "   float amplitude = 0.01;\n"
        "   vec2 resultPoint = vec2(0.0,0.0);\n"
        "	for (int i = 0; i < uReserve01; i++){\n"
        "       vec2 center = uReserve02[i].xy;\n"
        "       float size = uReserve02[i].z;\n"
        "       float wave = 3.141592 / 0.15;\n"
        "       float distortion = 0.3;\n"
        "       float time = uReserve02[i].w;\n"

        "       vec2 distance = abs(scaledCoord - center)/ size;\n"
        "       float scalar = length(distance);\n"
        "       scalar = abs(1.0 - scalar);\n"

        "       float sinoffset = cos(wave / scalar + time);\n"
        "       sinoffset = clamp(sinoffset, 0.0, 1.0);\n"

        "       float sinsign = cos(wave / scalar + time);\n"
        "       sinoffset = sinoffset * distortion / 32.0;\n"

        "       resultPoint += scaledCoord+(sinoffset*sinsign);\n"
        "       accum += 1.0;\n"
        "	}\n"
        "   resultPoint /= accum;\n"
        "   color= texture2D(uSampler0, resultPoint);\n"
        "   gl_FragColor = color * uColor;\n"
        "}\n";
        */

    static char Frag_SVIPostFinale[] =
        "precision mediump float;\n"
        "varying float		vAACoord;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "uniform sampler2D	uSampler1;\n"
        "uniform vec4		uTexInfo;\n"
        "void main() {\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
        "	vec4 highColor = texture2D( uSampler1, scaledCoord );\n"
        "   gl_FragColor = vec4(texColor.rgb + highColor.rgb * 0.35, texColor.a) * uColor;\n"
        "}\n";


    static char Frag_SVIPostBloom[]=
        "precision mediump float;\n"
        "varying vec2		vTexcoord;\n"
        "varying float		vAACoord;\n"

        "uniform vec4		uTexInfo;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "uniform vec4		uSampleWeight[16];\n"
        "uniform vec4		uSampleOffset[16];\n"

        "void main()\n"
        "{\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec3 color = vec3(0.0,0.0,0.0);\n"
        "	for (int i = 0; i < 16; i++){\n"
        "		color += uSampleWeight[i].rgb * texture2D(uSampler0, scaledCoord + uSampleOffset[i].xy).rgb;\n"
        "	}\n"
        "	gl_FragColor = vec4(color, 1.0) * uColor;\n"
        "}\n";

    static char Frag_SVIPostDebug[] =
        "precision mediump float;\n"
        "varying float		vAACoord;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "void main() {\n"
        "	vec2 scaledCoord = vTexcoord;\n"
        "	vec4 texColor = texture2D( uSampler0, scaledCoord );\n"
        "   gl_FragColor = vec4(texColor.rgb, 1.0) * uColor;\n"
        "}\n";

    static char Frag_SVIPostBlur[]=
        "precision mediump float;\n"
        "varying vec2		vTexcoord;\n"
        "varying float		vAACoord;\n"

        "uniform vec4		uTexInfo;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "uniform vec4		uSampleWeight[16];\n"
        "uniform vec4		uSampleOffset[16];\n"

        "void main()\n"
        "{\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec3 color = vec3(0.0,0.0,0.0);\n"
        "	for (int i = 0; i < 13; i++){\n"
        "		color += uSampleWeight[i].rgb * texture2D(uSampler0, scaledCoord + uSampleOffset[i].xy).rgb;\n"
        "	}\n"
        "	gl_FragColor = vec4(color, 1.0) * uColor;\n"
        "}\n";

    static char Frag_SVIPostMerge[] =
        "precision mediump float;\n"
        "varying float		vAACoord;\n"
        "varying vec2		vTexcoord;\n"
        "uniform vec4		uColor;\n"
        "uniform sampler2D	uSampler0;\n"
        "uniform sampler2D	uSampler1;\n"
        "uniform sampler2D	uSampler2;\n"
        "uniform vec4		uTexInfo;\n"
        "void main() {\n"
        "	vec2 scaledCoord = uTexInfo.xy + (uTexInfo.zw * vTexcoord);\n"
        "	vec4 texA = texture2D( uSampler0, scaledCoord );\n"
        "	vec4 texB = texture2D( uSampler1, scaledCoord );\n"
        "	vec4 texC = texture2D( uSampler2, scaledCoord );\n"
        "   gl_FragColor = vec4(texA.rgb * 0.1 + texB.rgb * 0.45 + texC.rgb * 0.45, 1.0) * uColor;\n"
        "}\n";

    static const SVIBool SVIPresetProgramUsage[] = {
        //colour programs (4)
        true,//"PR_COLOR",
        false,// "PR_COLOR_LT_SPOT",
        false,//"PR_COLOR_LT_LINEAR",
        false,//"PR_COLOR_SHADOW",

        //texture programs (8)
        true,//"PR_TEX_ADD",
        false,//"PR_TEX_ADD_LT_SPOT",
        false,//"PR_TEX_ADD_LT_LINEAR",
        false,//"PR_TEX_ADD_SHADOW",
        true,//"PR_TEX_MLT",
        false,//"PR_TEX_MLT_LT_SPOT",
        false,//"PR_TEX_MLT_LT_LINEAR",
        false,//"PR_TEX_MLT_SHADOW",

        //texture + shader based scale to fit programs (8)
        true,//"PR_TEX_ADD_FIT",
        false,//"PR_TEX_ADD_LT_SPOT_FIT",
        false,//"PR_TEX_ADD_LT_LINEAR_FIT",
        false,//"PR_TEX_ADD_SHADOW_FIT",
        true,//"PR_TEX_MLT_FIT",
        false,//"PR_TEX_MLT_LT_SPOT_FIT",
        false,//"PR_TEX_MLT_LT_LINEAR_FIT",
        false,//"PR_TEX_MLT_SHADOW_FIT",
        //2012-03-13 masterkeaton27@gmail.com
        false,//"PR_3DSHADOW",
#if SVI_USE_POST_EFFECT
        //2011-10-31 masterkeaton27@gmail.com
        //render target post effect added.
        false,//"PR_RIPPLE_RT",
        false,//"PR_PULSE_RT",
        false,//"PR_DREAMVISION_RT",
        false,//"PR_WIZZLE_RT",
        false,//"PR_PAPERFOLD_RT",
        false,//"PR_MAGNIFY_RT",
        false,//"PR_GLASSTILE_RT",
        false,//"PR_ZOOMBLUR_RT",
        false,//"PR_DIRECTIONBLUR_RT",
#endif
        false,//"PR_3DMODEL",
        //2012-11-21 masterkeaton27@gmail.com
        false,//"PR_3DMODEL_OFFSET",
        //2012-02-16 masterkeaton27@gmail.com
        false,//"PR_PHYSIC3DMODEL", //from SURC
        false,//"PR_PHYSIC3DMODELAA", //2012-04-09 masterkeaton27@gmail.com

        // 20111111 jbkim
        false,//"PR_LINEAR_GRADATION",
        false,//"PR_RADIAL_GRADATION",

        //2012-06-21 masterkeaton27@gmail.com
        //new shader effect for mobile office
        false,//"PR_LINEAR_SWIPE",
        false,//"PR_RADIAL_SWIPE",

        //2012-11-26 masterkeaton27@gmail.com
        //new shader for normal map calculation
        false,//"PR_NORMALMAP",
        true,//"PR_SPECULAR_NORMALMAP",

        //2012-12-14 masterkeaton27@gmail.com
        false,//"PR_POST_HIGHLIGHT",
        false,//"PR_POST_MERGE",
        false,//"PR_POST_FINALE",
        false,//"PR_POST_BLUR",
        false,//"PR_POST_DEBUG",
        false,//"PR_POST_BLOOM",
        false,//"PR_POST_DOWN8X8"
        false,//"PR_POST_RIPPLE"

        false,//"PR_TEX_CPU"
        false,//"PR_FILTER_SKETCH"
        false,//"PR_FILTER_OILPAINT"
        false,//"PR_FILTER_WATERCOLOR"
        false,//"PR_FILTER_CARTOON"
        false,//"PR_FILTER_RADIAL"
        false,//"PR_FILTER_MONOTONE"
        false,//"PR_FILTER_SMOKE"
        false,//"PR_TESSELATION"
    };

    static const char * SVIPresetProgramName[] = {
        //colour programs (4)
            "PR_COLOR",
            "PR_COLOR_LT_SPOT",
            "PR_COLOR_LT_LINEAR",
            "PR_COLOR_SHADOW",

            //texture programs (8)
            "PR_TEX_ADD",
            "PR_TEX_ADD_LT_SPOT",
            "PR_TEX_ADD_LT_LINEAR",
            "PR_TEX_ADD_SHADOW",
            "PR_TEX_MLT",
            "PR_TEX_MLT_LT_SPOT",
            "PR_TEX_MLT_LT_LINEAR",
            "PR_TEX_MLT_SHADOW",

            //texture + shader based scale to fit programs (8)
            "PR_TEX_ADD_FIT",
            "PR_TEX_ADD_LT_SPOT_FIT",
            "PR_TEX_ADD_LT_LINEAR_FIT",
            "PR_TEX_ADD_SHADOW_FIT",
            "PR_TEX_MLT_FIT",
            "PR_TEX_MLT_LT_SPOT_FIT",
            "PR_TEX_MLT_LT_LINEAR_FIT",
            "PR_TEX_MLT_SHADOW_FIT",
            //2012-03-13 masterkeaton27@gmail.com
            "PR_3DSHADOW",
#if SVI_USE_POST_EFFECT
            //2011-10-31 masterkeaton27@gmail.com
            //render target post effect added.
            "PR_RIPPLE_RT",
            "PR_PULSE_RT",
            "PR_DREAMVISION_RT",
            "PR_WIZZLE_RT",
            "PR_PAPERFOLD_RT",
            "PR_MAGNIFY_RT",
            "PR_GLASSTILE_RT",
            "PR_ZOOMBLUR_RT",
            "PR_DIRECTIONBLUR_RT",
#endif
            "PR_3DMODEL",
            //2012-11-21 masterkeaton27@gmail.com
            "PR_3DMODEL_OFFSET",
            //2012-02-16 masterkeaton27@gmail.com
            "PR_PHYSIC3DMODEL", //from SURC
            "PR_PHYSIC3DMODELAA", //2012-04-09 masterkeaton27@gmail.com

            // 20111111 jbkim
            "PR_LINEAR_GRADATION",
            "PR_RADIAL_GRADATION",

            //2012-06-21 masterkeaton27@gmail.com
            //new shader effect for mobile office
            "PR_LINEAR_SWIPE",
            "PR_RADIAL_SWIPE",

            //2012-11-26 masterkeaton27@gmail.com
            //new shader for normal map calculation
            "PR_NORMALMAP",
            "PR_SPECULAR_NORMALMAP",

            //2012-12-14 masterkeaton27@gmail.com
            "PR_POST_HIGHLIGHT",
            "PR_POST_MERGE",
            "PR_POST_FINALE",
            "PR_POST_BLUR",
            "PR_POST_DEBUG",
            "PR_POST_BLOOM",
            "PR_POST_DOWN8X8",

            //2013-01-29 masterkeaton27@gmail.com
            "PR_POST_RIPPLE",

            //2013-06-24 masterkeaton27@gmail.com
            "PR_TEX_CPU",

            //2013-06-26 masterkeaton27@gmail.com
            "PR_FILTER_SKETCH",
            "PR_FILTER_OILPAINT",
            "PR_FILTER_WATERCOLOR",
            "PR_FILTER_CARTOON",
            "PR_FILTER_RADIAL",
            "PR_FILTER_MONOTONE",
            "PR_FILTER_SMOKE",

            "PR_TESSELATION"
        };


	static const char * SVIPresetFragmentName[] = {
		"Frag_SVIColor",
		"Frag_SVIColor_LT_SPOT",
		"Frag_SVIColor_LT_LINEAR",
		"Frag_SVIColor_SHADOW",

		//texture shaders
		"Frag_SVITexAdd",
		"Frag_SVITexAdd_LT_SPOT",
		"Frag_SVITexAdd_LT_LINEAR",
		"Frag_SVITexAdd_SHADOW",
		"Frag_SVITexMlt",
		"Frag_SVITexMlt_LT_SPOT",
		"Frag_SVITexMlt_LT_LINEAR",
		"Frag_SVITexMlt_SHADOW",

		//texture scale based shaders
		"Frag_SVITexAdd_FIT",
		"Frag_SVITexAdd_LT_SPOT_FIT",
		"Frag_SVITexAdd_LT_LINEAR_FIT",
		"Frag_SVITexAdd_SHADOW_FIT",
		"Frag_SVITexMlt_FIT",
		"Frag_SVITexMlt_LT_SPOT_FIT",
		"Frag_SVITexMlt_LT_LINEAR_FIT",
		"Frag_SVITexMlt_SHADOW_FIT",
		"Frag_SVI3DShadow",
#if SVI_USE_POST_EFFECT		
		"Frag_SVITex_Ripple_RT",
		"Frag_SVITex_Pulse_RT",
		"Frag_SVITex_DreamVision_RT",
		"Frag_SVITex_Wizzle_RT",
		"Frag_SVITex_PaperFold_RT",
		"Frag_SVITex_Magnify_RT",
		"Frag_SVITex_GlassTile_RT",
		"Frag_SVITex_ZoomBlur_RT",
		"Frag_SVITex_DirectionBlur_RT",
#else
		"Frag_SVITexMlt_SHADOW_FIT"
#endif
		"Frag_SVI3DModel",
		"Frag_SVIPhysics3DModel",
		"Frag_SVIAAPhysics3DModel",

		// 2011-11-11 jbkim 
		"Frag_SVILinearGradationMlt",
		"Frag_SVIRadialGradationMlt",

		// 2012-06-21 masterkeaton27@gmail.com
		"Frag_SVITex_Linear_Swipe",
		"Frag_SVITex_Radial_Swipe",

        "Frag_SVITex_NormalMap",
        "Frag_SVITex_SpecularNormalMap",

        //2012-12-14 masterkeaton27@gmail.com
        "Frag_SVIPostHighLight",
        "Frag_SVIPostMerge",
        "Frag_SVIPostFinale",
        "Frag_SVIPostBlur",
        "Frag_SVIPostDebug",
        "Frag_SVIPostBloom",
        "Frag_SVIDown8X8",
        //2013-01-29 masterkeaton27@gmail.com
        "Frag_SVIRipple",

        //2013-06-24 masterkeaton27@gmail.com
        "Frag_SVICPUTex",

        //2013-06-26 masterkeaton27@gmail.com
        "Frag_SVIFilterSketch",
        "Frag_SVIFilterOilpaint",
        "Frag_SVIFilterWaterColor",
        "Frag_SVIFilterCartoon",
        "Frag_SVIFilterRadial",
        "Frag_SVIFilterMonotone",
        "Frag_SVIFilterSmoke",

        "Frag_SVIFilterSmoke"
	};

	static char* SVIPresetFragmentShaders[] = {
		//color shaders
		Frag_SVIColor,
		Frag_SVIColor_LT_SPOT,
		Frag_SVIColor_LT_LINEAR,
		Frag_SVIColor_SHADOW,

		//texture shaders
		Frag_SVITexAdd,
		Frag_SVITexAdd_LT_SPOT,
		Frag_SVITexAdd_LT_LINEAR,
		Frag_SVITexAdd_SHADOW,
		Frag_SVITexMlt,
		Frag_SVITexMlt_LT_SPOT,
		Frag_SVITexMlt_LT_LINEAR,
		Frag_SVITexMlt_SHADOW,

		//texture scale based shaders
		Frag_SVITexAdd_FIT,
		Frag_SVITexAdd_LT_SPOT_FIT,
		Frag_SVITexAdd_LT_LINEAR_FIT,
		Frag_SVITexAdd_SHADOW_FIT,
		Frag_SVITexMlt_FIT,
		Frag_SVITexMlt_LT_SPOT_FIT,
		Frag_SVITexMlt_LT_LINEAR_FIT,
		Frag_SVITexMlt_SHADOW_FIT,
		Frag_SVI3DShadow,

#if SVI_USE_POST_EFFECT
		//render target shader
		Frag_SVITex_Ripple_RT,
		Frag_SVITex_Pulse_RT,
		Frag_SVITex_DreamVision_RT,
		Frag_SVITex_Wizzle_RT,
		Frag_SVITex_PaperFold_RT,
		Frag_SVITex_Magnify_RT,
		Frag_SVITex_GlassTile_RT,
		Frag_SVITex_ZoomBlur_RT,
		Frag_SVITex_DirectionBlur_RT,
#endif
		Frag_SVI3DModel,
        Frag_SVI3DModelOffset,
		Frag_SVIPhysics3DModel,
		Frag_SVIAAPhysics3DModel,

		// jbkim 20111111
		Frag_SVILinearGradation,
		Frag_SVIRadialGradation,

		//2012-06-21 masterkeaton27@gmail.com
		//new shader effect for mobile office
		Frag_SVITex_Linear_Swipe,
		Frag_SVITex_Radial_Swipe,

        //2012-11-26 masterkeaton27@gmail.com
        Frag_SVITex_NormalMap,
        Frag_SVITex_SpecularNormalMap,

        //2012-12-14 masterkeaton27@gmail.com
        Frag_SVIPostHighLight,
        Frag_SVIPostMerge,
        Frag_SVIPostFinale,
        Frag_SVIPostBlur,
        Frag_SVIPostDebug,
        Frag_SVIPostBloom,
        Frag_SVIDown8X8,
        //2013-01-29 masterkeaton27@gmail.com
        Frag_SVIRipple,

        //2013-06-24 masterkeaton27@gmail.com
        Frag_SVICPUTex,

        //2013-06-26 masterkeaton27@gmail.com
        Frag_SVIFilterSketch,
        Frag_SVIFilterOilpaint,
        Frag_SVIFilterWaterColor,
        Frag_SVIFilterCartoon,
        Frag_SVIFilterRadial,
        Frag_SVIFilterMonotone,
        Frag_SVIFilterSmoke,

        //2012/12/10 rareboy0112@facebook.com
        Frag_SVITexMlt,
	};


#endif

    

	void SVIProgramManager::initializePreLoadVS(){
        if (DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP);
        if (DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL);
        if (DEFAULT_VERTEX_SHADER_TEXTURE != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE);
        if (DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION);
        if (DEFAULT_VERTEX_SHADER_CPU_TEXTURE != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_CPU_TEXTURE);
        if (DEFAULT_VERTEX_SHADER_COLOR != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_COLOR);
        if (DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW);
        if (DEFAULT_VERTEX_SHADER_COLOR_SHADOW != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_COLOR_SHADOW);
        if (DEFAULT_VERTEX_SHADER_3D_SHADOW != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_3D_SHADOW);
        if (DEFAULT_VERTEX_SHADER_HEIGHTS != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_HEIGHTS);

        DEFAULT_VERTEX_SHADER_TEXTURE = compileShader(GL_VERTEX_SHADER, SVITextureVertexShader);
        DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION = compileShader(GL_VERTEX_SHADER, SVITextureTesselationVertexShader);
        DEFAULT_VERTEX_SHADER_CPU_TEXTURE = compileShader(GL_VERTEX_SHADER, SVICPUTextureVertexShader);

        DEFAULT_VERTEX_SHADER_COLOR = compileShader(GL_VERTEX_SHADER, SVIColorVertexShader);
        DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW = compileShader(GL_VERTEX_SHADER, SVITextureShadowVertexShader);
        DEFAULT_VERTEX_SHADER_COLOR_SHADOW = compileShader(GL_VERTEX_SHADER, SVIColorShadowVertexShader);
        DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL = compileShader(GL_VERTEX_SHADER, SVITextureNormalVertexShader);
        DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP = compileShader(GL_VERTEX_SHADER, SVITextureNormalMapVertexShader);
        DEFAULT_VERTEX_SHADER_3D_SHADOW = compileShader(GL_VERTEX_SHADER, SVI3DShadowVertexShader);
        DEFAULT_VERTEX_SHADER_HEIGHTS = compileShader(GL_VERTEX_SHADER, SVIPostRippleVertexShader);

        mIsDefaultVertexShaderLoaded = SVITRUE;
    }

    void SVIProgramManager::restorePreLoadVS(){
        mIsDefaultVertexShaderLoaded = SVIFALSE;
    }

    void SVIProgramManager::finalizePreLoadVS(){

        if (DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP);
        if (DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL);
        if (DEFAULT_VERTEX_SHADER_TEXTURE != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE);
        if (DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION);
        if (DEFAULT_VERTEX_SHADER_CPU_TEXTURE != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_CPU_TEXTURE);
        if (DEFAULT_VERTEX_SHADER_COLOR != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_COLOR);
        if (DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW);
        if (DEFAULT_VERTEX_SHADER_COLOR_SHADOW != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_COLOR_SHADOW);
        if (DEFAULT_VERTEX_SHADER_3D_SHADOW != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_3D_SHADOW);
        if (DEFAULT_VERTEX_SHADER_HEIGHTS != SVI_NO_HANDLE) glDeleteShader(DEFAULT_VERTEX_SHADER_HEIGHTS);

        DEFAULT_VERTEX_SHADER_CPU_TEXTURE = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_TEXTURE = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_COLOR = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_COLOR_SHADOW = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_3D_SHADOW = SVI_NO_HANDLE;
        DEFAULT_VERTEX_SHADER_HEIGHTS = SVI_NO_HANDLE;
    }

	void SVIProgram::createShaderImmediate(){



#ifndef USE_BINARY_PROGRAM
        if (!mSVIGLSurface->getProgramManager()->mIsDefaultVertexShaderLoaded){
           mSVIGLSurface->getProgramManager()->initializePreLoadVS();
        }

        mID = createPresetShader();
        mHandler = new SVIProgramHandler(mID);
#else        
        loadShaderFromRAM();
#endif
    }


    SVIBool SVIProgram::checkBinaryShaderExist(){
        if (mActivated) return SVITRUE;

        char myBinaryFileName[1024] = "";
        sprintf(myBinaryFileName,"/sdcard/svi/%s.shader",SVIPresetProgramName[mType]);
        FILE* outfile = fopen(myBinaryFileName, "rb");

        if (outfile) {
            LOGE("Storage Shader exist. <%d><%s>", (SVIInt)outfile, SVIPresetProgramName[mType]);
			fclose(outfile);
            return SVITRUE;
        }

        LOGE("Storage Shader not exist. <%s>",SVIPresetProgramName[mType]);
        return SVIFALSE;
    }


    void SVIProgram::loadShaderFromRAM(){
        if (mActivated) return;

        int gpuType = SVIBinaryProgram::GPU_UNKNOWN;

        if (mSVIGLSurface->getRenderer()->isAdreno320()){
            gpuType = SVIBinaryProgram::ADRENO_320;
        }else if (mSVIGLSurface->getRenderer()->isSGX544()){
            gpuType = SVIBinaryProgram::SGX_544;
        }


        SVIBinaryInfo info;
        if (!SVIBinaryProgram::getProgram(mType, gpuType, &info)){
            mRAMBinary = SVIFALSE;
        }else{
            mRAMBinary = SVITRUE;
        }

        if (!mRAMBinary){
            LOGE("No RAM Binary founded. switch to RTC shader method.<%s>",SVIPresetProgramName[mType]);

            if (!mSVIGLSurface->getProgramManager()->mIsDefaultVertexShaderLoaded){
                mSVIGLSurface->getProgramManager()->initializePreLoadVS();
            }
			
	     if (mID != SVI_NO_HANDLE){
		 glDeleteProgram(mID);
	 	 mID = SVI_NO_HANDLE;
	     }
            mID = createPresetShader();
			
	     if (mHandler  !=  NULL)
			SVI_SVIFE_DELETE(mHandler);
		 
            mHandler = new SVIProgramHandler(mID);

            //2013-02-04 masterkeaton27@gmail.com
            //load program from storage is not a safety way to bind shader.
            /*
            if (checkBinaryShaderExist()){
                loadShaderFromBinary();
            }else{
                saveShaderToBinary();
            }*/
            return;
        }



        if (mID == SVI_NO_HANDLE) mID = glCreateProgram();

        //  get the program binary
        if (mProgramBinary == NULL){
            mProgramBinaryLength = info.mBinaryLength;
            mProgramBinary = info.mBinaryData;
            mProgramBinaryFormat = info.mBinaryFormat;
        }

        glProgramBinaryOES(mID, mProgramBinaryFormat, mProgramBinary, mProgramBinaryLength);

        GLint success;

        glGetProgramiv(mID, GL_LINK_STATUS, &success);

        if (!success)
        {
            mActivated = SVIFALSE;
            mProgramBinary = NULL;
            mProgramBinaryLength = 0;
            LOGE("RAM load & link failed switch to RTC shader method.<%s>",SVIPresetProgramName[mType]);


            if (!mSVIGLSurface->getProgramManager()->mIsDefaultVertexShaderLoaded){
                mSVIGLSurface->getProgramManager()->initializePreLoadVS();
            }

            mID = createPresetShader();
	     if (mHandler  !=  NULL)
			SVI_SVIFE_DELETE(mHandler);
		 
            mHandler = new SVIProgramHandler(mID);

        }else{
            mActivated = SVITRUE;
            if (mHandler == NULL) mHandler = new SVIProgramHandler(mID);
            LOGI("RAM Binary link completed.<%s>",SVIPresetProgramName[mType]);
        }
    }

    void SVIProgram::loadShaderFromBinary(){
        if (mActivated) return;

        char myBinaryFileName[1024] = "";
        GLint   binaryLength;
        GLvoid* binary;
        GLint   success;
        FILE*   infile;
        GLenum  binaryFormat;


        if (mID == SVI_NO_HANDLE) mID = glCreateProgram();

        sprintf(myBinaryFileName,"/sdcard/svi/%s.shader",SVIPresetProgramName[mType]);

        //  Read the program binary
        if (mProgramBinary == NULL){
            infile = fopen(myBinaryFileName, "rb");
            if(infile == NULL){
            	return;
            }
            fseek(infile, 0, SEEK_END);
            binaryLength = (GLint)ftell(infile);
            mProgramBinaryLength = binaryLength - sizeof(GLenum);

            binary = (GLvoid*)malloc(mProgramBinaryLength);
            fseek(infile, 0, SEEK_SET);
            fread(&binaryFormat, sizeof(GLenum), 1, infile);
            fread(binary, mProgramBinaryLength, 1, infile);
            if(infile)
            fclose(infile);

            mProgramBinary = binary;
            mProgramBinaryFormat = binaryFormat;
        }


        //  Load the binary into the program object -- no need to link!
        glProgramBinaryOES(mID, mProgramBinaryFormat, mProgramBinary, mProgramBinaryLength);


        glGetProgramiv(mID, GL_LINK_STATUS, &success);

        if (!success)
        {
            mRAMBinary =
                mActivated = SVIFALSE;
            free(mProgramBinary);
            mProgramBinary = NULL;
            mProgramBinaryLength = 0;
            LOGE("load & link failed <%s>",SVIPresetProgramName[mType]);
        }else{
            mRAMBinary = SVIFALSE;
            LOGE("Storage Shader load complete. <%s>",SVIPresetProgramName[mType]);
            mActivated = SVITRUE;
            if (mHandler == NULL) mHandler = new SVIProgramHandler(mID);
        }
    }

    SVIBool SVIProgram::saveShaderToBinary(){
        //using common vertex shader.
        //we don't need to compile vertex shader twice or more.
        char* vertexShader = getUseTexture() ? SVITextureVertexShader : SVIColorVertexShader;
        if (getUseShadow()){
            vertexShader = getUseTexture() ? SVITextureShadowVertexShader : SVIColorShadowVertexShader;
        }

        //2013-06-24 masterkeaton27@gmail.com
        if (getUseCPUTexture()){
            vertexShader = SVICPUTextureVertexShader;
        }               

        //2012/12/10 rareboy0112@facebook.com
        if(getTesselation())
        {
            vertexShader = SVITextureTesselationVertexShader;
        }

        //2011-11-11 masterkeaton27@gmail.com
        if (getUseNormal()){
            vertexShader = SVITextureNormalVertexShader;
            if (getUseShadow())	vertexShader = SVITextureNormalMapVertexShader;
        }

        //2012-11-27 masterkeaton27@gmail.com
        //NOTE: add 2d planner normal map vertex shader for normal texturing
        if (getUse2DNormalMap()){
            vertexShader = SVITextureNormalMapVertexShader;
        }


        //2013-01-30 masterkeaton27@gmail.com
        //for the ripple effect heights field
        if (getUseHeights()){
            vertexShader = SVIPostRippleVertexShader;
        }


        //LOGE("Compile Fragment<%s> Shader Begin..", SVIPresetFragmentName[mType]);

        //using preset fragment shader.
        const GLchar* vsSource = vertexShader;
        GLchar* fsSource = SVIPresetFragmentShaders[mType];
        char myBinaryFileName[1024] = "";
        sprintf(myBinaryFileName,"/sdcard/svi/%s.shader",SVIPresetProgramName[mType]);
        //sprintf(myBinaryFileName,"/system/preloaddata/svi/%s.shader",SVIPresetProgramName[mType]);

        GLenum* binaryFormat;

        GLint   success;
        //
        //  Create new shader/program objects and attach them together.
        //

        mVertexShaderID = mSVIGLSurface->getProgramManager()->compileShader(GL_VERTEX_SHADER, vertexShader);
        mFragmentShaderID = mSVIGLSurface->getProgramManager()->compileShader(GL_FRAGMENT_SHADER, SVIPresetFragmentShaders[mType]);

        mID = glCreateProgram();

        glAttachShader(mID , mVertexShaderID);
        glAttachShader(mID , mFragmentShaderID);

        glLinkProgram(mID);
        glGetProgramiv(mID, GL_LINK_STATUS, &success);

        GLint count = 0;
        GLenum format;

        glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS_OES, &count);

        if (count > 0){
            glGetIntegerv(GL_PROGRAM_BINARY_FORMATS_OES, (GLint*)&format);
        }else{
            LOGE("Failed get count '%s'....", myBinaryFileName);
            return SVIFALSE;
        }

        if (success)
        {
            GLint   binaryLength;
            GLvoid* binary;
            FILE*   outfile;

            //  Retrieve the binary from the program object
            int resultcode = mkdir("/sdcard/svi/", 0770);

            glGetProgramiv(mID, GL_PROGRAM_BINARY_LENGTH_OES, &binaryLength);
            binary = (GLvoid*)malloc(binaryLength);
            glGetProgramBinaryOES(mID, binaryLength, NULL, &format, binary);

            //  Cache the program binary for future runs
            LOGE("Saving Binary Shader '%s' (size: %d bytes)....", myBinaryFileName, binaryLength );
            outfile = fopen(myBinaryFileName, "wb");

            fwrite(&format, sizeof(GLenum), 1, outfile);
            fwrite(binary, binaryLength, 1, outfile);
            fclose(outfile);

            mProgramBinary = binary;
            mProgramBinaryFormat = format;
            mProgramBinaryLength = binaryLength;

            if (mHandler == NULL) mHandler = new SVIProgramHandler(mID);

            mActivated = SVITRUE;
         }else{
               LOGE("Failed Saving Binary Shader '%s'....", myBinaryFileName);
         }

         return success;
    }

	SVIInt SVIProgram::createPresetShader(){
		//TODO: should check error situation

		//create program
		SVIInt program = SVI_NO_HANDLE;
		program = glCreateProgram();


		//using common vertex shader.
		//we don't need to compile vertex shader twice or more.
		SVIInt vertexShader = getUseTexture() ? mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE : mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_COLOR;
		if (getUseShadow()){
			vertexShader = getUseTexture() ? mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW : mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_COLOR_SHADOW;
		}
        
        //2013-06-24 masterkeaton27@gmail.com
        if (getUseCPUTexture()){
            vertexShader = mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_CPU_TEXTURE;
        } 

        //2012/12/10 rareboy0112@facebook.com
        if(getTesselation())
        {
            vertexShader = mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION;
        }

		//2011-11-11 masterkeaton27@gmail.com
		if (getUseNormal()){
			vertexShader = mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL;

			if (getUseShadow())	vertexShader = mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_3D_SHADOW;
		}

		//2012-11-27 masterkeaton27@gmail.com
		//NOTE: add 2d planner normal map vertex shader for normal texturing
		if (getUse2DNormalMap()){
			vertexShader = mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP;
		}

        //2013-01-30 masterkeaton27@gmail.com
        //for the ripple effect heights field
        if (getUseHeights()){
            vertexShader = mSVIGLSurface->getProgramManager()->DEFAULT_VERTEX_SHADER_HEIGHTS;
        }


		//LOGE("Compile Fragment<%s> Shader Begin..", SVIPresetFragmentName[mType]);

        //using preset fragment shader.

#if USE_COMPILE
        SVIInt fragmentShader =
            mSVIGLSurface->getProgramManager()->compileShader(GL_FRAGMENT_SHADER, (const SVIPChar)SVIPresetFragmentShaders[mType]);
#else
        SVIInt fragmentShader =
            mSVIGLSurface->getProgramManager()->binaryShader(GL_FRAGMENT_SHADER, SVIPresetFragmentShaders[mType], SVIPresetFragmentShaders_Size[mType]);
#endif

		//let's link with program.
		if (vertexShader != 0 && fragmentShader != 0){
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);

			//link handles and program to GL context.
			linkShader(program);

			//2012-02-22 masterkeaton27@gmail.com
			mActivated = SVITRUE;
		}

		return program;
	}

	SVIProgramManager::SVIProgramManager(SVIGLSurface *surface){

		mSVIGLSurface = surface;
		
		for (SVIInt n = 0; n < PR_TYPE_COUNT; n++) mPrograms[n] = NULL;
		
		DEFAULT_VERTEX_SHADER_HEIGHTS = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_TEXTURE = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_CPU_TEXTURE = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_COLOR = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_COLOR_SHADOW = SVI_NO_HANDLE;
		DEFAULT_VERTEX_SHADER_3D_SHADOW = SVI_NO_HANDLE;
	    
		mIsDefaultVertexShaderLoaded = SVIFALSE;
	}

	SVIProgramManager::~SVIProgramManager(){
		destroy();
		mSVIGLSurface = NULL;
	}


	void SVIProgramManager::create(){
		//SVIProgram::initialize();
		createPresetShaders();
	}

	void SVIProgramManager::update(){

	}

	void SVIProgramManager::restore(){
        restorePreLoadVS();
		//SVIProgram::initialize();
		createPresetShaders();
	}

	void SVIProgramManager::destroy(){
		finalizePreLoadVS();
		for (SVIInt n = 0; n < PR_TYPE_COUNT; n++) SVI_SVIFE_DELETE(mPrograms[n]);
	}


	SVIProgramHandler * 	SVIProgramManager::getHandler(SVIProgramParams & pParam){
		SVIInt programType = pParam.getProgram();

		if (mPrograms[programType] != NULL && mPrograms[programType]->getActivated()){
			return mPrograms[programType]->getHandler();
		} else if (mPrograms[programType] != NULL && !mPrograms[programType]->getActivated()){
			mPrograms[programType]->createShaderImmediate();
			return mPrograms[programType]->getHandler();
		}
		return NULL;
	}

	SVIProgramHandler * 	SVIProgramManager::getHandler(SVIProgramParams * pParam){
		SVIInt programType = pParam->getProgram();
		if (mPrograms[programType] != NULL && mPrograms[programType]->getActivated()){
			return mPrograms[programType]->getHandler();
		} else if (mPrograms[programType] != NULL && !mPrograms[programType]->getActivated()){
			mPrograms[programType]->createShaderImmediate();
			return mPrograms[programType]->getHandler();
		}
		return NULL;
	}

	SVIProgram * SVIProgramManager::getProgram(SVIProgramParams & pParam){
		SVIInt programType = pParam.getProgram();
		if (mPrograms[programType] != NULL && mPrograms[programType]->getActivated()){
			return mPrograms[programType];
		}else if (mPrograms[programType] != NULL && !mPrograms[programType]->getActivated()){
			mPrograms[programType]->createShaderImmediate();
			return mPrograms[programType];
		}
		return NULL;
	}

	SVIProgram * SVIProgramManager::getProgram(SVIProgramParams * pParam){
		SVIInt programType = pParam->getProgram();
		if (mPrograms[programType] != NULL && mPrograms[programType]->getActivated()){
			return mPrograms[programType];
		}else if (mPrograms[programType] != NULL && !mPrograms[programType]->getActivated()){
			mPrograms[programType]->createShaderImmediate();
			return mPrograms[programType];
		}
		return NULL;
	}

	SVIProgram * SVIProgramManager::getProgram(SVIProgramType type){
		if (mPrograms[type] != NULL && !mPrograms[type]->getActivated()){
			mPrograms[type]->createShaderImmediate();
		}
		return mPrograms[type];
	}

	SVIProgram * SVIProgramManager::addCustomProgram(const SVIChar* pName,const SVIChar* pVertexSrc,const SVIChar* pFragmentSrc){
		return NULL;
	}

	SVIProgram * SVIProgramManager::getCustomProgram(const SVIChar* pName){
		return NULL;
	}

	SVIProgram * SVIProgramManager::freeCustomProgram(const SVIChar* pName){
		return NULL;
	}

	void SVIProgramManager::use(SVIProgramParams & pParam){
		SVIInt programType = pParam.getProgram();
		if (mPrograms[programType] != NULL && mPrograms[programType]->getActivated()){
			mPrograms[programType]->use();
		}
	}
	void SVIProgramManager::use(SVIProgramParams * pParam){
		SVIInt programType = pParam->getProgram();
		if (mPrograms[programType] != NULL && mPrograms[programType]->getActivated()){
			mPrograms[programType]->use();
		}
	}

	void SVIProgramManager::use(SVIProgramType type){
		if (mPrograms[type] != NULL && mPrograms[type]->getActivated()){
			mPrograms[type]->use();
		}
	}

	void SVIProgramManager::createPresetShaders(){
		for (SVIInt n = 0; n < PR_TYPE_COUNT; n++){
			if (mPrograms[n] == NULL){
				mPrograms[n] = new SVIProgram(mSVIGLSurface, (SVIProgramType)n);
#ifdef WRITE_BINARY_PROGRAM
                //if (mPrograms[n]->checkBinaryShaderExist()){

                //}else{
                    mPrograms[n]->saveShaderToBinary();
                //}
#endif                
			}else{
                if (mPrograms[n]->getActivated()){
                    SVI_SVIFE_DELETE(mPrograms[n]);
				    mPrograms[n] = new SVIProgram(mSVIGLSurface, (SVIProgramType)n);
                }
#ifdef WRITE_BINARY_PROGRAM
                //if (mPrograms[n]->checkBinaryShaderExist()){

                //}else{
                    mPrograms[n]->saveShaderToBinary();
                //}
#endif 
			}
		}
	}

	//2011-10-31 masterkeaton27@gmail.com
	//common variables for post effect shader

	SVICommonShaderVariables::SVICommonShaderVariables(){
		mElapsedTime = 0.0f;
		mInteractionPosition.x = 0.0f;
		mInteractionPosition.y = 0.0f;
		mInteractionVelocity.x = 0.0f;
		mInteractionVelocity.y = 0.0f;
		mScreenSize.x = 1280.0f;
		mScreenSize.y = 800.0f;
	}

	SVICommonShaderVariables::~SVICommonShaderVariables(){
	}

	void SVICommonShaderVariables::setInteraction(SVIVector2& position, SVIVector2& velocity){
		mInteractionPosition = position;
		mInteractionVelocity = velocity;
	}
	void SVICommonShaderVariables::setPosition(SVIVector2& position){
		mInteractionPosition = position;
	}

	SVIVector2  SVICommonShaderVariables::getNormalizedPosition(){
		if (mScreenSize.x == 0.0f || mScreenSize.y == 0.0f) return SVIVector2(0.5f,0.5f);
		if (mInteractionPosition.x == 0.0f && mInteractionPosition.y == 0.0f) return SVIVector2(0.0f,0.0f);

		SVIVector2 result = mInteractionPosition;

		result.x /= mScreenSize.x;
		result.y /= mScreenSize.y;

		return result;
	}

	void SVICommonShaderVariables::setVelocity(SVIVector2& velocity){
		mInteractionVelocity = velocity;
	}

	void SVICommonShaderVariables::setScreenSize(SVIVector2& size){
		mScreenSize = size;
	}

	void SVICommonShaderVariables::setScreenSize(float width, float height){
		mScreenSize.x = width;
		mScreenSize.y = height;
	}

	void SVICommonShaderVariables::setTime(float time){
		mElapsedTime = time;
	}

	void SVICommonShaderVariables::addTimeDelta(float timeDelta){
		mElapsedTime += timeDelta;
	}


}
