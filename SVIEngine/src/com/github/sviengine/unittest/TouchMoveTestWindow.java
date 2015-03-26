package com.github.sviengine.unittest;

import java.util.Random;

import com.github.sviengine.animation.SVIBasicAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation.PropertyAnimationType;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class TouchMoveTestWindow extends PlatformWindow{

	public TouchMoveTestWindow(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		
		mMsgHandler = new Handler() {

			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				
				if( mAutoX <= 480 ) {
					mAutoX += 10.0f;
					mAutoY = 400.0f;
					touchDraggingAnimation(mAutoX, mAutoY);
					requestRender();
					mMsgHandler.sendEmptyMessageDelayed(0, 16);
				} else {
					mAutoX = 0.0f;
					mAutoY = 400.0f;
				}
			}
		};
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
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
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub	
		
		if( event.getAction() == MotionEvent.ACTION_DOWN || 
			event.getAction() == MotionEvent.ACTION_MOVE )
			touchDraggingAnimation(event.getX(), event.getY());
		
		/*
		if( event.getAction() == MotionEvent.ACTION_DOWN ) {
			mMsgHandler.sendEmptyMessage(0);
		}
		*/
		return true;
	}
	
	protected void touchDraggingAnimation(float x, float y) {
		SVISlide mainSlide = getMainSlide();
		
		//Log.i("SVI", "x:"+x+", y:"+y);
		boolean useImplicit = true;
		
		if( useImplicit ) {
			mainSlide.setPosition(x, y);
			requestRender();
		}
		else {
		
			SVIPoint from = new SVIPoint(0.0f, 0.0f);
			SVIPoint to = new SVIPoint(x, y);
			SVIBasicAnimation moveAni = new SVIBasicAnimation(PropertyAnimationType.POSITION, from, to);
			mainSlide.startAnimation(moveAni);
		
		}
		
	}
	
	protected void buildSlideTree() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				
		SVISlide mainSlide = getMainSlide();		
		mainSlide.setImage(mImages[mImages.length-1]);
		mainSlide.setBackgroundColor(color);
		//mainSlide.setClipSubSlides(true);
		mainSlide.setRegion(0.0f, 0.0f, 600.0f, 600.0f);
		
		mIconSlide = new SVISlide[LANDSCAPE_COLUMN_COUNT*LANDSCAPE_ROW_COUNT];
		
		
		int imageCnt = mImages.length-1;
		int rowCount = PORTRAIT_ROW_COUNT;
		int columnCount = PORTRAIT_COLUMN_COUNT;
		int count = 0;

		if( mWidth > mHeight ) {
			columnCount = LANDSCAPE_COLUMN_COUNT;
			rowCount = LANDSCAPE_ROW_COUNT;
		}

		mCurrentRowCount = rowCount;
		mCurrentColumnCount = columnCount;

		float x, y;
		float width = mWidth / columnCount;
		float height = mHeight / rowCount;

		mLeftMargin = (width - ICON_WIDTH) / 2.0f;
		mRightMargin = mLeftMargin;
		mTopMargin = (height - ICON_HEIGHT) / 2.0f;
		mBottomMargin = mTopMargin;
		int  slideCount = rowCount * columnCount;
		for(int i=0; i<slideCount; i++) {
			int imgIdx = count % (imageCnt);
			x = (count%columnCount) * width;
			y = (count/columnCount) * height;
			
			mIconSlide[i] = new SVISlide(0, 0, 
					(x+mLeftMargin), (y+mTopMargin), ICON_WIDTH, ICON_HEIGHT, color);
			mainSlide.addSlide(mIconSlide[i]);
			mIconSlide[i].setImage(mImages[imgIdx]);			
			count++;
		}	
	
	}
	
	final protected int PORTRAIT_COLUMN_COUNT = 5;
	final protected int PORTRAIT_ROW_COUNT = 5;

	final protected float ICON_WIDTH		= 60.0f;
	final protected float ICON_HEIGHT		= 60.0f;
	
	final protected int LANDSCAPE_COLUMN_COUNT	= PORTRAIT_ROW_COUNT;
	final protected int LANDSCAPE_ROW_COUNT		= PORTRAIT_COLUMN_COUNT;
	
	Random mRandom = new Random();
	int mCurrentRowCount = 0;
	int mCurrentColumnCount = 0;
	float mLeftMargin = 0.0f;
	float mRightMargin = 0.0f;
	float mTopMargin = 0.0f;
	float mBottomMargin = 0.0f;
	
	
	protected SVISlide[]		mIconSlide;
	
	private Handler			mMsgHandler;
	private float			mAutoX;
	private float			mAutoY;

}
