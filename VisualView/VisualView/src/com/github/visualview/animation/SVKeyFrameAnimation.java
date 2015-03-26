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

import java.util.ArrayList;
import java.util.List;

import com.github.sviengine.animation.SVIKeyFrameAnimation;

/**
 * This class implements keyframe animation. 
 * You can specify keyframes in the time range 0.0f to 1.0f to control the animation. 
 * You can add any number of keyframes.
 * Key frame entered must be bigger than previous key frame value. (etc. 0.0f -> 0.5f -> 1.0f)
 * <p>
 * 
 *  As shown below VisualView throws exception if time values are not in order or out of the range.<p>
 *  <br><p>
 *  IllegalArgumentException<-- (0.0f)-+--+--+-(0.2)--+--+--+--+-(0.5)--+-..So..on..--+-(1.0f) ---> IllegalArgumentException<p>
 *  - - - - ---------------- -startkeyProperty()<--------addKeyProperty()---------->endKeyProperty()- ----------- - -<p>
 * 
 */
public class SVKeyFrameAnimation extends SVAnimation {
    
	SVIKeyFrameAnimation mKeyframeAnimation;
	
	private boolean mUsedStart = false;
	private boolean mUsedEnd = false;
	private boolean mSingleType;
	private int mType;
	
	private List<Float> keyFrameTimeValues = new ArrayList<Float>();
	
    /**
     * Creates an instance of SVKeyFrameAnimation for the specified slide property type.
     * 
     * @param type The slide property to change, see the list of slide properties in the SVAnimation class for more information
     * @see SVAnimation
     */
    public SVKeyFrameAnimation(int type) {
    	mType = type;
		mSingleType = checkSingleValue(mType);
    	mAnimation = new SVIKeyFrameAnimation(mType);
    	mKeyframeAnimation = (SVIKeyFrameAnimation)mAnimation;
    }

    /**
     * Add a new animation key property to the animation with the specified key time and float value. 
     * Use this method for properties with only one float value such as light angle.
     * You can enter values between time between 0.0f  and 1.0f.
     * If the property type take the value as float[]  then you should use the overloaded function.<p>
     * 
     * @param keyTime The time in the range 0.0f to 1.0f.
     * @param value The value at the given time (like opacity,cornerRadius ,..etc)
     * @throws IllegalArgumentException if the animation type takes an array as its argument. or if the time is invalid
     * 
     * @see SVAnimation
     */
    public void addKeyProperty(float keyTime, float value){
    	if(!mSingleType){
    		throw new IllegalArgumentException("Type is not a single value type. Use Array value type function.");
    	}
    	
    	isAddKeyTimeValid(keyTime);
		
    	
    	keyFrameTimeValues.add(keyTime);
    	if(mKeyframeAnimation != null){
    		mKeyframeAnimation.addKeyProperty(keyTime,value);
    	}
    }
    
    /**
     * Add value of animation at given time.If the animation type take the value as float then use the overloaded function.<p>
     * 
     * @param keyTime The key-frame time with the following input validation otherwise Exception will be thrown
     * 				<p>1.) Can't take negative value neither value greater then 1.0f
     * 				<p>2.) Value should be greater then the previous value
     * 				<p>3.) Value should be between 0.0f and 1.0f. For 0.0f use startKeyProperty() and endKeyProperty for 1.0f
     * @param value The float array value for the slide property at the keyframe (for example, color, region or rotation) see the list of slide properties in the SVAnimation class for more information.
     * @throws IllegalArgumentException if animation type doesn't take array as its argument or has the mismatched array length or if the input time is invalid
     * 
     */
    public void addKeyProperty(float keyTime, float[] value){
    	if(mSingleType){
    		throw new IllegalArgumentException("Type is not a Array value type. Use Single value type function.");
    	}
    	//Validate array length
    	checkArrayLength(value);
    	isAddKeyTimeValid(keyTime);
    	
    	keyFrameTimeValues.add(keyTime);
    	if(mKeyframeAnimation != null){
    		mKeyframeAnimation.addKeyProperty(keyTime,value);
    	}
    }
    
    
    /**
     * Enter Value at Key-time(0.0f).<p>
     * The function throws exception if the animation type takes the value as float[].use Overloaded Function otherwise
     * 
     * @param value The float value for the slide property at time 0.0f (for example opacity or corner radius), see the list of slide properties in the SVAnimation class for more information.
     * @throws IllegalArgumentException if animation property type takes array as its argument.
     * 
     * @see SVAnimation
     */
    public void startKeyProperty(float value) {
    	if(!mSingleType){
    		throw new IllegalArgumentException("Type is not a single value type. Use Array value type function.");
    	}
    	keyFrameTimeValues.add(0.0f);
    	mUsedStart = true;
    	if(mKeyframeAnimation != null){
    		mKeyframeAnimation.addKeyProperty(0.0f,value);
    	}
    }
    
