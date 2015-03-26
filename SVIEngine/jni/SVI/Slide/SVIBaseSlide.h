#ifndef __SVI_BASE_SLIDE_H_
#define __SVI_BASE_SLIDE_H_

#include <list>
#include "../SVICores.h"
#include "../Mathematics/SVIVector4.h"
#include "../Utils/SVIMutex.h"

namespace SVI {

	class SVIAnimation;
	class SVIBaseSlide;
	class SVIParticleEffect;
	class SVIGLSurface;
	//class SVIMutex;

	typedef std::list<SVIBaseSlide*>					BaseSlideList;
	typedef BaseSlideList::iterator					BaseSlideListIter;


	enum SlideType { BASE_SLIDE=0, IMAGE_SLIDE, TEXT_SLIDE };
	
	enum SlideLightType { NO_LIGHT = 0, SPOT_LIGHT, LINEAR_LIGHT };
	
	//2011-11-03 masterkeaton27@gmail.com
	enum SlideRTEffectType { RT_NONE = 0, RT_RIPPLE, RT_PULSE, RT_DREAMVISION, RT_WIZZLE, RT_PAPERFOLD, RT_MAGNIFY, RT_GLASSTILE, RT_ZOOMBLUR, RT_DIRECTIONBLUR };
	
	enum SlideZOrderType { BOTTOM = 0, NORMAL, TOPMOST };


	//2012-06-20 masterkeaton27@gmail.com
	//two deform types are added. (pannel / brick for depth shaped quad)

    //2012-11-19 masterkeaton27@gmail.com 
    //dome deform type added.
	enum SlideDeformType { DEFORM_NONE = 0,
		DEFORM_TWIST        = 1,
		DEFORM_EXPLODE      = 2,
		DEFORM_PANNEL       = 3,
		DEFORM_BRICK        = 4,
        DEFORM_DOME         = 5,
        DEFORM_DRUM         = 6,
        DEFORM_CONECURVE    = 7, //2012-12-11 masterkeaton27@gmail.com- venkat's
        DEFORM_PATH         = 8 //2013-06-21 masterkeaton27@gmail.com
	};

    //2013-06-26 masterkeaton27@gmail.com
    enum SlideFilterType { FILTER_NONE = 0, 
        FILTER_SKETCH = 1, 
        FILTER_OIL_PAINT = 2, 
        FILTER_WATERCOLOR = 3,
        FILTER_CARTOON = 4,
        FILTER_RADIAL = 5,
        FILTER_MONOTONE = 6,
        FILTER_SMOKE = 7
    };

    //2012-12-06 rareboy0112@facebook.com
    //trickle morphing type added.
    enum SlideMorphingType
    {
        MORPHING_NONE = 0,
        MORPHING_TRICKLE = 1,
    };

	//2012-06-21 masterkeaton27@gmail.com
	//add new shader effect for mobile office
	enum SlideSwipeType { SWIPE_NONE = 0, 
		SWIPE_LINEAR, 
		SWIPE_RADIAL,
		SWIPE_LINEAR_REVERSE, 
		SWIPE_RADIAL_REVERSE,
	};

	enum SlideDeformDirection {DEFORM_HORIZONTAL = 0, DEFORM_VERTICAL, DEFORM_HORIZONTAL_REVERSE, DEFORM_VERTICAL_REVERSE};
	//2011-07-01 masterkeaton27@gmail.com
	//2012-04-18 masterkeaton27@gmail.com
	enum SlideImageScaleType { 
		MATRIX = 0, 
		FIT_XY,
		FIT_START,
		FIT_CENTER,
		FIT_END,
		CENTER,
		CENTER_CROP,
		CENTER_INSIDE,
		CENTER_OUTSIDE,
	};

	enum SlideBlendType {BLEND_MULTIPLY = 0, BLEND_ADD, BLEND_ONE};

