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

import com.github.sviengine.animation.SVIAnimationSet;


/**
 * This class provides the functionality for creating composite animations by allowing different types of animations 
 * to be combined including other SviAnimationSets. Multiple animations created for the same slide must belong to 
 * same animation set
 * 
 */
public class SVAnimationSet extends SVAnimation {

    SVIAnimationSet mAnimationSet; 
	/**
	 * Creates an instance of SVAnimationSet.
	 */
    public SVAnimationSet() {
    	mAnimation = new SVIAnimationSet();
	    mAnimationSet = (SVIAnimationSet)mAnimation;
	    
    }
    
    /** 
     * Add the specified animation to the animation set.
     * 
     * @param animation The animation to add to the set.
     * @return true if animation is added successfully, false otherwise.
     * @throws IllegalArgumentException if the input animation object is passed as null.
     */
    public boolean addAnimation(SVAnimation animation) {
    	
    	if(animation != null){
    		if(animation instanceof SVKeyFrameAnimation){
    			if(!((SVKeyFrameAnimation) animation).isKeyFrameTimeValid()){
					return false;
				}
    			
    			return mAnimationSet.addAnimation(((SVKeyFrameAnimation) animation).mKeyframeAnimation);
    		}
    		else if(animation instanceof SVSpriteAnimation)
    			return mAnimationSet.addAnimation(((SVSpriteAnimation) animation).mSpriteAnimation);
    		else if(animation instanceof SVTransitionAnimation)
    			return mAnimationSet.addAnimation(((SVTransitionAnimation) animation).mTransitionAnimation);
    		else if(animation instanceof SVBasicAnimation)
    			return mAnimationSet.addAnimation(((SVBasicAnimation) animation).mBasicAnimation);
    		else 
    			throw new IllegalArgumentException("Cant add instance of Animationset..it  must add only keyframe/transition/sprite/Basic to itself...");

    	}else{
    		throw new IllegalArgumentException("Animation must not be null.");
    	}
    	
    }

    /** 
     * Remove the specified animation from the animation set.
     * 
     * @param animation The animation to remove from the set.
     * @throws IllegalArgumentException if the input animation object is passed as null.
     */
    public void removeAnimation(SVAnimation animation) {
	    
    	if(animation != null){
    		if(animation instanceof SVKeyFrameAnimation)
    			 mAnimationSet.removeAnimation(((SVKeyFrameAnimation) animation).mKeyframeAnimation);
    		else if(animation instanceof SVSpriteAnimation)
    			 mAnimationSet.removeAnimation(((SVSpriteAnimation) animation).mSpriteAnimation);
    		else if(animation instanceof SVTransitionAnimation)
    			 mAnimationSet.removeAnimation(((SVTransitionAnimation) animation).mTransitionAnimation);
    		else if(animation instanceof SVBasicAnimation)
    			 mAnimationSet.removeAnimation(((SVBasicAnimation) animation).mBasicAnimation);
    		else 
    			 mAnimationSet.removeAnimation(animation.mAnimation);
    	}else{
    		throw new IllegalArgumentException("Animation must not be null.");
    	}
    }
    
    /**
     * Set the specified interpolator for the animation.
     *
     * @param interpolator - Interpolator used by the animation, see the list of interpolators in the SV class for more information. The value should lie between 0~41
     * @throws IllegalArgumentException The interpolator type must lie between 0 ~41 , any value outside the range throws {@link IllegalArgumentException}
     * @see SV
     */
    public void setInterpolator(int interpolator) {
    	if(interpolator <0 || interpolator >41) {
        	throw new IllegalArgumentException("Set the correct Interpolator type ..The value must be 0 ~ 41"); 
    	}
    
    	if(mAnimationSet != null)
    	{	
    		mAnimationSet.setAnimationSetInterpolator(interpolator);
    	}
    }
    
    /**
    /** 
    * Enable or disable animation information sharing between animations in the animation set.
    * You can enable the sharing of animation information such as interpolator type, duration, repeat count, offset, auto reverse can be shared among all the animations
    * belonging to the animation set. This function enables sharing of the animation information across all the animations belonging to the animation set.
    * <p>The sharing is disabled by default.
    * <p>When the sharing is false. it considers the added animation as a box and then applies its own properties on top of the Box.
    * 
    * 
    * @param enabled :- Set to true to enable animation information sharing, set to false to disable animation information sharing
    */
    public void setSharedAnimationInfoEnabled(boolean enabled){
    	
    	if(mAnimationSet != null)
    		mAnimationSet.setShareAnimationInfo(enabled);
    
    }

     /**
      * Check if animation information sharing is enabled.
      *
      * @return true, if animation information sharing is enabled, false Otherwise.
      */
    public boolean isSharedAnimationInfoEnabled(){
    	if(mAnimationSet != null){
    		return mAnimationSet.isShareAnimationInfo();
    	}
    	
    	return false;
    }
    
}
