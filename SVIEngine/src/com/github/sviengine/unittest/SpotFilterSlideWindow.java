package com.github.sviengine.unittest;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnHoverListener;

public class SpotFilterSlideWindow extends PlatformWindow implements OnHoverListener{

	float mSketchScaleX = 1.0f;
	float mSketchScaleY = 1.0f;
	
	float mMinimumDistance = 0.05f;
	float mMaximumSpreadDistance = 0.3f;
	float mMaximumLockDistance = 0.3f;
	
	int mDuration = 100;
	public SpotFilterSlideWindow(Context context) {
		super(context);
		setHovered(true);
		setOnHoverListener(this);		
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
		mWidth = w;
		mHeight = h; 
	}

	public void onConfigurationChanged(int rotation) {
	}
		
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub		
		if( getMainSlide() == null )
			buildSlideTree();
		
		super.onResize(width, height);
	}
			
	public float getIntensity(){
		return mIntensity;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {	
		if (item.getTitle() == "Change Image") changeImage();
		if (item.getTitle() == "En/Disable Hover") mUseHover = !mUseHover;
		if (item.getTitle() == "Inverse Color") {
			mInverseColor = !mInverseColor;
			mSpotSlide.setDeformRatio(0.0f,0.0f,0.0f);
			mSpotSlide.setSwipeRatio(0.0f, 0.0f, mInverseColor ? 1.0f : 0.0f);		
		}
		return true;
	}
	
	boolean mInverseColor = true;
	boolean mUseHover = false;
	
	public void changeImage(){		
		mImageIdx++;
		mImageIdx = mImageIdx % 4;
		if (mImageIdx == 0) mImageIdx = 1;
		mFilterSlide.setImage(mCoverImage[mImageIdx]);		
		mSpotSlide.setImage(mCoverImage[mImageIdx]);
	}
		
	float downx;
	float downy;
	float mSkippedTime = 0.0f;
	float mAnimatedTime = 0.0f;
	
	private boolean mHovered = false;
	private boolean mHoveredStart = false;
	
	@Override
	public boolean onHover(View v, MotionEvent event) {
		if (!mUseHover) return false;
		if (mSpotSlide == null) return false;
		if (mHoveredStart) mSkippedTime += 0.1f;
		
		float x = event.getX() / mWidth;
		float y = event.getY() / mHeight;
		
		
		if (mSkippedTime > 1.0f && mHoveredStart && !mHovered){
			runEnter(x,y);
			mHovered = true;
			mSkippedTime = 0.0f;
		}		
		
		if (event.getAction() == MotionEvent.ACTION_HOVER_ENTER){		
			mHoveredStart = true;
		}
	
		if (event.getAction() == MotionEvent.ACTION_HOVER_MOVE){		
			if (mHovered) {
				mSpotSlide.setDeformRatio(x, y, 0.0f);
				
				mAnimatedTime += 0.02f;
				if (mAnimatedTime > 1.0f){
					mAnimatedTime = 0.0f;
				}
				float delta = (float)Math.sin(mAnimatedTime * 3.141592f);
				mSpotSlide.setSwipeRatio(mMinimumDistance+delta * 0.05f, mMaximumSpreadDistance-delta * 0.05f, mInverseColor ? 1.0f : 0.0f);
			}
		}

		if (event.getAction() == MotionEvent.ACTION_HOVER_EXIT){	
			if (mHoveredStart){
				runExit(x,y, mMinimumDistance);
			}
			mSkippedTime = 0.0f;
			mHoveredStart = false;
			mHovered = false;
		}						
		// TODO Auto-generated method stub
		return false;
	}	
	
	@Override
	public boolean onHoverEvent(MotionEvent event) {
		
		return false;
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (mSpotSlide == null) return false;
		float x = event.getX() / mWidth;
		float y = event.getY() / mHeight;
				
		if (event.getAction() == MotionEvent.ACTION_MOVE){		
			
			float distx = x - downx;
			float disty = y - downy;
			
			float dist = (float)Math.sqrt(distx*distx+disty*disty); 
			if (dist < mMinimumDistance) dist = mMinimumDistance;
			
			mSpotSlide.setSwipeRatio(dist, mMaximumSpreadDistance, mInverseColor ? 1.0f : 0.0f);		
			
			float xvalue = (float)Math.abs(Math.cos(x * 3.141592f * 0.5f)) * 0.75f + 0.25f;
			if (!mInverseColor) mFilterSlide.setDeformRatio(1.0f, 1.0f, xvalue);
		}
						
		if (event.getAction() == MotionEvent.ACTION_DOWN){
			runEnter(x,y); 
			mHovered = false; 
			downx = x;
			downy = y;
		}
		
		if (event.getAction() == MotionEvent.ACTION_UP){			
			float distx = x - downx;
			float disty = y - downy;			
			float dist = (float)Math.sqrt(distx*distx+disty*disty); 
			
			if (dist > mMaximumLockDistance){
				runExtend(downx,downy, dist);
			}else{
				runExit(downx,downy, dist);
			}			
		}		
		return true;
	}
	
	public void runEnter(float x, float y){
		if (mSpotSlide == null) return;
		int duration = mDuration;
		
		mFilterSlide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,1.0f));
		if (mInverseColor) mSpotSlide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,1.0f));
		{
			SVIAnimationSet spotslideAniSet = new SVIAnimationSet();
			spotslideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			spotslideAniSet.setDuration(duration);
			spotslideAniSet.setOffset(0);
			spotslideAniSet.setRepeatCount(0);
			spotslideAniSet.setAutoReverse(false);
					
			SVIKeyFrameAnimation swipeAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SWIPE_RATIO);
			swipeAni.setDuration(duration);
			swipeAni.setRepeatCount(0);
			swipeAni.setOffset(0);
			swipeAni.setAutoReverse(false);
			swipeAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f, mInverseColor ? 1.0f : 0.0f));
			swipeAni.addKeyProperty(1.0f, new SVIVector3(mMinimumDistance , mMaximumSpreadDistance, mInverseColor ? 1.0f : 0.0f));
			spotslideAniSet.addAnimation(swipeAni);		
			
			SVIKeyFrameAnimation deformAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.DEFORM_RATIO);
			deformAni.setDuration(duration);
			deformAni.setRepeatCount(0);
			deformAni.setOffset(0);
			deformAni.setAutoReverse(false);
			deformAni.addKeyProperty(0.0f, new SVIVector3(x,y, 0.0f));
			deformAni.addKeyProperty(1.0f, new SVIVector3(x,y, 0.0f));
						
			spotslideAniSet.addAnimation(deformAni);			
			mSpotSlide.startAnimation(spotslideAniSet);	
			mSpotSlide.setSwipeRatio(mMinimumDistance, mMaximumSpreadDistance, mInverseColor ? 1.0f : 0.0f);
		}
		
		{
			SVIAnimationSet filterslideAniSet = new SVIAnimationSet();
			filterslideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			filterslideAniSet.setDuration(duration*2);
			filterslideAniSet.setOffset(0);
			filterslideAniSet.setRepeatCount(0);
			filterslideAniSet.setAutoReverse(false);
					
			SVIKeyFrameAnimation deformAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.DEFORM_RATIO);
			deformAni.setDuration(duration*2);
			deformAni.setRepeatCount(0);
			deformAni.setOffset(0);
			deformAni.setAutoReverse(false);
			deformAni.addKeyProperty(0.0f, new SVIVector3(1.0f,1.0f, 0.0f));
			deformAni.addKeyProperty(1.0f, new SVIVector3(mSketchScaleX,mSketchScaleY, 1.0f));
			filterslideAniSet.addAnimation(deformAni);		
			
				
			mFilterSlide.startAnimation(filterslideAniSet);	
		}
	}
	
	public void runExit(float x, float y, float dist){
		if (mSpotSlide == null) return;
		
		int duration = mDuration;
		{
			SVIAnimationSet spotslideAniSet = new SVIAnimationSet();
			spotslideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			spotslideAniSet.setDuration(duration);
			spotslideAniSet.setOffset(0);
			spotslideAniSet.setRepeatCount(0);
			spotslideAniSet.setAutoReverse(false);
					
			SVIKeyFrameAnimation swipeAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SWIPE_RATIO);
			swipeAni.setDuration(duration);
			swipeAni.setRepeatCount(0);
			swipeAni.setOffset(0);
			swipeAni.setAutoReverse(false);
			swipeAni.addKeyProperty(0.0f, new SVIVector3(dist,mMaximumSpreadDistance, mInverseColor ? 1.0f : 0.0f));
			swipeAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f, mInverseColor ? 1.0f : 0.0f));
			spotslideAniSet.addAnimation(swipeAni);		
			
			SVIKeyFrameAnimation deformAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.DEFORM_RATIO);
			deformAni.setDuration(duration);
			deformAni.setRepeatCount(0);
			deformAni.setOffset(0);
			deformAni.setAutoReverse(false);
			deformAni.addKeyProperty(0.0f, new SVIVector3(x,y, 0.0f));
			deformAni.addKeyProperty(1.0f, new SVIVector3(x,y, 0.0f));
			
			spotslideAniSet.addAnimation(deformAni);			
			mSpotSlide.startAnimation(spotslideAniSet);	
			mSpotSlide.setSwipeRatio(0.0f, 0.0f, mInverseColor ? 1.0f : 0.0f);
		}
		
		{
			SVIAnimationSet filterslideAniSet = new SVIAnimationSet();
			filterslideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			filterslideAniSet.setDuration(duration*2);
			filterslideAniSet.setOffset(0);
			filterslideAniSet.setRepeatCount(0);
			filterslideAniSet.setAutoReverse(false);
					
			SVIKeyFrameAnimation deformAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.DEFORM_RATIO);
			deformAni.setDuration(duration*2);
			deformAni.setRepeatCount(0);
			deformAni.setOffset(0);
			deformAni.setAutoReverse(false);
			deformAni.addKeyProperty(0.0f, new SVIVector3(mSketchScaleX,mSketchScaleY, 1.0f));
			deformAni.addKeyProperty(1.0f, new SVIVector3(1.0f,1.0f, 0.0f));
			
			filterslideAniSet.addAnimation(deformAni);			
			mFilterSlide.startAnimation(filterslideAniSet);	
		}
	}
	
	public void runExtend(float x, float y, float dist){
		if (mSpotSlide == null) return;
		int duration = 400;
		{
			SVIAnimationSet spotslideAniSet = new SVIAnimationSet();
			spotslideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			spotslideAniSet.setDuration(duration);
			spotslideAniSet.setOffset(0);
			spotslideAniSet.setRepeatCount(0);
			spotslideAniSet.setAutoReverse(false);
					
			SVIKeyFrameAnimation swipeAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SWIPE_RATIO);
			swipeAni.setDuration(duration);
			swipeAni.setRepeatCount(0);
			swipeAni.setOffset(0);
			swipeAni.setAutoReverse(false);
			swipeAni.addKeyProperty(0.0f, new SVIVector3(dist, mMaximumSpreadDistance, mInverseColor ? 1.0f : 0.0f));
			swipeAni.addKeyProperty(1.0f, new SVIVector3(2.0f, mMaximumSpreadDistance, mInverseColor ? 1.0f : 0.0f));
			spotslideAniSet.addAnimation(swipeAni);		
			
			SVIKeyFrameAnimation deformAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.DEFORM_RATIO);
			deformAni.setDuration(duration);
			deformAni.setRepeatCount(0);
			deformAni.setOffset(0);
			deformAni.setAutoReverse(false);
			deformAni.addKeyProperty(0.0f, new SVIVector3(x,y, 0.0f));
			deformAni.addKeyProperty(1.0f, new SVIVector3(x,y, 0.0f));
			
			spotslideAniSet.addAnimation(deformAni);			
			
			
			if (mInverseColor){
				SVIKeyFrameAnimation colorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
				colorAni.setDuration(duration);
				colorAni.setRepeatCount(0);
				colorAni.setOffset(0);
				colorAni.setAutoReverse(false);
				colorAni.addKeyProperty(0.0f, new SVIVector4(1.0f,1.0f, 1.0f, 1.0f));
				colorAni.addKeyProperty(0.4f, new SVIVector4(1.0f,1.0f, 1.0f, 1.0f));
				colorAni.addKeyProperty(1.0f, new SVIVector4(0.0f,0.0f, 0.0f, 0.0f));
				spotslideAniSet.addAnimation(colorAni);			
			}
			
			mSpotSlide.startAnimation(spotslideAniSet);	
			mSpotSlide.setSwipeRatio(0.0f, 0.0f, mInverseColor ? 1.0f : 0.0f);			
		}
		
		{
			SVIAnimationSet filterslideAniSet = new SVIAnimationSet();
			filterslideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			filterslideAniSet.setDuration(duration);
			filterslideAniSet.setOffset(0);
			filterslideAniSet.setRepeatCount(0);
			filterslideAniSet.setAutoReverse(false);
					
			SVIKeyFrameAnimation colorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			colorAni.setDuration(duration);
			colorAni.setRepeatCount(0);
			colorAni.setOffset(0);
			colorAni.setAutoReverse(false);
			colorAni.addKeyProperty(0.0f, new SVIVector4(1.0f,1.0f, 1.0f, 1.0f));
			colorAni.addKeyProperty(0.4f, new SVIVector4(1.0f,1.0f, 1.0f, 1.0f));
			colorAni.addKeyProperty(1.0f, new SVIVector4(0.0f,0.0f, 0.0f, 0.0f));
			
			filterslideAniSet.addAnimation(colorAni);			
			mFilterSlide.startAnimation(filterslideAniSet);	
		}
	}
	
	
	protected void buildSlideTree() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };				
		SVISlide mainSlide = getMainSlide();		
			
		
		mHomeSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		mHomeSlide.setImage(mCoverImage[0]);		
		
		mFilterSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		mFilterSlide.setImage(mCoverImage[1]);		
		mFilterSlide.setDeformRatio(1.0f,1.0f,0.0f);
		mFilterSlide.setFilterType(SVISlide.FilterType.FILTER_SKETCH);
				
		mSpotSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 0.0f, 0.0f, mWidth, mHeight, color);
		mSpotSlide.setImage(mCoverImage[1]);		
		mSpotSlide.setFilterType(SVISlide.FilterType.FILTER_RADIAL);
		
		mSpotSlide.setDeformRatio(0.0f,0.0f,0.0f);
		mSpotSlide.setSwipeRatio(0.0f, 0.0f, mInverseColor ? 1.0f : 0.0f);		
	}
	
	protected float 			mIntensity = 0.0f;
	protected int				mImageIdx = 1;
	protected SVISlide			mFilterSlide;
	protected SVISlide			mSpotSlide;
	protected SVISlide			mHomeSlide;
	
}
