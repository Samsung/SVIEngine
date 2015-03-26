package com.github.sviengine.unittest;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.RTEffectType;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class CapturedPostEffectWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final String TAG = "CapturedPostEffectWindow";
	
	public CapturedPostEffectWindow(Context context){
		super(context);
			
		buildSlideTree();
		
		mMsgHandler = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);								
							
				//for (int n = 0; n < 4; n++) mQuaterSlide[n].setCapture(true); 
				//Log.e("SVI", "Message Triggered");
				
				sendEmptyMessageDelayed(0, (int)(1000.0f/60.0f));		
				//requestRender();
			}
		};
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}

	
	@Override 
	public void onPause(){
		SVISlide mainSlide = getMainSlide();
		if (mainSlide != null){
			for (int n = 0; n < 4; n++) mQuaterSlide[n].setCapture(false); 
			
			if (mMsgHandler != null){
				mMsgHandler.removeMessages(0);							
			}	
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub	
		if( event.getAction() == MotionEvent.ACTION_DOWN){
			runTestAnimation();
		}
		
		return true;
	}
	
	public void runTestAnimation(){
		int count = 0;
		int offset = 0;
		int duration = 5000;
		
		SVIAnimationSet aniSet = new SVIAnimationSet();
		aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		aniSet.setDuration(duration);
		aniSet.setRepeatCount(1);
		aniSet.setAutoReverse(false);
	
		SVIKeyFrameAnimation keyScaleAni = new 
		SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
		keyScaleAni.setDuration(duration);
		keyScaleAni.setRepeatCount(1);
		keyScaleAni.setAutoReverse(false);
		keyScaleAni.setOffset(offset * count);
		keyScaleAni.addKeyProperty(0.0f, new SVIPoint(1.0f,1.0f));
		keyScaleAni.addKeyProperty(0.5f, new SVIPoint(1.0f,1.0f));
		keyScaleAni.addKeyProperty(1.0f, new SVIPoint(1.0f,1.0f));
		aniSet.addAnimation(keyScaleAni);
		/*
		SVIKeyFrameAnimation keyOpacityAni = new 
		SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
		keyOpacityAni.setDuration(duration);
		keyOpacityAni.setRepeatCount(1);
		keyOpacityAni.setAutoReverse(false);
		keyOpacityAni.setOffset(offset * count);
		keyOpacityAni.addKeyProperty(0.0f, 1.0f);
		keyOpacityAni.addKeyProperty(0.5f, 0.0f);
		keyOpacityAni.addKeyProperty(1.0f, 1.0f);
		aniSet.addAnimation(keyOpacityAni);
		*/
		
		for (int n = 0; n < 4; n++) mQuaterSlide[n].startAnimation(aniSet);
		
		aniSet.setListener(this);
	}
	
	
	
	protected void buildSlideTree(){
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//float yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		
		SVISlide mainSlide = getMainSlide();		
		//SVISlide rootSlide = getRootSlide();
		mainSlide.setBackgroundColor(color);
		SVISize mainsize = mainSlide.getRegion().mSize;			
		
		
		mQuaterSlide[0] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				0.0f, 0.0f,  mainsize.mWidth*0.5f, mainsize.mHeight*0.5f, color);
		mQuaterSlide[1] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				mainsize.mWidth*0.5f, mainsize.mHeight*0.5f,  mainsize.mWidth*0.5f, mainsize.mHeight*0.5f, color);
		mQuaterSlide[2] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				mainsize.mWidth*0.5f, 0.0f,  mainsize.mWidth*0.5f, mainsize.mHeight*0.5f, color);
		mQuaterSlide[3] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				0.0f, mainsize.mHeight*0.5f,  mainsize.mWidth*0.5f, mainsize.mHeight*0.5f, color);
		
		mQuaterSlide[0].setImage(mImages[mImages.length-4]);
		mQuaterSlide[1].setImage(mImages[mImages.length-3]);
		mQuaterSlide[2].setImage(mImages[mImages.length-2]);
		mQuaterSlide[3].setImage(mImages[mImages.length-1]);		
	}
	
	private int mCurrentRTEffect = RTEffectType.RT_NONE;
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		//SVISlide mainSlide = getMainSlide();		
		 if (item.getTitle() == "Change Effect"){
			 for (int n = 0; n < 4; n++){
				 mCurrentRTEffect++;
				 mCurrentRTEffect = mCurrentRTEffect % 10;
				 mQuaterSlide[n].setRTEffectType(mCurrentRTEffect);
			 }		 
			 
		}else if (item.getTitle() == "Start Capture"){			
			if (mMsgHandler != null){
				mMsgHandler.sendEmptyMessage(0);
				for (int n = 0; n < 4; n++) mQuaterSlide[n].setCapture(true); 
			}		
		}else if (item.getTitle() == "Stop Capture"){
			for (int n = 0; n < 4; n++){
				mQuaterSlide[n].setCapture(false);
			}		
			if (mMsgHandler != null){
				mMsgHandler.removeMessages(0);							
			}			
		}
					
		requestRender();
		return super.onOptionsItemSelected(item);
	}

	SVISlide [] mQuaterSlide = new SVISlide[4];
	Handler mMsgHandler; 
	
	@Override
	public void onAnimationEnd(String tag) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
		
	}
}
