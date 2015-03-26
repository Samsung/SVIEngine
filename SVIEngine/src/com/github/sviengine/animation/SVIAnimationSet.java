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

import java.util.ArrayList;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide.ImageScaleType;
import com.github.sviengine.slide.SVISlide.LightType;

/** 
 * SVIAnimation Set Class
 * This class is Animation Set.
 * If you make animation not only one (two more) for slide, than can put together to only one animation set.
 */
public class SVIAnimationSet extends SVIAnimation {
	 public SVIAnimationSet() {
		 this((SVIGLSurface)null);
	 }
	 
	 public SVIAnimationSet(String tag) {
		 this(null, tag);
	 }
	 
	 public SVIAnimationSet(int duration, int repeatCount, boolean autoReverse, int offset) {
		 this(null, duration, repeatCount, autoReverse, offset);
	 }
	 
	 public SVIAnimationSet(int duration, int repeatCount, boolean autoReverse, int offset, String tag) {
		 this(null, duration, repeatCount, autoReverse, offset, tag);
	 }

	/**
	 * default constructor
	 */
    SVIAnimationSet(SVIGLSurface saGLSurface) {
    	super(saGLSurface);
    	
        initializeSVIAnimationSet("default");
        nativeSetPropertySVIAnimationSet(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * constructor with tag
     * 
     * @param tag > animation tag string
     */
    SVIAnimationSet(SVIGLSurface saGLSurface, String tag) {
    	super(saGLSurface);
    	
        initializeSVIAnimationSet(tag);		
        nativeSetPropertySVIAnimationSet(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * constructor use property ( duration, repeatCount, auto Reverse, offset)
     * 
     * @param duration > animation duration time
     * @param repeatCount > animation repeat count
     * @param autoReverse > animation auto reverse true = on false = off
     * @param offset > animation offset time
     */
    SVIAnimationSet(SVIGLSurface saGLSurface, int duration, int repeatCount, boolean autoReverse, int offset) {
        initializeSVIAnimationSet("default");

        mDuration = duration;
        if (repeatCount == -1) {
            mRepeatCount = unlimitedRepeat;
        }
        else {
            mRepeatCount = repeatCount + defaultRepeatCount;
        }
        mAutoReverse = autoReverse;
        mOffset = offset;

        nativeSetPropertySVIAnimationSet(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * constructor use property and tag
     * 
     * @param duration > animation duration time
     * @param repeatCount > animation repeat count
     * @param autoReverse > animation auto reverse true = on false = off
     * @param offset > animation offset time
     * @param tag > animation tag string
     */
    SVIAnimationSet(SVIGLSurface saGLSurface, int duration, int repeatCount, boolean autoReverse, int offset, String tag) {
        initializeSVIAnimationSet(tag);

        mDuration = duration;
        if (repeatCount == -1) {
            mRepeatCount = unlimitedRepeat;
        }
        else {
            mRepeatCount = repeatCount + defaultRepeatCount;
        }
        mAutoReverse = autoReverse;
        mOffset = offset;

        nativeSetPropertySVIAnimationSet(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * initialize
     * 
     * @param tag > animation tag string
     */
    public void initializeSVIAnimationSet(String tag) {
    	SVIEngineThreadProtection.validateMainThread();
        mAnimationList = new ArrayList<SVIAnimation>();

        mClassType = SVIAnimationClass.ANIMATION_SET;
        mNativeAnimation = nativeCreateSVIAnimationSet(mSVIGLSurface.getNativeHandle());
    }
	
    /**
     * finalize 
     */
    protected void finalize() throws Throwable {
    	super.finalize();
        deleteNativeAnimationHandle(); 
    }
    
    /** 
     * add animation
     * 
     * @param animation > maked SVIAnimation
     * @return {@link SVIAnimationSet#nativeAddAnimationSVIAnimationSet(int, int)}
     */
    public boolean addAnimation(SVIAnimation animation) {
    	SVIEngineThreadProtection.validateMainThread();
        if (animation.getLightType() != LightType.NO_LIGHT)
            mLightType = animation.getLightType();
        if (animation.getScaleType() == ImageScaleType.MATRIX)
            mScaleType = animation.getScaleType();
        
        mAnimationList.add(animation);
        return (nativeAddAnimationSVIAnimationSet(mNativeAnimation, animation.mNativeAnimation));
    }

    /** 
     * remove animation
     * 
     * @param animation > included SVIAnimation
     */
    public void removeAnimation(SVIAnimation animation) {
    	SVIEngineThreadProtection.validateMainThread();
        if (mAnimationList.contains(animation) == false)
            return;
        
        mAnimationList.remove(animation);
        nativeRemoveAnimationSVIAnimationSet(mNativeAnimation, animation.mNativeAnimation);
    }
    
    /** 
     * clear animation
     *  if set has animation, remove included all animation.
     *  
     *  @see SVIAnimationSet#removeAnimation(SVIAnimation);
     */
    public void clearAnimation() {
        SVIAnimation animation;

        for (int i = 0; i <mAnimationList.size(); i++) {
            animation = mAnimationList.get(i);
            nativeRemoveAnimationSVIAnimationSet(mNativeAnimation, animation.mNativeAnimation);
        }		
        mAnimationList.clear();
    }

    /** 
     * set InterpolatorType
     * 
     * @see InterpolatorType 
     * @param interpolatorType > animation set interpolator
     */
    public void setAnimationSetInterpolator(int interpolatorType) {
        mInterpolatorType = interpolatorType;

        int i;
        SVIAnimation aniHandle;

        for (i = 0; i <mAnimationList.size(); i++) {
            aniHandle = mAnimationList.get(i);
            aniHandle.setInterpolator(mInterpolatorType);
        }
    }
    
    /** 
     * get share InterpolatorType
     * 
     * @return int mShareInterpolator
     */
    public boolean getShareInterpolator() {
        return mShareInterpolator;
    }
    
    /** 
     * set share InterpolatorType
     * 
     * @param flag > true = on false = off
     */
    public void setShareInterpolator(boolean flag) {
        mShareInterpolator = flag;
    }
    
    /** 
     * set share animation information
     * 
     * @param flag > true = on false = off
     */
    public void setShareAnimationInfo(boolean flag) {
    	SVIEngineThreadProtection.validateMainThread();
		mShareAnimationInfo = flag;
		int syncInfo = mShareAnimationInfo==true?1:0;
		nativeShareAnimationInfo(mNativeAnimation ,syncInfo);
    }
    
    /** 
     * is share animation
     * if shared, return true
     * 
     * @return boolean mShareAnimationInfo
     */
    public boolean isShareAnimationInfo() {
    	return mShareAnimationInfo;
    }
    
    private boolean mShareInterpolator = false;
    private boolean mShareAnimationInfo = true;

    private ArrayList<SVIAnimation> mAnimationList;

    
    /**SVIAnimationSet native interface*/
    static {
    	System.loadLibrary(SVIEngineDesc.mName);
    }

    private static native int nativeCreateSVIAnimationSet(int nativeGLSurface);	
    private static native void nativeShareAnimationInfo(int aniSetHandle, int syncInfo);
    private static native boolean nativeAddAnimationSVIAnimationSet(int aniSetHandle, int animation);
    private static native void nativeRemoveAnimationSVIAnimationSet(int aniSetHandle, int animation);
    private static native void nativeSetPropertySVIAnimationSet(int aniSetHandle, int duration, int repeatCount, boolean autoReverse, int offset);
}