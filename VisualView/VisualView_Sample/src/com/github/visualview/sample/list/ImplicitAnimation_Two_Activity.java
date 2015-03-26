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

import android.app.Activity;
import android.graphics.Point;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;

import com.github.visualview.SV;
import com.github.visualview.SVColor;
import com.github.visualview.SVGLSurface;
import com.github.visualview.SVPoint;
import com.github.visualview.SVSlide;
import com.github.visualview.SVSlideManager;

public class ImplicitAnimation_Two_Activity extends Activity implements OnTouchListener{

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
		
		mSubSlide = new SVSlide(null, 100.0f, 100.0f, 500.0f, 500.0f, new SVColor(1.0f, 0.0f, 0.0f, 1.0f));
		
		mBgSlide.addSubSlide(mSubSlide);
		Toast.makeText(getApplicationContext(), "Touch to Start Animation", Toast.LENGTH_SHORT).show();
		mSurface.setOnTouchListener(this);
		
	}
	
	private void setImplicitAnimation(){
		int duration = 3000;
		//with checkIn & checkOut
		SVSlideManager.checkOutAnimation();
		SVSlideManager.setDuration(duration);
		SVSlideManager.setInterpolatorType(SV.INTERPOLATOR_ACCELERATE_DECELERATE);
		
		mSubSlide.setPosition(new SVPoint(500.0f, 500.0f));
		mSubSlide.setBorderWidth(100.0f);
		
		SVSlideManager.checkInAnimation();
		
	}
	
	@Override
	public boolean onTouch(View view, MotionEvent event) {
		if(event.getAction() == MotionEvent.ACTION_DOWN){
			
			setImplicitAnimation();
			
			return true;
		}
		return false;
	}
	

	private SVGLSurface mSurface = null;
	private SVSlide mBgSlide = null;
	private SVSlide mSubSlide = null;
}