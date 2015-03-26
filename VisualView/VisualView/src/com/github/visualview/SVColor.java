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



/**
 * This class holds the data for an RGBA color object in the Visual View package. The color parameters are normalized.
 * 
 */
public class SVColor {
	/**
	 * Red component.
	 */
	private float mR;
	
	/**
	 * Green component.
	 */
	private float mG;
	
	/**
	 * Blue component.
	 */
	private float mB;
	
	/**
	 * Alpha component.
	 */
	private float mA;
	
	/**
	 * Creates an instance of SVColor that is set to transparent black
	 */
	public SVColor() {
		
		mR = 0.0f;
		mG = 0.0f;
		mB = 0.0f;
		mA = 0.0f;
	}

	/**
	 * Creates an Instance of SVColor that is set to the specified color.
	 * <p>The color parameters must lie between 0~1 as they are normalized.
	 *
	 * @param r Red component,values between 0 and 1 allowed.
	 * @param g Green component,values between 0 and 1 allowed.
	 * @param b Blue component,values between 0 and 1 allowed.
	 * @param a Alpha component,values between 0 and 1 allowed.
	 * @throws IllegalArgumentException if r/g/b/a lies outside (0~1)
	 */
	public SVColor(float r, float g, float b, float a) {
		checkValue(r);
		checkValue(g);
		checkValue(b);
		checkValue(a);
		mR = r;
		mG = g;
		mB = b;
		mA = a;
	}
	
	
	/**
	 * Modifies the color components an SVColor object with the specified Color.
	 * <p>The color parameters must lie between 0~1 as they are normalized.
	 *
	 * @param r Red component,values between 0 and 1 allowed.
	 * @param g Green component,values between 0 and 1 allowed.
	 * @param b Blue component,values between 0 and 1 allowed.
	 * @param a Alpha component,values between 0 and 1 allowed.
	 * @throws IllegalArgumentException if r/g/b/a lies outside (0~1)
	 */
	public void setColor(float r, float g, float b, float a) {
		checkValue(r);
		checkValue(g);
		checkValue(b);
		checkValue(a);
		mR = r;
		mG = g;
		mB = b;
		mA = a;
	}
	
	/**
	 * Get the red component of the SVColor instance.
	 *
	 * @return float The red color component, value between 0 and 1.
	 */
	public float getRed(){
		return mR;
	}
	
	/**
	 * Get green component of the SVColor instance.
	 *
	 * @return float The green color component, value between 0 and 1.
	 */
	public float getGreen(){
		return mG;
	}
	
	/**
	 * Get the blue component of the SVColor instance.
	 *
	 * @return float The blue color component, value between 0 and 1.
	 */
	public float getBlue(){
		return mB;
	}
	
	/**
	 * Get the alpha component of the SVColor instance.
	 *
	 * @return float The alpha component, value between 0 and 1.
	 */
	public float getAlpha(){
		return mA;
	}
	
	private void checkValue(float value) throws IllegalArgumentException{
		if(value < 0.0f || value > 1.0f){
			throw new IllegalArgumentException("Color value must be between 0.0f ~ 1.0f");
		}
	}
}