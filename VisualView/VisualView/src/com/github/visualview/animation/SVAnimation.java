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
package com.github.visualview.animation;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIPropertyAnimation;

// TODO: Auto-generated Javadoc
/**
 * This class is the base animation class for all animation classes in the Visual View package.
 * 
 */
public abstract class SVAnimation {
	
	

    protected SVIAnimation mAnimation;
    

	/**
	 * Region of slide.<p>
	 * The Region is defined as a rectangle(left, top, width, height) with float array[4]. 
	 * It is expressed in terms of absolute coordinates for the Slide.
	 */
	public static final int REGION = SVIPropertyAnimation.PropertyAnimationType.REGION;
	
	/**
	 * Position of slide.<p>
	 * The Position is defined as a point(x,y) with float array[2]
	 * It is expressed in terms of absolute coordinates for the Slide.
	 */
	public static final int POSITION = SVIPropertyAnimation.PropertyAnimationType.POSITION;
	
	/**
	 * Pivot Point of slide.<p>
	 * The Pivot Point is defined as a point(x,y) with float array[2]
	 * It is expressed in terms of relative coordinates for the Slide.
	 */
	public static final int PIVOT_POINT = SVIPropertyAnimation.PropertyAnimationType.PIVOT_POINT;
	
	/**
	 * Z Position of slide.<p>
	 * The Z position defines the Depth of the slide.
	 * It is expressed in terms of absolute coordinates for the Slide.
	 */
	public static final int Z_POSITION = SVIPropertyAnimation.PropertyAnimationType.ZPOSITION;
	
	/**
	 * Rotation of slide.<p>
	 * Rotation is defined as (x, y, z) with float array[3]
	 */
	public static final int ROTATION = SVIPropertyAnimation.PropertyAnimationType.ROTATION;
	
	/**
	 * Scale of slide.<p>
	 * Scale is defined for all the three axes (xScale, yScale, zScale) with float array[3]
	 */
	public static final int SCALE = SVIPropertyAnimation.PropertyAnimationType.SCALE;
	
	/**
	 * Background color of slide.<p>
	 * Background Color is defined as (R, G, B, A) with float array[4] 
	 */
	public static final int BACKGROUND_COLOR = SVIPropertyAnimation.PropertyAnimationType.BG_COLOR;
	
	/**
	 * Opacity of slide.<p>
	 * The Opacity of a slide is a float value between 0.0 and 1.0.
	 */
	public static final int OPACITY = SVIPropertyAnimation.PropertyAnimationType.OPACITY;
	
	/**
	 * Corner Radius of slide.<p>
	 * The Corner Radius is a float value.
	 */
	public static final int CORNER_RADIUS = SVIPropertyAnimation.PropertyAnimationType.CORNER_RADIUS;
	
	/**
	 * Scale to Fit Region of slide.<p>
	 * The Region is defined as a rectangle(left, top, width, height) with a float array[4].
	 */
	public static final int SCALE_TO_FIT_REGION = SVIPropertyAnimation.PropertyAnimationType.SCALETOFIT_REGION;

	/**
	 * Texture Region of slide.<p>
	 * The Region is defined as a rectangle(left, top, width, height) with a float array[4].
	 * It is expressed in terms of relative coordinates for the texture (Image).
	 */
	public static final int TEXTURE_REGION = SVIPropertyAnimation.PropertyAnimationType.TEXTURE_REGION;
	
	/**
	 * Color of the light hitting the slide.<p>
	 * The Light Color is defined as (R, G, B, A) with a float array[4] 
	 */
	public static final int LIGHT_COLOR = SVIPropertyAnimation.PropertyAnimationType.LIGHT_COLOR;
	
	/**
	 * Angle at which the light hits the slide.<p>
	 * The Light angle with respect to the slide is as a float value.
	 */
	public static final int LIGHT_ANGLE = SVIPropertyAnimation.PropertyAnimationType.LIGHT_ANGLE;
	
	/**
	 * Intensity of the light source.<p>
	 * The light power is a float value.
	 */
	public static final int LIGHT_POWER = SVIPropertyAnimation.PropertyAnimationType.LIGHT_POWER;
	
	/**
	 * Opacity of the light hitting the slide.<p>
	 * The light opacity is a float value.
	 */
	public static final int LIGHT_OPACITY = SVIPropertyAnimation.PropertyAnimationType.LIGHT_OPACITY;
	
