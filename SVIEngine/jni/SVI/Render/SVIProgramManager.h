#pragma once

namespace SVI{
	/* note
	2011-09-27 masterkeaton27@gmail.com
	refactoring program module to provide more efficiency 
	for program & handler management.

	2011-09-28 masterkeaton27@gmail.com
	TODO: let's consider how external engineers can add their custom shaders into this module.
	*/

#define SVI_USE_POST_EFFECT 1


	enum SVIProgramHandleType{
		HD_U_VIEW = 0,			//M16 : matrix 16
		HD_U_INV_VIEW,			//M16 : matrix 16
		HD_U_PROJ,				//M16 : matrix 16
		HD_U_NORMAL,			//M16 : matrix 16 
		HD_U_EYEPOS,			//V04 : x,y,z,w	
		HD_A_POSITION,			//V04 : x,y,z,w
		HD_A_NORMAL,			//V04 : x,y,z,w
		HD_A_TEXCOORD,			//V02 : u,v
        HD_A_HEIGHTS,           //V04 : x,y,z,w //2013-01-30 masterkeaton27@gmail.com 
		HD_U_COLOR,				//C04 : r,g,b,a
		HD_U_SVIMPLER_0,			//Texture Sampler 0
		HD_U_SVIMPLER_1,			//Texture Sampler 1
		HD_U_SVIMPLER_2,			//Texture Sampler 2
		HD_U_OPACITY,			//V01 : alpha
		HD_U_LIGHT_OFFSET,		//V02 : offset
		HD_U_LIGHT_TARGET,		//V02 : target (only for linear)
		HD_U_LIGHT_RADIUS,		//V02 : radius start / radius end
		HD_U_LIGHT_COLOR,		//C04 : r,g,b,a
		HD_U_LIGHT_POWER,		//V01 : power
        HD_U_LIGHT_DIRECTION,	//V04 : direction of light
		HD_U_VIEW_DIRECTION,	//V04 : direction of view
		HD_U_SHADOW_OFFSET,		//V02 : offset
		HD_U_SHADOW_RADIUS,		//V02 : radius start / radius end
		HD_U_SHADOW_COLOR,		//C04 : r,g,b,a 
		HD_U_FITINFO, 			//V04 : offset uv / scale uv
		HD_U_TEXTUREINFO,		//V04 : offset uv / scale uv
		HD_U_TIME,				//V01 : time value
		HD_U_SCREEN_SIZE,		//V02 : screen size	
		HD_U_SIZE,				//V02 : slide size
		HD_U_PIVOT,				//V02 : slide pivot	
		
		HD_U_AMBIENT,			//C04 : r,g,b,a
		HD_U_DIFFUSE,			//C04 : r,g,b,a
		HD_U_SPECULAR,			//C04 : r,g,b,a
		HD_U_SHININESS,			//V01 : power

		HD_U_RESERVE01,			//V04 : reserve variable #1	
		HD_U_RESERVE02,			//V04 : reserve variable #2
		HD_U_RESERVE03,			//V04 : reserve variable #3	
		HD_U_RESERVE04,			//V04 : reserve variable #4	
		HD_U_RESERVE05,			//V04 : reserve variable #5	
		
		//2011.11.03 jongchae.moon
		HD_A_AACOORD,

		// 2011111 jbkim
		HD_U_NUM_GRADATION_COLORS,
		HD_U_GRADATION_COLORS,
		HD_U_GRADATION_OFFSETS,
		HD_U_GRADATION_START_POS,
		HD_U_GRADATION_END_POS,

		//2012-06-21 masterkeaton27@gmail.com
		//shader effect handle for swipe 
		HD_U_EDGE_RATIO,
		HD_U_EDGE_RADIUS,
		HD_U_LERP_WIDTH,
		HD_U_LERP_BASE,

	        //2012-12-17 masterkeaton27@gmail.com
	        HD_U_SVIMPLE_WEIGHT,
	        HD_U_SVIMPLE_OFFSET,

		HD_A_UNITCOORD,

		HD_COUNT 
	};

	

	enum SVIProgramType{
		//colour programs (4)
		PR_COLOR = 0,
		PR_COLOR_LT_SPOT,
		PR_COLOR_LT_LINEAR,
		PR_COLOR_SHADOW,

		//texture programs (8)
		PR_TEX_ADD,
		PR_TEX_ADD_LT_SPOT,
		PR_TEX_ADD_LT_LINEAR,
		PR_TEX_ADD_SHADOW,
		PR_TEX_MLT,	
		PR_TEX_MLT_LT_SPOT,
		PR_TEX_MLT_LT_LINEAR,	
		PR_TEX_MLT_SHADOW,

