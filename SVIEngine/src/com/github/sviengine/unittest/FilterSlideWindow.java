package com.github.sviengine.unittest;

import java.util.Random;
import java.util.Vector;

import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class FilterSlideWindow extends PlatformWindow{

	public FilterSlideWindow(Context context) {
		super(context);
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
		mWidth = w;
		mHeight = h; 
	}

	public void onConfigurationChanged(int rotation) {
	}
		
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub		
		if( getMainSlide() == null )
			buildSlideTree();
		
		super.onResize(width, height);
	}
		
		
	public void setFilter(int index){
		mIntensity = 0.0f;
		mBackSlide.setLightPower(mIntensity);
		
		if (index == 0){			
		} else if (index == 2){			
			mBackSlide.setFilterType(SVISlide.FilterType.FILTER_SKETCH);
		} else if (index == 3){
			mBackSlide.setFilterType(SVISlide.FilterType.FILTER_OILPAINT);
		} else if (index == 4){
			mBackSlide.setFilterType(SVISlide.FilterType.FILTER_WATERCOLOR);
		} else if (index == 5){
			
		}		
		requestRender();
	}
	
	public float getIntensity(){
		return mIntensity;
	}
	
	public void changeImage(){		
		mImageIdx++;
		mImageIdx = mImageIdx % 4;
		mBackSlide.setImage(mCoverImage[mImageIdx]);		
	}
		
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_MOVE){
			mIntensity += 0.1f;
			mBackSlide.setLightPower(mIntensity);
		}
				
		if (event.getAction() == MotionEvent.ACTION_UP){
			mParentSlide.setCapture(true);
			requestRender();
		}		
		return true;
	}
	
	
	protected void buildSlideTree() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };				
		SVISlide mainSlide = getMainSlide();		
		
		mParentSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);				
		mBackSlide = new SVISlide(0, mParentSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		mBackSlide.setImage(mCoverImage[3]);		
		mBackSlide.setLightPower(0.0f);
		mBackSlide.setFilterType(SVISlide.FilterType.FILTER_SKETCH);
		
		mParentSlide.setCapture(true);
	}
	
	protected float 			mIntensity = 0.0f;
	protected int				mImageIdx = 3;
	protected SVISlide			mBackSlide;	
	protected SVISlide			mParentSlide;	
}
