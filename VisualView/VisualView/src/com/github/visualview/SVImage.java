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

import android.graphics.Bitmap;

import com.github.sviengine.basetype.SVIImage;

/**
 * This class holds the data for an image object in the Visual View package.<p>
 * Visual View attaches image objects to a slide for rendering.
 * 
 */
public class SVImage {

    private SVIImage mImage;
    
    private boolean mRefEnabled = false;
    
   /**
	 * Creates an instance of SVImage without any pixel data.
	 */
	public SVImage() {
	    mImage = new SVIImage();
	}
	
	/**
	 *Creates an instance of SVImage without any pixel data but with a reference to a native engine if specified
	 * <p> If you set ref to true , the SVImage instance references a native engine. This approach is faster than 
	 * a normal constructor, but you must handle the bitmap lifecycle yourself. 
	 * 
	 * @param ref Set to true to reference a native engine; set to false to not reference a native engine.
	 */
	public SVImage(boolean ref){
		mImage = new SVIImage();
		mRefEnabled = ref;
	}

	/**
	 * Set the specified Android bitmap for the SVImage instance.
	 *
	 * @param bitmap The Android bitmap to add to the SVImage instance.
	 * 
	 */
	public void setBitmap(Bitmap bitmap) {
			
		if(!mRefEnabled){
			mImage.copyBitmap(bitmap,SVIImage.AlphaType.NORMAL);
		}else{
			mImage.setBitmap(bitmap, SVIImage.AlphaType.NORMAL);
		}
	}
	
	/**
	 * Get the Android bitmap currently set of the SVImage instance.
	 *
	 * @return The Android bitmap currently set for the SVImage instance.
	 */
	public Bitmap getBitmap(){
		return mImage.mBitmap;
	}
	
}