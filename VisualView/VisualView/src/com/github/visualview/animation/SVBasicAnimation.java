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

import com.github.sviengine.animation.SVIBasicAnimation;


/**
 * This class provides the basic functionality for animating changes to one Property Animation Type at a time. 
 * You can See SVAnimation Class for the type of properties.
 * 
 * @see SVAnimation
 */
public class SVBasicAnimation extends SVAnimation {


    SVIBasicAnimation mBasicAnimation; 
    
    
    /**
	 * Creates an instance of SVBasicAnimation with the specified values.
	 * 
	 * @param type The slide property to change, see the list of slide properties in the SVAnimation class for more information.
	 * @param from The initial float value for the property, see the list of slide properties in the SVAnimation class for more information
	 * @param to The final float value for the property, see the list of slide properties in the SVAnimation class for more information.
	 * @throws IllegalArgumentException if animation type from/to values are array type. 
	 * @see SVAnimation
	 */
    public SVBasicAnimation(int type, float from, float to) {
    	
    	
    	if(!checkSingleValue(type)){
    		throw new IllegalArgumentException("Type is not a single value type. Use Array value type function.");
    	}
    	
    	mAnimation = new SVIBasicAnimation(type,from,to);
    	mBasicAnimation = (SVIBasicAnimation)mAnimation;
    }
    
	/**
	 * Creates an instance of SVBasicAnimation with the specified values.
	 * 
	 * @param type The slide property to change, see the list of slide properties in the SVAnimation class for more information.
	 * @param from The initial float array value for the property, see the list of slide properties in the SVAnimation class for more information.
	 * @param to The final float array value for the property, see the list of slide properties in the SVAnimation class for more information.
	 * @throws IllegalArgumentException if the animation type doesn't take array as its argument or has the mismatched array length.
	 * @see SVAnimation
	 */
    public SVBasicAnimation(int type, float[] from, float[] to) {
    	if(checkSingleValue(type)){
    		throw new IllegalArgumentException("Type is not a Array value type. Use Single value type function.");
    	}
    	checkArrayLength(type,from, to);
    	mAnimation = new SVIBasicAnimation(type,from,to);
    	mBasicAnimation = (SVIBasicAnimation)mAnimation;
    }
    
    private void checkArrayLength(int type ,float[] from, float[] to){
    	int fromsize = from.length;
    	int tosize = to.length;
    	
    	if(fromsize != tosize){
    		throw new IllegalArgumentException("SVBasicAnimation:: Array length is wrong. ");
    	}
    	int length = 0;
    	switch (type) {
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
    	
    	if(length != fromsize){
    		throw new IllegalArgumentException("SVBasicAnimation:: Array length is wrong. ");
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
    		throw new IllegalArgumentException("SVBasicAnimation :: Invalid Basic Animation type ..!!");
    	}
    }
    
    
    
    
}
