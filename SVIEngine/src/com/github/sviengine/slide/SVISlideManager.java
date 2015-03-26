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

import java.util.HashMap;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.InterpolatorType;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlideManager;

import android.os.Handler;
import android.os.Message;

/**
 * slide manager class(singleton)
 */
public class SVISlideManager {


	/** slide priority */
	public interface SlidePriority {
		public static final int SUI_PRIORITY_NORMAL = 1;
		public static final int SUI_PRIORITY_TICKER = 2;
		public static final int SUI_PRIORITY_POPUP = 3;
		public static final int SUI_PRIORITY_CONTEXTMENU = 4;
		public static final int SUI_PRIORITY_OVERLAYBAR = 5;
		public static final int SUI_PRIORITY_TOOLTIP = 6;
	}
	
	private static final int MSG_REQUEST_RENDER = 0x01;
	private boolean mUpdating = false;
	private boolean mMarkInvalidate = false;
	private int mAnimationSetting = ALL_ANIMATION;
	
	private Handler mRequestRenderHandler = null;
	
	private SVISlideRequestRender			mSVISlideRequestRender = null;	
	private boolean 							mPauseImplicitAnimation = false;
	private static HashMap<Integer, SVISlideImplicitAnimationListener> mImplicitListenerMap;
	private boolean					mTransaction = false;
	private int						mInterpolatorType = InterpolatorType.LINEAR;
	private int						mDuration = 0;
	private int						mRepeatCount = 1;
	private int						mOffset = 0;
	private boolean					mAutoReverse = false;
	private boolean					mAnimationDisable = false;
	private boolean					mRequestRendering = false;
	
	public static final int IMPLICIT_ANIMATION_TYPE_ADD = 3;
	public static final int IMPLICT_ANIMATION_TYPE_MOVETO_TOP = 4;
	public static final int IMPLICT_ANIMATION_TYPE_SET_REGION = 5;
	public static final int IMPLICT_ANIMATION_TYPE_ACTIVITY_CHANGE = 6;
	public static final int IMPLICT_ANIMATION_TYPE_ROTATION = 7;
	public static final int IMPLICT_ANIMATION_TYPE_OPACITY = 8;
	public static final int IMPLICT_ANIMATION_TYPE_RITH_TO_LEFT = 9;

	public static final int NO_ANIMATION = 0;
	public static final int SOME_ANIMATION = 1;
	public static final int ALL_ANIMATION = 2;

	final int defaultRepeatCount = 1;
	private int 		mListenerCounter = 0;

	private int mNatSurfaceHandle = 0;

	private boolean isFinalized;
	
	/** 
	 * SVISlideManager default constructor 
	 */
	public SVISlideManager(SVIGLSurface surface) {
		mNatSurfaceHandle = surface.getNativeHandle();

		if(mImplicitListenerMap == null) {
			mImplicitListenerMap = new HashMap<Integer, SVISlideImplicitAnimationListener>();
		}
		
		if( mRequestRenderHandler == null ) {
			mRequestRenderHandler = new Handler() {
				public void handleMessage(Message msg) {
					if(isFinalized){
						return;
					}
					
					switch(msg.what) { 
						case MSG_REQUEST_RENDER: {
							if(mMarkInvalidate){
								mUpdating = false;
								return;
							}
							
							if( mSVISlideRequestRender != null ) {
								mSVISlideRequestRender.requestSlideRender();
								mUpdating = false;
							} else {
								throw new RuntimeException("SVISlideRequestRender instance is not defined!!!");
							}
						}
						break;
						default:
						break;
					}
				}
			};
		}
	}
	
	protected void finalize() throws Throwable {
		isFinalized = true;
		super.finalize();
	}
	
	public static SVISlideManager getInstance() {
		SVIEngineThreadProtection.validateMainThread();
		return SVIGLSurface.getSurface(null).getSlideManager();
	}

	/** 
	 * implicit animation check that the stop.
	 * @return boolean mPauseImplicitAnimation
	 */
	public boolean isPausedImplicitAnimation() {
		return mPauseImplicitAnimation;
	}

