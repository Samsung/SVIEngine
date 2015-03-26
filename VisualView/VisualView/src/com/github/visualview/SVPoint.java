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
 * This class holds the data for a point object in the Visual View package.
 * 
 */
public class SVPoint {
	/**
	 * Rectangle x coordinate.
	 */
	private float mX;
	
	/**
	 * Rectangle y coordinate.
	 */
	private float mY;
	
	/**
	 * Creates an instance of SVPoint with attribute values set to 0.
	 */
	public SVPoint() {
		mX = 0;
		mY = 0;
	}

	/**
	 *Creates an instance of SVPoint with the specified values.
	 *
	* @param x The X coordinate
	 * @param y The Y coordinate
	 */
	public SVPoint(float x, float y) {
		mX = x;
		mY = y;
	}
	
	/**
	 * Set the specified coordinates for the point.
	 * @param x The X coordinate
	 * @param y The Y coordinate
	 */
	public void setPosition(float x, float y){
		mX = x;
		mY = y;
	}
	
	/**
	 * get the X Position 
	 * @return float The X coordinate
	 */
	public float getX(){
		return mX;
	}
	
	/**
	 * get the Y position 
	 * @return float The Y coordinate
	 */
	public float getY(){
		return mY;
	}
}