    /**
     * Enter Value of animation at Key-time(0.0f).<p>
     * The function throws exception if the animation type takes the value as float. 
     *
     * @param value The Value at the time 0.0f.(like  color , region , rotation  ,..etc).see the list of slide properties in the SVAnimation class for more information.
     * @throws IllegalArgumentException if animation type doesn't take array as its argument or has the mismatched array length
     * @see SVAnimation
     */
    public void startKeyProperty(float[] value) {
    	if(mSingleType){
    		throw new IllegalArgumentException("Type is not a Array value type. Use Single value type function.");
    	}
    	checkArrayLength(value);
    	keyFrameTimeValues.add(0.0f);
    	mUsedStart = true;
    	if(mKeyframeAnimation != null){
    		mKeyframeAnimation.addKeyProperty(0.0f, value);
    	}
    }
    
    /**
     * Enter value of animation at Time(1.0f).<p>
     * The function throws exception if the animation type takes the value as float array.
     * 
     * @param value Value at the keyframe (like opacity,cornerRadius ,..etc)
     * @throws IllegalArgumentException if animation property type takes array as its argument.
     * @see SVAnimation
     */
    public void endKeyProperty(float value) {
    	if(!mSingleType){
    		throw new IllegalArgumentException("Type is not a single value type. Use Array value type function.");
    	}
    	keyFrameTimeValues.add(1.0f);
    	mUsedEnd = true;
    	if(mKeyframeAnimation != null){
    		mKeyframeAnimation.addKeyProperty(1.0f,value);
    	}
    }
    
    /**
     * Enter Value of animation at Key-time(1.0f).<p>
     * The function throws exception if the animation type takes the value as float. 
     *
     * @param value The value at the keyframe.(like  color , region , rotation  ,..etc).see the list of slide properties in the SVAnimation class for more information.
     * @throws IllegalArgumentException if animation type doesn't take array as its argument or has the mismatched array length
     * @see SVAnimation
     */
    public void endKeyProperty(float[] value) {
    	if(mSingleType){
    		throw new IllegalArgumentException("Type is not a Array value type. Use Single value type function.");
    	}
    	checkArrayLength(value);
    	keyFrameTimeValues.add(1.0f);
    	mUsedEnd = true;
    	if(mKeyframeAnimation != null){
    		mKeyframeAnimation.addKeyProperty(1.0f, value);
    	}
    }

    
    
    /**
     * check if the KeyFrameTime is Valid.
     * The KeyFrameTime should lie between 0.0f to 1.0f for the smooth transition of animation.
     * Any Value other then this throws exception and there will be no animation.
     *
     *@return boolean true if Valid , Exception otherwise.
     *@throws IllegalArgumentException If invalid time values
     *	  <p> 1. if the values at time 0.0 is not given. (user must use startKeyProperty() for the same)
     *	  <p> 2. if the values at time 1.0 is not given.  (user must use EndKeyProperty() for the same)
     *    <p> 3. if time values is not in ascending order (must follow 0.0 < 0.2 <0.5 < 1.0f)
     *    <p> 4. if the time values is out of the range. (must be 0.0f < Time < 1.0f)
     */
    public boolean isKeyFrameTimeValid() {
    	
    	if(keyFrameTimeValues.size()<2){
    		throw new IllegalArgumentException("Need SVKeyFrameAnimation::addKeyProperty() for atleast StartKey and EndKey");
    	}else if (!keyFrameTimeValues.contains(0.0f) && !keyFrameTimeValues.contains(1.0f)
    			||!mUsedStart&&!mUsedEnd){
    		throw new IllegalArgumentException("Need SVKeyFrameAnimation::startKeyProperty() & endKeyProperty()");
    	}else if (!keyFrameTimeValues.contains(0.0f) && keyFrameTimeValues.contains(1.0f)
    			||!mUsedStart&&mUsedEnd){
    		throw new IllegalArgumentException("Need SVKeyFrameAnimation::startKeyProperty()");
    	}else if (keyFrameTimeValues.contains(0.0f) && !keyFrameTimeValues.contains(1.0f)
    			||mUsedStart&&!mUsedEnd){
    		throw new IllegalArgumentException("Need SVKeyFrameAnimation::endKeyProperty()");
    	}else if (keyFrameTimeValues.contains(0.0f) && keyFrameTimeValues.contains(1.0f)){
    		//check for the bounds and valid order
    		for(int i = 0; i< keyFrameTimeValues.size()-1;i++){
    			if(keyFrameTimeValues.get(i+1) < keyFrameTimeValues.get(i))
    				throw new IllegalArgumentException("SVKeyFrameAnimation::Invalid order of time..!!");
    			if(keyFrameTimeValues.get(i) <0.0f || keyFrameTimeValues.get(i) > 1.0f)
    				throw new IllegalArgumentException("SVKeyFrameAnimation::Out of bounds..time should lie between 0.0f~0.1f");
    		}
    		return true;
    	}
    	
    	
    	
    	return false;
    }
    
