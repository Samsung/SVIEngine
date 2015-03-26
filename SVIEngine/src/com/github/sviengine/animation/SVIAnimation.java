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
import java.util.HashMap;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide.ImageScaleType;
import com.github.sviengine.slide.SVISlide.LightType;

import android.os.Handler;
import android.os.Message;
import android.util.Log;
/** 
 * SVIAnimation Class
 * include control animation property and type.
 */
public class SVIAnimation {
    final int defaultDuration = 250;
    final int defaultRepeatCount = 1;
    final int unlimitedRepeat = 0xffffffff;
    final boolean defaultAutoReverse = false;
    final int defaultOffset = 0;

    protected int mNativeAnimation;
    protected int mInterpolatorType;
    protected int mDuration;
    protected int mRepeatCount;	
    protected int mOffset;
    protected boolean mAutoReverse;
    protected int mLightType;
    protected int mScaleType;
    protected int mClassType;
    private ArrayList<SVIAnimationSet> mAnimationSet;
     
    protected String mTag;
    protected SVIAnimationListener mListener;
    static private int mListenerCounter = 0;
    static protected int mExplicitProxy = 0;
    static protected int mImplicitProxy = 0;
    static protected HashMap<Integer, SVIAnimationListener> mListenerMap;
    static protected HashMap<Integer, String> mTagMap;

    /**
     * InterpolatorType
     * this type control the animation speed except time.
     * for example LINEAR is just linear. but ACCELERATE is faster than past.
     * */
    public interface InterpolatorType {
        public static final int LINEAR = 0;
        public static final int ACCELERATE = 1;
        public static final int DECELERATE = 2;
        public static final int ACCELERATE_DECELERATE = 3;
        public static final int ANTICIPATE = 4;
        public static final int ANTICIPATE_OVERSHOOT = 5;
        public static final int BOUNCE = 6;
        public static final int OVERSHOOT = 7;
        public static final int CYCLE = 8;
        public static final int BACKEASEIN = 9;
        public static final int BACKEASEINOUT = 10;
        public static final int BACKEASEOUT = 11;
        public static final int BOUNCEEASEIN = 12;
        public static final int BOUNCEEASEINOUT = 13;
        public static final int BOUNCEEASEOUT = 14;
        public static final int CIRCULAREASEIN = 15;
        public static final int CIRCULAREASEINOUT = 16;
        public static final int CIRCULAREASEOUT = 17;
        public static final int CUBICEASEIN = 18;
        public static final int CUBICEASEINOUT = 19;
        public static final int CUBICEASEOUT = 20;
        public static final int ELASTICEASEIN = 21;
        public static final int ELASTICEASEINOUT = 22;
        public static final int ELASTICEASEOUT = 23;
        public static final int EXPONENTIALEASEIN = 24;
        public static final int EXPONENTIALEASEINOUT = 25;
        public static final int EXPONENTIALEASEOUT = 26;
        public static final int QUADEASEIN = 27;
        public static final int QUADEASEINOUT = 28;
        public static final int QUADEASEOUT = 29;
        public static final int QUARTEASEIN = 30;
        public static final int QUARTEASEINOUT = 31;
        public static final int QUARTEASEOUT = 32;
        public static final int QUINTEASEIN = 33;
        public static final int QUINTEASEINOUT = 34;
        public static final int QUINTEASEOUT = 35;
        public static final int SINEEASEIN = 36;
        public static final int SINEEASEINOUT = 37;
        public static final int SINEEASEOUT = 38;
        public static final int STRONGEASEIN = 39;
        public static final int STRONGEASEINOUT = 40;
        public static final int STRONGEASEOUT = 41;
    }
    
      
    
    /**SVIAnimationClassType*/
    public interface SVIAnimationClass {
        public static final int NONE = 0;
        public static final int BASIC = 1;
        public static final int KEYFRAME = 2;
        public static final int TRANSITION = 3;
        public static final int SPRITE = 4;
        public static final int ANIMATION_SET = 5;
    }
    
    SVIGLSurface mSVIGLSurface;
    
    public SVIAnimation() {
    	this(null);
    }
    
    /** 
     * default constructor
     */
    public SVIAnimation(SVIGLSurface saGLSurface) {
    	mSVIGLSurface = SVIGLSurface.getSurface(saGLSurface);
    	mSVIGLSurface.incrementUsageCount(this);
    	
        if(mListenerMap == null) {
            mListenerMap = new HashMap<Integer, SVIAnimationListener>();
        }

        if (mTagMap == null) {
            mTagMap = new HashMap<Integer, String>();
        }

        initializeSVIAnimation();
    }
    
    /** 
     * initialize
     */
    public void initializeSVIAnimation() {
        mNativeAnimation = 0;
        mInterpolatorType = InterpolatorType.LINEAR;
        mDuration = defaultDuration;
        mRepeatCount = defaultRepeatCount;
        mAutoReverse = defaultAutoReverse;
        mOffset = defaultOffset;
        mLightType = LightType.NO_LIGHT;
        mScaleType = ImageScaleType.FIT_XY;
        mClassType = 0;
        mListener = null;
    }

