package com.github.sviengine.unittest;

import java.io.InputStream;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideSwipeType;

public class ShaderEffectTestWindow extends PlatformWindow{

	public ShaderEffectTestWindow(Context context) {
		super(context, false);
		Log.i("SVI", "ShaderEffectTestWindow(Context context)");
		Activity activity = (Activity)context;
		InputStream is;
		
		is = activity.getResources().openRawResource(R.drawable.art_1);
		mFrontBitmap = BitmapFactory.decodeStream(is);
		mFrontImage = new SVIImage();
		mFrontImage.setBitmap(mFrontBitmap);
		
		is = activity.getResources().openRawResource(R.drawable.art_3);
		mBackBitmap = BitmapFactory.decodeStream(is);
		mBackImage = new SVIImage();
		mBackImage.setBitmap(mBackBitmap);
						
		if( getMainSlide() == null )
			buildSubSlide();
				
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		super.onResize(width, height);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		if( event.getAction() == MotionEvent.ACTION_UP ) {
			runRotationSlide(mLinearSlide);
		}
		
		return true;
	}
	
	
	private boolean mModeSelected = false;
	private boolean mDirectionSelected = false;
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
			
		
		if (item.getTitle() == "Change Swipe Mode"){
			mModeSelected = !mModeSelected;
			
			if (mModeSelected){
				mLinearSlide.setSwipeType(SlideSwipeType.SWIPE_RADIAL);
			}else{
				mLinearSlide.setSwipeType(SlideSwipeType.SWIPE_LINEAR);
			}
		}else if (item.getTitle() == "Change Swipe Direction"){
			mDirectionSelected = !mDirectionSelected;
		}
		
		return true;
	}
	
	private Random mRandom = new Random();
	
	protected void runRotationSlide(SVISlide slide){
				
		SVIAnimationSet slideAniSet = new SVIAnimationSet();
		slideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.SINEEASEINOUT);
		slideAniSet.setDuration(2000);
		slideAniSet.setOffset(0);
		slideAniSet.setRepeatCount(0);
		slideAniSet.setAutoReverse(false);
				
		SVIKeyFrameAnimation swipeAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SWIPE_RATIO);
		swipeAni.setDuration(2000);
		swipeAni.setRepeatCount(0);
		swipeAni.setOffset(0);
		swipeAni.setAutoReverse(false);
		swipeAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f, mDirectionSelected ? 1.0f : 0.0f));
		swipeAni.addKeyProperty(0.5f, new SVIVector3(1.0f,0.3f, mDirectionSelected ? 1.0f : 0.0f));
		swipeAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f, mDirectionSelected ? 1.0f : 0.0f));
		
		slideAniSet.addAnimation(swipeAni);			
		slide.startAnimation(slideAniSet);							
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 0.95f, 0.95f, 0.95f, 1.0f };
		
		SVISlide mainSlide = getMainSlide();
		SVISize mainSize = mainSlide.getRegion().mSize;			
		
		mainSlide.setBackgroundColor(color);
		mainSlide.setHoldOpacity(true);
		mainSlide.setOrthogonal(true);	
			
		
		mBackSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				mainSize.mWidth * 0.1f, mainSize.mHeight * 0.1f,  
				mainSize.mWidth * 0.8f, mainSize.mHeight * 0.8f, color);
		mBackSlide.setImage(mBackImage);
		
		mLinearSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
					mainSize.mWidth * 0.1f, mainSize.mHeight * 0.1f,  
					mainSize.mWidth * 0.8f, mainSize.mHeight * 0.8f, color);
			
		
		mLinearSlide.setImage(mFrontImage);
		mLinearSlide.setSwipeType(SlideSwipeType.SWIPE_LINEAR);
		
		//mLinearSlide.setOpacity(0.8f);
		//mRadialSlide.setOpacity(0.8f);
			
	}
		
	private SVISlide mLinearSlide = null;
	private SVISlide mBackSlide = null;
	private SVIImage mFrontImage = null;
	private Bitmap	mFrontBitmap = null;
	

	private SVIImage mBackImage = null;
	private Bitmap	mBackBitmap = null;
}