	enum PropertyChangeType {	
		REGION = 0,
		BOUNDS, //2011-06-20 masterkeaton27@gmail.com: this enum value may cause of size confusion of region
		POSITION,
		PIVOT_POINT,
		ZPOSITION,
		ROTATION,
		SCALE,
		BGCOLOR,
		CLIP_SUBSLIDE,
		OPACITY,
		HOLD_OPACITY, //2011-07-22 masterkeaton27@gmail.com: hold individual object's opacity
		HIDDEN,
		CORNER_RADIUS,
		BORDER_WIDTH,
		BORDER_COLOR,

		SHADOW_COLOR,
		SHADOW_RADIUS,
		SHADOW_OPACITY,
		SHADOW_OFFSET,

		TEXTURE_REGION, //2011-08-29 masterkeaton27@gmail.com: for sprite texture
		BACKFACE_TEXTURE_REGION, //2012-01-06 masterkeaton27@gmail.com: for backface texture region

		LIGHT_TYPE, //2011-08-26 masterkeaton27@gmail.com: per-slide light effect properties
		LIGHT_COLOR,
		LIGHT_RADIUS,
		LIGHT_OFFSET,
		LIGHT_OPACITY,
		LIGHT_ANGLE,
		LIGHT_POWER, //2011-10-14 masterkeaton27@gmail.com
		
		TEXTURE_SCALEFIT_REGION, //2011-10-11 masterkeaton27@gmail.com
		SWIPE_RATIO, //2012-06-21 masterkeaton27@gmail.com: new shader effect to mobile office
		DEFORM_RATIO, //2011-11-21 masterkeaton27@gmail.com
		RTE_RATIO, //2012-01-12 phyuzion@gmail.com
		DEFORM_TYPE, //2011-11-18 masterkeaton27@gmail.com
		DEFORM_DIRECTION,//2012-02-07 masterkeaton27@gmail.com
		SWIPE_TYPE, //2012-06-21 masterkeaton27@gmail.com: new shader effect to mobile office
		ZORDER_TYPE, 
		IMAGESCALE_TYPE, 
		BLEND_TYPE, //2011-08-01 masterkeaton27@gmail.com: blend type for SDS request.
		ANTI_ALIASING,	//2011.12.09 jongchae.moon
		ORTHOGONAL, //2012-11-26 masterkeaton27@gmail.com
        CUSTOM_RTE, //2012-01-03 phyuzion@gmail.com
		HOLD_SCALE, //2011-10-13 masterkeaton27@gmail.com: scaling tranformation
		SET_CAPTURE,
		RT_EEFECT_TYPE, //2011-11-03 masterkeaton27@gmail.com

		MODEL_POSITION, // 2011/11/30 rareboy0112@facebook.com
		MODEL_ROTATION, // 2011/11/30 rareboy0112@facebook.com
		MODEL_SCALE, // 2011/12/01 rareboy0112@facebook.com

        LIGHT_DIRECTION, //2012-11-21 masterkeaton27@gmail.com
		VIEW_DIRECTION, //2012-11-27 masterkeaton27@gmail.com

		PARTICLE_EFFECT, // 2012/06/21 rareboy0112@facebook.com
        USENORMALMAP, //2012-11-26 masterkeaton27@gmail.com

        ADD_PATH_POINT, //2013.06.21 masterkeaton27@gmail.com
        CLEAR_PATH_POINT, //2013.06.21 masterkeaton27@gmail.com

        FILTER_TYPE, //2013-06-26 masterkeaton27@gmail.com

        TESSELATION, //2012/12/08 rareboy0112@facebook.com: VI Effect
        MORPHING_TYPE, //2012/12/08 rareboy0112@facebook.com: VI Effect
        MORPHING_TIME, //2012/12/10 rareboy0112@facebook.com: VI Effect
        MORPHING_RADIUS, //2012/12/10 rareboy0112@facebook.com: VI Effect
        MORPHING_VERTEX_COUNT, //2012/12/21 rareboy0112@facebook.com: VI Effect
        MORPHING_COLOR, //2012/12/21 rareboy0112@facebook.com: VI Effect