	/**
	 * Light offset of Slide.<p>
	 * The light source offset with respect to the slide is a float value. 
	 */
	public static final int LIGHT_OFFSET = SVIPropertyAnimation.PropertyAnimationType.LIGHT_OFFSET;
	
	
	/**
	 * Light Radius of slide.<p>
	 * The Light type must be a Spot type.<p>
	 * The Light source radius is a float value.
	 */
	public static final int LIGHT_RADIUS = SVIPropertyAnimation.PropertyAnimationType.LIGHT_RADIUS;
		
	/**
	 * Shadow Opacity of slide.<p>
	 * The Shadow opacity is a float value.
	 */
	public static final int SHADOW_OPACITY = SVIPropertyAnimation.PropertyAnimationType.SHADOW_OPACITY;
	/**
	 * Shadow radius of slide.<p>
	 * The Shadow radius is a float value.
	 */
	public static final int SHADOW_RADIUS = SVIPropertyAnimation.PropertyAnimationType.SHADOW_RADIUS;
	/**
	 * Shadow offset of slide.<p>
	 * The Shadow offset is a float value.
	 */
	public static final int SHADOW_OFFSET = SVIPropertyAnimation.PropertyAnimationType.SHADOW_OFFSET;
	
	/**
	 * Shadow color of slide.<p>
	 * The Shadow Color is defined as (R, G, B, A) with float array[4] 
	 */
	public static final int SHADOW_COLOR = SVIPropertyAnimation.PropertyAnimationType.SHADOW_COLOR;
	
	/**
	 * Border color of slide.<p>
	 * The Border Color is defined as (R, G, B, A) with float array[4] 
	 */
	public static final int BORDER_COLOR = SVIPropertyAnimation.PropertyAnimationType.BORDER_COLOR;
	
	/**
	 * Border Width of slide.<p>
	 * The border width is a float value. 
	 */
	public static final int BORDER_WIDTH = SVIPropertyAnimation.PropertyAnimationType.BORDER_WIDTH;

	
	
	
    /** 
     * Creates an instance of SVAnimation
     */
    public SVAnimation() {
		mAnimation = new SVIAnimation();
    }
    
    
    /**
     * Set the specified duration for the animation in milliseconds
     * <p>The duration time is animation's running time in milliseconds</p>
     * 
     * @param duration Amount of time in milliseconds that the animation runs.
     * @throws IllegalArgumentException if the duration of the animation is a negative value.
     */
    public void setDuration(int duration) {
    	checkLowValue(duration);
    	if(mAnimation != null)
    	{
    		mAnimation.setDuration(duration);
    	}
    }
    
    /** 
     * Get animation duration in milliseconds.
     * 
     * @return int Amount of time in milliseconds that the animation runs.
     */
     public int getDuration() {
         
    	 if(mAnimation != null)
    	 {
    		 return mAnimation.getDuration();
    	 }
    	 
    	 return 0;
     }
    
    /**
     * Set the specified interpolator for the animation.
     * 
     *
     * @param interpolator Interpolator used by the animation, see the list of interpolators in the SV class for more information. This Value should lie between (0 ~ 41)
     * @throws IllegalArgumentException The interpolator type must lie between 0 ~41 , any value outside the range throws {@link IllegalArgumentException}
     * @see SV
     */
    public void setInterpolator(int interpolator) {
		
    	if(interpolator <0 || interpolator >41) {
        	throw new IllegalArgumentException("Set the correct Interpolator type ..The value must be 0 ~ 41"); 
    	}
    	if(mAnimation != null)
    	{
    		mAnimation.setInterpolator(interpolator);
    	}
    }
    
   /** 
    * Get the interpolator used in the animation.
    * 
    * @return int Interpolator used by the animation, see the list of interpolators in the SV class for more information
    * @see SV
    */
    public int getInterpolator() {
        
    	if(mAnimation != null)
    	{
    		return mAnimation.getInterpolator();
    	}
    	
    	return SVInterpolator.INTERPOLATOR_LINEAR;
    }
    
    /**
     * Set the number of times the animation is repeated.
     * <p> You can automatically restart animations after each cycle ends. </p>  
     * 
     * <p>Default value is 0</p>
     * 
     * @param repeatCount Number of times the animation repeats.
     * @throws IllegalArgumentException if repeat count is a negative value.
     */
    public void setRepeatCount(int repeatCount) {
    	if(repeatCount <0 ) {
        	throw new IllegalArgumentException("Repeat count should be >= 0"); 
    	}
    	if(mAnimation != null)
    	{
    		mAnimation.setRepeatCount(repeatCount);
    	}
    }
    
