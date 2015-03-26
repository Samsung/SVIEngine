/*
 * Copyright (C) 2013 SVV.
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
package com.github.visualview;

import com.github.sviengine.slide.SVISlideManager;
import com.github.sviengine.slide.SVISlideManager.SVISlideImplicitAnimationListener;

/**
 * This class controls the implicit animation.
 * 
 */
public class SVSlideManager {

	private static boolean mCheckOut = false;
	
	
	/**
	 * Constructor of SVSlideManager
	 */
	private SVSlideManager(){
	}
	/**
	 * Pause the implicit animation.
	 * <p>
	 * when you set this function, Visual View holds the next implicit animation.
	 * then, when you set releaseImplicitAnimation(), all hold animations are will start playing.
	 */
	public static void holdImplicitAnimation(){
		SVISlideManager.getInstance().setPauseImplicitAnimation(true);
	}
	

	/**
	 * Resume the implicit animation.
	 * <p>
	 * when you  set holdImplicitAnimation(), then all next implicit animation will be on hold.
	 * then, when you set this function, all animations on hold will start to  play.
	 */
	public static void releaseImplicitAnimation() {
		SVISlideManager.getInstance().setPauseImplicitAnimation(false);
		setRequestRender();
	}
	
	
	
	
	/**
	 * CheckOut Animation To modify implicit animation parameters of a slide. It has to be checked out before modifying the parameters.
	 * After modification, the same has to be checked in.
	 */
	public static void checkOutAnimation(){
		
    	try {
			isCheckOutValid(true);
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
		mCheckOut = true;
		SVISlideManager.getInstance().checkoutAnimation();
	}
	
	/**
	 *
	 * Check in animation after modifying implicit animation parameters for a slide. It has to be checked out before modifying the parameters.
	 * Before modification, This function need to be used.
	 * 
	 */
	public static void checkInAnimation(){

    	try {
			isCheckOutValid(false);
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
		mCheckOut = false;
		SVISlideManager.getInstance().checkinAnimation();
	}
	
	
	
	/**
	 * Set the specified duration for the implicit animation in milliseconds.
	 *
	 * @param duration  Duration of the animation.
	 * @throws IllegalArgumentException if the duration is passed as a negative integer value.
	 *
	 */
	public static void setDuration(int duration) {
		if(duration<0){
		   	throw new IllegalArgumentException("Value must be greater then 0"); 
		}
		SVISlideManager.getInstance().setDuration(duration);
	}
	
	
	/**
     * Set the specified animation repeat count for this animation.
     * <p> Animations can be restarted automatically after completion of each cycle. This function lets the applications to set the number of times the animation to repeat. </p>  
     * 
     * 
     * @param repeatCount repeat count of animation
     * @throws IllegalArgumentException if the Repeat count value is negative.
     */
    public static void setRepeatCount(int repeatCount) {
    	if(repeatCount <0 ) {
        	throw new IllegalArgumentException("Repeat count should be >= 0"); 
    	}
    	SVISlideManager.getInstance().setRepeatCount(repeatCount);
    }
	/**
	 * Set the specified interpolator type
	 *
	 * @param interpolator Interpolator used by the animation, see the list of interpolators in the SV class for more information. it must lie between 0~41.
	 * @throws IllegalArgumentException if the interpolator type is out of range (should be within 0~41).
	 * @see SV
	 */
	public static void setInterpolatorType(int interpolator) {
		if(interpolator <0 || interpolator >41) {
        	throw new IllegalArgumentException("Set the correct Interpolator type ..The value must be 0 ~ 41"); 
    	}
    
		SVISlideManager.getInstance().setInterpolatorType(interpolator);
	}
	
	
	/**
	 * Check if implicit animations are running
	 * 
	 * @return true, if animation is running, false otherwise.
	 */
	public static boolean isAnimating() {
		
		return SVISlideManager.getInstance().isAnimating();
	}
	
	
	private static void setRequestRender() {
		
		SVISlideManager.getInstance().requestRender();
	}
	
	
	private static boolean isCheckOutValid(boolean isCheckout) throws Exception{
    	
		if(isCheckout){
			if(mCheckOut){
				throw new Exception("Implicit Animation :: Multi Checkout Error. check checkOutAnimation");
			}
			
    	}else if(!isCheckout){
    		if(!mCheckOut){
    			throw new Exception("Implicit Animation :: need CheckOutAnimation()");
    		}
    	}else{
    		return true;
    	}
    	return false;
    }
	
	

	
	/**
	 * Set the specified ImplicitAnimationListener instance for receiving implicit animation events from the specified slide.
	 * @param slide Slide sending the events
	 * @param listener ImplicitAnimationListener instance for receiving events.
	 * @throws IllegalArgumentException If the Slide or listener object is passed as null.
	 *
	 */
	public static void setImplicitListener(final SVSlide slide, final ImplicitAnimationListener listener) {
		if(slide == null || listener == null){
			throw new IllegalArgumentException("Implicit Animation :: Slide and Listener must not be null.");
		}
		SVISlideManager.getInstance().setImplicitListener(slide.mSlide, new SVISlideImplicitAnimationListener(){

			@Override
			public void onImplicitAnimationEnd(String arg0) {
				listener.onImplicitAnimationEnded(slide.getSlideHandle());
			}
			@Override
			public void onImplicitAnimationRepeat(String arg0) {
				listener.onImplicitAnimationRepeated(slide.getSlideHandle());
			}
			@Override
			public void onImplicitAnimationStart(String arg0) {
				listener.onImplicitAnimationStarted(slide.getSlideHandle());
			}
		});
	}
	
	/**
	 * This listener interface provides callback methods for implicit animation events
	 * your application can implement this interface to
	 * get the desired result on start/repeat/End of implicit animation.
	 *
	 */
	public interface ImplicitAnimationListener{
		
		/**
		 * Called when implicit animation stops.
		 * @param slideHandle Handle for the slide sending the event.
		 */
		public void onImplicitAnimationEnded(int slideHandle);
		
		/**
		 * Called when implicit animation repeats.
		 * @param slideHandle Handle for the slide sending the event.
		 */
		public void onImplicitAnimationRepeated(int slideHandle);
		
		/**
		 * Called when implicit animation starts.
		 * @param slideHandle Handle for the slide sending the event.
		 *
		 */
		public void onImplicitAnimationStarted(int slideHandle);
	}
	

	
}