		RESET_ALL,
		CHAGNE_PROPERTY_COUNT};

	class SVIBaseSlide {

		typedef std::list<SVIParticleEffect*>	ParticleEffectList;
	
	public:
		SVIBaseSlide(SVIGLSurface* surface);
		SVIBaseSlide(const SVIBaseSlide& rhs);
		virtual ~SVIBaseSlide();

		SVIBaseSlide&	operator = (const SVIBaseSlide& rhs);
		SVIBool	operator == (const SVIBaseSlide& rhs);

	protected :
		void				clearSubSlides();
		void				copy(const SVIBaseSlide& rhs);
		void				copyBaseProperties(const SVIBaseSlide& rhs);
		BaseSlideListIter	findSlideIter(SVIBaseSlide* slide);
		BaseSlideListIter	findSlideIter(SVIInt index);

		virtual void		addChangePropery(PropertyChangeType type){}

		void				updateGeometryByRegion();
		void				updateGeometryByBoundsOrPosition();
		void				updateGeometryByPivotPoint();
		void				setParent(SVIBaseSlide* parent) { mSuperSlide = parent; } 

	public:
		SVIBaseSlide*		getParent() { return mSuperSlide; }
		BaseSlideList&		getSubSlides() { return mSubSlideList; }
		SVIInt				getSubSlideCount() { return (SVIInt)mSubSlideList.size(); }

		SVIBool				addSubSlide(SVIBaseSlide* slide);
		SVIBool				removeSubSlide(SVIBaseSlide* slide);
		void				removeFromSuperSlide();
		SVIBool				insertSubSlide(SVIInt index, SVIBaseSlide* newSlide);
		SVIBool				replaceSubSlide(SVIBaseSlide* oldSlide, SVIBaseSlide* newSlide);
		SVIInt				getSlideIndex(SVIBaseSlide* subSlide);

		virtual SVIBool		addAnimation(SVIAnimation* animation)=0;
		virtual void		removeAnimation()=0;

		void				setRegion(const SVIRect& region);
		SVIRect				getRegion();

		void				setBounds(const SVIRect& bounds);
		SVIRect				getBounds();

		void				setPosition(const SVIPoint& position);
		SVIPoint				getPosition();

		void				setPivotPoint(const SVIPoint& pivotPoint);
		SVIPoint				getPivotPoint();

		void				setZPosition(const SVIFloat& zPosition);
		SVIFloat				getZPosition();

		void				setRotation(const SVIVector4& rotation);
		SVIVector4			getRotation();

		void				setScale(const SVIVector4& scale);
		SVIVector3			getScale();

		void				setBgColor(const SVIColor& color);
		SVIColor				getBgColor();

		void				setClipSubSlide(SVIBool clipSubSlide);
		SVIBool				getClipSubSlide();

		void				setOpacity(SVIFloat opacity);
		SVIFloat				getOpacity();
	

		void				setCornerRadius(SVIFloat radius);
		SVIFloat				getCornerRadius();

		void				setBorderWidth(SVIFloat width);
		SVIFloat				getBorderWidth();

		void				setBorderColor(const SVIColor& color);
		SVIColor				getBorderColor();

		void				setShadowColor(const SVIColor& color);
		SVIColor				getShadowColor();

		void				setShadowRadius(SVIFloat radius);
		SVIFloat				getShadowRadius();

		void				setShadowOffset(const SVIPoint& offset);
		SVIPoint				getShadowOffset();

		void				setShadowOpacity(const SVIFloat& opacity);
		SVIFloat				getShadowOpacity();


		void				setLightOpacity(const SVIFloat& opacity);
		SVIFloat				getLightOpacity();

		void				setLightColor(const SVIColor& color);
		SVIColor				getLightColor();

		void				setLightOffset(const SVIPoint& offset);
		SVIPoint				getLightOffset();

