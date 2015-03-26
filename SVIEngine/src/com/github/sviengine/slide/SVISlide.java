/*
 * Copyright (C) 2011 SVI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.github.sviengine.slide;

import java.util.ArrayList;
import java.util.Vector;

import android.graphics.Bitmap;
import android.util.Log;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.InterpolatorType;
import com.github.sviengine.animation.SVIPropertyAnimation.PropertyAnimationType;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector2;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.basetype._PackageAccessBasetype;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.particle.SVIParticleEffect;
import com.github.sviengine.scene.SVISceneNode;


/**
 * slide class
 */

/**
 * @author rareboy0112@facebook.com
 */
public class SVISlide {
    /**
     * default implicit animation duration
     */
    public static final int IMPLICIT_ANIMATION_OFF_DURATION = 0x00000000;
    protected ArrayList<SVISlide> mListSlide;
    private static int rotationDuration = 0;


    private String mName = "SVISlide";
    private SVIRect mRegion;
    private SVIPoint mPivotPoint;
    private SVIPoint mPosition;
    private SVIVector3 mRotation;
    private SVIVector3 mSwipeRatio; //2012-06-21 masterkeaton27@gmail.com
    private SVIVector3 mDeformRatio;
    private SVIVector3 mRTERatio;
    private SVIVector3 mScale;
    private float mZPosition;

    private SVIColor mBgColor;
    private float mBorderWidth;
    private SVIColor mBorderColor;
    private boolean mClipSubSlide;
    private float mCornerRadius;
    private boolean mHidden;
    private boolean mCapture;
    private boolean mUseNormalMap;
    private boolean mIsOrthogonal;
    private float mOpacity;
    private SVIColor mShadowColor;
    private float mShadowOpacity;
    private float mShadowRadius;
    private SVIPoint mShadowOffset;

    //2011-10-11 masterkeaton27@gmail.com
    protected int mImageScaleType;
    protected SVIRect mImageScaleToFitRegion;

    //2013-06-26 masterkeaton27@gmail.com
    protected int mFilterType;
    
    //2011-08-26 masterkeaton27@gmail.com
    //light properties
    protected int mLightType;
    protected float mLightAngle;
    protected float mLightRadius;
    protected float mLightOpacity;
    protected float mLightPower;
    protected SVIPoint mLightOffset;
    protected SVIColor mLightColor;
    
    protected SVIVector4 mLightDirection;
    protected SVIVector4 mViewDirection;

    //2011-08-29 masterkeaton27@gmail.com
    protected SVIRect mTextureRegion;
    protected SVIRect mBackFaceTextureRegion;

    private int mDeformDirection;
    private int mDeformType;
    private int mMorphingType;
    private int mZOrderType;
    private int mSwipeType;
    
    private SVIImage mImage;
    private SVIImage mNormalImage;
    private SVIImage mSpecularImage;
    private SVIImage mBackFaceImage;

    private int mBlendType;
    private int mRTEffectType;

    //2011.12.09 jongchae.moon
    private boolean mEnableAntiAliasing;
    

	//2013-05-28 Ashish, Insu Song
    private boolean mHoldScale = false;
    private boolean mHoldOpacity = false;
    private boolean mEnableBackFaceImageFlip = false;
    private boolean mEnableBackFaceImageClear = false;

    private int mPriority;
    private SVISlide mParent;
    private SVIAnimation mAnimation;

    //2012.12.06 rareboy0112@facebook.com
    private Vector<SVIVector2> mTesselationPointList;
    private float mMorphingRadius;
    private int mMorphingVetexNum;
    private SVIColor mMorphingColor;
    
    protected SVIGLSurface mAttachedSurface = null;

    /**
     * slide type
     */
    public interface SlideType {
        public static final int BASE_SLIDE = 0;
        public static final int IMAGE_SLIDE = 1;
        public static final int TEXT_SLIDE = 2;
        public static final int SCROLL_SLIDE = 3;
    }

    /**
     * slide image scale type
     */
    public interface ImageScaleType {
        public static final int MATRIX = 0;
        public static final int FIT_XY = 1;
        public static final int FIT_START = 2;
        public static final int FIT_CENTER = 3;
        public static final int FIT_END = 4;
        public static final int CENTER = 5;
        public static final int CENTER_CROP = 6;
        public static final int CENTER_INSIDE = 7;
    }

    /**
     * slide render target effect type
     */
    public interface RTEffectType {
        public static final int RT_NONE = 0;
        public static final int RT_RIPPLE = 1;
        public static final int RT_PULSE = 2;
        public static final int RT_DREAMVISION = 3;
        public static final int RT_WIZZLE = 4;
        public static final int RT_PAPERFOLD = 5;
        public static final int RT_MAGNIFY = 6;
        public static final int RT_GLASSTILE = 7;
        public static final int RT_ZOOMBLUR = 8;
        public static final int RT_DIRECTIONBLUR = 9;
    }

    /**
     * slide light type
     */
    public interface LightType {
        public static final int NO_LIGHT = 0;
        public static final int SPOT_LIGHT = 1;
        public static final int LINEAR_LIGHT = 2;
    }

    /**
     * slide blend type
     */
    public interface BlendType {
        public static final int BLEND_MULTIPLY = 0;
        public static final int BLEND_ADD = 1;
        public static final int BLEND_ONE = 2; //2013-02-01 masterkeaton27@gmail.com
    }
    
    /**
     * slide Filter type
     */
    public interface FilterType {
        public static final int FILTER_NONE = 0;
        public static final int FILTER_SKETCH = 1;
        public static final int FILTER_OILPAINT = 2;
        public static final int FILTER_WATERCOLOR = 3; 
        public static final int FILTER_CARTOON = 4;
        public static final int FILTER_RADIAL = 5;
        public static final int FILTER_MONOTONE = 6;
        public static final int FILTER_SMOKE = 7;
    }

    /**
     * slide zorder type
     */
    public interface SlideZOrderType {
        public static final int BOTTOM = 0;
        public static final int NORMAL = 1;
        public static final int TOPMOST = 2;
    }

    /**
     * slide deformation type
     */
    public interface SlideDeformType {
        public static final int DEFORM_NONE = 0;
        public static final int DEFORM_TWIST = 1;
        public static final int DEFORM_EXPLODE = 2;
        public static final int DEFORM_PANNEL = 3;
        public static final int DEFORM_BRICK = 4;
        public static final int DEFORM_DOME = 5;
        public static final int DEFORM_DRUM = 6;
        public static final int DEFORM_CONECURVE = 7;
        public static final int DEFORM_PATH = 8;
    }

    /**
     * slide morphing type
     */
    public interface SlideMorphingType
    {
    	public static final int MORPHING_NONE = 0;
    	public static final int MORPHING_TRICKLE = 1;
    }
    
    /**
     * slide swipe type
     */
    public interface SlideSwipeType {
        public static final int SWIPE_NONE = 0;
        public static final int SWIPE_LINEAR = 1;
        public static final int SWIPE_RADIAL = 2;
    }

    /**
     * slide deformation direction type
     */
    public interface SlideDeformDirection {
        public static final int DEFORM_HORIZONTAL = 0;
        public static final int DEFORM_VERTICAL = 1;
        public static final int DEFORM_HORIZONTAL_REVERSE = 2;
        public static final int DEFORM_VERTICAL_REVERSE = 3;
    }

    /**
     * slide image format
     */
    public interface ImageFormat {
        public static final int NONE = 0;
        public static final int RGB_565 = 1;
        public static final int RGBA_8888 = 2;
    }
    
/*    private interface ImplicitAnimationSet {
        public static final int ISSETPROPERTY = 0;
        public static final int INTERPOLATORTYPE = 1;
        public static final int DURATION = 2;
        public static final int REPEATCOUNT = 3;
        public static final int OFFSET = 4;
        public static final int AUTOREVERSE = 5;
        public static final int SET_COUNT = 6;
    }*/

    /**
     * defulat color
     */
    private float[] defaultColor = {0f, 0f, 0f, 0f};
    private int mNativeSlide = -1;

    public static boolean mIsSuccessCreateSlide = false;
    
    public SVISlide() {
    	this(null);
    }
    
    public SVISlide(int type) {
    	this(null, type);
    }
    
    public SVISlide(int type, Bitmap bitmap) {
    	this(null, type, bitmap);
    }
    
    public SVISlide(int type, int parent, float x, float y, float width, float height, float[] color) {
    	this(null, type, parent, x, y, width, height, color);
    }
    public SVISlide(int type, int parent, float x, float y, float width, float height, float[] color, Bitmap bitmap) {
    	this(null, type, parent, x, y, width, height, color, bitmap);
    }

    /**
     * SVISlide default constructor
     */
    SVISlide(SVIGLSurface surface) {
    	mAttachedSurface = SVIGLSurface.getSurface(surface);
    	mAttachedSurface.incrementUsageCount(this);

        initialize(0.0f, 0.0f, 0.0f, 0.0f, defaultColor);
        mNativeSlide = nativeInitializeSVISlide(mAttachedSurface.getNativeHandle(), SlideType.BASE_SLIDE, 0, 0f, 0f, 0f, 0f, defaultColor, null);
        if (mNativeSlide != -1) {
            mIsSuccessCreateSlide = true;
        }
    }

    /**
     * SVISlide constructor
     *
     * @param type > set to native slide type(BASE_SLIDE=0, IMAGE_SLIDE=1, TEXT_SLIDE=2)
     */
    SVISlide(SVIGLSurface surface, int type) {
    	mAttachedSurface = SVIGLSurface.getSurface(surface);
    	mAttachedSurface.incrementUsageCount(this);

        initialize(0.0f, 0.0f, 0.0f, 0.0f, defaultColor);
        mNativeSlide = nativeInitializeSVISlide(mAttachedSurface.getNativeHandle(), type, 0, 0f, 0f, 0f, 0f, defaultColor, null);
        if (mNativeSlide != -1) {
            mIsSuccessCreateSlide = true;
        }
    }

    /**
     * SVISlide constructor
     *
     * @param type   > set to native slide type(BASE_SLIDE=0, IMAGE_SLIDE=1, TEXT_SLIDE=2)
     * @param bitmap > set bitmap information to native SVIImage side. bitArray of Bitmap will be referred in SVIImage native side.
     */
    SVISlide(SVIGLSurface surface, int type, Bitmap bitmap) {
    	mAttachedSurface = SVIGLSurface.getSurface(surface);
    	mAttachedSurface.incrementUsageCount(this);

    	initialize(0.0f, 0.0f, 0.0f, 0.0f, defaultColor);
        mNativeSlide = nativeInitializeSVISlide(mAttachedSurface.getNativeHandle(), type, 0, 0f, 0f, 0f, 0f, defaultColor, bitmap);
        if (mNativeSlide != -1) {
            mIsSuccessCreateSlide = true;
        }
    }

    /**
     * SVISlide constructor
     *
     * @param type   > set to native slide type(BASE_SLIDE=0, IMAGE_SLIDE=1, TEXT_SLIDE=2)
     * @param parent > set to native slide parent
     * @param x      > set to native slide x-axis offset
     * @param y      > set to native slide y-axis offset
     * @param width  > set to native slide width
     * @param height > set to native slide height
     * @param color  > set to native slide color
     */
    SVISlide(SVIGLSurface surface, int type, int parent, float x, float y, float width, float height, float[] color) {
    	mAttachedSurface = SVIGLSurface.getSurface(surface);
    	mAttachedSurface.incrementUsageCount(this);

		if(color == null){			
			color = defaultColor;
		}

        initialize(x, y, width, height, color);
        mNativeSlide = nativeInitializeSVISlide(mAttachedSurface.getNativeHandle(), type, parent, x, y, width, height, color, null);
        if (mNativeSlide != -1) {
            mIsSuccessCreateSlide = true;
        }
    }
    
