package com.github.sviengine.unittest;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideDeformType;

import android.content.Context;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class PageExplodeEffectWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final String TAG = "PageExplodeEffectWindow";
	
	public PageExplodeEffectWindow(Context context){
		super(context);
			
		buildSlideTree();			
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}

	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub	
		
		float x = event.getX();
		float y = event.getY();
		SVISize size = mPageSlide.getRegion().mSize;	
		SVIPoint offset = mPageSlide.getRegion().mOrigin;
		
		x -= offset.mX;			
		y -= offset.mY;
		
		float xRatio = (1.0f - (x/size.mWidth));
		float yRatio = y/size.mHeight;
		
		if (xRatio < 0.0f) xRatio = 0.0f; else if (xRatio > 1.0f) xRatio = 1.0f;
		if (yRatio < 0.0f) yRatio = 0.0f; else if (yRatio > 0.7f) yRatio = 0.7f;
					
		if( event.getAction() == MotionEvent.ACTION_DOWN){
			runAutoTwist();
		}else if( event.getAction() == MotionEvent.ACTION_MOVE){			
			//mPageSlide.setDeformRatio(xRatio, 0.5f, 1.0f);
		}else if( event.getAction() == MotionEvent.ACTION_UP){
			
		}	
		return true;
	}
		
		
	public void runAutoTwist(){
		//int count = 0;
		//int offset = 0;
		int duration = 2000;
		
		//SVISlide mainSlide = getMainSlide();		
		{
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.SINEEASEINOUT);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
				
			SVIKeyFrameAnimation keyDeformRatioAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.DEFORM_RATIO);
			keyDeformRatioAni.setDuration(duration);
			keyDeformRatioAni.setRepeatCount(0);
			keyDeformRatioAni.setAutoReverse(false);
			keyDeformRatioAni.setOffset(0);
			keyDeformRatioAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range	
			keyDeformRatioAni.addKeyProperty(1.0f, new SVIVector3(2.0f,0.0f,0.0f)); //x ratio, radius, range
						
			aniSet.addAnimation(keyDeformRatioAni);					
			
			SVIKeyFrameAnimation keyOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyOpacityAni.setDuration(duration);
			keyOpacityAni.setRepeatCount(0);
			keyOpacityAni.setAutoReverse(false);
			keyOpacityAni.setOffset(0);
			keyOpacityAni.addKeyProperty(0.0f, 0.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(0.1f, 1.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(0.4f, 1.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(0.8f, 0.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(1.0f, 0.0f);
			
			aniSet.addAnimation(keyOpacityAni);			
			
			//mPageSlide.setDeformRatio(0.0f, 0.0f, 0.0f);
			mPageSlide.setOpacity(0.0f);
			mPageSlide.startAnimation(aniSet);			
		}	
		
		{
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.SINEEASEINOUT);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			
			SVIKeyFrameAnimation keyOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyOpacityAni.setDuration(duration);
			keyOpacityAni.setRepeatCount(0);
			keyOpacityAni.setAutoReverse(false);
			keyOpacityAni.setOffset(0);
			keyOpacityAni.addKeyProperty(0.0f, 1.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(0.05f, 0.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(0.95f, 0.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(1.0f, 1.0f);
			
			aniSet.addAnimation(keyOpacityAni);			
			
			mBackSlide.startAnimation(aniSet);					
		}
	
		{
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.SINEEASEINOUT);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			
			SVIKeyFrameAnimation keyOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyOpacityAni.setDuration(duration);
			keyOpacityAni.setRepeatCount(0);
			keyOpacityAni.setAutoReverse(false);
			keyOpacityAni.setOffset(0);
			keyOpacityAni.addKeyProperty(0.0f, 0.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(0.25f, 1.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(0.95f, 1.0f); //x ratio, radius, range
			keyOpacityAni.addKeyProperty(1.0f, 0.0f);
			
			aniSet.addAnimation(keyOpacityAni);			
			
			SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
			keyScaleAni.setDuration(duration);
			keyScaleAni.setRepeatCount(0);
			keyScaleAni.setAutoReverse(false);
			keyScaleAni.setOffset(0);
			keyScaleAni.addKeyProperty(0.0f, new SVIVector3(0.4f,0.4f,0.4f)); //x ratio, radius, range
			keyScaleAni.addKeyProperty(1.0f, new SVIVector3(1.0f,1.0f,1.0f));
			
			
			aniSet.addAnimation(keyScaleAni);			
			
			mFrontSlide.startAnimation(aniSet);					
		}
	}
	
	
	
	protected void buildSlideTree(){
		super.buildSubSlide();
		
		float color[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		//float yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		
		SVISlide mainSlide = getMainSlide();		
		//SVISlide rootSlide = getRootSlide();
		mainSlide.setBackgroundColor(color);
		SVISize mainsize = mainSlide.getRegion().mSize;					
		
		mFrontSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				mainsize.mWidth * 0.1f, mainsize.mHeight * 0.1f,  mainsize.mWidth * 0.8f, mainsize.mHeight * 0.8f, color);
		
		mBackSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				mainsize.mWidth * 0.1f, mainsize.mHeight * 0.1f, mainsize.mWidth * 0.8f, mainsize.mHeight * 0.8f, color);
		
		mPageSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
		mainsize.mWidth * 0.1f, mainsize.mHeight * 0.1f,  mainsize.mWidth * 0.8f, mainsize.mHeight * 0.8f, color);
		
		
				
		mPageSlide.setPivotPoint(0.5f, 0.5f);		
		mBackSlide.setPivotPoint(0.5f, 0.5f);
		mFrontSlide.setPivotPoint(0.5f, 0.5f);
		
		mBackSlide.setImage(mImages[mImages.length-4]);
		mPageSlide.setImage(mImages[mImages.length-4]);
		//mPageSlide.setBackFaceImage(mImages[mImages.length-11]);
		
		mFrontSlide.setImage(mImages[mImages.length-5]);
		
		mPageSlide.setDeformType(SlideDeformType.DEFORM_EXPLODE);	
		mPageSlide.setDeformRatio(0.0f,0.0f,0.0f);	//x ratio, radius, range	
		mPageSlide.setOpacity(1.0f);						
		
		mPageSlide.setOrthogonal(false);			
		mBackSlide.setOrthogonal(false);
		mFrontSlide.setOrthogonal(false);
	}
		
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		//SVISlide mainSlide = getMainSlide();		
		//SVISize mainsize = mainSlide.getRegion().mSize;	
		requestRender();
		return super.onOptionsItemSelected(item);
	}


	SVISlide mPageSlide = null;
	SVISlide mBackSlide = null;
	SVISlide mFrontSlide = null;
		
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
	
	
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private static native void nativeTouchEvent(int type, float x, float y);
	
	
}
