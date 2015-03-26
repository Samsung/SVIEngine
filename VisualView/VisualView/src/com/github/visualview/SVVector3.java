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
 * This class holds data for 3 dimensional vector objects in the Visual View package.
 * 
 */
public class SVVector3 {
	/**
	 * Vector x coordinate.
	 */
	private float mX;
	
	/**
	 * Vector x coordinate.
	 */
	private float mY;
	
	/**
	 * Vector z coordinate.
	 */
	private float mZ;

	/**
	 * Creates an instance of SVVector3 with the attributes set to zero.
	 */
	public SVVector3() {
		mX = 0;
		mY = 0;
		mZ = 0;	
	}

	/**
	 *Constructs an SVVector3 object for the specified dimensions.
	 *
	 * @param x Float value for the x coordinate for the vector. 
	 * @param y Float value for the y coordinate for the vector. 
	 * @param z Float value for the z coordinate for the vector. 
	 */
	public SVVector3(float x, float y, float z) {
		mX = x;
		mY = y;
		mZ = z;	
	}
	
	/**
	 * set the Vector Params.
	 *
	 * @param x Float value for the x coordinate for the vector. 
	 * @param y Float value for the y coordinate for the vector. 
	 * @param z Float value for the z coordinate for the vector. 
	 */
	public void setVector(float x , float y , float z){
		mX = x;
		mY = y;
		mZ = z;	
	}
	
	/**
	 * get the X Coordinate.
	 *
	 * @return Float value for the X coordinate.
	 */
	public float getX(){
		return mX;
	}
	
	/**
	 * get The  Y Coordinate.
	 *
	 * @return Float value for the Y coordinate.
	 */
	public float getY(){
		return mY;
	}
	
	/**
	 * get the  Z Coordinate.
	 *
	 * @return Float value for the Z coordinate.
	 */
	public float getZ(){
		return mZ;
	}

}