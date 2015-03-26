/*

 * Copyright (C) 2011 SVI.

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

package com.github.sviengine.basetype;


/**
 * SVIVector3 Class has Vector3 property.
 * x, y, z elements
 */
public class SVIVector3 {

	/**
	 * SVIVector3 Default Constructor
	 */
	public SVIVector3() {
		mX = 0;
		mY = 0;
		mZ = 0;
	}

	/**
	 * constructor
	 *
	 * @param x > x
	 * @param y > y 
	 * @param z > z
	 */
	public SVIVector3(float x, float y, float z) {
		mX = x;
		mY = y;
		mZ = z;
	}

	public float mX;
	public float mY;
	public float mZ;
}