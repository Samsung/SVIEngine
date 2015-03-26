package com.github.sviengine.unittest;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideDeformDirection;
import com.github.sviengine.slide.SVISlide.SlideDeformType;

import android.content.Context;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class PageConeCurveEffectWindow extends PlatformWindow implements SVIAnimationListener{
	
	public static final String TAG = "ConeCurveEffectWindow";
	
	public PageConeCurveEffectWindow(Context context){
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
		int duration = 1000;
		
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
			keyDeformRatioAni.addKeyProperty(0.0f, new SVIVector3(-2.0f,0.5f,1.0f)); //x ratio, radius, range			 
			keyDeformRatioAni.addKeyProperty(1.0f, new SVIVector3(2.0f,0.5f,1.0f)); //x ratio, radius, range
						
			aniSet.addAnimation(keyDeformRatioAni);					
			
			
			mPageSlide.setDeformRatio(0.0f, 0.5f, 0.0f);
			
			mPageSlide.setOpacity(1.0f);
			mPageSlide.startAnimation(aniSet);			
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
		
		mPageSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
		mainsize.mWidth * 0.05f, mainsize.mHeight * 0.25f,  mainsize.mWidth * 0.9f, mainsize.mHeight * 0.5f, color);
		
		
		mPageSlide.setPivotPoint(0.5f, 0.5f);
		mPageSlide.setImage(mImages[mImages.length-7]);
		mPageSlide.setBackFaceImage(mImages[mImages.length-5]);
		
		mPageSlide.setDeformType(SlideDeformType.DEFORM_CONECURVE);	
		mPageSlide.setDeformDirection(SlideDeformDirection.DEFORM_VERTICAL);
		mPageSlide.setDeformRatio(0.0f,0.5f,0.0f);	//x ratio, radius, range	
		mPageSlide.setOpacity(1.0f);						
		
		mPageSlide.setOrthogonal(false);					
	}
		
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		//SVISlide mainSlide = getMainSlide();				

		if (item.getTitle() == "Reverse Curve"){
			mIsReverse = !mIsReverse;
			int direction = mIsVertical ? 
					SlideDeformDirection.DEFORM_VERTICAL : SlideDeformDirection.DEFORM_HORIZONTAL;
			direction = mIsReverse ? direction + 2 : direction;
			mPageSlide.setDeformDirection(direction);
		}else if (item.getTitle() == "Curve Vertical / Horizon"){
			mIsVertical = !mIsVertical;
			
			int direction = mIsVertical ? 
					SlideDeformDirection.DEFORM_VERTICAL : SlideDeformDirection.DEFORM_HORIZONTAL;
			direction = mIsReverse ? direction + 2 : direction;
			mPageSlide.setDeformDirection(direction);
		}
		
		requestRender();
		return super.onOptionsItemSelected(item);
	}

	boolean mIsReverse = false;
	boolean mIsVertical = false;
	SVISlide mPageSlide = null;
		
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
