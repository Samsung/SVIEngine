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
 * This class holds data for 4 dimensional vector objects in the Visual View package.
 *  
 */
public class SVVector4 {
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
	 * Vector w coordinate.
	 */
	private float mW;

	/**
	 * Creates an instance of SVVector4 with the attributes set to zero.
	 */
	public SVVector4() {
		mX = 0;
		mY = 0;
		mZ = 0;
		mW = 0;	
	}

	/**
	 *Constructs an SVVector4 object for the specified dimensions.
	 *
	 * @param x Float value for the X coordinate for the vector. 
	 * @param y Float value for the Y coordinate for the vector. 
	 * @param z Float value for the Z coordinate for the vector. 
	 * @param w Float value for the W coordinate for the vector. 
	 */
	public SVVector4(float x, float y, float z, float w) {
		mX = x;
		mY = y;
		mZ = z;
		mW = w;	
	}
	
	/**
	 * set the Vector Params.
	 *
	 * @param x Float value for the X coordinate for the vector. 
	 * @param y Float value for the Y coordinate for the vector. 
	 * @param z Float value for the Z coordinate for the vector. 
	 * @param w Float value for the W coordinate for the vector.
	 */
	public void setVector(float x , float y , float z, float w){
		mX = x;
		mY = y;
		mZ = z;	
		mW = w;	
	}
	
	/**
	 * get the X Coordinate.
	 *
	 * @return Float value for the X coordinate
	 */
	public float getX(){
		return mX;
	}
	
	/**
	 * get the Y Coordinate.
	 *
	 * @return Float value for the Y coordinate
	 */
	public float getY(){
		return mY;
	}
	
	/**
	 * get the Z Coordinate.
	 *
	 * @return Float value for the Z coordinate
	 */
	public float getZ(){
		return mZ;
	}
	
	/**
	 * get the W Coordinate.
	 *
	 * @return Float value for the W coordinate
	 */
	public float getW(){
		return mW;
	}
}