package com.github.sviengine.unittest;

import java.io.InputStream;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideDeformDirection;
import com.github.sviengine.slide.SVISlide.SlideDeformType;

public class DeformationWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final String TAG = "DeformationWindow";

	private SVIImage mParticleImage = null;
	private Bitmap	mParticleBitmap = null;
	
	//variables for customize demo
	public static final int			BUTTON_ANIMATION_DURATION = 400; 
	public static final SVIColor 	BUTTON_ON_COLOR = new SVIColor(1.0f,0.5f,0.5f,1.0f);
	public static final float 		SHADOW_OFFSET = 2.5f;
  	public static final float 		SHADOW_RADIUS = 3.0f;
  	public static final float 		SHADOW_ALPHA = 0.4f;
	
	public DeformationWindow(Context context){
		super(context);
			
		
		InputStream is;
		Activity activity = (Activity)context;
		is = activity.getResources().openRawResource(R.drawable.particle);
		mParticleBitmap = BitmapFactory.decodeStream(is);
		
		mParticleImage = new SVIImage();
		mParticleImage.setBitmap(mParticleBitmap);
		
		buildSlideTree();			
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}

	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
					
		if( event.getAction() == MotionEvent.ACTION_DOWN){
			for (int n = 0; n < 12; n++){
				SVIRect Region = mDeformSlide[n].getRegion();
				if (Region.checkHit(new SVIPoint(event.getX(), event.getY()))){
					runAnimation(mDeformSlide[n]);
					break;
				}
			}	
		}else if( event.getAction() == MotionEvent.ACTION_MOVE){			
		}else if( event.getAction() == MotionEvent.ACTION_UP){
			
		}	
		return true;
	}
		
	
	
	public void runAnimation(SVISlide slide){
		int duration = BUTTON_ANIMATION_DURATION;
		int duration_min = BUTTON_ANIMATION_DURATION;
		
		SVIAnimationSet aniSet = new SVIAnimationSet();
		aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		aniSet.setDuration(duration);
		aniSet.setRepeatCount(0);
		aniSet.setAutoReverse(false);
		aniSet.setOffset(0);
				
		SVIKeyFrameAnimation keyDeformRatioAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.DEFORM_RATIO);
		keyDeformRatioAni.setDuration(duration_min);
		keyDeformRatioAni.setRepeatCount(0);
		keyDeformRatioAni.setAutoReverse(false);
		keyDeformRatioAni.setOffset(0);
		keyDeformRatioAni.addKeyProperty(0.0f, new SVIVector3(-0.2f,0.5f,mDepthCut)); //x ratio, radius, range
		keyDeformRatioAni.addKeyProperty(0.5f, new SVIVector3(2.0f,0.5f,mDepthCut)); //x ratio, radius, range
		keyDeformRatioAni.addKeyProperty(0.75f, new SVIVector3(0.0f,0.5f,mDepthCut)); //x ratio, radius, range
		keyDeformRatioAni.addKeyProperty(0.85f, new SVIVector3(-0.7f,0.5f,mDepthCut)); //x ratio, radius, range
		keyDeformRatioAni.addKeyProperty(1.0f, new SVIVector3(-0.2f,0.5f,mDepthCut)); //x ratio, radius, range
		aniSet.addAnimation(keyDeformRatioAni);			
		
		SVIKeyFrameAnimation keyShadowOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OPACITY);
		keyShadowOpacityAni.setDuration(duration);
		keyShadowOpacityAni.setRepeatCount(0);
		keyShadowOpacityAni.setAutoReverse(false);
		keyShadowOpacityAni.setOffset(0);
		keyShadowOpacityAni.addKeyProperty(0.0f, 0.55f);
		keyShadowOpacityAni.addKeyProperty(0.5f, 0.0f);
		keyShadowOpacityAni.addKeyProperty(0.75f, 0.0f);
		keyShadowOpacityAni.addKeyProperty(0.85f, 0.3f);
		keyShadowOpacityAni.addKeyProperty(1.0f, 0.55f);
		aniSet.addAnimation(keyShadowOpacityAni);
		
		SVIKeyFrameAnimation keyShadowRadiusAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_RADIUS);
		keyShadowRadiusAni.setDuration(duration);
		keyShadowRadiusAni.setRepeatCount(0);
		keyShadowRadiusAni.setAutoReverse(false);
		keyShadowRadiusAni.setOffset(0);
		keyShadowRadiusAni.addKeyProperty(0.0f, SHADOW_RADIUS);
		keyShadowRadiusAni.addKeyProperty(0.5f, 0.0f);
		keyShadowRadiusAni.addKeyProperty(0.75f, SHADOW_RADIUS * 0.1f);
		keyShadowRadiusAni.addKeyProperty(0.85f, SHADOW_RADIUS * 1.4f);
		keyShadowRadiusAni.addKeyProperty(1.0f, SHADOW_RADIUS);
		aniSet.addAnimation(keyShadowRadiusAni);
				
			
		if (mChangeRotation){
			SVIKeyFrameAnimation keyRotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
			keyRotationAni.setDuration(duration_min);
			keyRotationAni.setRepeatCount(0);
			keyRotationAni.setAutoReverse(false);
			keyRotationAni.setOffset(0);
			keyRotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range			 
			keyRotationAni.addKeyProperty(0.5f, new SVIVector3((mRandom.nextFloat() - 0.5f) * 50.0f,(mRandom.nextFloat() - 0.5f) * 50.0f, 0.0f)); //x ratio, radius, range
			keyRotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyRotationAni);					
		}
			
		/*
		SVIKeyFrameAnimation keyLightDirectionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_DIRECTION);
		keyLightDirectionAni.setDuration(duration_min);
		keyLightDirectionAni.setRepeatCount(0);
		keyLightDirectionAni.setAutoReverse(false);
		keyLightDirectionAni.setOffset(0);
		
		//keyLightDirectionAni.addKeyProperty(0.0f, new SVIVector4(mRandom.nextFloat(),mRandom.nextFloat(),mRandom.nextFloat(),0.7f)); //x ratio, radius, range			 
		//keyLightDirectionAni.addKeyProperty(0.5f, new SVIVector4(mRandom.nextFloat(),mRandom.nextFloat(),mRandom.nextFloat(),0.7f)); //x ratio, radius, range
		//keyLightDirectionAni.addKeyProperty(1.0f, new SVIVector4(mRandom.nextFloat(),mRandom.nextFloat(),mRandom.nextFloat(),1.0f)); //x ratio, radius, range
		
		keyLightDirectionAni.addKeyProperty(0.0f, new SVIVector4(0.0f,1.0f,1.0f,0.6f)); //x ratio, radius, range			 
		keyLightDirectionAni.addKeyProperty(0.5f, new SVIVector4(0.0f,0.0f,1.0f,0.6f)); //x ratio, radius, range
		keyLightDirectionAni.addKeyProperty(1.0f, new SVIVector4(0.0f,1.0f,1.0f,0.6f)); //x ratio, radius, range
		
		aniSet.addAnimation(keyLightDirectionAni);	
		*/
		
		if (mChangeOnColor){
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			keyColorAni.addKeyProperty(0.5f, BUTTON_ON_COLOR); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
		}
		
		SVIKeyFrameAnimation keyLightPowerAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_POWER);
		keyLightPowerAni.setDuration(duration_min);
		keyLightPowerAni.setRepeatCount(0);
		keyLightPowerAni.setAutoReverse(false);
		keyLightPowerAni.setOffset(0);
		keyLightPowerAni.addKeyProperty(0.0f, 0.4f); //x ratio, radius, range
		keyLightPowerAni.addKeyProperty(0.15f, 0.3f); //x ratio, radius, range
		keyLightPowerAni.addKeyProperty(0.55f, 0.4f); //x ratio, radius, range
		keyLightPowerAni.addKeyProperty(0.85f, 0.5f); //x ratio, radius, range
		keyLightPowerAni.addKeyProperty(1.0f, 0.4f); //x ratio, radius, range
					
		aniSet.addAnimation(keyLightPowerAni);					
								
		slide.setDeformRatio(0.0f, 0.5f, 0.0f);
		slide.setOpacity(1.0f);
		slide.startAnimation(aniSet);
	}
	
	private SVIVector3 mSensorAcc = new SVIVector3(0.0f,0.0f,1.0f);
	
	public void setSensorValue(float x, float y, float z){
		mSensorAcc.mX = mLightReversed ? -x : x;
		mSensorAcc.mY = mLightReversed ? -y : y;
		mSensorAcc.mZ = z;
		
		for (int n = 0; n < 12; n++){
			mDeformSlide[n].setLightDirection(new SVIVector4(mSensorAcc.mX,mSensorAcc.mY, mSensorAcc.mZ, 0.6f));
			mDeformSlide[n].setShadowOffset(mSensorAcc.mX * SHADOW_OFFSET, mSensorAcc.mY * SHADOW_OFFSET);
		}
	}
	
	protected void buildSlideTree(){
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float blink[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float [] color_shadow = {0.0f, 0.0f, 0.0f, SHADOW_ALPHA};
		
		SVISlide mainSlide = getMainSlide();		
		mainSlide.setImage(mImages[mImages.length-13]);
		SVISize mainsize = mainSlide.getRegion().mSize;						
		int n = 0;
		
		n = 0;
		for (int y = 0; y < 4; y++){
			for (int x = 0; x < 3; x++){
				float size = mainsize.mWidth / 4.1f; 
				mDeformSlide[n] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				(mainsize.mWidth - size * 3.0f - (size * 0.3f * 3.0f * 0.7f)) * 0.5f + x * (size * 1.3f), 
				mainsize.mHeight * 0.075f + (mainsize.mHeight - size * 4.0f - size * 4.0f * 0.1f) * 0.5f + y * (size * 1.1f),  
				size, 
				size, 
				color);
							
				
				mDeformSlide[n].setPivotPoint(0.5f, 0.5f);
				mDeformSlide[n].setImage(mImages[mImages.length-12+n]);
				//mDeformSlide[n].setBackFaceImage(mImages[mImages.length-n]);
				
				mDeformSlide[n].setDeformType(SlideDeformType.DEFORM_DOME);	
				mDeformSlide[n].setDeformDirection(SlideDeformDirection.DEFORM_VERTICAL);
				mDeformSlide[n].setDeformRatio(-0.2f,0.5f,0.6f);	//x ratio, radius, range	
				mDeformSlide[n].setOpacity(1.0f);						
				mDeformSlide[n].setLightPower(0.4f);
				mDeformSlide[n].setOrthogonal(false);			
				
				mDeformSlide[n].setShadowColor(color_shadow);
				mDeformSlide[n].setShadowRadius(SHADOW_RADIUS);
				mDeformSlide[n].setShadowOffset(0.0f, 0.0f);
				mDeformSlide[n].setShadowOpacity(0.55f);
				
				n++;
			}
		}
		
		mCoverSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, 1,  1, blink);
	}
	
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		if (item.getTitle() == "Change OnColor"){
			mChangeOnColor = !mChangeOnColor;
		} else if (item.getTitle() == "Change Light"){
			mLightReversed = !mLightReversed;
		} else if (item.getTitle() == "Change Cut"){
			if (mDepthCut == 0.6f) mDepthCut = 1.0f; else
				if (mDepthCut == 1.0f) mDepthCut = 0.6f; 
		} else if (item.getTitle() == "Change Form"){
			mChangeRotation = !mChangeRotation;
		}
		requestRender();
		return super.onOptionsItemSelected(item);
	}

	Random mRandom = new Random();
	float mDepthCut = 0.6f;
	boolean mLightReversed = false;
	boolean mChangeOnColor = true;
	boolean mChangeRotation = true;
	boolean mIsVertical = false;
	SVISlide [] mDeformSlide = new SVISlide[12];
	SVISlide [] mBackSlide = new SVISlide[12];
	SVISlide mCoverSlide = null; 
	
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
