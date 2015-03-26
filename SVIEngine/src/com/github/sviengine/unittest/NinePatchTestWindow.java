package com.github.sviengine.unittest;

import java.io.InputStream;
import java.util.Random;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.MotionEvent;

import com.github.sviengine.R;
import com.github.sviengine.slide.SVINinePatchSlide;
import com.github.sviengine.slide.SVISlide;

public class NinePatchTestWindow extends PlatformWindow{
	private final int 			DURATION = 1000;	
	private SVINinePatchSlide 	mSVINinePatchSlide = null;
	
	public NinePatchTestWindow(Context context) {
		super(context);
		buildMainSlide();
		createChildSlide();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		float x = event.getX();
		float y = event.getY();
		startAni(x, y, DURATION);        
        
		return super.onTouchEvent(event);
	}
	
	@Override
	public void onResize(int width, int height) {
		buildMainSlide();
		super.onResize(width, height);
	}
	
	public void startAni(float x, float y, int duration){		
		if (mSVINinePatchSlide == null)
			return;
		
		if (!mSVINinePatchSlide.isAnimating()) {
			Random random = new Random();
			float scaleX = random.nextInt(3);
			float scaleY = random.nextInt(4);
			//float rotationZ = random.nextInt(360);
					
			mSVINinePatchSlide.setScale(scaleX, scaleY, 0f, duration);
			//mSVINinePatchSlide.setRotation(0f, 0f, rotationZ, 0f, duration);
			mSVINinePatchSlide.setPosition(x, y, duration);
		}
	}
	
	public void release(){
		mSVINinePatchSlide.destroy();
		destroySlide();
		
		super.release();
	}
	
	private void buildMainSlide(){
		if( getMainSlide() == null )
			super.buildSubSlide();
		
		SVISlide mainSlide = getMainSlide();
		
		float[] color = {1f, 1f, 1f, 1f};
		mainSlide.setBackgroundColor(color);
	}
	
	private void createChildSlide(){
		InputStream is;
		is = getResources().openRawResource(R.drawable.ninepatch_sample);
		Bitmap bitmap = BitmapFactory.decodeStream(is);		
		
		SVISlide mainSlide = getMainSlide();
		mSVINinePatchSlide = new SVINinePatchSlide(bitmap);
		mainSlide.addSlide(mSVINinePatchSlide);
		mSVINinePatchSlide.setPosition(500, 500);
	}	
	
	private boolean destroySlide() {
		SVISlide mainSlide = getMainSlide();
		
		if( mainSlide != null ) {
			for(int i=mainSlide.getSubSlideCount()-1; i>=0; i--) {
				SVISlide removeSlide = mainSlide.getSubSlide(i);
				if( removeSlide != null )
					mainSlide.removeSlide(removeSlide);
			}
		}
		return true;
	}
}