package com.github.sviengine.unittest;

import java.util.Random;
import java.util.Vector;

import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class PathSlideWindow extends PlatformWindow{

	public PathSlideWindow(Context context) {
		super(context);
		
		for (int k = 0; k < 1000; k++){
			float data = mPathWidth * 0.88f + mPathWidth * (float)mRandom.nextFloat() * 0.12f;
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
		if (item.getTitle() == "HOME"){
			mBackSlide.setImage(mCoverImage[0]);
			mPathSlide.setImage(mCoverImage[2]);
		} else if (item.getTitle() == "PAPER"){
			mBackSlide.setImage(mCoverImage[1]);
			mPathSlide.setImage(mCoverImage[0]);
		}
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
			}
		}		
		if (event.getAction() == MotionEvent.ACTION_MOVE ){
			mPathSlide.addPathPoint(event.getX(), event.getY(), mPathWidths.get(mPathIndex) * mScale);
			mPathIndex++;
			mPathIndex = mPathIndex % 1000;
		}
		float color[] = { 1.0f , 1.0f, 1.0f, 1.0f };
		mBackSlide.setBackgroundColor(color);
		return true;
	}
	
	@Override 
	public boolean onKeyDown(int keyCode, KeyEvent event){
		
		if (event.getAction() == KeyEvent.KEYCODE_VOLUME_UP){
			mScale += 0.1f;
		} else if (event.getAction() == KeyEvent.KEYCODE_VOLUME_DOWN){
			mScale -= 0.1f;
		}
		float color[] = { 1.0f , 1.0f, 1.0f, 1.0f };
		mBackSlide.setBackgroundColor(color);
		return true;		
	}

	protected void buildSlideTree() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				
		SVISlide mainSlide = getMainSlide();		
		
		mBackSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		mPathSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
				
		mBackSlide.setImage(mCoverImage[1]);
		mPathSlide.setImage(mCoverImage[0]);
		mPathSlide.setPivotPoint(0.0f,0.0f);
		mPathSlide.setShadowOffset(0.0f, 0.0f);
		mPathSlide.setShadowRadius(5.0f);
		mPathSlide.setShadowOpacity(0.2f);
		
		float scolor[] = { 0.1f, 0.1f, 0.1f, 0.2f };
		
		mPathSlide.setShadowColor(scolor);
				
		mPathSlide.setDeformType(SVISlide.SlideDeformType.DEFORM_PATH);	
	}
	
	private float 				mScale = 1.0f;
	private boolean				mMode = false;
	private static int 			mPathIndex = 0;
	protected float				mPathWidth = 400.0f;
	protected Random 			mRandom = new Random();
	protected Vector<Float>   	mPathWidths = new Vector<Float>();
	protected SVISlide			mPathSlide;
	protected SVISlide			mBackSlide;
	

}