		void				setLightType(const SVIInt& type); 
		SVIInt				getLightType();

		void				setLightRadius(const SVIFloat& radius);
		SVIFloat				getLightRadius();

		void				setLightAngle(const SVIFloat& angle);
		SVIFloat				getLightAngle();

		void				setLightPower(const SVIFloat& power);
		SVIFloat				getLightPower();

		void				setLightDirection(const SVIVector4& direction);
		SVIVector4			getLightDirection();

		void				setViewDirection(const SVIVector4& direction);
		SVIVector4			getViewDirection();

		virtual void		setDeformDirection(SlideDeformDirection direction);
		SlideDeformDirection getDeformDirection();

		virtual void		setSwipeType(SlideSwipeType type);
		SlideSwipeType		getSwipeType();

		virtual void		setDeformType(SlideDeformType type);
		SlideDeformType		getDeformType();

        //2012/12/08 rareboy0112@facebook.com
        //added for VI effect
        void				setMorphingTime(const SVIFloat &aTime);
        SVIFloat             getMorphingTime();

        SVIBool getTesselation();
        void setTesselationPointList(const std::vector<SVIVector2>& pointList);
        std::vector<SVIVector2>& getTesselationPointList();

        virtual void		setMorphingType(SlideMorphingType type);
        SlideMorphingType	getMorphingType();

        virtual void		setMorphingRadius(const SVIFloat radius);
        SVIFloat		        getMorphingRadius();

        virtual void		setMorphingVertexCount(SVIInt gridCount);
        SVIInt		        getMorphingVertexCount();

        virtual void		setMorphingColor(SVIColor morphingColor);
        SVIColor		        getMorphingColor();

		virtual void		setDeformRatio(const SVIVector3& ratio);
		SVIVector3			getDeformRatio();

		//2012-06-21 masterkeaton27@gmail.com
		//added for mobile office
		virtual void		setSwipeRatio(const SVIVector3& ratio);
		SVIVector3			getSwipeRatio();

		virtual void		setRTERatio(const SVIVector3& ratio);
		SVIVector3			getRTERatio();
		
		virtual void		setZOrderType(SlideZOrderType type);
		SlideZOrderType		getZOrderType();

		virtual void		setBlendType(SlideBlendType type);
		SlideBlendType		getBlendType();

		virtual void		setAntiAliasing(SVIBool enableAA);
		SVIBool				getAntiAliasing();


		virtual void		setRTEffectType(SlideRTEffectType type);
		SlideRTEffectType	getRTEffectType();

		//2011-07-01 masterkeaton27@gmail.com
		//2011-10-10 image scaled type finally added.
		virtual void		setImageScaleType(SlideImageScaleType type);
		SlideImageScaleType getImageScaleType();

		virtual void		setOrthogonal(SVIBool isOrtho);
		SVIBool				getOrthogonal();

        //2012-11-26 masterkeaton27@gmail.com
        virtual void		setUseNormalMap(SVIBool useNormalMap);
        SVIBool				getUseNormalMap();
        
		//2012-01-03 phyuzion@gmail.com
		virtual void		setCustomRTE(SVIBool isCusRTE);
		SVIBool				getCustomRTE();

		virtual void		setHidden(SVIBool hidden);
		SVIBool				getHidden();

		//2011-07-25 masterkeaton27@gmail.com
		virtual void		setHoldOpacity(SVIBool hold);
		SVIBool				getHoldOpacity();

		//2011-10-13 masterkeaton27@gmail.com
		virtual void		setHoldScale(SVIBool hold);
		SVIBool				getHoldScale();

		//2011-10-18 masterkeaton27@gmail.com
		virtual void		setCapture(SVIBool capture);
		SVIBool				getCapture();

		//2011-08-29 masterkeaton27@gmail.com
		void				setTextureRegion(const SVIRect& region);
		SVIRect				getTextureRegion();

		//2012-01-06
		void				setBackFaceTextureRegion(const SVIRect& region);
		SVIRect				getBackFaceTextureRegion();