    /**
     * SVISlide constructor
     *
     * @param type   > set to native slide type(BASE_SLIDE=0, IMAGE_SLIDE=1, TEXT_SLIDE=2)
     * @param parent > set to native slide parent
     * @param x      > set to native slide x-axis offset
     * @param y      > set to native slide y-axis offset
     * @param width  > set to native slide width
     * @param height > set to native slide height
     * @param color  > set to native slide color
     * @param bitmap > set bitmap information to native SVIImage side. bitArray of Bitmap will be referred in SVIImage native side.
     */
    SVISlide(SVIGLSurface surface, int type, int parent, float x, float y, float width, float height, float[] color, Bitmap bitmap) {
    	mAttachedSurface = SVIGLSurface.getSurface(surface);
    	mAttachedSurface.incrementUsageCount(this);

		if(color == null){			
			color = defaultColor;
		}

        initialize(x, y, width, height, color);
        mNativeSlide = nativeInitializeSVISlide(mAttachedSurface.getNativeHandle(), type, parent, x, y, width, height, color, bitmap);
        if (mNativeSlide != -1) {
            mIsSuccessCreateSlide = true;
        }
    }

    /**
     * default initialize
     */
    public void initialize() {
    	SVIEngineThreadProtection.validateMainThread();
        mListSlide = new ArrayList<SVISlide>();
        mTesselationPointList = new Vector<SVIVector2>();
    }

    private void initialize(float x, float y, float width, float height, float[] color) {
    	SVIEngineThreadProtection.validateMainThread();
        mListSlide = new ArrayList<SVISlide>();
        mTesselationPointList = new Vector<SVIVector2>();

        mImageScaleToFitRegion = new SVIRect(x, y, width, height);
        mTextureRegion = new SVIRect(x, y, width, height);
        mBackFaceTextureRegion = new SVIRect(x, y, width, height);
        mRegion = new SVIRect(x, y, width, height);
        mPivotPoint = new SVIPoint(0.5f, 0.5f);
        mPosition = new SVIPoint();
        mRotation = new SVIVector3(0.0f, 0.0f, 0.0f);
        //2012-06-21 masterkeaton27@gmail.com
        mSwipeRatio = new SVIVector3(0.0f, 0.0f, 0.0f);
        mDeformRatio = new SVIVector3(0.0f, 0.0f, 0.0f);
        mRTERatio = new SVIVector3(0.0f, 0.0f, 0.0f);
        
        mScale = new SVIVector3(1.0f, 1.0f, 1.0f);
        mZPosition = 0.0f;
        updateGeometryByRegion();

        mBgColor = new SVIColor(color[0], color[1], color[2], color[3]);
        mBorderWidth = 0.0f;
        mBorderColor = new SVIColor(0.5f, 0.5f, 0.5f, 1.0f);

        mUseNormalMap = false;
        
        mClipSubSlide = false;
        mCornerRadius = 0.0f;
        mHidden = false;
        mOpacity = 1.0f;

        mShadowColor = new SVIColor(0.0f, 0.0f, 0.0f, 0.6f);
        mShadowRadius = 0.0f;
        mShadowOffset = new SVIPoint(0.0f, 0.0f);

        mZOrderType = SlideZOrderType.NORMAL;

        mBackFaceImage = null;
        mImage = null;
        mNormalImage = null;
        mSpecularImage = null;
        
        mBlendType = BlendType.BLEND_MULTIPLY;
        mRTEffectType = RTEffectType.RT_NONE;
        mPriority = 0;
        mParent = null;

        //2011.12.09 jongchae.moon
        mEnableAntiAliasing = false;
        //

        mDeformDirection = SlideDeformDirection.DEFORM_HORIZONTAL;
        mDeformType = SlideDeformType.DEFORM_NONE;
        mSwipeType = SlideSwipeType.SWIPE_NONE;

        //2012/12/08 rareboy0112@facebook.com
        mMorphingType = SlideMorphingType.MORPHING_NONE;
        mMorphingRadius = 0.0f;
        mMorphingColor = new SVIColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        mImageScaleType = ImageScaleType.FIT_XY;
        mLightType = LightType.NO_LIGHT;
        mLightAngle = 0.0f;
        mLightRadius = 0.5f;
        mLightOpacity = 1.0f;
        mLightPower = 1.0f;
        mLightOffset = new SVIPoint(0.5f, 0.5f);
        mLightColor = new SVIColor(1.0f, 1.0f, 1.0f, 1.0f);
        mLightDirection = new SVIVector4(0.0f,0.0f,1.0f,0.0f);
        mViewDirection = new SVIVector4(0.0f,0.0f,1.0f,0.0f);
    }

    /**
     * SVISlideManager to the render request
     */
    public void invalidate() {
        // TODO: Applications should call mAttachedSurface.getSlideManager().requestRender() explicitly.
    	if ( mImage != null ) // mImage = null set during finalize by GC which is causing exceptions P131227-00485
    		SVIEngineThreadProtection.validateMainThread();
    	
        if (mAttachedSurface.getSlideManager().isPausedImplicitAnimation() == false)
        	mAttachedSurface.getSlideManager().requestRender();
    }

    protected void finalize() throws Throwable {
        //setImage function of SVIImage should be called to prevent accessing dangling pointer from SVITexture::upload.
        //modified by marx.kim(2011/08/22)
    	//SVIEngineThreadProtection.validateMainThread(); Always calls from different thread GCFinalizerDeamon thread
        if (mNativeSlide != -1) {
            if (mImage != null)
                setImage((SVIImage) null);
            
            //2012-12-10 masterkeaton27@gmail.com
            //normal & specular image added for lighting effect.
            if (mNormalImage != null)
                setNormalImage((SVIImage) null);
            
            if (mSpecularImage != null)
                setSpecularImage((SVIImage) null);

            if (mBackFaceImage != null)
                setBackFaceImage((SVIImage) null);

            nativeDestroySVISlide(mNativeSlide);
            mNativeSlide = -1;
        }
        
        mAttachedSurface.decrementUsageCount(this);
        super.finalize();
    }

    /**
     * get natvie slide handle
     *
     * @return int mNativeSlide
     */
    public int getNativeSlideHandle() {
        return mNativeSlide;
    }

	public SVIGLSurface getSurface(){
		return mAttachedSurface;
	}

    /**
     * set to native slide region
     *
     * @param x      > set to native slide x-axis offset
     * @param y      > set to native slide y-axis offset
     * @param width  > set to native slide width
     * @param height > set to native slide height
     */
    public void initRegion(float x, float y, float width, float height) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeInitRegionSVISlide(mNativeSlide, x, y, width, height);
    }

    /**
     * set to native slide rotation
     *
     * @param x > set to native slide x-axis angles
     * @param y > set to native slide y-axis angles
     * @param z > set to native slide z-axis angles
     * @param w > not used.
     */
    public void initRotation(float x, float y, float z, float w) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeInitRotationSVISlide(mNativeSlide, x, y, z, w);
    }

    /**
     * set to native slide scale
     *
     * @param x > set to native slide x-axis scale
     * @param y > set to native slide y-axis scale
     * @param z > set to native slide z-axis scale
     * @param w > not used.
     */
    public void initScale(float x, float y, float z, float w) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeInitScaleSVISlide(mNativeSlide, x, y, z, w);
    }

    /**
     * set to native slide opacity
     *
     * @param opacity > set to native slide opacity
     */
    public void initOpacity(float opacity) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeInitOpacitySVISlide(mNativeSlide, opacity);
    }

    /**
     * set to native slide background-color
     *
     * @param bgColor > set to native slide background-color(bgColor[0] = red, bgColor[1] = green, bgColor[2] = blue, bgColor[3] = alpha)
     */
    public void initBackgroundColor(float[] bgColor) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeInitBackgroundColorSVISlide(mNativeSlide, bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    }

    /**
     * set to native slide depth value
     *
     * @param zPosition > set to native slide depth
     */
    public void initZPosition(float zPosition) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeInitZPositionSVISlide(mNativeSlide, zPosition);
    }

    /**
     * set slide region and implicit region animation
     *
     * @param x      > set slide x-axis offset value
     * @param y      > set slide y-axis offset value
     * @param width  > set slide width value
     * @param height > set slide height value
     */
    public void setRegion(float x, float y, float width, float height) {
        setRegion(x, y, width, height, IMPLICIT_ANIMATION_OFF_DURATION);
    }