    /** 
     * finalize animation
     */
    protected void finalize() throws Throwable {
    	mSVIGLSurface.decrementUsageCount(this);
    	super.finalize();
    }
    
    /** 
     * set InterpolatorType for animation.
     * 
     * @see InterpolatorType
     * @param interpolatorType > interpolatertype
     */
    public void setInterpolator(int interpolatorType) {
    	SVIEngineThreadProtection.validateMainThread();
        mInterpolatorType = interpolatorType;
        nativeSetInterpolatorSVIAnimation(mNativeAnimation, mInterpolatorType);
    }
    
   /** 
    * get interpolator Type
    * 
    * @see InterpolatorType
    * @return int mInterpolatorType
    */
    public int getInterpolator() {
        return mInterpolatorType;
    }
    
    /** 
     * get mOffset 
     *
     * @return int mOffset
     */
     public int getOffset() {
         return mOffset;
     }
     
     
     /** 
      * get mTAG 
      *
      * @return String tag
      */
      public String getTag() {
          return mTag;
      }
     /** 
      * get mRepeatCount 
      * 
      * @return int count set by the user
      */
      public int getRepeatCount() {
          return (mRepeatCount - defaultRepeatCount);
      }
      
      /** 
       * get mDuration info
       * 
       * @return int mDuration
       */
       public int getDuration() {
           return mDuration;
       }
       
       /** 
        * check if animation is AutoReverse
        * 
        * @return boolean mAutoReverse
        */
        public boolean isAutoReverse() {
            return mAutoReverse;
        }