		//2011-10-11 masterkeaton27@gmail.com
		void				setTextureScaleFitRegion(const SVIRect& region);
		SVIRect				getTextureScaleFitRegion();
				
		// 2011/11/30 rareboy0112@facebook.com
		void				setModelPosition(const SVIVector3 &position);
		SVIVector3			getModelPosition();

		void				setModelRotation(const SVIVector4 &rotation);
		SVIVector4			getModelRotation();

		void				setModelScale(const SVIVector3 &scale);
		SVIVector3			getModelScale();

		SVIGLSurface* 	getSVIGLSurface();

		// 2012/06/21 rareboy0112@facebook.com
		// add particle effect
		void					addParticleEffect(SVIParticleEffect* effect);
		ParticleEffectList& getParticleEffectList() { return mParticleEffectList; }


        //2013-06-26 masterkeaton27@gmail.com
        void                setFilterType(SlideFilterType type);
        SlideFilterType     getFilterType();

		void				printInfo();

	protected:
		long long			mMemHack;
		SVIBool				mOrthogonal;
        SVIBool				mCustomRTE;
       	SVIBool              mUseNormalMap;

        SVIMutex*			mLock;
        BaseSlideList		mSubSlideList;
        SVIBaseSlide*		mSuperSlide;

        SVIRect				mRegion;
        SVIRect				mBounds;
        SVIPoint				mPosition;
        SVIPoint				mPivotPoint;
        SVIFloat				mZPosition;
        SVIVector4			mRotation;
        SVIVector3			mScale;

        SVIColor				mBgColor;
        SVIBool				mClipSubSlide;
        SVIFloat				mOpacity;
        SVIBool				mHoldScale;
        SVIBool				mCapture;
		SVIBool				mHoldOpacity;
		SVIBool				mHidden;
		SVIFloat				mCornerRadius;
		
		SVIFloat				mBorderWidth;
		SVIColor				mBorderColor;

		SVIColor				mShadowColor;
		SVIFloat				mShadowOpacity;
		SVIFloat				mShadowRadius;
		SVIPoint				mShadowOffset;
		
		SVIFloat				mLightRadius;
		SVIFloat				mLightOpacity;
		SVIFloat				mLightAngle;
		SVIFloat				mLightPower;
		SVIPoint				mLightOffset;
		SVIInt				mLightType;
		SVIColor				mLightColor;
       	SVIVector4           mLightDirection;
		SVIVector4           mViewDirection;

		SVIRect				mTextureBackFaceSpriteRegion;
		SVIRect				mTextureSpriteRegion;
		SVIRect				mTextureScaleFitRegion;

		SVIVector3			mSwipeRatio;
		SVIVector3			mDeformRatio;
		SVIVector3			mRTERatio;

		SlideRTEffectType	mRTEffectType;
		SlideBlendType		mBlendType;
		SlideZOrderType		mZOrderType;
		SlideImageScaleType mImageScaleType;
		
		SlideSwipeType		mSwipeType;
		SlideDeformType		mDeformType;
		SlideDeformDirection mDeformDirection;

        //2013-06-26 masterkeaton27@gmail.com
        SlideFilterType     mFilterType;

        //2012/12/08 rareboy0112@facebook.com
        SVIBool                  mTesselation;
        std::vector<SVIVector2>  mTesselationPointList;
        SlideMorphingType       mMorphingType;
        SVIFloat                 mMorphingTime;
        SVIFloat                 mMorphingRadius;
        SVIInt                   mMorphingVertexCount;
        SVIColor                 mMorphingColor;

		SVIVector3			mModelPosition;
		SVIVector4			mModelRotation;
		SVIVector3			mModelScale;

		//2011.12.09 jongchae.moon
		SVIBool				mEnableAA;
		//

		ParticleEffectList	mParticleEffectList;

		protected:
					SVIGLSurface* mSVIGLSurface;
	};
};

#endif
