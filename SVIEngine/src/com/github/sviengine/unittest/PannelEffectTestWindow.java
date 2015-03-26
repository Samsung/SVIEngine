package com.github.sviengine.unittest;

import java.io.InputStream;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideDeformType;

public class PannelEffectTestWindow extends PlatformWindow{

	public PannelEffectTestWindow(Context context) {
		super(context, false);
		Log.i("SVI", "PannelEffectTestWindow(Context context)");
		Activity activity = (Activity)context;
		InputStream is;
		
		is = activity.getResources().openRawResource(R.drawable.art_1);
		mFrontBitmap = BitmapFactory.decodeStream(is);
		mFrontImage = new SVIImage();
		mFrontImage.setBitmap(mFrontBitmap);
		
		is = activity.getResources().openRawResource(R.drawable.art_6);
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
									
			for (int n =0; n< mHeightCount; n++){		
				int d = mRandom.nextInt(10) * 20 * mAnimationSpeed;
				
				runRotationSlide(mPannelSlide[n],d);
			}
		}
		
		return true;
	}
	
	
	private Random mRandom = new Random();
	
	protected void runRotationSlide(SVISlide slide, int d){
		
		
		SVIAnimationSet slideAniSet = new SVIAnimationSet();
		slideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.SINEEASEINOUT);
		slideAniSet.setDuration(210*mAnimationSpeed);
		slideAniSet.setOffset(d);
		slideAniSet.setRepeatCount(0);
		slideAniSet.setAutoReverse(false);
		
		
		float newXTheta = mRandom.nextFloat() * 360.0f;
		float newYTheta = mRandom.nextFloat() * 360.0f;
		float newZTheta = mRandom.nextFloat() * 360.0f;		
		
		float newPivotX = mRandom.nextFloat() - 0.5f;
		float newPivotY = mRandom.nextFloat() - 0.5f;
		float newPosZ = mRandom.nextFloat() - 0.5f;
		
		
		SVIKeyFrameAnimation rotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
		rotationAni.setDuration(20*mAnimationSpeed);
		rotationAni.setRepeatCount(0);
		rotationAni.setOffset(10*mAnimationSpeed);
		rotationAni.setAutoReverse(false);
		rotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f));
		rotationAni.addKeyProperty(0.5f, new SVIVector3(newXTheta,newYTheta,newZTheta));
		rotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,180.0f,0.0f));
		
		SVIKeyFrameAnimation pivotAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.PIVOT_POINT);
		pivotAni.setDuration(20*mAnimationSpeed);
		pivotAni.setRepeatCount(0);
		pivotAni.setOffset(10*mAnimationSpeed);
		pivotAni.setAutoReverse(false);
		pivotAni.addKeyProperty(0.0f, new SVIPoint(0.5f,0.5f));
		pivotAni.addKeyProperty(0.5f, new SVIPoint(newPivotX*2.0f,newPivotY*2.0f));
		pivotAni.addKeyProperty(1.0f, new SVIPoint(0.5f,0.5f));
		
		SVIKeyFrameAnimation zposAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ZPOSITION);
		zposAni.setDuration(20*mAnimationSpeed);
		zposAni.setRepeatCount(0);
		zposAni.setOffset(10*mAnimationSpeed);
		zposAni.setAutoReverse(false);
		zposAni.addKeyProperty(0.0f, 0.0f);
		zposAni.addKeyProperty(0.5f, newPosZ * 1000.0f);
		zposAni.addKeyProperty(1.0f, 0.0f);
				
		
		slideAniSet.addAnimation(pivotAni);
		slideAniSet.addAnimation(rotationAni);
		slideAniSet.addAnimation(zposAni);
			
		slide.startAnimation(slideAniSet);			
				
		slide.setRotation(newXTheta, newYTheta, newZTheta, 0.0f, 1000);
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
				
		for (int n =0; n< mHeightCount; n++){
			
			float sizeCell = (mainSize.mHeight * 0.8f) / (float)mHeightCount; 
			float sizeHeight = sizeCell * mHeightCount * 0.5f;
			float sizeWidth = mainSize.mWidth * 0.7f; 			
			float yOffset = mainSize.mHeight * 0.5f - sizeHeight;
			
			mPannelSlide[n] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
					mainSize.mWidth * 0.15f, yOffset + sizeCell * (float)n,  
					sizeWidth, sizeCell, color);
		
			mPannelSlide[n].setImage(mFrontImage);
			mPannelSlide[n].setBackFaceImage(mBackImage);
			mPannelSlide[n].setDeformType(SlideDeformType.DEFORM_PANNEL);
			mPannelSlide[n].setOrthogonal(false);
			
			//2012-06-20 masterkeaton27@gmail.com
			//x :3D Depth , y : Z pivot position
			//if this deform ratio was not setted by user then engine will use
			//x size as a depth value and 0.5 for z axis pivot. 
			mPannelSlide[n].setDeformRatio(20, 0.5f, 0.0f);
			
			float xsize = mFrontImage.mBitmap.getWidth();
			float ysize = mFrontImage.mBitmap.getHeight() / mHeightCount;
			
			mPannelSlide[n].setTextureRegion(
					0, 
					ysize * n, 
					xsize, 
					ysize);
			
			xsize = mBackImage.mBitmap.getWidth();
			ysize = mBackImage.mBitmap.getHeight() / mHeightCount;
			
			mPannelSlide[n].setBackFaceTextureRegion(
					0, 
					ysize * n, 
					xsize, 
					ysize);
		}
	}
	
	private final int mAnimationSpeed = 10;
	private final int mHeightCount = 7;
	private SVISlide mPannelSlide[] = new SVISlide[mHeightCount];
	
	private SVIImage mFrontImage = null;
	private Bitmap	mFrontBitmap = null;
	
	private SVIImage mBackImage = null;
	private Bitmap	mBackBitmap = null;
	
}