		//texture + shader based scale to fit programs (8)
		PR_TEX_ADD_FIT,
		PR_TEX_ADD_LT_SPOT_FIT,
		PR_TEX_ADD_LT_LINEAR_FIT,
		PR_TEX_ADD_SHADOW_FIT,
		PR_TEX_MLT_FIT,	
		PR_TEX_MLT_LT_SPOT_FIT,
		PR_TEX_MLT_LT_LINEAR_FIT,	
		PR_TEX_MLT_SHADOW_FIT,
		//2012-03-13 masterkeaton27@gmail.com
		PR_3DSHADOW,
#if SVI_USE_POST_EFFECT
		//2011-10-31 masterkeaton27@gmail.com
		//render target post effect added.
		PR_RIPPLE_RT,
		PR_PULSE_RT,
		PR_DREAMVISION_RT,
		PR_WIZZLE_RT,
		PR_PAPERFOLD_RT,
		PR_MAGNIFY_RT,
		PR_GLASSTILE_RT,
		PR_ZOOMBLUR_RT,
		PR_DIRECTIONBLUR_RT,
#endif
		PR_3DMODEL,
        //2012-11-21 masterkeaton27@gmail.com
        PR_3DMODEL_OFFSET,
		//2012-02-16 masterkeaton27@gmail.com
		PR_PHYSIC3DMODEL, //from SURC
		PR_PHYSIC3DMODELAA, //2012-04-09 masterkeaton27@gmail.com

		// 20111111 jbkim
		PR_LINEAR_GRADATION,
		PR_RADIAL_GRADATION,

		//2012-06-21 masterkeaton27@gmail.com
		//new shader effect for mobile office
		PR_LINEAR_SWIPE,
		PR_RADIAL_SWIPE,

        //2012-11-26 masterkeaton27@gmail.com
        //new shader for normal map calculation
        PR_NORMALMAP,       
        PR_SPECULAR_NORMALMAP,

        //2012-12-14 masterkeaton27@gmail.com
        PR_POST_HIGHLIGHT,
        PR_POST_MERGE,
        PR_POST_FINALE,
        PR_POST_BLUR,
        PR_POST_DEBUG,
        PR_POST_BLOOM,
        PR_POST_DOWN8X8,
        PR_POST_RIPPLE,
        //---------------------
        //2013-06-24 masterkeaton27@gmail.com
        PR_TEX_CPU,
        //2013-06-26 jrd.park
        PR_FILTER_SKETCH,
        PR_FILTER_OILPAINT,
        PR_FILTER_WATERCOLOR,
        PR_FILTER_CARTOON,
        PR_FILTER_RADIAL,
        PR_FILTER_MONOTONE,
        PR_FILTER_SMOKE,

        //2012/12/10 rareboy0112@facebook.com
        PR_TESSELATION,

        //---------------------
		PR_TYPE_COUNT // 35 programs
	};

	enum SVIProgramElementType{
		EM_COLOR = 0,
        //---------------------
        //2013-06-26 masterkeaton27@gmail.com
        EM_FILTER_SKETCH,
        EM_FILTER_OILPAINT,
        EM_FILTER_WATERCOLOR,
        EM_FILTER_CARTOON,
        EM_FILTER_RADIAL,
        EM_FILTER_MONOTONE,
        EM_FILTER_SMOKE,
        //2013-06-24 masterkeaton27@gmail.com
		EM_CPU_TEXTURE,
        //---------------------
        EM_TEXTURE,
		EM_TEXTURE_SCALE,
		EM_BLEND_ADD,
		EM_BLEND_MLT,
		EM_LIGHT_SPOT,
		EM_LIGHT_LINEAR,
		EM_SHADOW,
		//2012-03-13 masterkeaton27@gmail.com
		EM_SHADOW3D,
		EM_3DMODEL,		

        //2012/12/10 rareboy0112@facebook.com
        EM_TESSELATION,
		
        //2012-11-21 masterkeaton27@gmail.com
        EM_3DMODEL_OFFSET,
        
        //2012-11-26 masterkeaton27@gmail.com
        EM_NORMALMAP,
        EM_NORMALSPECULARMAP,

        //2012-02-16 masterkeaton27@gmail.comfrom SURC
		//2011-12-26 d.kovalenko
		EM_PHYSIC3DMODEL,
               

		//2012-04-09 masterkeaton27@gmail.com
		//page model anti-aliasing work-in progress
		EM_PAGE3D_AA,

