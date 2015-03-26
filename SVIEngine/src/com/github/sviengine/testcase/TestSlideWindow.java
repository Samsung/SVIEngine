package com.github.sviengine.testcase;

import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIBasicAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.util.Log;
import android.view.MotionEvent;

public class TestSlideWindow extends PlatformWindow implements SVIAnimationListener{

	public TestSlideWindow(Context context) {
		super(context);
		buildSubSlide();
	}
	
	
	@Override
	protected void buildSubSlide(){
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		
		SVISlide mainSlide = getMainSlide();
		
		mainSlide.setBackgroundColor(color);
		mainSlide.setRegion(0.0f, 0.0f, mWidth, mHeight);

		
		float redColor[] = {1.0f, 0.0f, 0.0f, 1.0f };
		mSlide = new SVISlide(0,0,100, 100, 600.0f, 600.0f, redColor);
		//mSlide.setImage(mImages[0]);
		mainSlide.addSlide(mSlide);
		
		mainSlide.setOrthogonal(false);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		if(event.getAction() == MotionEvent.ACTION_DOWN) {
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setDuration(4000);
			aniSet.setOffset(0);
			aniSet.setRepeatCount(2);
			
			
			int type = SVIPropertyAnimation.PropertyAnimationType.ROTATION;
			float[] from1 = new float[]{0.0f, 0.0f, 0.0f};
			float[] to1 = new float[]{0.0f, 0.0f, 90.0f};
			SVIBasicAnimation ani1 = new SVIBasicAnimation(type, from1, to1);
			ani1.setOffset(2000);
			ani1.setDuration(2000);
			
			type = SVIPropertyAnimation.PropertyAnimationType.BORDER_WIDTH;
			SVIBasicAnimation ani2 = new SVIBasicAnimation(type, 0.0f, 100.0f);
			
			ani2.setDuration(1000);
			aniSet.addAnimation(ani1);
			aniSet.addAnimation(ani2);
			
			
			mSlide.startAnimation(aniSet);
		}
		
		return true;
	}
	
	@Override
	public void onAnimationEnd(String tag) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAnimationRepeat(String tag) {
		
		Log.i("test", "test is test");
		
	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
		
	}
	
	
	
	private SVISlide mSlide = null;
}