	/** 
	 * method to change the value of mPauseImplicitAnimation to pause.
	 * @param pause > set the value of mPauseImplicitAnimation
	 */
	public void setPauseImplicitAnimation(boolean pause) {
		mPauseImplicitAnimation = pause;
	}

	/** 
	 * implicit animation applied to slide
	 * @param slide > target
	 * @param type > animation type
	 * @param duration > animation time
	 */
    public void requestImplicitAnimation(SVISlide slide, int type, int duration) {
        if (getCheckoutAnimation() == true) {
            // Explicit transaction duration priority
            // 1. explicit animation disable : mDuration = 0
            // 2. explicit transaction duration : mDuration
            // 3. user setting duration : duration ( mDuration isn't exist : mDuration = 0)
            if (getAnimationDisable() == true) {
                mDuration = 0;
            }
            else if (mDuration == 0) {
                slide.setImplicitAnimation(type, mInterpolatorType, duration, mRepeatCount, mOffset, mAutoReverse == true ? 1 : 0);
            }
            else { 
                slide.setImplicitAnimation(type, mInterpolatorType, mDuration, mRepeatCount, mOffset, mAutoReverse == true ? 1 : 0);
            }
        }
        else {
            slide.setImplicitAnimation(type, InterpolatorType.LINEAR, duration, 1, 0, 0);
        }
        
        if (isPausedImplicitAnimation() == false)
            requestRender();
    }

    /** 
     * explicit animation applied to slide
     * @param slide > target
     * @param animation > animation to apply
     */
	public void requestExplicitAnimation(SVISlide slide, SVIAnimation animation) {
		slide.setExplicitAnimation(animation);

		if (isPausedImplicitAnimation() == false)
			requestRender();
	}

	/** 
	 * mTransaction is set to true. 
	 */
	public void checkoutAnimation() {
		mTransaction = true;
	}

	/** the default value is set implicit animation. */
	public void checkinAnimation() {
		mTransaction = false;
		mInterpolatorType = InterpolatorType.LINEAR;
		mDuration = 0;
		mRepeatCount = 1;
		mOffset = 0;
		mAutoReverse = false;
		mAnimationDisable = false;
	}

	/** 
	 * check if check out.
	 * @return boolean mTransaction
	 */
	public boolean getCheckoutAnimation() {
		return mTransaction;
	}

	/** 
	 * get interpolator type
	 * @return int mInterpolatorType
	 */
	public int getInterpolatorType() {
		return mInterpolatorType;
	}

	/** 
	 * get duration
	 * @return int mDuration
	 */
	public int getDuration() {
		return mDuration;
	}

	/** 
	 * get repeat count
	 * @return int mRepeatCount
	 */
	public int getRepeatCount() {
		return mRepeatCount;
	}

	/** 
	 * get offset
	 * @return int mOffset
	 */
	public int getOffset() {
		return mOffset;
	}

	/** 
	 * get animation disable
	 * @return boolean mAnimationDisable
	 */
	public boolean getAnimationDisable() {
		return mAnimationDisable;
	}
	
	/** 
	 * get auto reverse
	 * @return boolean mAutoReverse
	 */
	public boolean getAutoReverse() {
		return mAutoReverse;
	}
	
	/** 
	 * set interpolator type
	 * @param type > set the value of mInterpolatorType
	 */
	public void setInterpolatorType(int type) {
		mInterpolatorType = type;
	}

	/** set duration
	 * @param duration > set the value of mDuration
	 */
	public void setDuration(int duration) {
		mDuration = duration;
	}

	/**
	 * set repeat count
	 * @param count > set the value of mRepeatCount
	 */
	public void setRepeatCount(int count) {
		mRepeatCount = count + defaultRepeatCount;
	}

	/**
	 * set offset
	 * @param offset > set the value of mOffset
	 */
	public void setOffset(int offset) {
		mOffset = offset;
	}