    /**
     * set animation duration Time.
     * duration time is animation's running time.
     * 
     * @param duration > animation duration time
     */
    public void setDuration(int duration) {
    	SVIEngineThreadProtection.validateMainThread();
        mDuration = duration;
        nativeSetAnimationPropertySVIAnimation(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }
    
    /**
     * set animation repeat count.
     * if repeat count bigger than 1, repeat param's value
     * 
     * @param repeatCount > animation repeat count
     */
    public void setRepeatCount(int repeatCount) {
        if (repeatCount < 0) {// Will cover all the cases
            mRepeatCount = unlimitedRepeat;
        }
        else {
            mRepeatCount = repeatCount + defaultRepeatCount;
        }
        nativeSetAnimationPropertySVIAnimation(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * set animation reverse.
     * animation back to origin property
     * 
     * @param autoReverse > true = on false = off
     */
    public void setAutoReverse(boolean autoReverse) {
        mAutoReverse = autoReverse;
        nativeSetAnimationPropertySVIAnimation(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * set animation offset
     * offset time is wait change time but included duration
     * 
     * @param offset > animation offset time
     */
    public void setOffset(int offset) {
        mOffset = offset;
        nativeSetAnimationPropertySVIAnimation(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * set animation property
     * this method can change all property (duration, repeatCount, autoReverse, offset)
     * 
     * @param duration > animation duration time
     * @param repeatCount > animation repeat count
     * @param autoReverse > animation auto reverse true = on false = off
     * @param offset > animation offset time
     */
    public void setAnimationProperty(int duration, int repeatCount, boolean autoReverse, int offset) {
        mDuration = duration;
        if (repeatCount == -1) {
            mRepeatCount = unlimitedRepeat;
        }
        else {
            mRepeatCount = repeatCount + defaultRepeatCount;
        }
        mAutoReverse = autoReverse;
        mOffset = offset;
        nativeSetAnimationPropertySVIAnimation(mNativeAnimation, mDuration, mRepeatCount, mAutoReverse, mOffset);
    }

    /** 
     * set animation listener
     * when animation running or ending, listener always check animation's state.
     * 
     * @see SVIAnimationListener
     * 
     * @param listener > SVIAnimationListener 
     */
    public void setListener(SVIAnimationListener listener) {		
        mListener = listener;
    }

    /** 
     * get native animation handle
     * 
     * @return int mNativeAnimation
     */
    public int getNativeAnimationHandle() {
        return mNativeAnimation;
    }
    
    /** 
     * get  animation Listener
     * 
     * @return SVIAnimationListener mListener
     */
    public SVIAnimationListener getListener() {
        return mListener;
    }

    /** 
     * set animation Light Type
     * 
     * @see LightType
     * @param lightType > animation light type
     */
    public void setLightType(int lightType) {
        mLightType = lightType;
    }
    
    /** 
     * get animation Light Type
     * 
     * @see LightType
     * @return int mLightType
     */
    public int getLightType() {
        return mLightType;
    }

    /** 
     * get scale type
     * 
     * @return int mScaleType
     */
    public int getScaleType() {
        return mScaleType;
    }
    
    /** set animation tag
     * 
     * @param tag > animation tag
     */
    public void setTag(String tag) {
        mTag = tag;
    }
    
    /** 
     * register listener
     */
    public void registerListener() {
    	if( mListener != null ) {
    		int id = mListenerCounter++;
    		mListenerMap.put(id, mListener);
    		if( mTag != null )
    			mTagMap.put(id , mTag);
    		nativeSetListenerID(mNativeAnimation, id);
    		nativeSetListenerSVIAnimation(mNativeAnimation, getExplicitAnimationProxy());
    	}
    }
    
    /** 
     * get explicit animation proxy
     * 
     * @return int mExplicitProxy
     */
    public static int getExplicitAnimationProxy() {
    	if (mExplicitProxy == 0)
        	mExplicitProxy = nativeCreteProxySVIAnimation(0);
    	
    	return mExplicitProxy;
    }
    
    /** 
     * get implicit animation proxy
     * 
     * @return int mImplicitProxy
     */
    public static int getImplicitAnimationProxy() {
    	if (mImplicitProxy == 0)
    		mImplicitProxy = nativeCreteProxySVIAnimation(1);
    	
    	return mImplicitProxy;
    }
    
    private static final int MSG_EXPLICIT_ANIMATION_END = 0x01;
    private static final int MSG_EXPLICIT_ANIMATION_REPEAT = 0x02;
    private static final int MSG_EXPLICIT_ANIMATION_START = 0x03;
    private static Handler mExplicitHandler = new Handler(){
    	/** 
    	 * If has animation listener, this handler checked animation state { END, REPEAT, START }
    	 * 
    	 * @see SVIAnimationListener
    	 */
        public void handleMessage(Message msg) {
        	int listenerID = msg.arg1;
            SVIAnimationListener listener = (SVIAnimationListener)mListenerMap.get(listenerID);
            String tagString = mTagMap.get(listenerID);

            switch(msg.what) {
                case MSG_EXPLICIT_ANIMATION_END:
                    if (listener != null) {
                        listener.onAnimationEnd(tagString);
                    }
                    mListenerMap.remove(listenerID);
                    mTagMap.remove(listenerID);
                    break;
                case MSG_EXPLICIT_ANIMATION_REPEAT:
                    if (listener != null)
                        listener.onAnimationRepeat(tagString);
                    break;
                case MSG_EXPLICIT_ANIMATION_START:
                    if (listener != null)
                        listener.onAnimationStart(tagString);
                    break;
            }
        }
    };

    private static Handler getExplicitHandler() {
        return mExplicitHandler;
    }

    /** 
     * on animation end
     * 
     * @param listenerID > animation listener id
     */
    static public void onSVIAnimationEnd(int listenerID) {
        Message msg = getExplicitHandler().obtainMessage();
        msg.what = SVIAnimation.MSG_EXPLICIT_ANIMATION_END;
        msg.arg1 = listenerID;
        getExplicitHandler().sendMessage(msg);
    }

    /** 
     * on animation repeat
     * 
     * @param listenerID > animation listener id
     */
    static public void onSVIAnimationRepeat(int listenerID) {
        Message msg = getExplicitHandler().obtainMessage();
        msg.what = SVIAnimation.MSG_EXPLICIT_ANIMATION_REPEAT;
        msg.arg1 = listenerID;
        getExplicitHandler().sendMessage(msg);
    }
    
    /** 
     * on animation start
     * 
     * @param listenerID > animation listener id
     */
    static public void onSVIAnimationStart(int listenerID) {
        Message msg = getExplicitHandler().obtainMessage();
        msg.what = SVIAnimation.MSG_EXPLICIT_ANIMATION_START;
        msg.arg1 = listenerID;
        getExplicitHandler().sendMessage(msg);
    }


    /** SVIAnimationListener Interface*/
    public interface SVIAnimationListener {
        void onAnimationEnd(String tag);
        void onAnimationRepeat(String tag);
        void onAnimationStart(String tag);
    }

    /** Init Animation On Main Thread*/
    public static void initAnimationOnMainThread() {
        getExplicitHandler();
    }
    
    /** delete native animation handle*/
    public void deleteNativeAnimationHandle() {
    	if( mNativeAnimation != 0 ) {
    		nativeDeleteSVIAnimation(mNativeAnimation, mClassType);
    		mNativeAnimation = 0;
    	}
    }

    /** SVIAnimation native interface. */
    static {
        System.loadLibrary(SVIEngineDesc.mName);
    }

    private static native void nativeDeleteSVIAnimation(int aniHandle, int classType);
    private static native void nativeSetInterpolatorSVIAnimation(int aniHandle, int interpolatorType);
    private static native void nativeSetAnimationPropertySVIAnimation(int aniHandle, int duration, int repeatCount, boolean autoReverse, int offset);
    protected static native void nativeSetListenerSVIAnimation(int aniHandle, int proxyHandle);
    private static native int nativeCreteProxySVIAnimation(int proxyType);
    private static native void nativeDeleteProxySVIAnimation(int proxyHandle);
    private static native void nativeSetListenerID(int aniHandle, int listenerID);
}
