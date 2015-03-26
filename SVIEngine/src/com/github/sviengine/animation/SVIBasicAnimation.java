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
package com.github.sviengine.animation;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide.ImageScaleType;

/** SVIBasicAnimation Class
 * SVIBasicAnimation extends {@link SVIPropertyAnimation}
 */
public class SVIBasicAnimation extends SVIPropertyAnimation {
	
	public SVIBasicAnimation(int type, SVIVector3 from, SVIVector3 to) {
		this(null, type, from, to);
	}
	public SVIBasicAnimation(int type, SVIColor from, SVIColor to) {
		this(null, type, from, to);
	}
	public SVIBasicAnimation(int type, SVIRect from, SVIRect to) {
		this(null, type, from, to);
	}
	public SVIBasicAnimation(int type, SVIPoint from, SVIPoint to, int lightType) {
		this(null, type, from, to, lightType);
	}
	public SVIBasicAnimation(int type, SVIPoint from, SVIPoint to) {
		this(null, type, from, to);
	}
	public SVIBasicAnimation(int type, float from, float to, int lightType) {
		this(null, type, from, to, lightType);
	}
	public SVIBasicAnimation(int type, float from, float to) {
		this(null, type, from, to);
	}
	public SVIBasicAnimation(int type, float from[], float to[], int lightType) {
		this(null, type, from, to, lightType);
	}
	public SVIBasicAnimation(int type, float from[], float to[]) {
		this(null, type, from, to);
	}
    
    /** 
     * constructor
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > float array (etc. SVIColor)
     * @param to > float array
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, float from[], float to[]) {
    	super(saglSurface);
    	
        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, from, to);
    }

    /**
     * constructor use float[]
     * 
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > float array (etc. SVIColor ...)
     * @param to > float array
     * @param lightType > @see LightType
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, float from[], float to[], int lightType) {
    	super(saglSurface);
    	
        mLightType = lightType;

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, from, to);
    }

    /**
     * constructor use float
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > float value (etc. Opacity ...)
     * @param to  > float value
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, float from, float to) {
    	super(saglSurface);
    	
        float[] fromData = {from};
        float[] toData = {to};

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, fromData, toData);
    }

    /**
     * constructor use float and LightType
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > float (etc. Opacity ...)
     * @param to  > float
     * @param lightType > @see LightType
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, float from, float to, int lightType) {
    	super(saglSurface);
    	
        float[] fromData = {from};
        float[] toData = {to};

        mLightType = lightType;

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, fromData, toData);
    }

    /**
     * constructor use SVIPoint
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > SVIPoint (etc. pivot ...)
     * @param to > SVIPoint
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, SVIPoint from, SVIPoint to) {
    	super(saglSurface);
    	
        float[] fromData = {from.mX, from.mY};
        float[] toData = {to.mX, to.mY};

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, fromData, toData);
    }

    /**
     * constructor use SVIPoint and LightType
     * 
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > SVIPoint (etc. pivot ...)
     * @param to > SVIPoint
     * @param lightType > @see LightType
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, SVIPoint from, SVIPoint to, int lightType) {
    	super(saglSurface);
    	
        float[] fromData = {from.mX, from.mY};
        float[] toData = {to.mX, to.mY};

        mLightType = lightType;

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, fromData, toData);
    }

    /**
     * constructor use SVIRect
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > SVIRect (etc. region ...)
     * @param to > SVIRect
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, SVIRect from, SVIRect to) {
    	super(saglSurface);
    	
        float[] fromData = {from.mOrigin.mX, from.mOrigin.mY, from.mSize.mWidth, from.mSize.mHeight};
        float[] toData = {to.mOrigin.mX, to.mOrigin.mY, to.mSize.mWidth, to.mSize.mHeight};

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, fromData, toData);
    }

    /**
     * constructor use SVIColor
     * 
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > SVIColor (etc. color ...)
     * @param to > SVIColor
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, SVIColor from, SVIColor to) {
    	super(saglSurface);
    	
        float[] fromData = {from.mR, from.mG, from.mB, from.mA};
        float[] toData = {to.mR, to.mG, to.mB, to.mA};

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, fromData, toData);
    }

    /**
     * constructor use SVIVector3
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param from > SVIVector3 (etc. rotation ...)
     * @param to > SVIVector3
     */
    SVIBasicAnimation(SVIGLSurface saglSurface, int type, SVIVector3 from, SVIVector3 to) {
    	super(saglSurface);
    	
        float[] fromData = {from.mX, from.mY, from.mZ};
        float[] toData = {to.mX, to.mY, to.mZ};

        initializeSVIBasicAnimation(type);
        nativeSetValueSVIBasicAnimation(mNativeAnimation, mAnimationType, fromData, toData);
    }

    /**
     * initialize
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     */
    public void initializeSVIBasicAnimation(int type) {
    	SVIEngineThreadProtection.validateMainThread();
        if (type == PropertyAnimationType.SCALETOFIT_REGION)
            mScaleType = ImageScaleType.MATRIX;        
        mAnimationType = type;
        mClassType = SVIAnimationClass.BASIC;
        mNativeAnimation = nativeCreateSVIBasicAnimation(type, mSVIGLSurface.getNativeHandle());
    }
    
    /** 
     * finalize
     * 
     * @see com.github.sviengine.animation.SVIAnimation#finalize()
     */
    protected void finalize() throws Throwable {
    	super.finalize();
    	deleteNativeAnimationHandle();
    }
    
   /** SVIBasicAnimation native interface */
    static {
    	System.loadLibrary(SVIEngineDesc.mName);
    }

    private static native int nativeCreateSVIBasicAnimation(int type, int nativeGLSurface);
    private static native void nativeSetValueSVIBasicAnimation(int aniHandle, int type, float[] from, float[] to);
}