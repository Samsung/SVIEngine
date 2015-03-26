package com.github.sviengine.unittest;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.RTEffectType;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class ScaleCaptureTestWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final String TAG = "ScaleCaptureTestWindow";
	
	public ScaleCaptureTestWindow(Context context){
		super(context);
			
		buildSlideTree();
		
		mMsgHandler = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);								
							
				//SVISlide mainSlide = getMainSlide();		
				//mSubSlides[0].setCapture(true); 
				mSubSlides[0].setCapture(true); 
				//Log.e("SVI", "Message Triggered");
				
				sendEmptyMessageDelayed(0, (int)(1000.0f/60.0f));		
			}
		};
		
		
		/// test code!!!
		//SVISceneNode.load3dModelFile(mContext, "box.bsa");
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}


	
	final protected float ICON_WIDTH		= 100.0f;
	final protected float ICON_HEIGHT		= 100.0f;
	
	
	protected SVISlide[]		mSubSlides;
	private boolean mScaleMode = true;
	

	@Override 
	public void onPause(){
		SVISlide mainSlide = getMainSlide();
		if (mainSlide != null){
			mainSlide.setCapture(false);
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
		keyScaleAni.addKeyProperty(0.3f, new SVIPoint(1.5f,1.5f));
		keyScaleAni.addKeyProperty(0.7f, new SVIPoint(0.5f,0.5f));
		keyScaleAni.addKeyProperty(1.0f, new SVIPoint(1.0f,1.0f));
		aniSet.addAnimation(keyScaleAni);
		
		SVIKeyFrameAnimation keyRotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
		keyRotationAni.setDuration(duration);
		keyRotationAni.setRepeatCount(1);
		keyRotationAni.setAutoReverse(false);
		keyRotationAni.setOffset(offset * count);
		keyRotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f));
		keyRotationAni.addKeyProperty(0.3f, new SVIVector3(0.0f,0.0f,90.0f));
		keyRotationAni.addKeyProperty(0.5f, new SVIVector3(0.0f,0.0f,0.0f));
		keyRotationAni.addKeyProperty(0.8f, new SVIVector3(0.0f,0.0f,90.0f));
		keyRotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f,0.0f));
		aniSet.addAnimation(keyRotationAni);
		
		
