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

public class NonNormalMapWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final float SENSOR_SCALE = 0.995f;
	public static final int BUTTON_ANIMATION_DURATION = 500;
	public static final SVIColor 	BUTTON_ON_COLOR = new SVIColor(1.0f,0.6f,0.55f,1.0f);
	public static final String TAG = "NonNormalMapWindow";

	private SVIImage [] mButtonImage = new SVIImage [15];
	private Bitmap	[] mButtonBitmap = new Bitmap [15];
	
	private SVIImage mNormalMapImage = null;
	private Bitmap	mNormalMapBitmap = null;	
		
	private SVIImage mBGImage = null;
	private Bitmap	mBGBitmap = null;	
	
	public NonNormalMapWindow(Context context){
		super(context);
		//this.enableShowFPS(false);
		
		InputStream is;
		Activity activity = (Activity)context;
		
		int demoImage[] = { 				
				R.drawable.btn_1,
				R.drawable.btn_2,
				R.drawable.btn_3,
				R.drawable.btn_4,
				R.drawable.btn_5,
				R.drawable.btn_6,
				R.drawable.btn_7,
				R.drawable.btn_8,
				R.drawable.btn_9,
				R.drawable.btn_star,
				R.drawable.btn_0,
				R.drawable.btn_sharp,
				R.drawable.btn_vcall,
				R.drawable.btn_call,
				R.drawable.btn_back
				};
		
		for (int n = 0; n < 15; n++){
			is = activity.getResources().openRawResource(demoImage[n]);
			mButtonBitmap[n] = BitmapFactory.decodeStream(is);
			mButtonImage[n] = new SVIImage();
			mButtonImage[n].setBitmap(mButtonBitmap[n]);	
		}						
				
		is = activity.getResources().openRawResource(R.drawable.normal_test);
		mNormalMapBitmap = BitmapFactory.decodeStream(is);
		mNormalMapImage = new SVIImage();
		mNormalMapImage.setBitmap(mNormalMapBitmap);
		
		is = activity.getResources().openRawResource(R.drawable.bg_background);
		mBGBitmap = BitmapFactory.decodeStream(is);
		mBGImage = new SVIImage();
		mBGImage.setBitmap(mBGBitmap);
		
		
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
			for (int n = 0; n < 15; n++){
				SVIRect Region = mButtonSlide[n].getRegion();
				if (Region.checkHit(new SVIPoint(event.getX(), event.getY()))){
					runAnimation(mButtonSlide[n]);
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
		
		if (mChangeRotation){
			SVIKeyFrameAnimation keyRotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
			keyRotationAni.setDuration(duration_min);
			keyRotationAni.setRepeatCount(0);
			keyRotationAni.setAutoReverse(false);
			keyRotationAni.setOffset(0);
			keyRotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range			 
			keyRotationAni.addKeyProperty(0.5f, new SVIVector3((mRandom.nextFloat() - 0.5f) * 80.0f,(mRandom.nextFloat() - 0.5f) * 1.0f, 0.0f)); //x ratio, radius, range
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
		keyLightPowerAni.addKeyProperty(0.0f, 1.0f); //x ratio, radius, range
		keyLightPowerAni.addKeyProperty(0.5f, -1.0f); //x ratio, radius, range
		keyLightPowerAni.addKeyProperty(1.0f, 1.0f); //x ratio, radius, range
					
		aniSet.addAnimation(keyLightPowerAni);			
								
		slide.setOpacity(1.0f);
		slide.startAnimation(aniSet);
	}
	
	private SVIVector3 mSensorAcc = new SVIVector3(0.0f,0.0f,1.0f);
	private SVIVector3 mSensorGrv = new SVIVector3(0.0f,0.0f,1.0f);
	
	public void setSensorValueAcc(float x, float y, float z){				
		mSensorAcc.mX = x;
		mSensorAcc.mY = y;	
		mSensorAcc.mZ = z;
		
		for (int n = 0; n < 15; n++){				
			mButtonSlide[n].setViewDirection(
					new SVIVector4(mSensorAcc.mX,mSensorAcc.mY,mSensorAcc.mZ, 0.6f));
		}		
	}
	
	public void setSensorValueGrv(float x, float y, float z){
		
		if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
		if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;
		
		mSensorGrv.mX = mLightReversed ? -x : x;
		mSensorGrv.mY = y;
		mSensorGrv.mZ = z;		
		
		for (int n = 0; n < 15; n++){
			mButtonSlide[n].setLightDirection(new SVIVector4(
					mSensorGrv.mX,
					mSensorGrv.mY, 
					mSensorGrv.mZ, 1.6f));			
		}		
	}
	
	protected void buildSlideTree(){
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_shadow[] = { 0.3f, 0.3f, 0.3f, 0.6f };
				
		SVISlide mainSlide = getMainSlide();		
		mainSlide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,1.0f));
		
		mainSlide.setImage(mBGImage);
		SVISize mainsize = mainSlide.getRegion().mSize;						
		int n = 0;
		
		n = 0;
		for (int y = 0; y < 5; y++){
			for (int x = 0; x < 3; x++){
				float size = mainsize.mWidth / 3.0f; 
				float sizeHeight = size * 0.57f;
				
				mButtonSlide[n] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				(mainsize.mWidth - size * 3.0f) * 0.5f + x * size, 
				mainsize.mHeight - (sizeHeight * 5.0f) + y * sizeHeight,  
				size, 
				sizeHeight, 
				color);
				
				//mButtonSlide[n].setUseNormalMap(true);
				
				mButtonSlide[n].setPivotPoint(0.5f, 0.5f);
				mButtonSlide[n].setImage(mButtonImage[n]);
				//mButtonSlide[n].setBackFaceImage(mNormalMapImage);
				
				mButtonSlide[n].setOpacity(1.0f);						
				mButtonSlide[n].setLightPower(1.0f);
				mButtonSlide[n].setOrthogonal(false);			
				
				mButtonSlide[n].setViewDirection(
						new SVIVector4(0.0f,-0.5f,1.0f, 1.0f));
				
				mButtonSlide[n].setLightDirection(new SVIVector4(
						0,
						1, 
						0, 1.0f));			
				n++;
			}
		}
		
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
	boolean mChangeRotation = false;
	boolean mIsVertical = false;
	SVISlide [] mButtonSlide = new SVISlide[15];
	
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
