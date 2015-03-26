
package com.github.sviengine.unittest;

import java.util.Random;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.ImageScaleType;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class ScaleToFitTestWindow  extends PlatformWindow implements SVIAnimationListener{

	public ScaleToFitTestWindow(Context context) {
		super(context);
		
		mMsgHandler = new Handler() {

			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				
				if( mCounter < 30 ) {
					Message newMsg = new Message();
					mMsgHandler.sendMessageDelayed(newMsg, 100);
					mCounter++;
										
					runKeyFrameAnimation();					
				} else {
					mCounter = 0;
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
			buildSubSlide();
		
		super.onResize(width, height);
	}
	
	protected Random tempRandom = new Random();
	
	protected int g_width = 400;
	protected void runKeyFrameAnimation(){
		
		//SVISlide mainSlide = getMainSlide();
		int count = 0;
		int offset = 0;
		int duration = 5000;
		
		
		/*
		SVIAnimationSet aniSetMain = new SVIAnimationSet(duration, 1, false, 0);		
		SVIKeyFrameAnimation mainLightColorAni = new 
		SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_COLOR);
		mainLightColorAni.setDuration(duration);
		mainLightColorAni.setRepeatCount(1);
		mainLightColorAni.setAutoReverse(false);
		mainLightColorAni.setOffset(offset * count);
		mainLightColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.0f));
		mainLightColorAni.addKeyProperty(0.1f, new SVIColor(1.0f,0.0f,0.0f,1.0f));	
		mainLightColorAni.addKeyProperty(0.9f, new SVIColor(0.0f,0.0f,1.0f,1.0f));	
		mainLightColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,0.0f));
		aniSetMain.addAnimation(mainLightColorAni);
		
			
		
		SVIKeyFrameAnimation MainLightAngleAni = new 
		SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_ANGLE);
		MainLightAngleAni.setDuration(duration);
		MainLightAngleAni.setRepeatCount(1);
		MainLightAngleAni.setAutoReverse(false);
		MainLightAngleAni.setOffset(offset * count);
		MainLightAngleAni.addKeyProperty(0.0f, 0.0f);
		MainLightAngleAni.addKeyProperty(0.3f, 90.0f);
		MainLightAngleAni.addKeyProperty(0.5f, 270.0f);
		MainLightAngleAni.addKeyProperty(0.8f, -90.0f);
		aniSetMain.addAnimation(MainLightAngleAni);
		mainSlide.startAnimation(aniSetMain);
		*/
		
		for (int n = 0; n < mIconSlide.length; n++){
			SVISlide slide = mIconSlide[n];
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(1);
			aniSet.setAutoReverse(false);
			//aniSet.setListener(this);
			aniSet.setOffset(offset * count);

			//initializeKeyFrameAnimation
			
			//aniSet.addAnimation(keyPivotAni);
			
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
			
			SVIKeyFrameAnimation keyLightColorAni = new 
			SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_COLOR);
			keyLightColorAni.setDuration(duration);
			keyLightColorAni.setRepeatCount(1);
			keyLightColorAni.setAutoReverse(false);
			keyLightColorAni.setOffset(offset * count);
			keyLightColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.8f));
			keyLightColorAni.addKeyProperty(0.3f, 
					new SVIColor(tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f,1.0f));
			keyLightColorAni.addKeyProperty(0.5f, 
					new SVIColor(tempRandom.nextFloat(),1.0f,tempRandom.nextFloat(),1.0f));
			keyLightColorAni.addKeyProperty(0.8f, 
					new SVIColor(1.0f,tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f));
			keyLightColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,0.8f));
			aniSet.addAnimation(keyLightColorAni);
			
			
			
			boolean playRegionAni = true;
			if( playRegionAni ) {
				SVIKeyFrameAnimation keyRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.REGION);
				keyRegionAni.setDuration(duration);
				keyRegionAni.setRepeatCount(1);
				keyRegionAni.setAutoReverse(false);
				keyRegionAni.setOffset(offset * count);
				SVIRect region = slide.getRegion();
				keyRegionAni.addKeyProperty(0.0f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.1f, region.mSize.mHeight*1.1f));
				keyRegionAni.addKeyProperty(0.5f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.3f, region.mSize.mHeight*1.3f));
				keyRegionAni.addKeyProperty(1.0f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.0f, region.mSize.mHeight*1.0f));
				aniSet.addAnimation(keyRegionAni);
			}
			
			float height = slide.getImage().mBitmap.getHeight();
			float width = slide.getImage().mBitmap.getHeight();
			
			SVIKeyFrameAnimation keyTexRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALETOFIT_REGION);
			keyTexRegionAni.setDuration(duration);
			keyTexRegionAni.setRepeatCount(1);
			keyTexRegionAni.setAutoReverse(false);
			keyTexRegionAni.setOffset(offset * count);
			keyTexRegionAni.addKeyProperty(0.0f, new SVIRect(width/2,0.0f,width, height));
			keyTexRegionAni.addKeyProperty(0.5f, new SVIRect(width/2,height/2,width/2, height/2));
			keyTexRegionAni.addKeyProperty(0.8f, new SVIRect(0.0f,0.0f,width, height));
			keyTexRegionAni.addKeyProperty(1.0f, new SVIRect(width/2,0.0f,width, height));
			aniSet.addAnimation(keyTexRegionAni);
			*/
			
			SVIKeyFrameAnimation keyShadowOffsetAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OFFSET);
			keyShadowOffsetAni.setDuration(duration);
			keyShadowOffsetAni.setRepeatCount(1);
			keyShadowOffsetAni.setAutoReverse(false);
			keyShadowOffsetAni.setOffset(offset * count);
			keyShadowOffsetAni.addKeyProperty(0.0f, new SVIPoint(0.0f,0.0f));
			keyShadowOffsetAni.addKeyProperty(0.5f, new SVIPoint(50.0f,50.0f));
			keyShadowOffsetAni.addKeyProperty(1.0f , new SVIPoint(0.0f,0.0f));
			aniSet.addAnimation(keyShadowOffsetAni);
			
			
			SVIKeyFrameAnimation keyShadowOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.CORNER_RADIUS);
			keyShadowOpacityAni.setDuration(duration);
			keyShadowOpacityAni.setRepeatCount(1);
			keyShadowOpacityAni.setAutoReverse(false);
			keyShadowOpacityAni.setOffset(offset * count);
			
			keyShadowOpacityAni.addKeyProperty(0.0f, 50.0f);
			keyShadowOpacityAni.addKeyProperty(0.2f, 0.0f);
			keyShadowOpacityAni.addKeyProperty(0.5f, 50.0f);
			keyShadowOpacityAni.addKeyProperty(0.7f, 0.0f);
			keyShadowOpacityAni.addKeyProperty(1.0f, 50.0f);
			
			aniSet.addAnimation(keyShadowOpacityAni);
			
			

			SVIKeyFrameAnimation keyOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OPACITY);
			keyOpacityAni.setDuration(duration);
			keyOpacityAni.setRepeatCount(1);
			keyOpacityAni.setAutoReverse(false);
			keyOpacityAni.setOffset(offset * count);
			keyOpacityAni.addKeyProperty(0.0f, 0.5f);
			keyOpacityAni.addKeyProperty(0.5f, 0.2f);
			keyOpacityAni.addKeyProperty(1.0f, 0.5f);
			aniSet.addAnimation(keyOpacityAni);
			
			SVIKeyFrameAnimation keyShadowRadiusAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_RADIUS);
			keyShadowRadiusAni.setDuration(duration);
			keyShadowRadiusAni.setRepeatCount(1);
			keyShadowRadiusAni.setAutoReverse(false);
			keyShadowRadiusAni.setOffset(offset * count);
			keyShadowRadiusAni.addKeyProperty(0.0f, 0.0f);
			keyShadowRadiusAni.addKeyProperty(0.5f, 100.0f);
			keyShadowRadiusAni.addKeyProperty(1.0f, 00.0f);
			aniSet.addAnimation(keyShadowRadiusAni);
		
			slide.startAnimation(aniSet);
			count++;
		}
	}
	
	
	private int tempindex = 0;
	private int addindex = 0;
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub	
		if( !mHardTest ) {
			if( event.getAction() == MotionEvent.ACTION_DOWN ) {
				
					runKeyFrameAnimation();
				
			}
		} else {
			if( event.getAction() == MotionEvent.ACTION_DOWN ) {
				Bitmap bmp = null;				
				if (!mAddorRemove){
					mIconSlide[tempindex].setImage(bmp);
					tempindex++;
					tempindex = tempindex % mIconSlide.length;
				}else{
					int imageCnt = mImages.length-1;
					int imageIdx = addindex % imageCnt;
					mIconSlide[addindex].setImage(mImages[imageIdx]);
					addindex++;
					addindex = addindex % mIconSlide.length;
				}
				if( mCounter == 0 ) {
					Message newMsg = new Message();
					mMsgHandler.sendMessage(newMsg);
				} else {
					mCounter = 200;
				}
			} else if( event.getAction() == MotionEvent.ACTION_MOVE ) {
				getMainSlide().setPosition(event.getX(), event.getY());
			}
		}
		return true;
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//float lightcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
				
		SVISlide mainSlide = getMainSlide();		
		mainSlide.setImage(mImages[mImages.length-1]);
		mainSlide.setImageScaleType(ImageScaleType.CENTER_INSIDE);
		mainSlide.setBackgroundColor(color);
		mainSlide.setHoldOpacity(true);
		mainSlide.setOrthogonal(true);
		//mainSlide.setLightType(LightType.LINEAR_LIGHT);
		
		//mainSlide.setTextureRegion(0.0f, 0.0f, 400.0f, 400.0f);
		
		
		mIconSlide = new SVISlide[LANDSCAPE_COLUMN_COUNT*LANDSCAPE_ROW_COUNT];
		
		
		//int imageCnt = mImages.length-1;
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
		
		boolean testSlideHierarchy = false;
		for(int i=0; i<slideCount; i++) {
			//int imgIdx = count % (imageCnt);
			x = (count%columnCount) * width;
			y = (count/columnCount) * height;
			if( !testSlideHierarchy ) {
				mIconSlide[i] = new SVISlide(0, 0, 
						(x+mLeftMargin), (y+mTopMargin), ICON_WIDTH, ICON_HEIGHT, color);
			} else {
				mIconSlide[i] = new SVISlide(0, 0, 
						(3), (3), ICON_WIDTH, ICON_HEIGHT, color);
			}
			
			if( !testSlideHierarchy ) {
				mainSlide.addSlide(mIconSlide[i]);
			} else {
				if( i == 0 )
					mainSlide.addSlide(mIconSlide[i]);
				else 
					mIconSlide[i-1].addSlide(mIconSlide[i]);
			}
		
			//mIconSlide[i].setImage(mImages[5]);
			mIconSlide[i].setImage(mImages[mImages.length-2]);
			//mIconSlide[i].setLightType(i%3);
			mIconSlide[i].setImageScaleType(i%8);
			mIconSlide[i].setBorderWidth(20.0f);
			
						
			//mIconSlide[i].setBackgroundColor(new SVIColor(0.0f,0.0f,1.0f,1.0f));
			
			//mIconSlide[i].setLightColor(lightcolor);
			//mIconSlide[i].setBlendType(1);
			mIconSlide[i].setBackgroundColor(
					new float[]{
							tempRandom.nextFloat(),
							tempRandom.nextFloat(),
							tempRandom.nextFloat(),1.0f});
			
			mIconSlide[i].setBlendType(SVISlide.BlendType.BLEND_ADD);
			
			//2011.12.09 jongchae.moon
			mIconSlide[i].setAntiAliasing(mEnableAA);
			//
			
			/*
			mIconSlide[i].setBorderColor(
					new float[]{
							tempRandom.nextFloat(),
							tempRandom.nextFloat(),
							tempRandom.nextFloat(),1.0f});
			 */
			//mIconSlide[i].setShadowRadius(20.0f);
			//mIconSlide[i].setShadowOffset(0.0f, 10.0f);
			//mIconSlide[i].setShadowOpacity(0.5f);
			//mIconSlide[i].setShadowRadius(40.0f);
			//mIconSlide[i].setCornerRadius(50.0f);
			//if( (i % 2) == 0 )
			//	mIconSlide[i].setHidden(true);
			count++;
			mIconSlide[i].setHoldOpacity(true);
			mIconSlide[i].setHoldScale(true);
		}	
	
		
	}

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
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		if(item.getTitle() == "Shift Image Scale Type")
		{
			// TODO Auto-generated method stub	
			mAddorRemove = !mAddorRemove;
			
			//ImageScaleType.
			for (int n = 0; n < 8; n++){
				int scaleType =mIconSlide[n].getImageScaleType();
				scaleType++;
				scaleType = scaleType % 8;
				mIconSlide[n].setImageScaleType(scaleType);
			}
		}
		else if( item.getTitle() == "Enable Anti-Aliasing" )
		{
			mEnableAA = true;
			
			SVISlide mainSlide = getMainSlide();
			
			for (int n = 0; n < 8; n++){
				mainSlide.detachSlide(mIconSlide[n]);
			}
			
			buildSubSlide();
		}
		else if( item.getTitle() == "Disable Anti-Aliasing" )
		{
			mEnableAA = false;
			
			SVISlide mainSlide = getMainSlide();
			
			for (int n = 0; n < 8; n++){
				mainSlide.detachSlide(mIconSlide[n]);
			}
			
			buildSubSlide();
		}
		
		
		//requestRender();
		return super.onOptionsItemSelected(item);
	}
	

	final protected int PORTRAIT_COLUMN_COUNT = 2;
	final protected int PORTRAIT_ROW_COUNT = 4;

	final protected float ICON_WIDTH		= 250.0f;
	final protected float ICON_HEIGHT		= 200.0f;
	
	
	final protected int LANDSCAPE_COLUMN_COUNT	= PORTRAIT_ROW_COUNT;
	final protected int LANDSCAPE_ROW_COUNT		= PORTRAIT_COLUMN_COUNT;
	
	protected SVISlide[]		mIconSlide;
	
	Random mRandom = new Random();
	int mCurrentRowCount = 0;
	int mCurrentColumnCount = 0;
	float mLeftMargin = 0.0f;
	float mRightMargin = 0.0f;
	float mTopMargin = 0.0f;
	float mBottomMargin = 0.0f;
	
	private boolean				mAddorRemove = false;
	
	private boolean				mHardTest = false;
	private Handler				mMsgHandler;
	private int					mCounter = 0;
	private boolean				mEnableAA = false;
}