	/**
	 * set auto reverse
	 * @param autoReverse > set the value of mAutoReverse
	 */
	public void setAutoReverse(boolean autoReverse) {
		mAutoReverse = autoReverse;
	}

	/**
	 * set animation disable
	 * @param disable > set the value of mAnimationDisable
	 */
	public void setAnimationDisable(boolean disable) {
		mAnimationDisable = disable;
	}

	/**
	 * request render
	 */
	public void requestRender() {
		if(mUpdating == false) {
			mUpdating = true;
			Message msg = mRequestRenderHandler.obtainMessage();
			msg.what = MSG_REQUEST_RENDER;
			mRequestRenderHandler.sendMessage(msg);
		}
	}

	/**
	 * set mark invalidate
	 * @param markInvalidate > set the value of mMarkInvalidate
	 */
	public void setMarkInvalidate(boolean markInvalidate) {
		// TODO Auto-generated method stub
		mMarkInvalidate = markInvalidate;	
	}	
	
	/**
	 * set updating
	 * @param updating > set the value of mUpdating
	 */
	public void setUpdating(boolean updating) {
		mUpdating = updating;
	}

	/**
	 * set request rendering
	 * @param requestRendering > set the value of mRequestRendering
	 */
	public void setRequestRendering(boolean requestRendering) {
		mRequestRendering = requestRendering;
	}

	/**
	 * get request rendering value
	 * @return boolean mRequestRendering
	 */
	public boolean getRequestRendering() {
		return mRequestRendering;
	}
	
	/**
	 * set animation setting value
	 * @param type > set the value of mAnimationSetting
	 */
	public void setAnimationSettingValue(int type) {
		mAnimationSetting = type;
	}

	/**
	 * get animation setting value
	 * @return int mAnimationSetting
	 */
	public int getAnimationSettingValue() {
		return mAnimationSetting;
	}
	
	
	/** SVISlideRequestRender Interface definition & set function */
	public interface SVISlideRequestRender {
		public boolean requestSlideRender();
		public void animationSlideNotify();
	}

	/**
	 * set slide request render
	 * @param requestRender > set the value of mSVISlideRequestRender
	 */
	public void setSVISlideRequestRender(SVISlideRequestRender requestRender) {
		mSVISlideRequestRender = requestRender;
	}

	/**
	 * notify
	 */
	public void animaitonRenderNotify(){
		if (mSVISlideRequestRender != null)
			mSVISlideRequestRender.animationSlideNotify();
	}

	private static final int MSG_IMPLICIT_ANIMATION_END = 0x01;
	private static final int MSG_IMPLICIT_ANIMATION_REPEAT = 0x02;
	private static final int MSG_IMPLICIT_ANIMATION_START = 0x03;
	private static Handler mImplicitHandler = new Handler(){
		public void handleMessage(Message msg) {
			int listenerID = msg.arg1;
			SVISlideImplicitAnimationListener listener = (SVISlideImplicitAnimationListener)mImplicitListenerMap.get(listenerID);

			switch(msg.what){
			case MSG_IMPLICIT_ANIMATION_END:
				if (listener != null) {
					listener.onImplicitAnimationEnd(null);
				}
				mImplicitListenerMap.remove(listenerID);
				break;

			case MSG_IMPLICIT_ANIMATION_REPEAT:
				if (listener != null) {
					listener.onImplicitAnimationRepeat(null);
				}
				break;
				
			case MSG_IMPLICIT_ANIMATION_START:
				if (listener != null) {
					listener.onImplicitAnimationStart(null);
				}
				break;	
			}
		}
	};

	private static Handler getImplicitHandler() {
		return mImplicitHandler;
	}

	/** SVISlideImplicitAnimation Interface definition & set function */
	public interface SVISlideImplicitAnimationListener {
		public void onImplicitAnimationEnd(String tag);
		public void onImplicitAnimationRepeat(String tag);
		public void onImplicitAnimationStart(String tag);
	}

