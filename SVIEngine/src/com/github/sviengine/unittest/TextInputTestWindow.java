package com.github.sviengine.unittest;

import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class TextInputTestWindow  extends PlatformWindow{

	public TextInputTestWindow(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		mDrawingTestBitmap = Bitmap.createBitmap(480, 800, Bitmap.Config.ARGB_8888);
		mDrawingTestImage = new SVIImage();
		mDrawingTestImage.setBitmap(mDrawingTestBitmap);
	}
	
	public void drawContent() {
		Canvas c = new Canvas(mDrawingTestBitmap);
		Paint p = new Paint();
		p.setTextSize(40.0f);
		p.setAntiAlias(true);
		p.setFilterBitmap(true);
		//int imgIdx = mIndex % mIcons.length;
		
		mDrawingTestBitmap.eraseColor(0x00000000);
		
		p.setColor(0xff00ff00);
		mDrawingTestImage.lock();
		
		p.setColor(0xffffffff);
		
		for(int i=0; i<mText.length; i++) {
			int textIdx = mIndex % mText[i].length();
			String temp = mText[i].substring(0, textIdx);
			c.drawText(temp, 0.0f, 45.0f*(i+1), p);
		}
		mDrawingTestImage.unlock();
		
		mSubSlide.setImage(mDrawingTestImage);
		requestRender();
		mIndex++;
	}
	
	public void rotateAnimation() {
		//SVIBasicAnimation rotateAni = new SVIBasicAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY, 0.0f, 1.0f);
		//rotateAni.setDuration(2000);		
		//mSubSlide.startAnimation(rotateAni);		
		mSubSlide.setOpacity(0.0f, 2000);
		//mSubSlide.setBackgroundColor(new SVIColor(0.0f,0.0f,0.0f,0.0f), 2000);
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		
		if( getMainSlide() == null )
			buildSubSlide();
		
		super.onResize(width, height);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub	
		if( event.getAction() == MotionEvent.ACTION_DOWN){
			if( mAnimationCount == 0 ) {
				Message sg = new Message();
				mHandler.sendMessage(sg);
			}
//			SVIColor from = new SVIColor(1.0f, 0.0f, 0.0f, 1.0f);
//			SVIColor to = new SVIColor(0.0f, 1.0f, 0.0f, 1.0f);
//			SVIBasicAnimation bg = new SVIBasicAnimation(PropertyAnimationType.BG_COLOR, from, to);
//			SVIAnimationSet aniSet = new SVIAnimationSet(2000, 1, false, 0);//, "aniSet1");
//			
//			aniSet.addAnimation(bg);
//			//aniSet.setListener(this);
//			mSubSlide.startAnimation(aniSet);
		}
		return true;
	}


	public void onConfigurationChanged(int rotation) {
	}
	
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
				
		SVISlide rootSlide = getMainSlide();
		
		SVISlide subSlide = new SVISlide(2, rootSlide.getNativeSlideHandle(), 0.0f, 0.0f, 800.0f, 800.0f, color);
		subSlide.setBlendType(SVISlide.BlendType.BLEND_ADD);
		subSlide.setImage(mDrawingTestImage);
		mSubSlide = subSlide;
	}
	
	private Handler mHandler = new Handler() {
    	public void handleMessage(Message msg) {
    		drawContent();
    		rotateAnimation();
    		//Message sg = new Message();
    		
    		//if( mAnimationCount < 100 ) {
    		//	mHandler.sendMessageDelayed(sg, 30);
    		//	mAnimationCount++;
    		//} else {
    		//	mAnimationCount = 0;
    		//}
    	}
    };
	
	private Bitmap		mDrawingTestBitmap = null;
	private SVIImage		mDrawingTestImage = null;
	//private Bitmap 		mAlphaBitmap = null;
	private int			mIndex = 5;
	private String		mText[] = { "SVI",
									"1234567890",
									"ABCDEFGHIJKLMNOPQRST",
									"UVWXYZ",
									"ganadaramabasa",
									"SVI",
									"mobile Graphic development",
									"SGI"
								   };
	private int			mAnimationCount = 0;
	
	private SVISlide		mSubSlide = null;
}
