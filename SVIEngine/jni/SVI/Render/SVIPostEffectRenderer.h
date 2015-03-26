#pragma once
#include "SVIFrameRenderer.h"
#include "SVITexture.h"
#include "../BaseType/SVIExtendTypes.h"
#include "../GLSurface/SVIGLSurface.h"

namespace SVI{

    
    class SVIProgram;
    class SVIVector2;
	class SVITexture;


#define MAX_INTERACTION 4

    enum SVIPostEffectType{
        POST_EFFECT_BLOOM = 0,
        POST_EFFECT_RIPPLE
    };

    class SVIRippleRenderer{
    public:
        SVIRippleRenderer(SVIGLSurface* surface);
        ~SVIRippleRenderer();

    public:
        void Initialize();
        void update();
        void render(SVIUInt diffuseMap);
        void reset(float width, float height);
        void finalize();
                
        void reset();

        SVIBool isAnimating();

    public:
        void ripple(float mx, float my, float intensity);
        void setEnvImage(SVIImage* image);
        void setReductionRatio(float ratio);
        void setThreshhold(float t);

    protected:
        void processRippleInputs();

       
    private:    
        
        SVIVector3 * mVertices;
        SVIVector3 * mHeights;
        SVIUShort * mIndices;
        
        SVIFloat * mHeightMap;
        SVIFloat * mVelocityMap;

        SVIFloat   mReductionRatio;
        SVIFloat   mThreshhold;

        //2013-02-08
        SVITexture * mEnvTexture;
    private:
        SVIBool mStable;
        SVIBool mReseted;
        int mNumDetailWidth;
        int mNumDetailHeight;

        int mSurfaceDetailWidth;
        int mSurfaceDetailHeight;

        int mWidth;
        int mHeight;

        long mVerticesCount;
        long mIndicesCount;
        long mMaxCount;

        SVIMutex*		    mLock;

	SVIGLSurface* mSVIGLSurface;

        std::vector<SVIVector3> mRippleRequest;
    };


    class SVIPostEffectRenderer{
    public:
	 SVIPostEffectRenderer(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
        virtual ~SVIPostEffectRenderer();
        
    private:
	SVIPostEffectRenderer(const SVIPostEffectRenderer& rhs){}
	void operator=(const SVIPostEffectRenderer& rhs){}
    public:
        void initialize(float width, float height);
        void restore(float width, float height);
        void finalize();
        void update(float width, float height);

    public:

        void beginFrame();
        void endFrame();

        void drawPostRippleEffect();
        void drawPostProcess();
        void drawFinale();

        void setDebugDraw(SVIBool isDebugDraw);
        void setActivated(SVIBool isActivated);

        SVIBool getActivated(){return mActivated;}

    protected:
        void _drawRipple();

    protected:
        void renderQuad(SVIVector2 offset, SVIVector2 size,SVIProgram * pProgram, SVIUInt sampler0, SVIUInt sampler1, SVIUInt sampler2);
        void renderFullScreenQuad(SVIVector2 size,SVIProgram * pProgram, SVIUInt sampler0, SVIUInt sampler1, SVIUInt sampler2);

    private:
        SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support

        SVIVector2       mSize;
        SVICaptureData * mScene;


        SVIBool          mActivated;
        SVIBool          mDebugDraw;


        int             mInteractionCount;
        SVIVector4       mInteractionPoints[MAX_INTERACTION];  
    };	
	
};