	/**
	 * when implicit animation is finished handling.
	 * @param listenerID > set the value of Message.arg1
	 */
	public static void onSVISlideImplicitAnimationEnd(int listenerID) {
		Message msg = getImplicitHandler().obtainMessage();
		msg.what = SVISlideManager.MSG_IMPLICIT_ANIMATION_END;
		msg.arg1 = listenerID;
		getImplicitHandler().sendMessage(msg);
	}

	/**
	 * when implicit animation is repeated handling.
	 * @param listenerID > set the value of Message.arg1
	 */
	public static void onSVISlideImplicitAnimationRepeat(int listenerID) {
		Message msg = getImplicitHandler().obtainMessage();
		msg.what = SVISlideManager.MSG_IMPLICIT_ANIMATION_REPEAT;
		msg.arg1 = listenerID;
		getImplicitHandler().sendMessage(msg);
	}

	/**
	 * when implicit animation is starts handling.
	 * @param listenerID > set the value of Message.arg1
	 */
	public static void onSVISlideImplicitAnimationStart(int listenerID) {
		Message msg = getImplicitHandler().obtainMessage();
		msg.what = SVISlideManager.MSG_IMPLICIT_ANIMATION_START;
		msg.arg1 = listenerID;
		getImplicitHandler().sendMessage(msg);
	}
	
	/**
	 * listener and listener's id registered on the listener map is applied to the slide.
	 * @param slide > target
	 * @param listener > slide of the implicit animation listener
	 */
	public void setImplicitListener(SVISlide slide, SVISlideImplicitAnimationListener listener) {
		int proxy = 0;
		proxy = SVIAnimation.getImplicitAnimationProxy();

		if (proxy != 0) {
			int listenerID = ++mListenerCounter;
			mImplicitListenerMap.put(listenerID , listener);
			slide.setProxy(proxy, listenerID);
		}
	}
	/**
	 * Clear all listener which are registered on the listener map and applied to the slide
	 * which should called by app when it is paused or destroyed.
	 */
	public void clearAllImplicitListener(){
		mImplicitListenerMap.clear();
	}
	/**
	 * is animating
	 * @return boolean ( nativeIsAnimatingSVISlideManager() == 0 )
	 */
	public boolean isAnimating(){
		if (nativeIsAnimatingSVISlideManager(mNatSurfaceHandle) == 0)
			return false;
		else
			return true;
	}
	
	/**
	 * enable global animation
	 * @param enableAnimation > set the value native enable global animation
	 */
	public void enableGlobalAnimation(boolean enableAnimation) {
		nativeEnableGlobalAnimation(mNatSurfaceHandle, enableAnimation ? 1 : 0 );
	}
	
	/**
	 * is enable global animation
	 * @return boolean ( nativeIsEnableGlobalAnimation() == 1)
	 */
	public boolean isEnableGlobalAnimation() {
		return nativeIsEnableGlobalAnimation(mNatSurfaceHandle) == 1 ? true : false;
	}
	
	/**
	 * get rendering frames per second
	 * @return float native rendering frames per second
	 */
	//2012-01-02 masterkeaton27@gmail.com
	public float getFPS(){
		return nativeGetFPS(mNatSurfaceHandle);
	}
	
	/**
	 * Get current page status information from PageTurnEffect animation equation.
	 * This function will be used for drag animation functionality of PTE library especially.
	 * This function is useless for other use cases. 
	 * 
	 * @param tartget SVISlide instance which want to know page status.
	 * @return page status of PageTurnEffect animation equation.
	 */
	public int checkPageStatusForEBook(SVISlide slide) {
		return nativeCheckPageStatus(slide.getNativeSlideHandle());
	}
	
	
	/*****************************************************************************************************************/
	/**
	  * SVIAnimationSet native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}

	private native int nativeIsAnimatingSVISlideManager(int natSurfaceHandle);
	private native void nativeEnableGlobalAnimation(int natSurfaceHandle, int enableAnimation);
	private native int nativeIsEnableGlobalAnimation(int natSurfaceHandle);
	private native float nativeGetFPS(int natSurfaceHandle);
	private native int nativeCheckPageStatus(int nativeSlideHandler);
}