    private boolean isAddKeyTimeValid(float value){
    	
    	
    	if(value == 0.0f || value == 1.0f){
    		throw new IllegalArgumentException("SVKeyFrameAnimation::addKeyProperty() Can't use  for 0.0f or 1.0f. use Startkey and endkey property functions");
    	}else{
    		return true;
    	}
    }
    
  
    
    private void checkArrayLength(float[] value){
    	int size = value.length;
    	int length = 0;
    	switch (mType) {
    	//length 4
		case SVAnimation.REGION:
		case SVAnimation.BACKGROUND_COLOR:
		case SVAnimation.SCALE_TO_FIT_REGION:
		case SVAnimation.TEXTURE_REGION:
		case SVAnimation.BORDER_COLOR:
		case SVAnimation.LIGHT_COLOR:
		case SVAnimation.SHADOW_COLOR:
			length = 4;
			break;
			
		//length 3
		case SVAnimation.ROTATION:
		case SVAnimation.SCALE:
			length = 3;
			break;
		
		//length 2
		case SVAnimation.POSITION:
		case SVAnimation.PIVOT_POINT:
		case SVAnimation.LIGHT_OFFSET:
		case SVAnimation.SHADOW_OFFSET:
			length = 2;
			break;
			
		default:
			length = 0;
			break;
		}
    	
    	if(length != size){
    		throw new IllegalArgumentException("SVKeyFrameAnimation:: Array length is wrong. ");
    	}
    }
    
    
    private boolean checkSingleValue(int value){
    	if(value ==	SVAnimation.Z_POSITION
    		|| value == SVAnimation.OPACITY
    		|| value == SVAnimation.CORNER_RADIUS
    		|| value == SVAnimation.LIGHT_ANGLE
    		|| value == SVAnimation.LIGHT_POWER
    		|| value == SVAnimation.LIGHT_OPACITY
    		|| value == SVAnimation.LIGHT_RADIUS
    		|| value == SVAnimation.SHADOW_OPACITY
    		|| value == SVAnimation.SHADOW_RADIUS
    		|| value == SVAnimation.BORDER_WIDTH){
    		return true;
    	}
    	else if(value == SVAnimation.BACKGROUND_COLOR
        		|| value == SVAnimation.BORDER_COLOR
        		|| value == SVAnimation.LIGHT_COLOR
        		|| value == SVAnimation.LIGHT_OFFSET
        		|| value == SVAnimation.PIVOT_POINT
        		|| value == SVAnimation.POSITION
        		|| value == SVAnimation.REGION
        		|| value == SVAnimation.ROTATION
        		|| value == SVAnimation.SCALE_TO_FIT_REGION
        		|| value == SVAnimation.SHADOW_COLOR
        		|| value == SVAnimation.SHADOW_OFFSET
        		|| value == SVAnimation.TEXTURE_REGION
        		|| value == SVAnimation.SCALE){
    		return false;
    	}else{
    		throw new IllegalArgumentException("SVKeyFrameAnimation :: Invalid Key-Frame Animation type ..!!");
    	}
    }
    
}
