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
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide.ImageScaleType;


/** 
 * SVIKeyFrameAnimation Class
 * This Class is make animation use Key frame. you can use key frame time ( 0.0f ~ 1.0f) to control animation
 * Key frame must bigger than past key frame value. (etc. 0.0f -> 0.5f -> 1.0f)
 */
public class SVIKeyFrameAnimation extends SVIPropertyAnimation {
	public SVIKeyFrameAnimation(int type) {
		this(null, type);
	}
	
	public SVIKeyFrameAnimation(int type, int lightType) {
		this(null, type, lightType);
	}
    
    /**
     * constructor
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     */
    SVIKeyFrameAnimation(SVIGLSurface saglSurface, int type) {
    	super(saglSurface);
    	
        initializeKeyFrameAnimation(type);
    }

    /** 
     * constructor use LightType
     *
     * @see SVIPropertyAnimation
     * @param type > property type
     * @param lightType > @see LightType
     */
    SVIKeyFrameAnimation(SVIGLSurface saglSurface, int type, int lightType) {
    	super(saglSurface);
    	
        mLightType = lightType;
        initializeKeyFrameAnimation(type);
    }

    /** 
     * initialize
     * 
     * @see SVIPropertyAnimation
     * @param type > property type
     */
    public void initializeKeyFrameAnimation(int type){
    	SVIEngineThreadProtection.validateMainThread();
        if (type == PropertyAnimationType.SCALETOFIT_REGION)
            mScaleType = ImageScaleType.MATRIX;
        mAnimationType = type;
        mClassType = SVIAnimationClass.KEYFRAME;
        mNativeAnimation = nativeCreateSVIKeyFrameAnimation(type, mSVIGLSurface.getNativeHandle());
    }

    /**
     * add key property use float
     * 
     * @param keyTime > 0.0f ~ 1.0f
     * @param value > float (etc. opacity ...)
     */
    public void addKeyProperty(float keyTime, float value) {
    	SVIEngineThreadProtection.validateMainThread();
        float[] data = {value};
        nativeAddKeyProperty(mNativeAnimation, mAnimationType, keyTime, data);
    }

    /** 
     * add key property use SVIPoint
     *
     * @param keyTime > 0.0f ~ 1.0f
     * @param point > SVIPoint (etc. point ... )
     */
    public void addKeyProperty(float keyTime, SVIPoint point) {
    	SVIEngineThreadProtection.validateMainThread();
        float[] data = {point.mX, point.mY};
        nativeAddKeyProperty(mNativeAnimation, mAnimationType, keyTime, data);
    }

    /** 
     * add key property use SVIRect
     * 
     * @param keyTime > 0.0f ~ 1.0f
     * @param rect > SVIRect (etc. region ... )
     */
    public void addKeyProperty(float keyTime, SVIRect rect) {
    	SVIEngineThreadProtection.validateMainThread();
        float[] data = {rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth, rect.mSize.mHeight};
        nativeAddKeyProperty(mNativeAnimation, mAnimationType, keyTime, data);
    }

    /** 
     * add key property use SVIColor
     *
     * @param keyTime >  0.0f ~ 1.0f
     * @param color > SVIColor (etc. color ... )
     */
    public void addKeyProperty(float keyTime, SVIColor color) {
    	SVIEngineThreadProtection.validateMainThread();
        float[] data = {color.mR, color.mG, color.mB, color.mA};
        nativeAddKeyProperty(mNativeAnimation, mAnimationType, keyTime, data);
    }	

    /** 
     * add key property use SVIVector3
     * 
     * @param keyTime >  0.0f ~ 1.0f
     * @param value > SVIVector3 (etc. rotation ... )
     */
    public void addKeyProperty(float keyTime, SVIVector3 value) {
    	SVIEngineThreadProtection.validateMainThread();
        float[] data = {value.mX, value.mY, value.mZ};
        nativeAddKeyProperty(mNativeAnimation, mAnimationType, keyTime, data);
    }

    /** 
     * add key property use SVIVector4
     * 
     * @param keyTime >  0.0f ~ 1.0f
     * @param value > SVIVector4 (etc. rotation ... )
     */
    public void addKeyProperty(float keyTime, SVIVector4 value) {
    	SVIEngineThreadProtection.validateMainThread();
        float[] data = {value.mX, value.mY, value.mZ, value.mW};
        nativeAddKeyProperty(mNativeAnimation, mAnimationType, keyTime, data);
    }
    
    /** 
     * add key property use float[]
     * 
     * @param keyTime >  0.0f ~ 1.0f
     * @param value > float array (etc. color ... )
     */
    public void addKeyProperty(float keyTime, float[] value) {
    	SVIEngineThreadProtection.validateMainThread();
        //float[] data = {value.mX, value.mY, value.mZ};
        nativeAddKeyProperty(mNativeAnimation, mAnimationType, keyTime, value);
    }
    
    /** 
     * finalize
     * @see com.github.sviengine.animation.SVIAnimation#finalize()
     */
    protected void finalize() throws Throwable {
    	super.finalize();
    	deleteNativeAnimationHandle();
    }
    
  
    /** SVIKeyFrameAnimation native interface.*/
    static {
    	System.loadLibrary(SVIEngineDesc.mName);
    }

   
    private static native int nativeCreateSVIKeyFrameAnimation(int type, int nativeGLSurface);
    private static native void nativeAddKeyProperty(int aniHandle, int type, float keyTime, float[] value);
}
