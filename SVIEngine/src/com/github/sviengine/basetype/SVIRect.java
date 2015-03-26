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

import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVISize;

import android.graphics.Rect;


/**
 * SVIRect Class
 * @see Rect
 * like Android Rect Class
 */
public class SVIRect {

	/**
	 * default constructor
	 */
	public SVIRect() {		
	}

	/**
	 * constructor use SVIPoint
	 *
	 * @param origin > SVIPoint about start point
	 * @param size > SVISize about rectangle size
	 */
	public SVIRect(SVIPoint origin, SVISize size) {

		mOrigin = origin;
		mSize = size;
	}

	/**
	 * constructor use SVIRect
	 *
	 * @param rect > SVIRect class
	 */
	public SVIRect(SVIRect rect) {
		mOrigin.mX = rect.mOrigin.mX;
		mOrigin.mY = rect.mOrigin.mY;
		mSize.mWidth = rect.mSize.mWidth;
		mSize.mHeight = rect.mSize.mHeight;
	}
	
	/**
	 * is same rect
	 * compare A and B, if same than return true
	 *
	 * @param rect > SVIRect class
	 * @return > true = same false = different
	 */
	public boolean isSame(SVIRect rect){
		if (mOrigin.mX != rect.mOrigin.mX) return false;
		if (mOrigin.mY != rect.mOrigin.mY) return false;
		if (mSize.mWidth != rect.mSize.mWidth) return false;
		if (mSize.mHeight != rect.mSize.mHeight) return false;
		return true;
	}

	/**
	 * copy rect
	 *
	 * @param rect > SVIRect to copy
	 */
	public void copy(SVIRect rect){	
		mOrigin.mX = rect.mOrigin.mX;
		mOrigin.mY = rect.mOrigin.mY;
		mSize.mWidth = rect.mSize.mWidth;
		mSize.mHeight = rect.mSize.mHeight;		
	}

	/**
	 * constructor use x, y, width, height
	 *
	 * @param x > start point x
	 * @param y > start point y
	 * @param width > rectangle width
	 * @param height > rectangle height
	 */
	public SVIRect(float x, float y, float width, float height) {
		mOrigin.mX = x;
		mOrigin.mY = y;
		mSize.mWidth = width;
		mSize.mHeight = height;
	}
	
	/**
	 * check hit rect use SVIPoint
	 *
	 * @param point > pointed spot
	 * @return > true = in rectangle region  false = out rectangle region
	 */
	public boolean checkHit(SVIPoint point){		
		if (mOrigin.mX < point.mX && mOrigin.mX+mSize.mWidth > point.mX && 
			mOrigin.mY < point.mY && mOrigin.mY+mSize.mHeight > point.mY ) return true;
		return false;
	}	

	
	/**
	 * check hit rectanlge use SVIRect
	 *
	 * @param rect > new rectangle
	 * @return > true = new rectangle checked origin rectangle  false = new rectangle not checked origin rectangle
	 */
	public boolean checkHit(SVIRect rect){
		SVIPoint leftTopCorner = new SVIPoint(rect.mOrigin.mX,rect.mOrigin.mY);
		SVIPoint leftBottomCorner = new SVIPoint(rect.mOrigin.mX,rect.mOrigin.mY+rect.mSize.mHeight);
		SVIPoint rightTopCorner = new SVIPoint(rect.mOrigin.mX+rect.mSize.mWidth,rect.mOrigin.mY);
		SVIPoint rightBottomCorner = new SVIPoint(rect.mOrigin.mX+rect.mSize.mWidth,rect.mOrigin.mY+rect.mSize.mHeight);
		
		//2011-12-28 masterkeaton27@gmail.comadd intersection test
		//check intersection
		if (checkHit(leftTopCorner) || checkHit(leftBottomCorner) || 
				checkHit(rightTopCorner) || checkHit(rightBottomCorner)) return true;
		
		//check contains
		if( mOrigin.mX < rect.mOrigin.mX && 
			mOrigin.mX + mSize.mWidth > rect.mOrigin.mX + rect.mSize.mWidth &&
			mOrigin.mY < rect.mOrigin.mY &&
			mOrigin.mY + mSize.mHeight > rect.mOrigin.mY + rect.mSize.mHeight ) 
			return true;
		
		return false;
	}
	
	/**
	 * check contain SVIRect
	 *
	 * @param rect > new rectangle
	 * @return > true = new rectangle inside origin rectangle  false = new rectangle outside origin rectangle
	 */
	public boolean checkContains(SVIRect rect){
		//check contains
		if( mOrigin.mX < rect.mOrigin.mX && 
			mOrigin.mX + mSize.mWidth > rect.mOrigin.mX + rect.mSize.mWidth &&
			mOrigin.mY < rect.mOrigin.mY &&
			mOrigin.mY + mSize.mHeight > rect.mOrigin.mY + rect.mSize.mHeight ) 
			return true;
		
		return false;
	}
	
	public SVIPoint mOrigin = new SVIPoint();
	public SVISize mSize = new SVISize();

}