		//2011-11-03 masterkeaton27@gmail.com
		//render target elements
		EM_RENDER_TARGET,

#if SVI_USE_POST_EFFECT
		EM_RT_RIPPLE,
		EM_RT_PULSE,
		EM_RT_DREAMVISION,
		EM_RT_WIZZLE,
		EM_RT_PAPERFOLD,
		EM_RT_MAGNIFY,
		EM_RT_GLASSTILE,
		EM_RT_ZOOMBLUR,
		EM_RT_DIRECTIONBLUR,
#endif
		//2011-11-11 jbkim 
		EM_LINEAR_GRADATION,
		EM_RADIAL_GRADATION,

		//2012-06-21 masterkeaton27@gmail.com
		//new elements for mobile office
		EM_LINEAR_SWIPE,
		EM_RADIAL_SWIPE,
		EM_TYPE_COUNT // 9 elements
	};

	class SVIGLSurface;

	class SVIProgramParams{
	public:
		SVIProgramParams();
		virtual ~SVIProgramParams();

	public:
		void reset();
		void addElement(SVIProgramElementType type);	
		SVIProgramType getProgram();	

	private:
		void clear();
		std::vector<int> mElements;
	};

	
	class SVIProgramHandler{
	public:
		SVIProgramHandler();
		SVIProgramHandler(SVIInt programID);
		virtual ~SVIProgramHandler();

	public:
		void link(SVIInt programID);	

	public:	
		SVIInt 	getHandle(SVIProgramHandleType type);
		SVIBool 	getHandleLinked(SVIProgramHandleType type);

	public:
		void 	setUniform(SVIProgramHandleType type, SVIMatrix& pValue);
		//2011-11-14 jbkim83  
		void 	setUniform(SVIProgramHandleType type, SVIInt count, SVIVector4* pValue);
		void 	setUniform(SVIProgramHandleType type, SVIInt count, SVIFloat* pValue);
		void 	setUniform(SVIProgramHandleType type, SVIInt value);

		void 	setUniform(SVIProgramHandleType type, SVIVector4& pValue);
		void 	setUniform(SVIProgramHandleType type, SVIVector3& pValue);
		void 	setUniform(SVIProgramHandleType type, SVIVector2& pValue);
		void 	setUniform(SVIProgramHandleType type, float x, float y);
		void 	setUniform(SVIProgramHandleType type, float x, float y, float z);
		void 	setUniform(SVIProgramHandleType type, float x, float y, float z, float w);
		void 	setUniform(SVIProgramHandleType type, SVIColor& refValue);
        void 	setUniform(SVIProgramHandleType type, SVIFloat pValue);
        void 	setUniform(SVIProgramHandleType type, SVIBool pValue);

        void 	setSampler(SVIProgramHandleType type, SVIUInt textureID);

        void 	setAttribute(SVIProgramHandleType type, SVIVector4* pValue, SVIUInt stride = 0);
        void 	setAttribute(SVIProgramHandleType type, SVIVector3* pValue, SVIUInt stride = 0);
        void 	setAttribute(SVIProgramHandleType type, SVIVector2* pValue, SVIUInt stride = 0);
        void	setAttribute(SVIProgramHandleType type, SVIInt primCount, SVIFloat* pValues, SVIUInt stride = 0);
        void	releaseAttribute(SVIProgramHandleType type);

    private:
        SVIInt mHandles[HD_COUNT];
		SVIInt	mProgramID;
	};



	class SVIProgram{
	public:
		SVIProgram(SVIGLSurface* surface);
		SVIProgram(SVIGLSurface* surface, SVIProgramType type, SVIBool isAutoLoad = SVIFALSE);
		virtual ~SVIProgram();

	public:



		
		void create();
		void create(SVIInt vertexShaderId);
		void restore();
		void destroy();	

	public:	
		void use();
		

	public:
		SVIProgramHandler * 				getHandler();
		SVIInt							getID();
		SVIBool							getActivated();
		SVIBool							getUseTexture();
        //2013-06-24 masterkeaton27@gmail.com
        SVIBool							getUseCPUTexture();
		SVIBool							getUseShadow();
		SVIBool							getUseNormal();
        SVIBool                          getTesselation();
        SVIBool                          getUseHeights();
		//2012-11-27 masterkeaton27@gmail.com
		SVIBool							getUse2DNormalMap();
		SVIInt							getType();

	public:
		void createShaderImmediate(); //2012-02-22 masterkeaton27@gmail.com

        //2013-01-25 masterkeaton27@gmail.com
        SVIBool saveShaderToBinary();
        SVIBool checkBinaryShaderExist();       
        void loadShaderFromBinary();
        void loadShaderFromRAM();

	protected:

       
		SVIInt createPresetShader();		
		SVIInt linkShader(SVIInt program);