    /** 
     * Get the number of times the animation is repeated.
     * 
     * @return int Number of times the animation repeats.
     */
     public int getRepeatCount() {
         
    	 if(mAnimation != null)
    	 {
    		 int repeatCount = mAnimation.getRepeatCount();
    		 if(repeatCount <= 0){
    			 return 0;
    		 }else{
    			 return repeatCount;
    		 }
    	 }
    	 
    	 return 0;
     }
     
     /**
      * Set the specified offset for the animation in milliseconds.
      * <p>The offset time is the time that elapses before the animation starts.</p>.
      * <p> The default value for animation offset is 0. </p>
      *
      * @param offset Amount of time in milliseconds that elapses before the animation starts.
      * @throws IllegalArgumentException if offset value is negative.
      */
     public void setOffset(int offset) {
	     checkLowValue(offset);
    	 if(mAnimation != null)
    	 {
    		 mAnimation.setOffset(offset);
    	 }
     }
     
     /** 
      * Get the offset for the animation in milliseconds.
      * <p>The offset time is the time that elapses before the animation starts.
      * 
      * @return int Amount of time in milliseconds that elapses before the animation starts.
      */
      public int getOffset() {
          
    	  if(mAnimation != null)
    	  {
    		  return mAnimation.getOffset();
    	  }
    	  
    	  return 0;
      }
      
      /**
       * Enable or disable the auto-reverse property for the animation.
       * <p>If auto-reverse is enabled, the animation goes back to its initial property state at the end of the animation.</p> 
       * 
       * @param enabled Set to true to enable auto-reverse.Set to false to disable auto-reverse. The default value is false.
       */
      public void setAutoReverseEnabled(boolean enabled) {
    	  if(mAnimation != null) 
    	  {
    		  mAnimation.setAutoReverse(enabled);
    	  }
      }
      
      /**
       * Check if auto-reverse is enabled for the animation.
       * 
       * @return true if auto-reverse is enabled, false otherwise.
       */
      public boolean isAutoReverseEnabled() {
    	  
    	  if(mAnimation != null)
    	  {
    		  return mAnimation.isAutoReverse();
    	  }
    	  
    	  return false;
      }
      
      
      /**
       * Get the animation at the engine side.
       * @return The handle for the animation.
       */
      public SVIAnimation getAnimationFromEngine(){
    	  return mAnimation;
      }
      
      
      /**
       * get Animation's handle value. This value is pointer of animation.
       * @return animation handle value
       */
      public int getAnimationHandle(){
    	  if(mAnimation != null)
    		  return mAnimation.getNativeAnimationHandle();
    	  
    	  return -1;
      }
      
      private void checkLowValue(float value) throws IllegalArgumentException{
  		if(value < 0.0f){
  			throw new IllegalArgumentException("Value must be bigger than 0.0f.");
  		}
  	}
      

  	
  	/**
  	 * Set the specified AnimationListener instance.
  	 * @param listener AnimationListener instance for receiving animation start, stop, and repeat events.
  	 * @throws IllegalArgumentException if listener object is null.
  	 * @see AnimationListener
  	 */
  	public void setAnimationListener(AnimationListener listener){
  	  
  		if(listener == null){
  			throw new IllegalArgumentException("Listener must not be null");
  		}
		  if(mAnimation != null)
		  {
			final int aniHandle = getAnimationHandle();
			final AnimationListener aniListener = listener;
			
			mAnimation.setListener(new SVIAnimationListener() {
				
				@Override
				public void onAnimationStart(String arg0) {
					aniListener.onAnimationStarted(aniHandle);
				}
				@Override
				public void onAnimationRepeat(String arg0) {
					aniListener.onAnimationRepeated(aniHandle);
				}
				@Override
				public void onAnimationEnd(String arg0) {
					aniListener.onAnimationEnded(aniHandle);
				}
			});
		  }
  	}
  	
  	
  	

      /**
       * This listener interface receives animation events.
       * This listener interface provides callback methods for animation start, stop, and repeat events.
       * 
       */
      public interface AnimationListener {
    	  /**
           * Called when the animation starts.
           *         
           * @param aniHanle  The handle for the animation for the slide.
           */
          void onAnimationStarted(int aniHanle);
    	  
          /**
           * Called when the animation stops.
           * 
           * @param aniHanle  The handle for the animation for the slide.
           */
          void onAnimationEnded(int aniHanle);
          
          /**
           * Called when the animation repeated.
           * 
           * @param aniHanle  The handle for the animation for the slide.
           */
          void onAnimationRepeated(int aniHanle);
      }
      
      
      
      
}
