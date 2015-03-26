package com.github.sviengine.unittest;

import java.util.Random;
import java.util.Vector;

import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class PathFilterSlideWindow extends PlatformWindow{

	public PathFilterSlideWindow(Context context) {
		super(context);
		
		for (int k = 0; k < 1000; k++){
			float data = mPathWidth * 0.98f + mPathWidth * (float)mRandom.nextFloat() * 0.02f;
			mPathWidths.add(data);
		}
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
		
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		mImageIdx++;
		mImageIdx = mImageIdx % 4;
		mBottomSlide.setImage(mCoverImage[mImageIdx]);	
		mBackSlide.setImage(mCoverImage[mImageIdx]);	
		mPathSlide.setImage(mCoverImage[mImageIdx]);	
		
		mPathIndex = 0;
		mPathSlide.clearPathPoints();
		mIntensity = 0.0f;
		mBackSlide.setDeformRatio(1.0f,1.0f,0.0f);
		
		requestRender();
		return super.onOptionsItemSelected(item);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub			
		if( event.getAction() == MotionEvent.ACTION_DOWN){
			if (!mMode){
				mPathIndex = 0;
				mPathSlide.clearPathPoints();
				mIntensity = 0.0f;
				mBackSlide.setDeformRatio(1.0f,1.0f,0.0f);
			}
		}		
		if (event.getAction() == MotionEvent.ACTION_MOVE ){
			mPathSlide.addPathPoint(event.getX(), event.getY(), mPathWidths.get(mPathIndex) * mScale);
			mPathIndex++;
			mPathIndex = mPathIndex % 1000;
			mIntensity += 0.01f;
			if (mIntensity > 0.95f) mIntensity = 0.95f;
			mBackSlide.setDeformRatio(1.0f,1.0f,mIntensity);
		}
		float color[] = { 1.0f , 1.0f, 1.0f, 1.0f };
		mBackSlide.setBackgroundColor(color);
		return true;
	}
	
	
	protected void buildSlideTree() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				
		SVISlide mainSlide = getMainSlide();		
		
		
		mBottomSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		mBottomSlide.setImage(mCoverImage[mImageIdx]);
		
		mPathSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);				
		mPathSlide.setImage(mCoverImage[mImageIdx]);
		mPathSlide.setPivotPoint(0.0f,0.0f);
		
		float scolor[] = { 0.1f, 0.1f, 0.1f, 0.2f };
			
		mPathSlide.setDeformType(SVISlide.SlideDeformType.DEFORM_PATH);
		mPathSlide.setClipSubSlides(true);
		
		mBackSlide = new SVISlide(0, mPathSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		mBackSlide.setImage(mCoverImage[mImageIdx]);		
		mBackSlide.setDeformRatio(1.0f,1.0f, 0.0f);
		mBackSlide.setFilterType(SVISlide.FilterType.FILTER_SKETCH);
	}
	
	protected int				mImageIdx = 3;
	private float 				mScale = 1.0f;
	private boolean				mMode = false;
	private static int 			mPathIndex = 0;
	protected float 			mIntensity = 0.0f;
	protected float				mPathWidth = 230.0f;
	protected Random 			mRandom = new Random();
	protected Vector<Float>   	mPathWidths = new Vector<Float>();
	protected SVISlide			mPathSlide;
	protected SVISlide			mBackSlide;
	protected SVISlide			mBottomSlide;
	

}
