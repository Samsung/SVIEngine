package com.github.sviengine.unittest;

import java.io.InputStream;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIBasicAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation.PropertyAnimationType;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;

public class ScreenRotationTestWindow  extends PlatformWindow implements SVIAnimationListener, SensorEventListener{

	public ScreenRotationTestWindow(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		Activity activity = (Activity)context;
		InputStream is;
		
		is = activity.getResources().openRawResource(R.drawable.hc_portrait);
		mPortraitBitmap= BitmapFactory.decodeStream(is);
		mPortraitImage = new SVIImage();
		mPortraitImage.setBitmap(mPortraitBitmap);
		
		is = activity.getResources().openRawResource(R.drawable.hc_landscape);
		mLandscapeBitmap= BitmapFactory.decodeStream(is);
		mLandscapeImage = new SVIImage();
		mLandscapeImage.setBitmap(mLandscapeBitmap);
		
		if( mFixSurface ) {
			mSensorManager = (SensorManager)activity.getSystemService(Context.SENSOR_SERVICE);  
	        mOrientationSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION); 
	        mSensorManager.registerListener(this, mOrientationSensor, SensorManager.SENSOR_DELAY_FASTEST);
		}
		
		mOrgOrientation = activity.getRequestedOrientation();
		
		mActivity = activity;
		
		if( mFixSurface ) {
			activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		
		//marx.kim begin
		mOrgOrientation = mActivity.getRequestedOrientation();
		//marx.kim end
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
		
		mBaseWidth = w;
		mBaseHeight = h;
	}


