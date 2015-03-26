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
 * This class holds the data for a rectangle object in the Visual View package.
 *  
 */
public class SVRect {
	/**
	 * Rectangle Left Top x coordinate.
	 */
	private float mX;
	
	/**
	 * Rectangle Left Top y coordinate.
	 */
	private float mY;
	
	/**
	 * Rectangle width.
	 */
	private float mWidth;
	
	/**
	 * Rectangle height.
	 */
	private float mHeight;
	
	

	/**
	 * Creates an instance of SVRect with attribute values set to 0.
	 */
	public SVRect() {
		mX = 0;
		mY = 0;
		mWidth = 0;
		mHeight = 0;
	}

	
	/**
	 *Creates an instance of SVRect with the specified values.
	 *
	 * @param x X coordinate.
	 * @param y X coordinate. 
	 * @param width The Rectangle width.
	 * @param height The Rectangle height.
	 * @throws IllegalArgumentException if width/Height is a negative value.
	 */
	public SVRect(float x, float y, float width, float height) {
		checkLowValue(width);
		checkLowValue(height);
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
	}
	
	
	/**
	 * Set the specified values for the SVRect instance.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @param width The rectangle width.
	 * @param height The rectangle height.
	 * @throws IllegalArgumentException if width/Height is a negative value.
	 */
	public void setRect(float x, float y , float width , float height){
		checkLowValue(width);
		checkLowValue(height);
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
	}
	
	
	/**
	 * Get the X coordinate of the rectangular area.
	 * @return float The X coordinate
	 */
	public float getX(){
		return mX;
	}
	
	/**
	 * Get the Y coordinate of the rectangular area.
	 * @return float The Y coordinate.
	 */
	public float getY(){
		return mY;
	}
	
	/**
	 * Get the Width coordinate of the rectangular area.
	 * @return float The width of the rectangle
	 */
	public float getWidth(){
		return mWidth;
	}
	
	/**
	 * Get the Height coordinate of the rectangular area.
	 * @return float The height of the Rectangle
	 */
	public float getHeight(){
		return mHeight;
	}
	
	private void checkLowValue(float value) throws IllegalArgumentException{
		if(value < 0.0f ){
			throw new IllegalArgumentException("Width/Height must ne > 0");
		}
	}
}