/*
		SVIKeyFrameAnimation keyLightOffsetAni = new 
			SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_OFFSET);
		keyLightOffsetAni.setDuration(duration);
		keyLightOffsetAni.setRepeatCount(1);
		keyLightOffsetAni.setAutoReverse(false);
		keyLightOffsetAni.setOffset(offset * count);
		keyLightOffsetAni.addKeyProperty(0.0f, new SVIPoint(0.0f,0.1f));
		keyLightOffsetAni.addKeyProperty(0.3f, new SVIPoint(0.2f,0.0f));
		keyLightOffsetAni.addKeyProperty(0.5f, new SVIPoint(0.5f,0.5f));
		keyLightOffsetAni.addKeyProperty(0.8f, new SVIPoint(0.9f,0.9f));
		keyLightOffsetAni.addKeyProperty(1.0f, new SVIPoint(0.5f,0.1f));
		aniSet.addAnimation(keyLightOffsetAni);
		
		SVIKeyFrameAnimation keyLightRadiusAni = new 
		SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_RADIUS);
		keyLightRadiusAni.setDuration(duration);
		keyLightRadiusAni.setRepeatCount(1);
		keyLightRadiusAni.setAutoReverse(false);
		keyLightRadiusAni.setOffset(offset * count);
		keyLightRadiusAni.addKeyProperty(0.0f, 0.2f);
		keyLightRadiusAni.addKeyProperty(0.5f, 1.0f);
		keyLightRadiusAni.addKeyProperty(1.0f, 0.52f);
		aniSet.addAnimation(keyLightRadiusAni);
		
		SVIKeyFrameAnimation keyLightAngleAni = new 
		SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_ANGLE);
		keyLightAngleAni.setDuration(duration);
		keyLightAngleAni.setRepeatCount(1);
		keyLightAngleAni.setAutoReverse(false);
		keyLightAngleAni.setOffset(offset * count);
		keyLightAngleAni.addKeyProperty(0.0f, 0.0f);
		keyLightAngleAni.addKeyProperty(0.3f, 90.0f);
		keyLightAngleAni.addKeyProperty(0.5f, 270.0f);
		keyLightAngleAni.addKeyProperty(0.8f, -90.0f);
		keyLightAngleAni.addKeyProperty(1.0f, 0.0f);
		aniSet.addAnimation(keyLightAngleAni);
				*/
		mSubSlides[0].startAnimation(aniSet);
		aniSet.setListener(this);
	}
	
	
	
	protected void buildSlideTree(){
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		
		SVISlide mainSlide = getMainSlide();		
		mainSlide.setBackgroundColor(color);
		SVISize mainsize = mainSlide.getRegion().mSize;		
		
		
		mainSlide.setImage(mImages[mImages.length-16]);		
		mSubSlides = new SVISlide[5];
				
		color = new float[]{ 1.0f, 0.3f, 0.3f, 1.0f };
		float offsetx = mainsize.mWidth * 0.25f;
		float offsety = mainsize.mHeight * 0.25f;
		float sizex = mainsize.mWidth*0.5f;
		float sizey = mainsize.mHeight*0.5f;
		mSubSlides[0] = new SVISlide(0, mainSlide.getNativeSlideHandle(), offsetx, offsety, sizex, sizey, color);
		mSubSlides[0].setCornerRadius(50.0f);
		
		//mSubSlides[0].setImageScaleType(ImageScaleType.MATRIX);
		//mSubSlides[0].setLightType(LightType.LINEAR_LIGHT);
		
		offsetx = sizex * 0.125f;
		offsety = sizey * 0.125f;
		color = new float[]{ 0.3f, 1.0f, 0.3f, 1.0f };
		mSubSlides[1] = new SVISlide(0, mSubSlides[0].getNativeSlideHandle(), 
				offsetx, offsety,  sizex*0.25f, sizey*0.25f, color);
		
		
		
		offsetx = sizex * 0.5f + sizex*0.125f;
		offsety = sizey * 0.125f;
		mSubSlides[2] = new SVISlide(0, mSubSlides[0].getNativeSlideHandle(), 
				offsetx, offsety,  sizex*0.25f, sizey*0.25f, color);
		
		offsetx = sizex * 0.125f;
		offsety = sizey * 0.5f + sizey*0.125f;
		color = new float[]{ 0.3f, 0.3f, 1.0f, 1.0f };
		mSubSlides[3] = new SVISlide(0, mSubSlides[0].getNativeSlideHandle(), 
				offsetx, offsety,  sizex*0.25f, sizey*0.25f, color);
		
		offsetx = sizex * 0.5f + sizex*0.125f;
		offsety = sizey * 0.5f + sizey*0.125f;
		mSubSlides[4] = new SVISlide(0, mSubSlides[0].getNativeSlideHandle(), 
				offsetx, offsety,  sizex*0.25f, sizey*0.25f, color);

		
		for (int n = 0; n < 5; n++){
			mSubSlides[n].setImage(mImages[mImages.length-17 + n]);
		}
	}
	
	private int mCurrentRTEffect = RTEffectType.RT_NONE;
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		SVISlide mainSlide = getMainSlide();		
		 if (item.getTitle() == "Change RenderTarget Effect"){
			 mCurrentRTEffect++;
			 mCurrentRTEffect = mCurrentRTEffect % 10;
			 mainSlide.setRTEffectType(mCurrentRTEffect);				
		}else if (item.getTitle() == "Change Scale Mode"){
			mScaleMode = !mScaleMode;			
			for (int n = 1; n < 5; n++){
				mSubSlides[n].setHoldScale(mScaleMode);
			}
		}else if (item.getTitle() == "Start Capture"){			
			if (mMsgHandler != null){
				mMsgHandler.sendEmptyMessage(0);
			}		
		}else if (item.getTitle() == "Stop Capture"){
			mSubSlides[0].setCapture(false);
			if (mMsgHandler != null){
				mMsgHandler.removeMessages(0);							
			}			
		}
					
		requestRender();
		return super.onOptionsItemSelected(item);
	}

	
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
