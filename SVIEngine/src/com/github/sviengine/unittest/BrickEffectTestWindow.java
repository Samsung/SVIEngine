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
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideDeformType;

public class BrickEffectTestWindow extends PlatformWindow{

	public BrickEffectTestWindow(Context context) {
		super(context, false);
		Log.i("SVI", "BrickEffectTestWindow(Context context)");
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
	
	private Random mRandom = new Random();
	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		if( event.getAction() == MotionEvent.ACTION_UP ) {
			
			for (int n =0; n < 3; n++){
				for (int k =0; k < (int)mHeightCount; k++){
					int d = 20 * mRandom.nextInt(25) * mAnimationSpeed;					
					runRotationSlide(mBrickSlide[n][k], d);					
				}
			}
		}
		
		return true;
	}
		
	protected void runRotationSlide(SVISlide slide, int offset){
		
		SVIAnimationSet slideAniSet = new SVIAnimationSet();
		slideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.SINEEASEINOUT);
		slideAniSet.setDuration(210*mAnimationSpeed);
		slideAniSet.setOffset(offset);
		slideAniSet.setRepeatCount(0);
		slideAniSet.setAutoReverse(false);
		
		SVIKeyFrameAnimation rotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
		rotationAni.setDuration(20*mAnimationSpeed);
		rotationAni.setRepeatCount(0);
		rotationAni.setOffset(10*mAnimationSpeed);
		rotationAni.setAutoReverse(false);
		rotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f));
		rotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,90.0f,0.0f));
				
		slideAniSet.addAnimation(rotationAni);
			
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
				
		
		for (int n =0; n < 3; n++){
			for (int k =0; k < (int)mHeightCount; k++){
				
				float cube_width = mainSize.mWidth * 0.25f * mHeightCount * 0.5f;
				float height = mainSize.mHeight * 0.5f - cube_width;
				
				mBrickSlide[n][k] = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
						mainSize.mWidth * 0.125f + (mainSize.mWidth * 0.25f) * (float)n, height + (mainSize.mWidth * 0.25f) * (float)k,  
						mainSize.mWidth * 0.25f, mainSize.mWidth * 0.25f, color);
			
				mBrickSlide[n][k].setImage(mFrontImage);
				mBrickSlide[n][k].setBackFaceImage(mBackImage);
				mBrickSlide[n][k].setDeformType(SlideDeformType.DEFORM_BRICK);
				mBrickSlide[n][k].setOrthogonal(false);
				
				//2012-06-20 masterkeaton27@gmail.com
				//x :3D Depth , y : Z pivot position
				//if this deform ratio was not setted by user then engine will use
				//x size as a depth value and 0.5 for z axis pivot. 
				mBrickSlide[n][k].setDeformRatio(mainSize.mWidth * 0.25f, 0.5f, 0.0f);
				
				
				float xsize = mFrontImage.mBitmap.getWidth() / 3.0f;
				float ysize = mFrontImage.mBitmap.getHeight() / mHeightCount;
				
				mBrickSlide[n][k].setTextureRegion(
						xsize * n, 
						ysize * k, 
						xsize, 
						ysize);
				
				xsize = mBackImage.mBitmap.getWidth() / 3.0f;
				ysize = mBackImage.mBitmap.getHeight() / mHeightCount;
				
				mBrickSlide[n][k].setBackFaceTextureRegion(
						xsize * n, 
						ysize * k, 
						xsize, 
						ysize);
			}
		}
	}
	
	private final int mAnimationSpeed = 3;
	private final float mHeightCount = 6.0f;
	private SVISlide [][] mBrickSlide = new SVISlide[3][(int)mHeightCount];
	
	private SVIImage mFrontImage = null;
	private Bitmap	mFrontBitmap = null;
	
	private SVIImage mBackImage = null;
	private Bitmap	mBackBitmap = null;
	
}