	private:
		SVIProgramHandler * 			mHandler;
		SVIInt						mType;
		SVIBool						mActivated;

        //2013-02-04 masterkeaton27@gmail.com
        SVIBool                      mRAMBinary;
        
    private:
        GLvoid*                     mProgramBinary;
        GLint                       mProgramBinaryLength;
        GLenum                      mProgramBinaryFormat;

	private:
		SVIInt								mID;		
		SVIInt								mVertexShaderID;
		SVIInt								mFragmentShaderID;

	private:
		SVIGLSurface* mSVIGLSurface;

	};


	//2011-10-31 masterkeaton27@gmail.com
	//common variables for post effect shader
	class SVICommonShaderVariables{
	public:
		SVICommonShaderVariables();
		virtual ~SVICommonShaderVariables();
		
	public:
		void setInteraction(SVIVector2& position, SVIVector2& velocity);
		void setPosition(SVIVector2& position);
		void setVelocity(SVIVector2& velocity);
		void setScreenSize(SVIVector2& size);
		void setScreenSize(float width, float height);
		void setTime(float time);
		void addTimeDelta(float timeDelta);
	public:
		SVIVector2&	getPosition(){return mInteractionPosition;}
		SVIVector2  getNormalizedPosition();  
		SVIVector2&	getVelocity(){return mInteractionVelocity;}
		SVIVector2&	getScreenSize(){return mScreenSize;}
		float		getTime(){return mElapsedTime;}


	private:
		float		mElapsedTime;
		SVIVector2	mInteractionPosition;
		SVIVector2	mInteractionVelocity;
		SVIVector2	mScreenSize;
	};


	class SVIProgramManager{
	public:		

		SVIProgramManager(SVIGLSurface* surface);
		virtual ~SVIProgramManager();
	private:
		SVIProgramManager(const SVIProgramManager& rhs){}
		void operator=(const SVIProgramManager& rhs){}

	public:	
		void create();
		void update();
		void restore();
		void destroy();

	public:
		SVIProgramHandler * 	getHandler(SVIProgramParams & pParam);
		SVIProgramHandler * 	getHandler(SVIProgramParams * pParam);
		SVIProgram * 				getProgram(SVIProgramParams & pParam);
		SVIProgram * 				getProgram(SVIProgramParams * pParam);
		SVIProgram * 				getProgram(SVIProgramType type);

	 void restorePreLoadVS();
        void initializePreLoadVS();
        void finalizePreLoadVS();

	SVIInt compileShader( SVIInt shaderType, SVIPChar srcShader);
	SVIInt binaryShader( SVIInt shaderType, unsigned char *srcBinary, SVIUInt binarySize);


	public:
		SVIProgram * addCustomProgram(const SVIChar* pName,const SVIChar* pVertexSrc,const SVIChar* pFragmentSrc);
		SVIProgram * getCustomProgram(const SVIChar* pName);	
		SVIProgram * freeCustomProgram(const SVIChar* pName);	

	public:
		void use(SVIProgramParams & pParam);
		void use(SVIProgramParams * pParam);
		void use(SVIProgramType type);
	private:
		void createPresetShaders();

	private:
		SVIProgram * mPrograms[PR_TYPE_COUNT];
		std::vector<SVIProgram*> mCustomPrograms;
		SVIGLSurface* mSVIGLSurface;

	public:
		//2012-11-27 masterkeaton27@gmail.com
		//add two dimentionnal vertex shader for planner normal mapping
		SVIInt				DEFAULT_VERTEX_SHADER_TEXTURE_NORMALMAP;

        //2013-01-30 masterkeaton27@gmail.com
        SVIInt				DEFAULT_VERTEX_SHADER_HEIGHTS;

        
        //legacy shaders
		SVIInt				DEFAULT_VERTEX_SHADER_TEXTURE_NORMAL;
		SVIInt				DEFAULT_VERTEX_SHADER_TEXTURE;
        SVIInt				DEFAULT_VERTEX_SHADER_TEXTURE_TESSELATION;
		SVIInt				DEFAULT_VERTEX_SHADER_COLOR;
		SVIInt				DEFAULT_VERTEX_SHADER_COLOR_SHADOW;
		SVIInt				DEFAULT_VERTEX_SHADER_TEXTURE_SHADOW;
		SVIInt               DEFAULT_VERTEX_SHADER_CPU_TEXTURE;
		//2012-03-13 masterkeaton27@gmail.comshadow shader added for 3d shadow render. (which has texture coordinate)
		SVIInt				DEFAULT_VERTEX_SHADER_3D_SHADOW;
		SVIBool               mIsDefaultVertexShaderLoaded;

	};

	
};
