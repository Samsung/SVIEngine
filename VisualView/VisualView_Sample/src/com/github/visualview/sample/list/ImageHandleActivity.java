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
package com.github.visualview.sample.list;

import java.io.InputStream;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.os.Bundle;

import com.github.visualview.R;
import com.github.visualview.SVColor;
import com.github.visualview.SVGLSurface;
import com.github.visualview.SVImage;
import com.github.visualview.SVSlide;


public class ImageHandleActivity extends Activity{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		mSurface = new SVGLSurface(this);
		
		setContentView(mSurface);
		Point windowSize = new Point();

		this.getWindowManager().getDefaultDisplay().getSize(windowSize);

		mBgSlide = new SVSlide(null, 0.0f, 0.0f, windowSize.x, windowSize.y, new SVColor(1.0f, 1.0f, 1.0f, 1.0f));
		mSurface.addSlide(mBgSlide);
		
		mSubSlide = new SVSlide(null, 100.0f, 100.0f, 500.0f, 500.0f, new SVColor(1.0f, 1.0f, 1.0f, 1.0f));
		
		if(this.getResources().getConfiguration().orientation == 2)//Landscape
			mSubAdvanceSlide = new SVSlide(null, 700.0f, 100.0f, 500.0f, 500.0f, new SVColor(1.0f, 1.0f, 1.0f, 1.0f));
		else//portrait
			mSubAdvanceSlide = new SVSlide(null, 100.0f, 700.0f, 500.0f, 500.0f, new SVColor(1.0f, 1.0f, 1.0f, 1.0f));
		
		
		mBgSlide.addSubSlide(mSubSlide);
		mBgSlide.addSubSlide(mSubAdvanceSlide);
		
		setImage();
		setAdvancedImage();
	}
	
	private void setImage(){
		InputStream is;
		
		is = getResources().openRawResource(R.drawable.tr_00);
		Bitmap bitmap = BitmapFactory.decodeStream(is);
		
			
		mNormalImage = new SVImage(); // will copy to Native side
		mNormalImage.setBitmap(bitmap);
		mSubSlide.setImage(mNormalImage);
	}
	
	private void setAdvancedImage(){
		InputStream is;
		
		is = getResources().openRawResource(R.drawable.tr_01);
		mAdvancedBitmap = BitmapFactory.decodeStream(is);
		
			
		mAdvancedImage = new SVImage(true); // will reference to Native side
		mAdvancedImage.setBitmap(mAdvancedBitmap);
		mSubAdvanceSlide.setImage(mAdvancedImage);
		
	}
	

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		
		//destroy logic
		if(mSubSlide != null){
			SVImage image = mSubSlide.getImage();
			if(image != null){
				Bitmap bitmap = image.getBitmap();
				if(bitmap != null){
					bitmap.recycle();
				}
				image.setBitmap(null);
			}
			mSubSlide.setImage((SVImage)null);
		}
		
		
		if(mSubAdvanceSlide != null){
			if(mAdvancedImage != null){
				Bitmap bitmap = mAdvancedImage.getBitmap();
				if(bitmap != null){
					bitmap.recycle();
				}
				mAdvancedImage.setBitmap(null);
			}
			mSubAdvanceSlide.setImage((SVImage)null);
		}
		
	}
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}



	private SVGLSurface mSurface = null;
	private SVSlide mBgSlide = null;
	private SVSlide mSubSlide = null;
	private SVSlide mSubAdvanceSlide = null;
	
	private SVImage mNormalImage = null;
	
	private SVImage mAdvancedImage = null;
	private Bitmap mAdvancedBitmap = null;
}