	public void onConfigurationChanged(int rotation) {
		super.onConfigurationChanged(rotation);
		Log.i("Orientation", "rotation:"+rotation);
	}
	
	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		/*
		if( event.getAction() == MotionEvent.ACTION_DOWN ) {
			mWidth = 800;
			mHeight = 457;
			
			mPrevWidth = 480;
			mPrevHeight = 777;
			
			mRotation = ROTATION_90;
			mPrevRotation = ROTATION_0;
			
			runRotateAnimation(mRotation);
		}
		*/
		return true;
	}
	
	
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		if( getMainSlide() == null )
			buildSubSlide();
		
		mPrevWidth = mWidth;
		mPrevHeight = mHeight;
		
		mWidth = width;
		mHeight = height;
		
		Log.i("Orientation", "w:"+width+", h:"+height);
		//buildSubSlide();
		if( !mIsAnimating ) {
			//marx.kim begin
			int sdk_version = android.os.Build.VERSION.SDK_INT;
			if( sdk_version >= 9 ) {
				int requestedOrientation = getCurrentOrientation();
				mActivity.setRequestedOrientation(requestedOrientation);
			}
			//marx.kim end
			
			Log.i("SVI", "mRotation:"+mRotation);
			runRotateAnimation(mRotation);
		}
		
		super.onResize(width, height);
	}

	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
	
		SVISlide rootSlide = getMainSlide();
		
		if( mPortraitSlide == null ) {
			float color[] = {1.0f, 1.0f, 1.0f, 1.0f };
			if( mWidth < mHeight ) {
				mPortraitSlide = new SVISlide(0, 0, 0.0f, 0.0f, (float)mWidth, (float)mHeight, color);			
				mLandscapeSlide = new SVISlide(0, 0, 0.0f, 0.0f, (float)mWidth, (float)mHeight, color);
			} else {
				mLandscapeSlide = new SVISlide(0, 0, 0.0f, 0.0f, (float)mWidth, (float)mHeight, color);
				mPortraitSlide = new SVISlide(0, 0, 0.0f, 0.0f, (float)mWidth, (float)mHeight, color);
			}
	
			mPortraitSlide.setImage(mPortraitImage);	
			mLandscapeSlide.setImage(mLandscapeImage);
			
			rootSlide.addSlide(mPortraitSlide);
			rootSlide.addSlide(mLandscapeSlide);
		}
	}
	
	private void runRotateAnimation(int rotation) {
		int duration = 600;
	
		SVIAnimationSet aniSetFrom = new SVIAnimationSet(duration, repeatCount, false, 0);
		SVIAnimationSet aniSetTo = new SVIAnimationSet(duration, repeatCount, false, 0);		
		
		if( !mFixSurface ) {
			float fromAngle = getFromAngle(mPrevRotation, mRotation);
			SVIVector3 fromRotate = new SVIVector3(0.0f, 0.0f, fromAngle);
			SVIVector3 toRotate = new SVIVector3(0.0f, 0.0f, 0.0f);
			SVIBasicAnimation rotateAni = new SVIBasicAnimation(PropertyAnimationType.ROTATION, fromRotate, toRotate);
			aniSetFrom.addAnimation(rotateAni);
			aniSetTo.addAnimation(rotateAni);
			
			float xOffset = (mWidth - mPrevWidth) / 2.0f;
			float yOffset = (mHeight - mPrevHeight) / 2.0f;
			SVIRect fromRect = new SVIRect(xOffset, yOffset, mPrevWidth, mPrevHeight);
			SVIRect toRect = new SVIRect(0, 0, mWidth, mHeight);
			SVIBasicAnimation regionAni =  new SVIBasicAnimation(PropertyAnimationType.REGION, fromRect, toRect);
			aniSetFrom.addAnimation(regionAni);
			aniSetTo.addAnimation(regionAni);
		} else {
			float fromAngle = getAngleFromRotation(mPrevRotation);
			float toAngle = getAngleFromRotation(mRotation);
			Log.i("Orientation", fromAngle + " => " + toAngle);
			SVIVector3 fromRotate = new SVIVector3(0.0f, 0.0f, fromAngle);
			SVIVector3 toRotate = new SVIVector3(0.0f, 0.0f, toAngle);
			SVIBasicAnimation rotateAni = new SVIBasicAnimation(PropertyAnimationType.ROTATION, fromRotate, toRotate);
			aniSetFrom.addAnimation(rotateAni);
			aniSetTo.addAnimation(rotateAni);
			
			SVIRect fromRect = null;
			SVIRect toRect = null;
			
			if( mBaseWidth == mWidth ) {
				float xOffset = (mBaseWidth - mPrevWidth) / 2.0f;
				float yOffset = (mBaseHeight - mPrevHeight) / 2.0f;
				
				fromRect = new SVIRect(xOffset, yOffset, mPrevWidth, mPrevHeight);
				toRect = new SVIRect(0.0f, 0.0f, mBaseWidth, mBaseHeight);
			} else {
				float xOffset = (mBaseWidth - mWidth) / 2.0f;
				float yOffset = (mBaseHeight - mHeight) / 2.0f;
				
				fromRect = new SVIRect(0, 0, mBaseWidth, mBaseHeight);
				toRect = new SVIRect(xOffset, yOffset, mWidth, mHeight);
			}
			
			Log.i("Orientation", "fromRect :"+fromRect.mOrigin.mX+", "+fromRect.mOrigin.mY+", "+fromRect.mSize.mWidth+", "+fromRect.mSize.mHeight);
			Log.i("Orientation", "toRect :"+toRect.mOrigin.mX+", "+toRect.mOrigin.mY+", "+toRect.mSize.mWidth+", "+toRect.mSize.mHeight);
			
			SVIBasicAnimation regionAni =  new SVIBasicAnimation(PropertyAnimationType.REGION, fromRect, toRect);
			aniSetFrom.addAnimation(regionAni);
			aniSetTo.addAnimation(regionAni);
		}
		
		
			
		SVIBasicAnimation opacityAni1 = new SVIBasicAnimation(PropertyAnimationType.OPACITY, 1.0f, 0.0f);
		aniSetFrom.addAnimation(opacityAni1);
		SVIBasicAnimation opacityAni2 = new SVIBasicAnimation(PropertyAnimationType.OPACITY, 0.0f, 1.0f);
		aniSetTo.addAnimation(opacityAni2);
		
		/*
		SVIBasicAnimation cornerAni1 = new SVIBasicAnimation(PropertyAnimationType.CORNER_RADIUS, 0.0f, 300.0f);
		aniSetFrom.addAnimation(cornerAni1);
		SVIBasicAnimation cornerAni2 = new SVIBasicAnimation(PropertyAnimationType.CORNER_RADIUS, 300.0f, 0.0f);
		aniSetTo.addAnimation(cornerAni2);
		*/
	
		aniSetFrom.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		aniSetTo.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		
		aniSetFrom.setListener(this);
		
		if( mWidth > mHeight ) {
			mPortraitSlide.startAnimation(aniSetFrom);
			mLandscapeSlide.startAnimation(aniSetTo);
		} else {
			mLandscapeSlide.startAnimation(aniSetFrom);
			mPortraitSlide.startAnimation(aniSetTo);
		}		
	}
	
	private float getAngleFromRotation(int rotation) {
		
		float angle = 0.0f;
		switch(rotation) {
		case ROTATION_0:
			angle = 0.0f;
			break;
		case ROTATION_90:
			angle = 90.0f;
			break;
		case ROTATION_180:
			angle = 180.0f;
			break;
		case ROTATION_270:
			angle = 270.0f;
			break;
		default:
			angle = 0.0f;
			break;
		}
		
		return angle;
	}
	
	private float getFromAngle(int oldRotation, int newRotation) {
		float fromAngle = 0.0f;
		float from, to;
		
		switch(oldRotation) {
		case ROTATION_0:
			from = 0.0f;
			break;
		case ROTATION_90:
			from = 90.0f;
			break;
		case ROTATION_180:
			from = 180.0f;
			break;
		case ROTATION_270:
			from = 270.0f;
			break;
		default:
			from = 0.0f;
			break;
		}

		switch(newRotation) {
		case ROTATION_0:
			to = 0.0f;
			break;
		case ROTATION_90:
			to = 90.0f;
			break;
		case ROTATION_180:
			to = 180.0f;
			break;
		case ROTATION_270:
			to = 270.0f;
			break;
		default:
			to = 0.0f;
			break;
		}

		fromAngle = from - to;

		Log.i("Orientation", "from:"+from+", to:"+to);
		
		if( fromAngle == 270.0f )
			fromAngle = -90.0f;
		else if (fromAngle == -270.0f )
			fromAngle = 90.0f;

		//if( mInverse )
		//	fromAngle *= -1.0f;
		return fromAngle;
	}
	
	@Override
	public void onAnimationEnd(String tag) {
		// TODO Auto-generated method stub
		mIsAnimating = false;
		//marx.kim begin
		mActivity.setRequestedOrientation(mOrgOrientation);
		//marx.kim end
		Log.i("Rotation", "Rotation Listener onAnimationEnd");
	}

	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
		mIsAnimating = true;
		Log.i("Rotation", "Rotation Listener onAnimationStart");
	}
	
	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		// TODO Auto-generated method stub
		if( !mFixSurface )
			return;
		
		synchronized (this) {  
            switch(event.sensor.getType()){            	
    			case Sensor.TYPE_ORIENTATION:    				
    				float X = event.values[1];
    				float Y = event.values[2];
    				float absX = Math.abs(X);
    				float absY = Math.abs(Y);
    				//Log.e("KJH", "event.values[1]" + event.values[1]);
    				//Log.e("KJH", "event.values[2]" + event.values[2]);
    				
    				//filter
    				/*
    				if(X < -70.f || X > 70.f){
    					return;
    				}
    				if(Y < -70.f || Y > 70.f){
    					return;
    				}
    				*/
    				
    				//portrait
    				int width = getWidth();
    				int height = getHeight();
    				
    				if(absX > absY){
    					//newOrientation = 0;
    					if(X < 0){
    						if( mRotation != ROTATION_0 ) {
    							onConfigurationChanged(ROTATION_0);	
	    						onResize(width, height);
    						}
    					}
    					else{
    						if( mRotation != ROTATION_180 ) {
    							onConfigurationChanged(ROTATION_180);
    							onResize(width, height);
    						}
    					}
    				}
    				
    				//landscape
    				else{
    					//newOrientation = 1;
    					if( Y > 0.0f){
    						if( mRotation != ROTATION_90 ) {
    							onConfigurationChanged(ROTATION_90);//onConfigurationChanged(ROTATION_180);
	    						onResize(height, width);
    						}
    					}
    					else{
    						if( mRotation != ROTATION_270 ) {
    							onConfigurationChanged(ROTATION_270);//onConfigurationChanged(ROTATION_0);
	    						onResize(height, width);
    						}
    					} 						    					
    				}
    				
            }
            /*
            if(mOrientation != newOrientation){
            	mOrientation = newOrientation;
            	//To do something
            }
            */
        }
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) { 	
		mFixSurface = !mFixSurface;
		
		if( !mFixSurface ) {
			mActivity.setRequestedOrientation(mOrgOrientation);
		} else {
			if( mFixSurface ) {
				mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
			}
		}
		return true;
	}
	
	//marx.kim begin
	private int getCurrentOrientation() {
		// ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE and ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT 
		// attributes are provided from GingerBread.(SDK 9) 
		// So to support backward compatibility use number value instead of constant value.
		int rotation = mActivity.getWindowManager().getDefaultDisplay().getOrientation();
		Log.i("SVI", "Rotation :"+rotation);
        switch (rotation) {
            case Surface.ROTATION_90:
            	return 9;//ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT;
            case Surface.ROTATION_270:
            	return ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
            case Surface.ROTATION_180:
            	return 8;//ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE;

            default:
            	return ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
        }
    }
	//marx.kim end
	
	private Activity		mActivity;
	
	private Bitmap			mPortraitBitmap;
	private Bitmap			mLandscapeBitmap;
	
	private SVIImage			mPortraitImage;
	private SVIImage			mLandscapeImage;
	
	private SVISlide			mPortraitSlide;
	private SVISlide			mLandscapeSlide;

	private SensorManager 	mSensorManager = null;  
    private Sensor 			mOrientationSensor = null;  
    
    private int				mBaseWidth;
    private int				mBaseHeight;
    
    private int				repeatCount = 0;

  //marx.kim begin
    private int				mOrgOrientation = -1;
  //marx.kim end
    
    private boolean			mIsAnimating = false;
	
	private boolean			mFixSurface = false;
}
