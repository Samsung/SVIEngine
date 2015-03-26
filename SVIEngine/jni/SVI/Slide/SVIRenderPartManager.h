#ifndef __SVI_RENDER_PART_MANAGER_H_
#define __SVI_RENDER_PART_MANAGER_H_

#include "../SVICores.h"
#include "SVISlide.h"
#include "SVIBeamedSlide.h"
#include "SVIProjectionSlide.h"

#include "../TestBedDelegator/TestBedDelegator.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <map>
#include <set>
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

	class TestBedDelegator;

	typedef std::vector<SVIBeamedSlide*>				UpdatedBeamedSlideList;
	typedef std::vector<SVIImage*>					SVIImageList;
	typedef std::vector<SVIAnimation*>				AnimationList;
	typedef std::map<SVIAnimation*, SVIAnimation*>	AnimationMap;
	typedef std::pair<SVIAnimation*, SVIAnimation*>	AnimationPair;
	typedef std::vector<SVIUInt>						SVIUIntList;
	typedef std::list<SVISlide*>						AnimationSlideList;
	typedef std::map<SVIInt, SVISlide*>				SVISlideMap;

	class SVIRenderPartManager : public SVIAnimationListener{

		public :

		 SVIRenderPartManager(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		virtual ~SVIRenderPartManager();
	private:

		void								setTime();		
		void								handleMessage();

		//2012-05-23 masterkeaton27@gmail.com
		void handleRunSlideAnimation();
		SVIRenderPartManager(const SVIRenderPartManager& rhs);
		 void operator=(const SVIRenderPartManager& rhs);
		
	public:
		SVIBool								removeAnimationFromAnimationList(SVIAnimation* animation);

        //2013-03-15 rareboy0112@facebook.com
        //preserve glContext mode need call forceEndAllAnimation when onPause.
        void								forceEndAllAnimation();
		void								addUpdatedSlide(SVIBeamedSlide* slide);
		void								removeUpdatedSlideList(SVIBeamedSlide* slide);
		void								clearUpdateSlideList();
		void								checkUpdateSlideList();

		void								addAnimation(SVIAnimation* animation);
		void								removeAnimation(SVIAnimation* animation);
		void								checkAnimationList();
		void								clearAllAnimationList();
		
		SVIImage*							getImage(SVIInt idx);
		SVIInt								getImageCount();
		
		/**
		* Restores currently used textures to the present gl context.
		*/
		void 								restoreImages();

		void								addTestImage(SVIImage* image);

		void								drawFrame(SVIBool updateMsg);
		void								pause();
		void								resume();
		void								configurationChanged(SVIInt rotation);
		void								resize(SVIInt w, SVIInt h, SVIInt rotation);
		void								create();
		void								touchEvent(SVIInt type, SVIFloat x, SVIFloat y);
		void								requestRender();
		SVIImageList&						getImageList() { return mImageList; }
		void								setRootSlide(SVISlide* slide);
		void								setRootBeamedSlide(SVIBeamedSlide* root) { mRootBeamedSlide = root; }
		SVISlide*							getRootSlide() { return mRootSlide; }
		void								addAnimationSlide(SVISlide* slide);
		void								removeAnimationSlide(SVISlide* slide);
		void								swapAnimationSlide();
		void								clearAnimationSlide();
		SVISize								getSize();

		float								getFPS(){ return mFPS;}
		void								showFPSEnable(SVIBool enable){ mEnableShowFPS = enable;}
		void								logFPSEnable(SVIBool enable){ mEnableLogFPS = enable;}

		void								testBedEnable(SVIBool enable) { mTestBedEnable = enable; }
		void								removeAllRelatedToSlide(SVISlide* slide);
		SVIBool								isAnimating();
		void								enableAinmation(SVIBool	enableAnimation) { mEnableAnimation = enableAnimation; }
		SVIBool								isEnableAinmation() { return mEnableAnimation; }
        //2013-04-11 rareboy0112@facebook.com
        //why have return type?
		SVIBool								enableTranslucent(SVIBool enableTranslucent){mEnableTranslucent = enableTranslucent; return SVITRUE; }
		SVIBool								isTranslucent(){return mEnableTranslucent;}
        //2013-04-11 rareboy0112@facebook.com
        //why have return type?
        SVIBool								enableUpdateListener(SVIBool enableUpdateListener){mEnableUpdateListener = enableUpdateListener; return SVITRUE; }
		void								setImageHandled(SVIBool setImageHandled) { mIsHandledSetImage = mIsHandledSetImage || setImageHandled; }
		void								addMessageHandledSlide(SVISlide* slide);
		void								removeMessageHandledSlide(SVISlide* slide);
        

		void								setWallpaperMode(SVIBool value){ mIsWallpaper = value;}
		SVIBool								getWallpaperMode(){return mIsWallpaper;}

		void								setExternalMode(SVIBool value){ mIsExternal = value;}
		SVIBool								getExternalMode(){return mIsExternal;}

		void setAssetManager(AAssetManager *mgr) { mAssetManager = mgr; }
		AAssetManager* getAssetManager() { return mAssetManager; }

	public:
		//test code
		void onAnimationEnd(SVIAnimation* animation);
		void onAnimationRepeat(SVIAnimation* animation);
		void onAnimationStart(SVIAnimation* animation);

	private:
		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support
		SVIBool								mEnableUpdateListener;
		AAssetManager						*mAssetManager;
		UpdatedBeamedSlideList				mUpdatedSlideList;
		SVIImageList							mImageList;
		AnimationList						mPlayingAnimationList;
		AnimationList						mPlayingAnimationListBuf;
		AnimationList						mStopAnimationList;
		AnimationList						mStopAnimationListBuf;

		TestBedDelegator*					mTestBedDelegator;
		SVIBool								mTestBedEnable;
		SVIInt								mWidth;
		SVIInt								mHeight;
		SVIFloat								mFPS;

		SVIUInt								mDrawTime;
		SVIUInt								mDeltaTime;
		SVIUIntList							mElaspedDrawTimeList;
		SVIBool								mEnableEffect;
		SVISlide*							mRootSlide;
		SVIBeamedSlide*						mRootBeamedSlide;
		AnimationSlideList					*mAnimationSlideList;
		AnimationSlideList					*mAnimationSlideListBuf;
		SVISlideMap							mMessageHandledSlideMap;

		SVIMutex*							mAnimationSlideLock;
		SVIMutex*							mRenderPartMainLock;

		SVIBool								mEnableAnimation;
		SVIBool								mEnableShowFPS;
		SVIBool								mEnableLogFPS;
		SVIBool								mEnableTranslucent;
        SVIBool                              mEnablePostEffect;


		//2012-05-23 masterkeaton27@gmail.com
		//to prevent after pause SEGV error.
		SVIBool								mIsPaused;

		SVIBool								mIsWallpaper;
		SVIBool								mIsExternal;
		SVIBool								mIsHandledSetImage;


		std::vector<float>	g_FPSList;
		float				g_FPSMax;
		float				g_FPSMin;
		int					g_FPSCheckCount;

		SVIVector2 previousPos;
	};

	class SVIMessageHandler {
		typedef std::vector<SVIMessage>		MessageQueue;
		typedef std::set<SVIMessage>			MessageQueueSet;
	
	public:
		enum { 
			SM_SET_ROOT_SLIDE = 0,
			SM_ADD_SLIDE,
			SM_INSERT_SLIDE,
			SM_REMOVE_SLIDE,
			SM_FREE_SLIDE,
			SM_SET_REGION,
			SM_SET_BOUND,
			SM_SET_POSITION,
			SM_SET_PIVOT_POINT,
			SM_SET_ZPOSITION,
			SM_SET_ROTATION,
			SM_SET_SCALE,
			SM_SET_BG_COLOR,
			SM_SET_OPACITY,
			SM_SET_CORNER_RADIUS,
			SM_SET_BORDER_WIDTH,
			SM_SET_BORDER_COLOR,
			SM_SET_SHADOW_RADIUS,
			SM_SET_SHADOW_COLOR,
			SM_SET_SHADOW_OPACITY,
			SM_SET_SHADOW_OFFSET,
			SM_SET_TEXTURE_REGION,
			SM_SET_BACKFACE_TEXTURE_REGION,
			SM_SET_CLIP_SUBSLIDE,
			SM_SET_HIDDEN,
			SM_SET_ZORDER_TYPE,
			SM_SET_SWIPE_TYPE,
			SM_SET_DEFORM_TYPE,
			SM_SET_DEFORM_DIRECTION,
			SM_SET_ORTHOGONAL,
			SM_SET_CUSTOM_RTE,
			SM_SET_HOLD_OPACITY,
			SM_SET_HOLD_SCALE,
			SM_SET_CAPTURE,
			SM_SET_BLEND_TYPE,
			SM_SET_ANTI_ALIASING, //2011.12.09 jongchae.moon
			SM_SET_RT_EFFECT_TYPE,
			SM_SET_IMAGESCALE_TYPE,
			SM_SET_IMAGESCALE_REGION,
			SM_SET_LIGHT_TYPE,
			SM_SET_LIGHT_RADIUS,
			SM_SET_LIGHT_COLOR,
			SM_SET_LIGHT_OPACITY,
			SM_SET_LIGHT_ANGLE,
			SM_SET_LIGHT_OFFSET,
			SM_SET_LIGHT_POWER,
			SM_SET_IMAGE,
            SM_SET_NORMAL_IMAGE,
            SM_SET_SPECULAR_IMAGE,
			SM_SET_BACKFACE_IMAGE,
			SM_SET_BACKFACE_IMAGE_FLIP,
			SM_SET_BACKFACE_IMAGE_CLEAR,
			SM_FREE_IMAGE,
			SM_ADD_EXPLICIT_ANIMATION,
			SM_STOP_ANIMATION,
			SM_SET_ROTATION_FROM,
			SM_SET_SWIPE_RATIO,
			SM_SET_DEFORM_RATIO,
			SM_SET_DEFORM_TOUCH_DOWN,
			SM_SET_RTE_RATIO,
			SM_SET_MODEL_POSITION,
			SM_SET_MODEL_ROTATION,
			SM_SET_MODEL_SCALE,
            SM_SET_LIGHT_DIRECTION,
			SM_SET_VIEW_DIRECTION,
			SM_ADD_PARTICLE_EFFECT,
            SM_SET_USENORMALMAP,
            SM_SET_USEPOSTEFFECT,
            SM_SET_POSTEFFECT_TYPE,
            SM_SET_RIPPLE_WAVE,
            SM_SET_RIPPLE_ENVMAP,
            SM_SET_RIPPLE_INFO,
            SM_SET_RIPPLE_RESET,
            SM_ADD_PATH_POINT,      //2013-06-21 masterkeaton27@gmail.com
            SM_CLEAR_PATH_POINTS,   //2013-06-21 masterkeaton27@gmail.com
            SM_SET_FILTER_TYPE,     //2013-06-26 masterkeaton27@gmail.com
            SM_SET_TESSELATION,
            SM_SET_MORPHING_TYPE,
            SM_SET_MORPHING_RADIUS,
            SM_SET_MORPHING_GRID_COUNT,
            SM_SET_MORPHING_COLOR,
			SM_MSG_COUNT
		};

		static const SVIChar* mMessageString[];

		SVIMessageHandler(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		~SVIMessageHandler();

		static const SVIChar* getMessageString(const SVIInt& msgID);
		void sendMessage(SVIMessage& msg);
		void removeMessage(SVISlide* slide);
		void swapMessage();
		void clearAllMessage();
		void handleMessage();

		

	private:
		SVIMessageHandler();
		void messageProc(const SVIMessage& msg);
		void removeMessage(MessageQueue& msgQueue, SVISlide* slide);
		SVIMessageHandler(const SVIMessageHandler& rhs);
		 void operator=(const SVIMessageHandler& rhs);

	private :
		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support
		SVIMutex*					mLock;
		MessageQueue				mMessageQueue;
		MessageQueue				mMessageQueueBuf;
		SVIUInt						mMessageCounter;
	};
};

#endif //__SVI_RENDER_PART_MANAGER_H_
