package com.github.sviengine.performance;

import java.util.Random;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlideManager;
import com.github.sviengine.slide.SVISlide.BlendType;

import android.content.Context;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class PerformanceTestWindow extends PlatformWindow implements SVIAnimationListener{

	public PerformanceTestWindow(Context context) {
		super(context);
		
		if( getMainSlide() == null )
			buildSubSlide();
		
	
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
		
		
		
		super.onResize(width, height);
	}
	
	protected Random tempRandom = new Random();
	
	protected void runKeyFrameAnimation(){
		
		int count = 0;
		
		
		mSlideManager.setPauseImplicitAnimation(true);
		for (int n = 0; n < mIconSlide.length; n++){
			SVISlide slide = mIconSlide[n];
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(repeatCount);
			aniSet.setAutoReverse(false);
			aniSet.setListener(this);
			aniSet.setOffset(offset * count);

			
			
			
			
			SVIKeyFrameAnimation keyRotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
			keyRotationAni.setDuration(duration);
			keyRotationAni.setRepeatCount(repeatCount);
			keyRotationAni.setAutoReverse(false);
			keyRotationAni.setOffset(offset * count);
			keyRotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f));
			keyRotationAni.addKeyProperty(0.3f, new SVIVector3(0.0f,0.0f,90.0f));
			keyRotationAni.addKeyProperty(0.5f, new SVIVector3(0.0f,0.0f,0.0f));
			keyRotationAni.addKeyProperty(0.8f, new SVIVector3(0.0f,0.0f,90.0f));
			keyRotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f,0.0f));
			aniSet.addAnimation(keyRotationAni);
			
			
			SVIKeyFrameAnimation keyOpacityAni = new 
			SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyOpacityAni.setDuration(duration);
			keyOpacityAni.setRepeatCount(repeatCount);
			keyOpacityAni.setAutoReverse(false);
			keyOpacityAni.setOffset(offset * count);
			keyOpacityAni.addKeyProperty(0.0f, 1.0f);
			keyOpacityAni.addKeyProperty(0.5f, 0.0f);
			keyOpacityAni.addKeyProperty(1.0f, 1.0f);
			aniSet.addAnimation(keyOpacityAni);
		
			
			if(isLight)
			{
				SVIKeyFrameAnimation keyLightOffsetAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_OFFSET);
				keyLightOffsetAni.setDuration(duration);
				keyLightOffsetAni.setRepeatCount(repeatCount);
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
				keyLightRadiusAni.setRepeatCount(repeatCount);
				keyLightRadiusAni.setAutoReverse(false);
				keyLightRadiusAni.setOffset(offset * count);
				keyLightRadiusAni.addKeyProperty(0.0f, 0.2f);
				keyLightRadiusAni.addKeyProperty(0.5f, 1.0f);
				keyLightRadiusAni.addKeyProperty(0.8f, 0.52f);
				keyLightRadiusAni.addKeyProperty(1.0f, 0.2f);
				aniSet.addAnimation(keyLightRadiusAni);
				
				SVIKeyFrameAnimation keyLightAngleAni = new 
				SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_ANGLE);
				keyLightAngleAni.setDuration(duration);
				keyLightAngleAni.setRepeatCount(repeatCount);
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
				keyLightColorAni.setRepeatCount(repeatCount);
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
			}
			
			
			boolean playRegionAni = true;
			if( playRegionAni ) {
				SVIKeyFrameAnimation keyRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.REGION);
				keyRegionAni.setDuration(duration);
				keyRegionAni.setRepeatCount(repeatCount);
				keyRegionAni.setAutoReverse(false);
				keyRegionAni.setOffset(offset * count);
				SVIRect region = slide.getRegion();
				keyRegionAni.addKeyProperty(0.0f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.1f, region.mSize.mHeight*1.1f));
				keyRegionAni.addKeyProperty(0.5f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.3f, region.mSize.mHeight*1.3f));
				keyRegionAni.addKeyProperty(1.0f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.0f, region.mSize.mHeight*1.0f));
				aniSet.addAnimation(keyRegionAni);
			}
		
			
			
			if(isShadow)
			{
				SVIKeyFrameAnimation keyShadowOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OPACITY);
				keyShadowOpacityAni.setDuration(duration);
				keyShadowOpacityAni.setRepeatCount(repeatCount);
				keyShadowOpacityAni.setAutoReverse(false);
				keyShadowOpacityAni.setOffset(offset * count);
				keyShadowOpacityAni.addKeyProperty(0.0f, 0.0f);
				keyShadowOpacityAni.addKeyProperty(1.0f, 1.0f);
				aniSet.addAnimation(keyShadowOpacityAni);
				
				
				SVIKeyFrameAnimation keyShadowAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_RADIUS);
				keyShadowAni.setDuration(duration);
				keyShadowAni.setRepeatCount(repeatCount);
				keyShadowAni.setAutoReverse(false);
				keyShadowAni.setOffset(offset * count);
				keyShadowAni.addKeyProperty(0.0f, 0.0f);
				keyShadowAni.addKeyProperty(1.0f, 15.0f);
				aniSet.addAnimation(keyShadowAni);
				
				SVIKeyFrameAnimation keyShadowOffsetAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OFFSET);
				keyShadowOffsetAni.setDuration(duration);
				keyShadowOffsetAni.setRepeatCount(repeatCount);
				keyShadowOffsetAni.setAutoReverse(false);
				keyShadowOffsetAni.setOffset(offset * count);
				keyShadowOffsetAni.addKeyProperty(0.0f, new SVIPoint(0.0f,0.0f));
				keyShadowOffsetAni.addKeyProperty(0.5f, new SVIPoint(30.0f,30.0f));
				keyShadowOffsetAni.addKeyProperty(1.0f, new SVIPoint(0.0f,0.0f));
				aniSet.addAnimation(keyShadowOffsetAni);
				
			}
			
			
			if(isScaletoFitMat){
				float height = slide.getImage().mBitmap.getHeight();
				float width = slide.getImage().mBitmap.getWidth();
				
				SVIKeyFrameAnimation keyFitRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALETOFIT_REGION);
				keyFitRegionAni.setDuration(duration);
				keyFitRegionAni.setRepeatCount(repeatCount);
				keyFitRegionAni.setAutoReverse(false);
				keyFitRegionAni.setOffset(offset * count);
				float stepwidth = ICON_WIDTH/4.0f;
				keyFitRegionAni.addKeyProperty(0.0f, new SVIRect(0.0f,0.0f,0.0f, ICON_HEIGHT));
				keyFitRegionAni.addKeyProperty(0.3f, new SVIRect(0.0f,0.0f,stepwidth*2.0f, ICON_HEIGHT));
				keyFitRegionAni.addKeyProperty(0.8f, new SVIRect(0.0f,0.0f,stepwidth*3.0f, ICON_HEIGHT));
				keyFitRegionAni.addKeyProperty(1.0f, new SVIRect(0.0f,0.0f,stepwidth*4.0f, ICON_HEIGHT));
				aniSet.addAnimation(keyFitRegionAni);
				
				
				SVIKeyFrameAnimation keyTexRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.TEXTURE_REGION);
				keyTexRegionAni.setDuration(duration);
				keyTexRegionAni.setRepeatCount(repeatCount);
				keyTexRegionAni.setAutoReverse(false);
				keyTexRegionAni.setOffset(offset * count);
				stepwidth = width/4.0f;
				keyTexRegionAni.addKeyProperty(0.0f, new SVIRect(0.0f,0.0f,0.0f, height));
				keyTexRegionAni.addKeyProperty(0.3f, new SVIRect(0.0f,0.0f,stepwidth*2.0f, height));
				keyTexRegionAni.addKeyProperty(0.8f, new SVIRect(0.0f,0.0f,stepwidth*3.0f, height));
				keyTexRegionAni.addKeyProperty(1.0f, new SVIRect(0.0f,0.0f,stepwidth*4.0f, height));
				aniSet.addAnimation(keyTexRegionAni);
			}
			
			
			slide.startAnimation(aniSet);
			count++;
		}
		getSlideManager().setPauseImplicitAnimation(false);
		getSlideManager().requestRender();
	}
	
	
	
	
	protected void removeall(){
		SVISlide mainSlide = getMainSlide();		
			
		if (mIconSlide != null){
			int  slideCount = mCurrentRowCount * mCurrentColumnCount;			
			for(int i=0; i<slideCount; i++) {
				mainSlide.removeSlide(mIconSlide[i]);
				mIconSlide[i] = null;
			}
		}
		mIconSlide = null;
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 0.651f, 0.792f, 0.941f, 1.0f };
				
		SVISlide mainSlide = getMainSlide();		
		
		mainSlide.setBackgroundColor(color);
		mainSlide.setHoldOpacity(true);
		mainSlide.setOrthogonal(false);
		
	
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
		
		boolean testSlideHierarchy = false;
		for(int i=0; i<slideCount; i++) {
		
			//change imgIdx routine to avoid using 
			int imgIdx = count % (imageCnt-2) + 2;
			x = (count%columnCount) * width;
			y = (count/columnCount) * height;
			if( !testSlideHierarchy ) {
				mIconSlide[i] = new SVISlide(0, 0, (x+mLeftMargin), (y+mTopMargin), ICON_WIDTH, ICON_HEIGHT, color);
			} else {
				mIconSlide[i] = new SVISlide(0, 0, (3), (3), ICON_WIDTH, ICON_HEIGHT, color);
			}
			
			if( !testSlideHierarchy ) {
				mainSlide.addSlide(mIconSlide[i]);
			} else {
				if( i == 0 )
					mainSlide.addSlide(mIconSlide[i]);
				else 
					mIconSlide[i-1].addSlide(mIconSlide[i]);
			}
		
			mIconSlide[i].setImage(mImages[imgIdx]);
			
			
			if(isBlendAdd)
				mIconSlide[i].setBlendType(BlendType.BLEND_ADD);
			else
				mIconSlide[i].setBlendType(BlendType.BLEND_MULTIPLY);

			
			
			if(isLight)
				mIconSlide[i].setLightType(i%2+1);
			
			if(isShadow)
			{
				mIconSlide[i].setShadowRadius(20.0f);
				mIconSlide[i].setShadowOffset(0.0f, 10.0f);
				mIconSlide[i].setShadowOpacity(0.5f);
				mIconSlide[i].setShadowRadius(40.0f);
			}
			
			
		
			float imageHeight = mIconSlide[i].getImage().mBitmap.getHeight();
			float imageWidth = mIconSlide[i].getImage().mBitmap.getWidth();
			float mCornerSize;
			mCornerSize = (float) (cornerRadSize*0.01) * (imageWidth>=imageHeight?imageWidth:imageHeight);
			
			mIconSlide[i].setCornerRadius(mCornerSize);
			
			
			
			if(isScaletoFitMat){
				mIconSlide[i].setImageScaleType(0);
			}
			
			if(isBgColor){
				mIconSlide[i].setBackgroundColor(
						new float[]{
								tempRandom.nextFloat(),
								tempRandom.nextFloat(),
								tempRandom.nextFloat(),1.0f});
			}
			
			count++;
			mIconSlide[i].setHoldOpacity(true);
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
	public boolean onTouchEvent(MotionEvent event) {
		
		
		if( event.getAction() == MotionEvent.ACTION_DOWN ){
			runKeyFrameAnimation();
		}
		
		requestRender();
		return true;
	}
	
	
	
	
	public void setItemQuantity(int mColumn, int mRow)
	{
		PORTRAIT_COLUMN_COUNT = mColumn;
		PORTRAIT_ROW_COUNT = mRow;
		
		LANDSCAPE_COLUMN_COUNT = mRow;
		LANDSCAPE_ROW_COUNT = mColumn;
	}
	
	public void setItemSize(int mSize)
	{
		ICON_WIDTH = mSize * 2.0f;
		ICON_HEIGHT = mSize * 2.0f;
	}
	
	
	
	protected SVISlide[]			mIconSlide = null;

	protected int PORTRAIT_COLUMN_COUNT = 1;
	protected int PORTRAIT_ROW_COUNT = 1;

	protected float ICON_WIDTH		= 72.0f;
	protected float ICON_HEIGHT		= 72.0f;
	
	protected int LANDSCAPE_COLUMN_COUNT	= PORTRAIT_ROW_COUNT;
	protected int LANDSCAPE_ROW_COUNT		= PORTRAIT_COLUMN_COUNT;
	
	
	protected int 		duration = 5000;
	protected int		cornerRadSize = 0;
	
	
	final protected int  		offset = 0;
	final protected int 		repeatCount = 0;
	
	
	protected boolean				isShadow = false;
	protected boolean				isLight = false;
	protected boolean				isBlendAdd = false;
	protected boolean				isScaletoFitMat = false;
	protected boolean				isBgColor = false;
	
	
	
	Random mRandom = new Random();
	int mCurrentRowCount = 0;
	int mCurrentColumnCount = 0;
	float mLeftMargin = 0.0f;
	float mRightMargin = 0.0f;
	float mTopMargin = 0.0f;
	float mBottomMargin = 0.0f;
}



