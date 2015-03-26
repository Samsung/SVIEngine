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
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;

import com.github.visualview.R;
import com.github.visualview.SVColor;
import com.github.visualview.SVGLSurface;
import com.github.visualview.SVImage;
import com.github.visualview.SVSlide;
import com.github.visualview.animation.SVAnimation;
import com.github.visualview.animation.SVSpriteAnimation;

public class SpriteAnimationActivity extends Activity implements OnTouchListener{

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
		
		mBgSlide.addSubSlide(mSubSlide);
		Toast.makeText(getApplicationContext(), "Touch to Start Animation", Toast.LENGTH_SHORT).show();
		mSurface.setOnTouchListener(this);
		
		setSpriteAnimation();
	}
	
	private void setSpriteAnimation(){
		InputStream is;
		int frameWidth = 0;
		int frameHeight = 0;
		is = getResources().openRawResource(R.drawable.explosion_sprite);
		mSpriteBitmap = BitmapFactory.decodeStream(is);
		mSpriteImage = new SVImage();
		if(mSpriteImage != null)
			mSpriteImage.setBitmap(mSpriteBitmap);
		
		if(mSpriteBitmap != null){
			frameWidth = mSpriteBitmap.getWidth() / 4;
			frameHeight = mSpriteBitmap.getHeight() / 5;
		}
		SVSpriteAnimation spriteAni = new SVSpriteAnimation(mSpriteImage, frameWidth, frameHeight);
		spriteAni.setDuration(1000);
		
		mAni = (SVAnimation)spriteAni;
	}
	
	@Override
	public boolean onTouch(View view, MotionEvent event) {
		if(event.getAction() == MotionEvent.ACTION_DOWN){
			
			mSubSlide.startAnimation(mAni);
			
			return true;
		}
		return false;
	}

	

	private Bitmap mSpriteBitmap = null;
	private SVImage mSpriteImage = null;
	private SVAnimation mAni = null;
	
	private SVGLSurface mSurface = null;
	private SVSlide mBgSlide = null;
	private SVSlide mSubSlide = null;
}
