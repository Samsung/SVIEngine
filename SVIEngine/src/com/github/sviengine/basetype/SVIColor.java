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
 * SVIColor Class
 * SVIColor has 4 elements. { R, G, B, A }
 */
public class SVIColor {
	
	/**
	 * default constructor
	 */
	public SVIColor() {
		mR = 0.0f;
		mG = 0.0f;
		mB = 0.0f;
		mA = 0.0f;
	}

	/**
	 * constructor
	 *
	 * @param r > red color float
	 * @param g > green color float
	 * @param b > blue color float
	 * @param a > alpha float
	 */
	public SVIColor(float r, float g, float b, float a) {
		mR = r;
		mG = g;
		mB = b;
		mA = a;
	}

	public float mR;
	public float mG;
	public float mB;
	public float mA;
}