//	public void setBounds(float x, float y, float width, float height) {
//		setBounds(x, y, width, height, IMPLICIT_ANIMATION_OFF_DURATION);
//	}

    /**
     * set slide position and implicit position animation
     *
     * @param x > set slide x-axis position value
     * @param y > set slide y-axis position value
     */
    public void setPosition(float x, float y) {
        setPosition(x, y, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide pivot-point and implicit pivot-point animation
     *
     * @param x > set slide x-axis pivot-point value
     * @param y > set slide y-axis pivot-point value
     */
    public void setPivotPoint(float x, float y) {
        setPivotPoint(x, y, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide depth value and implicit depth animation
     *
     * @param zPosition > set slide depth value
     */
    public void setZPosition(float zPosition) {
        setZPosition(zPosition, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide rotation and implicit rotation animation
     *
     * @param x > set slide x-axis angles value
     * @param y > set slide y-axis angles value
     * @param z > set slide z-axis angles value
     * @param w > not used
     */
    public void setRotation(float x, float y, float z, float w) {
        setRotation(x, y, z, w, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide deformation ratio and implicit deformation animation
     *
     * @param x > set slide x-axis ratio value
     * @param y > set slide y-axis ratio value
     * @param z > set slide z-axis ratio value
     */
    public void setDeformRatio(float x, float y, float z) {
        setDeformRatio(x, y, z, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide swipe effect ratio and implicit swipe effect animation
     *
     * @param x > set slide x-axis ratio value
     * @param y > set slide y-axis ratio value
     * @param z > set slide z-axis ratio value
     */
    public void setSwipeRatio(float x, float y, float z) {
        setSwipeRatio(x, y, z, IMPLICIT_ANIMATION_OFF_DURATION);
    }
    
    /**
     * set slide render-target ratio and implicit render-target animation
     *
     * @param x > set slide x-axis ratio value
     * @param y > set slide y-axis ratio value
     * @param z > set slide z-axis ratio value
     */
    public void setRTERatio(float x, float y, float z) {
        setRTERatio(x, y, z, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide scale and implicit scale animation
     *
     * @param x > set slide x-axis scale value
     * @param y > set slide y-axis scale value
     * @param z > set slide z-axis scale value
     */
    public void setScale(float x, float y, float z) {
        setScale(x, y, z, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * capture settings.
     *
     * @param capture > true = on, false = off
     */
    public void setCapture(boolean capture) {
        mCapture = capture;
        nativeSetCaptureSVISlide(mNativeSlide, mCapture);
        invalidate();
    }

    /**
     * set the name of the slide.
     *
     * @param name > set the value of mName
     */
    public void setName(String name) {
        mName = name;
    }

    /**
     * set slide background-color and implicit background-color animation.
     *
     * @param bgColor > set slide background-color(bgColor[0] = red, bgColor[1] = green, bgColor[2] = blue, bgColor[3] = alpha) value
     */
    public void setBackgroundColor(float[] bgColor) {
        setBackgroundColor(bgColor, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide background-color and implicit background-color animation.
     *
     * @param color > set slide background-color value
     */
    public void setBackgroundColor(SVIColor color) {
        float[] bgColor = {color.mR, color.mG, color.mB, color.mA};
        setBackgroundColor(bgColor, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide background-color and implicit background-color animation.
     *
     * @param color    > set slide background-color value
     * @param duration > set implicit animation duration value
     */
    public void setBackgroundColor(SVIColor color, int duration) {
        float[] bgColor = {color.mR, color.mG, color.mB, color.mA};
        setBackgroundColor(bgColor, duration);
    }

    /**
     * set slide opacity and implicit opacity animation.
     *
     * @param opacity > set slide opacity value
     */
    public void setOpacity(float opacity) {
        setOpacity(opacity, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide corner-radius and implicit corner-radius animation.
     *
     * @param cornerRadius > set slide corner-radius value
     */
    public void setCornerRadius(float cornerRadius) {
        setCornerRadius(cornerRadius, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide border-width and implicit border-width animation.
     *
     * @param borderWidth > set slide border-width value
     */
    public void setBorderWidth(float borderWidth) {
        setBorderWidth(borderWidth, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide border-color and implicit border-color animation.
     *
     * @param borderColor > set slide border-color(borderColor[0] = red, borderColor[1] = green, borderColor[2] = blue, borderColor[3] = alpha) value
     */
    public void setBorderColor(float[] borderColor) {
        setBorderColor(borderColor, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide border-color and implicit border-color animation.
     *
     * @param color > set slide border-color value
     */
    public void setBorderColor(SVIColor color) {
        float[] bgColor = {color.mR, color.mG, color.mB, color.mA};
        setBorderColor(bgColor, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide border-color and implicit border-color animation.
     *
     * @param color    > set slide border-color value
     * @param duration > set implicit animation duration value
     */
    public void setBorderColor(SVIColor color, int duration) {
        float[] bgColor = {color.mR, color.mG, color.mB, color.mA};
        setBorderColor(bgColor, duration);
    }

    /**
     * set slide shadow-radius and implicit shadow-radius animation.
     *
     * @param shadowRadius > set slide shadow-radius value
     */
    public void setShadowRadius(float shadowRadius) {
        setShadowRadius(shadowRadius, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide shadow-color and implicit shadow-color animation.
     *
     * @param shadowColor > set slide shadow-color(shadowColor[0] = red, shadowColor[1] = green, shadowColor[2] = blue, shadowColor[3] = alpha) value
     */
    public void setShadowColor(float[] shadowColor) {
        setShadowColor(shadowColor, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide shadow-color and implicit shadow-color animation.
     *
     * @param color > set slide shadow-color value
     */
    public void setShadowColor(SVIColor color) {
        float[] bgColor = {color.mR, color.mG, color.mB, color.mA};
        setShadowColor(bgColor, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide shadow-color and implicit shadow-color animation.
     *
     * @param color    > set slide shadow-color value
     * @param duration > set implicit animation duration
     */
    public void setShadowColor(SVIColor color, int duration) {
        float[] bgColor = {color.mR, color.mG, color.mB, color.mA};
        setShadowColor(bgColor, duration);
    }

    /**
     * set slide shadow-opacity and implicit shadow-opacity animation.
     *
     * @param shadowOpacity > set slide shadow-opacity value
     */
    public void setShadowOpacity(float shadowOpacity) {
        setShadowOpacity(shadowOpacity, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide shadow-offset and implicit shadow-offset animation.
     *
     * @param x > set slide x-axis start value
     * @param y > set slide y-axis start value
     */
    public void setShadowOffset(float x, float y) {
        setShadowOffset(x, y, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * sets the color of light to the slide and implicit light-color animation.
     *
     * @param lightColor > set slide light-color(lightColor[0] = red, lightColor[1] = green, lightColor[2] = blue, lightColor[3] = alpha) value
     */
    public void setLightColor(float[] lightColor) {
        setLightColor(lightColor, IMPLICIT_ANIMATION_OFF_DURATION);
    }
    
    /**
     * sets the direction of light to the slide and implicit light-direction animation.
     *
     * @param lightDirection > set slide light-direction(direction) value
     */
    public void setLightDirection(SVIVector4 lightDirection) {
        setLightDirection(lightDirection, IMPLICIT_ANIMATION_OFF_DURATION);
    }
    
    /**
     * sets the direction of view to the slide and implicit view-direction animation.
     *
     * @param viewDirection > set slide view-direction(direction) value
     */
    public void setViewDirection(SVIVector4 viewDirection) {
        setViewDirection(viewDirection, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * sets the offset of light to the slide and implicit light-offset animation.
     *
     * @param x > set slide x-axis light-offset value
     * @param y > set slide y-axis light-offset value
     */
    public void setLightOffset(float x, float y) {
        setLightOffset(x, y, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * sets the opacity of light to the slide and implicit light-opacity animation.
     *
     * @param lightOpacity > set slide light-opacity value
     */
    public void setLightOpacity(float lightOpacity) {
        setLightOpacity(lightOpacity, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * sets the sharpness of light to the slide and implicit light-power animation.
     *
     * @param lightPower > sets the sharpness of the light
     */
    public void setLightPower(float lightPower) {
        setLightPower(lightPower, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * sets the angle of light to the slide and implicit light-angle animation.
     *
     * @param lightAngle > sets the angle of the light
     */
    public void setLightAngle(float lightAngle) {
        setLightAngle(lightAngle, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * sets the radius of light to the slide and implicit light-radius animation.
     *
     * @param lightRadius > sets the radius of the light
     */
    public void setLightRadius(float lightRadius) {
        setLightRadius(lightRadius, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide texture-region and implicit texture-region animation.
     *
     * @param x      > set starting point value of the x-axis texture in the slide.
     * @param y      > set starting point value of the y-axis texture in the slide.
     * @param width  > set texture width value
     * @param height > set texture height value
     */
    public void setTextureRegion(float x, float y, float width, float height) {
        setTextureRegion(x, y, width, height, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide back-face texture-region and implicit back-face texture-region animation.
     *
     * @param x      > set starting point value of the x-axis back-face texture in the slide.
     * @param y      > set starting point value of the y-axis back-face texture in the slide.
     * @param width  > set back-face texture width value
     * @param height > set back-face texture height value
     */
    public void setBackFaceTextureRegion(float x, float y, float width, float height) {
        setBackFaceTextureRegion(x, y, width, height, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * @param x
     * @param y
     * @param width
     * @param height
     */
    public void setScaleToFitRegion(float x, float y, float width, float height) {
        setScaleToFitRegion(x, y, width, height, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set slide region and implicit region animation.
     *
     * @param x        > set x-axis starting point value of the slide.
     * @param y        > set y-axis starting point value of the slide.
     * @param width    > set slide width
     * @param height   > set slide height
     * @param duration > set implicit animation duration
     */
    public void setRegion(float x, float y, float width, float height, int duration) {
        SVIEngineThreadProtection.validateMainThread();
        mRegion.mOrigin.mX = x;
        mRegion.mOrigin.mY = y;
        mRegion.mSize.mWidth = width;
        mRegion.mSize.mHeight = height;
        updateGeometryByRegion();

        nativeSetRegionSVISlide(mNativeSlide, x, y, width, height);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.REGION, duration);
    }

    /**
     * set slide position and implicit position animation
     *
     * @param x        > set slide x-axis position value
     * @param y        > set slide y-axis position value
     * @param duration > set implicit animation duration
     */
    public void setPosition(float x, float y, int duration) {
        SVIEngineThreadProtection.validateMainThread();
        mPosition.mX = x;
        mPosition.mY = y;
        updateGeometryByBoundsOrPosition();

        nativeSetPositionSVISlide(mNativeSlide, x, y);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.POSITION, duration);
    }

    /**
     * set slide pivot-point and implicit pivot-point animation
     *
     * @param x        > set slide x-axis pivot-point value
     * @param y        > set slide y-axis pivot-point value
     * @param duration > set implicit animation duration
     */
    public void setPivotPoint(float x, float y, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mPivotPoint.mX = x;
        mPivotPoint.mY = y;
        updateGeometryByPivotPoint();

        nativeSetPivotPointSVISlide(mNativeSlide, x, y);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.PIVOT_POINT, duration);
    }

    /**
     * set slide depth value and implicit depth animation
     *
     * @param zPosition > set slide depth value
     * @param duration  > set implicit animation duration
     */
    public void setZPosition(float zPosition, int duration) {
        SVIEngineThreadProtection.validateMainThread();
        mZPosition = zPosition;

        nativeSetZPositionSVISlide(mNativeSlide, zPosition);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.ZPOSITION, duration);
    }

    /**
     * set slide rotation and implicit rotation animation
     *
     * @param x        > set slide x-axis angles
     * @param y        > set slide y-axis angles
     * @param z        > set slide z-axis angles
     * @param w        > not used
     * @param duration > set implicit animation duration
     */
    public void setRotation(float x, float y, float z, float w, int duration) {
        SVIEngineThreadProtection.validateMainThread();
        mRotation.mX = x;
        mRotation.mY = y;
        mRotation.mZ = z;

        nativeSetRotationSVISlide(mNativeSlide, x, y, z, w);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.ROTATION, duration);
    }

    /**
     * set slide deformation ratio and implicit deformation animation
     *
     * @param x        > set slide x-axis ratio value
     * @param y        > set slide y-axis ratio value
     * @param z        > set slide z-axis ratio value
     * @param duration > set implicit animation duration
     */
    public void setDeformRatio(float x, float y, float z, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mDeformRatio.mX = x;
        mDeformRatio.mY = y;
        mDeformRatio.mZ = z;

        nativeSetDeformRatioSVISlide(mNativeSlide, x, y, z);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.DEFORM_RATIO, duration);
    }
    
    /**
     * set slide swipe effect ratio and implicit swipe effect animation
     *
     * @param x        > set slide x-axis ratio value
     * @param y        > set slide y-axis ratio value
     * @param z        > set slide z-axis ratio value
     * @param duration > set implicit animation duration
     */
    public void setSwipeRatio(float x, float y, float z, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mSwipeRatio.mX = x;
        mSwipeRatio.mY = y;
        mSwipeRatio.mZ = z;

        nativeSetSwipeRatioSVISlide(mNativeSlide, x, y, z);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.SWIPE_RATIO, duration);
    }

    /**
     * set slide render-target ratio and implicit render-target animation
     *
     * @param x        > set slide x-axis ratio value
     * @param y        > set slide y-axis ratio value
     * @param z        > set slide z-axis ratio value
     * @param duration > set implicit animation duration
     */
    public void setRTERatio(float x, float y, float z, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mRTERatio.mX = x;
        mRTERatio.mY = y;
        mRTERatio.mZ = z;

        nativeSetRTERatioSVISlide(mNativeSlide, x, y, z);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.RTE_RATIO, duration);
    }

    /**
     * set slide scale and implicit scale animation
     *
     * @param x        > set slide x-axis scale value
     * @param y        > set slide y-axis scale value
     * @param z        > set slide z-axis scale value
     * @param duration > set implicit animation duration
     */
    public void setScale(float x, float y, float z, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mScale.mX = x;
        mScale.mY = y;
        mScale.mZ = z;

        nativeSetScaleSVISlide(mNativeSlide, x, y, z);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.SCALE, duration);
    }

    /**
     * set slide background-color and implicit background-color animation.
     *
     * @param bgColor  > set slide background-color(bgColor[0] = red, bgColor[1] = green, bgColor[2] = blue, bgColor[3] = alpha) value
     * @param duration > set implicit animation duration
     */
    public void setBackgroundColor(float[] bgColor, int duration) {
	    SVIEngineThreadProtection.validateMainThread();
        mBgColor.mR = bgColor[0];
        mBgColor.mG = bgColor[1];
        mBgColor.mB = bgColor[2];
        mBgColor.mA = bgColor[3];

        //2011.12.06 masterkeaton27@gmail.com
        //remove vailed routines.
        /*if (mBlendType == BlendType.BLEND_MULTIPLY){
              float initBgColor[] = {1.0f, 1.0f, 1.0f, 0.0f};
              initBackgroundColor(initBgColor);
              setBlendType(BlendType.BLEND_ADD);
          }*/

        nativeSetBackgroundColorSVISlide(mNativeSlide, bgColor[0], bgColor[1], bgColor[2], bgColor[3]);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.BG_COLOR, duration);
    }

    /**
     * set slide opacity and implicit opacity animation.
     *
     * @param opacity  > set slide opacity value
     * @param duration > set implicit animation duration
     */
    public void setOpacity(float opacity, int duration) {
        SVIEngineThreadProtection.validateMainThread();  
        mOpacity = opacity;

        nativeSetOpacitySVISlide(mNativeSlide, opacity);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.OPACITY, duration);
    }

    /**
     * set slide corner-radius and implicit corner-radius animation.
     *
     * @param cornerRadius > set slide corner-radius value
     * @param duration     > set implicit animation duration
     */
    public void setCornerRadius(float cornerRadius, int duration) {
        SVIEngineThreadProtection.validateMainThread();  
        mCornerRadius = cornerRadius;

        nativeSetCornerRadiusSVISlide(mNativeSlide, cornerRadius);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.CORNER_RADIUS, duration);
    }

    /**
     * set slide border-width and implicit border-width animation.
     *
     * @param borderWidth > set slide border-width value
     * @param duration    > set implicit animation duration
     */
    public void setBorderWidth(float borderWidth, int duration) {
        SVIEngineThreadProtection.validateMainThread(); 
        mBorderWidth = borderWidth;

        nativeSetBorderWidthSVISlide(mNativeSlide, borderWidth);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.BORDER_WIDTH, duration);
    }

    /**
     * set slide border-color and implicit border-color animation.
     *
     * @param borderColor > set slide border-color(borderColor[0] = red, borderColor[1] = green, borderColor[2] = blue, borderColor[3] = alpha) value
     * @param duration    > set implicit animation duration
     */
    public void setBorderColor(float[] borderColor, int duration) {
        SVIEngineThreadProtection.validateMainThread();
        mBorderColor.mR = borderColor[0];
        mBorderColor.mG = borderColor[1];
        mBorderColor.mB = borderColor[2];
        mBorderColor.mA = borderColor[3];

        nativeSetBorderColorSVISlide(mNativeSlide, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.BORDER_COLOR, duration);
    }

    /**
     * set slide shadow-radius and implicit shadow-radius animation.
     *
     * @param shadowRadius > set slide shadow-radius value
     * @param duration     > set implicit animation duration
     */
    public void setShadowRadius(float shadowRadius, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mShadowRadius = shadowRadius;

        nativeSetShadowRadiusSVISlide(mNativeSlide, shadowRadius);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.SHADOW_RADIUS, duration);
    }

    /**
     * set slide shadow-color and implicit shadow-color animation.
     *
     * @param shadowColor > set slide shadow-color(shadowColor[0] = red, shadowColor[1] = green, shadowColor[2] = blue, shadowColor[3] = alpha) value
     * @param duration    > set implicit animation duration
     */
    public void setShadowColor(float[] shadowColor, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mShadowColor.mR = shadowColor[0];
        mShadowColor.mG = shadowColor[1];
        mShadowColor.mB = shadowColor[2];
        mShadowColor.mA = shadowColor[3];

        nativeSetShadowColorSVISlide(mNativeSlide, shadowColor[0], shadowColor[1], shadowColor[2], shadowColor[3]);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.SHADOW_COLOR, duration);
    }

    /**
     * set slide shadow-opacity and implicit shadow-opacity animation.
     *
     * @param shadowOpacity > set slide shadow-opacity value
     * @param duration      > set implicit animation duration
     */
    public void setShadowOpacity(float shadowOpacity, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeSetShadowOpacitySVISlide(mNativeSlide, shadowOpacity);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.SHADOW_OPACITY, duration);
    }

    /**
     * set slide shadow-offset and implicit shadow-offset animation.
     *
     * @param x        > set slide x-axis start value
     * @param y        > set slide y-axis start value
     * @param duration > set implicit animation duration
     */
    public void setShadowOffset(float x, float y, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mShadowOffset.mX = x;
        mShadowOffset.mY = y;

        nativeSetShadowOffsetSVISlide(mNativeSlide, x, y);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.SHADOW_OFFSET, duration);
    }

    /**
     * set slide rotation animation
     *
     * @param fx       > set the angle of the x-axis the from slide
     * @param fy       > set the angle of the y-axis the from slide
     * @param fz       > set the angle of the z-axis the from slide
     * @param fw       > not used
     * @param tx       > set the angle of the x-axis the to slide
     * @param ty       > set the angle of the y-axis the to slide
     * @param tz       > set the angle of the z-axis the to slide
     * @param tw       > not used
     * @param duration > set implicit animation duration
     */
    public void setRotationFrom(float fx, float fy, float fz, float fw, float tx, float ty, float tz, float tw, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeSetRotationFromSVISlide(mNativeSlide, fx, fy, fz, fw, tx, ty, tz, tw);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.ROTATION_FROM, duration);
    }

    /**
     * set the priority of the slide is rendered.
     *
     * @param type > set the value of mZOrderType(BOTTOM = 0, NORMAL = 1, TOPMOST = 2)
     */
    public void setZOrderType(int type) {
        mZOrderType = type;

        nativeSetZOrderTypeSVISlide(mNativeSlide, type);
        invalidate();
    }


    /**
     * set using background texture as a normal map
     *
     * @param useNormalMap flag
     */
    public void setUseNormalMap(boolean useNormalMap){
    	mUseNormalMap = useNormalMap;
    	nativeSetUseNormalMapSVISlide(mNativeSlide , useNormalMap);
    	invalidate();
    }
    
    
    /**
     * set the slide of deformation type
     *
     * @param type > set the value of mDeformType(DEFORM_NONE = 0, DEFORM_TWIST = 1, DEFORM_EXPLODE = 2)
     */
    public void setDeformType(int type) {
        mDeformType = type;
        nativeSetDeformTypeSVISlide(mNativeSlide, type);
        invalidate();
    }
    
    /**
     * set the slide of filter type
     *
     * @param type > set the value of mFilterType(FILTER_NONE = 0, FILTER_SKETCH = 1, FILTER_OILPAINT = 2)
     */
    public void setFilterType(int type) {
        mFilterType = type;
        nativeSetFilterTypeSVISlide(mNativeSlide, type);
        invalidate();
    }

    /**
     * set the slide of morphing type
     *
     * @param type > set the value of mMorphingType(MORPHING_NONE = 0, MORPHING_TRICKLE = 1)
     */
    public void setMorphingType(int type)
    {
    	mMorphingType = type;
    	nativeSetMorphingTypeSVISlide(mNativeSlide, type);
    	invalidate();
    }
   
    /**
     * set the slide of swipe effect type
     *
     * @param type > set the value of mSwipeType(SWIPE_NONE = 0, SWIPE_LINEAR = 1, SWIPE_RADIAL = 2)
     */
    public void setSwipeType(int type){
    	mSwipeType = type;
        nativeSetSwipeTypeSVISlide(mNativeSlide, type);
        invalidate();
    }
    
    /**
     * set the slide of deformation direction type
     *
     * @param type > set the value of mDeformDirection(DEFORM_HORIZONTAL = 0, DEFORM_VERTICAL = 1)
     */
    public void setDeformDirection(int direction) {
        mDeformDirection = direction;
        nativeSetDeformDirectionSVISlide(mNativeSlide, direction);
        invalidate();
    }

    /**
     * sets the color of light to the slide and implicit light-color animation.
     *
     * @param lightColor > set slide light-color(lightColor[0] = red, lightColor[1] = green, lightColor[2] = blue, lightColor[3] = alpha) value
     * @param duration   > set implicit animation duration
     */
    public void setLightColor(float[] lightColor, int duration) {
        SVIEngineThreadProtection.validateMainThread();
        mLightColor.mR = lightColor[0];
        mLightColor.mG = lightColor[1];
        mLightColor.mB = lightColor[2];
        mLightColor.mA = lightColor[3];
        nativeSetLightColorSVISlide(mNativeSlide, lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.LIGHT_COLOR, duration);
    }
    
    /**
     * sets the direction of light to the slide and implicit light-direction animation.
     *
     * @param direction > set slide light-direction(direction) value
     * @param duration   > set implicit animation duration
     */
    public void setLightDirection(SVIVector4 direction, int duration) {
        SVIEngineThreadProtection.validateMainThread();
        mLightDirection.mX = direction.mX;
        mLightDirection.mY = direction.mY;
        mLightDirection.mZ = direction.mZ;
        mLightDirection.mW = direction.mW;
        nativeSetLightDirectionSVISlide(mNativeSlide, direction.mX, direction.mY, direction.mZ, direction.mW);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.LIGHT_DIRECTION, duration);
    }
    
    /**
     * sets the direction of view to the slide and implicit view-direction animation.
     *
     * @param direction > set slide view-direction(direction) value
     * @param duration   > set implicit animation duration
     */
    public void setViewDirection(SVIVector4 direction, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mViewDirection.mX = direction.mX;
        mViewDirection.mY = direction.mY;
        mViewDirection.mZ = direction.mZ;
        mViewDirection.mW = direction.mW;
        nativeSetViewDirectionSVISlide(mNativeSlide, direction.mX, direction.mY, direction.mZ, direction.mW);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.VIEW_DIRECTION, duration);
    }

    /**
     * sets the offset of light to the slide and implicit light-offset animation.
     *
     * @param x        > set slide x-axis light-offset value
     * @param y        > set slide y-axis light-offset value
     * @param duration > set implicit animation duration
     */
    public void setLightOffset(float x, float y, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mLightOffset.mX = x;
        mLightOffset.mY = y;
        nativeSetLightOffsetSVISlide(mNativeSlide, x, y);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.LIGHT_OFFSET, duration);
    }

    /**
     * sets the opacity of light to the slide and implicit light-opacity animation.
     *
     * @param lightOpacity > set slide light-opacity value
     * @param duration     > set implicit animation duration
     */
    public void setLightOpacity(float lightOpacity, int duration) {
    	SVIEngineThreadProtection.validateMainThread(); 
        mLightOpacity = lightOpacity;
        nativeSetLightOpacitySVISlide(mNativeSlide, lightOpacity);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.LIGHT_OPACITY, duration);
    }

    /**
     * sets the sharpness of light to the slide and implicit light-power animation.
     *
     * @param lightPower > sets the sharpness of the light
     * @param duration   > set implicit animation duration
     */
    public void setLightPower(float lightPower, int duration) {
    	SVIEngineThreadProtection.validateMainThread(); 
        mLightPower = lightPower;
        nativeSetLightPowerSVISlide(mNativeSlide, lightPower);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.LIGHT_POWER, duration);
    }

    /**
     * sets the angle of light to the slide and implicit light-angle animation.
     *
     * @param lightAngle > sets the angle of the light
     * @param duration   > set implicit animation duration
     */
    public void setLightAngle(float lightAngle, int duration) {
    	SVIEngineThreadProtection.validateMainThread(); 
        mLightAngle = lightAngle;
        nativeSetLightAngleSVISlide(mNativeSlide, lightAngle);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.LIGHT_ANGLE, duration);
    }

    /**
     * sets the radius of light to the slide and implicit light-radius animation.
     *
     * @param lightRadius > sets the radius of the light
     * @param duration    > set implicit animation duration
     */
    public void setLightRadius(float lightRadius, int duration) {
    	SVIEngineThreadProtection.validateMainThread(); 
        mLightRadius = lightRadius;
        nativeSetLightRadiusSVISlide(mNativeSlide, lightRadius);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this,
                PropertyAnimationType.LIGHT_RADIUS, duration);
    }

    /**
     * set the slide of light type
     *
     * @param type > set the value of mLightType(NO_LIGHT = 0, SPOT_LIGHT = 1, LINEAR_LIGHT = 2)
     */
    public void setLightType(int type) {
        mLightType = type;
        nativeSetLightTypeSVISlide(mNativeSlide, type);
        invalidate();
    }
    
    /**
     * get the slide of light type
     *
     * @return int LightType
     */
    public int getLightType() {
        return mLightType;
    }
    
    /**
     * get the slide of light Opacity
     *
     * @return float LightOpacity
     */
    public float getLightOpacity() {
        return mLightOpacity;
    }
    
    /**
     * get the slide of LightAngle
     *
     * @return float LightAngle
     */
    public float getLightAngle() {
        return mLightAngle;
    }
    
    /**
     * get the slide of LightRadius
     *
     * @return float LightRadius
     */
    public float getLightRadius() {
        return mLightRadius;
    }
    
    /**
     * get the slide of LightOffset
     *
     * @return SVIPoint LightOffset
     */
    public SVIPoint getLightOffset() {
        return mLightOffset;
    }

    /**
     * get image scale type
     *
     * @return int mImageScaleType
     */
    public int getImageScaleType() {
        return mImageScaleType;
    }

    /**
     * set the slide of image scale type
     *
     * @param type > set the value of mImageScaleType
     */
    public void setImageScaleType(int type) {
        mImageScaleType = type;
        nativeSetScaleToFitTypeSVISlide(mNativeSlide, type);
        invalidate();
    }

    /**
     * set slide texture-region and implicit texture-region animation.
     *
     * @param x        > set starting point value of the x-axis texture in the slide.
     * @param y        > set starting point value of the y-axis texture in the slide.
     * @param width    > set texture width value
     * @param height   > set texture height value
     * @param duration > set implicit animation duration
     */
    public void setTextureRegion(float x, float y, float width, float height, int duration) {
    	SVIEngineThreadProtection.validateMainThread(); 
        mTextureRegion.mOrigin.mX = x;
        mTextureRegion.mOrigin.mY = y;
        mTextureRegion.mSize.mWidth = width;
        mTextureRegion.mSize.mHeight = height;
        nativeSetTextureRegionSVISlide(mNativeSlide, x, y, width, height);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.TEXTURE_REGION, duration);
    }

    /**
     * set slide back-face texture-region and implicit back-face texture-region animation.
     *
     * @param x        > set starting point value of the x-axis back-face texture in the slide.
     * @param y        > set starting point value of the y-axis back-face texture in the slide.
     * @param width    > set back-face texture width value
     * @param height   > set back-face texture height value
     * @param duration > set implicit animation duration
     */
    public void setBackFaceTextureRegion(float x, float y, float width, float height, int duration) {
    	SVIEngineThreadProtection.validateMainThread(); 
        mBackFaceTextureRegion.mOrigin.mX = x;
        mBackFaceTextureRegion.mOrigin.mY = y;
        mBackFaceTextureRegion.mSize.mWidth = width;
        mBackFaceTextureRegion.mSize.mHeight = height;
        nativeSetBackFaceTextureRegionSVISlide(mNativeSlide, x, y, width, height);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.BACKFACE_TEXTURE_REGION, duration);
    }

    /**
     * @param x
     * @param y
     * @param width
     * @param height
     * @param duration
     */
    public void setScaleToFitRegion(float x, float y, float width, float height, int duration) {
    	SVIEngineThreadProtection.validateMainThread(); 
        mImageScaleToFitRegion.mOrigin.mX = x;
        mImageScaleToFitRegion.mOrigin.mY = y;
        mImageScaleToFitRegion.mSize.mWidth = width;
        mImageScaleToFitRegion.mSize.mHeight = height;
        nativeSetTextureScaleToFitRegionSVISlide(mNativeSlide, x, y, width, height);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.SCALETOFIT_REGION, duration);
    }

    /**
     * set the slide should be invisible.
     *
     * @param hidden > set the value of mHidden(true = invisible, false = visible)
     */
    public void setHidden(boolean hidden) {
        mHidden = hidden;

        nativeSetHiddenSVISlide(mNativeSlide, hidden);
        invalidate();
    }

    /**
     * set whether the child slide the clip.
     *
     * @param clipSubSlides > set the value of mClipSubSlide(true = clip on, false = clip off)
     */
    public void setClipSubSlides(boolean clipSubSlides) {
        mClipSubSlide = clipSubSlides;

        nativeSetClipSubSlidesSVISlide(mNativeSlide, clipSubSlides);
        invalidate();
    }

    /**
     * set the slide should be orthographic.
     *
     * @param orthogonal > set to native slide orthogonal(true = orthogonal mode)
     */
    public void setOrthogonal(boolean orthogonal) {
        nativeSetOrthogonalSVISlide(mNativeSlide, orthogonal);
        mIsOrthogonal = orthogonal;
        invalidate();
    }

    /**
     * set whether render-target effect use.
     *
     * @param customRTE > set to native slide custormRTE value(true = on)
     */
    public void setCustomRTE(boolean customRTE) {
        nativeSetCustomRTESVISlide(mNativeSlide, customRTE);
        invalidate();
    }

    /**
     * set the order drawn from a parent-slide.
     *
     * @param priority > set the value of mPriority
     */
    public void setPriority(int priority) {
    	SVIEngineThreadProtection.validateMainThread();

        if (mPriority == priority)
            return;

        mPriority = priority;

        int findIndex = 0;
        SVISlide parent = this.getParent();

        if (parent == null)
            return;

        int curIndex = parent.getSubSlideIndex(this);
        int slideCount = parent.getSubSlideCount();

        for (int i = 0; i < slideCount; i++) {
            SVISlide childSlide = parent.getSubSlide(i);

            if (childSlide == this)
                continue;

            //if (mPriority == childSlide.getPriority() && curIndex <= findIndex)
            //	break;

            if (mPriority < childSlide.getPriority())
                break;

            findIndex++;
        }

        if (findIndex == curIndex)
            return;

        // Move
        parent.removeSlide(this);

        // temp
        if (mImage != null)
            setImage(mImage);

        if (mBackFaceImage != null)
            setBackFaceImage(mBackFaceImage);


        parent.addSlide(this, findIndex);
    }

    /**
     * set the parent-slide.
     *
     * @param parent > set the value of mParent
     */
    public void setParent(SVISlide parent) {
        mParent = parent;
    }

    /**
     * get region
     *
     * @return SVIRect mRegion
     */
    public SVIRect getRegion() {
        return mRegion;
    }

	/**
     * get Texture region
     *
     * @return SVIRect mTextureRegion
     */
    public SVIRect getTextureRegion() {
        return mTextureRegion;
    }

    /**
     * block hierarchical opacity blend it's own value.
     *
     * @param holdOpacity > set to native slide opacity(true = not affected)
     */
    public void setHoldOpacity(boolean holdOpacity) {
    	mHoldOpacity = holdOpacity;
        nativeSetHoldOpacitySVISlide(mNativeSlide, mHoldOpacity);
        invalidate();
    }
    
    /**
     * get the Hold Opacity Flag
     *
     * @return boolean HoldOpacity flag
     */
    public boolean getHoldOpacity() {
    	return mHoldOpacity ;
        
    }

    /**
     * block hierarchical scale blend it's own value.
     *
     * @param holdScale > set to native slide scale(true = not affected)
     */
    public void setHoldScale(boolean holdScale) {
    	mHoldScale = holdScale;
        nativeSetHoldScaleSVISlide(mNativeSlide, mHoldScale);
        invalidate();
    }
    
    /**
     * get the Hold Scale Flag
     *
     * @return boolean HoldScale flag
     */
    public boolean getHoldScale() {
    	return mHoldScale ;
        
    }

    /**
     * set blend type
     *
     * @param blendType > set the value of mBlendType(BLEND_MULTIPLY = 0, BLEND_ADD = 1)
     */
    public void setBlendType(int blendType) {
        mBlendType = blendType;
        nativeSetBlendTypeSVISlide(mNativeSlide, blendType);
        invalidate();
    }

    /**
     * set render target effect type
     *
     * @param effectType > set the value of mRTEffectType(RT_NONE = 0, RT_RIPPLE = 1, RT_PULSE = 2, RT_DREAMVISION = 3,
     *                   RT_WIZZLE = 4, RT_PAPERFOLD = 5, RT_MAGNIFY = 6, RT_GLASSTILE = 7, RT_ZOOMBLUR = 8, RT_DIRECTIONBLUR = 9)
     */
    public void setRTEffectType(int effectType) {
        mRTEffectType = effectType;
        nativeSetRTEffectTypeSVISlide(mNativeSlide, effectType);
        invalidate();
    }

    /**
     * anti-aliasing settings.
     *
     * @param enable > set the value of mEnableAntiAliasing(true = use anti-aliasing)
     */
    //2011.12.09 jongchae.moon
    public void setAntiAliasing(boolean enable) {
        mEnableAntiAliasing = enable;
        nativeSetAntiAliasing(mNativeSlide, enable);
        invalidate();
    }
    
    /**
     * get the Anti-Aliasing flag of the slide
     */
    public boolean getAntiAliasing(){
    	return mEnableAntiAliasing;
    }

    /**
     * get name of slide
     *
     * @return String mName
     */
    public String getName() {
        return mName;
    }

    /**
     * get pivot-point of slide
     *
     * @return SVIPoint mPivotPoint
     */
    public SVIPoint getPivotPoint() {
        return mPivotPoint;
    }

    /**
     * get position of slide
     *
     * @return SVIPoint mPosition
     */
    public SVIPoint getPosition() {
        return mPosition;
    }
    
    /**
     * get x of slide
     *
     * @return float mX
     */
    public float getX() {
        return mPosition.mX;
    }
    
    /**
     * get y of slide
     *
     * @return float mY
     */
    public float getY() {
        return mPosition.mY;
    }
    
    /**
     * get width of slide
     *
     * @return float mWidth
     */
    public float getWidth() {
        return mRegion.mSize.mWidth;
    }
    
    /**
     * get height of slide
     *
     * @return float mHeight
     */
    public float getHeight() {
        return mRegion.mSize.mHeight;
    }

    /**
     * get rotation of slide
     *
     * @return SVIVector3 mRotation
     */
    public SVIVector3 getRotation() {
        return mRotation;
    }

    /**
     * get deformation-ratio of slide
     *
     * @return SVIVector3 mDeformRatio
     */
    public SVIVector3 getDeformRatio() {
        return mDeformRatio;
    }

    /**
     * get swipe-ratio of slide
     *
     * @return SVIVector3 mSwipeRatio
     */
    public SVIVector3 getSwipeRatio() {
        return mSwipeRatio;
    }
    
    /**
     * get render target effect-ratio of slide
     *
     * @return SVIVector3 mSwipeRatio
     */
    public SVIVector3 getRTERatio() {
        return mRTERatio;
    }
    
    /**
     * get scale of slide
     *
     * @return SVIVector3 mScale
     */
    public SVIVector3 getScale() {
        return mScale;
    }

    /**
     * get z-position of slide
     *
     * @return float mZPosition
     */
    public float getZPosition() {
        return mZPosition;
    }

    /**
     * get background-color of slide
     *
     * @return SVIColor mBgColor
     */
    public SVIColor getBackbgoundColor() {
        return mBgColor;
    }
    /**
     * get Light-color of slide
     *
     * @return SVIColor mLightColor
     */
    public SVIColor getLightColor() {
        return mLightColor;
    }
    
    /**
     * get Light-Direction of slide
     *
     * @return SVIVector4 mLightDirection
     */
    public SVIVector4 getLightDirection() {
        return mLightDirection;
    }

    /**
     * get View-Direction of slide
     *
     * @return SVIVector4 mViewDirection
     */
    public SVIVector4 getViewDirection() {
        return mViewDirection;
    }
    
    /**
     * get border-width of slide
     *
     * @return float mBorderWidth
     */
    public float getBorderWidth() {
        return mBorderWidth;
    }

    /**
     * get border-color of slide
     *
     * @return SVIColor mBorderColor
     */
    public SVIColor getBorderColor() {
        return mBorderColor;
    }

    /**
     * get state of the sub-slide clip
     *
     * @return boolean mClipSubSlide
     */
    public boolean getClipSubSlides() {
        return mClipSubSlide;
    }

    /**
     * get corner-radius of slide
     *
     * @return float mCornerRadius
     */
    public float getCornerRadius() {
        return mCornerRadius;
    }
    
    
    /**
     * get BlendType of slide
     *
     * @return int mBlendType
     */
    public int getBlendType() {
        return mBlendType;
    }
    
    /**
     * get LightPower of slide
     *
     * @return float mLightPower
     */
    public float getLightPower() {
        return mLightPower;
    }
    
    /**
     * get normalmap of slide
     *
     * @return boolean mUseNormalMap
     */
    public boolean isUseNormalMap() {
        return mUseNormalMap;
    }
    
    /**
     * get normalmap of slide
     *
     * @return boolean mUseNormalMap
     */
    public boolean isOrthogonal() {
        return mIsOrthogonal;
    }

    /**
     * get hidden of slide
     *
     * @return boolean mHidden
     */
    public boolean getHidden() {
        return mHidden;
    }

    /**
     * get opacity of slide
     *
     * @return float mOpacity
     */
    public float getOpacity() {
        return mOpacity;
    }

    /**
     * get shadow-color of slide
     *
     * @return SVIColor mShadowColor
     */
    public SVIColor getShadowColor() {
        return mShadowColor;
    }

    /**
     * get shadow-opacity of slide
     *
     * @return float mShadowOpacity
     */
    public float getShadowOpacity() {
        return mShadowOpacity;
    }

    /**
     * get shadow-radius of slide
     *
     * @return float mShadowRadius
     */
    public float getShadowRadius() {
        return mShadowRadius;
    }

    /**
     * get shadow-offset of slide
     *
     * @return SVIPoint mShadowOffset
     */
    public SVIPoint getShadowOffect() {
        return mShadowOffset;
    }

    /**
     * get z-order type of slide
     *
     * @return int mZOrderType
     */
    public int getZOrderType() {
        return mZOrderType;
    }

    /**
     * get deformation type of slide
     *
     * @return int mDeformType
     */
    public int getDeformType() {
        return mDeformType;
    }
    
    /**
     * get swipe effect type of slide
     *
     * @return int mSwipeType
     */
    public int getSwipeType() {
        return mSwipeType;
    }

    /**
     * get priority of slide
     *
     * @return int mPriority
     */
    public int getPriority() {
        return mPriority;
    }

    /**
     * get parent of slide
     *
     * @return SVISlide mParent
     */
    public SVISlide getParent() {
        return mParent;
    }

    /**
     * adds a child slide
     *
     * @param childSlide > add specified child slide
     * @param index      > the position at which to add the child
     */
    public void addSlide(SVISlide childSlide, int index) {
        if (mNativeSlide == -1)
            return;

        if (childSlide == null)
            return;

        //Remove from previous parent
        SVISlide currentParent = childSlide.getParent();
        if(currentParent != null){
            currentParent.removeSlide(childSlide);
        } 

        int findIndex = 0;
        int childPriority = childSlide.getPriority();
        int slideCount = getSubSlideCount();

        if (index < 0 || index > slideCount)
            return;

        if (index < slideCount && childPriority == getSubSlide(index).getPriority()) {
            findIndex = index;
        } else {
            for (findIndex = 0; findIndex < slideCount; findIndex++) {
                SVISlide slide = getSubSlide(findIndex);

                if (childPriority < slide.getPriority()) {
                    break;
                }
            }
        }

        // Insert
        nativeAddSlideIndexSVISlide(mNativeSlide, childSlide.mNativeSlide, findIndex);
        mListSlide.add(findIndex, childSlide);
        childSlide.setParent(this);
        invalidate();
    }

    /**
     * adds a child slide
     *
     * @param childSlide > add specified child slide.
     */
    public void addSlide(SVISlide childSlide) {
        if (mNativeSlide == -1)
            return;

        if (childSlide == null)
            return;

        //Remove from previous parent
        SVISlide currentParent = childSlide.getParent();
        if(currentParent != null){
            currentParent.removeSlide(childSlide);
        }        

        int findIndex = 0;
        int childPriority = childSlide.getPriority();
        int slideCount = getSubSlideCount();

        for (findIndex = 0; findIndex < slideCount; findIndex++) {
            SVISlide slide = mListSlide.get(findIndex);

            if (childPriority < slide.getPriority()) {
                break;
            }
        }

        if (findIndex == slideCount) {    // Add
            nativeAddSlideSVISlide(mNativeSlide, childSlide.mNativeSlide);
            mListSlide.add(childSlide);
            childSlide.setParent(this);
            invalidate();
        } else if (findIndex < slideCount) {    // Insert
            //addSlide(childSlide, findIndex);
            nativeAddSlideIndexSVISlide(mNativeSlide, childSlide.mNativeSlide, findIndex);
            mListSlide.add(findIndex, childSlide);
            childSlide.setParent(this);
            invalidate();
        }
    }

    /**
     * removes the specified child slide
     *
     * @param removeSlide > remove specified child slide
     */
    public void removeSlide(SVISlide removeSlide) {
        if (mNativeSlide == -1)
            return;

        //final int slideCount = removeSlide.getSubSlideCount();

        mListSlide.remove(removeSlide);

        nativeRemoveSlideSVISlide(mNativeSlide, removeSlide.mNativeSlide);
        //nativeDestroySVISlide(removeSlide.mNativeSlide);

        removeSlide.setParent(null);
        invalidate();
    }

    /**
     * returns the specified child slide
     *
     * @param index > the index lists the child slide
     * @return SVISlide null or SVISlide
     */
    public SVISlide getSubSlide(int index) {
        if (index < 0 || index >= mListSlide.size())
            return null;

        return mListSlide.get(index);
    }

    /**
     * to specify a slide index returns.
     *
     * @param slide > find a slide
     * @return int 0..*
     */
    public int getSubSlideIndex(SVISlide slide) {
        return mListSlide.indexOf(slide);
    }

    /**
     * returns the count of child slides
     *
     * @return int 0..*
     */
    public int getSubSlideCount() {
        return mListSlide.size();
    }

    /**
     * detach the specified child slide
     *
     * @param detachSlide > detach specified child slide.
     */
    public void detachSlide(SVISlide detachSlide) {
        //final int slideCount = detachSlide.getSubSlideCount();

        mListSlide.remove(detachSlide);

        nativeRemoveSlideSVISlide(mNativeSlide, detachSlide.mNativeSlide);
        
        invalidate();
    }

    private boolean mIsBGColorChanged = false;
    private static final float[] SLIDE_BACKGROUND_SETTING = {1.0f, 1.0f, 1.0f, 1.0f};

    /**
     * set the image to the specified bitmap.
     *
     * @param bitmap > set to bitmap
     */
    public void setImage(Bitmap bitmap) {
        if (mNativeSlide == -1)
            return;

        if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        
        if (mImage == null) {
            mImage = _PackageAccessBasetype.createSVIImage(mAttachedSurface);
        }

        if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mImage.setBitmap(bitmap);

        nativeSetImageSVISlide(mNativeSlide, mImage.getNativeHandle());

        invalidate();
    }

    
    /**
     * set the normal map image to the specified bitmap.
     *
     * @param bitmap > set to bitmap
     */
    public void setNormalImage(Bitmap bitmap) {
        if (mNativeSlide == -1)
            return;

        if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        
        
        if (mNormalImage == null) {
        	mNormalImage = _PackageAccessBasetype.createSVIImage(mAttachedSurface);
        }

        if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mNormalImage.setBitmap(bitmap);

        nativeSetNormalImageSVISlide(mNativeSlide, mNormalImage.getNativeHandle());

        invalidate();
    }
    
    /**
     * set the specular map image to the specified bitmap.
     *
     * @param bitmap > set to bitmap
     */
    public void setSpecularImage(Bitmap bitmap) {
        if (mNativeSlide == -1)
            return;

        if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        
        
        if (mSpecularImage == null) {
        	mSpecularImage = _PackageAccessBasetype.createSVIImage(mAttachedSurface);
        }

        if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mSpecularImage.setBitmap(bitmap);

        nativeSetSpecularImageSVISlide(mNativeSlide, mSpecularImage.getNativeHandle());

        invalidate();
    }

    
    /**
     * set the back-face image to the specified bitmap.
     *
     * @param bitmap > set to bitmap
     */
    public void setBackFaceImage(Bitmap bitmap) {
        if (mNativeSlide == -1)
            return;

        if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
        
        
        if (mBackFaceImage == null) {
            mBackFaceImage = _PackageAccessBasetype.createSVIImage(mAttachedSurface);
        }

        if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mBackFaceImage.setBitmap(bitmap);

        nativeSetBackFaceImageSVISlide(mNativeSlide, mBackFaceImage.getNativeHandle());

        invalidate();
    }

    /**
     * set the back-face image flip flag.
     * 
     * @param isFlip > set to isFlip
     */
    public void setBackFaceImageFlip(boolean isFlip) {
    	
    	if(mNativeSlide == -1)
    		return;
    	
    	int flag = isFlip ? 1 : 0;
    	mEnableBackFaceImageFlip = isFlip;
    	nativeSetBackFaceImageFlipSVISlide(mNativeSlide, flag);
    	
    	invalidate();
    }
    
    /**
     * get the back-face image flip flag.
     * 
     * @return boolean flip flag(true/false)
     */
    public boolean getBackFaceImageFlip() {
    	return mEnableBackFaceImageFlip;
    }
    
    /**
     * set the back-face image clear flag.
     * 
     * @param isClear > set to isClear
     */
    public void setBackFaceImageClear(boolean isClear) {
    	if(mNativeSlide == -1)
    		return;
    	
    	int flag = isClear ? 1 : 0;
    	mEnableBackFaceImageClear = isClear;
    	nativeSetBackFaceImageClearSVISlide(mNativeSlide, flag);
    	
    	invalidate();
    }
    
    /**
     * set the back-face image clear flag.
     * 
     * @param isClear > set to isClear
     */
    public boolean getBackFaceImageClear() {
    	return mEnableBackFaceImageClear;
    }
    
    
    /**
     * set the image to the specified bitmap and alpha type
     *
     * @param bitmap    > set to bitmap
     * @param alphaType > set to alpha type(NORMAL, PREMULTIPLIED)
     */
    public void setImage(Bitmap bitmap, SVIImage.AlphaType alphaType) {
        if (mNativeSlide == -1)
            return;

        if (bitmap != null && (bitmap.getWidth() <= 0 || bitmap.getHeight() <= 0)){
        	return;
        }
                
        if (mImage == null) {
            mImage = _PackageAccessBasetype.createSVIImage(mAttachedSurface);
        }

        if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mImage.setBitmap(bitmap, alphaType);

        nativeSetImageSVISlide(mNativeSlide, mImage.getNativeHandle());

        invalidate();
    }

    /**
     * set the image to the specified image.
     *
     * @param image > set to image
     */
    public void setImage(SVIImage image) {
        if (image != null && mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mImage = image;

        if (image != null)
            nativeSetImageSVISlide(mNativeSlide, mImage.getNativeHandle());
        else
            nativeSetImageSVISlide(mNativeSlide, 0);
        
        invalidate();
    }
    
    /**
     * set the normal map image to the specified image.
     *
     * @param image > set to image
     */
    public void setNormalImage(SVIImage image) {
    	if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();
        mNormalImage = image;

        if (image != null)
            nativeSetNormalImageSVISlide(mNativeSlide, mNormalImage.getNativeHandle());
        else
            nativeSetNormalImageSVISlide(mNativeSlide, 0);
        
        invalidate();
    }
    
    /**
     * set the specular map image to the specified image.
     *
     * @param image > set to image
     */
    public void setSpecularImage(SVIImage image) {
    	if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();
    	
        mSpecularImage = image;

        if (image != null)
            nativeSetSpecularImageSVISlide(mNativeSlide, mSpecularImage.getNativeHandle());
        else
            nativeSetSpecularImageSVISlide(mNativeSlide, 0);
        
        invalidate();
    }

    /**
     * set the back-face image to the specified image.
     *
     * @param image > set to image
     */
    public void setBackFaceImage(SVIImage image) {
        if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mBackFaceImage = image;

        if (image != null)
            nativeSetBackFaceImageSVISlide(mNativeSlide, mBackFaceImage.getNativeHandle());
        else
            nativeSetBackFaceImageSVISlide(mNativeSlide, 0);
        
        invalidate();
    }

    /**
     * set the image to the specified image and alpha type
     *
     * @param image     > set to image
     * @param alphaType > set to alpha type(NORMAL, PREMULTIPLIED)
     */
    public void setImage(SVIImage image, SVIImage.AlphaType alphaType) {
        if (mBlendType == BlendType.BLEND_MULTIPLY)
            setBackgroundSetting();

        mImage = image;

        mImage.setAlphaType(alphaType);

        if (mImage != null)
            nativeSetImageSVISlide(mNativeSlide, mImage.getNativeHandle());
        else
            nativeSetImageSVISlide(mNativeSlide, 0);
        
        invalidate();
    }
    
       /**
     * get image of slide
     *
     * @return SVIImage mImage
     */
    public SVIImage getImage() {
        return mImage;
    }
    
    /**
     * get normal image of slide
     *
     * @return SVIImage mNormalImage
     */
    public SVIImage getNormalImage() {
        return mNormalImage;
    }
    
    /**
     * get specular image of slide
     *
     * @return SVIImage mSpecularImage
     */
    public SVIImage getSpecularImage() {
        return mSpecularImage;
    }

    /**
     * get back-face image
     *
     * @return SVIImage mBackFaceImage
     */
    public SVIImage getBackFaceImage() {
        return mBackFaceImage;
    }

    /**
     * the slide and the child shows the slide in relations between the log.
     */
    public void displayTrees() {
    	SVIEngineThreadProtection.validateMainThread();
        nativeDisplaySlideTreesSVISlide(mNativeSlide);
    }

    /**
     * the total number of slides.
     *
     * @param depth       > slide to retrieve the depth of the tree
     * @param visibleOnly > count only visible whether to add a slide
     * @return int 0..*
     */
    // nemustech
    public int dumpTrees(int depth, boolean visibleOnly) {
    	SVIEngineThreadProtection.validateMainThread();
        if (visibleOnly && mHidden) {
            return 0;
        }

        int total = 0;

        String depthStr = "";
        for (int i = 0; i < depth; i++) {
            depthStr += "  ";
        }

        int count = getSubSlideCount();
        total += count;

        if (depth == 0) {
            Log.i("#####", "slide count = " + count);
        }
        Log.i("#####", depthStr + this);

        for (int i = 0; i < count; i++) {
            total += getSubSlide(i).dumpTrees(depth + 1, visibleOnly);
        }

        if (depth == 0) {
            Log.i("#####", "total slide count = " + total);
        }

        return total;
    }

    /**
     * slide into a string in the visible region by combining the state and return.
     */
    @Override
    public String toString() {
        String str = "";
        str += "[" + mNativeSlide + "] ";
        if (mHidden) {
            str += "Hidden ";
        }
        str += "Region: " + (int) mRegion.mOrigin.mX + "," + (int) mRegion.mOrigin.mY + "-" + (int) mRegion.mSize.mWidth + "," + (int) mRegion.mSize.mHeight
        ;
        return str;
    }

    
    
    //2013-06-21 masterkeaton27@gmail.com
    public void addPathPoint(float x, float y, float z){
    	SVIEngineThreadProtection.validateMainThread();
        //2013-06-21 masterkeaton27@gmail.com
        nativeAddPathPoint(mNativeSlide, x, y, z);
    }
    
    //2013-06-21 masterkeaton27@gmail.com
    public void clearPathPoints(){
        nativeClearPathPoints(mNativeSlide);
    }
    
    /**
     * the return is being animated.
     *
     * @return boolean (true = animating)
     */
    public boolean isAnimating() {
    	SVIEngineThreadProtection.validateMainThread();
        return nativeIsSlideAnimating(mNativeSlide) == 0 ? false : true;
    }

    /**
     * returns the value of animations that have been setup.
     *
     * @return boolean (true = has the value of the animation.)
     */
    public boolean isSetAnimation() {
        if (mAnimation == null)
            return false;
        else
            return true;
    }

    /**
     * set animation
     *
     * @param animation > set the value of mAnimation
     */
    public void setAnimation(SVIAnimation animation) {
        mAnimation = animation;
    }

    /**
     * set explicit animation that is set to animation.
     */
    public void startAnimation() {
    	SVIEngineThreadProtection.validateMainThread();
        if (mAnimation != null) {
            int lightType = mAnimation.getLightType();
            int scaleType = mAnimation.getScaleType();

            if (lightType != LightType.NO_LIGHT)
                setLightType(lightType);
            if (scaleType == ImageScaleType.MATRIX)
                setImageScaleType(scaleType);

            mAnimation.registerListener();
            mAttachedSurface.getSlideManager().requestExplicitAnimation(this, mAnimation);
            mAnimation = null;
        }
    }

    /**
     * the explicit animation set to the specified animation.
     *
     * @param animation > set explicit animation
     */
    public void startAnimation(SVIAnimation animation) {
    	SVIEngineThreadProtection.validateMainThread();
        int lightType = animation.getLightType();
        int scaleType = animation.getScaleType();

        if (lightType != LightType.NO_LIGHT)
            setLightType(lightType);
        if (scaleType == ImageScaleType.MATRIX)
            setImageScaleType(scaleType);

        animation.registerListener();
        mAttachedSurface.getSlideManager().requestExplicitAnimation(this, animation);
        if (mAnimation != null) {
            mAnimation = null;
        }
    }

    /**
     * stop animation
     */
    public void stopAnimation() {
    	SVIEngineThreadProtection.validateMainThread();
        nativeStopAnimationSVISlide(mNativeSlide);
        mAttachedSurface.getSlideManager().requestRender();
    }

    /**
     * set rotation duration
     *
     * @param duration > set the value of rotationDuration
     */
    public void setRotationDuration(int duration) {
        rotationDuration = duration;
    }

    /**
     * get rotation duration
     *
     * @return int rotationDuration
     */
    public int getRotationDuration() {
        return rotationDuration;
    }

    /**
     * set 3d scene-node
     *
     * @param sceneNode
     * @return boolean (true = success, false = fail)
     */
    @Deprecated
    public boolean setSceneNode(SVISceneNode sceneNode) {
    	SVIEngineThreadProtection.validateMainThread();
        if (sceneNode.getId() == null || sceneNode.getId().equalsIgnoreCase(""))
            return false;

        int result = nativeLoad3dModelFileSVISlide(mNativeSlide, sceneNode.getId());

        if (result == -1)
            return false;

        setCapture(true);

        return true;
    }

    /**
     * remove scene-node
     */
    @Deprecated
    public void removeSceneNode() {
    	SVIEngineThreadProtection.validateMainThread();
        nativeRemoveSceneNodeSVISlide(mNativeSlide);
        setCapture(false);
        invalidate();
    }

    /**
     * set model position and implicit model-position animation
     *
     * @param x > set position value of the x-axis 3d model in the slide.
     * @param y > set position value of the y-axis 3d model in the slide.
     * @param z > set position value of the z-axis 3d model in the slide.
     */
    @Deprecated
    public void setModelPosition(float x, float y, float z) {
        setModelPosition(x, y, z, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set model position and implicit model-position animation
     *
     * @param x        > set position value of the x-axis 3d model in the slide.
     * @param y        > set position value of the y-axis 3d model in the slide.
     * @param z        > set position value of the z-axis 3d model in the slide.
     * @param duration > set implicit animation duration
     */
    @Deprecated
    public void setModelPosition(float x, float y, float z, int duration) {
        nativeSetModelPositionSVISlide(mNativeSlide, x, y, z);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.MODEL_POSITION, duration);
    }

    /**
     * set model rotation and implicit model-rotation animation
     *
     * @param x > set rotation value of the x-axis 3d model in the slide.
     * @param y > set rotation value of the y-axis 3d model in the slide.
     * @param z > set rotation value of the z-axis 3d model in the slide.
     */
    @Deprecated
    public void setModelRotation(float degreeX, float degreeY, float degreeZ) {
        setModelRotation(degreeX, degreeY, degreeZ, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set model rotation and implicit model-rotation animation
     *
     * @param x        > set rotation value of the x-axis 3d model in the slide.
     * @param y        > set rotation value of the y-axis 3d model in the slide.
     * @param z        > set rotation value of the z-axis 3d model in the slide.
     * @param duration > set implicit animation duration
     */
    @Deprecated
    public void setModelRotation(float degreeX, float degreeY, float degreeZ, int duration) {
        nativeSetModelRotationSVISlide(mNativeSlide, degreeX, degreeY, degreeZ);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.MODEL_ROTATION, duration);
    }

    /**
     * set model scale and implicit model-scale animation
     *
     * @param x > set scale value of the x-axis 3d model in the slide.
     * @param y > set scale value of the y-axis 3d model in the slide.
     * @param z > set scale value of the z-axis 3d model in the slide.
     */
    @Deprecated
    public void setModelScale(float x, float y, float z) {
        setModelScale(x, y, z, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set model scale and implicit model-scale animation
     *
     * @param x        > set scale value of the x-axis 3d model in the slide.
     * @param y        > set scale value of the y-axis 3d model in the slide.
     * @param z        > set scale value of the z-axis 3d model in the slide.
     * @param duration > set implicit animation duration
     */
    @Deprecated
    public void setModelScale(float x, float y, float z, int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeSetModelScaleSVISlide(mNativeSlide, x, y, z);

        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.MODEL_SCALE, duration);
    }


    protected void setImplicitAnimation(int type, int interpolatorType, int duration, int repeatCount, int offset, int mAutoReverse) {
    	SVIEngineThreadProtection.validateMainThread();
        int implcitDuration = 0;
        int interpolator = InterpolatorType.LINEAR;

        if (type == PropertyAnimationType.ROTATION_FROM) {
            setRotationDuration(duration);
        }

        if (mAttachedSurface.getSlideManager().isPausedImplicitAnimation() == true) {
            implcitDuration = getRotationDuration();

            // add duration
            if (implcitDuration == 0) {
                implcitDuration = duration;
            }

            interpolator = InterpolatorType.CUBICEASEINOUT;
        } else {
            implcitDuration = duration;
            interpolator = interpolatorType;
        }

        if (mAttachedSurface.getSlideManager().getAnimationSettingValue() == SVISlideManager.NO_ANIMATION) {
            nativeSetImplicitAnimationSVISlide(mNativeSlide, type, 0, 0, 0, 0, 0);
        } else {
            nativeSetImplicitAnimationSVISlide(mNativeSlide, type, interpolator, implcitDuration, repeatCount, offset, mAutoReverse);
        }
    }

    protected void setExplicitAnimation(SVIAnimation animation) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeSetExplicitAnimationSVISlide(mNativeSlide, animation.getNativeAnimationHandle());
    }

    /**
     * returns the topmost parent.
     *
     * @return SVISlide
     */
    public SVISlide findUltimateRoot() {
    	SVIEngineThreadProtection.validateMainThread();
        SVISlide root = mParent;
        if (root == null) return this;
        if (root.getParent() == null) return root;
        while (root.getParent() != null) {
            root = root.getParent();
        }
        return root;
    }

    /**
     * returns the final region.
     *
     * @return SVIRect
     */
    public SVIRect getGlobalRegion() {
    	SVIEngineThreadProtection.validateMainThread();
        SVISlide root = mParent;
        if (root == null) return getRegion();
        SVIRect region = new SVIRect();
        region.mOrigin.mX = getRegion().mOrigin.mX;
        region.mOrigin.mY = getRegion().mOrigin.mY;
        region.mSize.mWidth = getRegion().mSize.mWidth;
        region.mSize.mHeight = getRegion().mSize.mHeight;

        if (root.getParent() == null) {
            region.mOrigin.mX += root.getRegion().mOrigin.mX;
            region.mOrigin.mY += root.getRegion().mOrigin.mY;
            return region;
        }

        while (root.getParent() != null) {
            region.mOrigin.mX += root.getRegion().mOrigin.mX;
            region.mOrigin.mY += root.getRegion().mOrigin.mY;
            root = root.getParent();
        }

        return region;
    }

    private void updateGeometryByRegion() {
    	SVIEngineThreadProtection.validateMainThread();
        float dx = mRegion.mSize.mWidth * mPivotPoint.mX * mScale.mX;
        float dy = mRegion.mSize.mHeight * mPivotPoint.mY * mScale.mY;

        mPosition.mX = mRegion.mOrigin.mX + dx;
        mPosition.mY = mRegion.mOrigin.mY + dy;
    }

    private void updateGeometryByBoundsOrPosition() {
    	SVIEngineThreadProtection.validateMainThread();
        float dx = mRegion.mSize.mWidth * mPivotPoint.mX;
        float dy = mRegion.mSize.mHeight * mPivotPoint.mY;

        mRegion.mOrigin.mX = mPosition.mX - dx;
        mRegion.mOrigin.mY = mPosition.mY - dy;
    }

    private void updateGeometryByPivotPoint() {
    	SVIEngineThreadProtection.validateMainThread();
        float dx = mRegion.mSize.mWidth * mPivotPoint.mX;
        float dy = mRegion.mSize.mHeight * mPivotPoint.mY;

        mPosition.mX = mRegion.mOrigin.mX + dx;
        mPosition.mY = mRegion.mOrigin.mY + dy;
    }

    // 2011.09.26 alpha blending test
    private void setBackgroundSetting() {
        final float[] SLIDE_BACKGROUND_SETTING = {1.0f, 1.0f, 1.0f, 1.0f};

        float[] bgColor = SLIDE_BACKGROUND_SETTING;

        mBgColor.mR = bgColor[0];
        mBgColor.mG = bgColor[1];
        mBgColor.mB = bgColor[2];
        mBgColor.mA = bgColor[3];

        nativeSetBackgroundColorSVISlide(mNativeSlide, bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        mAttachedSurface.getSlideManager().requestImplicitAnimation(this, PropertyAnimationType.BG_COLOR, IMPLICIT_ANIMATION_OFF_DURATION);
    }

    /**
     * set implicit animation listener information in the slide.
     *
     * @param proxy      > set to native slide proxy value
     * @param listenerID > set to native slide listenerID value
     */
    public void setProxy(int proxy, int listenerID) {
    	SVIEngineThreadProtection.validateMainThread();
        nativeSetProxySVISlide(mNativeSlide, proxy, listenerID);
    }
    
    
    /**
     * create particleEffect from info and add to slide
     * 
     * @param info
     */
    public void addParticleEffect(SVIParticleEffect particleEffect) {
    	if(particleEffect != null) {
    		if(particleEffect.getType() == SVIParticleEffect.ParticleEffectType.PARTICLE_EFFECT_DEFAULT) {
    			nativeAddParticleEffectSVISlide( mNativeSlide, particleEffect.getNativeHandle() );
    		}
    		else if(particleEffect.getType() == SVIParticleEffect.ParticleEffectType.PARTICLE_EFFECT_KEYFRAME) {
    			// TODO : change native call
    			nativeAddParticleEffectSVISlide( mNativeSlide, particleEffect.getNativeHandle() );
    		}
    		else {
    			Log.e("SVI", "ParticleEffectInfo's type not supported !!!");
    		}
    		
    		invalidate();
    	}
    }

    /**
     * set native tesselation point list
     */
    public void setTesselationPointList(Vector<SVIVector2> pointList)
    {
    	//TODO: call JNI code
    	mTesselationPointList.clear();
    	mTesselationPointList.addAll(pointList);
    	SVIVector2[] pointArray = new SVIVector2[mTesselationPointList.size()];
    	mTesselationPointList.toArray(pointArray);
    	
    	/*// check real size
        float left, right, up, bottom;
        left = mRegion.mSize.mWidth; 
        up = mRegion.mSize.mHeight; 
        right = 0.0f; 
        bottom = 0.0f;

        for(int i = 0; i < mTesselationPointList.size(); ++i)
        {
            if(left > mTesselationPointList.get(i).mX) left = mTesselationPointList.get(i).mX;
            if(up > mTesselationPointList.get(i).mY) up = mTesselationPointList.get(i).mY;
            if(right < mTesselationPointList.get(i).mX) right = mTesselationPointList.get(i).mX;
            if(bottom < mTesselationPointList.get(i).mY) bottom = mTesselationPointList.get(i).mY;
        }
    	
        mRegion.mSize.mWidth = right - left;
        mRegion.mSize.mHeight = bottom - up;*/
        
    	nativeSetTesselationPointListSVISlide(mNativeSlide, pointArray);
    	invalidate();
    }
    
    
    /**
     * set native morphing radius
     */
    public void setMorphingRadius(float radius)
    {
    	mMorphingRadius = radius;
    	nativeSetMorphingRadiusSVISlide(mNativeSlide, radius);
    }
    
    
    /**
     * set natvie morphing geometry vertex count
     */
    public void setMorphingVertexCount(int count)
    {
    	mMorphingVetexNum = count;
    	nativeSetMorphingVertexCountSVISlide(mNativeSlide, mMorphingVetexNum);
    }
    
    /**
     * sets the color of morphing to the slide.
     *
     * @param morphingColor > set slide morphing-color(morphingColor[0] = red, morphingColor[1] = green, morphingColor[2] = blue, morphingColor[3] = alpha) value
     */
    public void setMorphingColor(float[] morphingColor) {
        mMorphingColor.mR = morphingColor[0];
        mMorphingColor.mG = morphingColor[1];
        mMorphingColor.mB = morphingColor[2];
        mMorphingColor.mA = morphingColor[3];
        nativeSetMorphingColorSVISlide(mNativeSlide, morphingColor[0], morphingColor[1], morphingColor[2], morphingColor[3]);
    }
    
    /**
     * remove tesselation point list
     */
    public void removeTesselationPointList()
    {
    	mTesselationPointList.clear();
    	//TODO: call JNI code
    }
    

    /*****************************************************************************************************************/
    /**
     * SVISlide native interface.
     *
     */
    static {
    	System.loadLibrary(SVIEngineDesc.mName);
    }

    public static native void nativeSetRootSlideSVISlide(int nativeSlide);//Multi-Instance-Support

    private native void nativeDisplaySlideTreesSVISlide(int nativeSlide);

    private native void nativeDestroySVISlide(int nativeSlide);

    private native int nativeInitializeSVISlide(int nativeGLSurface, int type, int parent, float x, float y, float width, float height, float[] color, Bitmap bitmap);

    private native void nativeInitRegionSVISlide(int nativeSlide, float x, float y, float width, float height);

    private native void nativeInitRotationSVISlide(int nativeSlide, float x, float y, float z, float w);

    private native void nativeInitScaleSVISlide(int nativeSlide, float x, float y, float z, float w);

    private native void nativeInitOpacitySVISlide(int nativeSlide, float opacity);

    private native void nativeInitBackgroundColorSVISlide(int nativeSlide, float r, float g, float b, float a);

    private native void nativeInitZPositionSVISlide(int nativeSlide, float zPosition);

    private native void nativeSetSwipeRatioSVISlide(int nativeSlide, float x, float y, float z);
    
    private native void nativeSetDeformRatioSVISlide(int nativeSlide, float x, float y, float z);

    private native void nativeSetRTERatioSVISlide(int nativeSlide, float x, float y, float z);

    private native void nativeSetRegionSVISlide(int nativeSlide, float x, float y, float width, float height);

    private native void nativeSetBoundsSVISlide(int nativeSlide, float x, float y, float width, float height);

    private native void nativeSetPositionSVISlide(int nativeSlide, float x, float y);

    private native void nativeSetPivotPointSVISlide(int nativeSlide, float x, float y);

    private native void nativeSetZPositionSVISlide(int nativeSlide, float zPosition);

    private native void nativeSetRotationSVISlide(int nativeSlide, float x, float y, float z, float w);

    private native void nativeSetScaleSVISlide(int nativeSlide, float x, float y, float z);

    private native void nativeSetBackgroundColorSVISlide(int nativeSlide, float r, float g, float b, float a);

    private native void nativeSetOpacitySVISlide(int nativeSlide, float opacity);

    private native void nativeSetCornerRadiusSVISlide(int nativeSlide, float cornerRadius);

    private native void nativeSetBorderWidthSVISlide(int nativeSlide, float borderWidth);

    private native void nativeSetBorderColorSVISlide(int nativeSlide, float r, float g, float b, float a);

    private native void nativeSetShadowRadiusSVISlide(int nativeSlide, float shadowRadius);

    private native void nativeSetShadowColorSVISlide(int nativeSlide, float r, float g, float b, float a);

    private native void nativeSetShadowOpacitySVISlide(int nativeSlide, float shadowOpacity);

    private native void nativeSetShadowOffsetSVISlide(int nativeSlide, float width, float height);


    private native void nativeSetLightPowerSVISlide(int nativeSlide, float lightPower);

    private native void nativeSetLightAngleSVISlide(int nativeSlide, float lightAngle);

    private native void nativeSetLightRadiusSVISlide(int nativeSlide, float lightRadius);

    private native void nativeSetLightOpacitySVISlide(int nativeSlide, float lightOpacity);

    private native void nativeSetLightOffsetSVISlide(int nativeSlide, float x, float y);

    private native void nativeSetLightColorSVISlide(int nativeSlide, float r, float g, float b, float a);
    
    private native void nativeSetLightDirectionSVISlide(int nativeSlide, float x, float y, float z, float w);
    
    private native void nativeSetViewDirectionSVISlide(int nativeSlide, float x, float y, float z, float w);

    private native void nativeSetLightTypeSVISlide(int nativeSlide, int type);

    private native void nativeSetScaleToFitTypeSVISlide(int nativeSlide, int type);

    private native void nativeSetTextureScaleToFitRegionSVISlide(int nativeSlide, float x, float y, float width, float height);

    private native void nativeSetTextureRegionSVISlide(int nativeSlide, float x, float y, float width, float height);

    private native void nativeSetBackFaceTextureRegionSVISlide(int nativeSlide, float x, float y, float width, float height);

    private native void nativeSetRotationFromSVISlide(int nativeSlide, float fx, float fy, float fz, float fw,
                                                            float tx, float ty, float tz, float tw);

    private native void nativeSetHiddenSVISlide(int nativeSlide, boolean hidden);

    private native void nativeSetSwipeTypeSVISlide(int nativeSlide, int type);
    
    private native void nativeSetDeformTypeSVISlide(int nativeSlide, int type);

    private static native void nativeSetMorphingTypeSVISlide(int nativeSlide, int type);

    private native void nativeSetFilterTypeSVISlide(int nativeSlide, int type);
    
    private native void nativeSetDeformDirectionSVISlide(int nativeSlide, int direction);

    private native void nativeSetZOrderTypeSVISlide(int nativeSlide, int type);

    private native void nativeSetClipSubSlidesSVISlide(int nativeSlide, boolean clipSubSlides);

    private native void nativeSetOrthogonalSVISlide(int nativeSlide, boolean orthogonal);
    
    private native void nativeSetUseNormalMapSVISlide(int nativeSlide, boolean usenormalmap);
    
    private native void nativeSetCustomRTESVISlide(int nativeSlide, boolean customRTE);

    private native void nativeSetHoldOpacitySVISlide(int nativeSlide, boolean holdOpacity);

    private native void nativeSetHoldScaleSVISlide(int nativeSlide, boolean holdScale);

    private native void nativeSetCaptureSVISlide(int nativeSlide, boolean capture);

    private native void nativeSetBlendTypeSVISlide(int nativeSlide, int blendType);

    private native void nativeSetRTEffectTypeSVISlide(int nativeSlide, int effectType);

    //2011.12.09 jongchae.moon
    private native void nativeSetAntiAliasing(int nativeSlide, boolean enable);
    //

    private native void nativeAddSlideIndexSVISlide(int parentSlide, int childSlide, int index);

    private native void nativeAddSlideSVISlide(int parentSlide, int childSlide);

    private native void nativeRemoveSlideSVISlide(int parentSlide, int removeSlide);

    private native void nativeRemoveAllSlidesSVISlide(int viewGroupSlide);

    private native void nativeRemoveSlideAtSVISlide(int viewGroupSlide, int index);

    private native void nativeRemoveSlidesSVISlide(int viewGroupSlide, int start, int count);

    protected native void nativeSetBitmapSVISlide(int nativeSlide, Bitmap bitmap);

    private native void nativeSetImageSVISlide(int nativeSlide, int imageHandle);
    
    private native void nativeSetNormalImageSVISlide(int nativeSlide, int imageHandle);
    
    private native void nativeSetSpecularImageSVISlide(int nativeSlide, int imageHandle);

    private native void nativeSetBackFaceImageSVISlide(int nativeSlide, int imageHandle);
    
    private native void nativeSetBackFaceImageFlipSVISlide(int nativeSlide, int isFlip);
    
    private native void nativeSetBackFaceImageClearSVISlide(int nativeSlide, int isClear);

    private native void nativeStartAnimationSVISlide(int nativeSlide, int aniSet);

    private native void nativeStopAnimationSVISlide(int nativeSlide);

    private native void nativeRunExplicitAnimationSVISlide(int slideHandle, int anmationHandle);

    private native void nativeRunImplicitAnimationSVISlide(int slideHandle, int[][] implicitProperty);

    private native void nativeSetImplicitAnimationSVISlide(int slideHandle, int type, int interpolatorType, int duration, int repeatCount, int offset, int AutoReverse);

    private native void nativeSetExplicitAnimationSVISlide(int slideHandle, int animationHandle);

    private native void nativeSetProxySVISlide(int slideHandle, int proxyHandle, int listenerID);

    private native int nativeIsSlideAnimating(int slideHandle);
    
    //2013-06-21 masterkeaton27@gmail.com
    private native void nativeAddPathPoint(int slideHandle, float x, float y, float z);
    
    //2013-06-21 masterkeaton27@gmail.com
    private native void nativeClearPathPoints(int slideHandle);
    
    /// Related to the method declaration SVISceneNode.
    private native int nativeLoad3dModelFileSVISlide(int parentSlide, String fileName);

    private native int nativeRemoveSceneNodeSVISlide(int nativeSlide);

    private native void nativeSetModelPositionSVISlide(int nativeSlide, float x, float y, float z);

    private native void nativeSetModelRotationSVISlide(int nativeSlide, float x, float y, float z);

    private native void nativeSetModelScaleSVISlide(int nativeSlide, float x, float y, float z);
    
    private native void nativeAddParticleEffectSVISlide(int nativeSlide, int effectHandle);
    
    //morphing method.
    private static native void nativeSetTesselationPointListSVISlide(int nativeSlide, SVIVector2[] pointList);
    
    private static native void nativeSetMorphingRadiusSVISlide(int nativeSlide, float radius);
    
    private static native void nativeSetMorphingVertexCountSVISlide(int nativeSlide, int vertexCount);
    
    private static native void nativeSetMorphingColorSVISlide(int nativeSlide, float r, float g, float b, float a);
}
